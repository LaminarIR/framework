from codegen.prefetcher.prefetcher import prefetcher
from codegen.writer import writer

class gnu_builtin_prefetcher(prefetcher):

    def read_prefetch(self, var, source, target):
        code=writer()
        locality=2
        if not source == target:
            code.writeln('__builtin_prefetch(&'+var+', 0, '+str(locality)+');')
        return code.get()
    
    def write_prefetch(self, var, source, target):
        code=writer()
        locality=2
        if not source == target:
            code.writeln('__builtin_prefetch(&'+var+', 1, '+str(locality)+');')
        return code.get()
