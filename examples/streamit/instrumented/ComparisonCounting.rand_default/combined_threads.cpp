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

#define LOC_RAND_MAX 1000

int __max_iteration;
int __timer_enabled = 0;
int __frequency_of_chkpts;
volatile int __vol;
volatile long long sum;
static unsigned int seed;
proc_timer tt("total runtime");


int BUFFER_0_1[__BUF_SIZE_MASK_0_1 + 1];
int HEAD_0_1 = 0;
int TAIL_0_1 = 0;
int BUFFER_1_2[__BUF_SIZE_MASK_1_2 + 1];
int HEAD_1_2 = 0;
int TAIL_1_2 = 0;
int BUFFER_1_6[__BUF_SIZE_MASK_1_6 + 1];
int HEAD_1_6 = 0;
int TAIL_1_6 = 0;
int BUFFER_1_7[__BUF_SIZE_MASK_1_7 + 1];
int HEAD_1_7 = 0;
int TAIL_1_7 = 0;
int BUFFER_1_8[__BUF_SIZE_MASK_1_8 + 1];
int HEAD_1_8 = 0;
int TAIL_1_8 = 0;
int BUFFER_1_9[__BUF_SIZE_MASK_1_9 + 1];
int HEAD_1_9 = 0;
int TAIL_1_9 = 0;
int BUFFER_1_10[__BUF_SIZE_MASK_1_10 + 1];
int HEAD_1_10 = 0;
int TAIL_1_10 = 0;
int BUFFER_1_11[__BUF_SIZE_MASK_1_11 + 1];
int HEAD_1_11 = 0;
int TAIL_1_11 = 0;
int BUFFER_1_12[__BUF_SIZE_MASK_1_12 + 1];
int HEAD_1_12 = 0;
int TAIL_1_12 = 0;
int BUFFER_1_13[__BUF_SIZE_MASK_1_13 + 1];
int HEAD_1_13 = 0;
int TAIL_1_13 = 0;
int BUFFER_1_14[__BUF_SIZE_MASK_1_14 + 1];
int HEAD_1_14 = 0;
int TAIL_1_14 = 0;
int BUFFER_1_15[__BUF_SIZE_MASK_1_15 + 1];
int HEAD_1_15 = 0;
int TAIL_1_15 = 0;
int BUFFER_1_16[__BUF_SIZE_MASK_1_16 + 1];
int HEAD_1_16 = 0;
int TAIL_1_16 = 0;
int BUFFER_1_17[__BUF_SIZE_MASK_1_17 + 1];
int HEAD_1_17 = 0;
int TAIL_1_17 = 0;
int BUFFER_1_18[__BUF_SIZE_MASK_1_18 + 1];
int HEAD_1_18 = 0;
int TAIL_1_18 = 0;
int BUFFER_1_19[__BUF_SIZE_MASK_1_19 + 1];
int HEAD_1_19 = 0;
int TAIL_1_19 = 0;
int BUFFER_1_20[__BUF_SIZE_MASK_1_20 + 1];
int HEAD_1_20 = 0;
int TAIL_1_20 = 0;
int BUFFER_1_21[__BUF_SIZE_MASK_1_21 + 1];
int HEAD_1_21 = 0;
int TAIL_1_21 = 0;
int BUFFER_2_3[__BUF_SIZE_MASK_2_3 + 1];
int HEAD_2_3 = 0;
int TAIL_2_3 = 0;
int BUFFER_3_4[__BUF_SIZE_MASK_3_4 + 1];
int HEAD_3_4 = 0;
int TAIL_3_4 = 0;
int BUFFER_4_5[__BUF_SIZE_MASK_4_5 + 1];
int HEAD_4_5 = 0;
int TAIL_4_5 = 0;
int BUFFER_6_3[__BUF_SIZE_MASK_6_3 + 1];
int HEAD_6_3 = 0;
int TAIL_6_3 = 0;
int BUFFER_7_3[__BUF_SIZE_MASK_7_3 + 1];
int HEAD_7_3 = 0;
int TAIL_7_3 = 0;
int BUFFER_8_3[__BUF_SIZE_MASK_8_3 + 1];
int HEAD_8_3 = 0;
int TAIL_8_3 = 0;
int BUFFER_9_3[__BUF_SIZE_MASK_9_3 + 1];
int HEAD_9_3 = 0;
int TAIL_9_3 = 0;
int BUFFER_10_3[__BUF_SIZE_MASK_10_3 + 1];
int HEAD_10_3 = 0;
int TAIL_10_3 = 0;
int BUFFER_11_3[__BUF_SIZE_MASK_11_3 + 1];
int HEAD_11_3 = 0;
int TAIL_11_3 = 0;
int BUFFER_12_3[__BUF_SIZE_MASK_12_3 + 1];
int HEAD_12_3 = 0;
int TAIL_12_3 = 0;
int BUFFER_13_3[__BUF_SIZE_MASK_13_3 + 1];
int HEAD_13_3 = 0;
int TAIL_13_3 = 0;
int BUFFER_14_3[__BUF_SIZE_MASK_14_3 + 1];
int HEAD_14_3 = 0;
int TAIL_14_3 = 0;
int BUFFER_15_3[__BUF_SIZE_MASK_15_3 + 1];
int HEAD_15_3 = 0;
int TAIL_15_3 = 0;
int BUFFER_16_3[__BUF_SIZE_MASK_16_3 + 1];
int HEAD_16_3 = 0;
int TAIL_16_3 = 0;
int BUFFER_17_3[__BUF_SIZE_MASK_17_3 + 1];
int HEAD_17_3 = 0;
int TAIL_17_3 = 0;
int BUFFER_18_3[__BUF_SIZE_MASK_18_3 + 1];
int HEAD_18_3 = 0;
int TAIL_18_3 = 0;
int BUFFER_19_3[__BUF_SIZE_MASK_19_3 + 1];
int HEAD_19_3 = 0;
int TAIL_19_3 = 0;
int BUFFER_20_3[__BUF_SIZE_MASK_20_3 + 1];
int HEAD_20_3 = 0;
int TAIL_20_3 = 0;
int BUFFER_21_3[__BUF_SIZE_MASK_21_3 + 1];
int HEAD_21_3 = 0;
int TAIL_21_3 = 0;
void init_IntSource__4_20__0();
void work_IntSource__4_20__0(int);
#ifdef BUFFER_MERGE
void work_IntSource__4_20__0__mod(int ____n, void *____in, int *____out);
void work_IntSource__4_20__0__mod2(int ____n, void *____in, int *____out, int s1, int s2);
#endif
void __splitter_1_work(int);
void init_Counter__11_22__2();
void work_Counter__11_22__2(int);
#ifdef BUFFER_MERGE
void work_Counter__11_22__2__mod(int ____n, int *____in, int *____out);
void work_Counter__11_22__2__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void __joiner_3_work(int);
void init_RearrangeIndices__125_39__4();
void work_RearrangeIndices__125_39__4(int);
#ifdef BUFFER_MERGE
void work_RearrangeIndices__125_39__4__mod(int ____n, int *____in, int *____out);
void work_RearrangeIndices__125_39__4__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_IntPrinter__129_40__5();
void work_IntPrinter__129_40__5(int);
#ifdef BUFFER_MERGE
void work_IntPrinter__129_40__5__mod(int ____n, int *____in, void *____out);
void work_IntPrinter__129_40__5__mod2(int ____n, int *____in, void *____out, int s1, int s2);
#endif
void init_Counter__18_23__6();
void work_Counter__18_23__6(int);
#ifdef BUFFER_MERGE
void work_Counter__18_23__6__mod(int ____n, int *____in, int *____out);
void work_Counter__18_23__6__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Counter__25_24__7();
void work_Counter__25_24__7(int);
#ifdef BUFFER_MERGE
void work_Counter__25_24__7__mod(int ____n, int *____in, int *____out);
void work_Counter__25_24__7__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Counter__32_25__8();
void work_Counter__32_25__8(int);
#ifdef BUFFER_MERGE
void work_Counter__32_25__8__mod(int ____n, int *____in, int *____out);
void work_Counter__32_25__8__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Counter__39_26__9();
void work_Counter__39_26__9(int);
#ifdef BUFFER_MERGE
void work_Counter__39_26__9__mod(int ____n, int *____in, int *____out);
void work_Counter__39_26__9__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Counter__46_27__10();
void work_Counter__46_27__10(int);
#ifdef BUFFER_MERGE
void work_Counter__46_27__10__mod(int ____n, int *____in, int *____out);
void work_Counter__46_27__10__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Counter__53_28__11();
void work_Counter__53_28__11(int);
#ifdef BUFFER_MERGE
void work_Counter__53_28__11__mod(int ____n, int *____in, int *____out);
void work_Counter__53_28__11__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Counter__60_29__12();
void work_Counter__60_29__12(int);
#ifdef BUFFER_MERGE
void work_Counter__60_29__12__mod(int ____n, int *____in, int *____out);
void work_Counter__60_29__12__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Counter__67_30__13();
void work_Counter__67_30__13(int);
#ifdef BUFFER_MERGE
void work_Counter__67_30__13__mod(int ____n, int *____in, int *____out);
void work_Counter__67_30__13__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Counter__74_31__14();
void work_Counter__74_31__14(int);
#ifdef BUFFER_MERGE
void work_Counter__74_31__14__mod(int ____n, int *____in, int *____out);
void work_Counter__74_31__14__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Counter__81_32__15();
void work_Counter__81_32__15(int);
#ifdef BUFFER_MERGE
void work_Counter__81_32__15__mod(int ____n, int *____in, int *____out);
void work_Counter__81_32__15__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Counter__88_33__16();
void work_Counter__88_33__16(int);
#ifdef BUFFER_MERGE
void work_Counter__88_33__16__mod(int ____n, int *____in, int *____out);
void work_Counter__88_33__16__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Counter__95_34__17();
void work_Counter__95_34__17(int);
#ifdef BUFFER_MERGE
void work_Counter__95_34__17__mod(int ____n, int *____in, int *____out);
void work_Counter__95_34__17__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Counter__102_35__18();
void work_Counter__102_35__18(int);
#ifdef BUFFER_MERGE
void work_Counter__102_35__18__mod(int ____n, int *____in, int *____out);
void work_Counter__102_35__18__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Counter__109_36__19();
void work_Counter__109_36__19(int);
#ifdef BUFFER_MERGE
void work_Counter__109_36__19__mod(int ____n, int *____in, int *____out);
void work_Counter__109_36__19__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Counter__116_37__20();
void work_Counter__116_37__20(int);
#ifdef BUFFER_MERGE
void work_Counter__116_37__20__mod(int ____n, int *____in, int *____out);
void work_Counter__116_37__20__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Identity__119_38__21();
void Identity__119_38__work__21(int);
#ifdef BUFFER_MERGE
void Identity__119_38__work__21__mod(int ____n, int *____in, int *____out);
void Identity__119_38__work__21__mod2(int ____n, int *____in, int *____out, int s1, int s2);
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
// number of phases: 6


