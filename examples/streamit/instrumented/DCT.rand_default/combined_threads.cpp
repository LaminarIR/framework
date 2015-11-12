#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <netsocket.h>
#include <node_server.h>
#include <init_instance.h>
#include <master_server.h>
#include <save_state.h>
#include <save_manager.h>
#include <delete_chkpts.h>
#include <object_write_buffer.h>
#include <read_setup.h>
#include <ccp.h>
#include <timer.h>
#include "fusion.h"
#include "structs.h"

#ifdef MEASURE_BY_RTS
#include "../../../../rts/rts.h"
struct RTS_VARS* thr_vars __attribute__((aligned(CACHELINE_SIZE)));
#else //default ``MEASURE_BY_PAPI''
#include "../../../../papi_wrapper/papi_hw_counter.h"
struct PAPI_VARS* thr_vars __attribute__((aligned(CACHELINE_SIZE)));
FILE *summary_f;
#endif
int numcpus __attribute__((aligned(CACHELINE_SIZE)));
int numiters __attribute__((aligned(CACHELINE_SIZE)));


#define LOC_RAND_MAX 16

volatile long long sum = 0;
static unsigned int seed;

int __max_iteration;
int __timer_enabled = 0;
int __frequency_of_chkpts;
volatile int __vol;
proc_timer tt("total runtime");


int BUFFER_0_1[__BUF_SIZE_MASK_0_1 + 1];
int HEAD_0_1 = 0;
int TAIL_0_1 = 0;
int BUFFER_1_2[__BUF_SIZE_MASK_1_2 + 1];
int HEAD_1_2 = 0;
int TAIL_1_2 = 0;
void init_Source__3_74__0();
void work_Source__3_74__0(int);
#ifdef BUFFER_MERGE
void work_Source__3_74__0__mod(int ____n, void *____in, int *____out);
void work_Source__3_74__0__mod2(int ____n, void *____in, int *____out, int s1, int s2);
#endif
void init_iDCT_2D_reference_coarse__15_76__1();
void work_iDCT_2D_reference_coarse__15_76__1(int);
#ifdef BUFFER_MERGE
void work_iDCT_2D_reference_coarse__15_76__1__mod(int ____n, int *____in, int *____out);
void work_iDCT_2D_reference_coarse__15_76__1__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_FloatPrinter__19_77__2();
void work_FloatPrinter__19_77__2(int);
#ifdef BUFFER_MERGE
void work_FloatPrinter__19_77__2__mod(int ____n, int *____in, void *____out);
void work_FloatPrinter__19_77__2__mod2(int ____n, int *____in, void *____out, int s1, int s2);
#endif

int main(int argc, char **argv) {
  read_setup::read_setup_file();
  __max_iteration = read_setup::max_iteration;
  for (int a = 1; a < argc; a++) {
    if (argc > a + 1 && strcmp(argv[a], "-i") == 0) {
      int tmp;
      sscanf(argv[a + 1], "%d", &tmp);
#ifdef VERBOSE
      fprintf(stderr,"Number of Iterations: %d\n", tmp);
#endif
      __max_iteration = tmp;
    }
    if (strcmp(argv[a], "-t") == 0) {
#ifdef VERBOSE
       fprintf(stderr,"Timer enabled.\n");
#endif
       __timer_enabled = 1;    }
  }
// number of phases: 3

#ifdef MEASURE_BY_RTS
RTS_init(1,1);
#else //default ``MEASURE_BY_PAPI''
PAPI_HW_COUNTER_init(1,1);
#endif
  // ============= Initialization =============

init_Source__3_74__0();
init_iDCT_2D_reference_coarse__15_76__1();
init_FloatPrinter__19_77__2();

#ifdef MEASURE_BY_RTS
RTS_thread_init(0,1);
#else //default ``MEASURE_BY_PAPI''
PAPI_HW_COUNTER_thread_init(0,1);
PAPI_HW_COUNTER_open(0);
#endif

  // ============= Steady State =============

  if (__timer_enabled) {
    tt.start();
  }
#ifdef MEASURE_BY_RTS
  RTS_start_timer(0,0);
#else //default ``MEASURE_BY_PAPI''
 PAPI_HW_COUNTER_on(0,0);
#endif
  for (int n = 0; n < (__max_iteration  ); n++) {
HEAD_0_1 = 0;
TAIL_0_1 = 0;
    work_Source__3_74__0(1 );
HEAD_1_2 = 0;
TAIL_1_2 = 0;
    work_iDCT_2D_reference_coarse__15_76__1(1 );
    work_FloatPrinter__19_77__2(1 );
  }
#ifdef MEASURE_BY_RTS
  RTS_stop_timer(0,0);
  RTS_record("PROGRAM",0,0,0);
  RTS_write_time(0);
#else //default ``MEASURE_BY_PAPI''
  PAPI_HW_COUNTER_off(0,0);
  PAPI_HW_COUNTER_record("PROGRAM",0,0,0);
  PAPI_HW_COUNTER_write(0);
  PAPI_HW_COUNTER_close(0);
  PAPI_HW_COUNTER_write_summary();
#endif


if (__timer_enabled) {
    tt.stop();
    tt.output(stderr);
  }

  printf("%lld\n", sum);
  return 0;
}

