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

volatile double sum = 0;

int __max_iteration;
int __timer_enabled = 0;
int __frequency_of_chkpts;
volatile int __vol;
proc_timer tt("total runtime");


float BUFFER_0_1[__BUF_SIZE_MASK_0_1 + 1];
int HEAD_0_1 = 0;
int TAIL_0_1 = 0;
float BUFFER_1_2[__BUF_SIZE_MASK_1_2 + 1];
int HEAD_1_2 = 0;
int TAIL_1_2 = 0;
float BUFFER_1_11[__BUF_SIZE_MASK_1_11 + 1];
int HEAD_1_11 = 0;
int TAIL_1_11 = 0;
float BUFFER_2_3[__BUF_SIZE_MASK_2_3 + 1];
int HEAD_2_3 = 0;
int TAIL_2_3 = 0;
float BUFFER_3_4[__BUF_SIZE_MASK_3_4 + 1];
int HEAD_3_4 = 0;
int TAIL_3_4 = 0;
float BUFFER_4_5[__BUF_SIZE_MASK_4_5 + 1];
int HEAD_4_5 = 0;
int TAIL_4_5 = 0;
float BUFFER_5_6[__BUF_SIZE_MASK_5_6 + 1];
int HEAD_5_6 = 0;
int TAIL_5_6 = 0;
float BUFFER_6_7[__BUF_SIZE_MASK_6_7 + 1];
int HEAD_6_7 = 0;
int TAIL_6_7 = 0;
float BUFFER_7_8[__BUF_SIZE_MASK_7_8 + 1];
int HEAD_7_8 = 0;
int TAIL_7_8 = 0;
float BUFFER_8_9[__BUF_SIZE_MASK_8_9 + 1];
int HEAD_8_9 = 0;
int TAIL_8_9 = 0;
float BUFFER_9_10[__BUF_SIZE_MASK_9_10 + 1];
int HEAD_9_10 = 0;
int TAIL_9_10 = 0;
float BUFFER_11_12[__BUF_SIZE_MASK_11_12 + 1];
int HEAD_11_12 = 0;
int TAIL_11_12 = 0;
float BUFFER_12_13[__BUF_SIZE_MASK_12_13 + 1];
int HEAD_12_13 = 0;
int TAIL_12_13 = 0;
float BUFFER_13_14[__BUF_SIZE_MASK_13_14 + 1];
int HEAD_13_14 = 0;
int TAIL_13_14 = 0;
float BUFFER_14_15[__BUF_SIZE_MASK_14_15 + 1];
int HEAD_14_15 = 0;
int TAIL_14_15 = 0;
float BUFFER_15_16[__BUF_SIZE_MASK_15_16 + 1];
int HEAD_15_16 = 0;
int TAIL_15_16 = 0;
float BUFFER_16_17[__BUF_SIZE_MASK_16_17 + 1];
int HEAD_16_17 = 0;
int TAIL_16_17 = 0;
float BUFFER_17_9[__BUF_SIZE_MASK_17_9 + 1];
int HEAD_17_9 = 0;
int TAIL_17_9 = 0;
void init_FFTTestSource__3_31__0();
void work_FFTTestSource__3_31__0(int);
#ifdef BUFFER_MERGE
void work_FFTTestSource__3_31__0__mod(int ____n, void *____in, float *____out);
void work_FFTTestSource__3_31__0__mod2(int ____n, void *____in, float *____out, int s1, int s2);
#endif
void __splitter_1_work(int);
void init_FFTReorderSimple__7_32__2();
void work_FFTReorderSimple__7_32__2(int);
#ifdef BUFFER_MERGE
void work_FFTReorderSimple__7_32__2__mod(int ____n, float *____in, float *____out);
void work_FFTReorderSimple__7_32__2__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_FFTReorderSimple__11_33__3();
void work_FFTReorderSimple__11_33__3(int);
#ifdef BUFFER_MERGE
void work_FFTReorderSimple__11_33__3__mod(int ____n, float *____in, float *____out);
void work_FFTReorderSimple__11_33__3__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_FFTReorderSimple__15_34__4();
void work_FFTReorderSimple__15_34__4(int);
#ifdef BUFFER_MERGE
void work_FFTReorderSimple__15_34__4__mod(int ____n, float *____in, float *____out);
void work_FFTReorderSimple__15_34__4__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_CombineDFT__35_35__5();
void work_CombineDFT__35_35__5(int);
#ifdef BUFFER_MERGE
void work_CombineDFT__35_35__5__mod(int ____n, float *____in, float *____out);
void work_CombineDFT__35_35__5__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_CombineDFT__55_36__6();
void work_CombineDFT__55_36__6(int);
#ifdef BUFFER_MERGE
void work_CombineDFT__55_36__6__mod(int ____n, float *____in, float *____out);
void work_CombineDFT__55_36__6__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_CombineDFT__75_37__7();
void work_CombineDFT__75_37__7(int);
#ifdef BUFFER_MERGE
void work_CombineDFT__75_37__7__mod(int ____n, float *____in, float *____out);
void work_CombineDFT__75_37__7__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_CombineDFT__95_38__8();
void work_CombineDFT__95_38__8(int);
#ifdef BUFFER_MERGE
void work_CombineDFT__95_38__8__mod(int ____n, float *____in, float *____out);
void work_CombineDFT__95_38__8__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void __joiner_9_work(int);
void init_FloatPrinter__191_46__10();
void work_FloatPrinter__191_46__10(int);
#ifdef BUFFER_MERGE
void work_FloatPrinter__191_46__10__mod(int ____n, float *____in, void *____out);
void work_FloatPrinter__191_46__10__mod2(int ____n, float *____in, void *____out, int s1, int s2);
#endif
void init_FFTReorderSimple__99_39__11();
void work_FFTReorderSimple__99_39__11(int);
#ifdef BUFFER_MERGE
void work_FFTReorderSimple__99_39__11__mod(int ____n, float *____in, float *____out);
void work_FFTReorderSimple__99_39__11__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_FFTReorderSimple__103_40__12();
void work_FFTReorderSimple__103_40__12(int);
#ifdef BUFFER_MERGE
void work_FFTReorderSimple__103_40__12__mod(int ____n, float *____in, float *____out);
void work_FFTReorderSimple__103_40__12__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_FFTReorderSimple__107_41__13();
void work_FFTReorderSimple__107_41__13(int);
#ifdef BUFFER_MERGE
void work_FFTReorderSimple__107_41__13__mod(int ____n, float *____in, float *____out);
void work_FFTReorderSimple__107_41__13__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_CombineDFT__127_42__14();
void work_CombineDFT__127_42__14(int);
#ifdef BUFFER_MERGE
void work_CombineDFT__127_42__14__mod(int ____n, float *____in, float *____out);
void work_CombineDFT__127_42__14__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_CombineDFT__147_43__15();
void work_CombineDFT__147_43__15(int);
#ifdef BUFFER_MERGE
void work_CombineDFT__147_43__15__mod(int ____n, float *____in, float *____out);
void work_CombineDFT__147_43__15__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_CombineDFT__167_44__16();
void work_CombineDFT__167_44__16(int);
#ifdef BUFFER_MERGE
void work_CombineDFT__167_44__16__mod(int ____n, float *____in, float *____out);
void work_CombineDFT__167_44__16__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_CombineDFT__187_45__17();
void work_CombineDFT__187_45__17(int);
#ifdef BUFFER_MERGE
void work_CombineDFT__187_45__17__mod(int ____n, float *____in, float *____out);
void work_CombineDFT__187_45__17__mod2(int ____n, float *____in, float *____out, int s1, int s2);
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
// number of phases: 11


#ifdef MEASURE_BY_RTS
RTS_init(1,1);
#else //default ``MEASURE_BY_PAPI''
PAPI_HW_COUNTER_init(1,1);
#endif
  // ============= Initialization =============

init_FFTTestSource__3_31__0();
init_FFTReorderSimple__7_32__2();
init_FFTReorderSimple__99_39__11();
init_FFTReorderSimple__11_33__3();
init_FFTReorderSimple__103_40__12();
init_FFTReorderSimple__15_34__4();
init_FFTReorderSimple__107_41__13();
init_CombineDFT__35_35__5();
init_CombineDFT__127_42__14();
init_CombineDFT__55_36__6();
init_CombineDFT__147_43__15();
init_CombineDFT__75_37__7();
init_CombineDFT__167_44__16();
init_CombineDFT__95_38__8();
init_CombineDFT__187_45__17();
init_FloatPrinter__191_46__10();

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
    work_FFTTestSource__3_31__0(2 );
HEAD_1_2 = 0;
TAIL_1_2 = 0;
HEAD_1_11 = 0;
TAIL_1_11 = 0;
    __splitter_1_work(1 );
HEAD_2_3 = 0;
TAIL_2_3 = 0;
    work_FFTReorderSimple__7_32__2(1 );
HEAD_11_12 = 0;
TAIL_11_12 = 0;
    work_FFTReorderSimple__99_39__11(1 );
HEAD_3_4 = 0;
TAIL_3_4 = 0;
    work_FFTReorderSimple__11_33__3(2 );
HEAD_12_13 = 0;
TAIL_12_13 = 0;
    work_FFTReorderSimple__103_40__12(2 );
HEAD_4_5 = 0;
TAIL_4_5 = 0;
    work_FFTReorderSimple__15_34__4(4 );
HEAD_13_14 = 0;
TAIL_13_14 = 0;
    work_FFTReorderSimple__107_41__13(4 );
HEAD_5_6 = 0;
TAIL_5_6 = 0;
    work_CombineDFT__35_35__5(8 );
HEAD_14_15 = 0;
TAIL_14_15 = 0;
    work_CombineDFT__127_42__14(8 );
HEAD_6_7 = 0;
TAIL_6_7 = 0;
    work_CombineDFT__55_36__6(4 );
HEAD_15_16 = 0;
TAIL_15_16 = 0;
    work_CombineDFT__147_43__15(4 );
HEAD_7_8 = 0;
TAIL_7_8 = 0;
    work_CombineDFT__75_37__7(2 );
HEAD_16_17 = 0;
TAIL_16_17 = 0;
    work_CombineDFT__167_44__16(2 );
HEAD_8_9 = 0;
TAIL_8_9 = 0;
    work_CombineDFT__95_38__8(1 );
HEAD_17_9 = 0;
TAIL_17_9 = 0;
    work_CombineDFT__187_45__17(1 );
HEAD_9_10 = 0;
TAIL_9_10 = 0;
    __joiner_9_work(1 );
    work_FloatPrinter__191_46__10(64 );
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
  printf("%f\n", sum);
  return 0;
}

// moved or inserted by concat_cluster_threads.pl
#include <message.h>
message *__msg_stack_3;
message *__msg_stack_10;
message *__msg_stack_7;
message *__msg_stack_6;
message *__msg_stack_9;
message *__msg_stack_4;
message *__msg_stack_0;
message *__msg_stack_8;
message *__msg_stack_12;
message *__msg_stack_1;
message *__msg_stack_11;
message *__msg_stack_14;
message *__msg_stack_5;
message *__msg_stack_15;
message *__msg_stack_2;
message *__msg_stack_13;
message *__msg_stack_17;
message *__msg_stack_16;

// end of moved or inserted by concat_cluster_threads.pl

// peek: 0 pop: 0 push 32
// init counts: 0 steady counts: 2

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

 
void init_FFTTestSource__3_31__0();
inline void check_status__0();

void work_FFTTestSource__3_31__0(int);



inline void __push__0(float data) {
BUFFER_0_1[HEAD_0_1]=data;
HEAD_0_1++;
}



 
void init_FFTTestSource__3_31__0(){
}
void save_file_pointer__0(object_write_buffer *buf) {}
void load_file_pointer__0(object_write_buffer *buf) {}
 
void work_FFTTestSource__3_31__0(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int i__2 = 0;/* int */

      // mark begin: SIRFilter FFTTestSource

      __push__0(((float)0.0));
      __push__0(((float)0.0));
      __push__0(((float)1.0));
      __push__0(((float)0.0));
      for ((i__2 = 0)/*int*/; (i__2 < 28); (i__2++)) {{
          __push__0(((float)0.0));
        }
      }
      // mark end: SIRFilter FFTTestSource

    }
  }
}

// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_1;
int __counter_1 = 0;
int __steady_1 = 0;
int __tmp_1 = 0;
int __tmp2_1 = 0;
int *__state_flag_1 = NULL;
thread_info *__thread_1 = NULL;




void __splitter_1_work(int ____n) {
  for (;____n > 0; ____n--) {
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_2[HEAD_1_2]=BUFFER_0_1[TAIL_0_1];
HEAD_1_2++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
BUFFER_1_11[HEAD_1_11]=BUFFER_0_1[TAIL_0_1];
HEAD_1_11++; ; TAIL_0_1++;
  }
}


// peek: 32 pop: 32 push 32
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

 
void init_FFTReorderSimple__7_32__2();
inline void check_status__2();

void work_FFTReorderSimple__7_32__2(int);


inline float __pop__2() {
float res=BUFFER_1_2[TAIL_1_2];
TAIL_1_2++;
return res;
}

inline float __pop__2(int n) {
float res=BUFFER_1_2[TAIL_1_2];
TAIL_1_2+=n;

return res;
}

inline float __peek__2(int offs) {
return BUFFER_1_2[TAIL_1_2+offs];
}



inline void __push__2(float data) {
BUFFER_2_3[HEAD_2_3]=data;
HEAD_2_3++;
}



 
void init_FFTReorderSimple__7_32__2(){
}
void save_file_pointer__2(object_write_buffer *buf) {}
void load_file_pointer__2(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_FFTReorderSimple__7_32__2__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  int i__6 = 0;/* int */

  // mark begin: SIRFilter FFTReorderSimple

  for ((i__6 = 0)/*int*/; (i__6 < 32); (i__6 = (i__6 + 4))/*int*/) {{
      ((*____out++)=(*(____in+i__6)));
      ((*____out++)=(*(____in+(i__6 + 1))));
    }
  }
  for ((i__6 = 2)/*int*/; (i__6 < 32); (i__6 = (i__6 + 4))/*int*/) {{
      ((*____out++)=(*(____in+i__6)));
      ((*____out++)=(*(____in+(i__6 + 1))));
    }
  }
  for ((i__6 = 0)/*int*/; (i__6 < 16); (i__6++)) {{
      assert(false);__pop__2(2);
    }
  }
  // mark end: SIRFilter FFTReorderSimple

}}


