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

int __max_iteration;
int __timer_enabled = 0;
int __frequency_of_chkpts;
volatile int __vol;
volatile long long sum = 0;
proc_timer tt("total runtime");


int BUFFER_0_1[__BUF_SIZE_MASK_0_1 + 1];
int HEAD_0_1 = 0;
int TAIL_0_1 = 0;
int BUFFER_1_2[__BUF_SIZE_MASK_1_2 + 1];
int HEAD_1_2 = 0;
int TAIL_1_2 = 0;
int BUFFER_2_3[__BUF_SIZE_MASK_2_3 + 1];
int HEAD_2_3 = 0;
int TAIL_2_3 = 0;
int BUFFER_3_4[__BUF_SIZE_MASK_3_4 + 1];
int HEAD_3_4 = 0;
int TAIL_3_4 = 0;
int BUFFER_4_5[__BUF_SIZE_MASK_4_5 + 1];
int HEAD_4_5 = 0;
int TAIL_4_5 = 0;
int BUFFER_5_6[__BUF_SIZE_MASK_5_6 + 1];
int HEAD_5_6 = 0;
int TAIL_5_6 = 0;
int BUFFER_6_7[__BUF_SIZE_MASK_6_7 + 1];
int HEAD_6_7 = 0;
int TAIL_6_7 = 0;
int BUFFER_7_8[__BUF_SIZE_MASK_7_8 + 1];
int HEAD_7_8 = 0;
int TAIL_7_8 = 0;
int BUFFER_8_9[__BUF_SIZE_MASK_8_9 + 1];
int HEAD_8_9 = 0;
int TAIL_8_9 = 0;
int BUFFER_9_10[__BUF_SIZE_MASK_9_10 + 1];
int HEAD_9_10 = 0;
int TAIL_9_10 = 0;
int BUFFER_10_11[__BUF_SIZE_MASK_10_11 + 1];
int HEAD_10_11 = 0;
int TAIL_10_11 = 0;
int BUFFER_11_12[__BUF_SIZE_MASK_11_12 + 1];
int HEAD_11_12 = 0;
int TAIL_11_12 = 0;
void init_IntSource__4_8__0();
void work_IntSource__4_8__0(int);
#ifdef BUFFER_MERGE
void work_IntSource__4_8__0__mod(int ____n, void *____in, int *____out);
void work_IntSource__4_8__0__mod2(int ____n, void *____in, int *____out, int s1, int s2);
#endif
void init_Sort__12_9__1();
void work_Sort__12_9__1(int);
#ifdef BUFFER_MERGE
void work_Sort__12_9__1__mod(int ____n, int *____in, int *____out);
void work_Sort__12_9__1__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Sort__20_10__2();
void work_Sort__20_10__2(int);
#ifdef BUFFER_MERGE
void work_Sort__20_10__2__mod(int ____n, int *____in, int *____out);
void work_Sort__20_10__2__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Sort__28_11__3();
void work_Sort__28_11__3(int);
#ifdef BUFFER_MERGE
void work_Sort__28_11__3__mod(int ____n, int *____in, int *____out);
void work_Sort__28_11__3__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Sort__36_12__4();
void work_Sort__36_12__4(int);
#ifdef BUFFER_MERGE
void work_Sort__36_12__4__mod(int ____n, int *____in, int *____out);
void work_Sort__36_12__4__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Sort__44_13__5();
void work_Sort__44_13__5(int);
#ifdef BUFFER_MERGE
void work_Sort__44_13__5__mod(int ____n, int *____in, int *____out);
void work_Sort__44_13__5__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Sort__52_14__6();
void work_Sort__52_14__6(int);
#ifdef BUFFER_MERGE
void work_Sort__52_14__6__mod(int ____n, int *____in, int *____out);
void work_Sort__52_14__6__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Sort__60_15__7();
void work_Sort__60_15__7(int);
#ifdef BUFFER_MERGE
void work_Sort__60_15__7__mod(int ____n, int *____in, int *____out);
void work_Sort__60_15__7__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Sort__68_16__8();
void work_Sort__68_16__8(int);
#ifdef BUFFER_MERGE
void work_Sort__68_16__8__mod(int ____n, int *____in, int *____out);
void work_Sort__68_16__8__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Sort__76_17__9();
void work_Sort__76_17__9(int);
#ifdef BUFFER_MERGE
void work_Sort__76_17__9__mod(int ____n, int *____in, int *____out);
void work_Sort__76_17__9__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Sort__84_18__10();
void work_Sort__84_18__10(int);
#ifdef BUFFER_MERGE
void work_Sort__84_18__10__mod(int ____n, int *____in, int *____out);
void work_Sort__84_18__10__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Sort__92_19__11();
void work_Sort__92_19__11(int);
#ifdef BUFFER_MERGE
void work_Sort__92_19__11__mod(int ____n, int *____in, int *____out);
void work_Sort__92_19__11__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_IntPrinter__96_20__12();
void work_IntPrinter__96_20__12(int);
#ifdef BUFFER_MERGE
void work_IntPrinter__96_20__12__mod(int ____n, int *____in, void *____out);
void work_IntPrinter__96_20__12__mod2(int ____n, int *____in, void *____out, int s1, int s2);
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
// number of phases: 13


#ifdef MEASURE_BY_RTS
RTS_init(1,1);
#else //default ``MEASURE_BY_PAPI''
PAPI_HW_COUNTER_init(1,1);
#endif
  // ============= Initialization =============

init_IntSource__4_8__0();
init_Sort__12_9__1();
init_Sort__20_10__2();
init_Sort__28_11__3();
init_Sort__36_12__4();
init_Sort__44_13__5();
init_Sort__52_14__6();
init_Sort__60_15__7();
init_Sort__68_16__8();
init_Sort__76_17__9();
init_Sort__84_18__10();
init_Sort__92_19__11();
init_IntPrinter__96_20__12();

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
    work_IntSource__4_8__0(16 );
HEAD_1_2 = 0;
TAIL_1_2 = 0;
    work_Sort__12_9__1(1 );
HEAD_2_3 = 0;
TAIL_2_3 = 0;
    work_Sort__20_10__2(1 );
HEAD_3_4 = 0;
TAIL_3_4 = 0;
    work_Sort__28_11__3(1 );
HEAD_4_5 = 0;
TAIL_4_5 = 0;
    work_Sort__36_12__4(1 );
HEAD_5_6 = 0;
TAIL_5_6 = 0;
    work_Sort__44_13__5(1 );
HEAD_6_7 = 0;
TAIL_6_7 = 0;
    work_Sort__52_14__6(1 );
HEAD_7_8 = 0;
TAIL_7_8 = 0;
    work_Sort__60_15__7(1 );
HEAD_8_9 = 0;
TAIL_8_9 = 0;
    work_Sort__68_16__8(1 );
HEAD_9_10 = 0;
TAIL_9_10 = 0;
    work_Sort__76_17__9(1 );