#ifdef MEASURE_BY_RTS
RTS_init(1,1);
#else //default ``MEASURE_BY_PAPI''
PAPI_HW_COUNTER_init(1,1);
#endif
  // ============= Initialization =============

init_IntSource__4_20__0();
init_Counter__95_34__17();
init_Counter__102_35__18();
init_Counter__81_32__15();
init_Counter__88_33__16();
init_Identity__119_38__21();
init_Counter__18_23__6();
init_Counter__109_36__19();
init_Counter__11_22__2();
init_Counter__116_37__20();
init_Counter__32_25__8();
init_Counter__25_24__7();
init_Counter__46_27__10();
init_Counter__39_26__9();
init_Counter__60_29__12();
init_Counter__53_28__11();
init_Counter__74_31__14();
init_Counter__67_30__13();
init_RearrangeIndices__125_39__4();
init_IntPrinter__129_40__5();

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
    work_IntSource__4_20__0(16 );
HEAD_1_2 = 0;
TAIL_1_2 = 0;
HEAD_1_6 = 0;
TAIL_1_6 = 0;
HEAD_1_7 = 0;
TAIL_1_7 = 0;
HEAD_1_8 = 0;
TAIL_1_8 = 0;
HEAD_1_9 = 0;
TAIL_1_9 = 0;
HEAD_1_10 = 0;
TAIL_1_10 = 0;
HEAD_1_11 = 0;
TAIL_1_11 = 0;
HEAD_1_12 = 0;
TAIL_1_12 = 0;
HEAD_1_13 = 0;
TAIL_1_13 = 0;
HEAD_1_14 = 0;
TAIL_1_14 = 0;
HEAD_1_15 = 0;
TAIL_1_15 = 0;
HEAD_1_16 = 0;
TAIL_1_16 = 0;
HEAD_1_17 = 0;
TAIL_1_17 = 0;
HEAD_1_18 = 0;
TAIL_1_18 = 0;
HEAD_1_19 = 0;
TAIL_1_19 = 0;
HEAD_1_20 = 0;
TAIL_1_20 = 0;
HEAD_1_21 = 0;
TAIL_1_21 = 0;
    __splitter_1_work(16 );
HEAD_17_3 = 0;
TAIL_17_3 = 0;
    work_Counter__95_34__17(1 );
HEAD_18_3 = 0;
TAIL_18_3 = 0;
    work_Counter__102_35__18(1 );
HEAD_15_3 = 0;
TAIL_15_3 = 0;
    work_Counter__81_32__15(1 );
HEAD_16_3 = 0;
TAIL_16_3 = 0;
    work_Counter__88_33__16(1 );
HEAD_21_3 = 0;
TAIL_21_3 = 0;
    Identity__119_38__work__21(16 );
HEAD_6_3 = 0;
TAIL_6_3 = 0;
    work_Counter__18_23__6(1 );
HEAD_19_3 = 0;
TAIL_19_3 = 0;
    work_Counter__109_36__19(1 );
HEAD_2_3 = 0;
TAIL_2_3 = 0;
    work_Counter__11_22__2(1 );
HEAD_20_3 = 0;
TAIL_20_3 = 0;
    work_Counter__116_37__20(1 );
HEAD_8_3 = 0;
TAIL_8_3 = 0;
    work_Counter__32_25__8(1 );
HEAD_7_3 = 0;
TAIL_7_3 = 0;
    work_Counter__25_24__7(1 );
HEAD_10_3 = 0;
TAIL_10_3 = 0;
    work_Counter__46_27__10(1 );
HEAD_9_3 = 0;
TAIL_9_3 = 0;
    work_Counter__39_26__9(1 );
HEAD_12_3 = 0;
TAIL_12_3 = 0;
    work_Counter__60_29__12(1 );
HEAD_11_3 = 0;
TAIL_11_3 = 0;
    work_Counter__53_28__11(1 );
HEAD_14_3 = 0;
TAIL_14_3 = 0;
    work_Counter__74_31__14(1 );
HEAD_13_3 = 0;
TAIL_13_3 = 0;
    work_Counter__67_30__13(1 );
HEAD_3_4 = 0;
TAIL_3_4 = 0;
    __joiner_3_work(1 );
HEAD_4_5 = 0;
TAIL_4_5 = 0;
    work_RearrangeIndices__125_39__4(1 );
    work_IntPrinter__129_40__5(16 );
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
message *__msg_stack_3;
message *__msg_stack_10;
message *__msg_stack_7;
message *__msg_stack_6;
message *__msg_stack_20;
message *__msg_stack_9;
message *__msg_stack_4;
message *__msg_stack_0;
message *__msg_stack_8;
message *__msg_stack_12;
message *__msg_stack_1;
message *__msg_stack_11;
message *__msg_stack_14;
message *__msg_stack_19;
message *__msg_stack_5;
message *__msg_stack_15;
message *__msg_stack_2;
message *__msg_stack_13;
message *__msg_stack_18;
message *__msg_stack_17;
message *__msg_stack_16;
message *__msg_stack_21;

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

 
void init_IntSource__4_20__0();
inline void check_status__0();

void work_IntSource__4_20__0(int);



inline void __push__0(int data) {
BUFFER_0_1[HEAD_0_1]=data;
HEAD_0_1++;
}



 
void init_IntSource__4_20__0(){
    printf("Enter seed: ");
    scanf("%d", &seed);
    srand(seed);
}
void save_file_pointer__0(object_write_buffer *buf) {}
void load_file_pointer__0(object_write_buffer *buf) {}
 
void work_IntSource__4_20__0(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter IntSource

      __push__0(rand_r(&seed));
      // mark end: SIRFilter IntSource

    }
  }
}

// init counts: 0 steady counts: 16

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
  int tmp;
  tmp = BUFFER_0_1[TAIL_0_1]; TAIL_0_1++;
  BUFFER_1_2[HEAD_1_2]=tmp;
HEAD_1_2++; ;
  BUFFER_1_6[HEAD_1_6]=tmp;
HEAD_1_6++; ;
  BUFFER_1_7[HEAD_1_7]=tmp;
HEAD_1_7++; ;
  BUFFER_1_8[HEAD_1_8]=tmp;
HEAD_1_8++; ;
  BUFFER_1_9[HEAD_1_9]=tmp;
HEAD_1_9++; ;
  BUFFER_1_10[HEAD_1_10]=tmp;
HEAD_1_10++; ;
  BUFFER_1_11[HEAD_1_11]=tmp;
HEAD_1_11++; ;
  BUFFER_1_12[HEAD_1_12]=tmp;
HEAD_1_12++; ;
  BUFFER_1_13[HEAD_1_13]=tmp;
HEAD_1_13++; ;
  BUFFER_1_14[HEAD_1_14]=tmp;
HEAD_1_14++; ;
  BUFFER_1_15[HEAD_1_15]=tmp;
HEAD_1_15++; ;
  BUFFER_1_16[HEAD_1_16]=tmp;
HEAD_1_16++; ;
  BUFFER_1_17[HEAD_1_17]=tmp;
HEAD_1_17++; ;
  BUFFER_1_18[HEAD_1_18]=tmp;
HEAD_1_18++; ;
  BUFFER_1_19[HEAD_1_19]=tmp;
HEAD_1_19++; ;
  BUFFER_1_20[HEAD_1_20]=tmp;
HEAD_1_20++; ;
  BUFFER_1_21[HEAD_1_21]=tmp;
HEAD_1_21++; ;
  }
}


// peek: 16 pop: 16 push 1
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

 
void init_Counter__11_22__2();
inline void check_status__2();

void work_Counter__11_22__2(int);


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



 
void init_Counter__11_22__2(){
}
void save_file_pointer__2(object_write_buffer *buf) {}
void load_file_pointer__2(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Counter__11_22__2__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int mine__7 = 0;/* int */
  int count__8 = 0;/* int */
  int i__conflict__0__9 = 0;/* int */
  int i__10 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__7 = (*(____in+0)))/*int*/;
  (count__8 = 0)/*int*/;
  for ((i__conflict__0__9 = 0)/*int*/; (i__conflict__0__9 < 16); (i__conflict__0__9++)) {{
      if (((*(____in+i__conflict__0__9)) < mine__7)) {(count__8 = (count__8 + 1))/*int*/;}
    }
  }
  for ((i__10 = 0)/*int*/; (i__10 < 16); (i__10++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__8);
  // mark end: SIRFilter Counter

}}


