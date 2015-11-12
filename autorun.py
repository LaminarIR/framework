import sys
import re
import os
import commands
import getopt
import math
import subprocess
import string
from collections import OrderedDict

from settings import *

############################
#     Custom variables     #
############################
# SEED_FILE is used for random input evaluation
SEED_FILE='rts/seed.txt'

##############################
#     Benchmark settings     #
##############################
# BMARK_POSTFIX decides to read random input (`.rand') or static input (`')
BMARK_POSTFIX='.rand'

# BMARK = (benchmark id, (number of steady-state iter, max proc to test))
BMARKS=OrderedDict((\
        ("DCT",                 (10000000, MAXPROC)),\
        ("DES",                 (10000000, MAXPROC)),\
        ("FFT2",                (10000000, MAXPROC)),\
        ("MatrixMult",          (10000000, MAXPROC)),\
        ("AutoCor",             (10000000, MAXPROC)),\
        ("Lattice",             (10000000, MAXPROC)),\
        ("Serpent",             (10000000, MAXPROC)),\
        ("JPEGFeed",            (10000000, MAXPROC)),\
        ("BeamFormer",          (10000000, MAXPROC)),\
        ("ComparisonCounting",  (10000000, MAXPROC)),\
        ("RadixSort",           (10000000, MAXPROC))
        ))

# DEFAULT_ITER is the default steady-state iteration 
# for the bencharks which are not in the BMARKS.
DEFAULT_ITER=10000

# SVN_HATERS is a set of benchmarks that show drastically bad performance when GVN opt pass is applied.
# To rule out non LamianrIR related factors from performance, gvn pass is 
# not applied for evaluation to the benchmarks in GVN_HATERS.
GVN_HATERS=["DES","Serpent"]

# list of frontend implemented for LaminarIR
FRONTENDS=["streamit"]

###############################
#       Helper functions      #
###############################

# user command-line input related vars and functions
BENCH=""
LAMINAR_DIRECT=False
LAMINAR_FIFO=False
STREAMIT=False
COUNTER='papi_wrapper'
GENERATE_LAMINARIR=False
GENERATE_C_CODE=True
FRONTEND='streamit'
MESSAGE=''
LLVM_ANALYZER=False
OPT_TEST=False
SANITY_CHECK=False

STREAMIT_COMP_OPT=''
STREAMIT_LIB_CLUSTER=''

def help():
   print 'Usage:'
   print ' autorun.py <opts...> <benchmark>'
   print '   -l| --laminar <direct|fifo>      '
   print '   -s| --streamit                   '
   print '      run benchmarks by LaminarIR (direct) and/or FIFO queues (fifo) and/or StreamIt'
   print '      (default: --laminar direct)'
   print '   -c| --counter <papi_wrapper|rts> '
   print '      generate codes to run (default: '+str(COUNTER)+')'
   print '   --generate-c-code'
   print '      generate C code from LaminarIR (default: '+str(GENERATE_C_CODE)+')'
   print '   --generate-LaminarIR streamit' 
   print '      generate LaminarIR code from the designated language.'+\
                ' (default: '+str(GENERATE_LAMINARIR)+')'
   print '   --random-input | --static-input '
   print '      run benchmarks with random or static inputs (default: --random-input)'
   print '   -a| --llvm-analyzer              '
   print '      run llvm analyzer in addition (default: '+str(LLVM_ANALYZER)+')'
   print '   -O| --opt-test                  '
   print '      measure eexecution times of each steps of llvm optimization passes.'
   print '      (default: '+str(OPT_TEST)+')'
   print '   -S| --sanity-check'
   print '      print each item token reached the sink node on console.'