// moved or inserted by concat_cluster_threads.pl
#include <message.h>
message *__msg_stack_1;
message *__msg_stack_0;
message *__msg_stack_2;

// end of moved or inserted by concat_cluster_threads.pl

// peek: 0 pop: 0 push 64
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false


#include <mysocket.h>
#include <sdep.h>
#include <thread_info.h>
#include <consumer2.h>
#include <consumer2p.h>
#include <producer2.h>
#include "cluster.h"
#include "global.h"

int __number_of_iterations_0;
int __counter_0 = 0;
int __steady_0 = 0;
int __tmp_0 = 0;
int __tmp2_0 = 0;
int *__state_flag_0 = NULL;
thread_info *__thread_0 = NULL;



void save_file_pointer__0(object_write_buffer *buf);
void load_file_pointer__0(object_write_buffer *buf);

 
void init_Source__3_74__0();
inline void check_status__0();

void work_Source__3_74__0(int);



inline void __push__0(int data) {
BUFFER_0_1[HEAD_0_1]=data;
HEAD_0_1++;
}



 
void init_Source__3_74__0(){
    printf("Enter seed: ");
    scanf("%d", &seed);
    srand(seed);
}
void save_file_pointer__0(object_write_buffer *buf) {}
void load_file_pointer__0(object_write_buffer *buf) {}
 
void work_Source__3_74__0(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int i__2 = 0;/* int */

      // mark begin: SIRFilter Source

      for ((i__2 = 0)/*int*/; (i__2 < 64); (i__2++)) {{
          __push__0(rand_r(&seed));
        }
      }
      // mark end: SIRFilter Source

    }
  }
}

// peek: 64 pop: 64 push 64
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_1;
int __counter_1 = 0;
int __steady_1 = 0;
int __tmp_1 = 0;
int __tmp2_1 = 0;
int *__state_flag_1 = NULL;
thread_info *__thread_1 = NULL;



float coeff__4__1[8][8] = {0};
void save_peek_buffer__1(object_write_buffer *buf);
void load_peek_buffer__1(object_write_buffer *buf);
void save_file_pointer__1(object_write_buffer *buf);
void load_file_pointer__1(object_write_buffer *buf);

 
void init_iDCT_2D_reference_coarse__15_76__1();
inline void check_status__1();

void work_iDCT_2D_reference_coarse__15_76__1(int);


inline int __pop__1() {
int res=BUFFER_0_1[TAIL_0_1];
TAIL_0_1++;
return res;
}

inline int __pop__1(int n) {
int res=BUFFER_0_1[TAIL_0_1];
TAIL_0_1+=n;

return res;
}

inline int __peek__1(int offs) {
return BUFFER_0_1[TAIL_0_1+offs];
}



