# Implementation of an SDF graph class
# in Python that constructs the graph,
# provides query methods, and computes
# repetition vector and schedule.

import sys
import csv
import re
from pygraph.classes.digraph import digraph
from pygraph.algorithms.sorting import topological_sorting
from fractions import gcd
from sets import Set
from helper.factors import lcm, listgcd, listlcm
from scheduler.selector import selector

class sdfgraph:
    """Implementation of a SDF graph computation
    """

    def __init__(self,name='',preamble='',postamble=''):
        """Initializes SDF graph class
        """
        # SDF related attributes
        self.graph = digraph()            # graph representation
        self.name = name                  # name of sdf function
        self.preamble = preamble          # C-code for preamble
        self.postamble = postamble        # C-code for postamble
        self.no_code = False              # Error state for parsing
        self.index_ctr = 0

        # node related attributes
        self.node_index = {}              # index of node to order parallel nodes properly
        self.node_type = {}               # node type generic or splitter/joiner
        self.node_param = {}              # parameters of actors                                               
        self.repetition = {}              # repetition vector
        self.actor_code = {}              # code of actor for actor firing
        self.comm_code = {}
        self.state = {}                   # state variables of actors
        self.init = {}                    # initialization for state variables
        self.exec_time = {}               # execution times of a single invocation for each actors
        self.allocation = {}

        # edge related attributes
        self.consumption = {}             # consumption rate
        self.production = {}              # production rate
        self.delay = {}                   # number of delay tokens (i.e., init tokens)
        self.token_type = {}              # data type of token
        self.source_tokens = {}           # tokens for producer
        self.target_tokens = {}           # tokens for consumer
        self.peek_tokens = {}             # tokens for peeking
        self.delay_tokens = {}            # delay tokens on the edge

    #
    # construction methods for adding actors and channels
    #

    def add_node(self,name,nt='generic'):
        """Add node to SDF graph

           :param name: identifier of the new node.
           :param nt: type of the new node.
           :param in_names: a dict of token names for each source edge.
           :param out_names: a dict of token names for each target edge.
           :param code: the code the actor fires.
        """
        if not self.graph.has_node(name):
            self.graph.add_node(name)
            self.node_param[name]=[]
            self.actor_code[name]=''
            self.init[name]=''
            self.state[name]=''
        self.node_index[name]=self.index_ctr
        self.index_ctr = self.index_ctr+1

        self.repetition[name]=1
        self.node_type[name]=nt

    def add_edge(self,source,target,p_rate=0,c_rate=0,delay=0,ttype="int",label="",st=[],tt=[],dt=[]):
        """Add edge to SDF graph

           :param source: identifier of the source node.
           :param target: identifier of the target node.
           :param p_rate: number of tokens that are produced by the source node.
           :param c_rate: number of tokens that are consumed by the target node.
           :param delay: number of delay tokens.
           :param ttype: type of the tokens.
           :param label: label for the edge.
        """
        edge=(source,target)
        self.graph.add_edge(edge,label=label)
        self.production[edge]=p_rate
        self.consumption[edge]=c_rate
        self.delay[edge]=delay
        self.token_type[edge]=ttype
        self.source_tokens[edge]=st
        self.peek_tokens[edge]=[]
        self.target_tokens[edge]=tt
        self.delay_tokens[edge]=dt
        if self.delay[edge] == 0 and len(dt) > 0:
            self.delay[edge] = len(dt)
        if p_rate > 0 and len(st) == 0:
            self.source_tokens[edge] = ['y_'+source+'_'+target+'_'+str(i) for i in range(1,p_rate+1)]
        if c_rate > 0 and len(tt) == 0:
            self.target_tokens[edge] = ['x_'+source+'_'+target+'_'+str(i) for i in range(1,c_rate+1)]

    def set_node(self,name,nt='generic',code='',init='',state=''):
        """Add node to SDF graph

        :param name: identifier of the new node.
        :param nt: type of the new node.
        :param in_names: a dict of token names for each source edge.
        :param out_names: a dict of token names for each target edge.
        :param code: the code the actor fires.
        """
        if not self.graph.has_node(name):
            self.error("No topological information is found for "+name+".")

        self.repetition[name]=1
        self.node_type[name]=nt
        self.actor_code[name]=code
        self.init[name]=init
        self.state[name]=state


    def set_sourcetokens(self,source,target,tokens):
        """ set tokens of the source of an edge
            :param source: source of the edge
            :param target: target of the edge
            :param tokens: source tokens of the edge
        """
        self.source_tokens[(source,target)]=tokens
        self.production[(source,target)]=len(tokens)

    def set_targettokens(self,source,target,tokens):
        """ set tokens of the target of an edge
            :param source: source of the edge
            :param target: target of the edge
            :param tokens: target tokens of the edge
        """
        self.target_tokens[(source,target)]=tokens
        self.consumption[(source,target)]=len(tokens)

    def set_peektokens(self,source,target,tokens):
        """ set tokens of the target of an edge
            :param source: source of the edge
            :param target: target of the edge
            :param tokens: target tokens of the edge
        """
        self.peek_tokens[(source,target)]=tokens

    def add_param(self, actor, param_type, param_name, param_value):
        """ add each parameter of actors
            :param actor: name of the actor
            :param param_type: type of the parameter
            :param param_name: name of the parameter
            :param param_value: value of the parameter
        """
        param_list = self.node_param[actor]
        for i in range(0, len(param_list)):
           (p_type, p_name, p_value) = param_list[i]
           if (p_type == param_type) and (p_name == param_name):
              param_list[i] = (param_type, param_name, param_value)
              return

        self.node_param[actor].append((param_type, param_name,
                    param_value))
       
    def get_params(self, actor):
        """ get a list of parameters
            :param actor: name of the actor
        """
        return self.node_param[actor]

    def get_param_value(self, actor, param_name):
        """ get the value of a parameter
            :param actor: name of the actor
            :param para_name: name of the parameter
        """
        param_list=self.node_param[actor]
        for i in range(0, len(param_list)):
           (p_type, p_name, p_value) = param_list[i]
           if param_name == p_name:
              return p_valuel
        self.error("No parameter, "+p_name+" is found from "+actor+".")

    def error(self,txt):
        """ set error state caused by parsing
            :param txt: error message
        """
        self.no_code = True
        sys.stderr.write('Error: '+txt+'.\n')

    def set_postamble(self,code):
        """ set postamble """
        self.postamble = code

    #
    # query methods for quering nodes, edges, and in/out-nodes
    #

    def edges(self):
        """ Return list of nodes in alphabetical order using the source as
            a primary key.
        """
        return sorted(self.graph.edges(), key=lambda source_node: source_node[0])

    def nodes(self):
        """ Return list of nodes in alphabetical order
        """
        return topological_sorting(self.graph)

    def in_nodes(self, node):
        """ Return list of predecessors of an actor
        :param node: identifier of actor
        """
        return sorted(self.graph.incidents(node), key=lambda node: self.node_index[node],reverse=False)

    def out_nodes(self, node):
        """ Return list of successors of an actor
        :param node: identifier of actor
        """
        return sorted(self.graph.neighbors(node), key=lambda node: self.node_index[node],reverse=False)

    def is_generic(self,node):
        """ Queries type of node, i.e., generic or splitter/joiner otherwise
        :param node: identifier of actor
        """
        return self.node_type[node] == 'generic'

    def has_error(self):
        """ Return true if parsing error occurred. """
        return self.no_code

    def has_edge(self,u,v):
        """ Checks whether edge u->v exists in graph """
        return self.graph.has_edge((u,v))

    def compute_repetitions(self):
        """Compute repetitions for nodes such that balance equations hold.
        """
        # defines the factors for a node
        factors={}
        # set repetitions of nodes to 1
        for u in self.nodes():
            self.repetition[u]=1

        ctr=0;
        changed=True
        while changed:
            changed=False
            # init factors for a node
            for u in self.nodes():
                factors[u]=Set()
            # traverse all edges and compute factors
            for (source,target) in self.edges():
                edgelcm=lcm(self.repetition[source]*self.production[(source,target)], \
                            self.repetition[target]*self.consumption[(source,target)])
                factors[source].add(edgelcm / self.repetition[source]/self.production[(source,target)])
                factors[target].add(edgelcm / self.repetition[target]/self.consumption[(source,target)])
            for u in self.nodes():
                value=listlcm(list(factors[u]))
                if value <> 1:
                    changed=True
                    self.repetition[u]=self.repetition[u]*value
            ctr=ctr+1
            if ctr > len(self.nodes()):
                return False
        """
        #
        # flatten repetitions
        for node in self.nodes():
            if not self.is_generic(node):
                continue

            new_actor_code=''
           
            # initialize new actor code, new in/out tokens
            new_actor_code='\n{\n'+self.actor_code[node]+'\n}\n'
            new_out_tokens={}
            for post in self.out_nodes(node):
                edge = (node, post)
                if not edge in new_out_tokens:
                    new_out_tokens[edge]=[]
            new_in_tokens={}
            for pred in self.in_nodes(node):
                edge = (pred,node)
                if not edge in new_in_tokens:
                    new_in_tokens[edge]=[]

            # duplicate actor codes
            for rep in range(1, self.repetition[node]):
                actor_code=self.actor_code[node]
                postfix='_'+str(rep)
                for pred in self.in_nodes(node):
                    edge = (pred,node)
                    for in_t in self.target_tokens[edge]:
                        new_in_t=in_t+postfix
                        actor_code=re.sub(r"%s(?=[^0-9A-Za-z_])" % in_t, new_in_t, actor_code)
                        new_in_tokens[edge].append(new_in_t)
                        
                for post in self.out_nodes(node):
                    edge = (node, post)
                    for out_t in self.source_tokens[edge]:
                        new_out_t=out_t+postfix
                        actor_code=re.sub(r"%s(?=[^0-9A-Za-z_])" % out_t, new_out_t, actor_code)
                        new_out_tokens[edge].append(new_out_t)
                new_actor_code=new_actor_code+'{\n'+actor_code+'\n}\n'
            self.actor_code[node]=new_actor_code

            # duplicate outgoing delays
            for post in self.out_nodes(node):
                edge=(node,post)
                self.delay[edge] = self.delay[edge]*self.repetition[node]
                self.delay_tokens[edge] = self.delay_tokens[edge]*self.repetition[node]

            # update token list
            for pred in self.in_nodes(node):
                edge=(pred,node)
                self.target_tokens[edge]=self.target_tokens[edge]+new_in_tokens[edge]
                self.consumption[edge]=self.consumption[edge]*self.repetition[node]
            for post in self.out_nodes(node):
                edge=(node,post)
                self.source_tokens[edge]=self.source_tokens[edge]+new_out_tokens[edge]
                self.production[edge]=self.production[edge]*self.repetition[node]
            self.repetition[node]=1
        """
        return True

    def get_comm_in_byte(self):
        """ Return the defined data communication in byte for a steady state
            iteration.
        """
        if self.repetition == {}:
            self.error("Repetition vector computation is required.")
            sys.exit(1)
            
        total_comm = 0
        direct_comm = 0
        eliminated_comm = 0

        for edge in self.edges():
            (source, target) = edge
            token_type = self.token_type[edge]
            token_size = 0
            
            if token_type == 'char':
                token_size = 1
            elif token_type == 'int':
                token_size = 4
            elif token_type == 'float':
                token_size = 4
            elif token_type == 'complex':
                token_size = 8
            else:
                self.error("Not supported token type "+token_type+" found on edge ("\
                            +source+","+target+").")
                sys.exit(1)
            store = self.production[edge]*self.repetition[source]*token_size
            load = self.consumption[edge]*self.repetition[target]*token_size
            if not store == load:
               self.error("Data rate of ("+source+","+target+") for store,"\
                       +str(store)+" does not match to the one for load, "+str(load))
               sys.exit(1)

            if self.is_generic(source):
                direct_comm = direct_comm+store
            else:
                eliminated_comm = eliminated_comm+store

            if self.is_generic(target):
                direct_comm = direct_comm+load
            else:
                eliminated_comm = eliminated_comm+load
            total_comm = total_comm+store
            total_comm = total_comm+load
        return (total_comm, direct_comm, eliminated_comm)
        
    def get_nodes(self):
        """ Return overall number of nodes(filter/splitjoin) and edges
        """
        num_filters = [x for x in self.nodes() if self.is_generic(x)]
        num_splitjoins = [x for x in self.nodes() if not self.is_generic(x)]

        return (num_filters, num_splitjoins)
        

    def get_actor_exec_times(self, fname, select_method):
        """ Read fname to obtain the raw execution times of actors
            template of input file should be:
            <actor id>\t<proc. index>\t<task index in the schedule>\t<exec. time in us>
        """
        # set a selector to pick the representative numbers out of raw data
        exectime_selector = selector(select_method)

        # read raw execution times
        actor_exec_times={}
        for node in self.nodes():
            if not self.is_generic(node):
                continue 
            actor_exec_times[node] = []

        with open(fname, 'r') as csvfile:
            rawdata = csv.reader(csvfile, delimiter='\t')
            for row in rawdata:
                actor = row[0]
                proc_id = int(row[1])   # ignored
                iteration = int(row[2]) # ignored
                exec_time = float(row[3])
                actor_exec_times[actor].append(exec_time)

        # store selected execution times
        for node in self.nodes():
            if self.is_generic(node):
                self.exec_time[node] = \
                     exectime_selector.select(actor_exec_times[node])
            else:
                self.exec_time[node] = 0

    def print_actor_exec_times(self,fname,select_method):
        if self.exec_time == {}:
            self.get_actor_exec_times(fname, select_method)
        print('------------------------------')
        for node in self.nodes():
            sys.stdout.write('%20s' %(node))
            sys.stdout.write('%14.4f' % (self.exec_time[node]))
            print ''
        print('------------------------------')
        print ''

        
    def print_proc_exec_times(self, fname, select_method):
        """ Read fname to obtain the raw execution times of actors
            template of input file should be:
            \t<proc. index>\t<task index in the schedule>\t<exec. time in us>
        """
        # set a selector to pick the representative numbers out of raw data
        exectime_selector = selector(select_method)

        # read raw execution times
        proc_exec_times={}
        results={}
        num_processors=0
        with open(fname, 'r') as csvfile:
            rawdata = csv.reader(csvfile, delimiter='\t')
            for row in rawdata:
                actor = row[0]
                proc_id = int(row[1])
                iteration = int(row[2]) # ignored
                exec_time = float(row[3])
                if not proc_id in proc_exec_times:
                    proc_exec_times[proc_id]=[]
                proc_exec_times[proc_id].append(exec_time)
                if num_processors < proc_id:
                    num_processors = proc_id

        # from 0-based index to 1-based index
        num_processors = num_processors+1

        max = 0.0;
        # store selected execution times
        print '------------------------------'
        for proc in range(0, num_processors):
            results[proc] = \
                exectime_selector.select(proc_exec_times[proc])
            sys.stdout.write('%10s' %('PROC '+str(proc)))
            sys.stdout.write('%14.4f' % (results[proc]))
            if max < results[proc]:
                max = results[proc]
            print ''
        print '------------------------------'
        print(' Overall :%10.4f' % (max))
        print ''

    def dump(self):
        """Dumps all info about the graph.
        """
        print 'name'
        print self.name
        print 'preamble'
        print self.preamble
        print 'postamble'
        print self.postamble

        print 'nodes'
        for node in self.nodes():
            print node
            print '\tnode type:', self.node_type[node]
            print '\tnode index:', self.node_index[node]
            print '\trepetition:', self.repetition[node]
            print '\t{\n', self.actor_code[node], '\n\t}'

        print '\n'
        print 'edges'
        for edge in self.edges():
            print edge
            print '\tconsumption:',self.consumption[edge]
            print '\tproduction :',self.production[edge]
            print '\tdelay      :',self.delay[edge]
            print '\ttoken type :',self.token_type[edge]
            print '\ttarget toks:',self.target_tokens[edge]
            print '\tpeek toks:',self.peek_tokens[edge]
            print '\tsource toks:',self.source_tokens[edge]