void work_FFTReorderSimple__7_32__2__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int i__6 = 0;/* int */

  // mark begin: SIRFilter FFTReorderSimple

  for ((i__6 = 0)/*int*/; (i__6 < 32); (i__6 = (i__6 + 4))/*int*/) {{
      ((*____out++)=(*(____in+i__6)));
      ((*____out++)=(*(____in+(i__6 + 1))));
    }
  }
  for ((i__6 = 2)/*int*/; (i__6 < 32); (i__6 = (i__6 + 4))/*int*/) {{
      ((*____out++)=(*(____in+i__6)));
      ((*____out++)=(*(____in+(i__6 + 1))));
    }
  }
  for ((i__6 = 0)/*int*/; (i__6 < 16); (i__6++)) {{
      assert(false);__pop__2(2);
    }
  }
  // mark end: SIRFilter FFTReorderSimple

}}


#endif // BUFFER_MERGE


 
void work_FFTReorderSimple__7_32__2(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int i__6 = 0;/* int */

      // mark begin: SIRFilter FFTReorderSimple

      for ((i__6 = 0)/*int*/; (i__6 < 32); (i__6 = (i__6 + 4))/*int*/) {{
          __push__2(__peek__2(i__6));
          __push__2(__peek__2((i__6 + 1)));
        }
      }
      for ((i__6 = 2)/*int*/; (i__6 < 32); (i__6 = (i__6 + 4))/*int*/) {{
          __push__2(__peek__2(i__6));
          __push__2(__peek__2((i__6 + 1)));
        }
      }
      for ((i__6 = 0)/*int*/; (i__6 < 16); (i__6++)) {{
          __pop__2(2);
        }
      }
      // mark end: SIRFilter FFTReorderSimple

    }
  }
}

// peek: 16 pop: 16 push 16
// init counts: 0 steady counts: 2

// ClusterFusion isEliminated: false



int __number_of_iterations_3;
int __counter_3 = 0;
int __steady_3 = 0;
int __tmp_3 = 0;
int __tmp2_3 = 0;
int *__state_flag_3 = NULL;
thread_info *__thread_3 = NULL;



void save_peek_buffer__3(object_write_buffer *buf);
void load_peek_buffer__3(object_write_buffer *buf);
void save_file_pointer__3(object_write_buffer *buf);
void load_file_pointer__3(object_write_buffer *buf);

 
void init_FFTReorderSimple__11_33__3();
inline void check_status__3();

void work_FFTReorderSimple__11_33__3(int);


inline float __pop__3() {
float res=BUFFER_2_3[TAIL_2_3];
TAIL_2_3++;
return res;
}

inline float __pop__3(int n) {
float res=BUFFER_2_3[TAIL_2_3];
TAIL_2_3+=n;

return res;
}

inline float __peek__3(int offs) {
return BUFFER_2_3[TAIL_2_3+offs];
}



inline void __push__3(float data) {
BUFFER_3_4[HEAD_3_4]=data;
HEAD_3_4++;
}



 
void init_FFTReorderSimple__11_33__3(){
}
void save_file_pointer__3(object_write_buffer *buf) {}
void load_file_pointer__3(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_FFTReorderSimple__11_33__3__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  int i__10 = 0;/* int */

  // mark begin: SIRFilter FFTReorderSimple

  for ((i__10 = 0)/*int*/; (i__10 < 16); (i__10 = (i__10 + 4))/*int*/) {{
      ((*____out++)=(*(____in+i__10)));
      ((*____out++)=(*(____in+(i__10 + 1))));
    }
  }
  for ((i__10 = 2)/*int*/; (i__10 < 16); (i__10 = (i__10 + 4))/*int*/) {{
      ((*____out++)=(*(____in+i__10)));
      ((*____out++)=(*(____in+(i__10 + 1))));
    }
  }
  for ((i__10 = 0)/*int*/; (i__10 < 8); (i__10++)) {{
      assert(false);__pop__3(2);
    }
  }
  // mark end: SIRFilter FFTReorderSimple

}}


void work_FFTReorderSimple__11_33__3__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int i__10 = 0;/* int */

  // mark begin: SIRFilter FFTReorderSimple

  for ((i__10 = 0)/*int*/; (i__10 < 16); (i__10 = (i__10 + 4))/*int*/) {{
      ((*____out++)=(*(____in+i__10)));
      ((*____out++)=(*(____in+(i__10 + 1))));
    }
  }
  for ((i__10 = 2)/*int*/; (i__10 < 16); (i__10 = (i__10 + 4))/*int*/) {{
      ((*____out++)=(*(____in+i__10)));
      ((*____out++)=(*(____in+(i__10 + 1))));
    }
  }
  for ((i__10 = 0)/*int*/; (i__10 < 8); (i__10++)) {{
      assert(false);__pop__3(2);
    }
  }
  // mark end: SIRFilter FFTReorderSimple

}}


#endif // BUFFER_MERGE


 
void work_FFTReorderSimple__11_33__3(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int i__10 = 0;/* int */

      // mark begin: SIRFilter FFTReorderSimple

      for ((i__10 = 0)/*int*/; (i__10 < 16); (i__10 = (i__10 + 4))/*int*/) {{
          __push__3(__peek__3(i__10));
          __push__3(__peek__3((i__10 + 1)));
        }
      }
      for ((i__10 = 2)/*int*/; (i__10 < 16); (i__10 = (i__10 + 4))/*int*/) {{
          __push__3(__peek__3(i__10));
          __push__3(__peek__3((i__10 + 1)));
        }
      }
      for ((i__10 = 0)/*int*/; (i__10 < 8); (i__10++)) {{
          __pop__3(2);
        }
      }
      // mark end: SIRFilter FFTReorderSimple

    }
  }
}

// peek: 8 pop: 8 push 8
// init counts: 0 steady counts: 4

// ClusterFusion isEliminated: false



int __number_of_iterations_4;
int __counter_4 = 0;
int __steady_4 = 0;
int __tmp_4 = 0;
int __tmp2_4 = 0;
int *__state_flag_4 = NULL;
thread_info *__thread_4 = NULL;



void save_peek_buffer__4(object_write_buffer *buf);
void load_peek_buffer__4(object_write_buffer *buf);
void save_file_pointer__4(object_write_buffer *buf);
void load_file_pointer__4(object_write_buffer *buf);

 
void init_FFTReorderSimple__15_34__4();
inline void check_status__4();

void work_FFTReorderSimple__15_34__4(int);


inline float __pop__4() {
float res=BUFFER_3_4[TAIL_3_4];
TAIL_3_4++;
return res;
}

inline float __pop__4(int n) {
float res=BUFFER_3_4[TAIL_3_4];
TAIL_3_4+=n;

return res;
}

inline float __peek__4(int offs) {
return BUFFER_3_4[TAIL_3_4+offs];
}



inline void __push__4(float data) {
BUFFER_4_5[HEAD_4_5]=data;
HEAD_4_5++;
}



 
void init_FFTReorderSimple__15_34__4(){
}
void save_file_pointer__4(object_write_buffer *buf) {}
void load_file_pointer__4(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_FFTReorderSimple__15_34__4__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  int i__14 = 0;/* int */

  // mark begin: SIRFilter FFTReorderSimple

  for ((i__14 = 0)/*int*/; (i__14 < 8); (i__14 = (i__14 + 4))/*int*/) {{
      ((*____out++)=(*(____in+i__14)));
      ((*____out++)=(*(____in+(i__14 + 1))));
    }
  }
  for ((i__14 = 2)/*int*/; (i__14 < 8); (i__14 = (i__14 + 4))/*int*/) {{
      ((*____out++)=(*(____in+i__14)));
      ((*____out++)=(*(____in+(i__14 + 1))));
    }
  }
  for ((i__14 = 0)/*int*/; (i__14 < 4); (i__14++)) {{
      assert(false);__pop__4(2);
    }
  }
  // mark end: SIRFilter FFTReorderSimple

}}


void work_FFTReorderSimple__15_34__4__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int i__14 = 0;/* int */

  // mark begin: SIRFilter FFTReorderSimple

  for ((i__14 = 0)/*int*/; (i__14 < 8); (i__14 = (i__14 + 4))/*int*/) {{
      ((*____out++)=(*(____in+i__14)));
      ((*____out++)=(*(____in+(i__14 + 1))));
    }
  }
  for ((i__14 = 2)/*int*/; (i__14 < 8); (i__14 = (i__14 + 4))/*int*/) {{
      ((*____out++)=(*(____in+i__14)));
      ((*____out++)=(*(____in+(i__14 + 1))));
    }
  }
  for ((i__14 = 0)/*int*/; (i__14 < 4); (i__14++)) {{
      assert(false);__pop__4(2);
    }
  }
  // mark end: SIRFilter FFTReorderSimple

}}


#endif // BUFFER_MERGE


 
void work_FFTReorderSimple__15_34__4(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int i__14 = 0;/* int */

      // mark begin: SIRFilter FFTReorderSimple

      for ((i__14 = 0)/*int*/; (i__14 < 8); (i__14 = (i__14 + 4))/*int*/) {{
          __push__4(__peek__4(i__14));
          __push__4(__peek__4((i__14 + 1)));
        }
      }
      for ((i__14 = 2)/*int*/; (i__14 < 8); (i__14 = (i__14 + 4))/*int*/) {{
          __push__4(__peek__4(i__14));
          __push__4(__peek__4((i__14 + 1)));
        }
      }
      for ((i__14 = 0)/*int*/; (i__14 < 4); (i__14++)) {{
          __pop__4(2);
        }
      }
      // mark end: SIRFilter FFTReorderSimple

    }
  }
}

// peek: 4 pop: 4 push 4
// init counts: 0 steady counts: 8

// ClusterFusion isEliminated: false



int __number_of_iterations_5;
int __counter_5 = 0;
int __steady_5 = 0;
int __tmp_5 = 0;
int __tmp2_5 = 0;
int *__state_flag_5 = NULL;
thread_info *__thread_5 = NULL;



float w__16__5[2] = {0};
void save_peek_buffer__5(object_write_buffer *buf);
void load_peek_buffer__5(object_write_buffer *buf);
void save_file_pointer__5(object_write_buffer *buf);
void load_file_pointer__5(object_write_buffer *buf);

 
void init_CombineDFT__35_35__5();
inline void check_status__5();

void work_CombineDFT__35_35__5(int);


inline float __pop__5() {
float res=BUFFER_4_5[TAIL_4_5];
TAIL_4_5++;
return res;
}

inline float __pop__5(int n) {
float res=BUFFER_4_5[TAIL_4_5];
TAIL_4_5+=n;

return res;
}

inline float __peek__5(int offs) {
return BUFFER_4_5[TAIL_4_5+offs];
}



inline void __push__5(float data) {
BUFFER_5_6[HEAD_5_6]=data;
HEAD_5_6++;
}



 
void init_CombineDFT__35_35__5(){
  float real__30 = 0.0f;/* float */
  float imag__31 = 0.0f;/* float */
  float next_real__32 = 0.0f;/* float */float next_imag__33 = 0.0f;/* float */
  int i__34 = 0;/* int */

  (real__30 = ((float)1.0))/*float*/;
  (imag__31 = ((float)0.0))/*float*/;
  for ((i__34 = 0)/*int*/; (i__34 < 2); (i__34 = (i__34 + 2))/*int*/) {{
      (((w__16__5)[(int)i__34]) = real__30)/*float*/;
      (((w__16__5)[(int)(i__34 + 1)]) = imag__31)/*float*/;
      (next_real__32 = ((real__30 * ((float)-1.0)) - (imag__31 * ((float)8.742278E-8))))/*float*/;
      (next_imag__33 = ((real__30 * ((float)8.742278E-8)) + (imag__31 * ((float)-1.0))))/*float*/;
      (real__30 = next_real__32)/*float*/;
      (imag__31 = next_imag__33)/*float*/;
    }
  }
}
void save_file_pointer__5(object_write_buffer *buf) {}
void load_file_pointer__5(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_CombineDFT__35_35__5__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  int i__19 = 0;/* int */
  float results__20[4] = {0};/* float[4] */
  int i_plus_1__21 = 0;/* int */
  float y0_r__22 = 0.0f;/* float */
  float y0_i__23 = 0.0f;/* float */
  float y1_r__24 = 0.0f;/* float */
  float y1_i__25 = 0.0f;/* float */
  float weight_real__26 = 0.0f;/* float */
  float weight_imag__27 = 0.0f;/* float */
  float y1w_r__28 = 0.0f;/* float */
  float y1w_i__29 = 0.0f;/* float */

  // mark begin: SIRFilter CombineDFT

  for ((i__19 = 0)/*int*/; (i__19 < 2); (i__19 = (i__19 + 2))/*int*/) {{
      (i_plus_1__21 = (i__19 + 1))/*int*/;
      (y0_r__22 = (*(____in+i__19)))/*float*/;
      (y0_i__23 = (*(____in+i_plus_1__21)))/*float*/;
      (y1_r__24 = (*(____in+(2 + i__19))))/*float*/;
      (y1_i__25 = (*(____in+(2 + i_plus_1__21))))/*float*/;
      (weight_real__26 = ((w__16__5)[(int)i__19]))/*float*/;
      (weight_imag__27 = ((w__16__5)[(int)i_plus_1__21]))/*float*/;
      (y1w_r__28 = ((y1_r__24 * weight_real__26) - (y1_i__25 * weight_imag__27)))/*float*/;
      (y1w_i__29 = ((y1_r__24 * weight_imag__27) + (y1_i__25 * weight_real__26)))/*float*/;
      ((results__20[(int)i__19]) = (y0_r__22 + y1w_r__28))/*float*/;
      ((results__20[(int)(i__19 + 1)]) = (y0_i__23 + y1w_i__29))/*float*/;
      ((results__20[(int)(2 + i__19)]) = (y0_r__22 - y1w_r__28))/*float*/;
      ((results__20[(int)((2 + i__19) + 1)]) = (y0_i__23 - y1w_i__29))/*float*/;
    }
  }
  for ((i__19 = 0)/*int*/; (i__19 < 4); (i__19++)) {{
      (*____in++);
      ((*____out++)=(results__20[(int)i__19]));
    }
  }
  // mark end: SIRFilter CombineDFT

}}


