import sys
from codegen.writer import writer
from codegen.fifo_generator import fifo_generator

class serial_fifo_generator (fifo_generator):

   def gen_schedule(self):
      """Generate C code based on the SDF graph with buffers for
         communication. Communications in between two nodes are done by
         pushing/popping tokens to/from a buffer which is assigned on
         each edge of the SDF graph.
      """
      sdfgraph = self.sdfgraph
      schedule = self.scheduler.get_schedule()
      prof     = self.profiler

      code = writer()
      global_block = writer()
     

      ##############################################################
      # generate global functions
      global_block.writeln(self.gen_state(schedule[0]))
		# generate fifo related functions
      global_block.writeln(self.gen_fifo_functions())
      # generate actor codes
      global_block.writeln(self.gen_actors())


      ##############################################################
      # push delay tokens
      code.indent()
      for edge in sdfgraph.edges():
         (source,target) = edge
         for i in range(0,sdfgraph.delay[edge]):
            code.write('push_'+source+'_'+target+'(')
            code.writeln( sdfgraph.delay_tokens[edge][i] + ');')

      # prepare for profiling if needed
      code.writeln(prof.gen_profile_thread_init(0,len(schedule[0])))
      code.writeln(prof.gen_profile_open(0))

      # start - program profiling
      code.writeln(prof.gen_program_profile_start())

      # loop statement for steady state iteration
      if self.iterations > 0:
         code.writeln("int i;");
         code.writeln('for(i = 0; i < '+str(self.iterations)+'; i++){')
      else:
         code.writeln('for(;;){')
      code.indent()

      code.writeln(prof.gen_thread_profile_start(0))
      # generate code for the steady state iteration based on schedule
      code.writeln(self.gen_sequence(schedule[0], 0, LocalVars=False))

      # preparation for the next iteration
      # update pointers (function declaration in gen_fifo_functions())
      code.writeln('__RESET_PTR();')
      code.writeln(prof.gen_thread_profile_stop(0))
      code.writeln(prof.gen_thread_profile_record(0,'i'))
      code.unindent()
      code.writeln('}')

      # stop - program profiling
      code.writeln(prof.gen_program_profile_stop())
     
      code.writeln(prof.gen_program_profile_record())
      # 
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
      block.writeln(self.gen_init())
      block.write(code.get())
      block.writeln('\t'+self.gen_close())
      block.writeln('}')
      block.writeln('')

      return (global_block.get(), block.get())