void work_Counter__11_22__2__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int mine__7 = 0;/* int */
  int count__8 = 0;/* int */
  int i__conflict__0__9 = 0;/* int */
  int i__10 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__7 = (*(____in+0)))/*int*/;
  (count__8 = 0)/*int*/;
  for ((i__conflict__0__9 = 0)/*int*/; (i__conflict__0__9 < 16); (i__conflict__0__9++)) {{
      if (((*(____in+i__conflict__0__9)) < mine__7)) {(count__8 = (count__8 + 1))/*int*/;}
    }
  }
  for ((i__10 = 0)/*int*/; (i__10 < 16); (i__10++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__8);
  // mark end: SIRFilter Counter

}}


#endif // BUFFER_MERGE


 
void work_Counter__11_22__2(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int mine__7 = 0;/* int */
      int count__8 = 0;/* int */
      int i__conflict__0__9 = 0;/* int */
      int i__10 = 0;/* int */

      // mark begin: SIRFilter Counter

      (mine__7 = __peek__2(0))/*int*/;
      (count__8 = 0)/*int*/;
      for ((i__conflict__0__9 = 0)/*int*/; (i__conflict__0__9 < 16); (i__conflict__0__9++)) {{
          if ((__peek__2(i__conflict__0__9) < mine__7)) {(count__8 = (count__8 + 1))/*int*/;}
        }
      }
      for ((i__10 = 0)/*int*/; (i__10 < 16); (i__10++)) {{
          __pop__2();
        }
      }
      __push__2(count__8);
      // mark end: SIRFilter Counter

    }
  }
}

// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_3;
int __counter_3 = 0;
int __steady_3 = 0;
int __tmp_3 = 0;
int __tmp2_3 = 0;
int *__state_flag_3 = NULL;
thread_info *__thread_3 = NULL;




void __joiner_3_work(int ____n) {
  for (;____n > 0; ____n--) {
BUFFER_3_4[HEAD_3_4]=BUFFER_2_3[TAIL_2_3];
HEAD_3_4++; ; TAIL_2_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_6_3[TAIL_6_3];
HEAD_3_4++; ; TAIL_6_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_7_3[TAIL_7_3];
HEAD_3_4++; ; TAIL_7_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_8_3[TAIL_8_3];
HEAD_3_4++; ; TAIL_8_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_9_3[TAIL_9_3];
HEAD_3_4++; ; TAIL_9_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_10_3[TAIL_10_3];
HEAD_3_4++; ; TAIL_10_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_11_3[TAIL_11_3];
HEAD_3_4++; ; TAIL_11_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_12_3[TAIL_12_3];
HEAD_3_4++; ; TAIL_12_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_13_3[TAIL_13_3];
HEAD_3_4++; ; TAIL_13_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_14_3[TAIL_14_3];
HEAD_3_4++; ; TAIL_14_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_15_3[TAIL_15_3];
HEAD_3_4++; ; TAIL_15_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_16_3[TAIL_16_3];
HEAD_3_4++; ; TAIL_16_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_17_3[TAIL_17_3];
HEAD_3_4++; ; TAIL_17_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_18_3[TAIL_18_3];
HEAD_3_4++; ; TAIL_18_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_19_3[TAIL_19_3];
HEAD_3_4++; ; TAIL_19_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_20_3[TAIL_20_3];
HEAD_3_4++; ; TAIL_20_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_21_3[TAIL_21_3];
HEAD_3_4++; ; TAIL_21_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_21_3[TAIL_21_3];
HEAD_3_4++; ; TAIL_21_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_21_3[TAIL_21_3];
HEAD_3_4++; ; TAIL_21_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_21_3[TAIL_21_3];
HEAD_3_4++; ; TAIL_21_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_21_3[TAIL_21_3];
HEAD_3_4++; ; TAIL_21_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_21_3[TAIL_21_3];
HEAD_3_4++; ; TAIL_21_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_21_3[TAIL_21_3];
HEAD_3_4++; ; TAIL_21_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_21_3[TAIL_21_3];
HEAD_3_4++; ; TAIL_21_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_21_3[TAIL_21_3];
HEAD_3_4++; ; TAIL_21_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_21_3[TAIL_21_3];
HEAD_3_4++; ; TAIL_21_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_21_3[TAIL_21_3];
HEAD_3_4++; ; TAIL_21_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_21_3[TAIL_21_3];
HEAD_3_4++; ; TAIL_21_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_21_3[TAIL_21_3];
HEAD_3_4++; ; TAIL_21_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_21_3[TAIL_21_3];
HEAD_3_4++; ; TAIL_21_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_21_3[TAIL_21_3];
HEAD_3_4++; ; TAIL_21_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_21_3[TAIL_21_3];
HEAD_3_4++; ; TAIL_21_3++;
  }
}


// peek: 32 pop: 32 push 16
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

 
void init_RearrangeIndices__125_39__4();
inline void check_status__4();

void work_RearrangeIndices__125_39__4(int);


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



 
void init_RearrangeIndices__125_39__4(){
}
void save_file_pointer__4(object_write_buffer *buf) {}
void load_file_pointer__4(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_RearrangeIndices__125_39__4__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int outputArray__122[32] = {0};/* int[32] */
  int i__conflict__0__123 = 0;/* int */
  int i__124 = 0;/* int */

  // mark begin: SIRFilter RearrangeIndices

  for ((i__conflict__0__123 = 0)/*int*/; (i__conflict__0__123 < 16); (i__conflict__0__123++)) {{
      ((outputArray__122[(int)(*(____in+i__conflict__0__123))]) = (*(____in+(i__conflict__0__123 + 16))))/*int*/;
    }
  }
  for ((i__124 = 0)/*int*/; (i__124 < 16); (i__124++)) {{
      ((*____out++)=(outputArray__122[(int)i__124]));
      assert(false);__pop__4(2);
    }
  }
  // mark end: SIRFilter RearrangeIndices

}}


void work_RearrangeIndices__125_39__4__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int outputArray__122[32] = {0};/* int[32] */
  int i__conflict__0__123 = 0;/* int */
  int i__124 = 0;/* int */

  // mark begin: SIRFilter RearrangeIndices

  for ((i__conflict__0__123 = 0)/*int*/; (i__conflict__0__123 < 16); (i__conflict__0__123++)) {{
      ((outputArray__122[(int)(*(____in+i__conflict__0__123))]) = (*(____in+(i__conflict__0__123 + 16))))/*int*/;
    }
  }
  for ((i__124 = 0)/*int*/; (i__124 < 16); (i__124++)) {{
      ((*____out++)=(outputArray__122[(int)i__124]));
      assert(false);__pop__4(2);
    }
  }
  // mark end: SIRFilter RearrangeIndices

}}


#endif // BUFFER_MERGE


 
void work_RearrangeIndices__125_39__4(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int outputArray__122[32] = {0};/* int[32] */
      int i__conflict__0__123 = 0;/* int */
      int i__124 = 0;/* int */

      // mark begin: SIRFilter RearrangeIndices

      for ((i__conflict__0__123 = 0)/*int*/; (i__conflict__0__123 < 16); (i__conflict__0__123++)) {{
          ((outputArray__122[(int)__peek__4(i__conflict__0__123)]) = __peek__4((i__conflict__0__123 + 16)))/*int*/;
        }
      }
      for ((i__124 = 0)/*int*/; (i__124 < 16); (i__124++)) {{
          __push__4((outputArray__122[(int)i__124]));
          __pop__4(2);
        }
      }
      // mark end: SIRFilter RearrangeIndices

    }
  }
}

// peek: 1 pop: 1 push 0
// init counts: 0 steady counts: 16

// ClusterFusion isEliminated: false



int __number_of_iterations_5;
int __counter_5 = 0;
int __steady_5 = 0;
int __tmp_5 = 0;
int __tmp2_5 = 0;
int *__state_flag_5 = NULL;
thread_info *__thread_5 = NULL;



int x__126__5 = 0;
void save_peek_buffer__5(object_write_buffer *buf);
void load_peek_buffer__5(object_write_buffer *buf);
void save_file_pointer__5(object_write_buffer *buf);
void load_file_pointer__5(object_write_buffer *buf);

 
void init_IntPrinter__129_40__5();
inline void check_status__5();

void work_IntPrinter__129_40__5(int);


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


 
void init_IntPrinter__129_40__5(){
}
void save_file_pointer__5(object_write_buffer *buf) {}
void load_file_pointer__5(object_write_buffer *buf) {}
 
void work_IntPrinter__129_40__5(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter IntPrinter

      ((x__126__5) = __pop__5())/*int*/;

      sum+=x__126__5;
      // mark end: SIRFilter IntPrinter

    }
  }
}

// peek: 16 pop: 16 push 1
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

 
void init_Counter__18_23__6();
inline void check_status__6();

void work_Counter__18_23__6(int);


inline int __pop__6() {
int res=BUFFER_1_6[TAIL_1_6];
TAIL_1_6++;
return res;
}

inline int __pop__6(int n) {
int res=BUFFER_1_6[TAIL_1_6];
TAIL_1_6+=n;

return res;
}

inline int __peek__6(int offs) {
return BUFFER_1_6[TAIL_1_6+offs];
}



inline void __push__6(int data) {
BUFFER_6_3[HEAD_6_3]=data;
HEAD_6_3++;
}



 
void init_Counter__18_23__6(){
}
void save_file_pointer__6(object_write_buffer *buf) {}
void load_file_pointer__6(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Counter__18_23__6__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int mine__14 = 0;/* int */
  int count__15 = 0;/* int */
  int i__conflict__0__16 = 0;/* int */
  int i__17 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__14 = (*(____in+1)))/*int*/;
  (count__15 = 0)/*int*/;
  for ((i__conflict__0__16 = 0)/*int*/; (i__conflict__0__16 < 16); (i__conflict__0__16++)) {{
      if (((*(____in+i__conflict__0__16)) < mine__14)) {(count__15 = (count__15 + 1))/*int*/;}
    }
  }
  for ((i__17 = 0)/*int*/; (i__17 < 16); (i__17++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__15);
  // mark end: SIRFilter Counter

}}