void work_CombineDFT__35_35__5__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int i__19 = 0;/* int */
  float results__20[4] = {0};/* float[4] */
  int i_plus_1__21 = 0;/* int */
  float y0_r__22 = 0.0f;/* float */
  float y0_i__23 = 0.0f;/* float */
  float y1_r__24 = 0.0f;/* float */
  float y1_i__25 = 0.0f;/* float */
  float weight_real__26 = 0.0f;/* float */
  float weight_imag__27 = 0.0f;/* float */
  float y1w_r__28 = 0.0f;/* float */
  float y1w_i__29 = 0.0f;/* float */

  // mark begin: SIRFilter CombineDFT

  for ((i__19 = 0)/*int*/; (i__19 < 2); (i__19 = (i__19 + 2))/*int*/) {{
      (i_plus_1__21 = (i__19 + 1))/*int*/;
      (y0_r__22 = (*(____in+i__19)))/*float*/;
      (y0_i__23 = (*(____in+i_plus_1__21)))/*float*/;
      (y1_r__24 = (*(____in+(2 + i__19))))/*float*/;
      (y1_i__25 = (*(____in+(2 + i_plus_1__21))))/*float*/;
      (weight_real__26 = ((w__16__5)[(int)i__19]))/*float*/;
      (weight_imag__27 = ((w__16__5)[(int)i_plus_1__21]))/*float*/;
      (y1w_r__28 = ((y1_r__24 * weight_real__26) - (y1_i__25 * weight_imag__27)))/*float*/;
      (y1w_i__29 = ((y1_r__24 * weight_imag__27) + (y1_i__25 * weight_real__26)))/*float*/;
      ((results__20[(int)i__19]) = (y0_r__22 + y1w_r__28))/*float*/;
      ((results__20[(int)(i__19 + 1)]) = (y0_i__23 + y1w_i__29))/*float*/;
      ((results__20[(int)(2 + i__19)]) = (y0_r__22 - y1w_r__28))/*float*/;
      ((results__20[(int)((2 + i__19) + 1)]) = (y0_i__23 - y1w_i__29))/*float*/;
    }
  }
  for ((i__19 = 0)/*int*/; (i__19 < 4); (i__19++)) {{
      (*____in++);
      ((*____out++)=(results__20[(int)i__19]));
    }
  }
  // mark end: SIRFilter CombineDFT

}}


#endif // BUFFER_MERGE


 
void work_CombineDFT__35_35__5(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int i__19 = 0;/* int */
      float results__20[4] = {0};/* float[4] */
      int i_plus_1__21 = 0;/* int */
      float y0_r__22 = 0.0f;/* float */
      float y0_i__23 = 0.0f;/* float */
      float y1_r__24 = 0.0f;/* float */
      float y1_i__25 = 0.0f;/* float */
      float weight_real__26 = 0.0f;/* float */
      float weight_imag__27 = 0.0f;/* float */
      float y1w_r__28 = 0.0f;/* float */
      float y1w_i__29 = 0.0f;/* float */

      // mark begin: SIRFilter CombineDFT

      for ((i__19 = 0)/*int*/; (i__19 < 2); (i__19 = (i__19 + 2))/*int*/) {{
          (i_plus_1__21 = (i__19 + 1))/*int*/;
          (y0_r__22 = __peek__5(i__19))/*float*/;
          (y0_i__23 = __peek__5(i_plus_1__21))/*float*/;
          (y1_r__24 = __peek__5((2 + i__19)))/*float*/;
          (y1_i__25 = __peek__5((2 + i_plus_1__21)))/*float*/;
          (weight_real__26 = ((w__16__5)[(int)i__19]))/*float*/;
          (weight_imag__27 = ((w__16__5)[(int)i_plus_1__21]))/*float*/;
          (y1w_r__28 = ((y1_r__24 * weight_real__26) - (y1_i__25 * weight_imag__27)))/*float*/;
          (y1w_i__29 = ((y1_r__24 * weight_imag__27) + (y1_i__25 * weight_real__26)))/*float*/;
          ((results__20[(int)i__19]) = (y0_r__22 + y1w_r__28))/*float*/;
          ((results__20[(int)(i__19 + 1)]) = (y0_i__23 + y1w_i__29))/*float*/;
          ((results__20[(int)(2 + i__19)]) = (y0_r__22 - y1w_r__28))/*float*/;
          ((results__20[(int)((2 + i__19) + 1)]) = (y0_i__23 - y1w_i__29))/*float*/;
        }
      }
      for ((i__19 = 0)/*int*/; (i__19 < 4); (i__19++)) {{
          __pop__5();
          __push__5((results__20[(int)i__19]));
        }
      }
      // mark end: SIRFilter CombineDFT

    }
  }
}

// peek: 8 pop: 8 push 8
// init counts: 0 steady counts: 4

// ClusterFusion isEliminated: false



int __number_of_iterations_6;
int __counter_6 = 0;
int __steady_6 = 0;
int __tmp_6 = 0;
int __tmp2_6 = 0;
int *__state_flag_6 = NULL;
thread_info *__thread_6 = NULL;



float w__36__6[4] = {0};
void save_peek_buffer__6(object_write_buffer *buf);
void load_peek_buffer__6(object_write_buffer *buf);
void save_file_pointer__6(object_write_buffer *buf);
void load_file_pointer__6(object_write_buffer *buf);

 
void init_CombineDFT__55_36__6();
inline void check_status__6();

void work_CombineDFT__55_36__6(int);


inline float __pop__6() {
float res=BUFFER_5_6[TAIL_5_6];
TAIL_5_6++;
return res;
}

inline float __pop__6(int n) {
float res=BUFFER_5_6[TAIL_5_6];
TAIL_5_6+=n;

return res;
}

inline float __peek__6(int offs) {
return BUFFER_5_6[TAIL_5_6+offs];
}



inline void __push__6(float data) {
BUFFER_6_7[HEAD_6_7]=data;
HEAD_6_7++;
}



 
void init_CombineDFT__55_36__6(){
  float real__50 = 0.0f;/* float */
  float imag__51 = 0.0f;/* float */
  float next_real__52 = 0.0f;/* float */float next_imag__53 = 0.0f;/* float */
  int i__54 = 0;/* int */

  (real__50 = ((float)1.0))/*float*/;
  (imag__51 = ((float)0.0))/*float*/;
  for ((i__54 = 0)/*int*/; (i__54 < 4); (i__54 = (i__54 + 2))/*int*/) {{
      (((w__36__6)[(int)i__54]) = real__50)/*float*/;
      (((w__36__6)[(int)(i__54 + 1)]) = imag__51)/*float*/;
      (next_real__52 = ((real__50 * ((float)-4.371139E-8)) - (imag__51 * ((float)-1.0))))/*float*/;
      (next_imag__53 = ((real__50 * ((float)-1.0)) + (imag__51 * ((float)-4.371139E-8))))/*float*/;
      (real__50 = next_real__52)/*float*/;
      (imag__51 = next_imag__53)/*float*/;
    }
  }
}
void save_file_pointer__6(object_write_buffer *buf) {}
void load_file_pointer__6(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_CombineDFT__55_36__6__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  int i__39 = 0;/* int */
  float results__40[8] = {0};/* float[8] */
  int i_plus_1__41 = 0;/* int */
  float y0_r__42 = 0.0f;/* float */
  float y0_i__43 = 0.0f;/* float */
  float y1_r__44 = 0.0f;/* float */
  float y1_i__45 = 0.0f;/* float */
  float weight_real__46 = 0.0f;/* float */
  float weight_imag__47 = 0.0f;/* float */
  float y1w_r__48 = 0.0f;/* float */
  float y1w_i__49 = 0.0f;/* float */

  // mark begin: SIRFilter CombineDFT

  for ((i__39 = 0)/*int*/; (i__39 < 4); (i__39 = (i__39 + 2))/*int*/) {{
      (i_plus_1__41 = (i__39 + 1))/*int*/;
      (y0_r__42 = (*(____in+i__39)))/*float*/;
      (y0_i__43 = (*(____in+i_plus_1__41)))/*float*/;
      (y1_r__44 = (*(____in+(4 + i__39))))/*float*/;
      (y1_i__45 = (*(____in+(4 + i_plus_1__41))))/*float*/;
      (weight_real__46 = ((w__36__6)[(int)i__39]))/*float*/;
      (weight_imag__47 = ((w__36__6)[(int)i_plus_1__41]))/*float*/;
      (y1w_r__48 = ((y1_r__44 * weight_real__46) - (y1_i__45 * weight_imag__47)))/*float*/;
      (y1w_i__49 = ((y1_r__44 * weight_imag__47) + (y1_i__45 * weight_real__46)))/*float*/;
      ((results__40[(int)i__39]) = (y0_r__42 + y1w_r__48))/*float*/;
      ((results__40[(int)(i__39 + 1)]) = (y0_i__43 + y1w_i__49))/*float*/;
      ((results__40[(int)(4 + i__39)]) = (y0_r__42 - y1w_r__48))/*float*/;
      ((results__40[(int)((4 + i__39) + 1)]) = (y0_i__43 - y1w_i__49))/*float*/;
    }
  }
  for ((i__39 = 0)/*int*/; (i__39 < 8); (i__39++)) {{
      (*____in++);
      ((*____out++)=(results__40[(int)i__39]));
    }
  }
  // mark end: SIRFilter CombineDFT

}}


void work_CombineDFT__55_36__6__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int i__39 = 0;/* int */
  float results__40[8] = {0};/* float[8] */
  int i_plus_1__41 = 0;/* int */
  float y0_r__42 = 0.0f;/* float */
  float y0_i__43 = 0.0f;/* float */
  float y1_r__44 = 0.0f;/* float */
  float y1_i__45 = 0.0f;/* float */
  float weight_real__46 = 0.0f;/* float */
  float weight_imag__47 = 0.0f;/* float */
  float y1w_r__48 = 0.0f;/* float */
  float y1w_i__49 = 0.0f;/* float */

  // mark begin: SIRFilter CombineDFT

  for ((i__39 = 0)/*int*/; (i__39 < 4); (i__39 = (i__39 + 2))/*int*/) {{
      (i_plus_1__41 = (i__39 + 1))/*int*/;
      (y0_r__42 = (*(____in+i__39)))/*float*/;
      (y0_i__43 = (*(____in+i_plus_1__41)))/*float*/;
      (y1_r__44 = (*(____in+(4 + i__39))))/*float*/;
      (y1_i__45 = (*(____in+(4 + i_plus_1__41))))/*float*/;
      (weight_real__46 = ((w__36__6)[(int)i__39]))/*float*/;
      (weight_imag__47 = ((w__36__6)[(int)i_plus_1__41]))/*float*/;
      (y1w_r__48 = ((y1_r__44 * weight_real__46) - (y1_i__45 * weight_imag__47)))/*float*/;
      (y1w_i__49 = ((y1_r__44 * weight_imag__47) + (y1_i__45 * weight_real__46)))/*float*/;
      ((results__40[(int)i__39]) = (y0_r__42 + y1w_r__48))/*float*/;
      ((results__40[(int)(i__39 + 1)]) = (y0_i__43 + y1w_i__49))/*float*/;
      ((results__40[(int)(4 + i__39)]) = (y0_r__42 - y1w_r__48))/*float*/;
      ((results__40[(int)((4 + i__39) + 1)]) = (y0_i__43 - y1w_i__49))/*float*/;
    }
  }
  for ((i__39 = 0)/*int*/; (i__39 < 8); (i__39++)) {{
      (*____in++);
      ((*____out++)=(results__40[(int)i__39]));
    }
  }
  // mark end: SIRFilter CombineDFT

}}


#endif // BUFFER_MERGE


 
void work_CombineDFT__55_36__6(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int i__39 = 0;/* int */
      float results__40[8] = {0};/* float[8] */
      int i_plus_1__41 = 0;/* int */
      float y0_r__42 = 0.0f;/* float */
      float y0_i__43 = 0.0f;/* float */
      float y1_r__44 = 0.0f;/* float */
      float y1_i__45 = 0.0f;/* float */
      float weight_real__46 = 0.0f;/* float */
      float weight_imag__47 = 0.0f;/* float */
      float y1w_r__48 = 0.0f;/* float */
      float y1w_i__49 = 0.0f;/* float */

      // mark begin: SIRFilter CombineDFT

      for ((i__39 = 0)/*int*/; (i__39 < 4); (i__39 = (i__39 + 2))/*int*/) {{
          (i_plus_1__41 = (i__39 + 1))/*int*/;
          (y0_r__42 = __peek__6(i__39))/*float*/;
          (y0_i__43 = __peek__6(i_plus_1__41))/*float*/;
          (y1_r__44 = __peek__6((4 + i__39)))/*float*/;
          (y1_i__45 = __peek__6((4 + i_plus_1__41)))/*float*/;
          (weight_real__46 = ((w__36__6)[(int)i__39]))/*float*/;
          (weight_imag__47 = ((w__36__6)[(int)i_plus_1__41]))/*float*/;
          (y1w_r__48 = ((y1_r__44 * weight_real__46) - (y1_i__45 * weight_imag__47)))/*float*/;
          (y1w_i__49 = ((y1_r__44 * weight_imag__47) + (y1_i__45 * weight_real__46)))/*float*/;
          ((results__40[(int)i__39]) = (y0_r__42 + y1w_r__48))/*float*/;
          ((results__40[(int)(i__39 + 1)]) = (y0_i__43 + y1w_i__49))/*float*/;
          ((results__40[(int)(4 + i__39)]) = (y0_r__42 - y1w_r__48))/*float*/;
          ((results__40[(int)((4 + i__39) + 1)]) = (y0_i__43 - y1w_i__49))/*float*/;
        }
      }
      for ((i__39 = 0)/*int*/; (i__39 < 8); (i__39++)) {{
          __pop__6();
          __push__6((results__40[(int)i__39]));
        }
      }
      // mark end: SIRFilter CombineDFT

    }
  }
}