def getArgs(argv):
   """ Parses user input parameters
   """
   global BENCH
   global LAMINAR_DIRECT
   global LAMINAR_FIFO
   global STREAMIT
   global COUNTER
   global GENERATE_C_CODE
   global GENERATE_LAMINARIR
   global FRONTEND
   global MESSAGE
   global OPT_TEST
   global SANITY_CHECK
   global BMARK_POSTFIX
   global LLVM_ANALYZER

   try:
      opts, args = getopt.getopt(argv, "hl:sc:am:OS", ["laminar=","streamit","counter=","random-input","static-input","generate-c-code","generate-LaminarIR=","llvm-analyzer","message=","opt-test","sanity-check"])
   except getopt.GetoptError:
      help()
      sys.exit(1)
  
   for opt, arg in opts:
      if opt == '-h':
         help()
         sys.exit()
      elif opt in ("-l", "--laminar"):
         comm_type=str(arg)
         if comm_type == 'direct':
            LAMINAR_DIRECT=True
         if comm_type == 'fifo':
            LAMINAR_FIFO=True
         if not LAMINAR_DIRECT and not LAMINAR_FIFO:
            ERROR('Invalid argument for --laminar, '+comm_type \
                  + ' is given. Exiting.')
      elif opt in ("-s", "--streamit"):
         STREAMIT=True
      elif opt in ("-c", "--counter"):
         COUNTER=str(arg)
         if not COUNTER=='papi_wrapper' and not COUNTER=='rts':
            ERROR('Invalid argument for --counter, '+COUNTER \
                  + ' is given. Exiting.')
      elif opt in ("--generate-c-code"):
         GENERATE_C_CODE=True
      elif opt in ("--generate-LaminarIR"):
         GENERATE_LAMINARIR=True
         FRONTEND=str(arg)
         if FRONTEND not in FRONTENDS:
            ERROR('Invalid argument for --generate-LaminarIR, '+FRONTEND \
                  + ' is given. Exiting.')
      elif opt in ("-a", "--llvm-analyzer"):
         LLVM_ANALYZER=True
      elif opt in ("--random-input"):
         BMARK_POSTFIX='.rand'
      elif opt in ("--static-input"):
         BMARK_POSTFIX=''
      elif opt in ("-O", "--opt-test"):
         OPT_TEST=True
      elif opt in ("-S", "--sanity-check"):
         SANITY_CHECK=True
      elif opt in ("-m", "--message"):
         MESSAGE=str(arg)

   if not LAMINAR_DIRECT and not LAMINAR_FIFO and not STREAMIT:
      LAMINAR_DIRECT=True

   if len(args) > 1:
      help()
      sys.exit(1)
   elif len(args) == 1:
      BENCH=args[0]


first_convert_csv=True
def convert_csv(bmark, papi_results, filename):
   """ Convert filename into csv and accumulate on papi_results
   """
   global first_convert_csv
   outfile = open(filename)
   if not bmark in papi_results:
      papi_results[bmark]=[]
   for line in outfile:
      tokens = line.split()
      if len(tokens) < 2:
         continue
      if 'second' in tokens[0]:
         papi_results[bmark].append(str(float(tokens[3].strip())/1000))
         if first_convert_csv:
            papi_results[MESSAGE].append('TIME(second)')
      if 'INSTRUCTIONS' in tokens[0]:
         papi_results[bmark].append(tokens[3].strip())
         if first_convert_csv:
            papi_results[MESSAGE].append('INSTRUCTIONS')
      if 'CPI' in tokens[0]:
         papi_results[bmark].append(str(float(tokens[3].strip())/1000))
         if first_convert_csv:
            papi_results[MESSAGE].append('CPI')
      if 'LOADS' in tokens[0]:
         papi_results[bmark].append(tokens[3].strip())
         if first_convert_csv:
            papi_results[MESSAGE].append('LOADS')
      if 'STORES' in tokens[0]:
         papi_results[bmark].append(tokens[3].strip())
         if first_convert_csv:
            papi_results[MESSAGE].append('STORES')
      if 'ICACHE:MISSES' in tokens[0]:
         papi_results[bmark].append(tokens[3].strip())
         if first_convert_csv:
            papi_results[MESSAGE].append('ICACHE:MISSES')
      if 'PACKAGE_ENERGY:PACKAGE0' in tokens[0]:
         papi_results[bmark].append(tokens[1].strip())
         if first_convert_csv:
            papi_results[MESSAGE].append('PACKAGE_ENERGY:PACKAGE0')
      if 'PP0_ENERGY:PACKAGE0' in tokens[0]:
         papi_results[bmark].append(tokens[1].strip())
         if first_convert_csv:
            papi_results[MESSAGE].append('PP0_ENERGY:PACKAGE0')
      if 'DRAM_ENERGY:PACKAGE0' in tokens[0]:
         papi_results[bmark].append(tokens[1].strip())
         if first_convert_csv:
            papi_results[MESSAGE].append('DRAM_ENERGY:PACKAGE0')
   first_convert_csv = False
   return papi_results

