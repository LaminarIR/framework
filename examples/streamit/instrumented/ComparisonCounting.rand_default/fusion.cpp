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
extern void init_IntSource__4_20__0();
extern void work_IntSource__4_20__0(int);
#ifdef BUFFER_MERGE
extern void work_IntSource__4_20__0__mod(int ____n, void *____in, int *____out);
extern void work_IntSource__4_20__0__mod2(int ____n, void *____in, int *____out, int s1, int s2);
#endif
extern void __splitter_1_work(int);
extern void init_Counter__11_22__2();
extern void work_Counter__11_22__2(int);
#ifdef BUFFER_MERGE
extern void work_Counter__11_22__2__mod(int ____n, int *____in, int *____out);
extern void work_Counter__11_22__2__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void __joiner_3_work(int);
extern void init_RearrangeIndices__125_39__4();
extern void work_RearrangeIndices__125_39__4(int);
#ifdef BUFFER_MERGE
extern void work_RearrangeIndices__125_39__4__mod(int ____n, int *____in, int *____out);
extern void work_RearrangeIndices__125_39__4__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_IntPrinter__129_40__5();
extern void work_IntPrinter__129_40__5(int);
#ifdef BUFFER_MERGE
extern void work_IntPrinter__129_40__5__mod(int ____n, int *____in, void *____out);
extern void work_IntPrinter__129_40__5__mod2(int ____n, int *____in, void *____out, int s1, int s2);
#endif
extern void init_Counter__18_23__6();
extern void work_Counter__18_23__6(int);
#ifdef BUFFER_MERGE
extern void work_Counter__18_23__6__mod(int ____n, int *____in, int *____out);
extern void work_Counter__18_23__6__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Counter__25_24__7();
extern void work_Counter__25_24__7(int);
#ifdef BUFFER_MERGE
extern void work_Counter__25_24__7__mod(int ____n, int *____in, int *____out);
extern void work_Counter__25_24__7__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Counter__32_25__8();
extern void work_Counter__32_25__8(int);
#ifdef BUFFER_MERGE
extern void work_Counter__32_25__8__mod(int ____n, int *____in, int *____out);
extern void work_Counter__32_25__8__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Counter__39_26__9();
extern void work_Counter__39_26__9(int);
#ifdef BUFFER_MERGE
extern void work_Counter__39_26__9__mod(int ____n, int *____in, int *____out);
extern void work_Counter__39_26__9__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Counter__46_27__10();
extern void work_Counter__46_27__10(int);
#ifdef BUFFER_MERGE
extern void work_Counter__46_27__10__mod(int ____n, int *____in, int *____out);
extern void work_Counter__46_27__10__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Counter__53_28__11();
extern void work_Counter__53_28__11(int);
#ifdef BUFFER_MERGE
extern void work_Counter__53_28__11__mod(int ____n, int *____in, int *____out);
extern void work_Counter__53_28__11__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Counter__60_29__12();
extern void work_Counter__60_29__12(int);
#ifdef BUFFER_MERGE
extern void work_Counter__60_29__12__mod(int ____n, int *____in, int *____out);
extern void work_Counter__60_29__12__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Counter__67_30__13();
extern void work_Counter__67_30__13(int);
#ifdef BUFFER_MERGE
extern void work_Counter__67_30__13__mod(int ____n, int *____in, int *____out);
extern void work_Counter__67_30__13__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Counter__74_31__14();
extern void work_Counter__74_31__14(int);
#ifdef BUFFER_MERGE
extern void work_Counter__74_31__14__mod(int ____n, int *____in, int *____out);
extern void work_Counter__74_31__14__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Counter__81_32__15();
extern void work_Counter__81_32__15(int);
#ifdef BUFFER_MERGE
extern void work_Counter__81_32__15__mod(int ____n, int *____in, int *____out);
extern void work_Counter__81_32__15__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Counter__88_33__16();
extern void work_Counter__88_33__16(int);
#ifdef BUFFER_MERGE
extern void work_Counter__88_33__16__mod(int ____n, int *____in, int *____out);
extern void work_Counter__88_33__16__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Counter__95_34__17();
extern void work_Counter__95_34__17(int);
#ifdef BUFFER_MERGE
extern void work_Counter__95_34__17__mod(int ____n, int *____in, int *____out);
extern void work_Counter__95_34__17__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Counter__102_35__18();
extern void work_Counter__102_35__18(int);
#ifdef BUFFER_MERGE
extern void work_Counter__102_35__18__mod(int ____n, int *____in, int *____out);
extern void work_Counter__102_35__18__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Counter__109_36__19();
extern void work_Counter__109_36__19(int);
#ifdef BUFFER_MERGE
extern void work_Counter__109_36__19__mod(int ____n, int *____in, int *____out);
extern void work_Counter__109_36__19__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Counter__116_37__20();
extern void work_Counter__116_37__20(int);
#ifdef BUFFER_MERGE
extern void work_Counter__116_37__20__mod(int ____n, int *____in, int *____out);
extern void work_Counter__116_37__20__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Identity__119_38__21();
extern void Identity__119_38__work__21(int);
#ifdef BUFFER_MERGE
extern void Identity__119_38__work__21__mod(int ____n, int *____in, int *____out);
extern void Identity__119_38__work__21__mod2(int ____n, int *____in, int *____out, int s1, int s2);
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

  // ============= Steady State =============

  if (__timer_enabled) {
    tt.start();
  }
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
if (__timer_enabled) {
    tt.stop();
    tt.output(stderr);
  }


  return 0;
}