void work_Counter__18_23__6__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int mine__14 = 0;/* int */
  int count__15 = 0;/* int */
  int i__conflict__0__16 = 0;/* int */
  int i__17 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__14 = (*(____in+1)))/*int*/;
  (count__15 = 0)/*int*/;
  for ((i__conflict__0__16 = 0)/*int*/; (i__conflict__0__16 < 16); (i__conflict__0__16++)) {{
      if (((*(____in+i__conflict__0__16)) < mine__14)) {(count__15 = (count__15 + 1))/*int*/;}
    }
  }
  for ((i__17 = 0)/*int*/; (i__17 < 16); (i__17++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__15);
  // mark end: SIRFilter Counter

}}


#endif // BUFFER_MERGE


 
void work_Counter__18_23__6(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int mine__14 = 0;/* int */
      int count__15 = 0;/* int */
      int i__conflict__0__16 = 0;/* int */
      int i__17 = 0;/* int */

      // mark begin: SIRFilter Counter

      (mine__14 = __peek__6(1))/*int*/;
      (count__15 = 0)/*int*/;
      for ((i__conflict__0__16 = 0)/*int*/; (i__conflict__0__16 < 16); (i__conflict__0__16++)) {{
          if ((__peek__6(i__conflict__0__16) < mine__14)) {(count__15 = (count__15 + 1))/*int*/;}
        }
      }
      for ((i__17 = 0)/*int*/; (i__17 < 16); (i__17++)) {{
          __pop__6();
        }
      }
      __push__6(count__15);
      // mark end: SIRFilter Counter

    }
  }
}

// peek: 16 pop: 16 push 1
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

 
void init_Counter__25_24__7();
inline void check_status__7();

void work_Counter__25_24__7(int);


inline int __pop__7() {
int res=BUFFER_1_7[TAIL_1_7];
TAIL_1_7++;
return res;
}

inline int __pop__7(int n) {
int res=BUFFER_1_7[TAIL_1_7];
TAIL_1_7+=n;

return res;
}

inline int __peek__7(int offs) {
return BUFFER_1_7[TAIL_1_7+offs];
}



inline void __push__7(int data) {
BUFFER_7_3[HEAD_7_3]=data;
HEAD_7_3++;
}



 
void init_Counter__25_24__7(){
}
void save_file_pointer__7(object_write_buffer *buf) {}
void load_file_pointer__7(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Counter__25_24__7__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int mine__21 = 0;/* int */
  int count__22 = 0;/* int */
  int i__conflict__0__23 = 0;/* int */
  int i__24 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__21 = (*(____in+2)))/*int*/;
  (count__22 = 0)/*int*/;
  for ((i__conflict__0__23 = 0)/*int*/; (i__conflict__0__23 < 16); (i__conflict__0__23++)) {{
      if (((*(____in+i__conflict__0__23)) < mine__21)) {(count__22 = (count__22 + 1))/*int*/;}
    }
  }
  for ((i__24 = 0)/*int*/; (i__24 < 16); (i__24++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__22);
  // mark end: SIRFilter Counter

}}


void work_Counter__25_24__7__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int mine__21 = 0;/* int */
  int count__22 = 0;/* int */
  int i__conflict__0__23 = 0;/* int */
  int i__24 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__21 = (*(____in+2)))/*int*/;
  (count__22 = 0)/*int*/;
  for ((i__conflict__0__23 = 0)/*int*/; (i__conflict__0__23 < 16); (i__conflict__0__23++)) {{
      if (((*(____in+i__conflict__0__23)) < mine__21)) {(count__22 = (count__22 + 1))/*int*/;}
    }
  }
  for ((i__24 = 0)/*int*/; (i__24 < 16); (i__24++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__22);
  // mark end: SIRFilter Counter

}}


#endif // BUFFER_MERGE


 
void work_Counter__25_24__7(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int mine__21 = 0;/* int */
      int count__22 = 0;/* int */
      int i__conflict__0__23 = 0;/* int */
      int i__24 = 0;/* int */

      // mark begin: SIRFilter Counter

      (mine__21 = __peek__7(2))/*int*/;
      (count__22 = 0)/*int*/;
      for ((i__conflict__0__23 = 0)/*int*/; (i__conflict__0__23 < 16); (i__conflict__0__23++)) {{
          if ((__peek__7(i__conflict__0__23) < mine__21)) {(count__22 = (count__22 + 1))/*int*/;}
        }
      }
      for ((i__24 = 0)/*int*/; (i__24 < 16); (i__24++)) {{
          __pop__7();
        }
      }
      __push__7(count__22);
      // mark end: SIRFilter Counter

    }
  }
}

// peek: 16 pop: 16 push 1
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

 
void init_Counter__32_25__8();
inline void check_status__8();

void work_Counter__32_25__8(int);


inline int __pop__8() {
int res=BUFFER_1_8[TAIL_1_8];
TAIL_1_8++;
return res;
}

inline int __pop__8(int n) {
int res=BUFFER_1_8[TAIL_1_8];
TAIL_1_8+=n;

return res;
}

inline int __peek__8(int offs) {
return BUFFER_1_8[TAIL_1_8+offs];
}



inline void __push__8(int data) {
BUFFER_8_3[HEAD_8_3]=data;
HEAD_8_3++;
}



 
void init_Counter__32_25__8(){
}
void save_file_pointer__8(object_write_buffer *buf) {}
void load_file_pointer__8(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Counter__32_25__8__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int mine__28 = 0;/* int */
  int count__29 = 0;/* int */
  int i__conflict__0__30 = 0;/* int */
  int i__31 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__28 = (*(____in+3)))/*int*/;
  (count__29 = 0)/*int*/;
  for ((i__conflict__0__30 = 0)/*int*/; (i__conflict__0__30 < 16); (i__conflict__0__30++)) {{
      if (((*(____in+i__conflict__0__30)) < mine__28)) {(count__29 = (count__29 + 1))/*int*/;}
    }
  }
  for ((i__31 = 0)/*int*/; (i__31 < 16); (i__31++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__29);
  // mark end: SIRFilter Counter

}}


void work_Counter__32_25__8__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int mine__28 = 0;/* int */
  int count__29 = 0;/* int */
  int i__conflict__0__30 = 0;/* int */
  int i__31 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__28 = (*(____in+3)))/*int*/;
  (count__29 = 0)/*int*/;
  for ((i__conflict__0__30 = 0)/*int*/; (i__conflict__0__30 < 16); (i__conflict__0__30++)) {{
      if (((*(____in+i__conflict__0__30)) < mine__28)) {(count__29 = (count__29 + 1))/*int*/;}
    }
  }
  for ((i__31 = 0)/*int*/; (i__31 < 16); (i__31++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__29);
  // mark end: SIRFilter Counter

}}


#endif // BUFFER_MERGE


 
void work_Counter__32_25__8(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int mine__28 = 0;/* int */
      int count__29 = 0;/* int */
      int i__conflict__0__30 = 0;/* int */
      int i__31 = 0;/* int */

      // mark begin: SIRFilter Counter

      (mine__28 = __peek__8(3))/*int*/;
      (count__29 = 0)/*int*/;
      for ((i__conflict__0__30 = 0)/*int*/; (i__conflict__0__30 < 16); (i__conflict__0__30++)) {{
          if ((__peek__8(i__conflict__0__30) < mine__28)) {(count__29 = (count__29 + 1))/*int*/;}
        }
      }
      for ((i__31 = 0)/*int*/; (i__31 < 16); (i__31++)) {{
          __pop__8();
        }
      }
      __push__8(count__29);
      // mark end: SIRFilter Counter

    }
  }
}

// peek: 16 pop: 16 push 1
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

 
void init_Counter__39_26__9();
inline void check_status__9();

void work_Counter__39_26__9(int);


inline int __pop__9() {
int res=BUFFER_1_9[TAIL_1_9];
TAIL_1_9++;
return res;
}

inline int __pop__9(int n) {
int res=BUFFER_1_9[TAIL_1_9];
TAIL_1_9+=n;

return res;
}

inline int __peek__9(int offs) {
return BUFFER_1_9[TAIL_1_9+offs];
}



inline void __push__9(int data) {
BUFFER_9_3[HEAD_9_3]=data;
HEAD_9_3++;
}



 
void init_Counter__39_26__9(){
}
void save_file_pointer__9(object_write_buffer *buf) {}
void load_file_pointer__9(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Counter__39_26__9__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int mine__35 = 0;/* int */
  int count__36 = 0;/* int */
  int i__conflict__0__37 = 0;/* int */
  int i__38 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__35 = (*(____in+4)))/*int*/;
  (count__36 = 0)/*int*/;
  for ((i__conflict__0__37 = 0)/*int*/; (i__conflict__0__37 < 16); (i__conflict__0__37++)) {{
      if (((*(____in+i__conflict__0__37)) < mine__35)) {(count__36 = (count__36 + 1))/*int*/;}
    }
  }
  for ((i__38 = 0)/*int*/; (i__38 < 16); (i__38++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__36);
  // mark end: SIRFilter Counter

}}


void work_Counter__39_26__9__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int mine__35 = 0;/* int */
  int count__36 = 0;/* int */
  int i__conflict__0__37 = 0;/* int */
  int i__38 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__35 = (*(____in+4)))/*int*/;
  (count__36 = 0)/*int*/;
  for ((i__conflict__0__37 = 0)/*int*/; (i__conflict__0__37 < 16); (i__conflict__0__37++)) {{
      if (((*(____in+i__conflict__0__37)) < mine__35)) {(count__36 = (count__36 + 1))/*int*/;}
    }
  }
  for ((i__38 = 0)/*int*/; (i__38 < 16); (i__38++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__36);
  // mark end: SIRFilter Counter

}}


