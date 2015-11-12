# define abstract scheduler class
#

from pygraph.algorithms.accessibility import mutual_accessibility
from copy import deepcopy
import sys

class scheduler:
 
    def __init__(self, sdfgraph, num_processors=1):
        """ initialize scheduler class
        """
        self.sdfgraph = sdfgraph
        self.strong_connected = mutual_accessibility(sdfgraph.graph)
        self.num_processors = num_processors
        self.steady_schedule = {}
        self.boot_schedule = []
        self.max_lev_diff = 1

    def get_numprocessors(self):
        """ return the number of processors
        """
        return self.num_processors

    def get_bootschedule(self):
        """ return the sequence of actor firings to get into steady state
        """
        return []

    def get_schedule(self):
        """ return sequence for each processor
        """
        return []

    def get_max_lev_diff(self):
        """ return maximum level difference to decide number of repetitions in a loop 
        """
        return int(self.max_lev_diff)

    def get_sequence_of_scc(self, actor):
        """ return a ordered sequence of actors in a strongly connected 
            component of the given actor
        """
        # find a sequence of the execution in a strongly connected 
        # component
        strong_connected = self.strong_connected
        sdfgraph = self.sdfgraph
        start_actor = actor
        sequence = []

        for connected_actor in strong_connected[actor]:
            LOADED = True
            for pre in sdfgraph.in_nodes(connected_actor):
                in_edge = (pre,connected_actor)
                if sdfgraph.delay[in_edge]<sdfgraph.consumption[in_edge]:
                    LOADED = False
            if LOADED:
                start_actor = connected_actor

        # make a schedule for the strongly connected component
        while start_actor not in sequence:
            sequence.extend([start_actor for i in range(0,sdfgraph.repetition[start_actor])])
            for connected_actor in strong_connected[start_actor]:
                if connected_actor in sdfgraph.out_nodes(start_actor):
                    start_actor = connected_actor
                    break
        return sequence
       
        

    def check_schedules(self):
        """ Test boot schedule and steady state schedule
        """
        sdfgraph = self.sdfgraph

        # initialize fifo
        fifo = {}
        for edge in sdfgraph.edges():
            fifo[edge] = sdfgraph.delay[edge]

        # simulate the boot schedule
        for actor in self.boot_schedule:
            for pre in sdfgraph.in_nodes(actor):
                in_edge = (pre,actor)
                if fifo[in_edge] < sdfgraph.consumption[in_edge]:
                    print 'ERROR: edge ('+pre+','+actor+') lacks tokens to run the boot schedule'
                    print 'schedule:'
                    print self.boot_schedule
                    sys.exit(1)
                else:
                    fifo[in_edge] = fifo[in_edge] - sdfgraph.consumption[in_edge]
            for post in sdfgraph.out_nodes(actor):
                out_edge = (actor,post)
                fifo[out_edge] = fifo[out_edge] + sdfgraph.production[out_edge]

        # simulate the steady schedule for all procs
        fifo_init_snapshot = deepcopy(fifo)
        for proc in range(0, self.get_numprocessors()):
            fifo = fifo_init_snapshot
            for actor in self.steady_schedule[proc]:
                for pre in sdfgraph.in_nodes(actor):
                    in_edge = (pre,actor)
                    if fifo[in_edge] < sdfgraph.consumption[in_edge]:
                        print 'ERROR: edge ('+pre+','+actor+') lacks tokens to run the steady-state schedule'
                        print 'schedule:'
                        print self.steady_schedule
                        sys.exit(1)
                    else:
                        fifo[in_edge] = fifo[in_edge] - sdfgraph.consumption[in_edge]
                for post in sdfgraph.out_nodes(actor):
                    out_edge = (actor,post)
                    fifo[out_edge] = fifo[out_edge] + sdfgraph.production[out_edge]