inline void __push__1(int data) {
BUFFER_1_2[HEAD_1_2]=data;
HEAD_1_2++;
}



 
void init_iDCT_2D_reference_coarse__15_76__1(){
  float scale__12 = 0.0f;/* float */
  int time__13 = 0;/* int */
  int freq__14 = 0;/* int */

  for ((freq__14 = 0)/*int*/; (freq__14 < 8); (freq__14++)) {{
      (scale__12 = ((freq__14 == 0) ? ((float)0.35355338) : ((float)0.5)))/*float*/;
      for ((time__13 = 0)/*int*/; (time__13 < 8); (time__13++)) {{
          ((((coeff__4__1)[(int)freq__14])[(int)time__13]) = (scale__12 * ((float)(cosf(((double)(((((float)0.3926991) * ((float)(freq__14))) * (((float)(time__13)) + ((float)0.5))))))))))/*float*/;
        }
      }
    }
  }
}
void save_file_pointer__1(object_write_buffer *buf) {}
void load_file_pointer__1(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_iDCT_2D_reference_coarse__15_76__1__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  float block_x__7[8][8] = {0};/* float[8][8] */
  int i__8 = 0;/* int */int j__9 = 0;/* int */int k__10 = 0;/* int */
  float block_y__11 = 0.0f;/* float */

  // mark begin: SIRFilter iDCT_2D_reference_coarse

  for ((i__8 = 0)/*int*/; (i__8 < 8); (i__8++)) {{
      for ((j__9 = 0)/*int*/; (j__9 < 8); (j__9++)) {{
          (((block_x__7[(int)i__8])[(int)j__9]) = ((float)0.0))/*float*/;
          for ((k__10 = 0)/*int*/; (k__10 < 8); (k__10++)) {{
              (((block_x__7[(int)i__8])[(int)j__9]) = (((block_x__7[(int)i__8])[(int)j__9]) + ((((coeff__4__1)[(int)k__10])[(int)j__9]) * ((float)((*(____in+((8 * i__8) + k__10))))))))/*float*/;
            }
          }
        }
      }
    }
  }
  for ((i__8 = 0)/*int*/; (i__8 < 8); (i__8++)) {{
      for ((j__9 = 0)/*int*/; (j__9 < 8); (j__9++)) {{
          (block_y__11 = ((float)0.0))/*float*/;
          for ((k__10 = 0)/*int*/; (k__10 < 8); (k__10++)) {{
              (block_y__11 = (block_y__11 + ((((coeff__4__1)[(int)k__10])[(int)i__8]) * ((block_x__7[(int)k__10])[(int)j__9]))))/*float*/;
            }
          }
          (block_y__11 = ((float)(floorf(((double)((block_y__11 + ((float)0.5))))))))/*float*/;
          ((*____out++)=((int)(block_y__11)));
        }
      }
    }
  }
  for ((i__8 = 0)/*int*/; (i__8 < 64); (i__8++)) {{
      (*____in++);
    }
  }
  // mark end: SIRFilter iDCT_2D_reference_coarse

}}


void work_iDCT_2D_reference_coarse__15_76__1__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float block_x__7[8][8] = {0};/* float[8][8] */
  int i__8 = 0;/* int */int j__9 = 0;/* int */int k__10 = 0;/* int */
  float block_y__11 = 0.0f;/* float */

  // mark begin: SIRFilter iDCT_2D_reference_coarse

  for ((i__8 = 0)/*int*/; (i__8 < 8); (i__8++)) {{
      for ((j__9 = 0)/*int*/; (j__9 < 8); (j__9++)) {{
          (((block_x__7[(int)i__8])[(int)j__9]) = ((float)0.0))/*float*/;
          for ((k__10 = 0)/*int*/; (k__10 < 8); (k__10++)) {{
              (((block_x__7[(int)i__8])[(int)j__9]) = (((block_x__7[(int)i__8])[(int)j__9]) + ((((coeff__4__1)[(int)k__10])[(int)j__9]) * ((float)((*(____in+((8 * i__8) + k__10))))))))/*float*/;
            }
          }
        }
      }
    }
  }
  for ((i__8 = 0)/*int*/; (i__8 < 8); (i__8++)) {{
      for ((j__9 = 0)/*int*/; (j__9 < 8); (j__9++)) {{
          (block_y__11 = ((float)0.0))/*float*/;
          for ((k__10 = 0)/*int*/; (k__10 < 8); (k__10++)) {{
              (block_y__11 = (block_y__11 + ((((coeff__4__1)[(int)k__10])[(int)i__8]) * ((block_x__7[(int)k__10])[(int)j__9]))))/*float*/;
            }
          }
          (block_y__11 = ((float)(floorf(((double)((block_y__11 + ((float)0.5))))))))/*float*/;
          ((*____out++)=((int)(block_y__11)));
        }
      }
    }
  }
  for ((i__8 = 0)/*int*/; (i__8 < 64); (i__8++)) {{
      (*____in++);
    }
  }
  // mark end: SIRFilter iDCT_2D_reference_coarse

}}


