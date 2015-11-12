#!/usr/bin/python

# Options:

# -o <output>
# --output <output>

# -g <generatorname>
# --generator <generatorname>

# Input: Either from stdin or from file

import sys

sys.path.append("./parser")
sys.path.append("./parser/antlr3")

import antlr3
import getopt
import os.path

from laminarLexer import laminarLexer
from laminarParser import laminarParser

from codegen.serial_direct_generator import serial_direct_generator
from codegen.serial_fifo_generator import serial_fifo_generator

from scheduler.serial_scheduler import serial_scheduler

from codegen.profiler.actor_profiler import actor_profiler
from codegen.profiler.thread_profiler import thread_profiler
from codegen.profiler.program_profiler import program_profiler
from codegen.profiler.profiler import profiler

from codegen.counter.rts import rts
from codegen.counter.papi_wrapper import papi_wrapper
from codegen.counter.counter import counter

from codegen.prefetcher.prefetcher import prefetcher
from codegen.prefetcher.gnu_builtin_prefetcher import gnu_builtin_prefetcher

from scheduler.selector import selectMedian

from codegen.writer import writer

#
# Default input Parameters
INFILE= ""              # name of input test file (.sdf)
OUTFILE=""              # name of generated code  (.c)
GENOPT= "serial-direct" # option for actor communications
PROFOPT="no-profiler"   # option for time measurement
COUNTEROPT= "rts"
PREFETCHEROPT= "gnu_builtin_prefetcher"
PREFETCHEROPT= "no_prefetcher"
NUMPROC= 1              # number of thrads
ITER=   100             # number of steady-state iteration to execute
RESULT=False            # print representative execution times of actors/threads reading times.out
ELIM_COMM=False         # print total and eliminated communications in byte

GENS = {
    'serial-direct' : serial_direct_generator,
    'serial-fifo'   : serial_fifo_generator,
}

PROFS = {
    'actor' : actor_profiler,
    'thread' : thread_profiler,
    'program' : program_profiler,
    'no-profiler' : profiler
}

SCHEDS = {
    'serial-direct' : serial_scheduler,
    'serial-fifo'   : serial_scheduler,
}

COUNTERS = {
    'rts' : rts,
    'papi_wrapper' : papi_wrapper,
    'no-counter' : counter
}

PREFETCHERS = {
    'gnu_builtin_prefetcher': gnu_builtin_prefetcher,
    'no_prefetcher' : prefetcher
}

def help():
    """ Print usage of laminar.py
    """
    print 'Usage: python laminar.py <option> ... <input file name>'
    print '    Options:'
    print '        -o|--output <file name>  '
    print '           File name for generated code.'
    print '           Print results on screen when no output file name is given'
    print ''
    print '        -g|--code_generator <opt>  '
    print '           Select option for code-generator'
    print '           options: '
    print '             serial-direct : use direct memory access'
    print '             serial-fifo   : use fifo buffers'
    print '             '+GENOPT+' (default)'
    print ''
    print '        -p|--profiler <opt> '
    print '           Select option for time measurement'
    print '           Options: '
    print '             no-profiler: no execution time measurement'
    print '             actor: measure execution times of each actors'
    print '             thread: measure execution times of each threads'
    print '             '+PROFOPT+' (default)'
    print ''
    print '        -c|--counter <opt> '
    print '           Select option for counter'
    print '           Options:'
    print '             rts : clock counter for Intel arch.'
    print '             papi: multi-platform counter'
    print '             '+COUNTEROPT+' (default)'
    print ''
    print '        -P|--Proc <num> '
    print '           Integer number of processors to utilize'
    print '             '+str(NUMPROC)+' (default)'
    print ''
    print '        -I|--Iter <num> '
    print '           Integer number of steady-state iterations to run'
    print '             '+str(ITER)+' (default)'
    print ''
    print '        -r|--result'
    print '           Print representative execution times of actors/threads reading times.out'
    print ''
    print '        -e|--elim_comm'
    print '           Print total and eliminated communications in byte.'