HEAD_10_11 = 0;
TAIL_10_11 = 0;
    work_Sort__84_18__10(1 );
HEAD_11_12 = 0;
TAIL_11_12 = 0;
    work_Sort__92_19__11(1 );
    work_IntPrinter__96_20__12(16 );
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
message *__msg_stack_12;
message *__msg_stack_1;
message *__msg_stack_11;
message *__msg_stack_3;
message *__msg_stack_10;
message *__msg_stack_5;
message *__msg_stack_7;
message *__msg_stack_2;
message *__msg_stack_6;
message *__msg_stack_9;
message *__msg_stack_4;
message *__msg_stack_0;
message *__msg_stack_8;

// end of moved or inserted by concat_cluster_threads.pl

// peek: 0 pop: 0 push 1
// init counts: 0 steady counts: 16

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



int data__0__0[16] = {0};
int index__1__0 = 0;
void save_file_pointer__0(object_write_buffer *buf);
void load_file_pointer__0(object_write_buffer *buf);

 
void init_IntSource__4_8__0();
inline void check_status__0();

void work_IntSource__4_8__0(int);



inline void __push__0(int data) {
BUFFER_0_1[HEAD_0_1]=data;
HEAD_0_1++;
}



 
void init_IntSource__4_8__0(){
  ((index__1__0) = 0)/*int*/;
  (((data__0__0)[(int)0]) = 503)/*int*/;
  (((data__0__0)[(int)1]) = 87)/*int*/;
  (((data__0__0)[(int)2]) = 512)/*int*/;
  (((data__0__0)[(int)3]) = 61)/*int*/;
  (((data__0__0)[(int)4]) = 908)/*int*/;
  (((data__0__0)[(int)5]) = 170)/*int*/;
  (((data__0__0)[(int)6]) = 897)/*int*/;
  (((data__0__0)[(int)7]) = 275)/*int*/;
  (((data__0__0)[(int)8]) = 653)/*int*/;
  (((data__0__0)[(int)9]) = 426)/*int*/;
  (((data__0__0)[(int)10]) = 154)/*int*/;
  (((data__0__0)[(int)11]) = 509)/*int*/;
  (((data__0__0)[(int)12]) = 612)/*int*/;
  (((data__0__0)[(int)13]) = 677)/*int*/;
  (((data__0__0)[(int)14]) = 765)/*int*/;
  (((data__0__0)[(int)15]) = 703)/*int*/;
}
void save_file_pointer__0(object_write_buffer *buf) {}
void load_file_pointer__0(object_write_buffer *buf) {}
 
void work_IntSource__4_8__0(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter IntSource

      __push__0(((data__0__0)[(int)((index__1__0)++)]));

      if (((index__1__0) == 16)) {((index__1__0) = 0)/*int*/;}
      // mark end: SIRFilter IntSource

    }
  }
}

// peek: 16 pop: 16 push 16
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_1;
int __counter_1 = 0;
int __steady_1 = 0;
int __tmp_1 = 0;
int __tmp2_1 = 0;
int *__state_flag_1 = NULL;
thread_info *__thread_1 = NULL;



void save_peek_buffer__1(object_write_buffer *buf);
void load_peek_buffer__1(object_write_buffer *buf);
void save_file_pointer__1(object_write_buffer *buf);
void load_file_pointer__1(object_write_buffer *buf);

 
void init_Sort__12_9__1();
inline void check_status__1();

void work_Sort__12_9__1(int);


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



 
void init_Sort__12_9__1(){
}
void save_file_pointer__1(object_write_buffer *buf) {}
void load_file_pointer__1(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Sort__12_9__1__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int ordering__7[16] = {0};/* int[16] */
  int j__8 = 0;/* int */
  int current__9 = 0;/* int */
  int i__conflict__0__10 = 0;/* int */
  int i__11 = 0;/* int */

  // mark begin: SIRFilter Sort

  (j__8 = 0)/*int*/;
  for ((i__conflict__0__10 = 0)/*int*/; (i__conflict__0__10 < 16); (i__conflict__0__10++)) {{
      (current__9 = (*____in++))/*int*/;
      if (((current__9 & 1) == 0)) {((*____out++)=current__9); } else {{
        ((ordering__7[(int)j__8]) = current__9)/*int*/;
        (j__8 = (j__8 + 1))/*int*/;
      }}
    }
  }
  for ((i__11 = 0)/*int*/; (i__11 < j__8); (i__11++)) {{
      ((*____out++)=(ordering__7[(int)i__11]));
    }
  }
  // mark end: SIRFilter Sort

}}


void work_Sort__12_9__1__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int ordering__7[16] = {0};/* int[16] */
  int j__8 = 0;/* int */
  int current__9 = 0;/* int */
  int i__conflict__0__10 = 0;/* int */
  int i__11 = 0;/* int */

  // mark begin: SIRFilter Sort

  (j__8 = 0)/*int*/;
  for ((i__conflict__0__10 = 0)/*int*/; (i__conflict__0__10 < 16); (i__conflict__0__10++)) {{
      (current__9 = (*____in++))/*int*/;
      if (((current__9 & 1) == 0)) {((*____out++)=current__9); } else {{
        ((ordering__7[(int)j__8]) = current__9)/*int*/;
        (j__8 = (j__8 + 1))/*int*/;
      }}
    }
  }
  for ((i__11 = 0)/*int*/; (i__11 < j__8); (i__11++)) {{
      ((*____out++)=(ordering__7[(int)i__11]));
    }
  }
  // mark end: SIRFilter Sort

}}


#endif // BUFFER_MERGE


 
void work_Sort__12_9__1(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int ordering__7[16] = {0};/* int[16] */
      int j__8 = 0;/* int */
      int current__9 = 0;/* int */
      int i__conflict__0__10 = 0;/* int */
      int i__11 = 0;/* int */

      // mark begin: SIRFilter Sort

      (j__8 = 0)/*int*/;
      for ((i__conflict__0__10 = 0)/*int*/; (i__conflict__0__10 < 16); (i__conflict__0__10++)) {{
          (current__9 = __pop__1())/*int*/;
          if (((current__9 & 1) == 0)) {__push__1(current__9); } else {{
            ((ordering__7[(int)j__8]) = current__9)/*int*/;
            (j__8 = (j__8 + 1))/*int*/;
          }}
        }
      }
      for ((i__11 = 0)/*int*/; (i__11 < j__8); (i__11++)) {{
          __push__1((ordering__7[(int)i__11]));
        }
      }
      // mark end: SIRFilter Sort

    }
  }
}

// peek: 16 pop: 16 push 16
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

 
void init_Sort__20_10__2();
inline void check_status__2();

void work_Sort__20_10__2(int);


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



