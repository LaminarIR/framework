import sys
import re
from codegen.writer import writer
from codegen.generator import generator
from codegen.profiler.profiler import profiler
from codegen.profiler.actor_profiler import actor_profiler

class fifo_generator (generator):
   """ Generate sequence of actor firings employing FIFO queues for a given schedule
   """

   def get_bufsize(self):
      """ Calculate maximum number of tokens on each edges for a steady-state iteration.
      """
      sdfgraph = self.sdfgraph
      num_processors = self.scheduler.get_numprocessors()

      # initialize variables
      fillstate = {}
      maxfillstate = {}
      for edge in sdfgraph.edges():
         fillstate[edge] = sdfgraph.delay[edge]
         maxfillstate[edge] = sdfgraph.delay[edge]

      # init schedule
      init_schedule = self.scheduler.get_bootschedule()
      for actor in init_schedule:
         for pred in sdfgraph.in_nodes(actor):    # incoming nodes
             edge = (pred,actor)
             if fillstate[edge] <= 0:
                print "ERROR: Wrong attempt to consume a token from an empty edge ("+pred+","+actor+")."
                sys.exit(1)
         for succ in sdfgraph.out_nodes(actor):    # outgoing nodes
            edge = (actor,succ)
            fillstate[edge] = fillstate[edge]+sdfgraph.production[edge]
            if maxfillstate[edge] < fillstate[edge]:
               maxfillstate[edge] = fillstate[edge]

      # steady schedule
      steady_schedule = self.scheduler.get_schedule()
      for proc in range(0, num_processors):
         for actor in steady_schedule[proc]:
            for pred in sdfgraph.in_nodes(actor):    # incoming nodes
               edge = (pred,actor)
               if fillstate[edge] <= 0:
                  print "ERROR: Wrong attempt to consume a token from an empty edge ("+node+","+actor+")."
                  sys.exit(1)
            for succ in sdfgraph.out_nodes(actor):    # outgoing nodes
               edge = (actor,succ)
               fillstate[edge] = fillstate[edge]+sdfgraph.production[edge]
               if maxfillstate[edge] < fillstate[edge]:
                  maxfillstate[edge] = fillstate[edge]
      return maxfillstate

   def gen_fifo_functions(self):
      """ Push and pop code generator for fifo buffering.
      """
      sdfgraph = self.sdfgraph
      bufsize = self.get_bufsize()
      pfn = writer()

      # generate reset code
      reset_code = writer();
      reset_code.writeln('static inline void __RESET_PTR(){')
      reset_code.indent()

      # iterator for delay copy
      reset_code.writeln('int i = 0;')

      # generate push and pop functions and variables
      for edge in sdfgraph.edges():
         (source,target) = edge
         buffer_id='buf_'+source+'_'+target
         token_type=sdfgraph.token_type[edge]
         num_delay=sdfgraph.delay[edge]
         pfn.writeln('static '+token_type+' '+buffer_id+'['+str(bufsize[edge])+'] = {0};')
         pfn.writeln('static int head_'+source+'_'+target+'=0;')
         pfn.writeln('static int tail_'+source+'_'+target+'=0;')
         pfn.writeln('static inline void push_'+source+'_'+target+'('+token_type+' x){')
         pfn.indent()
         pfn.writeln('buf_'+source+'_'+target+'[tail_'+source+'_'+target+'++]=x;')
         reset_code.writeln('tail_'+source+'_'+target+'='+str(num_delay)+';')
         if num_delay > 0:
            reset_code.writeln('for ( i = 0; i < '+str(num_delay)+'; i++){')
            reset_code.indent()
            reset_code.writeln(buffer_id+'[i]='+buffer_id+'['+str(bufsize[edge]-num_delay)+'+i];')
            reset_code.unindent()
            reset_code.writeln('}')
         reset_code.writeln('')
         pfn.unindent()
         pfn.writeln('}')

         pfn.writeln('static inline ' + token_type+' pop_'+source+'_'+target+'(){')
         pfn.indent()
         pfn.writeln(token_type+' x;');
         pfn.writeln('x=buf_'+source+'_'+target+'[head_'+source+'_'+target+'++];')
         reset_code.writeln('head_'+source+'_'+target+'=0;')
         pfn.writeln('return x;')
         pfn.unindent()
         pfn.writeln('}')

         pfn.writeln('static inline ' + token_type+' peek_'+source+'_'+target+'(int _idx){')
         pfn.indent()
         pfn.writeln(token_type+' x;');
         pfn.writeln('x=buf_'+source+'_'+target+'[head_'+source+'_'+target+'+_idx];')
         pfn.writeln('return x;')
         pfn.unindent()
         pfn.writeln('}')
         pfn.writeln('')
      reset_code.writeln('return;')
      reset_code.unindent()
      reset_code.writeln('}')
      return pfn.get() + reset_code.get()

   def gen_actors(self):
      """ Generate codes for actor functions with fifo buffers.
      """
      sdfgraph = self.sdfgraph
      code = writer()
      for actor in sdfgraph.nodes():
         code.write('inline static void actor_'+actor+'(')
         code.indent()
         first = True
         in_queue=[]
         out_queue=[]
         for pred in sdfgraph.in_nodes(actor):    # incoming nodes
            edge = (pred,actor)
            for i in range(0,len(sdfgraph.target_tokens[edge])):
               in_queue.append(sdfgraph.target_tokens[edge][i])
         for succ in sdfgraph.out_nodes(actor):   # outgoing nodes
            edge = (actor,succ)
            for i in range(0,len(sdfgraph.source_tokens[edge])):
               out_queue.append(sdfgraph.source_tokens[edge][i])
         code.unindent()
         code.writeln('){')
         code.indent()
         # TBD: add #line directive to each line so that C-Compiler can track source code of sdf program!
         if sdfgraph.is_generic(actor):  
            if len(sdfgraph.in_nodes(actor)) > 1:
               print "ERROR: More than 1 incoming node is found: "+actor+"."
               sys.exit(1)
            if len(sdfgraph.out_nodes(actor)) > 1:
               print "ERROR: More than 1 outgoing node is found: "+actor+"."
               sys.exit(1)

            pop_fn  = 'pop'
            push_fn = 'push'
            peek_fn = 'peek'
            if len(sdfgraph.in_nodes(actor)) == 0 and len(sdfgraph.out_nodes(actor)) == 1: # Source
               push_fn = 'push_'+actor+'_'+sdfgraph.out_nodes(actor)[0]
            
            elif len(sdfgraph.in_nodes(actor)) == 1 and len(sdfgraph.out_nodes(actor)) == 0: # Sink
               pop_fn  = 'pop_'+sdfgraph.in_nodes(actor)[0]+'_'+actor
               peek_fn = 'peek_'+sdfgraph.in_nodes(actor)[0]+'_'+actor
            else:
               pop_fn  = 'pop_'+sdfgraph.in_nodes(actor)[0]+'_'+actor
               push_fn = 'push_'+actor+'_'+sdfgraph.out_nodes(actor)[0]
               peek_fn = 'peek_'+sdfgraph.in_nodes(actor)[0]+'_'+actor
           
            actor_code = sdfgraph.actor_code[actor]
            param_list = sdfgraph.node_param[actor]
            for i in range(0, len(param_list)):
               (p_type, p_name, p_value) = param_list[i]
               actor_code = actor_code.replace(p_name, str(p_value))
               actor_code = re.sub(r"(?=[^0-9A-Za-z_])"+p_name+"(?=[^0-9A-Za-z_])",str(p_value),actor_code)
            actor_code = re.sub(r"(?<=[^0-9A-Za-z_])pop(?=[^0-9A-Za-z_])",pop_fn,actor_code)
            actor_code = re.sub(r"(?<=[^0-9A-Za-z_])push(?=[^0-9A-Za-z_])",push_fn,actor_code)
            actor_code = re.sub(r"(?<=[^0-9A-Za-z_])peek(?=[^0-9A-Za-z_])",peek_fn,actor_code)
            code.writeln(actor_code)

         else:
            # round robin
            if len(in_queue) == len(out_queue):
               for i in range(0,len(in_queue)):
                  tokens=out_queue[i].split('_')
                  out_postfix=tokens[1]
                  for j in range(2, len(tokens)-1):
                     out_postfix=out_postfix+'_'+tokens[j]
                  tokens=in_queue[i].split('_')
                  in_postfix=tokens[1]
                  for j in range(2, len(tokens)-1):
                     in_postfix=in_postfix+'_'+tokens[j]
                  code.writeln('push_'+out_postfix+'(pop_'+in_postfix+'());')
            # duplicate
            elif len(in_queue) < len(out_queue) and \
              len(out_queue)%len(in_queue) == 0:
               i = 0
               for o in range(0,len(out_queue)):
                  tokens=out_queue[o].split('_')
                  out_postfix=tokens[1]
                  for j in range(2, len(tokens)-1):
                     out_postfix=out_postfix+'_'+tokens[j]
                  tokens=in_queue[i].split('_')
                  in_postfix=tokens[1]
                  for j in range(2, len(tokens)-1):
                     in_postfix=in_postfix+'_'+tokens[j]
                  code.writeln('push_'+out_postfix+'(peek_'+in_postfix+'('+str(i)+'));')
                  i = i + 1
                  i = i % len(in_queue)
               for i in range(0, len(in_queue)):
                  code.writeln('pop_'+in_postfix+'();')
            # invalid (inconsistent) sdf graph
            else:
                print "ERROR: Graph is inconsistent."
                print "       "+actor+" with "+str(len(in_queue))+" in-tokens and "+str(len(out_queue))+" out-tokens."
                sys.exit(1)
         code.unindent()
         code.writeln('}')
         code.writeln('')
      return code.get()


   def gen_sequence(self, schedule, tid, LocalVars=True, ivar='i'):
      """ Generate code to invoke actors in sequence of a given schedule.
          :param schedule: A sequance of actors as a schedule
          :param procId: Index of the processor to store the execution \
          time. If the value is negative, then the functions calls for \
          execution time measurement are omitted \
          (e.g., sequence for booting).
          :param ivar: loop variable for a steady state iteration
      """
      sdfgraph = self.sdfgraph
      if tid < 0:
          prof = profiler()
      else:
          prof = self.profiler
      code = writer()
      code.indent()
      code.indent()
      for seq in range(0,len(schedule)):
         actor = schedule[seq]
         code.writeln('{')
         code.indent()
         code.write(prof.gen_actor_profile_start(tid,seq))
         varctr = 1
         for pred in sdfgraph.in_nodes(actor):    # incoming nodes
            edge = (pred,actor)
            for i in range(0,sdfgraph.consumption[edge]):
               var = sdfgraph.token_type[edge]+' x'+str(varctr)
               fn  = 'pop_' + pred +'_' + actor + '()'
               varctr = varctr+1
         varctr = 1
         for succ in sdfgraph.out_nodes(actor):   # outgoing nodes
            edge = (actor,succ)
            for i in range(0,sdfgraph.production[edge]):
               var    = 'y'+str(varctr)
               varctr = varctr+1
         actor_function = 'actor_' + actor
         code.write(actor_function+ '(')
         code.indent()
         first = True
         varctr = 1
         for pred in sdfgraph.in_nodes(actor):    # incoming nodes
            edge = (pred,actor)
            for i in range(0,sdfgraph.consumption[edge]):
               var = 'x'+str(varctr)
               varctr = varctr+1
         varctr = 1
         for succ in sdfgraph.out_nodes(actor):   # outgoing nodes
            edge = (actor,succ)
            for i in range(0,sdfgraph.production[edge]):
               var    = 'y'+str(varctr)
               varctr = varctr+1
         code.unindent()
         code.writeln(');')
         code.unindent()
         code.writeln('}')
         code.writeln('')
         code.write(prof.gen_actor_profile_stop(tid,seq))
         code.write(prof.gen_actor_profile_record(actor,0,seq,ivar))
      return code.get()
