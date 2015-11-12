#
# Profiler for profiling actors. The generated code measures
# the runtime of each actor firing.
#

from codegen.writer import writer
from codegen.profiler.profiler import profiler

class actor_profiler(profiler):
   def gen_profile_init(self,num_proc,num_iter):
      return self.counter.init(num_proc,num_iter)

   def gen_profile_thread_init(self,tid,num_tasks):
      return self.counter.thread_init(tid,num_tasks)

   def gen_profile_open(self,tid):
      return self.counter.open(tid)

   def gen_actor_profile_start(self,tid,aid):
      return self.counter.start(tid,aid)

   def gen_actor_profile_stop(self,tid,aid):
      return self.counter.stop(tid,aid)

   def gen_actor_profile_record(self,name,tid,aid,ivar):
      return self.counter.record(name,tid,aid,ivar)

   def gen_profile_close(self,tid):
      return self.counter.close(tid)

   def gen_profile_write(self,tid):
      return self.counter.write(tid)

   def gen_profile_write_summary(self):
      return self.counter.write_summary()