inline void __push__2(int data) {
BUFFER_2_3[HEAD_2_3]=data;
HEAD_2_3++;
}



 
void init_Sort__20_10__2(){
}
void save_file_pointer__2(object_write_buffer *buf) {}
void load_file_pointer__2(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Sort__20_10__2__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int ordering__15[16] = {0};/* int[16] */
  int j__16 = 0;/* int */
  int current__17 = 0;/* int */
  int i__conflict__0__18 = 0;/* int */
  int i__19 = 0;/* int */

  // mark begin: SIRFilter Sort

  (j__16 = 0)/*int*/;
  for ((i__conflict__0__18 = 0)/*int*/; (i__conflict__0__18 < 16); (i__conflict__0__18++)) {{
      (current__17 = (*____in++))/*int*/;
      if (((current__17 & 2) == 0)) {((*____out++)=current__17); } else {{
        ((ordering__15[(int)j__16]) = current__17)/*int*/;
        (j__16 = (j__16 + 1))/*int*/;
      }}
    }
  }
  for ((i__19 = 0)/*int*/; (i__19 < j__16); (i__19++)) {{
      ((*____out++)=(ordering__15[(int)i__19]));
    }
  }
  // mark end: SIRFilter Sort

}}


void work_Sort__20_10__2__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int ordering__15[16] = {0};/* int[16] */
  int j__16 = 0;/* int */
  int current__17 = 0;/* int */
  int i__conflict__0__18 = 0;/* int */
  int i__19 = 0;/* int */

  // mark begin: SIRFilter Sort

  (j__16 = 0)/*int*/;
  for ((i__conflict__0__18 = 0)/*int*/; (i__conflict__0__18 < 16); (i__conflict__0__18++)) {{
      (current__17 = (*____in++))/*int*/;
      if (((current__17 & 2) == 0)) {((*____out++)=current__17); } else {{
        ((ordering__15[(int)j__16]) = current__17)/*int*/;
        (j__16 = (j__16 + 1))/*int*/;
      }}
    }
  }
  for ((i__19 = 0)/*int*/; (i__19 < j__16); (i__19++)) {{
      ((*____out++)=(ordering__15[(int)i__19]));
    }
  }
  // mark end: SIRFilter Sort

}}


#endif // BUFFER_MERGE


 
void work_Sort__20_10__2(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int ordering__15[16] = {0};/* int[16] */
      int j__16 = 0;/* int */
      int current__17 = 0;/* int */
      int i__conflict__0__18 = 0;/* int */
      int i__19 = 0;/* int */

      // mark begin: SIRFilter Sort

      (j__16 = 0)/*int*/;
      for ((i__conflict__0__18 = 0)/*int*/; (i__conflict__0__18 < 16); (i__conflict__0__18++)) {{
          (current__17 = __pop__2())/*int*/;
          if (((current__17 & 2) == 0)) {__push__2(current__17); } else {{
            ((ordering__15[(int)j__16]) = current__17)/*int*/;
            (j__16 = (j__16 + 1))/*int*/;
          }}
        }
      }
      for ((i__19 = 0)/*int*/; (i__19 < j__16); (i__19++)) {{
          __push__2((ordering__15[(int)i__19]));
        }
      }
      // mark end: SIRFilter Sort

    }
  }
}

// peek: 16 pop: 16 push 16
// init counts: 0 steady counts: 1

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

 
void init_Sort__28_11__3();
inline void check_status__3();

void work_Sort__28_11__3(int);


inline int __pop__3() {
int res=BUFFER_2_3[TAIL_2_3];
TAIL_2_3++;
return res;
}

inline int __pop__3(int n) {
int res=BUFFER_2_3[TAIL_2_3];
TAIL_2_3+=n;

return res;
}

inline int __peek__3(int offs) {
return BUFFER_2_3[TAIL_2_3+offs];
}



inline void __push__3(int data) {
BUFFER_3_4[HEAD_3_4]=data;
HEAD_3_4++;
}



 
void init_Sort__28_11__3(){
}
void save_file_pointer__3(object_write_buffer *buf) {}
void load_file_pointer__3(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Sort__28_11__3__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int ordering__23[16] = {0};/* int[16] */
  int j__24 = 0;/* int */
  int current__25 = 0;/* int */
  int i__conflict__0__26 = 0;/* int */
  int i__27 = 0;/* int */

  // mark begin: SIRFilter Sort

  (j__24 = 0)/*int*/;
  for ((i__conflict__0__26 = 0)/*int*/; (i__conflict__0__26 < 16); (i__conflict__0__26++)) {{
      (current__25 = (*____in++))/*int*/;
      if (((current__25 & 4) == 0)) {((*____out++)=current__25); } else {{
        ((ordering__23[(int)j__24]) = current__25)/*int*/;
        (j__24 = (j__24 + 1))/*int*/;
      }}
    }
  }
  for ((i__27 = 0)/*int*/; (i__27 < j__24); (i__27++)) {{
      ((*____out++)=(ordering__23[(int)i__27]));
    }
  }
  // mark end: SIRFilter Sort

}}


void work_Sort__28_11__3__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int ordering__23[16] = {0};/* int[16] */
  int j__24 = 0;/* int */
  int current__25 = 0;/* int */
  int i__conflict__0__26 = 0;/* int */
  int i__27 = 0;/* int */

  // mark begin: SIRFilter Sort

  (j__24 = 0)/*int*/;
  for ((i__conflict__0__26 = 0)/*int*/; (i__conflict__0__26 < 16); (i__conflict__0__26++)) {{
      (current__25 = (*____in++))/*int*/;
      if (((current__25 & 4) == 0)) {((*____out++)=current__25); } else {{
        ((ordering__23[(int)j__24]) = current__25)/*int*/;
        (j__24 = (j__24 + 1))/*int*/;
      }}
    }
  }
  for ((i__27 = 0)/*int*/; (i__27 < j__24); (i__27++)) {{
      ((*____out++)=(ordering__23[(int)i__27]));
    }
  }
  // mark end: SIRFilter Sort

}}


#endif // BUFFER_MERGE


 
void work_Sort__28_11__3(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int ordering__23[16] = {0};/* int[16] */
      int j__24 = 0;/* int */
      int current__25 = 0;/* int */
      int i__conflict__0__26 = 0;/* int */
      int i__27 = 0;/* int */

      // mark begin: SIRFilter Sort

      (j__24 = 0)/*int*/;
      for ((i__conflict__0__26 = 0)/*int*/; (i__conflict__0__26 < 16); (i__conflict__0__26++)) {{
          (current__25 = __pop__3())/*int*/;
          if (((current__25 & 4) == 0)) {__push__3(current__25); } else {{
            ((ordering__23[(int)j__24]) = current__25)/*int*/;
            (j__24 = (j__24 + 1))/*int*/;
          }}
        }
      }
      for ((i__27 = 0)/*int*/; (i__27 < j__24); (i__27++)) {{
          __push__3((ordering__23[(int)i__27]));
        }
      }
      // mark end: SIRFilter Sort

    }
  }
}

