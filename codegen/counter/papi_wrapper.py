from codegen.writer import writer
from codegen.counter.counter import counter

class papi_wrapper(counter):

    def header(self):
        code = writer()
        code.writeln('#include "papi_wrapper/papi_hw_counter.h"')
        code.writeln('')
        code.writeln('FILE *summary_f;')
        code.writeln('struct PAPI_VARS* thr_vars __attribute__((aligned(CACHELINE_SIZE)));')
        code.writeln('int numcpus __attribute__((aligned(CACHELINE_SIZE)));')
        code.writeln('int numiters __attribute__((aligned(CACHELINE_SIZE)));')
        return code.get()

    def init(self,num_proc,num_iter):
        return 'PAPI_HW_COUNTER_init('+str(num_proc)+','+str(num_iter)+');'

    def thread_init(self,tid,num_tasks):
        return 'PAPI_HW_COUNTER_thread_init('+str(tid)+','+str(num_tasks)+');'

    def open(self,tid):
        return 'PAPI_HW_COUNTER_open('+str(tid)+');'

    def start(self,tid,aid):
        return 'PAPI_HW_COUNTER_on('+str(tid)+','+str(aid)+');'

    def stop(self,tid,aid):
        return 'PAPI_HW_COUNTER_off('+str(tid)+','+str(aid)+');'

    def record(self,name,tid,aid,ivar):
        return 'PAPI_HW_COUNTER_record("'+name+'",'+str(tid)+','+str(aid)+','+ivar+');'

    def close(self,tid):
        return 'PAPI_HW_COUNTER_close('+str(tid)+');'

    def write(self,tid):
        return 'PAPI_HW_COUNTER_write('+str(tid)+');'
    
    def write_summary(self):
        return 'PAPI_HW_COUNTER_write_summary();'