def getArgs(argv):
    """ Parse command-line arguments.
    """
    global INFILE
    global OUTFILE
    global GENOPT
    global PROFOPT
    global COUNTEROPT
    global NUMPROC
    global ITER
    global RESULT
    global ELIM_COMM

    try:
        opts, args = getopt.getopt(argv, "hreo:g:p:c:P:I:", \
                ["help","result","elim_comm","output=","code_generator=",\
                 "profiler=","counter=","Proc=","Iter="])
    except getopt.GetoptError:
        help()
        sys.exit(1)

    for opt, arg in opts:
        if opt in ('-h', '--help'):
            help()
            sys.exit()
        elif opt in ("-i", "--input"):
            INFILE=arg
        elif opt in ("-o", "--output"):
            OUTFILE=arg
        elif opt in ("-r", "--result"):
            RESULT=True
        elif opt in ("-e", "--elim_comm"):
            ELIM_COMM=True
        elif opt in ("-g", "--code_generator"):
            GENOPT=arg
            if GENOPT not in GENS.keys():
                print 'ERROR: Wrong parameter is given for option code_generator, '+ arg
                print '       Exit program.'
                sys.exit(1)
        elif opt in ("-p", "--profiler"):
            PROFOPT=arg
            if PROFOPT not in PROFS.keys():
                print 'ERROR: Wrong parameter is given for option profiler, ' + arg
                print '       Exit program.'
                sys.exit(1)
        elif opt in ("-c", "--counter"):
            COUNTEROPT=arg
            if COUNTEROPT not in COUNTERS.keys():
                print 'ERROR: Wrong parameter is given for option counter, ' + arg
                print '       Exit program.'
                sys.exit(1)
        elif opt in ("-P", "--Proc"):
            NUMPROC=int(arg)
            if NUMPROC <= 0:
                print 'ERROR: Wrong parameter is given for the option Proc, ' + str(arg)
                print '       Exit program.'
                sys.exit(1)
        elif opt in ("-I", "--Iteration"):
            ITER=int(arg)

    if len(args) > 1:
        help()
        sys.exit(1)
    elif len(args) == 1:
        INFILE=args[0]
    
# parse command line arguments
getArgs(sys.argv[1:])

# parse input and build immediate representation
if INFILE == "":
    input = sys.stdin.read()
else:
    fin = open(INFILE, 'r')
    input = fin.read()
    fin.close()

char_stream = antlr3.ANTLRStringStream(input)
lexer = laminarLexer(char_stream)
tokens = antlr3.CommonTokenStream(lexer)
parser = laminarParser(tokens)
graph = parser.graph()


if graph <> None:

    code = ""

    # if parsing was successful generate code
    if not graph.has_error():

        if RESULT:
           if not os.path.isfile('times.out'):
               print 'ERROR: No times.out found.'
               print '       Exit program.'
               sys.exit(1)
           if PROFOPT == 'actor':
              graph.print_actor_exec_times('times.out',selectMedian);
           elif PROFOPT == 'thread' or PROFOPT == 'program':
              graph.print_proc_exec_times('times.out',selectMedian);
            
        else:
            # select scheduler
            s = SCHEDS[GENOPT](graph,NUMPROC)

            # compute repetitions & schedule
            if not graph.compute_repetitions():
               print 'ERROR: Failed to compute repetition vector.'
               sys.exit(1)

            if ELIM_COMM:
               # compute number of nodes
               (num_f, num_sj) = graph.get_nodes()
               print "# of filters: ",len(num_f)
               print "\t", num_f
               print "# of sjs:     ",len(num_sj)
               print "\t", num_sj

               # compute communications in bytes
               (total_comm, direct_comm, eliminated_comm) = graph.get_comm_in_byte()
               print "total communication     :",total_comm
               print "direct communication    :",direct_comm
               print "eliminated communication:",eliminated_comm
               print "cross check             :",(total_comm - direct_comm - eliminated_comm)

               # exit program
               sys.exit(0)

            # select performance counter
            c = COUNTERS[COUNTEROPT]()

            # select profiler
            p = PROFS[PROFOPT](c)

            # select prefetcher
            f = PREFETCHERS[PREFETCHEROPT]()

            # set code generator
            g = GENS[GENOPT](graph,s,p,f,ITER)

            code = g.gen()

            if OUTFILE == "":
                print code
            else:
                fout = open(OUTFILE, 'w')
                fout.write(code)
                fout.close()
