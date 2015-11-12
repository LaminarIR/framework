# define writer class to generate code in a string.
# class has indendation capabilities.
#
class writer:
    """Writer class for code generation assembles code
       snippets into a string allowing indentations.
    """

    def __init__(self):
        """Initializes code generation class
        """
        self.text = ""
        self.tab = '  '
        self.tabs = 0
        self.first = True

    def write(self,txt):
        """Append given code to the buffer string without newline
        """

        if self.first:
            for i in range(0,self.tabs):
                self.text = self.text + self.tab
            self.first = False
        self.text = self.text + txt

    def writeln(self,txt=''):
        """Append given code to the buffer string with newline
        """
        if self.first:
            for i in range(0,self.tabs):
                self.text = self.text + self.tab
            self.first = False
        self.text = self.text + txt + "\n"
        self.first = True

    def setIndent(self, width):
        """Set size of a indent
        """
        self.tab = width;

    def indent(self):
        """Increase indent level.
        """
        self.tabs = self.tabs + 1

    def unindent(self):
        """Decrease indent level.
        """
        self.tabs = self.tabs - 1

    def get(self):
        """Return the buffer string.
        """
        return self.text
