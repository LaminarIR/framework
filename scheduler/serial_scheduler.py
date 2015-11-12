#
# serial scheduler using greedy algorithm
#

from pygraph.algorithms.sorting import topological_sorting
from scheduler import scheduler
from random import choice
from math import ceil
from math import fabs
from helper.factors import lcm, listlcm
import sys

class serial_scheduler(scheduler):
    #
    # compute the schedule using a greedy algorithm
    # to minimize the memory consumption.
    #
    def get_schedule(self):
        """Greedy algorithm for computing a schedule for sequential run.
        """
        # get graph
        sdfgraph = self.sdfgraph
        # reset schedule
        schedule = []
        # initialize fillstate
        fillstate = {}
        for edge in sdfgraph.edges():
            fillstate[edge]=sdfgraph.delay[edge]
        # initialize number of invocations with repetitions
        num_invocations = {}
        for node in sdfgraph.nodes():
            num_invocations[node] = sdfgraph.repetition[node]

        # set allocation
        for node in sdfgraph.nodes():
            sdfgraph.allocation[node]=0

        # compute the gain/loss of a node
        # for the objective function
        gain_loss = {}
        schedule_len = 0
        for node in sdfgraph.nodes():
            schedule_len = schedule_len + sdfgraph.repetition[node]
            gain_loss[node] = 0
            for out_node in sdfgraph.out_nodes(node):
                edge=(node,out_node)
                gain_loss[node] = gain_loss[node] + sdfgraph.production[edge]
            for in_node in sdfgraph.in_nodes(node):
                edge=(in_node,node)
                gain_loss[node] = gain_loss[node] - sdfgraph.consumption[edge]
        sorted_nodes = sorted(sdfgraph.nodes(),key=lambda node: gain_loss[node],reverse=True)
        # greedly search for nodes that effect the objective the least

        # initialize fillstate
        fillstate = {}
        for edge in sdfgraph.edges():
            fillstate[edge]=sdfgraph.delay[edge]

        # run boot schedule if needed
        bootschedule = self.get_bootschedule()
        for seq in range(0,len(bootschedule)):
            node = bootschedule[seq]
            for in_node in sdfgraph.in_nodes(node):
                edge=(in_node,node)
                fillstate[edge]=fillstate[edge]-sdfgraph.consumption[edge]
            for out_node in sdfgraph.out_nodes(node):
                edge=(node,out_node)
                fillstate[edge]=fillstate[edge]+sdfgraph.production[edge]

        # initialize number of invocations with repetitions
        num_invocations = {}
        for node in sdfgraph.nodes():
            num_invocations[node] = sdfgraph.repetition[node]
        while schedule_len > 0:
            for node in sorted_nodes:
                if num_invocations[node] > 0:
                    # check whether node is fireable, i.e. enough tokens on the input channel
                    firable = True
                    for in_node in sdfgraph.in_nodes(node):
                        edge=(in_node,node)
                        pop_rate = sdfgraph.consumption[edge]
                        peek_rate = len(sdfgraph.peek_tokens[edge])
                        if fillstate[edge]-(pop_rate+peek_rate) < 0:
                            firable = False
                    # If so, add to schedule and update fillstate
                    if firable:
                        for in_node in sdfgraph.in_nodes(node):
                            edge=(in_node,node)
                            fillstate[edge]=fillstate[edge]-sdfgraph.consumption[edge]
                        for out_node in sdfgraph.out_nodes(node):
                            edge=(node,out_node)
                            fillstate[edge]=fillstate[edge]+sdfgraph.production[edge]
                        num_invocations[node] = num_invocations[node] - 1
                        schedule.append(node)
                        break
            schedule_len = schedule_len - 1

        # print result
#        sched_out=open('sched.out','w')
#        for index in range(0, len(schedule)):
#          sched_out.write( '\t '+schedule[index]+'\n')
#        sched_out.close()
        return [ schedule ]


    def get_bootschedule(self):
        """ Calculate a Single Appearance Schedule to boot peeking tokens.
        """

        sdfgraph = self.sdfgraph
        edges = sdfgraph.edges()
        peek_tokens = sdfgraph.peek_tokens
        strong_connected = self.strong_connected
        repetition = sdfgraph.repetition
        boot_schedule = []

        if self.boot_schedule != []:
            return self.boot_schedule

        PEEK_FOUND = False
        for edge in edges:
            if len(peek_tokens[edge]) > 0:
                PEEK_FOUND = True
                break

        if not PEEK_FOUND:
            return []

        # calculate repetitions of each to fill up peeks only
        rep={}

        nodes=topological_sorting(sdfgraph.graph)
        nodes.reverse()         # reversed
        for node in nodes:
            if node in rep:   # the node is backedge
                continue
            rep[node] = 0
            backedge = [];
            for succ in sdfgraph.out_nodes(node):
                edge = (node, succ)
                if succ not in rep:   # the node is backedge
                    backedge.append(succ)   # process later
                    continue
                rep[node]=max(rep[node],int(ceil((rep[succ]*sdfgraph.consumption[edge]+len(peek_tokens[edge]))/float(sdfgraph.production[edge]))))
            for u in backedge:
                rep[u]=rep[node]

            # calculate max level difference
            for succ in sdfgraph.out_nodes(node):
                edge = (node,succ)
                diff = fabs(int(ceil(float(rep[node])/repetition[node]))-int(ceil(float(rep[succ])/repetition[succ])))
                if self.max_lev_diff < diff:
                    self.max_lev_diff = diff

        nodes.reverse()         # ordered

        # initialize number of actor invocations required
        schedule_len = 0
        for node in nodes:
            schedule_len = schedule_len + rep[node]

        # initialize fillstate
        fillstate = {}
        for edge in sdfgraph.edges():
            fillstate[edge]=sdfgraph.delay[edge]
        
        while schedule_len > 0:
            init_sched_len = schedule_len
            for node in nodes:
                if rep[node] > 0:
                    # check whether node is fireable, i.e. enough tokens on the input channel
                    firable = True
                    for in_node in sdfgraph.in_nodes(node):
                        edge=(in_node,node)
                        pop_rate = sdfgraph.consumption[edge]
                        peek_rate = len(sdfgraph.peek_tokens[edge])
                        if fillstate[edge]-(pop_rate+peek_rate) < 0:
                            firable = False
                    # If so, add to bootschedule and update fillstate
                    if firable:
                        for in_node in sdfgraph.in_nodes(node):
                            edge=(in_node,node)
                            fillstate[edge]=fillstate[edge]-sdfgraph.consumption[edge]
                        for out_node in sdfgraph.out_nodes(node):
                            edge=(node,out_node)
                            fillstate[edge]=fillstate[edge]+sdfgraph.production[edge]
                        rep[node] = rep[node] - 1
                        boot_schedule.append(node)
                        schedule_len = schedule_len - 1
            if init_sched_len == schedule_len:
                print 'ERROR: Boot schedule calculation failed.'
                sys.exit(1)
        self.boot_schedule = boot_schedule
        
        # test automatically if both of schedules are computed
        #self.check_schedules()
        return self.boot_schedule

    def get_max_lev_diff(self):
        """ return maximum level difference to decide number of repetitions in a loop
        """
        if self.boot_schedule == []:
            self.get_bootschedule()
        return int(self.max_lev_diff)