#endif // BUFFER_MERGE


 
void work_Counter__39_26__9(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int mine__35 = 0;/* int */
      int count__36 = 0;/* int */
      int i__conflict__0__37 = 0;/* int */
      int i__38 = 0;/* int */

      // mark begin: SIRFilter Counter

      (mine__35 = __peek__9(4))/*int*/;
      (count__36 = 0)/*int*/;
      for ((i__conflict__0__37 = 0)/*int*/; (i__conflict__0__37 < 16); (i__conflict__0__37++)) {{
          if ((__peek__9(i__conflict__0__37) < mine__35)) {(count__36 = (count__36 + 1))/*int*/;}
        }
      }
      for ((i__38 = 0)/*int*/; (i__38 < 16); (i__38++)) {{
          __pop__9();
        }
      }
      __push__9(count__36);
      // mark end: SIRFilter Counter

    }
  }
}

// peek: 16 pop: 16 push 1
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

 
void init_Counter__46_27__10();
inline void check_status__10();

void work_Counter__46_27__10(int);


inline int __pop__10() {
int res=BUFFER_1_10[TAIL_1_10];
TAIL_1_10++;
return res;
}

inline int __pop__10(int n) {
int res=BUFFER_1_10[TAIL_1_10];
TAIL_1_10+=n;

return res;
}

inline int __peek__10(int offs) {
return BUFFER_1_10[TAIL_1_10+offs];
}



inline void __push__10(int data) {
BUFFER_10_3[HEAD_10_3]=data;
HEAD_10_3++;
}



 
void init_Counter__46_27__10(){
}
void save_file_pointer__10(object_write_buffer *buf) {}
void load_file_pointer__10(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Counter__46_27__10__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int mine__42 = 0;/* int */
  int count__43 = 0;/* int */
  int i__conflict__0__44 = 0;/* int */
  int i__45 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__42 = (*(____in+5)))/*int*/;
  (count__43 = 0)/*int*/;
  for ((i__conflict__0__44 = 0)/*int*/; (i__conflict__0__44 < 16); (i__conflict__0__44++)) {{
      if (((*(____in+i__conflict__0__44)) < mine__42)) {(count__43 = (count__43 + 1))/*int*/;}
    }
  }
  for ((i__45 = 0)/*int*/; (i__45 < 16); (i__45++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__43);
  // mark end: SIRFilter Counter

}}


void work_Counter__46_27__10__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int mine__42 = 0;/* int */
  int count__43 = 0;/* int */
  int i__conflict__0__44 = 0;/* int */
  int i__45 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__42 = (*(____in+5)))/*int*/;
  (count__43 = 0)/*int*/;
  for ((i__conflict__0__44 = 0)/*int*/; (i__conflict__0__44 < 16); (i__conflict__0__44++)) {{
      if (((*(____in+i__conflict__0__44)) < mine__42)) {(count__43 = (count__43 + 1))/*int*/;}
    }
  }
  for ((i__45 = 0)/*int*/; (i__45 < 16); (i__45++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__43);
  // mark end: SIRFilter Counter

}}


#endif // BUFFER_MERGE


 
void work_Counter__46_27__10(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int mine__42 = 0;/* int */
      int count__43 = 0;/* int */
      int i__conflict__0__44 = 0;/* int */
      int i__45 = 0;/* int */

      // mark begin: SIRFilter Counter

      (mine__42 = __peek__10(5))/*int*/;
      (count__43 = 0)/*int*/;
      for ((i__conflict__0__44 = 0)/*int*/; (i__conflict__0__44 < 16); (i__conflict__0__44++)) {{
          if ((__peek__10(i__conflict__0__44) < mine__42)) {(count__43 = (count__43 + 1))/*int*/;}
        }
      }
      for ((i__45 = 0)/*int*/; (i__45 < 16); (i__45++)) {{
          __pop__10();
        }
      }
      __push__10(count__43);
      // mark end: SIRFilter Counter

    }
  }
}

// peek: 16 pop: 16 push 1
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

 
void init_Counter__53_28__11();
inline void check_status__11();

void work_Counter__53_28__11(int);


inline int __pop__11() {
int res=BUFFER_1_11[TAIL_1_11];
TAIL_1_11++;
return res;
}

inline int __pop__11(int n) {
int res=BUFFER_1_11[TAIL_1_11];
TAIL_1_11+=n;

return res;
}

inline int __peek__11(int offs) {
return BUFFER_1_11[TAIL_1_11+offs];
}



inline void __push__11(int data) {
BUFFER_11_3[HEAD_11_3]=data;
HEAD_11_3++;
}



 
void init_Counter__53_28__11(){
}
void save_file_pointer__11(object_write_buffer *buf) {}
void load_file_pointer__11(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Counter__53_28__11__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int mine__49 = 0;/* int */
  int count__50 = 0;/* int */
  int i__conflict__0__51 = 0;/* int */
  int i__52 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__49 = (*(____in+6)))/*int*/;
  (count__50 = 0)/*int*/;
  for ((i__conflict__0__51 = 0)/*int*/; (i__conflict__0__51 < 16); (i__conflict__0__51++)) {{
      if (((*(____in+i__conflict__0__51)) < mine__49)) {(count__50 = (count__50 + 1))/*int*/;}
    }
  }
  for ((i__52 = 0)/*int*/; (i__52 < 16); (i__52++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__50);
  // mark end: SIRFilter Counter

}}


void work_Counter__53_28__11__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int mine__49 = 0;/* int */
  int count__50 = 0;/* int */
  int i__conflict__0__51 = 0;/* int */
  int i__52 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__49 = (*(____in+6)))/*int*/;
  (count__50 = 0)/*int*/;
  for ((i__conflict__0__51 = 0)/*int*/; (i__conflict__0__51 < 16); (i__conflict__0__51++)) {{
      if (((*(____in+i__conflict__0__51)) < mine__49)) {(count__50 = (count__50 + 1))/*int*/;}
    }
  }
  for ((i__52 = 0)/*int*/; (i__52 < 16); (i__52++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__50);
  // mark end: SIRFilter Counter

}}


#endif // BUFFER_MERGE


 
void work_Counter__53_28__11(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int mine__49 = 0;/* int */
      int count__50 = 0;/* int */
      int i__conflict__0__51 = 0;/* int */
      int i__52 = 0;/* int */

      // mark begin: SIRFilter Counter

      (mine__49 = __peek__11(6))/*int*/;
      (count__50 = 0)/*int*/;
      for ((i__conflict__0__51 = 0)/*int*/; (i__conflict__0__51 < 16); (i__conflict__0__51++)) {{
          if ((__peek__11(i__conflict__0__51) < mine__49)) {(count__50 = (count__50 + 1))/*int*/;}
        }
      }
      for ((i__52 = 0)/*int*/; (i__52 < 16); (i__52++)) {{
          __pop__11();
        }
      }
      __push__11(count__50);
      // mark end: SIRFilter Counter

    }
  }
}

// peek: 16 pop: 16 push 1
// init counts: 0 steady counts: 1

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

 
void init_Counter__60_29__12();
inline void check_status__12();

void work_Counter__60_29__12(int);


inline int __pop__12() {
int res=BUFFER_1_12[TAIL_1_12];
TAIL_1_12++;
return res;
}

inline int __pop__12(int n) {
int res=BUFFER_1_12[TAIL_1_12];
TAIL_1_12+=n;

return res;
}

inline int __peek__12(int offs) {
return BUFFER_1_12[TAIL_1_12+offs];
}



inline void __push__12(int data) {
BUFFER_12_3[HEAD_12_3]=data;
HEAD_12_3++;
}



 
void init_Counter__60_29__12(){
}
void save_file_pointer__12(object_write_buffer *buf) {}
void load_file_pointer__12(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Counter__60_29__12__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int mine__56 = 0;/* int */
  int count__57 = 0;/* int */
  int i__conflict__0__58 = 0;/* int */
  int i__59 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__56 = (*(____in+7)))/*int*/;
  (count__57 = 0)/*int*/;
  for ((i__conflict__0__58 = 0)/*int*/; (i__conflict__0__58 < 16); (i__conflict__0__58++)) {{
      if (((*(____in+i__conflict__0__58)) < mine__56)) {(count__57 = (count__57 + 1))/*int*/;}
    }
  }
  for ((i__59 = 0)/*int*/; (i__59 < 16); (i__59++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__57);
  // mark end: SIRFilter Counter

}}


void work_Counter__60_29__12__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int mine__56 = 0;/* int */
  int count__57 = 0;/* int */
  int i__conflict__0__58 = 0;/* int */
  int i__59 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__56 = (*(____in+7)))/*int*/;
  (count__57 = 0)/*int*/;
  for ((i__conflict__0__58 = 0)/*int*/; (i__conflict__0__58 < 16); (i__conflict__0__58++)) {{
      if (((*(____in+i__conflict__0__58)) < mine__56)) {(count__57 = (count__57 + 1))/*int*/;}
    }
  }
  for ((i__59 = 0)/*int*/; (i__59 < 16); (i__59++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__57);
  // mark end: SIRFilter Counter

}}


#endif // BUFFER_MERGE


 
void work_Counter__60_29__12(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int mine__56 = 0;/* int */
      int count__57 = 0;/* int */
      int i__conflict__0__58 = 0;/* int */
      int i__59 = 0;/* int */

      // mark begin: SIRFilter Counter

      (mine__56 = __peek__12(7))/*int*/;
      (count__57 = 0)/*int*/;
      for ((i__conflict__0__58 = 0)/*int*/; (i__conflict__0__58 < 16); (i__conflict__0__58++)) {{
          if ((__peek__12(i__conflict__0__58) < mine__56)) {(count__57 = (count__57 + 1))/*int*/;}
        }
      }
      for ((i__59 = 0)/*int*/; (i__59 < 16); (i__59++)) {{
          __pop__12();
        }
      }
      __push__12(count__57);
      // mark end: SIRFilter Counter

    }
  }
}