// peek: 16 pop: 16 push 16
// init counts: 0 steady counts: 1

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

 
void init_Sort__36_12__4();
inline void check_status__4();

void work_Sort__36_12__4(int);


inline int __pop__4() {
int res=BUFFER_3_4[TAIL_3_4];
TAIL_3_4++;
return res;
}

inline int __pop__4(int n) {
int res=BUFFER_3_4[TAIL_3_4];
TAIL_3_4+=n;

return res;
}

inline int __peek__4(int offs) {
return BUFFER_3_4[TAIL_3_4+offs];
}



inline void __push__4(int data) {
BUFFER_4_5[HEAD_4_5]=data;
HEAD_4_5++;
}



 
void init_Sort__36_12__4(){
}
void save_file_pointer__4(object_write_buffer *buf) {}
void load_file_pointer__4(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Sort__36_12__4__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int ordering__31[16] = {0};/* int[16] */
  int j__32 = 0;/* int */
  int current__33 = 0;/* int */
  int i__conflict__0__34 = 0;/* int */
  int i__35 = 0;/* int */

  // mark begin: SIRFilter Sort

  (j__32 = 0)/*int*/;
  for ((i__conflict__0__34 = 0)/*int*/; (i__conflict__0__34 < 16); (i__conflict__0__34++)) {{
      (current__33 = (*____in++))/*int*/;
      if (((current__33 & 8) == 0)) {((*____out++)=current__33); } else {{
        ((ordering__31[(int)j__32]) = current__33)/*int*/;
        (j__32 = (j__32 + 1))/*int*/;
      }}
    }
  }
  for ((i__35 = 0)/*int*/; (i__35 < j__32); (i__35++)) {{
      ((*____out++)=(ordering__31[(int)i__35]));
    }
  }
  // mark end: SIRFilter Sort

}}


void work_Sort__36_12__4__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int ordering__31[16] = {0};/* int[16] */
  int j__32 = 0;/* int */
  int current__33 = 0;/* int */
  int i__conflict__0__34 = 0;/* int */
  int i__35 = 0;/* int */

  // mark begin: SIRFilter Sort

  (j__32 = 0)/*int*/;
  for ((i__conflict__0__34 = 0)/*int*/; (i__conflict__0__34 < 16); (i__conflict__0__34++)) {{
      (current__33 = (*____in++))/*int*/;
      if (((current__33 & 8) == 0)) {((*____out++)=current__33); } else {{
        ((ordering__31[(int)j__32]) = current__33)/*int*/;
        (j__32 = (j__32 + 1))/*int*/;
      }}
    }
  }
  for ((i__35 = 0)/*int*/; (i__35 < j__32); (i__35++)) {{
      ((*____out++)=(ordering__31[(int)i__35]));
    }
  }
  // mark end: SIRFilter Sort

}}


#endif // BUFFER_MERGE


 
void work_Sort__36_12__4(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int ordering__31[16] = {0};/* int[16] */
      int j__32 = 0;/* int */
      int current__33 = 0;/* int */
      int i__conflict__0__34 = 0;/* int */
      int i__35 = 0;/* int */

      // mark begin: SIRFilter Sort

      (j__32 = 0)/*int*/;
      for ((i__conflict__0__34 = 0)/*int*/; (i__conflict__0__34 < 16); (i__conflict__0__34++)) {{
          (current__33 = __pop__4())/*int*/;
          if (((current__33 & 8) == 0)) {__push__4(current__33); } else {{
            ((ordering__31[(int)j__32]) = current__33)/*int*/;
            (j__32 = (j__32 + 1))/*int*/;
          }}
        }
      }
      for ((i__35 = 0)/*int*/; (i__35 < j__32); (i__35++)) {{
          __push__4((ordering__31[(int)i__35]));
        }
      }
      // mark end: SIRFilter Sort

    }
  }
}

// peek: 16 pop: 16 push 16
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_5;
int __counter_5 = 0;
int __steady_5 = 0;
int __tmp_5 = 0;
int __tmp2_5 = 0;
int *__state_flag_5 = NULL;
thread_info *__thread_5 = NULL;



void save_peek_buffer__5(object_write_buffer *buf);
void load_peek_buffer__5(object_write_buffer *buf);
void save_file_pointer__5(object_write_buffer *buf);
void load_file_pointer__5(object_write_buffer *buf);

 
void init_Sort__44_13__5();
inline void check_status__5();

void work_Sort__44_13__5(int);


inline int __pop__5() {
int res=BUFFER_4_5[TAIL_4_5];
TAIL_4_5++;
return res;
}

inline int __pop__5(int n) {
int res=BUFFER_4_5[TAIL_4_5];
TAIL_4_5+=n;

return res;
}

inline int __peek__5(int offs) {
return BUFFER_4_5[TAIL_4_5+offs];
}



inline void __push__5(int data) {
BUFFER_5_6[HEAD_5_6]=data;
HEAD_5_6++;
}



 
void init_Sort__44_13__5(){
}
void save_file_pointer__5(object_write_buffer *buf) {}
void load_file_pointer__5(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Sort__44_13__5__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int ordering__39[16] = {0};/* int[16] */
  int j__40 = 0;/* int */
  int current__41 = 0;/* int */
  int i__conflict__0__42 = 0;/* int */
  int i__43 = 0;/* int */

  // mark begin: SIRFilter Sort

  (j__40 = 0)/*int*/;
  for ((i__conflict__0__42 = 0)/*int*/; (i__conflict__0__42 < 16); (i__conflict__0__42++)) {{
      (current__41 = (*____in++))/*int*/;
      if (((current__41 & 16) == 0)) {((*____out++)=current__41); } else {{
        ((ordering__39[(int)j__40]) = current__41)/*int*/;
        (j__40 = (j__40 + 1))/*int*/;
      }}
    }
  }
  for ((i__43 = 0)/*int*/; (i__43 < j__40); (i__43++)) {{
      ((*____out++)=(ordering__39[(int)i__43]));
    }
  }
  // mark end: SIRFilter Sort

}}


void work_Sort__44_13__5__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int ordering__39[16] = {0};/* int[16] */
  int j__40 = 0;/* int */
  int current__41 = 0;/* int */
  int i__conflict__0__42 = 0;/* int */
  int i__43 = 0;/* int */

  // mark begin: SIRFilter Sort

  (j__40 = 0)/*int*/;
  for ((i__conflict__0__42 = 0)/*int*/; (i__conflict__0__42 < 16); (i__conflict__0__42++)) {{
      (current__41 = (*____in++))/*int*/;
      if (((current__41 & 16) == 0)) {((*____out++)=current__41); } else {{
        ((ordering__39[(int)j__40]) = current__41)/*int*/;
        (j__40 = (j__40 + 1))/*int*/;
      }}
    }
  }
  for ((i__43 = 0)/*int*/; (i__43 < j__40); (i__43++)) {{
      ((*____out++)=(ordering__39[(int)i__43]));
    }
  }
  // mark end: SIRFilter Sort

}}