// peek: 16 pop: 16 push 16
// init counts: 0 steady counts: 2

// ClusterFusion isEliminated: false



int __number_of_iterations_7;
int __counter_7 = 0;
int __steady_7 = 0;
int __tmp_7 = 0;
int __tmp2_7 = 0;
int *__state_flag_7 = NULL;
thread_info *__thread_7 = NULL;



float w__56__7[8] = {0};
void save_peek_buffer__7(object_write_buffer *buf);
void load_peek_buffer__7(object_write_buffer *buf);
void save_file_pointer__7(object_write_buffer *buf);
void load_file_pointer__7(object_write_buffer *buf);

 
void init_CombineDFT__75_37__7();
inline void check_status__7();

void work_CombineDFT__75_37__7(int);


inline float __pop__7() {
float res=BUFFER_6_7[TAIL_6_7];
TAIL_6_7++;
return res;
}

inline float __pop__7(int n) {
float res=BUFFER_6_7[TAIL_6_7];
TAIL_6_7+=n;

return res;
}

inline float __peek__7(int offs) {
return BUFFER_6_7[TAIL_6_7+offs];
}



inline void __push__7(float data) {
BUFFER_7_8[HEAD_7_8]=data;
HEAD_7_8++;
}



 
void init_CombineDFT__75_37__7(){
  float real__70 = 0.0f;/* float */
  float imag__71 = 0.0f;/* float */
  float next_real__72 = 0.0f;/* float */float next_imag__73 = 0.0f;/* float */
  int i__74 = 0;/* int */

  (real__70 = ((float)1.0))/*float*/;
  (imag__71 = ((float)0.0))/*float*/;
  for ((i__74 = 0)/*int*/; (i__74 < 8); (i__74 = (i__74 + 2))/*int*/) {{
      (((w__56__7)[(int)i__74]) = real__70)/*float*/;
      (((w__56__7)[(int)(i__74 + 1)]) = imag__71)/*float*/;
      (next_real__72 = ((real__70 * ((float)0.70710677)) - (imag__71 * ((float)-0.70710677))))/*float*/;
      (next_imag__73 = ((real__70 * ((float)-0.70710677)) + (imag__71 * ((float)0.70710677))))/*float*/;
      (real__70 = next_real__72)/*float*/;
      (imag__71 = next_imag__73)/*float*/;
    }
  }
}
void save_file_pointer__7(object_write_buffer *buf) {}
void load_file_pointer__7(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_CombineDFT__75_37__7__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  int i__59 = 0;/* int */
  float results__60[16] = {0};/* float[16] */
  int i_plus_1__61 = 0;/* int */
  float y0_r__62 = 0.0f;/* float */
  float y0_i__63 = 0.0f;/* float */
  float y1_r__64 = 0.0f;/* float */
  float y1_i__65 = 0.0f;/* float */
  float weight_real__66 = 0.0f;/* float */
  float weight_imag__67 = 0.0f;/* float */
  float y1w_r__68 = 0.0f;/* float */
  float y1w_i__69 = 0.0f;/* float */

  // mark begin: SIRFilter CombineDFT

  for ((i__59 = 0)/*int*/; (i__59 < 8); (i__59 = (i__59 + 2))/*int*/) {{
      (i_plus_1__61 = (i__59 + 1))/*int*/;
      (y0_r__62 = (*(____in+i__59)))/*float*/;
      (y0_i__63 = (*(____in+i_plus_1__61)))/*float*/;
      (y1_r__64 = (*(____in+(8 + i__59))))/*float*/;
      (y1_i__65 = (*(____in+(8 + i_plus_1__61))))/*float*/;
      (weight_real__66 = ((w__56__7)[(int)i__59]))/*float*/;
      (weight_imag__67 = ((w__56__7)[(int)i_plus_1__61]))/*float*/;
      (y1w_r__68 = ((y1_r__64 * weight_real__66) - (y1_i__65 * weight_imag__67)))/*float*/;
      (y1w_i__69 = ((y1_r__64 * weight_imag__67) + (y1_i__65 * weight_real__66)))/*float*/;
      ((results__60[(int)i__59]) = (y0_r__62 + y1w_r__68))/*float*/;
      ((results__60[(int)(i__59 + 1)]) = (y0_i__63 + y1w_i__69))/*float*/;
      ((results__60[(int)(8 + i__59)]) = (y0_r__62 - y1w_r__68))/*float*/;
      ((results__60[(int)((8 + i__59) + 1)]) = (y0_i__63 - y1w_i__69))/*float*/;
    }
  }
  for ((i__59 = 0)/*int*/; (i__59 < 16); (i__59++)) {{
      (*____in++);
      ((*____out++)=(results__60[(int)i__59]));
    }
  }
  // mark end: SIRFilter CombineDFT

}}


void work_CombineDFT__75_37__7__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int i__59 = 0;/* int */
  float results__60[16] = {0};/* float[16] */
  int i_plus_1__61 = 0;/* int */
  float y0_r__62 = 0.0f;/* float */
  float y0_i__63 = 0.0f;/* float */
  float y1_r__64 = 0.0f;/* float */
  float y1_i__65 = 0.0f;/* float */
  float weight_real__66 = 0.0f;/* float */
  float weight_imag__67 = 0.0f;/* float */
  float y1w_r__68 = 0.0f;/* float */
  float y1w_i__69 = 0.0f;/* float */

  // mark begin: SIRFilter CombineDFT

  for ((i__59 = 0)/*int*/; (i__59 < 8); (i__59 = (i__59 + 2))/*int*/) {{
      (i_plus_1__61 = (i__59 + 1))/*int*/;
      (y0_r__62 = (*(____in+i__59)))/*float*/;
      (y0_i__63 = (*(____in+i_plus_1__61)))/*float*/;
      (y1_r__64 = (*(____in+(8 + i__59))))/*float*/;
      (y1_i__65 = (*(____in+(8 + i_plus_1__61))))/*float*/;
      (weight_real__66 = ((w__56__7)[(int)i__59]))/*float*/;
      (weight_imag__67 = ((w__56__7)[(int)i_plus_1__61]))/*float*/;
      (y1w_r__68 = ((y1_r__64 * weight_real__66) - (y1_i__65 * weight_imag__67)))/*float*/;
      (y1w_i__69 = ((y1_r__64 * weight_imag__67) + (y1_i__65 * weight_real__66)))/*float*/;
      ((results__60[(int)i__59]) = (y0_r__62 + y1w_r__68))/*float*/;
      ((results__60[(int)(i__59 + 1)]) = (y0_i__63 + y1w_i__69))/*float*/;
      ((results__60[(int)(8 + i__59)]) = (y0_r__62 - y1w_r__68))/*float*/;
      ((results__60[(int)((8 + i__59) + 1)]) = (y0_i__63 - y1w_i__69))/*float*/;
    }
  }
  for ((i__59 = 0)/*int*/; (i__59 < 16); (i__59++)) {{
      (*____in++);
      ((*____out++)=(results__60[(int)i__59]));
    }
  }
  // mark end: SIRFilter CombineDFT

}}


#endif // BUFFER_MERGE


 
void work_CombineDFT__75_37__7(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int i__59 = 0;/* int */
      float results__60[16] = {0};/* float[16] */
      int i_plus_1__61 = 0;/* int */
      float y0_r__62 = 0.0f;/* float */
      float y0_i__63 = 0.0f;/* float */
      float y1_r__64 = 0.0f;/* float */
      float y1_i__65 = 0.0f;/* float */
      float weight_real__66 = 0.0f;/* float */
      float weight_imag__67 = 0.0f;/* float */
      float y1w_r__68 = 0.0f;/* float */
      float y1w_i__69 = 0.0f;/* float */

      // mark begin: SIRFilter CombineDFT

      for ((i__59 = 0)/*int*/; (i__59 < 8); (i__59 = (i__59 + 2))/*int*/) {{
          (i_plus_1__61 = (i__59 + 1))/*int*/;
          (y0_r__62 = __peek__7(i__59))/*float*/;
          (y0_i__63 = __peek__7(i_plus_1__61))/*float*/;
          (y1_r__64 = __peek__7((8 + i__59)))/*float*/;
          (y1_i__65 = __peek__7((8 + i_plus_1__61)))/*float*/;
          (weight_real__66 = ((w__56__7)[(int)i__59]))/*float*/;
          (weight_imag__67 = ((w__56__7)[(int)i_plus_1__61]))/*float*/;
          (y1w_r__68 = ((y1_r__64 * weight_real__66) - (y1_i__65 * weight_imag__67)))/*float*/;
          (y1w_i__69 = ((y1_r__64 * weight_imag__67) + (y1_i__65 * weight_real__66)))/*float*/;
          ((results__60[(int)i__59]) = (y0_r__62 + y1w_r__68))/*float*/;
          ((results__60[(int)(i__59 + 1)]) = (y0_i__63 + y1w_i__69))/*float*/;
          ((results__60[(int)(8 + i__59)]) = (y0_r__62 - y1w_r__68))/*float*/;
          ((results__60[(int)((8 + i__59) + 1)]) = (y0_i__63 - y1w_i__69))/*float*/;
        }
      }
      for ((i__59 = 0)/*int*/; (i__59 < 16); (i__59++)) {{
          __pop__7();
          __push__7((results__60[(int)i__59]));
        }
      }
      // mark end: SIRFilter CombineDFT

    }
  }
}

// peek: 32 pop: 32 push 32
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_8;
int __counter_8 = 0;
int __steady_8 = 0;
int __tmp_8 = 0;
int __tmp2_8 = 0;
int *__state_flag_8 = NULL;
thread_info *__thread_8 = NULL;



float w__76__8[16] = {0};
void save_peek_buffer__8(object_write_buffer *buf);
void load_peek_buffer__8(object_write_buffer *buf);
void save_file_pointer__8(object_write_buffer *buf);
void load_file_pointer__8(object_write_buffer *buf);

 
void init_CombineDFT__95_38__8();
inline void check_status__8();

void work_CombineDFT__95_38__8(int);


inline float __pop__8() {
float res=BUFFER_7_8[TAIL_7_8];
TAIL_7_8++;
return res;
}

inline float __pop__8(int n) {
float res=BUFFER_7_8[TAIL_7_8];
TAIL_7_8+=n;

return res;
}

inline float __peek__8(int offs) {
return BUFFER_7_8[TAIL_7_8+offs];
}



inline void __push__8(float data) {
BUFFER_8_9[HEAD_8_9]=data;
HEAD_8_9++;
}



 
void init_CombineDFT__95_38__8(){
  float real__90 = 0.0f;/* float */
  float imag__91 = 0.0f;/* float */
  float next_real__92 = 0.0f;/* float */float next_imag__93 = 0.0f;/* float */
  int i__94 = 0;/* int */

  (real__90 = ((float)1.0))/*float*/;
  (imag__91 = ((float)0.0))/*float*/;
  for ((i__94 = 0)/*int*/; (i__94 < 16); (i__94 = (i__94 + 2))/*int*/) {{
      (((w__76__8)[(int)i__94]) = real__90)/*float*/;
      (((w__76__8)[(int)(i__94 + 1)]) = imag__91)/*float*/;
      (next_real__92 = ((real__90 * ((float)0.9238795)) - (imag__91 * ((float)-0.38268346))))/*float*/;
      (next_imag__93 = ((real__90 * ((float)-0.38268346)) + (imag__91 * ((float)0.9238795))))/*float*/;
      (real__90 = next_real__92)/*float*/;
      (imag__91 = next_imag__93)/*float*/;
    }
  }
}
void save_file_pointer__8(object_write_buffer *buf) {}
void load_file_pointer__8(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_CombineDFT__95_38__8__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  int i__79 = 0;/* int */
  float results__80[32] = {0};/* float[32] */
  int i_plus_1__81 = 0;/* int */
  float y0_r__82 = 0.0f;/* float */
  float y0_i__83 = 0.0f;/* float */
  float y1_r__84 = 0.0f;/* float */
  float y1_i__85 = 0.0f;/* float */
  float weight_real__86 = 0.0f;/* float */
  float weight_imag__87 = 0.0f;/* float */
  float y1w_r__88 = 0.0f;/* float */
  float y1w_i__89 = 0.0f;/* float */

  // mark begin: SIRFilter CombineDFT

  for ((i__79 = 0)/*int*/; (i__79 < 16); (i__79 = (i__79 + 2))/*int*/) {{
      (i_plus_1__81 = (i__79 + 1))/*int*/;
      (y0_r__82 = (*(____in+i__79)))/*float*/;
      (y0_i__83 = (*(____in+i_plus_1__81)))/*float*/;
      (y1_r__84 = (*(____in+(16 + i__79))))/*float*/;
      (y1_i__85 = (*(____in+(16 + i_plus_1__81))))/*float*/;
      (weight_real__86 = ((w__76__8)[(int)i__79]))/*float*/;
      (weight_imag__87 = ((w__76__8)[(int)i_plus_1__81]))/*float*/;
      (y1w_r__88 = ((y1_r__84 * weight_real__86) - (y1_i__85 * weight_imag__87)))/*float*/;
      (y1w_i__89 = ((y1_r__84 * weight_imag__87) + (y1_i__85 * weight_real__86)))/*float*/;
      ((results__80[(int)i__79]) = (y0_r__82 + y1w_r__88))/*float*/;
      ((results__80[(int)(i__79 + 1)]) = (y0_i__83 + y1w_i__89))/*float*/;
      ((results__80[(int)(16 + i__79)]) = (y0_r__82 - y1w_r__88))/*float*/;
      ((results__80[(int)((16 + i__79) + 1)]) = (y0_i__83 - y1w_i__89))/*float*/;
    }
  }
  for ((i__79 = 0)/*int*/; (i__79 < 32); (i__79++)) {{
      (*____in++);
      ((*____out++)=(results__80[(int)i__79]));
    }
  }
  // mark end: SIRFilter CombineDFT

}}


