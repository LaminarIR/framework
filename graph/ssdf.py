# Implementation of an SSDF graph class
# inherits from the SDF graph class

import sdf

class ssdfgraph(sdf.sdfgraph):
    """Implementation of an SSDF graph computation
    """
    struct_num = 0 # Structure number

    def __init__(self, name='', preamble='', postamble=''):
        """Initializes the SSDF graph class
        """

        sdf.sdfgraph.__init__(self, name, preamble, postamble) # Call SDF class init

        # SSDF related attributes
        self.structures = {}              # The set of structures in the graph

    def add_structure(self, st='pipeline', name='', contents=None):
        """ Add a structure to the SSDF graph

        :param st: type of the new structure.
        :param name: identifier of the new structure.
        :param contents: the identifiers of the structures comprising the new structure.
        """

        if not name:
            name = "structure_" + str(ssdfgraph.struct_num)
            ssdfgraph.struct_num += 1

        structure = Structure(self, st, name, contents)
        self.structures[name] = structure

        return structure

    def add_actor(self, name, nt='generic', code='', init='', state=''):
        """Add node to SSDF graph as a structure

        :param name: identifier of the new node.
        :param nt: type of the new node.
        :param in_names: a dict of token names for each source edge.
        :param out_names: a dict of token names for each target edge.
        :param code: the code the actor fires.
        """

        sdf.sdfgraph.add_node(self, name, nt, code, init, state) # Add to the SDF

        structure = Structure(self, 'actor', name)
        self.structures[name] = structure

        return structure

    def link_structures(self):
        """Link together the internal structures of each structure
        """

        for structure in self.structures.values():
            structure.link()

    def get_structure(self, name):
        """Get a structure from the SSDF graph

        :param name: identifier of the structure.
        """

        if name in self.structures:
            return self.structures[name]

    def get_structure_in(self, name):
        """Get a structure's in node

        :param name: identifier of the structure.
        """

        struct = self.get_structure(name)
        if struct:
            return struct.in_node
        else:
            # Assume the structure is an actor
            self.add_actor(name, nt="distributor")
            return name

    def get_structure_out(self, name):
        """Get a structure's out node

        :param name: identifier of the structure.
        """

        struct = self.get_structure(name)
        if struct:
            return struct.out_node
        else:
            # Assume the structure is an actor
            self.add_actor(name, nt="distributor")
            return name


class Structure:
    """Implementation of a structure in an SSDF graph
    """

    def __init__(self, graph, st='pipeline', name='', contents=None):
        """Create an SSDF graph structure

        :param graph: graph to which the new structure will be added.
        :param st: type of the new structure.
        :param name: identifier of the new structure.
        :param contents: the identifiers of the structures comprising the new structure.
        """

        # Structure related attributes
        self.name = name
        self.contents = contents
        self.structure_type = st

        # graph related attributes
        self.graph = graph
        self.in_node = None
        self.out_node = None

        # In and out nodes
        if self.structure_type in "actor":
            self.in_node = self.name
            self.out_node = self. name

        elif self.structure_type == "pipeline":
            if self.contents:
                self.in_node = self.graph.get_structure_in(contents[0])
                self.out_node = self.graph.get_structure_out(contents[-1])

        elif self.structure_type == "feedback":
            if self.contents:
                self.in_node = self.contents[0]
                self.out_node = self.contents[0]

        elif self.structure_type == "splitjoin":
            # Create nodes before and after the split
            self.in_node = self.name+"__in__"
            self.graph.add_node(self.in_node)
            self.out_node = self.name+"__out__"
            self.graph.add_node(self.out_node)

    def link(self):
        """Link together the internal structures of this structure
        """

        if self.structure_type == "pipeline":
            # Internal pipeline structure
            for i in xrange(len(self.contents)-1):
                source,target = self.contents[i:i+2]
                source = self.graph.get_structure_out(source)
                target = self.graph.get_structure_in(target)

                self.graph.add_edge(source, target, 1,1)

        elif self.structure_type == "feedback":
            # Feedback loop
            if self.contents:
                self.graph.add_edge(self.contents[0], self.contents[0], 1, 1, dt=['0','1'])

        elif self.structure_type == "splitjoin":
            # Split Join structure
            for structure in self.contents:
                self.graph.add_edge(self.in_node, structure, 1, 1)
                self.graph.add_edge(structure, self.out_node, 1, 1)
                
