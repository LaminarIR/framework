import sys
from copy import deepcopy
from codegen.writer import writer
from codegen.direct_generator import direct_generator

class serial_direct_generator(direct_generator):

   def gen_schedule(self):
      """Generate C code based on the SDF graph without buffers for
         communication. Communications in between two nodes are done by
         accessing variables assigned on each tokens directly. Thus no
         additional buffers are required for communication.
      """
      sdfgraph = self.sdfgraph
      init_schedule = self.scheduler.get_bootschedule()
      schedule = self.scheduler.get_schedule()
      max_rep = self.scheduler.get_max_lev_diff()

      # adjust max_rep to the total number of iteration
      if self.iterations < max_rep:
         print 'WARNING: Number of iteration given,',self.iterations,'is not sufficient to run a steady-state iteration. Adjust the number of iteration automatically to', max_rep
         self.iterations=max_rep
      iterations = self.iterations

      while True:
         if iterations%max_rep == 0:
            break
         max_rep=max_rep+1

      prof = self.profiler
      self.fifo = {}
      self.varctr = 1

      code = writer()
      code.indent()
      decl = writer()
      decl.indent()
      global_block=writer()

      # generate tokens varctr must be not changed
      (local_tokens, global_tokens) = self.gen_tokens(init_schedule, schedule)


      ##############################################################
      # write global token declarations
      for idx in range(0, len(global_tokens)): 
         (type,var)=global_tokens[idx]
         global_block.writeln('\nstatic '+type+' '+var+';')


      ##############################################################
      # write local token declarations
      for idx in range(0, len(local_tokens[0])):
         (type,var)=local_tokens[0][idx]
         code.writeln(type+' '+var+';')

      # push delay tokens to fifo queue
      for edge in sdfgraph.edges():
         (source,target) = edge
         self.fifo[edge] = []
         for i in range(0,sdfgraph.delay[edge]):
            var = self.token_prefix+str(self.varctr)
            code.writeln(var+' = '+ sdfgraph.delay_tokens[edge][i]+";")
            self.varctr = self.varctr+1
            self.fifo[edge].append(var)

      # prepare for profiling if needed
      code.writeln(prof.gen_profile_thread_init(0,len(schedule[0])))
      code.writeln(prof.gen_profile_open(0))

      # start - program profiling
      code.writeln(prof.gen_program_profile_start())
      
      # write booting code if needed
      code.writeln('\n'+self.gen_sequence(init_schedule, -1)[0].get())

      # generate code for processes
      fifo_snapshot = deepcopy(self.fifo)

      # loop statement for steady state iteation
      code.writeln('')
      if iterations > 0:
         decl.writeln("int i;")
         code.writeln('for(i = 0; i <'+str(iterations/max_rep)+'; i++){')
      else:
         code.writeln('for(;;){')
      code.indent()
      
      code.write(prof.gen_thread_profile_start(0))

      for rep in range(0, max_rep):
         iter=str(max_rep)+'*i+'+str(rep)
         # generate code for the steady state iteration based on schedule
         code.writeln('\n'+ self.gen_sequence(schedule[0], 0, ivar=iter))

      # preparation for the next iteration
      # generate code for copying variables for the first iteration and close functions def
      for edge in sdfgraph.edges():
         (source,target) = edge
         for i in range(0,len(self.fifo[edge])):
            src_var=fifo_snapshot[edge][i]
            dst_var=self.fifo[edge][i]
            code.writeln(src_var + '=' + dst_var + ';')

      code.unindent()
      code.writeln(prof.gen_thread_profile_stop(0))
      code.writeln(prof.gen_thread_profile_record(0,'i'))
      code.writeln('}')
    
      # stop - program profiling
      code.writeln(prof.gen_program_profile_stop())

      # record 
      code.writeln(prof.gen_program_profile_record())

      # write profiled data
      code.writeln(prof.gen_profile_write(0))
      # finish profiling 
      code.writeln(prof.gen_profile_close(0))


      ##############################################################
      # wrap up as a block
      block = writer()
      block.writeln('')
      if sdfgraph.name == '':
         name = 'execute'
      else:
         name = sdfgraph.name
      block.writeln('static inline void '+name+'(){')
      # generate state declarations
      block.writeln(self.gen_state(schedule[0]))
      block.writeln(decl.get())
      block.writeln(self.gen_init())
      block.write(code.get())
      block.writeln('\t'+self.gen_close())
      block.writeln('}')
      block.writeln('')

      return (global_block.get(), block.get())