void work_CombineDFT__95_38__8__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int i__79 = 0;/* int */
  float results__80[32] = {0};/* float[32] */
  int i_plus_1__81 = 0;/* int */
  float y0_r__82 = 0.0f;/* float */
  float y0_i__83 = 0.0f;/* float */
  float y1_r__84 = 0.0f;/* float */
  float y1_i__85 = 0.0f;/* float */
  float weight_real__86 = 0.0f;/* float */
  float weight_imag__87 = 0.0f;/* float */
  float y1w_r__88 = 0.0f;/* float */
  float y1w_i__89 = 0.0f;/* float */

  // mark begin: SIRFilter CombineDFT

  for ((i__79 = 0)/*int*/; (i__79 < 16); (i__79 = (i__79 + 2))/*int*/) {{
      (i_plus_1__81 = (i__79 + 1))/*int*/;
      (y0_r__82 = (*(____in+i__79)))/*float*/;
      (y0_i__83 = (*(____in+i_plus_1__81)))/*float*/;
      (y1_r__84 = (*(____in+(16 + i__79))))/*float*/;
      (y1_i__85 = (*(____in+(16 + i_plus_1__81))))/*float*/;
      (weight_real__86 = ((w__76__8)[(int)i__79]))/*float*/;
      (weight_imag__87 = ((w__76__8)[(int)i_plus_1__81]))/*float*/;
      (y1w_r__88 = ((y1_r__84 * weight_real__86) - (y1_i__85 * weight_imag__87)))/*float*/;
      (y1w_i__89 = ((y1_r__84 * weight_imag__87) + (y1_i__85 * weight_real__86)))/*float*/;
      ((results__80[(int)i__79]) = (y0_r__82 + y1w_r__88))/*float*/;
      ((results__80[(int)(i__79 + 1)]) = (y0_i__83 + y1w_i__89))/*float*/;
      ((results__80[(int)(16 + i__79)]) = (y0_r__82 - y1w_r__88))/*float*/;
      ((results__80[(int)((16 + i__79) + 1)]) = (y0_i__83 - y1w_i__89))/*float*/;
    }
  }
  for ((i__79 = 0)/*int*/; (i__79 < 32); (i__79++)) {{
      (*____in++);
      ((*____out++)=(results__80[(int)i__79]));
    }
  }
  // mark end: SIRFilter CombineDFT

}}


#endif // BUFFER_MERGE


 
void work_CombineDFT__95_38__8(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int i__79 = 0;/* int */
      float results__80[32] = {0};/* float[32] */
      int i_plus_1__81 = 0;/* int */
      float y0_r__82 = 0.0f;/* float */
      float y0_i__83 = 0.0f;/* float */
      float y1_r__84 = 0.0f;/* float */
      float y1_i__85 = 0.0f;/* float */
      float weight_real__86 = 0.0f;/* float */
      float weight_imag__87 = 0.0f;/* float */
      float y1w_r__88 = 0.0f;/* float */
      float y1w_i__89 = 0.0f;/* float */

      // mark begin: SIRFilter CombineDFT

      for ((i__79 = 0)/*int*/; (i__79 < 16); (i__79 = (i__79 + 2))/*int*/) {{
          (i_plus_1__81 = (i__79 + 1))/*int*/;
          (y0_r__82 = __peek__8(i__79))/*float*/;
          (y0_i__83 = __peek__8(i_plus_1__81))/*float*/;
          (y1_r__84 = __peek__8((16 + i__79)))/*float*/;
          (y1_i__85 = __peek__8((16 + i_plus_1__81)))/*float*/;
          (weight_real__86 = ((w__76__8)[(int)i__79]))/*float*/;
          (weight_imag__87 = ((w__76__8)[(int)i_plus_1__81]))/*float*/;
          (y1w_r__88 = ((y1_r__84 * weight_real__86) - (y1_i__85 * weight_imag__87)))/*float*/;
          (y1w_i__89 = ((y1_r__84 * weight_imag__87) + (y1_i__85 * weight_real__86)))/*float*/;
          ((results__80[(int)i__79]) = (y0_r__82 + y1w_r__88))/*float*/;
          ((results__80[(int)(i__79 + 1)]) = (y0_i__83 + y1w_i__89))/*float*/;
          ((results__80[(int)(16 + i__79)]) = (y0_r__82 - y1w_r__88))/*float*/;
          ((results__80[(int)((16 + i__79) + 1)]) = (y0_i__83 - y1w_i__89))/*float*/;
        }
      }
      for ((i__79 = 0)/*int*/; (i__79 < 32); (i__79++)) {{
          __pop__8();
          __push__8((results__80[(int)i__79]));
        }
      }
      // mark end: SIRFilter CombineDFT

    }
  }
}

// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_9;
int __counter_9 = 0;
int __steady_9 = 0;
int __tmp_9 = 0;
int __tmp2_9 = 0;
int *__state_flag_9 = NULL;
thread_info *__thread_9 = NULL;




void __joiner_9_work(int ____n) {
  for (;____n > 0; ____n--) {
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
HEAD_9_10++; ; TAIL_8_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
BUFFER_9_10[HEAD_9_10]=BUFFER_17_9[TAIL_17_9];
HEAD_9_10++; ; TAIL_17_9++;
  }
}


// peek: 1 pop: 1 push 0
// init counts: 0 steady counts: 64

// ClusterFusion isEliminated: false



int __number_of_iterations_10;
int __counter_10 = 0;
int __steady_10 = 0;
int __tmp_10 = 0;
int __tmp2_10 = 0;
int *__state_flag_10 = NULL;
thread_info *__thread_10 = NULL;



float x__188__10 = 0.0f;
void save_peek_buffer__10(object_write_buffer *buf);
void load_peek_buffer__10(object_write_buffer *buf);
void save_file_pointer__10(object_write_buffer *buf);
void load_file_pointer__10(object_write_buffer *buf);

 
void init_FloatPrinter__191_46__10();
inline void check_status__10();

void work_FloatPrinter__191_46__10(int);


inline float __pop__10() {
float res=BUFFER_9_10[TAIL_9_10];
TAIL_9_10++;
return res;
}

inline float __pop__10(int n) {
float res=BUFFER_9_10[TAIL_9_10];
TAIL_9_10+=n;

return res;
}

inline float __peek__10(int offs) {
return BUFFER_9_10[TAIL_9_10+offs];
}


 
void init_FloatPrinter__191_46__10(){
}
void save_file_pointer__10(object_write_buffer *buf) {}
void load_file_pointer__10(object_write_buffer *buf) {}
 
void work_FloatPrinter__191_46__10(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter FloatPrinter

      ((x__188__10) = __pop__10())/*float*/;
      sum+=x__188__10;
      //printf("%f\n", x__188__10);
      // mark end: SIRFilter FloatPrinter

    }
  }
}

// peek: 32 pop: 32 push 32
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_11;
int __counter_11 = 0;
int __steady_11 = 0;
int __tmp_11 = 0;
int __tmp2_11 = 0;
int *__state_flag_11 = NULL;
thread_info *__thread_11 = NULL;



void save_peek_buffer__11(object_write_buffer *buf);
void load_peek_buffer__11(object_write_buffer *buf);
void save_file_pointer__11(object_write_buffer *buf);
void load_file_pointer__11(object_write_buffer *buf);

 
void init_FFTReorderSimple__99_39__11();
inline void check_status__11();

void work_FFTReorderSimple__99_39__11(int);


inline float __pop__11() {
float res=BUFFER_1_11[TAIL_1_11];
TAIL_1_11++;
return res;
}

inline float __pop__11(int n) {
float res=BUFFER_1_11[TAIL_1_11];
TAIL_1_11+=n;

return res;
}

inline float __peek__11(int offs) {
return BUFFER_1_11[TAIL_1_11+offs];
}



inline void __push__11(float data) {
BUFFER_11_12[HEAD_11_12]=data;
HEAD_11_12++;
}



 
void init_FFTReorderSimple__99_39__11(){
}
void save_file_pointer__11(object_write_buffer *buf) {}
void load_file_pointer__11(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_FFTReorderSimple__99_39__11__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  int i__98 = 0;/* int */

  // mark begin: SIRFilter FFTReorderSimple

  for ((i__98 = 0)/*int*/; (i__98 < 32); (i__98 = (i__98 + 4))/*int*/) {{
      ((*____out++)=(*(____in+i__98)));
      ((*____out++)=(*(____in+(i__98 + 1))));
    }
  }
  for ((i__98 = 2)/*int*/; (i__98 < 32); (i__98 = (i__98 + 4))/*int*/) {{
      ((*____out++)=(*(____in+i__98)));
      ((*____out++)=(*(____in+(i__98 + 1))));
    }
  }
  for ((i__98 = 0)/*int*/; (i__98 < 16); (i__98++)) {{
      assert(false);__pop__11(2);
    }
  }
  // mark end: SIRFilter FFTReorderSimple

}}


void work_FFTReorderSimple__99_39__11__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int i__98 = 0;/* int */

  // mark begin: SIRFilter FFTReorderSimple

  for ((i__98 = 0)/*int*/; (i__98 < 32); (i__98 = (i__98 + 4))/*int*/) {{
      ((*____out++)=(*(____in+i__98)));
      ((*____out++)=(*(____in+(i__98 + 1))));
    }
  }
  for ((i__98 = 2)/*int*/; (i__98 < 32); (i__98 = (i__98 + 4))/*int*/) {{
      ((*____out++)=(*(____in+i__98)));
      ((*____out++)=(*(____in+(i__98 + 1))));
    }
  }
  for ((i__98 = 0)/*int*/; (i__98 < 16); (i__98++)) {{
      assert(false);__pop__11(2);
    }
  }
  // mark end: SIRFilter FFTReorderSimple

}}


#endif // BUFFER_MERGE


 
void work_FFTReorderSimple__99_39__11(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int i__98 = 0;/* int */

      // mark begin: SIRFilter FFTReorderSimple

      for ((i__98 = 0)/*int*/; (i__98 < 32); (i__98 = (i__98 + 4))/*int*/) {{
          __push__11(__peek__11(i__98));
          __push__11(__peek__11((i__98 + 1)));
        }
      }
      for ((i__98 = 2)/*int*/; (i__98 < 32); (i__98 = (i__98 + 4))/*int*/) {{
          __push__11(__peek__11(i__98));
          __push__11(__peek__11((i__98 + 1)));
        }
      }
      for ((i__98 = 0)/*int*/; (i__98 < 16); (i__98++)) {{
          __pop__11(2);
        }
      }
      // mark end: SIRFilter FFTReorderSimple

    }
  }
}

// peek: 16 pop: 16 push 16
// init counts: 0 steady counts: 2

// ClusterFusion isEliminated: false



int __number_of_iterations_12;
int __counter_12 = 0;
int __steady_12 = 0;
int __tmp_12 = 0;
int __tmp2_12 = 0;
int *__state_flag_12 = NULL;
thread_info *__thread_12 = NULL;



void save_peek_buffer__12(object_write_buffer *buf);
void load_peek_buffer__12(object_write_buffer *buf);
void save_file_pointer__12(object_write_buffer *buf);
void load_file_pointer__12(object_write_buffer *buf);

 
void init_FFTReorderSimple__103_40__12();
inline void check_status__12();

void work_FFTReorderSimple__103_40__12(int);


inline float __pop__12() {
float res=BUFFER_11_12[TAIL_11_12];
TAIL_11_12++;
return res;
}

inline float __pop__12(int n) {
float res=BUFFER_11_12[TAIL_11_12];
TAIL_11_12+=n;

return res;
}

inline float __peek__12(int offs) {
return BUFFER_11_12[TAIL_11_12+offs];
}



inline void __push__12(float data) {
BUFFER_12_13[HEAD_12_13]=data;
HEAD_12_13++;
}



 
void init_FFTReorderSimple__103_40__12(){
}
void save_file_pointer__12(object_write_buffer *buf) {}
void load_file_pointer__12(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_FFTReorderSimple__103_40__12__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  int i__102 = 0;/* int */

  // mark begin: SIRFilter FFTReorderSimple

  for ((i__102 = 0)/*int*/; (i__102 < 16); (i__102 = (i__102 + 4))/*int*/) {{
      ((*____out++)=(*(____in+i__102)));
      ((*____out++)=(*(____in+(i__102 + 1))));
    }
  }
  for ((i__102 = 2)/*int*/; (i__102 < 16); (i__102 = (i__102 + 4))/*int*/) {{
      ((*____out++)=(*(____in+i__102)));
      ((*____out++)=(*(____in+(i__102 + 1))));
    }
  }
  for ((i__102 = 0)/*int*/; (i__102 < 8); (i__102++)) {{
      assert(false);__pop__12(2);
    }
  }
  // mark end: SIRFilter FFTReorderSimple

}}


void work_FFTReorderSimple__103_40__12__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int i__102 = 0;/* int */

  // mark begin: SIRFilter FFTReorderSimple

  for ((i__102 = 0)/*int*/; (i__102 < 16); (i__102 = (i__102 + 4))/*int*/) {{
      ((*____out++)=(*(____in+i__102)));
      ((*____out++)=(*(____in+(i__102 + 1))));
    }
  }
  for ((i__102 = 2)/*int*/; (i__102 < 16); (i__102 = (i__102 + 4))/*int*/) {{
      ((*____out++)=(*(____in+i__102)));
      ((*____out++)=(*(____in+(i__102 + 1))));
    }
  }
  for ((i__102 = 0)/*int*/; (i__102 < 8); (i__102++)) {{
      assert(false);__pop__12(2);
    }
  }
  // mark end: SIRFilter FFTReorderSimple

}}