def write_csv (dict,filename):
   DEL=','
   f = open(filename,'a')
   for b in dict.keys():
      f.write(b+DEL)
      for v in dict[b]:
         f.write(v+DEL)
      f.write('\n')

def frontend_run_command(FRONTEND, comm_type, bmark):
    """ Generate LaminarIR code for FRONTEND
    """
    # command='(cd '+bmarkDIR+'; make -f Makefile.cluster clean)'
    bmarkname=''
    command='(cd '+FRONTEND_DIR+'; python LaminarIRGen.py '
    if BMARK_POSTFIX == '':
        command=command+'--static-input '
        bmarkname = bmark
    else:
        command=command+'--random-input '
        bmarkname = re.sub(BMARK_POSTFIX,'',bmark)

    if SANITY_CHECK:
        command=command+'--sanity-check '

    command=command+'--laminar '+comm_type+' '

    command=command+bmarkname+')'
    return command

def parse_clang_arguments(opt=''):
   """ Parse llvm optimization passes of the given compile option 
       (e.g., -O3) in order
   """
   command='llvm-as </dev/null | opt '+opt+' -disable-output -debug-pass=Arguments'
   output=commands.getoutput(command)
   first_mods=output.split('Pass Arguments: ')[1].split()
   second_mods=output.split('Pass Arguments: ')[2].split()
   return (first_mods, second_mods)

def laminar_seq_code_gen_command(bmark, comm_type, iter, opt=''):
   """ Retuern a python command to generate sequential C code from 
       the LaminarIR
   """
   command="python laminar.py -g serial-"+comm_type+" -P 1 -I "+str(iter)+\
           " -o "+bmark+".seq.c "+opt+" "+BENCH_DIR+"/"+bmark+".sdf"
   return command    

def laminar_comp_command(file, comp_opt=''):
   """ Return a command to compile generated C codes
   """
   command='make '+file+' '+comp_opt
   return command    

def laminar_llvmir_command(file, comp_opt=''):
   """ Return a command to generate llvmIR from C codes
   """
   command='make '+file+'.ll '+comp_opt
   return command    

def laminar_run_command(bin,run_opt):
   """ Return a command to execute a binary compiled from C code that
       is generated by LaminarIR. 
   """
   ret,out,error=get('hostname')
   HOSTNAME=out.strip() 
   if HOSTNAME == 'xeonphi':
      run('scp '+bin+' mic0:.')
      command='ssh mic0 \'export LD_LIBRARY_PATH=.:/tmp/likwid_xeon_phi/lib:/tmp/intel/lib/mic; '+run_opt+' ./'+bin+'<'+SEED_FILE+'\';scp mic0:times0.out .'
   else:
       command=run_opt+"./"+bin+'<'+SEED_FILE
   return command    

def streamit_clean_command(bmarkDIR):
   """ Return a command to cleanup the working directory of the StreamIt
   """
   command='(cd '+bmarkDIR+'; make -f Makefile.cluster clean)'
   return command

def streamit_llvmir_command(bmarkDIR, file):
   """ Return a command to generate llvmIR from cpp codes
   """
   command='(cd '+bmarkDIR+'; make '+file+'.ll -f Makefile.cluster '\
           +'CCFLAGS=\''+STREAMIT_COMP_OPT+'\' LIB_CLUSTER=\''+STREAMIT_LIB_CLUSTER+'\')'
   return command

