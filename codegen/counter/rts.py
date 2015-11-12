from codegen.writer import writer
from codegen.counter.counter import counter

class rts(counter):

   def header(self):
      code = writer()
      code.writeln('#include "rts/rts.h"')
      code.writeln('')
      code.writeln('struct RTS_VARS* thr_vars __attribute__((aligned(CACHELINE_SIZE)));')
      code.writeln('int numcpus __attribute__((aligned(CACHELINE_SIZE)));')
      code.writeln('int numiters __attribute__((aligned(CACHELINE_SIZE)));')
      return code.get()

   def init(self, num_proc, num_iter):
      return 'RTS_init('+str(num_proc)+','+str(num_iter)+');'

   def thread_init(self,tid,num_tasks):
      return 'RTS_thread_init('+str(tid)+','+str(num_tasks)+');'

   def start(self, tid, aid):
      return 'RTS_start_timer('+str(tid)+','+str(aid)+');'
   
   def stop(self, tid, aid):
      return 'RTS_stop_timer('+str(tid)+','+str(aid)+');'

   def record(self,name,tid,aid,ivar):
      return 'RTS_record("'+name+'",'+str(tid)+','+str(aid)+','+ivar+');'

   def write(self,tid):
      return 'RTS_write_time('+str(tid)+');'