#endif // BUFFER_MERGE


 
void work_FFTReorderSimple__103_40__12(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int i__102 = 0;/* int */

      // mark begin: SIRFilter FFTReorderSimple

      for ((i__102 = 0)/*int*/; (i__102 < 16); (i__102 = (i__102 + 4))/*int*/) {{
          __push__12(__peek__12(i__102));
          __push__12(__peek__12((i__102 + 1)));
        }
      }
      for ((i__102 = 2)/*int*/; (i__102 < 16); (i__102 = (i__102 + 4))/*int*/) {{
          __push__12(__peek__12(i__102));
          __push__12(__peek__12((i__102 + 1)));
        }
      }
      for ((i__102 = 0)/*int*/; (i__102 < 8); (i__102++)) {{
          __pop__12(2);
        }
      }
      // mark end: SIRFilter FFTReorderSimple

    }
  }
}

// peek: 8 pop: 8 push 8
// init counts: 0 steady counts: 4

// ClusterFusion isEliminated: false



int __number_of_iterations_13;
int __counter_13 = 0;
int __steady_13 = 0;
int __tmp_13 = 0;
int __tmp2_13 = 0;
int *__state_flag_13 = NULL;
thread_info *__thread_13 = NULL;



void save_peek_buffer__13(object_write_buffer *buf);
void load_peek_buffer__13(object_write_buffer *buf);
void save_file_pointer__13(object_write_buffer *buf);
void load_file_pointer__13(object_write_buffer *buf);

 
void init_FFTReorderSimple__107_41__13();
inline void check_status__13();

void work_FFTReorderSimple__107_41__13(int);


inline float __pop__13() {
float res=BUFFER_12_13[TAIL_12_13];
TAIL_12_13++;
return res;
}

inline float __pop__13(int n) {
float res=BUFFER_12_13[TAIL_12_13];
TAIL_12_13+=n;

return res;
}

inline float __peek__13(int offs) {
return BUFFER_12_13[TAIL_12_13+offs];
}



inline void __push__13(float data) {
BUFFER_13_14[HEAD_13_14]=data;
HEAD_13_14++;
}



 
void init_FFTReorderSimple__107_41__13(){
}
void save_file_pointer__13(object_write_buffer *buf) {}
void load_file_pointer__13(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_FFTReorderSimple__107_41__13__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  int i__106 = 0;/* int */

  // mark begin: SIRFilter FFTReorderSimple

  for ((i__106 = 0)/*int*/; (i__106 < 8); (i__106 = (i__106 + 4))/*int*/) {{
      ((*____out++)=(*(____in+i__106)));
      ((*____out++)=(*(____in+(i__106 + 1))));
    }
  }
  for ((i__106 = 2)/*int*/; (i__106 < 8); (i__106 = (i__106 + 4))/*int*/) {{
      ((*____out++)=(*(____in+i__106)));
      ((*____out++)=(*(____in+(i__106 + 1))));
    }
  }
  for ((i__106 = 0)/*int*/; (i__106 < 4); (i__106++)) {{
      assert(false);__pop__13(2);
    }
  }
  // mark end: SIRFilter FFTReorderSimple

}}


void work_FFTReorderSimple__107_41__13__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int i__106 = 0;/* int */

  // mark begin: SIRFilter FFTReorderSimple

  for ((i__106 = 0)/*int*/; (i__106 < 8); (i__106 = (i__106 + 4))/*int*/) {{
      ((*____out++)=(*(____in+i__106)));
      ((*____out++)=(*(____in+(i__106 + 1))));
    }
  }
  for ((i__106 = 2)/*int*/; (i__106 < 8); (i__106 = (i__106 + 4))/*int*/) {{
      ((*____out++)=(*(____in+i__106)));
      ((*____out++)=(*(____in+(i__106 + 1))));
    }
  }
  for ((i__106 = 0)/*int*/; (i__106 < 4); (i__106++)) {{
      assert(false);__pop__13(2);
    }
  }
  // mark end: SIRFilter FFTReorderSimple

}}


#endif // BUFFER_MERGE


 
void work_FFTReorderSimple__107_41__13(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int i__106 = 0;/* int */

      // mark begin: SIRFilter FFTReorderSimple

      for ((i__106 = 0)/*int*/; (i__106 < 8); (i__106 = (i__106 + 4))/*int*/) {{
          __push__13(__peek__13(i__106));
          __push__13(__peek__13((i__106 + 1)));
        }
      }
      for ((i__106 = 2)/*int*/; (i__106 < 8); (i__106 = (i__106 + 4))/*int*/) {{
          __push__13(__peek__13(i__106));
          __push__13(__peek__13((i__106 + 1)));
        }
      }
      for ((i__106 = 0)/*int*/; (i__106 < 4); (i__106++)) {{
          __pop__13(2);
        }
      }
      // mark end: SIRFilter FFTReorderSimple

    }
  }
}

// peek: 4 pop: 4 push 4
// init counts: 0 steady counts: 8

// ClusterFusion isEliminated: false



int __number_of_iterations_14;
int __counter_14 = 0;
int __steady_14 = 0;
int __tmp_14 = 0;
int __tmp2_14 = 0;
int *__state_flag_14 = NULL;
thread_info *__thread_14 = NULL;



float w__108__14[2] = {0};
void save_peek_buffer__14(object_write_buffer *buf);
void load_peek_buffer__14(object_write_buffer *buf);
void save_file_pointer__14(object_write_buffer *buf);
void load_file_pointer__14(object_write_buffer *buf);

 
void init_CombineDFT__127_42__14();
inline void check_status__14();

void work_CombineDFT__127_42__14(int);


inline float __pop__14() {
float res=BUFFER_13_14[TAIL_13_14];
TAIL_13_14++;
return res;
}

inline float __pop__14(int n) {
float res=BUFFER_13_14[TAIL_13_14];
TAIL_13_14+=n;

return res;
}

inline float __peek__14(int offs) {
return BUFFER_13_14[TAIL_13_14+offs];
}



inline void __push__14(float data) {
BUFFER_14_15[HEAD_14_15]=data;
HEAD_14_15++;
}



 
void init_CombineDFT__127_42__14(){
  float real__122 = 0.0f;/* float */
  float imag__123 = 0.0f;/* float */
  float next_real__124 = 0.0f;/* float */float next_imag__125 = 0.0f;/* float */
  int i__126 = 0;/* int */

  (real__122 = ((float)1.0))/*float*/;
  (imag__123 = ((float)0.0))/*float*/;
  for ((i__126 = 0)/*int*/; (i__126 < 2); (i__126 = (i__126 + 2))/*int*/) {{
      (((w__108__14)[(int)i__126]) = real__122)/*float*/;
      (((w__108__14)[(int)(i__126 + 1)]) = imag__123)/*float*/;
      (next_real__124 = ((real__122 * ((float)-1.0)) - (imag__123 * ((float)8.742278E-8))))/*float*/;
      (next_imag__125 = ((real__122 * ((float)8.742278E-8)) + (imag__123 * ((float)-1.0))))/*float*/;
      (real__122 = next_real__124)/*float*/;
      (imag__123 = next_imag__125)/*float*/;
    }
  }
}
void save_file_pointer__14(object_write_buffer *buf) {}
void load_file_pointer__14(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_CombineDFT__127_42__14__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  int i__111 = 0;/* int */
  float results__112[4] = {0};/* float[4] */
  int i_plus_1__113 = 0;/* int */
  float y0_r__114 = 0.0f;/* float */
  float y0_i__115 = 0.0f;/* float */
  float y1_r__116 = 0.0f;/* float */
  float y1_i__117 = 0.0f;/* float */
  float weight_real__118 = 0.0f;/* float */
  float weight_imag__119 = 0.0f;/* float */
  float y1w_r__120 = 0.0f;/* float */
  float y1w_i__121 = 0.0f;/* float */

  // mark begin: SIRFilter CombineDFT

  for ((i__111 = 0)/*int*/; (i__111 < 2); (i__111 = (i__111 + 2))/*int*/) {{
      (i_plus_1__113 = (i__111 + 1))/*int*/;
      (y0_r__114 = (*(____in+i__111)))/*float*/;
      (y0_i__115 = (*(____in+i_plus_1__113)))/*float*/;
      (y1_r__116 = (*(____in+(2 + i__111))))/*float*/;
      (y1_i__117 = (*(____in+(2 + i_plus_1__113))))/*float*/;
      (weight_real__118 = ((w__108__14)[(int)i__111]))/*float*/;
      (weight_imag__119 = ((w__108__14)[(int)i_plus_1__113]))/*float*/;
      (y1w_r__120 = ((y1_r__116 * weight_real__118) - (y1_i__117 * weight_imag__119)))/*float*/;
      (y1w_i__121 = ((y1_r__116 * weight_imag__119) + (y1_i__117 * weight_real__118)))/*float*/;
      ((results__112[(int)i__111]) = (y0_r__114 + y1w_r__120))/*float*/;
      ((results__112[(int)(i__111 + 1)]) = (y0_i__115 + y1w_i__121))/*float*/;
      ((results__112[(int)(2 + i__111)]) = (y0_r__114 - y1w_r__120))/*float*/;
      ((results__112[(int)((2 + i__111) + 1)]) = (y0_i__115 - y1w_i__121))/*float*/;
    }
  }
  for ((i__111 = 0)/*int*/; (i__111 < 4); (i__111++)) {{
      (*____in++);
      ((*____out++)=(results__112[(int)i__111]));
    }
  }
  // mark end: SIRFilter CombineDFT

}}


void work_CombineDFT__127_42__14__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int i__111 = 0;/* int */
  float results__112[4] = {0};/* float[4] */
  int i_plus_1__113 = 0;/* int */
  float y0_r__114 = 0.0f;/* float */
  float y0_i__115 = 0.0f;/* float */
  float y1_r__116 = 0.0f;/* float */
  float y1_i__117 = 0.0f;/* float */
  float weight_real__118 = 0.0f;/* float */
  float weight_imag__119 = 0.0f;/* float */
  float y1w_r__120 = 0.0f;/* float */
  float y1w_i__121 = 0.0f;/* float */

  // mark begin: SIRFilter CombineDFT

  for ((i__111 = 0)/*int*/; (i__111 < 2); (i__111 = (i__111 + 2))/*int*/) {{
      (i_plus_1__113 = (i__111 + 1))/*int*/;
      (y0_r__114 = (*(____in+i__111)))/*float*/;
      (y0_i__115 = (*(____in+i_plus_1__113)))/*float*/;
      (y1_r__116 = (*(____in+(2 + i__111))))/*float*/;
      (y1_i__117 = (*(____in+(2 + i_plus_1__113))))/*float*/;
      (weight_real__118 = ((w__108__14)[(int)i__111]))/*float*/;
      (weight_imag__119 = ((w__108__14)[(int)i_plus_1__113]))/*float*/;
      (y1w_r__120 = ((y1_r__116 * weight_real__118) - (y1_i__117 * weight_imag__119)))/*float*/;
      (y1w_i__121 = ((y1_r__116 * weight_imag__119) + (y1_i__117 * weight_real__118)))/*float*/;
      ((results__112[(int)i__111]) = (y0_r__114 + y1w_r__120))/*float*/;
      ((results__112[(int)(i__111 + 1)]) = (y0_i__115 + y1w_i__121))/*float*/;
      ((results__112[(int)(2 + i__111)]) = (y0_r__114 - y1w_r__120))/*float*/;
      ((results__112[(int)((2 + i__111) + 1)]) = (y0_i__115 - y1w_i__121))/*float*/;
    }
  }
  for ((i__111 = 0)/*int*/; (i__111 < 4); (i__111++)) {{
      (*____in++);
      ((*____out++)=(results__112[(int)i__111]));
    }
  }
  // mark end: SIRFilter CombineDFT

}}


#endif // BUFFER_MERGE


 
void work_CombineDFT__127_42__14(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int i__111 = 0;/* int */
      float results__112[4] = {0};/* float[4] */
      int i_plus_1__113 = 0;/* int */
      float y0_r__114 = 0.0f;/* float */
      float y0_i__115 = 0.0f;/* float */
      float y1_r__116 = 0.0f;/* float */
      float y1_i__117 = 0.0f;/* float */
      float weight_real__118 = 0.0f;/* float */
      float weight_imag__119 = 0.0f;/* float */
      float y1w_r__120 = 0.0f;/* float */
      float y1w_i__121 = 0.0f;/* float */

      // mark begin: SIRFilter CombineDFT

      for ((i__111 = 0)/*int*/; (i__111 < 2); (i__111 = (i__111 + 2))/*int*/) {{
          (i_plus_1__113 = (i__111 + 1))/*int*/;
          (y0_r__114 = __peek__14(i__111))/*float*/;
          (y0_i__115 = __peek__14(i_plus_1__113))/*float*/;
          (y1_r__116 = __peek__14((2 + i__111)))/*float*/;
          (y1_i__117 = __peek__14((2 + i_plus_1__113)))/*float*/;
          (weight_real__118 = ((w__108__14)[(int)i__111]))/*float*/;
          (weight_imag__119 = ((w__108__14)[(int)i_plus_1__113]))/*float*/;
          (y1w_r__120 = ((y1_r__116 * weight_real__118) - (y1_i__117 * weight_imag__119)))/*float*/;
          (y1w_i__121 = ((y1_r__116 * weight_imag__119) + (y1_i__117 * weight_real__118)))/*float*/;
          ((results__112[(int)i__111]) = (y0_r__114 + y1w_r__120))/*float*/;
          ((results__112[(int)(i__111 + 1)]) = (y0_i__115 + y1w_i__121))/*float*/;
          ((results__112[(int)(2 + i__111)]) = (y0_r__114 - y1w_r__120))/*float*/;
          ((results__112[(int)((2 + i__111) + 1)]) = (y0_i__115 - y1w_i__121))/*float*/;
        }
      }
      for ((i__111 = 0)/*int*/; (i__111 < 4); (i__111++)) {{
          __pop__14();
          __push__14((results__112[(int)i__111]));
        }
      }
      // mark end: SIRFilter CombineDFT

    }
  }
}

