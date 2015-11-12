import sys
import re
from copy import deepcopy
from codegen.writer import writer
from codegen.generator import generator
from codegen.profiler.profiler import profiler
from codegen.prefetcher.prefetcher import prefetcher

class direct_generator (generator):
   """ Generate sequence of actor firings employing direct access for a given schedule
   """

   def gen_sequence(self, schedule, tid, ivar='i'):
      """ Generate the code for executing the actor invocations in sequence given a schedule.
          :param schedule: A sequance of actors 
          :param tid: Index of the processor to execute the sequence on. \
          The value is negative if the given schedule is for booting.
          :param ivar: loop variable for a steady state iteration
      """
      sdfgraph = self.sdfgraph
      num_processors = self.scheduler.get_numprocessors()
      prefetcher = self.prefetcher

      # for booting phase
      booting_codes={}
      cur_proc=-1
      prev_proc=-1

      if self.global_tokens == [] and self.local_tokens == {}:
         print 'ERROR: Tokens are not generated to run gen_sequence.'
         sys.exit(1)

      # when tid is negative, gen sequence for booting phase
      if tid < 0: 
         prof=profiler()
         for proc in range(0, num_processors):
            booting_codes[proc]=writer()
            booting_codes[proc].indent()
            booting_codes[proc].writeln('')
      # when tid is valid, gen sequence for steady state phase
      else:
         prof=self.profiler

      code = writer()
      code.indent()
      code.indent()
      for seq in range(0,len(schedule)):
         actor = schedule[seq]

         # code for prefetch
         prefetch_code = writer()
         if tid < 0:
            code=writer()
            cur_proc=sdfgraph.allocation[actor]

         # generate firing codes for actor
         if sdfgraph.is_generic(actor):
            code.write(prof.gen_actor_profile_start(tid,seq))

            actor_function = 'actor_'+actor
            param_list = sdfgraph.node_param[actor]
            actor_code = sdfgraph.actor_code[actor]

            first = True
            code.indent()
            code.write('\n//'+actor_function+'_'+str(seq))
            code.write('\n{')

            for i in range(0, len(param_list)):     # input params
               (p_type, p_name, p_value) = param_list[i]
               actor_code = actor_code.replace(p_name, str(p_value))

            for pred in sdfgraph.in_nodes(actor):    # incoming nodes
               edge = (pred,actor)
               for i in range(0,sdfgraph.consumption[edge]):
                  param = sdfgraph.target_tokens[edge][i]
                  var = self.fifo[edge][0]
                  actor_code = re.sub(r"@"+param+"(?=[^0-9A-Za-z_])", var, actor_code)
                  self.fifo[edge].pop(0)
                  prefetch_code.write(prefetcher.read_prefetch(var, \
                              sdfgraph.allocation[pred], sdfgraph.allocation[actor]))

               for i in range(0,len(sdfgraph.peek_tokens[edge])):
                  param = sdfgraph.peek_tokens[edge][i]
                  var = self.fifo[edge][i]
                  actor_code = re.sub(r"@"+param+"(?=[^0-9A-Za-z_])", var, actor_code)
                  prefetch_code.write(prefetcher.read_prefetch(var, \
                              sdfgraph.allocation[pred], sdfgraph.allocation[actor]))

            for succ in sdfgraph.out_nodes(actor):   # outgoing nodes
               edge = (actor,succ)
               for i in range(0,sdfgraph.production[edge]):
                  param=sdfgraph.source_tokens[edge][i]
                  # find if the var is local or not
                  var = self.token_prefix+str(self.varctr)
                  self.fifo[edge].append(var)
                  self.varctr = self.varctr+1
                  actor_code = re.sub(r"@"+param+"(?=[^0-9A-Za-z_])", var, actor_code)
                  prefetch_code.write(prefetcher.write_prefetch(var, \
                              sdfgraph.allocation[succ], sdfgraph.allocation[actor]))
             
            new_code=prefetch_code.get()+code.get()
            code=writer()
            code.writeln(new_code)
            code.write(actor_code)
            code.write('}')
            code.unindent()
            code.write(prof.gen_actor_profile_stop(tid,seq))
            code.writeln(prof.gen_actor_profile_record(actor,0,seq,ivar))
         else:
            in_rate = 0
            out_rate = 0
            multiplicity = 0
            for pre in sdfgraph.in_nodes(actor):
               edge = (pre, actor)
               in_rate = in_rate + sdfgraph.consumption[edge]
            for post in sdfgraph.out_nodes(actor):
               edge = (actor, post)
               out_rate = out_rate + sdfgraph.production[edge]
            # round robin
            if in_rate == out_rate:
               multiplicity = 1
            # duplicate
            elif in_rate < out_rate and out_rate % in_rate == 0:
               multiplicity = out_rate/in_rate
            # invalid (inconsistent) sdf graph
            else:
               print "ERROR: Graph is inconsistent."
               print "       "+actor+" with "+str(in_rate)+" in-tokens and "+str(out_rate)+" out-tokens."
               sys.exit(1)
            queue = []
            for mult in range(0, multiplicity):
               for pred in sdfgraph.in_nodes(actor):   # incoming nodes
                  edge = (pred,actor)
                  for i in range(0,sdfgraph.consumption[edge]):
                     queue.append(self.fifo[edge][0])
                     if mult == multiplicity-1:
                        self.fifo[edge].pop(0)
            for succ in sdfgraph.out_nodes(actor):  # outgoing nodes
               edge = (actor,succ)
               for i in range(0,sdfgraph.production[edge]):
                  var = queue.pop(0)
                  self.fifo[edge].append(var)
         # for boot schedule
         if tid < 0: 
            booting_codes[cur_proc].writeln(code.get())
            #if not cur_proc == prev_proc or seq == len(schedule)-1:
            if num_processors > 1:
               for proc in range (0, num_processors):
                  booting_codes[proc].writeln('rts_sync('+str(proc)+');')
                  prev_proc=cur_proc

      if tid < 0:
         return booting_codes
      else:
         return code.get()

   def gen_tokens(self, boot_schedule, multi_schedule):
      """ Generate tokens for intra-/inter-communication
          :param boot_schedule: A sequance of actors for booting
          :param multi_schedule: one or more sequence(s) of actors for a steady-state schedule
      """
      sdfgraph = self.sdfgraph
      max_rep=self.scheduler.get_max_lev_diff()

      fifo={}
      declared={}
      varctr=1
      token_list={}   # (token type, source proc idx, target proc idx)

      # initialize output
      self.local_tokens = {}
      for proc in range(0,self.scheduler.get_numprocessors()):
         self.local_tokens[proc]=[]
      self.global_tokens = []

      # adjust max_rep to the total number of iteration
      while True:
         if self.iterations%max_rep == 0:
            break
         max_rep=max_rep+1

      def simulate_tokenflow(schedule, fifo, varctr, token_list):
         """ Simulate token flow to link tokens with indirections
             :param fifo: backed up state of edges
             :param varctr:
             :param token_list: backed up pool of tokens
         """
         for seq in range(0,len(schedule)):
            actor = schedule[seq]
            if sdfgraph.is_generic(actor):
               for pred in sdfgraph.in_nodes(actor):    # incoming nodes
                  edge = (pred,actor)
                  for i in range(0,sdfgraph.consumption[edge]):
                     var=fifo[edge].pop(0)
                     (type, source_proc, target_proc)=token_list[var][0]
                     # the token is consumed by this actor. update the target proc idx.
                     new_token=(type,source_proc,sdfgraph.allocation[actor])
                     if len(token_list[var]) == 1 and target_proc == -1: # ??
                        token_list[var][0]=new_token
                     else:
                        token_list[var].append(new_token)


               for succ in sdfgraph.out_nodes(actor):   # outgoing nodes
                  edge = (actor,succ)
                  for i in range(0,sdfgraph.production[edge]):
                     var = self.token_prefix+str(varctr)
                     fifo[edge].append(var)
                     if not var in token_list:
                        token_list[var]=[]

                     token_list[var].append(
                       (sdfgraph.token_type[edge],sdfgraph.allocation[actor],-1))
                     
                     varctr = varctr+1
            else:
               in_rate = 0
               out_rate = 0
               multiplicity = 0
               for pre in sdfgraph.in_nodes(actor):
                  edge = (pre, actor)
                  in_rate = in_rate + sdfgraph.consumption[edge]
               for post in sdfgraph.out_nodes(actor):
                  edge = (actor, post)
                  out_rate = out_rate + sdfgraph.production[edge]
               # round robin
               if in_rate == out_rate:
                  multiplicity = 1
               # duplicate
               elif in_rate < out_rate and out_rate % in_rate == 0:
                  multiplicity = out_rate/in_rate
               # invalid (inconsistent) sdf graph
               else:
                  print "ERROR: Graph is inconsistent."
                  print "       "+actor+" with "+str(in_rate)+" in-tokens and "+str(out_rate)+" out-tokens."
                  sys.exit(1)

               queue = []
               for mult in range(0, multiplicity):
                  for pred in sdfgraph.in_nodes(actor):   # incoming nodes
                     edge = (pred,actor)
                     for i in range(0,sdfgraph.consumption[edge]):
                        queue.append(fifo[edge][0])
                        if mult == multiplicity-1:
                           fifo[edge].pop(0)
               for succ in sdfgraph.out_nodes(actor):  # outgoing nodes
                  edge = (actor,succ)
                  for i in range(0,sdfgraph.production[edge]):
                     var = queue.pop(0)
                     fifo[edge].append(var)
         return (fifo, varctr, token_list)

      # add delay tokens
      for edge in sdfgraph.edges():
         (source, target) = edge
         fifo[edge]=[]
         for i in range(0, sdfgraph.delay[edge]):
            var = self.token_prefix+str(varctr)
            varctr=varctr+1
            fifo[edge].append(var)
            if not var in token_list:
               token_list[var]=[]
            token_list[var].append((sdfgraph.token_type[edge],sdfgraph.allocation[source],-1))
      
      # simulate booting schedule
      (fifo,varctr,token_list)=simulate_tokenflow(boot_schedule,fifo,varctr,token_list)

      fifo_snapshot=deepcopy(fifo)

      # simulate steady schedule