#endif // BUFFER_MERGE


 
void work_Sort__44_13__5(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int ordering__39[16] = {0};/* int[16] */
      int j__40 = 0;/* int */
      int current__41 = 0;/* int */
      int i__conflict__0__42 = 0;/* int */
      int i__43 = 0;/* int */

      // mark begin: SIRFilter Sort

      (j__40 = 0)/*int*/;
      for ((i__conflict__0__42 = 0)/*int*/; (i__conflict__0__42 < 16); (i__conflict__0__42++)) {{
          (current__41 = __pop__5())/*int*/;
          if (((current__41 & 16) == 0)) {__push__5(current__41); } else {{
            ((ordering__39[(int)j__40]) = current__41)/*int*/;
            (j__40 = (j__40 + 1))/*int*/;
          }}
        }
      }
      for ((i__43 = 0)/*int*/; (i__43 < j__40); (i__43++)) {{
          __push__5((ordering__39[(int)i__43]));
        }
      }
      // mark end: SIRFilter Sort

    }
  }
}

// peek: 16 pop: 16 push 16
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_6;
int __counter_6 = 0;
int __steady_6 = 0;
int __tmp_6 = 0;
int __tmp2_6 = 0;
int *__state_flag_6 = NULL;
thread_info *__thread_6 = NULL;



void save_peek_buffer__6(object_write_buffer *buf);
void load_peek_buffer__6(object_write_buffer *buf);
void save_file_pointer__6(object_write_buffer *buf);
void load_file_pointer__6(object_write_buffer *buf);

 
void init_Sort__52_14__6();
inline void check_status__6();

void work_Sort__52_14__6(int);


inline int __pop__6() {
int res=BUFFER_5_6[TAIL_5_6];
TAIL_5_6++;
return res;
}

inline int __pop__6(int n) {
int res=BUFFER_5_6[TAIL_5_6];
TAIL_5_6+=n;

return res;
}

inline int __peek__6(int offs) {
return BUFFER_5_6[TAIL_5_6+offs];
}



inline void __push__6(int data) {
BUFFER_6_7[HEAD_6_7]=data;
HEAD_6_7++;
}



 
void init_Sort__52_14__6(){
}
void save_file_pointer__6(object_write_buffer *buf) {}
void load_file_pointer__6(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Sort__52_14__6__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int ordering__47[16] = {0};/* int[16] */
  int j__48 = 0;/* int */
  int current__49 = 0;/* int */
  int i__conflict__0__50 = 0;/* int */
  int i__51 = 0;/* int */

  // mark begin: SIRFilter Sort

  (j__48 = 0)/*int*/;
  for ((i__conflict__0__50 = 0)/*int*/; (i__conflict__0__50 < 16); (i__conflict__0__50++)) {{
      (current__49 = (*____in++))/*int*/;
      if (((current__49 & 32) == 0)) {((*____out++)=current__49); } else {{
        ((ordering__47[(int)j__48]) = current__49)/*int*/;
        (j__48 = (j__48 + 1))/*int*/;
      }}
    }
  }
  for ((i__51 = 0)/*int*/; (i__51 < j__48); (i__51++)) {{
      ((*____out++)=(ordering__47[(int)i__51]));
    }
  }
  // mark end: SIRFilter Sort

}}


void work_Sort__52_14__6__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int ordering__47[16] = {0};/* int[16] */
  int j__48 = 0;/* int */
  int current__49 = 0;/* int */
  int i__conflict__0__50 = 0;/* int */
  int i__51 = 0;/* int */

  // mark begin: SIRFilter Sort

  (j__48 = 0)/*int*/;
  for ((i__conflict__0__50 = 0)/*int*/; (i__conflict__0__50 < 16); (i__conflict__0__50++)) {{
      (current__49 = (*____in++))/*int*/;
      if (((current__49 & 32) == 0)) {((*____out++)=current__49); } else {{
        ((ordering__47[(int)j__48]) = current__49)/*int*/;
        (j__48 = (j__48 + 1))/*int*/;
      }}
    }
  }
  for ((i__51 = 0)/*int*/; (i__51 < j__48); (i__51++)) {{
      ((*____out++)=(ordering__47[(int)i__51]));
    }
  }
  // mark end: SIRFilter Sort

}}


#endif // BUFFER_MERGE


 
void work_Sort__52_14__6(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int ordering__47[16] = {0};/* int[16] */
      int j__48 = 0;/* int */
      int current__49 = 0;/* int */
      int i__conflict__0__50 = 0;/* int */
      int i__51 = 0;/* int */

      // mark begin: SIRFilter Sort

      (j__48 = 0)/*int*/;
      for ((i__conflict__0__50 = 0)/*int*/; (i__conflict__0__50 < 16); (i__conflict__0__50++)) {{
          (current__49 = __pop__6())/*int*/;
          if (((current__49 & 32) == 0)) {__push__6(current__49); } else {{
            ((ordering__47[(int)j__48]) = current__49)/*int*/;
            (j__48 = (j__48 + 1))/*int*/;
          }}
        }
      }
      for ((i__51 = 0)/*int*/; (i__51 < j__48); (i__51++)) {{
          __push__6((ordering__47[(int)i__51]));
        }
      }
      // mark end: SIRFilter Sort

    }
  }
}

// peek: 16 pop: 16 push 16
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_7;
int __counter_7 = 0;
int __steady_7 = 0;
int __tmp_7 = 0;
int __tmp2_7 = 0;
int *__state_flag_7 = NULL;
thread_info *__thread_7 = NULL;



void save_peek_buffer__7(object_write_buffer *buf);
void load_peek_buffer__7(object_write_buffer *buf);
void save_file_pointer__7(object_write_buffer *buf);
void load_file_pointer__7(object_write_buffer *buf);

 
void init_Sort__60_15__7();
inline void check_status__7();

void work_Sort__60_15__7(int);


inline int __pop__7() {
int res=BUFFER_6_7[TAIL_6_7];
TAIL_6_7++;
return res;
}

inline int __pop__7(int n) {
int res=BUFFER_6_7[TAIL_6_7];
TAIL_6_7+=n;

return res;
}

inline int __peek__7(int offs) {
return BUFFER_6_7[TAIL_6_7+offs];
}