// peek: 8 pop: 8 push 8
// init counts: 0 steady counts: 4

// ClusterFusion isEliminated: false



int __number_of_iterations_15;
int __counter_15 = 0;
int __steady_15 = 0;
int __tmp_15 = 0;
int __tmp2_15 = 0;
int *__state_flag_15 = NULL;
thread_info *__thread_15 = NULL;



float w__128__15[4] = {0};
void save_peek_buffer__15(object_write_buffer *buf);
void load_peek_buffer__15(object_write_buffer *buf);
void save_file_pointer__15(object_write_buffer *buf);
void load_file_pointer__15(object_write_buffer *buf);

 
void init_CombineDFT__147_43__15();
inline void check_status__15();

void work_CombineDFT__147_43__15(int);


inline float __pop__15() {
float res=BUFFER_14_15[TAIL_14_15];
TAIL_14_15++;
return res;
}

inline float __pop__15(int n) {
float res=BUFFER_14_15[TAIL_14_15];
TAIL_14_15+=n;

return res;
}

inline float __peek__15(int offs) {
return BUFFER_14_15[TAIL_14_15+offs];
}



inline void __push__15(float data) {
BUFFER_15_16[HEAD_15_16]=data;
HEAD_15_16++;
}



 
void init_CombineDFT__147_43__15(){
  float real__142 = 0.0f;/* float */
  float imag__143 = 0.0f;/* float */
  float next_real__144 = 0.0f;/* float */float next_imag__145 = 0.0f;/* float */
  int i__146 = 0;/* int */

  (real__142 = ((float)1.0))/*float*/;
  (imag__143 = ((float)0.0))/*float*/;
  for ((i__146 = 0)/*int*/; (i__146 < 4); (i__146 = (i__146 + 2))/*int*/) {{
      (((w__128__15)[(int)i__146]) = real__142)/*float*/;
      (((w__128__15)[(int)(i__146 + 1)]) = imag__143)/*float*/;
      (next_real__144 = ((real__142 * ((float)-4.371139E-8)) - (imag__143 * ((float)-1.0))))/*float*/;
      (next_imag__145 = ((real__142 * ((float)-1.0)) + (imag__143 * ((float)-4.371139E-8))))/*float*/;
      (real__142 = next_real__144)/*float*/;
      (imag__143 = next_imag__145)/*float*/;
    }
  }
}
void save_file_pointer__15(object_write_buffer *buf) {}
void load_file_pointer__15(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_CombineDFT__147_43__15__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  int i__131 = 0;/* int */
  float results__132[8] = {0};/* float[8] */
  int i_plus_1__133 = 0;/* int */
  float y0_r__134 = 0.0f;/* float */
  float y0_i__135 = 0.0f;/* float */
  float y1_r__136 = 0.0f;/* float */
  float y1_i__137 = 0.0f;/* float */
  float weight_real__138 = 0.0f;/* float */
  float weight_imag__139 = 0.0f;/* float */
  float y1w_r__140 = 0.0f;/* float */
  float y1w_i__141 = 0.0f;/* float */

  // mark begin: SIRFilter CombineDFT

  for ((i__131 = 0)/*int*/; (i__131 < 4); (i__131 = (i__131 + 2))/*int*/) {{
      (i_plus_1__133 = (i__131 + 1))/*int*/;
      (y0_r__134 = (*(____in+i__131)))/*float*/;
      (y0_i__135 = (*(____in+i_plus_1__133)))/*float*/;
      (y1_r__136 = (*(____in+(4 + i__131))))/*float*/;
      (y1_i__137 = (*(____in+(4 + i_plus_1__133))))/*float*/;
      (weight_real__138 = ((w__128__15)[(int)i__131]))/*float*/;
      (weight_imag__139 = ((w__128__15)[(int)i_plus_1__133]))/*float*/;
      (y1w_r__140 = ((y1_r__136 * weight_real__138) - (y1_i__137 * weight_imag__139)))/*float*/;
      (y1w_i__141 = ((y1_r__136 * weight_imag__139) + (y1_i__137 * weight_real__138)))/*float*/;
      ((results__132[(int)i__131]) = (y0_r__134 + y1w_r__140))/*float*/;
      ((results__132[(int)(i__131 + 1)]) = (y0_i__135 + y1w_i__141))/*float*/;
      ((results__132[(int)(4 + i__131)]) = (y0_r__134 - y1w_r__140))/*float*/;
      ((results__132[(int)((4 + i__131) + 1)]) = (y0_i__135 - y1w_i__141))/*float*/;
    }
  }
  for ((i__131 = 0)/*int*/; (i__131 < 8); (i__131++)) {{
      (*____in++);
      ((*____out++)=(results__132[(int)i__131]));
    }
  }
  // mark end: SIRFilter CombineDFT

}}


void work_CombineDFT__147_43__15__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int i__131 = 0;/* int */
  float results__132[8] = {0};/* float[8] */
  int i_plus_1__133 = 0;/* int */
  float y0_r__134 = 0.0f;/* float */
  float y0_i__135 = 0.0f;/* float */
  float y1_r__136 = 0.0f;/* float */
  float y1_i__137 = 0.0f;/* float */
  float weight_real__138 = 0.0f;/* float */
  float weight_imag__139 = 0.0f;/* float */
  float y1w_r__140 = 0.0f;/* float */
  float y1w_i__141 = 0.0f;/* float */

  // mark begin: SIRFilter CombineDFT

  for ((i__131 = 0)/*int*/; (i__131 < 4); (i__131 = (i__131 + 2))/*int*/) {{
      (i_plus_1__133 = (i__131 + 1))/*int*/;
      (y0_r__134 = (*(____in+i__131)))/*float*/;
      (y0_i__135 = (*(____in+i_plus_1__133)))/*float*/;
      (y1_r__136 = (*(____in+(4 + i__131))))/*float*/;
      (y1_i__137 = (*(____in+(4 + i_plus_1__133))))/*float*/;
      (weight_real__138 = ((w__128__15)[(int)i__131]))/*float*/;
      (weight_imag__139 = ((w__128__15)[(int)i_plus_1__133]))/*float*/;
      (y1w_r__140 = ((y1_r__136 * weight_real__138) - (y1_i__137 * weight_imag__139)))/*float*/;
      (y1w_i__141 = ((y1_r__136 * weight_imag__139) + (y1_i__137 * weight_real__138)))/*float*/;
      ((results__132[(int)i__131]) = (y0_r__134 + y1w_r__140))/*float*/;
      ((results__132[(int)(i__131 + 1)]) = (y0_i__135 + y1w_i__141))/*float*/;
      ((results__132[(int)(4 + i__131)]) = (y0_r__134 - y1w_r__140))/*float*/;
      ((results__132[(int)((4 + i__131) + 1)]) = (y0_i__135 - y1w_i__141))/*float*/;
    }
  }
  for ((i__131 = 0)/*int*/; (i__131 < 8); (i__131++)) {{
      (*____in++);
      ((*____out++)=(results__132[(int)i__131]));
    }
  }
  // mark end: SIRFilter CombineDFT

}}


#endif // BUFFER_MERGE


 
void work_CombineDFT__147_43__15(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int i__131 = 0;/* int */
      float results__132[8] = {0};/* float[8] */
      int i_plus_1__133 = 0;/* int */
      float y0_r__134 = 0.0f;/* float */
      float y0_i__135 = 0.0f;/* float */
      float y1_r__136 = 0.0f;/* float */
      float y1_i__137 = 0.0f;/* float */
      float weight_real__138 = 0.0f;/* float */
      float weight_imag__139 = 0.0f;/* float */
      float y1w_r__140 = 0.0f;/* float */
      float y1w_i__141 = 0.0f;/* float */

      // mark begin: SIRFilter CombineDFT

      for ((i__131 = 0)/*int*/; (i__131 < 4); (i__131 = (i__131 + 2))/*int*/) {{
          (i_plus_1__133 = (i__131 + 1))/*int*/;
          (y0_r__134 = __peek__15(i__131))/*float*/;
          (y0_i__135 = __peek__15(i_plus_1__133))/*float*/;
          (y1_r__136 = __peek__15((4 + i__131)))/*float*/;
          (y1_i__137 = __peek__15((4 + i_plus_1__133)))/*float*/;
          (weight_real__138 = ((w__128__15)[(int)i__131]))/*float*/;
          (weight_imag__139 = ((w__128__15)[(int)i_plus_1__133]))/*float*/;
          (y1w_r__140 = ((y1_r__136 * weight_real__138) - (y1_i__137 * weight_imag__139)))/*float*/;
          (y1w_i__141 = ((y1_r__136 * weight_imag__139) + (y1_i__137 * weight_real__138)))/*float*/;
          ((results__132[(int)i__131]) = (y0_r__134 + y1w_r__140))/*float*/;
          ((results__132[(int)(i__131 + 1)]) = (y0_i__135 + y1w_i__141))/*float*/;
          ((results__132[(int)(4 + i__131)]) = (y0_r__134 - y1w_r__140))/*float*/;
          ((results__132[(int)((4 + i__131) + 1)]) = (y0_i__135 - y1w_i__141))/*float*/;
        }
      }
      for ((i__131 = 0)/*int*/; (i__131 < 8); (i__131++)) {{
          __pop__15();
          __push__15((results__132[(int)i__131]));
        }
      }
      // mark end: SIRFilter CombineDFT

    }
  }
}

// peek: 16 pop: 16 push 16
// init counts: 0 steady counts: 2

// ClusterFusion isEliminated: false



int __number_of_iterations_16;
int __counter_16 = 0;
int __steady_16 = 0;
int __tmp_16 = 0;
int __tmp2_16 = 0;
int *__state_flag_16 = NULL;
thread_info *__thread_16 = NULL;



float w__148__16[8] = {0};
void save_peek_buffer__16(object_write_buffer *buf);
void load_peek_buffer__16(object_write_buffer *buf);
void save_file_pointer__16(object_write_buffer *buf);
void load_file_pointer__16(object_write_buffer *buf);

 
void init_CombineDFT__167_44__16();
inline void check_status__16();

void work_CombineDFT__167_44__16(int);


inline float __pop__16() {
float res=BUFFER_15_16[TAIL_15_16];
TAIL_15_16++;
return res;
}

inline float __pop__16(int n) {
float res=BUFFER_15_16[TAIL_15_16];
TAIL_15_16+=n;

return res;
}

inline float __peek__16(int offs) {
return BUFFER_15_16[TAIL_15_16+offs];
}



inline void __push__16(float data) {
BUFFER_16_17[HEAD_16_17]=data;
HEAD_16_17++;
}



 
void init_CombineDFT__167_44__16(){
  float real__162 = 0.0f;/* float */
  float imag__163 = 0.0f;/* float */
  float next_real__164 = 0.0f;/* float */float next_imag__165 = 0.0f;/* float */
  int i__166 = 0;/* int */

  (real__162 = ((float)1.0))/*float*/;
  (imag__163 = ((float)0.0))/*float*/;
  for ((i__166 = 0)/*int*/; (i__166 < 8); (i__166 = (i__166 + 2))/*int*/) {{
      (((w__148__16)[(int)i__166]) = real__162)/*float*/;
      (((w__148__16)[(int)(i__166 + 1)]) = imag__163)/*float*/;
      (next_real__164 = ((real__162 * ((float)0.70710677)) - (imag__163 * ((float)-0.70710677))))/*float*/;
      (next_imag__165 = ((real__162 * ((float)-0.70710677)) + (imag__163 * ((float)0.70710677))))/*float*/;
      (real__162 = next_real__164)/*float*/;
      (imag__163 = next_imag__165)/*float*/;
    }
  }
}
void save_file_pointer__16(object_write_buffer *buf) {}
void load_file_pointer__16(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_CombineDFT__167_44__16__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  int i__151 = 0;/* int */
  float results__152[16] = {0};/* float[16] */
  int i_plus_1__153 = 0;/* int */
  float y0_r__154 = 0.0f;/* float */
  float y0_i__155 = 0.0f;/* float */
  float y1_r__156 = 0.0f;/* float */
  float y1_i__157 = 0.0f;/* float */
  float weight_real__158 = 0.0f;/* float */
  float weight_imag__159 = 0.0f;/* float */
  float y1w_r__160 = 0.0f;/* float */
  float y1w_i__161 = 0.0f;/* float */

  // mark begin: SIRFilter CombineDFT

  for ((i__151 = 0)/*int*/; (i__151 < 8); (i__151 = (i__151 + 2))/*int*/) {{
      (i_plus_1__153 = (i__151 + 1))/*int*/;
      (y0_r__154 = (*(____in+i__151)))/*float*/;
      (y0_i__155 = (*(____in+i_plus_1__153)))/*float*/;
      (y1_r__156 = (*(____in+(8 + i__151))))/*float*/;
      (y1_i__157 = (*(____in+(8 + i_plus_1__153))))/*float*/;
      (weight_real__158 = ((w__148__16)[(int)i__151]))/*float*/;
      (weight_imag__159 = ((w__148__16)[(int)i_plus_1__153]))/*float*/;
      (y1w_r__160 = ((y1_r__156 * weight_real__158) - (y1_i__157 * weight_imag__159)))/*float*/;
      (y1w_i__161 = ((y1_r__156 * weight_imag__159) + (y1_i__157 * weight_real__158)))/*float*/;
      ((results__152[(int)i__151]) = (y0_r__154 + y1w_r__160))/*float*/;
      ((results__152[(int)(i__151 + 1)]) = (y0_i__155 + y1w_i__161))/*float*/;
      ((results__152[(int)(8 + i__151)]) = (y0_r__154 - y1w_r__160))/*float*/;
      ((results__152[(int)((8 + i__151) + 1)]) = (y0_i__155 - y1w_i__161))/*float*/;
    }
  }
  for ((i__151 = 0)/*int*/; (i__151 < 16); (i__151++)) {{
      (*____in++);
      ((*____out++)=(results__152[(int)i__151]));
    }
  }
  // mark end: SIRFilter CombineDFT

}}