#      for proc in range(0, len(multi_schedule)):
#          (fifo,varctr,token_list)=simulate_tokenflow(multi_schedule[proc],fifo,varctr,token_list)
#      if len(multi_schedule) > 1: 
      for rep in range(0, max_rep):
         for proc in range(0, len(multi_schedule)):
            (fifo,varctr,token_list)=\
            simulate_tokenflow(multi_schedule[proc],fifo,varctr,token_list)

      if len(multi_schedule) == 1:
          # all tokens are local for seqential execution
          for var in token_list:
             (type, source_proc, target_proc) = token_list[var][0]
             self.local_tokens[source_proc].append((type,var))
      else:
          for var in token_list:
             DECLARED=False
             # tokens active after booting and a steady state iteration are to be global
             for edge in fifo_snapshot:
                if var in fifo_snapshot[edge] or var in fifo[edge]:
                   self.global_tokens.append((sdfgraph.token_type[edge],var))
                   DECLARED=True
                   break;
             if DECLARED:
                continue
             for idx in range(0,len(token_list[var])):
                (type, source_proc, target_proc) = token_list[var][idx]
                if source_proc != target_proc:
                   self.global_tokens.append((type,var))
                   break
                elif source_proc == target_proc and idx == len(token_list[var])-1:
                   self.local_tokens[source_proc].append((type,var))

      # sort tokens
      self.global_tokens = sorted(self.global_tokens,key=lambda x: map(int,re.findall(r'[0-9]+',x[1])))
      for proc in range(0, len(multi_schedule)):
         self.local_tokens[proc] = sorted(self.local_tokens[proc],key=lambda x: map(int,re.findall(r'[0-9]+',x[1])))

      return (self.local_tokens,self.global_tokens)
