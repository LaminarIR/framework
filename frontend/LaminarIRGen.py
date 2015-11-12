import sys
import os
import subprocess
import getopt
import re
from collections import OrderedDict
sys.path.append("..")
from settings import *

#------------------------------------------------#
#       helper functions
#------------------------------------------------#
BENCH = ''
COMM_TYPE = 'direct'
INPUT = 'random'
SANITY_CHECK = False

COMM_TYPES=['direct','fifo']

def help():
   print 'Usage:'
   print ' LaminarIRGen.py <opts> <benchmark>'
   print '   -h|--help'
   print '      show this page.'
   print '   --laminar <direct|fifo>'
   print '     generate LaminarIR w/ direct token access (direct) or w/ FIFO queues (fifo)'
   print '   --random-input | --static-input'
   print '      use random or static inputs (default: --'+INPUT+'-input)'
   print '   --sanity-check'
   print '      print each item token reached the sink node on console (default: '+str(SANITY_CHECK)+')'

def getArgs(argv):
   """ Parses user input parameters
   """
   global BENCH
   global COMM_TYPE
   global INPUT
   global SANITY_CHECK

   try:
      opts, args = getopt.getopt(argv, "hi:", ["help","laminar=","random-input","static-input","sanity-check"])
   except getopt.GetoptError:
      help()
      sys.exit(1)
  
   for opt, arg in opts:
      if opt in ('-h', '--help'):
         help()
         sys.exit()
      elif opt in ('--laminar'):
         COMM_TYPE = str(arg)
         if not COMM_TYPE in COMM_TYPES:
            ERROR('Invalid argument for --laminar, '+COMM_TYPE\
                    +' is given. Exiting.')
      elif opt in ('--random-input'):
         INPUT='random'
      elif opt in ('--static-input'):
         INPUT='static'
      elif opt in ('--sanity-check'):
         SANITY_CHECK=True

   if len(args) != 1:
      help()
      sys.exit(1)
   else:
      BENCH=args[0]

#------------------------------------------------#
#       functions to handle each part
#------------------------------------------------#
PWD=subprocess.check_output("cd ..; pwd",shell=True).rstrip()
STR_PATH=STREAMIT_DIR+"/strs"
CMP_PATH=FRONTEND_DIR+"/compile"

# parse command line arguments
getArgs(sys.argv[1:])

# generate files
run('rm -rf '+CMP_PATH)
run('mkdir '+CMP_PATH)

# add current dir to classpath if needed
ret,classpath,err=get('echo ${CLASSPATH}')
export_classpath=''
if not FRONTEND_DIR in classpath:
   export_classpath='export CLASSPATH='+FRONTEND_DIR+':${CLASSPATH};'

run(export_classpath+'make')

# copy streamit code to the working directory
run('cp '+STR_PATH+'/'+BENCH+'.str '+CMP_PATH+'/.')

# generate java code to obtain constant values and stream graph
run(export_classpath+'java streamit.frontend.ToJava '+CMP_PATH+'/'+BENCH+'.str --library --output '+CMP_PATH+'/'+BENCH+'.java')

# run the generated java code
run(export_classpath+'cd '+CMP_PATH+'; javac '+BENCH+'.java; java '+BENCH+' -i 1')

# set flags to generate LaminarIR
FLAGS='--laminar '+COMM_TYPE+' --graph_file '+CMP_PATH+"/"+BENCH+".edges.out --const_file "+CMP_PATH+"/"+BENCH+".const_vars.out --path_file "+CMP_PATH+"/"+BENCH+".filename.out"
STRNAME=BENCH
if INPUT == 'random':
   FLAGS=FLAGS+" --randomize"
   BENCH=BENCH+".rand"
if SANITY_CHECK:
   FLAGS=FLAGS+" --sanity-check"

# generate LaminarIR code
run(export_classpath+'java laminarir.frontend.ToLaminarIR '+CMP_PATH+'/'+STRNAME+'.str --output '+CMP_PATH+'/'+BENCH+'.sdf '+FLAGS)
