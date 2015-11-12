import os
import sys
import subprocess

##############################
#     Benchmark settings     #
##############################
# Set the paths to the benchamrks
PWD=os.path.dirname(os.path.abspath(__file__))
LAMINARIR_DIR=PWD+"/examples/laminarir"
STREAMIT_DIR=PWD+"/examples/streamit"
C_DIR=PWD+"/examples/c"
INPUT_DIR=PWD+"examples/input"
FRONTEND_DIR=PWD+"/frontend"
RESULT_DIR=PWD+"/results"

STREAMIT_TRANS_TYPE="instrumented"

# LOG accumulates all the commands executed
LOG='report.log'
# MAXPROC is the upper limit of number of processors for evaluation
MAXPROC=64

###############################
#       Helper functions      #
###############################

def PRINT(msg):
   """ Print a message on colsole and make a log.
   """
   print msg
   msg = msg.replace("'","\"")
   os.system('echo \''+msg+'\' >> '+LOG)

def ERROR(msg, ret=1):
   """ Print an error message on colsole and exit the program.
   """
   PRINT("ERROR: "+msg)
   sys.exit(ret)

def get(cmd, stdin=None):
   """ Execute a command and return the return code, output and 
       error message.
   """
   def timeout_handler(signum, frame):
      raise TimeoutException()
   try:
      p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
      out, err = p.communicate(stdin)
   except TimeoutException:
      ERROR('Timeout, '+cmd)
   return p.returncode, out, err

def run(cmd):
   """ Execute a command and make a log.
       stop program when execution occurs errors.
   """
   PRINT(cmd)
   retval=os.system(cmd)
   if not retval == 0:
      ERROR('Command "'+cmd+'" failed.')
