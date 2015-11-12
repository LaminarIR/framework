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
extern void init_FFTTestSource__3_31__0();
extern void work_FFTTestSource__3_31__0(int);
#ifdef BUFFER_MERGE
extern void work_FFTTestSource__3_31__0__mod(int ____n, void *____in, float *____out);
extern void work_FFTTestSource__3_31__0__mod2(int ____n, void *____in, float *____out, int s1, int s2);
#endif
extern void __splitter_1_work(int);
extern void init_FFTReorderSimple__7_32__2();
extern void work_FFTReorderSimple__7_32__2(int);
#ifdef BUFFER_MERGE
extern void work_FFTReorderSimple__7_32__2__mod(int ____n, float *____in, float *____out);
extern void work_FFTReorderSimple__7_32__2__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_FFTReorderSimple__11_33__3();
extern void work_FFTReorderSimple__11_33__3(int);
#ifdef BUFFER_MERGE
extern void work_FFTReorderSimple__11_33__3__mod(int ____n, float *____in, float *____out);
extern void work_FFTReorderSimple__11_33__3__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_FFTReorderSimple__15_34__4();
extern void work_FFTReorderSimple__15_34__4(int);
#ifdef BUFFER_MERGE
extern void work_FFTReorderSimple__15_34__4__mod(int ____n, float *____in, float *____out);
extern void work_FFTReorderSimple__15_34__4__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_CombineDFT__35_35__5();
extern void work_CombineDFT__35_35__5(int);
#ifdef BUFFER_MERGE
extern void work_CombineDFT__35_35__5__mod(int ____n, float *____in, float *____out);
extern void work_CombineDFT__35_35__5__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_CombineDFT__55_36__6();
extern void work_CombineDFT__55_36__6(int);
#ifdef BUFFER_MERGE
extern void work_CombineDFT__55_36__6__mod(int ____n, float *____in, float *____out);
extern void work_CombineDFT__55_36__6__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_CombineDFT__75_37__7();
extern void work_CombineDFT__75_37__7(int);
#ifdef BUFFER_MERGE
extern void work_CombineDFT__75_37__7__mod(int ____n, float *____in, float *____out);
extern void work_CombineDFT__75_37__7__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_CombineDFT__95_38__8();
extern void work_CombineDFT__95_38__8(int);
#ifdef BUFFER_MERGE
extern void work_CombineDFT__95_38__8__mod(int ____n, float *____in, float *____out);
extern void work_CombineDFT__95_38__8__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void __joiner_9_work(int);
extern void init_FloatPrinter__191_46__10();
extern void work_FloatPrinter__191_46__10(int);
#ifdef BUFFER_MERGE
extern void work_FloatPrinter__191_46__10__mod(int ____n, float *____in, void *____out);
extern void work_FloatPrinter__191_46__10__mod2(int ____n, float *____in, void *____out, int s1, int s2);
#endif
extern void init_FFTReorderSimple__99_39__11();
extern void work_FFTReorderSimple__99_39__11(int);
#ifdef BUFFER_MERGE
extern void work_FFTReorderSimple__99_39__11__mod(int ____n, float *____in, float *____out);
extern void work_FFTReorderSimple__99_39__11__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_FFTReorderSimple__103_40__12();
extern void work_FFTReorderSimple__103_40__12(int);
#ifdef BUFFER_MERGE
extern void work_FFTReorderSimple__103_40__12__mod(int ____n, float *____in, float *____out);
extern void work_FFTReorderSimple__103_40__12__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_FFTReorderSimple__107_41__13();
extern void work_FFTReorderSimple__107_41__13(int);
#ifdef BUFFER_MERGE
extern void work_FFTReorderSimple__107_41__13__mod(int ____n, float *____in, float *____out);
extern void work_FFTReorderSimple__107_41__13__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_CombineDFT__127_42__14();
extern void work_CombineDFT__127_42__14(int);
#ifdef BUFFER_MERGE
extern void work_CombineDFT__127_42__14__mod(int ____n, float *____in, float *____out);
extern void work_CombineDFT__127_42__14__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_CombineDFT__147_43__15();
extern void work_CombineDFT__147_43__15(int);
#ifdef BUFFER_MERGE
extern void work_CombineDFT__147_43__15__mod(int ____n, float *____in, float *____out);
extern void work_CombineDFT__147_43__15__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_CombineDFT__167_44__16();
extern void work_CombineDFT__167_44__16(int);
#ifdef BUFFER_MERGE
extern void work_CombineDFT__167_44__16__mod(int ____n, float *____in, float *____out);
extern void work_CombineDFT__167_44__16__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_CombineDFT__187_45__17();
extern void work_CombineDFT__187_45__17(int);
#ifdef BUFFER_MERGE
extern void work_CombineDFT__187_45__17__mod(int ____n, float *____in, float *____out);
extern void work_CombineDFT__187_45__17__mod2(int ____n, float *____in, float *____out, int s1, int s2);
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

  // ============= Steady State =============

  if (__timer_enabled) {
    tt.start();
  }
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
if (__timer_enabled) {
    tt.stop();
    tt.output(stderr);
  }


  return 0;
}