inline void __push__7(int data) {
BUFFER_7_8[HEAD_7_8]=data;
HEAD_7_8++;
}



 
void init_Sort__60_15__7(){
}
void save_file_pointer__7(object_write_buffer *buf) {}
void load_file_pointer__7(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Sort__60_15__7__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int ordering__55[16] = {0};/* int[16] */
  int j__56 = 0;/* int */
  int current__57 = 0;/* int */
  int i__conflict__0__58 = 0;/* int */
  int i__59 = 0;/* int */

  // mark begin: SIRFilter Sort

  (j__56 = 0)/*int*/;
  for ((i__conflict__0__58 = 0)/*int*/; (i__conflict__0__58 < 16); (i__conflict__0__58++)) {{
      (current__57 = (*____in++))/*int*/;
      if (((current__57 & 64) == 0)) {((*____out++)=current__57); } else {{
        ((ordering__55[(int)j__56]) = current__57)/*int*/;
        (j__56 = (j__56 + 1))/*int*/;
      }}
    }
  }
  for ((i__59 = 0)/*int*/; (i__59 < j__56); (i__59++)) {{
      ((*____out++)=(ordering__55[(int)i__59]));
    }
  }
  // mark end: SIRFilter Sort

}}


void work_Sort__60_15__7__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int ordering__55[16] = {0};/* int[16] */
  int j__56 = 0;/* int */
  int current__57 = 0;/* int */
  int i__conflict__0__58 = 0;/* int */
  int i__59 = 0;/* int */

  // mark begin: SIRFilter Sort

  (j__56 = 0)/*int*/;
  for ((i__conflict__0__58 = 0)/*int*/; (i__conflict__0__58 < 16); (i__conflict__0__58++)) {{
      (current__57 = (*____in++))/*int*/;
      if (((current__57 & 64) == 0)) {((*____out++)=current__57); } else {{
        ((ordering__55[(int)j__56]) = current__57)/*int*/;
        (j__56 = (j__56 + 1))/*int*/;
      }}
    }
  }
  for ((i__59 = 0)/*int*/; (i__59 < j__56); (i__59++)) {{
      ((*____out++)=(ordering__55[(int)i__59]));
    }
  }
  // mark end: SIRFilter Sort

}}


#endif // BUFFER_MERGE


 
void work_Sort__60_15__7(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int ordering__55[16] = {0};/* int[16] */
      int j__56 = 0;/* int */
      int current__57 = 0;/* int */
      int i__conflict__0__58 = 0;/* int */
      int i__59 = 0;/* int */

      // mark begin: SIRFilter Sort

      (j__56 = 0)/*int*/;
      for ((i__conflict__0__58 = 0)/*int*/; (i__conflict__0__58 < 16); (i__conflict__0__58++)) {{
          (current__57 = __pop__7())/*int*/;
          if (((current__57 & 64) == 0)) {__push__7(current__57); } else {{
            ((ordering__55[(int)j__56]) = current__57)/*int*/;
            (j__56 = (j__56 + 1))/*int*/;
          }}
        }
      }
      for ((i__59 = 0)/*int*/; (i__59 < j__56); (i__59++)) {{
          __push__7((ordering__55[(int)i__59]));
        }
      }
      // mark end: SIRFilter Sort

    }
  }
}

// peek: 16 pop: 16 push 16
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_8;
int __counter_8 = 0;
int __steady_8 = 0;
int __tmp_8 = 0;
int __tmp2_8 = 0;
int *__state_flag_8 = NULL;
thread_info *__thread_8 = NULL;



void save_peek_buffer__8(object_write_buffer *buf);
void load_peek_buffer__8(object_write_buffer *buf);
void save_file_pointer__8(object_write_buffer *buf);
void load_file_pointer__8(object_write_buffer *buf);

 
void init_Sort__68_16__8();
inline void check_status__8();

void work_Sort__68_16__8(int);


inline int __pop__8() {
int res=BUFFER_7_8[TAIL_7_8];
TAIL_7_8++;
return res;
}

inline int __pop__8(int n) {
int res=BUFFER_7_8[TAIL_7_8];
TAIL_7_8+=n;

return res;
}

inline int __peek__8(int offs) {
return BUFFER_7_8[TAIL_7_8+offs];
}



inline void __push__8(int data) {
BUFFER_8_9[HEAD_8_9]=data;
HEAD_8_9++;
}



 
void init_Sort__68_16__8(){
}
void save_file_pointer__8(object_write_buffer *buf) {}
void load_file_pointer__8(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Sort__68_16__8__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int ordering__63[16] = {0};/* int[16] */
  int j__64 = 0;/* int */
  int current__65 = 0;/* int */
  int i__conflict__0__66 = 0;/* int */
  int i__67 = 0;/* int */

  // mark begin: SIRFilter Sort

  (j__64 = 0)/*int*/;
  for ((i__conflict__0__66 = 0)/*int*/; (i__conflict__0__66 < 16); (i__conflict__0__66++)) {{
      (current__65 = (*____in++))/*int*/;
      if (((current__65 & 128) == 0)) {((*____out++)=current__65); } else {{
        ((ordering__63[(int)j__64]) = current__65)/*int*/;
        (j__64 = (j__64 + 1))/*int*/;
      }}
    }
  }
  for ((i__67 = 0)/*int*/; (i__67 < j__64); (i__67++)) {{
      ((*____out++)=(ordering__63[(int)i__67]));
    }
  }
  // mark end: SIRFilter Sort

}}


void work_Sort__68_16__8__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int ordering__63[16] = {0};/* int[16] */
  int j__64 = 0;/* int */
  int current__65 = 0;/* int */
  int i__conflict__0__66 = 0;/* int */
  int i__67 = 0;/* int */

  // mark begin: SIRFilter Sort

  (j__64 = 0)/*int*/;
  for ((i__conflict__0__66 = 0)/*int*/; (i__conflict__0__66 < 16); (i__conflict__0__66++)) {{
      (current__65 = (*____in++))/*int*/;
      if (((current__65 & 128) == 0)) {((*____out++)=current__65); } else {{
        ((ordering__63[(int)j__64]) = current__65)/*int*/;
        (j__64 = (j__64 + 1))/*int*/;
      }}
    }
  }
  for ((i__67 = 0)/*int*/; (i__67 < j__64); (i__67++)) {{
      ((*____out++)=(ordering__63[(int)i__67]));
    }
  }
  // mark end: SIRFilter Sort

}}


#endif // BUFFER_MERGE


 
void work_Sort__68_16__8(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int ordering__63[16] = {0};/* int[16] */
      int j__64 = 0;/* int */
      int current__65 = 0;/* int */
      int i__conflict__0__66 = 0;/* int */
      int i__67 = 0;/* int */

      // mark begin: SIRFilter Sort

      (j__64 = 0)/*int*/;
      for ((i__conflict__0__66 = 0)/*int*/; (i__conflict__0__66 < 16); (i__conflict__0__66++)) {{
          (current__65 = __pop__8())/*int*/;
          if (((current__65 & 128) == 0)) {__push__8(current__65); } else {{
            ((ordering__63[(int)j__64]) = current__65)/*int*/;
            (j__64 = (j__64 + 1))/*int*/;
          }}
        }
      }
      for ((i__67 = 0)/*int*/; (i__67 < j__64); (i__67++)) {{
          __push__8((ordering__63[(int)i__67]));
        }
      }
      // mark end: SIRFilter Sort

    }
  }
}