void work_CombineDFT__167_44__16__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int i__151 = 0;/* int */
  float results__152[16] = {0};/* float[16] */
  int i_plus_1__153 = 0;/* int */
  float y0_r__154 = 0.0f;/* float */
  float y0_i__155 = 0.0f;/* float */
  float y1_r__156 = 0.0f;/* float */
  float y1_i__157 = 0.0f;/* float */
  float weight_real__158 = 0.0f;/* float */
  float weight_imag__159 = 0.0f;/* float */
  float y1w_r__160 = 0.0f;/* float */
  float y1w_i__161 = 0.0f;/* float */

  // mark begin: SIRFilter CombineDFT

  for ((i__151 = 0)/*int*/; (i__151 < 8); (i__151 = (i__151 + 2))/*int*/) {{
      (i_plus_1__153 = (i__151 + 1))/*int*/;
      (y0_r__154 = (*(____in+i__151)))/*float*/;
      (y0_i__155 = (*(____in+i_plus_1__153)))/*float*/;
      (y1_r__156 = (*(____in+(8 + i__151))))/*float*/;
      (y1_i__157 = (*(____in+(8 + i_plus_1__153))))/*float*/;
      (weight_real__158 = ((w__148__16)[(int)i__151]))/*float*/;
      (weight_imag__159 = ((w__148__16)[(int)i_plus_1__153]))/*float*/;
      (y1w_r__160 = ((y1_r__156 * weight_real__158) - (y1_i__157 * weight_imag__159)))/*float*/;
      (y1w_i__161 = ((y1_r__156 * weight_imag__159) + (y1_i__157 * weight_real__158)))/*float*/;
      ((results__152[(int)i__151]) = (y0_r__154 + y1w_r__160))/*float*/;
      ((results__152[(int)(i__151 + 1)]) = (y0_i__155 + y1w_i__161))/*float*/;
      ((results__152[(int)(8 + i__151)]) = (y0_r__154 - y1w_r__160))/*float*/;
      ((results__152[(int)((8 + i__151) + 1)]) = (y0_i__155 - y1w_i__161))/*float*/;
    }
  }
  for ((i__151 = 0)/*int*/; (i__151 < 16); (i__151++)) {{
      (*____in++);
      ((*____out++)=(results__152[(int)i__151]));
    }
  }
  // mark end: SIRFilter CombineDFT

}}


#endif // BUFFER_MERGE


 
void work_CombineDFT__167_44__16(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int i__151 = 0;/* int */
      float results__152[16] = {0};/* float[16] */
      int i_plus_1__153 = 0;/* int */
      float y0_r__154 = 0.0f;/* float */
      float y0_i__155 = 0.0f;/* float */
      float y1_r__156 = 0.0f;/* float */
      float y1_i__157 = 0.0f;/* float */
      float weight_real__158 = 0.0f;/* float */
      float weight_imag__159 = 0.0f;/* float */
      float y1w_r__160 = 0.0f;/* float */
      float y1w_i__161 = 0.0f;/* float */

      // mark begin: SIRFilter CombineDFT

      for ((i__151 = 0)/*int*/; (i__151 < 8); (i__151 = (i__151 + 2))/*int*/) {{
          (i_plus_1__153 = (i__151 + 1))/*int*/;
          (y0_r__154 = __peek__16(i__151))/*float*/;
          (y0_i__155 = __peek__16(i_plus_1__153))/*float*/;
          (y1_r__156 = __peek__16((8 + i__151)))/*float*/;
          (y1_i__157 = __peek__16((8 + i_plus_1__153)))/*float*/;
          (weight_real__158 = ((w__148__16)[(int)i__151]))/*float*/;
          (weight_imag__159 = ((w__148__16)[(int)i_plus_1__153]))/*float*/;
          (y1w_r__160 = ((y1_r__156 * weight_real__158) - (y1_i__157 * weight_imag__159)))/*float*/;
          (y1w_i__161 = ((y1_r__156 * weight_imag__159) + (y1_i__157 * weight_real__158)))/*float*/;
          ((results__152[(int)i__151]) = (y0_r__154 + y1w_r__160))/*float*/;
          ((results__152[(int)(i__151 + 1)]) = (y0_i__155 + y1w_i__161))/*float*/;
          ((results__152[(int)(8 + i__151)]) = (y0_r__154 - y1w_r__160))/*float*/;
          ((results__152[(int)((8 + i__151) + 1)]) = (y0_i__155 - y1w_i__161))/*float*/;
        }
      }
      for ((i__151 = 0)/*int*/; (i__151 < 16); (i__151++)) {{
          __pop__16();
          __push__16((results__152[(int)i__151]));
        }
      }
      // mark end: SIRFilter CombineDFT

    }
  }
}

// peek: 32 pop: 32 push 32
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_17;
int __counter_17 = 0;
int __steady_17 = 0;
int __tmp_17 = 0;
int __tmp2_17 = 0;
int *__state_flag_17 = NULL;
thread_info *__thread_17 = NULL;



float w__168__17[16] = {0};
void save_peek_buffer__17(object_write_buffer *buf);
void load_peek_buffer__17(object_write_buffer *buf);
void save_file_pointer__17(object_write_buffer *buf);
void load_file_pointer__17(object_write_buffer *buf);

 
void init_CombineDFT__187_45__17();
inline void check_status__17();

void work_CombineDFT__187_45__17(int);


inline float __pop__17() {
float res=BUFFER_16_17[TAIL_16_17];
TAIL_16_17++;
return res;
}

inline float __pop__17(int n) {
float res=BUFFER_16_17[TAIL_16_17];
TAIL_16_17+=n;

return res;
}

inline float __peek__17(int offs) {
return BUFFER_16_17[TAIL_16_17+offs];
}



inline void __push__17(float data) {
BUFFER_17_9[HEAD_17_9]=data;
HEAD_17_9++;
}



 
void init_CombineDFT__187_45__17(){
  float real__182 = 0.0f;/* float */
  float imag__183 = 0.0f;/* float */
  float next_real__184 = 0.0f;/* float */float next_imag__185 = 0.0f;/* float */
  int i__186 = 0;/* int */

  (real__182 = ((float)1.0))/*float*/;
  (imag__183 = ((float)0.0))/*float*/;
  for ((i__186 = 0)/*int*/; (i__186 < 16); (i__186 = (i__186 + 2))/*int*/) {{
      (((w__168__17)[(int)i__186]) = real__182)/*float*/;
      (((w__168__17)[(int)(i__186 + 1)]) = imag__183)/*float*/;
      (next_real__184 = ((real__182 * ((float)0.9238795)) - (imag__183 * ((float)-0.38268346))))/*float*/;
      (next_imag__185 = ((real__182 * ((float)-0.38268346)) + (imag__183 * ((float)0.9238795))))/*float*/;
      (real__182 = next_real__184)/*float*/;
      (imag__183 = next_imag__185)/*float*/;
    }
  }
}
void save_file_pointer__17(object_write_buffer *buf) {}
void load_file_pointer__17(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_CombineDFT__187_45__17__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  int i__171 = 0;/* int */
  float results__172[32] = {0};/* float[32] */
  int i_plus_1__173 = 0;/* int */
  float y0_r__174 = 0.0f;/* float */
  float y0_i__175 = 0.0f;/* float */
  float y1_r__176 = 0.0f;/* float */
  float y1_i__177 = 0.0f;/* float */
  float weight_real__178 = 0.0f;/* float */
  float weight_imag__179 = 0.0f;/* float */
  float y1w_r__180 = 0.0f;/* float */
  float y1w_i__181 = 0.0f;/* float */

  // mark begin: SIRFilter CombineDFT

  for ((i__171 = 0)/*int*/; (i__171 < 16); (i__171 = (i__171 + 2))/*int*/) {{
      (i_plus_1__173 = (i__171 + 1))/*int*/;
      (y0_r__174 = (*(____in+i__171)))/*float*/;
      (y0_i__175 = (*(____in+i_plus_1__173)))/*float*/;
      (y1_r__176 = (*(____in+(16 + i__171))))/*float*/;
      (y1_i__177 = (*(____in+(16 + i_plus_1__173))))/*float*/;
      (weight_real__178 = ((w__168__17)[(int)i__171]))/*float*/;
      (weight_imag__179 = ((w__168__17)[(int)i_plus_1__173]))/*float*/;
      (y1w_r__180 = ((y1_r__176 * weight_real__178) - (y1_i__177 * weight_imag__179)))/*float*/;
      (y1w_i__181 = ((y1_r__176 * weight_imag__179) + (y1_i__177 * weight_real__178)))/*float*/;
      ((results__172[(int)i__171]) = (y0_r__174 + y1w_r__180))/*float*/;
      ((results__172[(int)(i__171 + 1)]) = (y0_i__175 + y1w_i__181))/*float*/;
      ((results__172[(int)(16 + i__171)]) = (y0_r__174 - y1w_r__180))/*float*/;
      ((results__172[(int)((16 + i__171) + 1)]) = (y0_i__175 - y1w_i__181))/*float*/;
    }
  }
  for ((i__171 = 0)/*int*/; (i__171 < 32); (i__171++)) {{
      (*____in++);
      ((*____out++)=(results__172[(int)i__171]));
    }
  }
  // mark end: SIRFilter CombineDFT

}}


void work_CombineDFT__187_45__17__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int i__171 = 0;/* int */
  float results__172[32] = {0};/* float[32] */
  int i_plus_1__173 = 0;/* int */
  float y0_r__174 = 0.0f;/* float */
  float y0_i__175 = 0.0f;/* float */
  float y1_r__176 = 0.0f;/* float */
  float y1_i__177 = 0.0f;/* float */
  float weight_real__178 = 0.0f;/* float */
  float weight_imag__179 = 0.0f;/* float */
  float y1w_r__180 = 0.0f;/* float */
  float y1w_i__181 = 0.0f;/* float */

  // mark begin: SIRFilter CombineDFT

  for ((i__171 = 0)/*int*/; (i__171 < 16); (i__171 = (i__171 + 2))/*int*/) {{
      (i_plus_1__173 = (i__171 + 1))/*int*/;
      (y0_r__174 = (*(____in+i__171)))/*float*/;
      (y0_i__175 = (*(____in+i_plus_1__173)))/*float*/;
      (y1_r__176 = (*(____in+(16 + i__171))))/*float*/;
      (y1_i__177 = (*(____in+(16 + i_plus_1__173))))/*float*/;
      (weight_real__178 = ((w__168__17)[(int)i__171]))/*float*/;
      (weight_imag__179 = ((w__168__17)[(int)i_plus_1__173]))/*float*/;
      (y1w_r__180 = ((y1_r__176 * weight_real__178) - (y1_i__177 * weight_imag__179)))/*float*/;
      (y1w_i__181 = ((y1_r__176 * weight_imag__179) + (y1_i__177 * weight_real__178)))/*float*/;
      ((results__172[(int)i__171]) = (y0_r__174 + y1w_r__180))/*float*/;
      ((results__172[(int)(i__171 + 1)]) = (y0_i__175 + y1w_i__181))/*float*/;
      ((results__172[(int)(16 + i__171)]) = (y0_r__174 - y1w_r__180))/*float*/;
      ((results__172[(int)((16 + i__171) + 1)]) = (y0_i__175 - y1w_i__181))/*float*/;
    }
  }
  for ((i__171 = 0)/*int*/; (i__171 < 32); (i__171++)) {{
      (*____in++);
      ((*____out++)=(results__172[(int)i__171]));
    }
  }
  // mark end: SIRFilter CombineDFT

}}


#endif // BUFFER_MERGE


 
void work_CombineDFT__187_45__17(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int i__171 = 0;/* int */
      float results__172[32] = {0};/* float[32] */
      int i_plus_1__173 = 0;/* int */
      float y0_r__174 = 0.0f;/* float */
      float y0_i__175 = 0.0f;/* float */
      float y1_r__176 = 0.0f;/* float */
      float y1_i__177 = 0.0f;/* float */
      float weight_real__178 = 0.0f;/* float */
      float weight_imag__179 = 0.0f;/* float */
      float y1w_r__180 = 0.0f;/* float */
      float y1w_i__181 = 0.0f;/* float */

      // mark begin: SIRFilter CombineDFT

      for ((i__171 = 0)/*int*/; (i__171 < 16); (i__171 = (i__171 + 2))/*int*/) {{
          (i_plus_1__173 = (i__171 + 1))/*int*/;
          (y0_r__174 = __peek__17(i__171))/*float*/;
          (y0_i__175 = __peek__17(i_plus_1__173))/*float*/;
          (y1_r__176 = __peek__17((16 + i__171)))/*float*/;
          (y1_i__177 = __peek__17((16 + i_plus_1__173)))/*float*/;
          (weight_real__178 = ((w__168__17)[(int)i__171]))/*float*/;
          (weight_imag__179 = ((w__168__17)[(int)i_plus_1__173]))/*float*/;
          (y1w_r__180 = ((y1_r__176 * weight_real__178) - (y1_i__177 * weight_imag__179)))/*float*/;
          (y1w_i__181 = ((y1_r__176 * weight_imag__179) + (y1_i__177 * weight_real__178)))/*float*/;
          ((results__172[(int)i__171]) = (y0_r__174 + y1w_r__180))/*float*/;
          ((results__172[(int)(i__171 + 1)]) = (y0_i__175 + y1w_i__181))/*float*/;
          ((results__172[(int)(16 + i__171)]) = (y0_r__174 - y1w_r__180))/*float*/;
          ((results__172[(int)((16 + i__171) + 1)]) = (y0_i__175 - y1w_i__181))/*float*/;
        }
      }
      for ((i__171 = 0)/*int*/; (i__171 < 32); (i__171++)) {{
          __pop__17();
          __push__17((results__172[(int)i__171]));
        }
      }
      // mark end: SIRFilter CombineDFT

    }
  }
}