// peek: 16 pop: 16 push 1
// init counts: 0 steady counts: 1

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

 
void init_Counter__67_30__13();
inline void check_status__13();

void work_Counter__67_30__13(int);


inline int __pop__13() {
int res=BUFFER_1_13[TAIL_1_13];
TAIL_1_13++;
return res;
}

inline int __pop__13(int n) {
int res=BUFFER_1_13[TAIL_1_13];
TAIL_1_13+=n;

return res;
}

inline int __peek__13(int offs) {
return BUFFER_1_13[TAIL_1_13+offs];
}



inline void __push__13(int data) {
BUFFER_13_3[HEAD_13_3]=data;
HEAD_13_3++;
}



 
void init_Counter__67_30__13(){
}
void save_file_pointer__13(object_write_buffer *buf) {}
void load_file_pointer__13(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Counter__67_30__13__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int mine__63 = 0;/* int */
  int count__64 = 0;/* int */
  int i__conflict__0__65 = 0;/* int */
  int i__66 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__63 = (*(____in+8)))/*int*/;
  (count__64 = 0)/*int*/;
  for ((i__conflict__0__65 = 0)/*int*/; (i__conflict__0__65 < 16); (i__conflict__0__65++)) {{
      if (((*(____in+i__conflict__0__65)) < mine__63)) {(count__64 = (count__64 + 1))/*int*/;}
    }
  }
  for ((i__66 = 0)/*int*/; (i__66 < 16); (i__66++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__64);
  // mark end: SIRFilter Counter

}}


void work_Counter__67_30__13__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int mine__63 = 0;/* int */
  int count__64 = 0;/* int */
  int i__conflict__0__65 = 0;/* int */
  int i__66 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__63 = (*(____in+8)))/*int*/;
  (count__64 = 0)/*int*/;
  for ((i__conflict__0__65 = 0)/*int*/; (i__conflict__0__65 < 16); (i__conflict__0__65++)) {{
      if (((*(____in+i__conflict__0__65)) < mine__63)) {(count__64 = (count__64 + 1))/*int*/;}
    }
  }
  for ((i__66 = 0)/*int*/; (i__66 < 16); (i__66++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__64);
  // mark end: SIRFilter Counter

}}


#endif // BUFFER_MERGE


 
void work_Counter__67_30__13(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int mine__63 = 0;/* int */
      int count__64 = 0;/* int */
      int i__conflict__0__65 = 0;/* int */
      int i__66 = 0;/* int */

      // mark begin: SIRFilter Counter

      (mine__63 = __peek__13(8))/*int*/;
      (count__64 = 0)/*int*/;
      for ((i__conflict__0__65 = 0)/*int*/; (i__conflict__0__65 < 16); (i__conflict__0__65++)) {{
          if ((__peek__13(i__conflict__0__65) < mine__63)) {(count__64 = (count__64 + 1))/*int*/;}
        }
      }
      for ((i__66 = 0)/*int*/; (i__66 < 16); (i__66++)) {{
          __pop__13();
        }
      }
      __push__13(count__64);
      // mark end: SIRFilter Counter

    }
  }
}

// peek: 16 pop: 16 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_14;
int __counter_14 = 0;
int __steady_14 = 0;
int __tmp_14 = 0;
int __tmp2_14 = 0;
int *__state_flag_14 = NULL;
thread_info *__thread_14 = NULL;



void save_peek_buffer__14(object_write_buffer *buf);
void load_peek_buffer__14(object_write_buffer *buf);
void save_file_pointer__14(object_write_buffer *buf);
void load_file_pointer__14(object_write_buffer *buf);

 
void init_Counter__74_31__14();
inline void check_status__14();

void work_Counter__74_31__14(int);


inline int __pop__14() {
int res=BUFFER_1_14[TAIL_1_14];
TAIL_1_14++;
return res;
}

inline int __pop__14(int n) {
int res=BUFFER_1_14[TAIL_1_14];
TAIL_1_14+=n;

return res;
}

inline int __peek__14(int offs) {
return BUFFER_1_14[TAIL_1_14+offs];
}



inline void __push__14(int data) {
BUFFER_14_3[HEAD_14_3]=data;
HEAD_14_3++;
}



 
void init_Counter__74_31__14(){
}
void save_file_pointer__14(object_write_buffer *buf) {}
void load_file_pointer__14(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Counter__74_31__14__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int mine__70 = 0;/* int */
  int count__71 = 0;/* int */
  int i__conflict__0__72 = 0;/* int */
  int i__73 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__70 = (*(____in+9)))/*int*/;
  (count__71 = 0)/*int*/;
  for ((i__conflict__0__72 = 0)/*int*/; (i__conflict__0__72 < 16); (i__conflict__0__72++)) {{
      if (((*(____in+i__conflict__0__72)) < mine__70)) {(count__71 = (count__71 + 1))/*int*/;}
    }
  }
  for ((i__73 = 0)/*int*/; (i__73 < 16); (i__73++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__71);
  // mark end: SIRFilter Counter

}}


void work_Counter__74_31__14__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int mine__70 = 0;/* int */
  int count__71 = 0;/* int */
  int i__conflict__0__72 = 0;/* int */
  int i__73 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__70 = (*(____in+9)))/*int*/;
  (count__71 = 0)/*int*/;
  for ((i__conflict__0__72 = 0)/*int*/; (i__conflict__0__72 < 16); (i__conflict__0__72++)) {{
      if (((*(____in+i__conflict__0__72)) < mine__70)) {(count__71 = (count__71 + 1))/*int*/;}
    }
  }
  for ((i__73 = 0)/*int*/; (i__73 < 16); (i__73++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__71);
  // mark end: SIRFilter Counter

}}


#endif // BUFFER_MERGE


 
void work_Counter__74_31__14(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int mine__70 = 0;/* int */
      int count__71 = 0;/* int */
      int i__conflict__0__72 = 0;/* int */
      int i__73 = 0;/* int */

      // mark begin: SIRFilter Counter

      (mine__70 = __peek__14(9))/*int*/;
      (count__71 = 0)/*int*/;
      for ((i__conflict__0__72 = 0)/*int*/; (i__conflict__0__72 < 16); (i__conflict__0__72++)) {{
          if ((__peek__14(i__conflict__0__72) < mine__70)) {(count__71 = (count__71 + 1))/*int*/;}
        }
      }
      for ((i__73 = 0)/*int*/; (i__73 < 16); (i__73++)) {{
          __pop__14();
        }
      }
      __push__14(count__71);
      // mark end: SIRFilter Counter

    }
  }
}

// peek: 16 pop: 16 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_15;
int __counter_15 = 0;
int __steady_15 = 0;
int __tmp_15 = 0;
int __tmp2_15 = 0;
int *__state_flag_15 = NULL;
thread_info *__thread_15 = NULL;



void save_peek_buffer__15(object_write_buffer *buf);
void load_peek_buffer__15(object_write_buffer *buf);
void save_file_pointer__15(object_write_buffer *buf);
void load_file_pointer__15(object_write_buffer *buf);

 
void init_Counter__81_32__15();
inline void check_status__15();

void work_Counter__81_32__15(int);


inline int __pop__15() {
int res=BUFFER_1_15[TAIL_1_15];
TAIL_1_15++;
return res;
}

inline int __pop__15(int n) {
int res=BUFFER_1_15[TAIL_1_15];
TAIL_1_15+=n;

return res;
}

inline int __peek__15(int offs) {
return BUFFER_1_15[TAIL_1_15+offs];
}



inline void __push__15(int data) {
BUFFER_15_3[HEAD_15_3]=data;
HEAD_15_3++;
}



 
void init_Counter__81_32__15(){
}
void save_file_pointer__15(object_write_buffer *buf) {}
void load_file_pointer__15(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Counter__81_32__15__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int mine__77 = 0;/* int */
  int count__78 = 0;/* int */
  int i__conflict__0__79 = 0;/* int */
  int i__80 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__77 = (*(____in+10)))/*int*/;
  (count__78 = 0)/*int*/;
  for ((i__conflict__0__79 = 0)/*int*/; (i__conflict__0__79 < 16); (i__conflict__0__79++)) {{
      if (((*(____in+i__conflict__0__79)) < mine__77)) {(count__78 = (count__78 + 1))/*int*/;}
    }
  }
  for ((i__80 = 0)/*int*/; (i__80 < 16); (i__80++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__78);
  // mark end: SIRFilter Counter

}}


void work_Counter__81_32__15__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int mine__77 = 0;/* int */
  int count__78 = 0;/* int */
  int i__conflict__0__79 = 0;/* int */
  int i__80 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__77 = (*(____in+10)))/*int*/;
  (count__78 = 0)/*int*/;
  for ((i__conflict__0__79 = 0)/*int*/; (i__conflict__0__79 < 16); (i__conflict__0__79++)) {{
      if (((*(____in+i__conflict__0__79)) < mine__77)) {(count__78 = (count__78 + 1))/*int*/;}
    }
  }
  for ((i__80 = 0)/*int*/; (i__80 < 16); (i__80++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__78);
  // mark end: SIRFilter Counter

}}