// peek: 16 pop: 16 push 16
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_9;
int __counter_9 = 0;
int __steady_9 = 0;
int __tmp_9 = 0;
int __tmp2_9 = 0;
int *__state_flag_9 = NULL;
thread_info *__thread_9 = NULL;



void save_peek_buffer__9(object_write_buffer *buf);
void load_peek_buffer__9(object_write_buffer *buf);
void save_file_pointer__9(object_write_buffer *buf);
void load_file_pointer__9(object_write_buffer *buf);

 
void init_Sort__76_17__9();
inline void check_status__9();

void work_Sort__76_17__9(int);


inline int __pop__9() {
int res=BUFFER_8_9[TAIL_8_9];
TAIL_8_9++;
return res;
}

inline int __pop__9(int n) {
int res=BUFFER_8_9[TAIL_8_9];
TAIL_8_9+=n;

return res;
}

inline int __peek__9(int offs) {
return BUFFER_8_9[TAIL_8_9+offs];
}



inline void __push__9(int data) {
BUFFER_9_10[HEAD_9_10]=data;
HEAD_9_10++;
}



 
void init_Sort__76_17__9(){
}
void save_file_pointer__9(object_write_buffer *buf) {}
void load_file_pointer__9(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Sort__76_17__9__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int ordering__71[16] = {0};/* int[16] */
  int j__72 = 0;/* int */
  int current__73 = 0;/* int */
  int i__conflict__0__74 = 0;/* int */
  int i__75 = 0;/* int */

  // mark begin: SIRFilter Sort

  (j__72 = 0)/*int*/;
  for ((i__conflict__0__74 = 0)/*int*/; (i__conflict__0__74 < 16); (i__conflict__0__74++)) {{
      (current__73 = (*____in++))/*int*/;
      if (((current__73 & 256) == 0)) {((*____out++)=current__73); } else {{
        ((ordering__71[(int)j__72]) = current__73)/*int*/;
        (j__72 = (j__72 + 1))/*int*/;
      }}
    }
  }
  for ((i__75 = 0)/*int*/; (i__75 < j__72); (i__75++)) {{
      ((*____out++)=(ordering__71[(int)i__75]));
    }
  }
  // mark end: SIRFilter Sort

}}


void work_Sort__76_17__9__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int ordering__71[16] = {0};/* int[16] */
  int j__72 = 0;/* int */
  int current__73 = 0;/* int */
  int i__conflict__0__74 = 0;/* int */
  int i__75 = 0;/* int */

  // mark begin: SIRFilter Sort

  (j__72 = 0)/*int*/;
  for ((i__conflict__0__74 = 0)/*int*/; (i__conflict__0__74 < 16); (i__conflict__0__74++)) {{
      (current__73 = (*____in++))/*int*/;
      if (((current__73 & 256) == 0)) {((*____out++)=current__73); } else {{
        ((ordering__71[(int)j__72]) = current__73)/*int*/;
        (j__72 = (j__72 + 1))/*int*/;
      }}
    }
  }
  for ((i__75 = 0)/*int*/; (i__75 < j__72); (i__75++)) {{
      ((*____out++)=(ordering__71[(int)i__75]));
    }
  }
  // mark end: SIRFilter Sort

}}


#endif // BUFFER_MERGE


 
void work_Sort__76_17__9(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int ordering__71[16] = {0};/* int[16] */
      int j__72 = 0;/* int */
      int current__73 = 0;/* int */
      int i__conflict__0__74 = 0;/* int */
      int i__75 = 0;/* int */

      // mark begin: SIRFilter Sort

      (j__72 = 0)/*int*/;
      for ((i__conflict__0__74 = 0)/*int*/; (i__conflict__0__74 < 16); (i__conflict__0__74++)) {{
          (current__73 = __pop__9())/*int*/;
          if (((current__73 & 256) == 0)) {__push__9(current__73); } else {{
            ((ordering__71[(int)j__72]) = current__73)/*int*/;
            (j__72 = (j__72 + 1))/*int*/;
          }}
        }
      }
      for ((i__75 = 0)/*int*/; (i__75 < j__72); (i__75++)) {{
          __push__9((ordering__71[(int)i__75]));
        }
      }
      // mark end: SIRFilter Sort

    }
  }
}

// peek: 16 pop: 16 push 16
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_10;
int __counter_10 = 0;
int __steady_10 = 0;
int __tmp_10 = 0;
int __tmp2_10 = 0;
int *__state_flag_10 = NULL;
thread_info *__thread_10 = NULL;



void save_peek_buffer__10(object_write_buffer *buf);
void load_peek_buffer__10(object_write_buffer *buf);
void save_file_pointer__10(object_write_buffer *buf);
void load_file_pointer__10(object_write_buffer *buf);

 
void init_Sort__84_18__10();
inline void check_status__10();

void work_Sort__84_18__10(int);


inline int __pop__10() {
int res=BUFFER_9_10[TAIL_9_10];
TAIL_9_10++;
return res;
}

inline int __pop__10(int n) {
int res=BUFFER_9_10[TAIL_9_10];
TAIL_9_10+=n;

return res;
}

inline int __peek__10(int offs) {
return BUFFER_9_10[TAIL_9_10+offs];
}



inline void __push__10(int data) {
BUFFER_10_11[HEAD_10_11]=data;
HEAD_10_11++;
}



 
void init_Sort__84_18__10(){
}
void save_file_pointer__10(object_write_buffer *buf) {}
void load_file_pointer__10(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Sort__84_18__10__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int ordering__79[16] = {0};/* int[16] */
  int j__80 = 0;/* int */
  int current__81 = 0;/* int */
  int i__conflict__0__82 = 0;/* int */
  int i__83 = 0;/* int */

  // mark begin: SIRFilter Sort

  (j__80 = 0)/*int*/;
  for ((i__conflict__0__82 = 0)/*int*/; (i__conflict__0__82 < 16); (i__conflict__0__82++)) {{
      (current__81 = (*____in++))/*int*/;
      if (((current__81 & 512) == 0)) {((*____out++)=current__81); } else {{
        ((ordering__79[(int)j__80]) = current__81)/*int*/;
        (j__80 = (j__80 + 1))/*int*/;
      }}
    }
  }
  for ((i__83 = 0)/*int*/; (i__83 < j__80); (i__83++)) {{
      ((*____out++)=(ordering__79[(int)i__83]));
    }
  }
  // mark end: SIRFilter Sort

}}


