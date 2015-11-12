class counter:

    #
    # generate counter code for profiling variables
    #
    def header(self):
        return ''

    #
    # generate counter init code for profiler
    #
    def init(self,num_proc,num_iter):
        return ''

    #
    # generate counter thread init code for profiler
    #
    def thread_init(self,tid,num_tasks):
        return ''

    #
    # generate counter open code for profiler
    #
    def open(self, tid):
        return ''

    #
    # generate counter code for initiating measurement
    #
    def start(self,tid,aid):
        return ''
    
    #
    # generate counter code for stopping measurement
    #
    def stop(self,tid,aid):
        return ''

    #
    # generate counter closing code for profiler
    #
    def close(self,tid):
        return ''

    def record(self,name,tid,aid,ivar):
        """ store the value into the internal data array
            :param name: string. Name of the value to be shown on the record. 
            :param tid: int. Index of the current thread.
            :param aid: int. Index of the current actor.
            :param ivar: string. Name of iteration variable.
        """
        return ''

    def write(self,tid):
        return ''    

    def write_summary(self):
        return ''
