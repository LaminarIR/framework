# define abstract code generation class
# using strategy pattern
#
from codegen.writer import writer
from codegen.profiler.profiler import profiler
from codegen.prefetcher.prefetcher import prefetcher

class generator:

   token_prefix='laminar_x'

   def __init__(self, sdfgraph, scheduler, profiler, prefetcher, iterations=0):
      """ initialize generator class
          :param scheduler: scheduler object
          :param profiler: profiler object
          :param iterations: number of steady-state iterations to execute
      """
      self.sdfgraph = sdfgraph
      self.scheduler = scheduler
      self.profiler = profiler
      profiler.set_generator(self)
      self.prefetcher = prefetcher
      self.iterations = iterations
       
   def gen_init(self):
      """ Generate init codes
      """
      sdfgraph = self.sdfgraph
      code = writer()
      code.indent()
      code.writeln(self.profiler.gen_profile_init(self.scheduler.get_numprocessors(), self.iterations))
      code.unindent()
      code.writeln('// BEGIN: init functions of actors')
      code.indent()
      for actor in sdfgraph.nodes():
         if sdfgraph.is_generic(actor) and sdfgraph.init[actor] <> '':
            code.writeln('{')
            code.indent()
            init_code = sdfgraph.init[actor]
            param_list = sdfgraph.node_param[actor]
            for i in range(0, len(param_list)):
               (p_type, p_name, p_value) = param_list[i]
               init_code = init_code.replace(p_name, str(p_value))
            code.writeln(init_code)
            code.unindent()
            code.writeln('}')
      code.unindent()
      code.writeln('// END: init functions of actors')
      return code.get()

   def gen_close(self):
      return self.profiler.gen_profile_write_summary()

   def gen_state(self, schedule):
      """ Generate list of state variables accessed by the given schedule.
          :param schedule: A sequance of actors as a schedule
      """
      sdfgraph = self.sdfgraph
      num_processor=self.scheduler.get_numprocessors()

      state = writer()
      state_decls = {}

      state.writeln('// BEGIN: state variables for actors')
      for seq in range(0,len(schedule)):
         actor = schedule[seq]
         if sdfgraph.is_generic(actor):
            # add states to decl
            if actor not in state_decls:
               state_decls[actor]=writer()
               state_decls[actor].write(sdfgraph.state[actor])
      for actor in state_decls:
         if state_decls[actor].get() != "":
            state.write(state_decls[actor].get())
      state.writeln('\n// END: state variables for actors')

      return state.get()

   def gen(self):
       """ generate c code
       """
       sdfgraph = self.sdfgraph

       (global_code, actor_exec_code) = self.gen_schedule()

       # write preamble
       code = sdfgraph.preamble

       # write global codes
       code = code + self.profiler.gen_header()
       code = code + '\n'
       code = code + global_code

       # write codes of periodic schedule
       code = code + actor_exec_code

       # write postamble
       code = code + sdfgraph.postamble

       return code