def streamit_comp_command(bmarkDIR, file):
   """ Return a command to compile cpp codes of StreamIt
   """
   command='(cd '+bmarkDIR+'; make '+file+' -f Makefile.cluster '\
           +'CCFLAGS=\''+STREAMIT_COMP_OPT+'\' LIB_CLUSTER=\''+STREAMIT_LIB_CLUSTER+'\')'
   return command

def streamit_run_command(bmarkDIR, run_opt, bin, iter):
   """ Return a command to execute a binary compiled from cpp codes
       that is generated by StreamIt
   """
   ret,out,error=get('hostname')
   HOSTNAME=out.strip() 
   if HOSTNAME == 'xeonphi':
      run('scp '+bmarkDIR+'/'+bin+' mic0:.')
      command='ssh mic0 \'export LD_LIBRARY_PATH=.:/tmp/likwid_xeon_phi/lib:/tmp/intel/lib/mic; '+run_opt+' ./'+bin+' -i '+str(iter)+' <'+SEED_FILE+'\';scp mic0:times0.out '+bmarkDIR+'/.'
   else:
      command='(cd '+bmarkDIR+';time -p '+run_opt+' ./'+bin+' -i '+str(iter)+' <../../../../'+SEED_FILE+')'
   return command
   
 
#####################
#       Main        #
#####################
# parse command line arguments
getArgs(sys.argv[1:])

# set the list of benchmarks to test
# test all benchmarks if no benchmark specified
if BENCH == "":
   runList=BMARKS.keys()
   for i in range(len(BMARKS)):
       runList
else:
   runList=[BENCH]
   if not BENCH in BMARKS.keys():
      BMARKS[BENCH]=(DEFAULT_ITER,MAXPROC)

# set the list of testing sets to run
EXPS=[]
if LAMINAR_DIRECT:
   EXPS.append(('laminar','direct'))
if LAMINAR_FIFO:
   EXPS.append(('laminar','fifo'))
if STREAMIT:
   EXPS.append(('streamit',''))