void work_Sort__84_18__10__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int ordering__79[16] = {0};/* int[16] */
  int j__80 = 0;/* int */
  int current__81 = 0;/* int */
  int i__conflict__0__82 = 0;/* int */
  int i__83 = 0;/* int */

  // mark begin: SIRFilter Sort

  (j__80 = 0)/*int*/;
  for ((i__conflict__0__82 = 0)/*int*/; (i__conflict__0__82 < 16); (i__conflict__0__82++)) {{
      (current__81 = (*____in++))/*int*/;
      if (((current__81 & 512) == 0)) {((*____out++)=current__81); } else {{
        ((ordering__79[(int)j__80]) = current__81)/*int*/;
        (j__80 = (j__80 + 1))/*int*/;
      }}
    }
  }
  for ((i__83 = 0)/*int*/; (i__83 < j__80); (i__83++)) {{
      ((*____out++)=(ordering__79[(int)i__83]));
    }
  }
  // mark end: SIRFilter Sort

}}


#endif // BUFFER_MERGE


 
void work_Sort__84_18__10(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int ordering__79[16] = {0};/* int[16] */
      int j__80 = 0;/* int */
      int current__81 = 0;/* int */
      int i__conflict__0__82 = 0;/* int */
      int i__83 = 0;/* int */

      // mark begin: SIRFilter Sort

      (j__80 = 0)/*int*/;
      for ((i__conflict__0__82 = 0)/*int*/; (i__conflict__0__82 < 16); (i__conflict__0__82++)) {{
          (current__81 = __pop__10())/*int*/;
          if (((current__81 & 512) == 0)) {__push__10(current__81); } else {{
            ((ordering__79[(int)j__80]) = current__81)/*int*/;
            (j__80 = (j__80 + 1))/*int*/;
          }}
        }
      }
      for ((i__83 = 0)/*int*/; (i__83 < j__80); (i__83++)) {{
          __push__10((ordering__79[(int)i__83]));
        }
      }
      // mark end: SIRFilter Sort

    }
  }
}

// peek: 16 pop: 16 push 16
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

 
void init_Sort__92_19__11();
inline void check_status__11();

void work_Sort__92_19__11(int);


inline int __pop__11() {
int res=BUFFER_10_11[TAIL_10_11];
TAIL_10_11++;
return res;
}

inline int __pop__11(int n) {
int res=BUFFER_10_11[TAIL_10_11];
TAIL_10_11+=n;

return res;
}

inline int __peek__11(int offs) {
return BUFFER_10_11[TAIL_10_11+offs];
}



inline void __push__11(int data) {
BUFFER_11_12[HEAD_11_12]=data;
HEAD_11_12++;
}



 
void init_Sort__92_19__11(){
}
void save_file_pointer__11(object_write_buffer *buf) {}
void load_file_pointer__11(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Sort__92_19__11__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int ordering__87[16] = {0};/* int[16] */
  int j__88 = 0;/* int */
  int current__89 = 0;/* int */
  int i__conflict__0__90 = 0;/* int */
  int i__91 = 0;/* int */

  // mark begin: SIRFilter Sort

  (j__88 = 0)/*int*/;
  for ((i__conflict__0__90 = 0)/*int*/; (i__conflict__0__90 < 16); (i__conflict__0__90++)) {{
      (current__89 = (*____in++))/*int*/;
      if (((current__89 & 1024) == 0)) {((*____out++)=current__89); } else {{
        ((ordering__87[(int)j__88]) = current__89)/*int*/;
        (j__88 = (j__88 + 1))/*int*/;
      }}
    }
  }
  for ((i__91 = 0)/*int*/; (i__91 < j__88); (i__91++)) {{
      ((*____out++)=(ordering__87[(int)i__91]));
    }
  }
  // mark end: SIRFilter Sort

}}


void work_Sort__92_19__11__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int ordering__87[16] = {0};/* int[16] */
  int j__88 = 0;/* int */
  int current__89 = 0;/* int */
  int i__conflict__0__90 = 0;/* int */
  int i__91 = 0;/* int */

  // mark begin: SIRFilter Sort

  (j__88 = 0)/*int*/;
  for ((i__conflict__0__90 = 0)/*int*/; (i__conflict__0__90 < 16); (i__conflict__0__90++)) {{
      (current__89 = (*____in++))/*int*/;
      if (((current__89 & 1024) == 0)) {((*____out++)=current__89); } else {{
        ((ordering__87[(int)j__88]) = current__89)/*int*/;
        (j__88 = (j__88 + 1))/*int*/;
      }}
    }
  }
  for ((i__91 = 0)/*int*/; (i__91 < j__88); (i__91++)) {{
      ((*____out++)=(ordering__87[(int)i__91]));
    }
  }
  // mark end: SIRFilter Sort

}}


#endif // BUFFER_MERGE


 
void work_Sort__92_19__11(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int ordering__87[16] = {0};/* int[16] */
      int j__88 = 0;/* int */
      int current__89 = 0;/* int */
      int i__conflict__0__90 = 0;/* int */
      int i__91 = 0;/* int */

      // mark begin: SIRFilter Sort

      (j__88 = 0)/*int*/;
      for ((i__conflict__0__90 = 0)/*int*/; (i__conflict__0__90 < 16); (i__conflict__0__90++)) {{
          (current__89 = __pop__11())/*int*/;
          if (((current__89 & 1024) == 0)) {__push__11(current__89); } else {{
            ((ordering__87[(int)j__88]) = current__89)/*int*/;
            (j__88 = (j__88 + 1))/*int*/;
          }}
        }
      }
      for ((i__91 = 0)/*int*/; (i__91 < j__88); (i__91++)) {{
          __push__11((ordering__87[(int)i__91]));
        }
      }
      // mark end: SIRFilter Sort

    }
  }
}

// peek: 1 pop: 1 push 0
// init counts: 0 steady counts: 16

// ClusterFusion isEliminated: false



int __number_of_iterations_12;
int __counter_12 = 0;
int __steady_12 = 0;
int __tmp_12 = 0;
int __tmp2_12 = 0;
int *__state_flag_12 = NULL;
thread_info *__thread_12 = NULL;



int x__93__12 = 0;
void save_peek_buffer__12(object_write_buffer *buf);
void load_peek_buffer__12(object_write_buffer *buf);
void save_file_pointer__12(object_write_buffer *buf);
void load_file_pointer__12(object_write_buffer *buf);

 
void init_IntPrinter__96_20__12();
inline void check_status__12();

void work_IntPrinter__96_20__12(int);


inline int __pop__12() {
int res=BUFFER_11_12[TAIL_11_12];
TAIL_11_12++;
return res;
}

inline int __pop__12(int n) {
int res=BUFFER_11_12[TAIL_11_12];
TAIL_11_12+=n;

return res;
}

inline int __peek__12(int offs) {
return BUFFER_11_12[TAIL_11_12+offs];
}


 
void init_IntPrinter__96_20__12(){
}
void save_file_pointer__12(object_write_buffer *buf) {}
void load_file_pointer__12(object_write_buffer *buf) {}
 
void work_IntPrinter__96_20__12(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter IntPrinter

      ((x__93__12) = __pop__12())/*int*/;
      sum+=x__93__12;
      // mark end: SIRFilter IntPrinter

    }
  }
}