#endif // BUFFER_MERGE


 
void work_Counter__81_32__15(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int mine__77 = 0;/* int */
      int count__78 = 0;/* int */
      int i__conflict__0__79 = 0;/* int */
      int i__80 = 0;/* int */

      // mark begin: SIRFilter Counter

      (mine__77 = __peek__15(10))/*int*/;
      (count__78 = 0)/*int*/;
      for ((i__conflict__0__79 = 0)/*int*/; (i__conflict__0__79 < 16); (i__conflict__0__79++)) {{
          if ((__peek__15(i__conflict__0__79) < mine__77)) {(count__78 = (count__78 + 1))/*int*/;}
        }
      }
      for ((i__80 = 0)/*int*/; (i__80 < 16); (i__80++)) {{
          __pop__15();
        }
      }
      __push__15(count__78);
      // mark end: SIRFilter Counter

    }
  }
}

// peek: 16 pop: 16 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_16;
int __counter_16 = 0;
int __steady_16 = 0;
int __tmp_16 = 0;
int __tmp2_16 = 0;
int *__state_flag_16 = NULL;
thread_info *__thread_16 = NULL;



void save_peek_buffer__16(object_write_buffer *buf);
void load_peek_buffer__16(object_write_buffer *buf);
void save_file_pointer__16(object_write_buffer *buf);
void load_file_pointer__16(object_write_buffer *buf);

 
void init_Counter__88_33__16();
inline void check_status__16();

void work_Counter__88_33__16(int);


inline int __pop__16() {
int res=BUFFER_1_16[TAIL_1_16];
TAIL_1_16++;
return res;
}

inline int __pop__16(int n) {
int res=BUFFER_1_16[TAIL_1_16];
TAIL_1_16+=n;

return res;
}

inline int __peek__16(int offs) {
return BUFFER_1_16[TAIL_1_16+offs];
}



inline void __push__16(int data) {
BUFFER_16_3[HEAD_16_3]=data;
HEAD_16_3++;
}



 
void init_Counter__88_33__16(){
}
void save_file_pointer__16(object_write_buffer *buf) {}
void load_file_pointer__16(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Counter__88_33__16__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int mine__84 = 0;/* int */
  int count__85 = 0;/* int */
  int i__conflict__0__86 = 0;/* int */
  int i__87 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__84 = (*(____in+11)))/*int*/;
  (count__85 = 0)/*int*/;
  for ((i__conflict__0__86 = 0)/*int*/; (i__conflict__0__86 < 16); (i__conflict__0__86++)) {{
      if (((*(____in+i__conflict__0__86)) < mine__84)) {(count__85 = (count__85 + 1))/*int*/;}
    }
  }
  for ((i__87 = 0)/*int*/; (i__87 < 16); (i__87++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__85);
  // mark end: SIRFilter Counter

}}


void work_Counter__88_33__16__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int mine__84 = 0;/* int */
  int count__85 = 0;/* int */
  int i__conflict__0__86 = 0;/* int */
  int i__87 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__84 = (*(____in+11)))/*int*/;
  (count__85 = 0)/*int*/;
  for ((i__conflict__0__86 = 0)/*int*/; (i__conflict__0__86 < 16); (i__conflict__0__86++)) {{
      if (((*(____in+i__conflict__0__86)) < mine__84)) {(count__85 = (count__85 + 1))/*int*/;}
    }
  }
  for ((i__87 = 0)/*int*/; (i__87 < 16); (i__87++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__85);
  // mark end: SIRFilter Counter

}}


#endif // BUFFER_MERGE


 
void work_Counter__88_33__16(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int mine__84 = 0;/* int */
      int count__85 = 0;/* int */
      int i__conflict__0__86 = 0;/* int */
      int i__87 = 0;/* int */

      // mark begin: SIRFilter Counter

      (mine__84 = __peek__16(11))/*int*/;
      (count__85 = 0)/*int*/;
      for ((i__conflict__0__86 = 0)/*int*/; (i__conflict__0__86 < 16); (i__conflict__0__86++)) {{
          if ((__peek__16(i__conflict__0__86) < mine__84)) {(count__85 = (count__85 + 1))/*int*/;}
        }
      }
      for ((i__87 = 0)/*int*/; (i__87 < 16); (i__87++)) {{
          __pop__16();
        }
      }
      __push__16(count__85);
      // mark end: SIRFilter Counter

    }
  }
}

// peek: 16 pop: 16 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_17;
int __counter_17 = 0;
int __steady_17 = 0;
int __tmp_17 = 0;
int __tmp2_17 = 0;
int *__state_flag_17 = NULL;
thread_info *__thread_17 = NULL;



void save_peek_buffer__17(object_write_buffer *buf);
void load_peek_buffer__17(object_write_buffer *buf);
void save_file_pointer__17(object_write_buffer *buf);
void load_file_pointer__17(object_write_buffer *buf);

 
void init_Counter__95_34__17();
inline void check_status__17();

void work_Counter__95_34__17(int);


inline int __pop__17() {
int res=BUFFER_1_17[TAIL_1_17];
TAIL_1_17++;
return res;
}

inline int __pop__17(int n) {
int res=BUFFER_1_17[TAIL_1_17];
TAIL_1_17+=n;

return res;
}

inline int __peek__17(int offs) {
return BUFFER_1_17[TAIL_1_17+offs];
}



inline void __push__17(int data) {
BUFFER_17_3[HEAD_17_3]=data;
HEAD_17_3++;
}



 
void init_Counter__95_34__17(){
}
void save_file_pointer__17(object_write_buffer *buf) {}
void load_file_pointer__17(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Counter__95_34__17__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int mine__91 = 0;/* int */
  int count__92 = 0;/* int */
  int i__conflict__0__93 = 0;/* int */
  int i__94 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__91 = (*(____in+12)))/*int*/;
  (count__92 = 0)/*int*/;
  for ((i__conflict__0__93 = 0)/*int*/; (i__conflict__0__93 < 16); (i__conflict__0__93++)) {{
      if (((*(____in+i__conflict__0__93)) < mine__91)) {(count__92 = (count__92 + 1))/*int*/;}
    }
  }
  for ((i__94 = 0)/*int*/; (i__94 < 16); (i__94++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__92);
  // mark end: SIRFilter Counter

}}


void work_Counter__95_34__17__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int mine__91 = 0;/* int */
  int count__92 = 0;/* int */
  int i__conflict__0__93 = 0;/* int */
  int i__94 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__91 = (*(____in+12)))/*int*/;
  (count__92 = 0)/*int*/;
  for ((i__conflict__0__93 = 0)/*int*/; (i__conflict__0__93 < 16); (i__conflict__0__93++)) {{
      if (((*(____in+i__conflict__0__93)) < mine__91)) {(count__92 = (count__92 + 1))/*int*/;}
    }
  }
  for ((i__94 = 0)/*int*/; (i__94 < 16); (i__94++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__92);
  // mark end: SIRFilter Counter

}}


#endif // BUFFER_MERGE


 
void work_Counter__95_34__17(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int mine__91 = 0;/* int */
      int count__92 = 0;/* int */
      int i__conflict__0__93 = 0;/* int */
      int i__94 = 0;/* int */

      // mark begin: SIRFilter Counter

      (mine__91 = __peek__17(12))/*int*/;
      (count__92 = 0)/*int*/;
      for ((i__conflict__0__93 = 0)/*int*/; (i__conflict__0__93 < 16); (i__conflict__0__93++)) {{
          if ((__peek__17(i__conflict__0__93) < mine__91)) {(count__92 = (count__92 + 1))/*int*/;}
        }
      }
      for ((i__94 = 0)/*int*/; (i__94 < 16); (i__94++)) {{
          __pop__17();
        }
      }
      __push__17(count__92);
      // mark end: SIRFilter Counter

    }
  }
}

// peek: 16 pop: 16 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_18;
int __counter_18 = 0;
int __steady_18 = 0;
int __tmp_18 = 0;
int __tmp2_18 = 0;
int *__state_flag_18 = NULL;
thread_info *__thread_18 = NULL;



void save_peek_buffer__18(object_write_buffer *buf);
void load_peek_buffer__18(object_write_buffer *buf);
void save_file_pointer__18(object_write_buffer *buf);
void load_file_pointer__18(object_write_buffer *buf);

 
void init_Counter__102_35__18();
inline void check_status__18();

void work_Counter__102_35__18(int);


inline int __pop__18() {
int res=BUFFER_1_18[TAIL_1_18];
TAIL_1_18++;
return res;
}

inline int __pop__18(int n) {
int res=BUFFER_1_18[TAIL_1_18];
TAIL_1_18+=n;

return res;
}

inline int __peek__18(int offs) {
return BUFFER_1_18[TAIL_1_18+offs];
}



inline void __push__18(int data) {
BUFFER_18_3[HEAD_18_3]=data;
HEAD_18_3++;
}



 
void init_Counter__102_35__18(){
}
void save_file_pointer__18(object_write_buffer *buf) {}
void load_file_pointer__18(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Counter__102_35__18__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int mine__98 = 0;/* int */
  int count__99 = 0;/* int */
  int i__conflict__0__100 = 0;/* int */
  int i__101 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__98 = (*(____in+13)))/*int*/;
  (count__99 = 0)/*int*/;
  for ((i__conflict__0__100 = 0)/*int*/; (i__conflict__0__100 < 16); (i__conflict__0__100++)) {{
      if (((*(____in+i__conflict__0__100)) < mine__98)) {(count__99 = (count__99 + 1))/*int*/;}
    }
  }
  for ((i__101 = 0)/*int*/; (i__101 < 16); (i__101++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__99);
  // mark end: SIRFilter Counter

}}


void work_Counter__102_35__18__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int mine__98 = 0;/* int */
  int count__99 = 0;/* int */
  int i__conflict__0__100 = 0;/* int */
  int i__101 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__98 = (*(____in+13)))/*int*/;
  (count__99 = 0)/*int*/;
  for ((i__conflict__0__100 = 0)/*int*/; (i__conflict__0__100 < 16); (i__conflict__0__100++)) {{
      if (((*(____in+i__conflict__0__100)) < mine__98)) {(count__99 = (count__99 + 1))/*int*/;}
    }
  }
  for ((i__101 = 0)/*int*/; (i__101 < 16); (i__101++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__99);
  // mark end: SIRFilter Counter

}}


