#
# abstract profiler  class that is used by the code generator to
# generate code for profiling a stream program. The interface permits
# to generate code for measurement variables, a function that writes
# the information to a file and start & stop timer codes for measuring
# an actor firing and the workload of a thread whilst executing a
# single steady state iteration.
#

from codegen import writer
from codegen.counter.counter import counter

class profiler:

    def __init__(self, counter=counter()):
        """ initialize profiler with a specific counter
            :param counter: performance counter
        """
        self.counter=counter

    #
    # Constructor
    #
    def set_generator(self,generator):
        self.generator = generator

    #
    # generate header
    # 
    def gen_header(self):
        return self.counter.header()

    #
    # generate init code for profiler
    #
    def gen_profile_init(self,num_proc,num_iter):
        return ''

    #
    # generate thread init code for profiler
    #
    def gen_profile_thread_init(self,tid,num_tasks):
        return ''

    #
    # generate open code for profiler
    #
    def gen_profile_open(self,tid):
        return ''

    #
    # generate code for initiating a measurement for an actor firing
    #
    def gen_actor_profile_start(self,tid,aid):
        return ''

    #
    # generate code for stopping a measurement for an actor firing
    #
    def gen_actor_profile_stop(self,tid,aid):
        return ''

    def gen_actor_profile_record(self,name,tid,aid,ivar):
        return ''

    #
    # generate code for initiating measurement at the beginning of a threads' work
    # for a single steady state iteration
    #
    def gen_thread_profile_start(self,tid):
        return ''

    #
    # generate code for stopping measurement at the beginning of a threads' work
    # for a single steady state iteration
    #
    def gen_thread_profile_stop(self,tid):
        return ''

    def gen_thread_profile_record(self,tid,ivar):
        return ''

    #
    # generate code for initiating measurement at the beginning of a threads' work
    # for a single steady state iteration
    #
    def gen_program_profile_start(self):
        return ''

    #
    # generate code for stopping measurement at the beginning of a threads' work
    # for a single steady state iteration
    #
    def gen_program_profile_stop(self):
        return ''

    def gen_program_profile_record(self):
        return ''

    #
    # generate closing code for profiler
    #
    def gen_profile_close(self,tid):
        return ''

    #
    # generate code for writing profile information to a file
    #
    def gen_profile_write(self,tid):
        return ''

    #
    # generate code for writing summary profile information to a file
    #
    def gen_profile_write_summary(self):
        return ''
