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
extern void init_IntSource__4_8__0();
extern void work_IntSource__4_8__0(int);
#ifdef BUFFER_MERGE
extern void work_IntSource__4_8__0__mod(int ____n, void *____in, int *____out);
extern void work_IntSource__4_8__0__mod2(int ____n, void *____in, int *____out, int s1, int s2);
#endif
extern void init_Sort__12_9__1();
extern void work_Sort__12_9__1(int);
#ifdef BUFFER_MERGE
extern void work_Sort__12_9__1__mod(int ____n, int *____in, int *____out);
extern void work_Sort__12_9__1__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Sort__20_10__2();
extern void work_Sort__20_10__2(int);
#ifdef BUFFER_MERGE
extern void work_Sort__20_10__2__mod(int ____n, int *____in, int *____out);
extern void work_Sort__20_10__2__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Sort__28_11__3();
extern void work_Sort__28_11__3(int);
#ifdef BUFFER_MERGE
extern void work_Sort__28_11__3__mod(int ____n, int *____in, int *____out);
extern void work_Sort__28_11__3__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Sort__36_12__4();
extern void work_Sort__36_12__4(int);
#ifdef BUFFER_MERGE
extern void work_Sort__36_12__4__mod(int ____n, int *____in, int *____out);
extern void work_Sort__36_12__4__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Sort__44_13__5();
extern void work_Sort__44_13__5(int);
#ifdef BUFFER_MERGE
extern void work_Sort__44_13__5__mod(int ____n, int *____in, int *____out);
extern void work_Sort__44_13__5__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Sort__52_14__6();
extern void work_Sort__52_14__6(int);
#ifdef BUFFER_MERGE
extern void work_Sort__52_14__6__mod(int ____n, int *____in, int *____out);
extern void work_Sort__52_14__6__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Sort__60_15__7();
extern void work_Sort__60_15__7(int);
#ifdef BUFFER_MERGE
extern void work_Sort__60_15__7__mod(int ____n, int *____in, int *____out);
extern void work_Sort__60_15__7__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Sort__68_16__8();
extern void work_Sort__68_16__8(int);
#ifdef BUFFER_MERGE
extern void work_Sort__68_16__8__mod(int ____n, int *____in, int *____out);
extern void work_Sort__68_16__8__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Sort__76_17__9();
extern void work_Sort__76_17__9(int);
#ifdef BUFFER_MERGE
extern void work_Sort__76_17__9__mod(int ____n, int *____in, int *____out);
extern void work_Sort__76_17__9__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Sort__84_18__10();
extern void work_Sort__84_18__10(int);
#ifdef BUFFER_MERGE
extern void work_Sort__84_18__10__mod(int ____n, int *____in, int *____out);
extern void work_Sort__84_18__10__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Sort__92_19__11();
extern void work_Sort__92_19__11(int);
#ifdef BUFFER_MERGE
extern void work_Sort__92_19__11__mod(int ____n, int *____in, int *____out);
extern void work_Sort__92_19__11__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_IntPrinter__96_20__12();
extern void work_IntPrinter__96_20__12(int);
#ifdef BUFFER_MERGE
extern void work_IntPrinter__96_20__12__mod(int ____n, int *____in, void *____out);
extern void work_IntPrinter__96_20__12__mod2(int ____n, int *____in, void *____out, int s1, int s2);
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

  // ============= Steady State =============

  if (__timer_enabled) {
    tt.start();
  }
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
if (__timer_enabled) {
    tt.stop();
    tt.output(stderr);
  }


  return 0;
}