#endif // BUFFER_MERGE


 
void work_Counter__102_35__18(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int mine__98 = 0;/* int */
      int count__99 = 0;/* int */
      int i__conflict__0__100 = 0;/* int */
      int i__101 = 0;/* int */

      // mark begin: SIRFilter Counter

      (mine__98 = __peek__18(13))/*int*/;
      (count__99 = 0)/*int*/;
      for ((i__conflict__0__100 = 0)/*int*/; (i__conflict__0__100 < 16); (i__conflict__0__100++)) {{
          if ((__peek__18(i__conflict__0__100) < mine__98)) {(count__99 = (count__99 + 1))/*int*/;}
        }
      }
      for ((i__101 = 0)/*int*/; (i__101 < 16); (i__101++)) {{
          __pop__18();
        }
      }
      __push__18(count__99);
      // mark end: SIRFilter Counter

    }
  }
}

// peek: 16 pop: 16 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_19;
int __counter_19 = 0;
int __steady_19 = 0;
int __tmp_19 = 0;
int __tmp2_19 = 0;
int *__state_flag_19 = NULL;
thread_info *__thread_19 = NULL;



void save_peek_buffer__19(object_write_buffer *buf);
void load_peek_buffer__19(object_write_buffer *buf);
void save_file_pointer__19(object_write_buffer *buf);
void load_file_pointer__19(object_write_buffer *buf);

 
void init_Counter__109_36__19();
inline void check_status__19();

void work_Counter__109_36__19(int);


inline int __pop__19() {
int res=BUFFER_1_19[TAIL_1_19];
TAIL_1_19++;
return res;
}

inline int __pop__19(int n) {
int res=BUFFER_1_19[TAIL_1_19];
TAIL_1_19+=n;

return res;
}

inline int __peek__19(int offs) {
return BUFFER_1_19[TAIL_1_19+offs];
}



inline void __push__19(int data) {
BUFFER_19_3[HEAD_19_3]=data;
HEAD_19_3++;
}



 
void init_Counter__109_36__19(){
}
void save_file_pointer__19(object_write_buffer *buf) {}
void load_file_pointer__19(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Counter__109_36__19__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int mine__105 = 0;/* int */
  int count__106 = 0;/* int */
  int i__conflict__0__107 = 0;/* int */
  int i__108 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__105 = (*(____in+14)))/*int*/;
  (count__106 = 0)/*int*/;
  for ((i__conflict__0__107 = 0)/*int*/; (i__conflict__0__107 < 16); (i__conflict__0__107++)) {{
      if (((*(____in+i__conflict__0__107)) < mine__105)) {(count__106 = (count__106 + 1))/*int*/;}
    }
  }
  for ((i__108 = 0)/*int*/; (i__108 < 16); (i__108++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__106);
  // mark end: SIRFilter Counter

}}


void work_Counter__109_36__19__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int mine__105 = 0;/* int */
  int count__106 = 0;/* int */
  int i__conflict__0__107 = 0;/* int */
  int i__108 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__105 = (*(____in+14)))/*int*/;
  (count__106 = 0)/*int*/;
  for ((i__conflict__0__107 = 0)/*int*/; (i__conflict__0__107 < 16); (i__conflict__0__107++)) {{
      if (((*(____in+i__conflict__0__107)) < mine__105)) {(count__106 = (count__106 + 1))/*int*/;}
    }
  }
  for ((i__108 = 0)/*int*/; (i__108 < 16); (i__108++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__106);
  // mark end: SIRFilter Counter

}}


#endif // BUFFER_MERGE


 
void work_Counter__109_36__19(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int mine__105 = 0;/* int */
      int count__106 = 0;/* int */
      int i__conflict__0__107 = 0;/* int */
      int i__108 = 0;/* int */

      // mark begin: SIRFilter Counter

      (mine__105 = __peek__19(14))/*int*/;
      (count__106 = 0)/*int*/;
      for ((i__conflict__0__107 = 0)/*int*/; (i__conflict__0__107 < 16); (i__conflict__0__107++)) {{
          if ((__peek__19(i__conflict__0__107) < mine__105)) {(count__106 = (count__106 + 1))/*int*/;}
        }
      }
      for ((i__108 = 0)/*int*/; (i__108 < 16); (i__108++)) {{
          __pop__19();
        }
      }
      __push__19(count__106);
      // mark end: SIRFilter Counter

    }
  }
}

// peek: 16 pop: 16 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_20;
int __counter_20 = 0;
int __steady_20 = 0;
int __tmp_20 = 0;
int __tmp2_20 = 0;
int *__state_flag_20 = NULL;
thread_info *__thread_20 = NULL;



void save_peek_buffer__20(object_write_buffer *buf);
void load_peek_buffer__20(object_write_buffer *buf);
void save_file_pointer__20(object_write_buffer *buf);
void load_file_pointer__20(object_write_buffer *buf);

 
void init_Counter__116_37__20();
inline void check_status__20();

void work_Counter__116_37__20(int);


inline int __pop__20() {
int res=BUFFER_1_20[TAIL_1_20];
TAIL_1_20++;
return res;
}

inline int __pop__20(int n) {
int res=BUFFER_1_20[TAIL_1_20];
TAIL_1_20+=n;

return res;
}

inline int __peek__20(int offs) {
return BUFFER_1_20[TAIL_1_20+offs];
}



inline void __push__20(int data) {
BUFFER_20_3[HEAD_20_3]=data;
HEAD_20_3++;
}



 
void init_Counter__116_37__20(){
}
void save_file_pointer__20(object_write_buffer *buf) {}
void load_file_pointer__20(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Counter__116_37__20__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  int mine__112 = 0;/* int */
  int count__113 = 0;/* int */
  int i__conflict__0__114 = 0;/* int */
  int i__115 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__112 = (*(____in+15)))/*int*/;
  (count__113 = 0)/*int*/;
  for ((i__conflict__0__114 = 0)/*int*/; (i__conflict__0__114 < 16); (i__conflict__0__114++)) {{
      if (((*(____in+i__conflict__0__114)) < mine__112)) {(count__113 = (count__113 + 1))/*int*/;}
    }
  }
  for ((i__115 = 0)/*int*/; (i__115 < 16); (i__115++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__113);
  // mark end: SIRFilter Counter

}}


void work_Counter__116_37__20__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int mine__112 = 0;/* int */
  int count__113 = 0;/* int */
  int i__conflict__0__114 = 0;/* int */
  int i__115 = 0;/* int */

  // mark begin: SIRFilter Counter

  (mine__112 = (*(____in+15)))/*int*/;
  (count__113 = 0)/*int*/;
  for ((i__conflict__0__114 = 0)/*int*/; (i__conflict__0__114 < 16); (i__conflict__0__114++)) {{
      if (((*(____in+i__conflict__0__114)) < mine__112)) {(count__113 = (count__113 + 1))/*int*/;}
    }
  }
  for ((i__115 = 0)/*int*/; (i__115 < 16); (i__115++)) {{
      (*____in++);
    }
  }
  ((*____out++)=count__113);
  // mark end: SIRFilter Counter

}}


#endif // BUFFER_MERGE


 
void work_Counter__116_37__20(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int mine__112 = 0;/* int */
      int count__113 = 0;/* int */
      int i__conflict__0__114 = 0;/* int */
      int i__115 = 0;/* int */

      // mark begin: SIRFilter Counter

      (mine__112 = __peek__20(15))/*int*/;
      (count__113 = 0)/*int*/;
      for ((i__conflict__0__114 = 0)/*int*/; (i__conflict__0__114 < 16); (i__conflict__0__114++)) {{
          if ((__peek__20(i__conflict__0__114) < mine__112)) {(count__113 = (count__113 + 1))/*int*/;}
        }
      }
      for ((i__115 = 0)/*int*/; (i__115 < 16); (i__115++)) {{
          __pop__20();
        }
      }
      __push__20(count__113);
      // mark end: SIRFilter Counter

    }
  }
}

// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 16

// ClusterFusion isEliminated: false



int __number_of_iterations_21;
int __counter_21 = 0;
int __steady_21 = 0;
int __tmp_21 = 0;
int __tmp2_21 = 0;
int *__state_flag_21 = NULL;
thread_info *__thread_21 = NULL;



void save_peek_buffer__21(object_write_buffer *buf);
void load_peek_buffer__21(object_write_buffer *buf);
void save_file_pointer__21(object_write_buffer *buf);
void load_file_pointer__21(object_write_buffer *buf);

 
void init_Identity__119_38__21();
inline void check_status__21();

void Identity__119_38__work__21(int);


inline int __pop__21() {
int res=BUFFER_1_21[TAIL_1_21];
TAIL_1_21++;
return res;
}

inline int __pop__21(int n) {
int res=BUFFER_1_21[TAIL_1_21];
TAIL_1_21+=n;

return res;
}

inline int __peek__21(int offs) {
return BUFFER_1_21[TAIL_1_21+offs];
}



inline void __push__21(int data) {
BUFFER_21_3[HEAD_21_3]=data;
HEAD_21_3++;
}



void init_Identity__119_38__21() {
}

void save_file_pointer__21(object_write_buffer *buf) {}
void load_file_pointer__21(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Identity__119_38__21__mod(int ____n, int *____in, int *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRIdentity Identity

  ((*____out++)=(*____in++));
  // mark end: SIRIdentity Identity

}}


void work_Identity__119_38__21__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRIdentity Identity

  ((*____out++)=(*____in++));
  // mark end: SIRIdentity Identity

}}


#endif // BUFFER_MERGE


void Identity__119_38__work__21(int ____n) {
    for (; 0 < ____n; ____n--) {
        __push__21(__pop__21());
    }
}