#endif // BUFFER_MERGE


 
void work_iDCT_2D_reference_coarse__15_76__1(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float block_x__7[8][8] = {0};/* float[8][8] */
      int i__8 = 0;/* int */int j__9 = 0;/* int */int k__10 = 0;/* int */
      float block_y__11 = 0.0f;/* float */

      // mark begin: SIRFilter iDCT_2D_reference_coarse

      for ((i__8 = 0)/*int*/; (i__8 < 8); (i__8++)) {{
          for ((j__9 = 0)/*int*/; (j__9 < 8); (j__9++)) {{
              (((block_x__7[(int)i__8])[(int)j__9]) = ((float)0.0))/*float*/;
              for ((k__10 = 0)/*int*/; (k__10 < 8); (k__10++)) {{
                  (((block_x__7[(int)i__8])[(int)j__9]) = (((block_x__7[(int)i__8])[(int)j__9]) + ((((coeff__4__1)[(int)k__10])[(int)j__9]) * ((float)(__peek__1(((8 * i__8) + k__10)))))))/*float*/;
                }
              }
            }
          }
        }
      }
      for ((i__8 = 0)/*int*/; (i__8 < 8); (i__8++)) {{
          for ((j__9 = 0)/*int*/; (j__9 < 8); (j__9++)) {{
              (block_y__11 = ((float)0.0))/*float*/;
              for ((k__10 = 0)/*int*/; (k__10 < 8); (k__10++)) {{
                  (block_y__11 = (block_y__11 + ((((coeff__4__1)[(int)k__10])[(int)i__8]) * ((block_x__7[(int)k__10])[(int)j__9]))))/*float*/;
                }
              }
              (block_y__11 = ((float)(floorf(((double)((block_y__11 + ((float)0.5))))))))/*float*/;
              __push__1(((int)(block_y__11)));
            }
          }
        }
      }
      for ((i__8 = 0)/*int*/; (i__8 < 64); (i__8++)) {{
          __pop__1();
        }
      }
      // mark end: SIRFilter iDCT_2D_reference_coarse

    }
  }
}

// peek: 64 pop: 64 push 0
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_2;
int __counter_2 = 0;
int __steady_2 = 0;
int __tmp_2 = 0;
int __tmp2_2 = 0;
int *__state_flag_2 = NULL;
thread_info *__thread_2 = NULL;



void save_peek_buffer__2(object_write_buffer *buf);
void load_peek_buffer__2(object_write_buffer *buf);
void save_file_pointer__2(object_write_buffer *buf);
void load_file_pointer__2(object_write_buffer *buf);

 
void init_FloatPrinter__19_77__2();
inline void check_status__2();

void work_FloatPrinter__19_77__2(int);


inline int __pop__2() {
int res=BUFFER_1_2[TAIL_1_2];
TAIL_1_2++;
return res;
}

inline int __pop__2(int n) {
int res=BUFFER_1_2[TAIL_1_2];
TAIL_1_2+=n;

return res;
}

inline int __peek__2(int offs) {
return BUFFER_1_2[TAIL_1_2+offs];
}


 
void init_FloatPrinter__19_77__2(){
}
void save_file_pointer__2(object_write_buffer *buf) {}
void load_file_pointer__2(object_write_buffer *buf) {}
 
void work_FloatPrinter__19_77__2(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int i__18 = 0;/* int */

      // mark begin: SIRFilter FloatPrinter

      for ((i__18 = 0)/*int*/; (i__18 < 64); (i__18++)) {{

          // TIMER_PRINT_CODE: __print_sink__ += (int)(__pop__2()); 
          sum+=__pop__2();
        }
      }
      // mark end: SIRFilter FloatPrinter

    }
  }
}