# iterate experiments over testing sets
for exp in EXPS:
   (codegen, comm_type)=exp
   CODE_REPOS=PWD+"/examples/c/"+comm_type
   if comm_type == '':
      TARGET_DIR=RESULT_DIR+'/'+codegen
   else:
      TARGET_DIR=RESULT_DIR+'/'+codegen+'_'+comm_type

   # configure the framework
   run('./configure --'+codegen)

   # check if c compiler is clang/clang++
   ret,out,error=get('make print-CC')
   compiler=out.strip() 
   if compiler not in ['clang', 'clang++']:
      ERROR('Only clang/clang++ is supported for this distribution.')

   # initialize testing environment
   ret,out,error=get('make print-RUN_OPT')
   run_opt=out.strip()+' '
   run('rm -rf '+TARGET_DIR)
   run('mkdir -p '+TARGET_DIR)
   papi_results=OrderedDict(())
   if MESSAGE not in papi_results:
      papi_results[MESSAGE]=[]
   if comm_type != '':
      papi_results[MESSAGE].append(codegen+'_'+comm_type)
   else:
      papi_results[MESSAGE].append(codegen)

   # iterate experiments over benchmarks
   for bmark in runList:
      (iter,max_p)=BMARKS[bmark]
      bmark=bmark+BMARK_POSTFIX


      #################### LAMINARIR / FIFO QUEUES ####################
      if codegen == 'laminar':
         PRINT('------------------------------------------------------')
         PRINT('Run laminar ( ' +comm_type+' ) '+bmark+' [ ITER ='+str(iter)+']')
         PRINT('------------------------------------------------------')

         # initialize testing environment
         seq_proc_file=bmark
         run('rm -f times0.out')

         # generate LaminarIR code from streamit from FRONTEND when GENERATE_LAMINARIR flag is set.
         if GENERATE_LAMINARIR:
            run(frontend_run_command(FRONTEND, comm_type, bmark))
            run('mv '+FRONTEND_DIR+'/compile/'+bmark+'.sdf .')
            BENCH_DIR=PWD
         # read corresponding .sdf file from BENCH_DIR, otherwise.
         else:
            BENCH_DIR=LAMINARIR_DIR+'/'+comm_type

         # generate C code from LaminarIR when GENERATE_C_CODE flag is set.
         if GENERATE_C_CODE:
            run(laminar_seq_code_gen_command(bmark, comm_type, iter,'-c '+COUNTER+' -p program'))
            run('mv '+bmark+'.seq.c '+seq_proc_file+'.c')
         # read corresponding .c file from CODE_REPOS, otherwise.
         else:
            run('cp '+CODE_REPOS+'/'+seq_proc_file+'.c .')

         ret,out,error=get('make print-CFLAGS')
         comp_opt='CFLAGS=\''+out.strip()+' -DITER='+str(iter)+'\''
  
         (first_mods, second_mods) = parse_clang_arguments('-O3')
         PRINT('* ORIGINAL first modules called: '+' '.join(first_mods)+'\n')
         PRINT('* ORIGINAL second modules called: '+' '.join(second_mods)+'\n')
         O3_mods=first_mods+second_mods

         # remove -gvn from the O3_mods when the benchmark hates it
         if comm_type == 'direct':
            if bmark in [b+BMARK_POSTFIX for b in GVN_HATERS]:
               O3_mods=[m for m in O3_mods if m != '-gvn']
               O3_mods=[m for m in O3_mods if m != '-verify-di']

         ret,out,error=get('make print-LLC_OPT')
         llc_opt=out.strip()

         # generate llvm ir -O0 (only if CC=clang)
         O0_file=bmark+'.'+comm_type+'.O0'
         comp_O0_opt=comp_opt.replace('O3','O0')
         run(laminar_llvmir_command(seq_proc_file,comp_O0_opt))
         run('mv '+seq_proc_file+'.ll '+O0_file+'.ll')
         run('llc '+llc_opt+' '+O0_file+'.ll -stats 2> '+O0_file+'.llc.stats')

         # generate llvm ir -O3 flags
         O3_flag_file=bmark+'.'+comm_type+'.O3'
         run('opt '+' '.join(O3_mods)+' '+O0_file+'.ll -S -o '+O3_flag_file+'.ll')
         if LLVM_ANALYZER:
            run('opt '+' '.join(O3_mods)+' '+O0_file+'.ll -analyze -stats -o '+O3_flag_file+'.analyze 2> '+O3_flag_file+'.stats')
         run('llc '+llc_opt+' '+O3_flag_file+'.ll -stats 2> '+O3_flag_file+'.llc.stats')
         run(laminar_comp_command(O3_flag_file, comp_opt))
         run(laminar_run_command(O3_flag_file, run_opt))
         papi_results=convert_csv(O3_flag_file, papi_results, 'times0.out')
         run('rm -rf times0.out')

         if OPT_TEST: 
            new_ll = ''
            old_ll = ''
            ctr=0
            for mod in O3_mods:
               PRINT('   ---------------------------------------')
               PRINT('   * Generating code for '+mod+'...')
               PRINT('   ---------------------------------------')
               new_ll=bmark+'.'+str(ctr)+mod
               ctr=ctr+1
               test_mods=' '.join(O3_mods[:ctr])
               run('opt '+test_mods+' '+O0_file+'.ll -S -o '+new_ll+'.ll')
               if LLVM_ANALYZER:
                  run('opt '+test_mods+' '+O0_file+'.ll -S -analyze -stats ' +'-o '+new_ll+'.analyze 2> '+new_ll+'.stats')
               run('llc '+llc_opt+' '+new_ll+'.ll -stats 2> '+new_ll+'.llc.stats')
               run(laminar_comp_command(new_ll, comp_opt))
               run(laminar_run_command(new_ll, run_opt))
               papi_results=convert_csv(new_ll, papi_results, 'times0.out')
               if LLVM_ANALYZER:
                  run('mv '+new_ll+'.stats '+TARGET_DIR)
                  run('mv '+new_ll+'.analyze '+TARGET_DIR)
               run('mv '+new_ll+'.llc.stats '+TARGET_DIR)
               run('mv '+new_ll+'.s '+TARGET_DIR)
               run('mv '+new_ll+' '+TARGET_DIR)
               run('rm -rf times0.out')
               run('mv '+new_ll+'.ll '+TARGET_DIR)
         run('mv '+O0_file+'.ll '+TARGET_DIR+'/.')
         run('mv '+O0_file+'.llc.stats '+TARGET_DIR+'/.')
         run('mv '+O0_file+'.s '+TARGET_DIR+'/.')
         run('mv '+O3_flag_file+'.ll '+TARGET_DIR+'/.')
         if LLVM_ANALYZER:
            run('mv '+O3_flag_file+'.stats '+TARGET_DIR+'/.')
            run('mv '+O3_flag_file+'.analyze '+TARGET_DIR+'/.')
         run('mv '+O3_flag_file+'.llc.stats '+TARGET_DIR+'/.')
         run('mv '+O3_flag_file+'.s '+TARGET_DIR+'/.')
         run('mv '+O3_flag_file+' '+TARGET_DIR+'/.')

         # backup data
         if GENERATE_C_CODE:
            run('mv '+seq_proc_file+'.c '+TARGET_DIR)
         else:
            run('rm -rf '+seq_proc_file+'.c')
         
         if GENERATE_LAMINARIR:
            run('mv '+bmark+'.sdf '+TARGET_DIR)

      #################### STREAMIT ####################
      elif codegen == 'streamit':
         PRINT('------------------------------------------------------')
         PRINT('Run streamit '+bmark+' [ ITER ='+str(iter)+']')
         PRINT('------------------------------------------------------')
         bmarkDIR=STREAMIT_DIR+'/'+STREAMIT_TRANS_TYPE+'/'+bmark+'_default'
         ret,out,error=get('make print-CCFLAGS')
         STREAMIT_COMP_OPT=out.strip() 
         ret,out,error=get('make print-CFLAGS')
         STREAMIT_COMP_OPT=STREAMIT_COMP_OPT+' '+out.strip()
         if COUNTER == 'rts':
            STREAMIT_COMP_OPT=STREAMIT_COMP_OPT+' -DMEASURE_BY_RTS'
         ret,out,error=get('make print-STREAMIT_LIB_CLUSTER')
         STREAMIT_LIB_CLUSTER=out.strip()
         ret,out,error=get('make print-LLC_OPT')
         llc_opt=out.strip()
             
         orig_file='combined_threads'
         O0_file=bmark+'.streamit.O0'
         O3_flag_file=bmark+'.streamit.O3'
          
         # use ready-made Makefile.cluster if there is none
         if not os.path.isfile(bmarkDIR+'/Makefile.cluster'):
            run('cp '+STREAMIT_DIR+'/Makefile.cluster '+bmarkDIR+'/.')
         run(streamit_clean_command(bmarkDIR))

         (first_mods, second_mods) = parse_clang_arguments('-O3')
         PRINT('* ORIGINAL first modules called: '+' '.join(first_mods)+'\n')
         PRINT('* ORIGINAL second modules called: '+' '.join(second_mods)+'\n')
         O3_mods=first_mods+second_mods

         # generate llvm ir -O0
         STREAMIT_COMP_OPT=STREAMIT_COMP_OPT.replace('O3','O0')
         run(streamit_llvmir_command(bmarkDIR, orig_file))
         run('mv '+bmarkDIR+'/'+orig_file+'.ll '+bmarkDIR+'/'+O0_file+'.ll')
         run('llc '+llc_opt+' '+bmarkDIR+'/'+O0_file+'.ll -stats -o '\
                 +bmarkDIR+'/'+O0_file+'.s 2> '+O0_file+'.llc.stats')
         STREAMIT_COMP_OPT=STREAMIT_COMP_OPT.replace('O0','O3')

         # generate llvm ir -O3 flags
         run('cp '+bmarkDIR+'/'+O0_file+'.ll .')
         run('opt '+' '.join(O3_mods)+' '+O0_file+'.ll -S -o '+bmarkDIR+'/'+O3_flag_file+'.ll')
         if LLVM_ANALYZER:
            run('opt '+' '.join(O3_mods)+' '+O0_file+'.ll -analyze -stats -o '+bmarkDIR+'/'+O3_flag_file+'.analyze 2> '+bmarkDIR+'/'+O3_flag_file+'.stats')
         run('llc '+llc_opt+' '+bmarkDIR+'/'+O3_flag_file+'.ll -stats -o '\
                 +bmarkDIR+'/'+O3_flag_file+'.s 2> '+O3_flag_file+'.llc.stats')
         run(streamit_comp_command(bmarkDIR, O3_flag_file))
         ret,out,error=get(streamit_run_command(bmarkDIR, run_opt, O3_flag_file, iter))
         papi_results=convert_csv(O3_flag_file, papi_results, bmarkDIR+'/times0.out')
         run('rm -rf '+bmarkDIR+'/times0.out')

         if OPT_TEST:
            new_ll = ''
            old_ll = ''
            ctr=0
            
            run('cp '+bmarkDIR+'/'+O0_file+'.ll .')
            for mod in O3_mods:
               PRINT('   ---------------------------------------')
               PRINT('   * Generating code for '+mod+'...')
               PRINT('   ---------------------------------------')
               new_ll=bmark+'.'+str(ctr)+mod
               ctr=ctr+1
               test_mods=' '.join(O3_mods[:ctr])
               run('opt '+test_mods+' '+O0_file+'.ll -S -o '+new_ll+'.ll')
               if LLVM_ANALYZER:
                  run('opt '+test_mods+' '+O0_file+'.ll -S -analyze -stats '+'-o '+new_ll+'.analyze 2> '+new_ll+'.stats')
               run('llc '+llc_opt+' '+new_ll+'.ll -stats 2> '+new_ll+'.llc.stats')
               run('mv '+new_ll+'.s '+bmarkDIR)
               run(streamit_comp_command(bmarkDIR, new_ll))
               ret,out,error=get(streamit_run_command(bmarkDIR, run_opt, new_ll, iter))
               papi_results=convert_csv(new_ll, papi_results, bmarkDIR+'/times0.out')
               run('rm -rf '+bmarkDIR+'/times0.out')
               if LLVM_ANALYZER:
                  run('mv '+new_ll+'.analyze '+TARGET_DIR)
               run('mv '+new_ll+'.stats '+TARGET_DIR)
               run('mv '+new_ll+'.llc.stats '+TARGET_DIR)
               run('mv '+bmarkDIR+'/'+new_ll+' '+TARGET_DIR)
               run('mv '+bmarkDIR+'/'+new_ll+'.s '+TARGET_DIR)
               run('mv '+new_ll+'.ll '+TARGET_DIR)
         run('mv '+bmarkDIR+'/'+O0_file+'.ll '+TARGET_DIR+'/.')
         run('mv '+O0_file+'.llc.stats '+TARGET_DIR+'/.')
         run('mv '+bmarkDIR+'/'+O0_file+'.s '+TARGET_DIR+'/.')
         run('mv '+bmarkDIR+'/'+O3_flag_file+'.ll '+TARGET_DIR+'/.')
         if LLVM_ANALYZER:
            run('mv '+bmarkDIR+'/'+O3_flag_file+'.stats '+TARGET_DIR+'/.')
            run('mv '+bmarkDIR+'/'+O3_flag_file+'.analyze '+TARGET_DIR+'/.')
         run('mv '+O3_flag_file+'.llc.stats '+TARGET_DIR+'/.')
         run('mv '+bmarkDIR+'/'+O3_flag_file+'.s '+TARGET_DIR+'/.')
         run('mv '+bmarkDIR+'/'+O3_flag_file+' '+TARGET_DIR+'/.')
         run('rm '+O0_file+'.ll')
   write_csv(papi_results, 'papi_events.csv')
