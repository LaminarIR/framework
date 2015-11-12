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
float BUFFER_1_2[__BUF_SIZE_MASK_1_2 + 1];
int HEAD_1_2 = 0;
int TAIL_1_2 = 0;
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
int BUFFER_7_8[__BUF_SIZE_MASK_7_8 + 1];
int HEAD_7_8 = 0;
int TAIL_7_8 = 0;
int BUFFER_8_9[__BUF_SIZE_MASK_8_9 + 1];
int HEAD_8_9 = 0;
int TAIL_8_9 = 0;
int BUFFER_8_35[__BUF_SIZE_MASK_8_35 + 1];
int HEAD_8_35 = 0;
int TAIL_8_35 = 0;
int BUFFER_9_10[__BUF_SIZE_MASK_9_10 + 1];
int HEAD_9_10 = 0;
int TAIL_9_10 = 0;
int BUFFER_10_11[__BUF_SIZE_MASK_10_11 + 1];
int HEAD_10_11 = 0;
int TAIL_10_11 = 0;
int BUFFER_11_12[__BUF_SIZE_MASK_11_12 + 1];
int HEAD_11_12 = 0;
int TAIL_11_12 = 0;
int BUFFER_11_34[__BUF_SIZE_MASK_11_34 + 1];
int HEAD_11_34 = 0;
int TAIL_11_34 = 0;
int BUFFER_12_13[__BUF_SIZE_MASK_12_13 + 1];
int HEAD_12_13 = 0;
int TAIL_12_13 = 0;
int BUFFER_13_14[__BUF_SIZE_MASK_13_14 + 1];
int HEAD_13_14 = 0;
int TAIL_13_14 = 0;
int BUFFER_14_15[__BUF_SIZE_MASK_14_15 + 1];
int HEAD_14_15 = 0;
int TAIL_14_15 = 0;
int BUFFER_15_16[__BUF_SIZE_MASK_15_16 + 1];
int HEAD_15_16 = 0;
int TAIL_15_16 = 0;
int BUFFER_16_17[__BUF_SIZE_MASK_16_17 + 1];
int HEAD_16_17 = 0;
int TAIL_16_17 = 0;
int BUFFER_17_18[__BUF_SIZE_MASK_17_18 + 1];
int HEAD_17_18 = 0;
int TAIL_17_18 = 0;
int BUFFER_18_19[__BUF_SIZE_MASK_18_19 + 1];
int HEAD_18_19 = 0;
int TAIL_18_19 = 0;
int BUFFER_18_33[__BUF_SIZE_MASK_18_33 + 1];
int HEAD_18_33 = 0;
int TAIL_18_33 = 0;
int BUFFER_19_20[__BUF_SIZE_MASK_19_20 + 1];
int HEAD_19_20 = 0;
int TAIL_19_20 = 0;
int BUFFER_20_21[__BUF_SIZE_MASK_20_21 + 1];
int HEAD_20_21 = 0;
int TAIL_20_21 = 0;
int BUFFER_21_22[__BUF_SIZE_MASK_21_22 + 1];
int HEAD_21_22 = 0;
int TAIL_21_22 = 0;
int BUFFER_21_32[__BUF_SIZE_MASK_21_32 + 1];
int HEAD_21_32 = 0;
int TAIL_21_32 = 0;
int BUFFER_22_23[__BUF_SIZE_MASK_22_23 + 1];
int HEAD_22_23 = 0;
int TAIL_22_23 = 0;
int BUFFER_23_24[__BUF_SIZE_MASK_23_24 + 1];
int HEAD_23_24 = 0;
int TAIL_23_24 = 0;
float BUFFER_24_25[__BUF_SIZE_MASK_24_25 + 1];
int HEAD_24_25 = 0;
int TAIL_24_25 = 0;
float BUFFER_25_26[__BUF_SIZE_MASK_25_26 + 1];
int HEAD_25_26 = 0;
int TAIL_25_26 = 0;
float BUFFER_26_27[__BUF_SIZE_MASK_26_27 + 1];
int HEAD_26_27 = 0;
int TAIL_26_27 = 0;
float BUFFER_27_28[__BUF_SIZE_MASK_27_28 + 1];
int HEAD_27_28 = 0;
int TAIL_27_28 = 0;
float BUFFER_28_29[__BUF_SIZE_MASK_28_29 + 1];
int HEAD_28_29 = 0;
int TAIL_28_29 = 0;
float BUFFER_29_30[__BUF_SIZE_MASK_29_30 + 1];
int HEAD_29_30 = 0;
int TAIL_29_30 = 0;
int BUFFER_30_31[__BUF_SIZE_MASK_30_31 + 1];
int HEAD_30_31 = 0;
int TAIL_30_31 = 0;
int BUFFER_32_19[__BUF_SIZE_MASK_32_19 + 1];
int HEAD_32_19 = 0;
int TAIL_32_19 = 0;
int BUFFER_33_23[__BUF_SIZE_MASK_33_23 + 1];
int HEAD_33_23 = 0;
int TAIL_33_23 = 0;
int BUFFER_34_9[__BUF_SIZE_MASK_34_9 + 1];
int HEAD_34_9 = 0;
int TAIL_34_9 = 0;
int BUFFER_35_13[__BUF_SIZE_MASK_35_13 + 1];
int HEAD_35_13 = 0;
int TAIL_35_13 = 0;
extern void __feedbackjoiner_9_prep();
extern void __feedbackjoiner_19_prep();
extern void init_ACMSample8BitBlock__196_110__0();
extern void work_ACMSample8BitBlock__196_110__0(int);
#ifdef BUFFER_MERGE
extern void work_ACMSample8BitBlock__196_110__0__mod(int ____n, void *____in, int *____out);
extern void work_ACMSample8BitBlock__196_110__0__mod2(int ____n, void *____in, int *____out, int s1, int s2);
#endif
extern void init_Int2Float__199_111__1();
extern void work_Int2Float__199_111__1(int);
#ifdef BUFFER_MERGE
extern void work_Int2Float__199_111__1__mod(int ____n, int *____in, float *____out);
extern void work_Int2Float__199_111__1__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
extern void init_Add__202_112__2();
extern void work_Add__202_112__2(int);
#ifdef BUFFER_MERGE
extern void work_Add__202_112__2__mod(int ____n, float *____in, float *____out);
extern void work_Add__202_112__2__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_DCT_1D__210_114__3();
extern void work_DCT_1D__210_114__3(int);
#ifdef BUFFER_MERGE
extern void work_DCT_1D__210_114__3__mod(int ____n, float *____in, float *____out);
extern void work_DCT_1D__210_114__3__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_Pre_CollapsedDataParallel_1__216_260__4();
extern void work_Pre_CollapsedDataParallel_1__216_260__4(int);
#ifdef BUFFER_MERGE
extern void work_Pre_CollapsedDataParallel_1__216_260__4__mod(int ____n, float *____in, float *____out);
extern void work_Pre_CollapsedDataParallel_1__216_260__4__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_DCT_1D__224_122__5();
extern void work_DCT_1D__224_122__5(int);
#ifdef BUFFER_MERGE
extern void work_DCT_1D__224_122__5__mod(int ____n, float *____in, float *____out);
extern void work_DCT_1D__224_122__5__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_Post_CollapsedDataParallel_2__230_261__6();
extern void work_Post_CollapsedDataParallel_2__230_261__6(int);
#ifdef BUFFER_MERGE
extern void work_Post_CollapsedDataParallel_2__230_261__6__mod(int ____n, float *____in, float *____out);
extern void work_Post_CollapsedDataParallel_2__230_261__6__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_staticQuantization__237_131__7();
extern void work_staticQuantization__237_131__7(int);
#ifdef BUFFER_MERGE
extern void work_staticQuantization__237_131__7__mod(int ____n, float *____in, int *____out);
extern void work_staticQuantization__237_131__7__mod2(int ____n, float *____in, int *____out, int s1, int s2);
#endif
extern void __splitter_8_work(int);
extern void __joiner_9_work(int);
extern void init_AnonFilter_a0__240_135__10();
extern void work_AnonFilter_a0__240_135__10(int);
#ifdef BUFFER_MERGE
extern void work_AnonFilter_a0__240_135__10__mod(int ____n, int *____in, int *____out);
extern void work_AnonFilter_a0__240_135__10__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void __splitter_11_work(int);
extern void init_IDforNothing__246_137__12();
extern void work_IDforNothing__246_137__12(int);
#ifdef BUFFER_MERGE
extern void work_IDforNothing__246_137__12__mod(int ____n, int *____in, int *____out);
extern void work_IDforNothing__246_137__12__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void __joiner_13_work(int);
extern void init_ZigZagOrdering__254_139__14();
extern void work_ZigZagOrdering__254_139__14(int);
#ifdef BUFFER_MERGE
extern void work_ZigZagOrdering__254_139__14__mod(int ____n, int *____in, int *____out);
extern void work_ZigZagOrdering__254_139__14__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_RunLengthEncoder__262_140__15();
extern void work_RunLengthEncoder__262_140__15(int);
#ifdef BUFFER_MERGE
extern void work_RunLengthEncoder__262_140__15__mod(int ____n, int *____in, int *____out);
extern void work_RunLengthEncoder__262_140__15__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_RunLengthDecoder__268_141__16();
extern void work_RunLengthDecoder__268_141__16(int);
#ifdef BUFFER_MERGE
extern void work_RunLengthDecoder__268_141__16__mod(int ____n, int *____in, int *____out);
extern void work_RunLengthDecoder__268_141__16__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_ZigZagUnordering__273_142__17();
extern void work_ZigZagUnordering__273_142__17(int);
#ifdef BUFFER_MERGE
extern void work_ZigZagUnordering__273_142__17__mod(int ____n, int *____in, int *____out);
extern void work_ZigZagUnordering__273_142__17__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void __splitter_18_work(int);
extern void __joiner_19_work(int);
extern void init_AnonFilter_a1__276_146__20();
extern void work_AnonFilter_a1__276_146__20(int);
#ifdef BUFFER_MERGE
extern void work_AnonFilter_a1__276_146__20__mod(int ____n, int *____in, int *____out);
extern void work_AnonFilter_a1__276_146__20__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void __splitter_21_work(int);
extern void init_IDforNothing__282_148__22();
extern void work_IDforNothing__282_148__22(int);
#ifdef BUFFER_MERGE
extern void work_IDforNothing__282_148__22__mod(int ____n, int *____in, int *____out);
extern void work_IDforNothing__282_148__22__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void __joiner_23_work(int);
extern void init_staticDeQuantization__292_151__24();
extern void work_staticDeQuantization__292_151__24(int);
#ifdef BUFFER_MERGE
extern void work_staticDeQuantization__292_151__24__mod(int ____n, int *____in, float *____out);
extern void work_staticDeQuantization__292_151__24__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
extern void init_iDCT_1D__300_153__25();
extern void work_iDCT_1D__300_153__25(int);
#ifdef BUFFER_MERGE
extern void work_iDCT_1D__300_153__25__mod(int ____n, float *____in, float *____out);
extern void work_iDCT_1D__300_153__25__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_Pre_CollapsedDataParallel_1__306_262__26();
extern void work_Pre_CollapsedDataParallel_1__306_262__26(int);
#ifdef BUFFER_MERGE
extern void work_Pre_CollapsedDataParallel_1__306_262__26__mod(int ____n, float *____in, float *____out);
extern void work_Pre_CollapsedDataParallel_1__306_262__26__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_iDCT_1D__314_161__27();
extern void work_iDCT_1D__314_161__27(int);
#ifdef BUFFER_MERGE
extern void work_iDCT_1D__314_161__27__mod(int ____n, float *____in, float *____out);
extern void work_iDCT_1D__314_161__27__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_Post_CollapsedDataParallel_2__320_263__28();
extern void work_Post_CollapsedDataParallel_2__320_263__28(int);
#ifdef BUFFER_MERGE
extern void work_Post_CollapsedDataParallel_2__320_263__28__mod(int ____n, float *____in, float *____out);
extern void work_Post_CollapsedDataParallel_2__320_263__28__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_Add__323_169__29();
extern void work_Add__323_169__29(int);
#ifdef BUFFER_MERGE
extern void work_Add__323_169__29__mod(int ____n, float *____in, float *____out);
extern void work_Add__323_169__29__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_Float2Int__326_170__30();
extern void work_Float2Int__326_170__30(int);
#ifdef BUFFER_MERGE
extern void work_Float2Int__326_170__30__mod(int ____n, float *____in, int *____out);
extern void work_Float2Int__326_170__30__mod2(int ____n, float *____in, int *____out, int s1, int s2);
#endif
extern void init_Display8BitIntBlock__331_171__31();
extern void work_Display8BitIntBlock__331_171__31(int);
#ifdef BUFFER_MERGE
extern void work_Display8BitIntBlock__331_171__31__mod(int ____n, int *____in, void *____out);
extern void work_Display8BitIntBlock__331_171__31__mod2(int ____n, int *____in, void *____out, int s1, int s2);
#endif
extern void init_Identity__279_147__32();
extern void Identity__279_147__work__32(int);
#ifdef BUFFER_MERGE
extern void Identity__279_147__work__32__mod(int ____n, int *____in, int *____out);
extern void Identity__279_147__work__32__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Identity__285_149__33();
extern void Identity__285_149__work__33(int);
#ifdef BUFFER_MERGE
extern void Identity__285_149__work__33__mod(int ____n, int *____in, int *____out);
extern void Identity__285_149__work__33__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Identity__243_136__34();
extern void Identity__243_136__work__34(int);
#ifdef BUFFER_MERGE
extern void Identity__243_136__work__34__mod(int ____n, int *____in, int *____out);
extern void Identity__243_136__work__34__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
extern void init_Identity__249_138__35();
extern void Identity__249_138__work__35(int);
#ifdef BUFFER_MERGE
extern void Identity__249_138__work__35__mod(int ____n, int *____in, int *____out);
extern void Identity__249_138__work__35__mod2(int ____n, int *____in, int *____out, int s1, int s2);
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
// number of phases: 34


  // ============= Initialization =============
__feedbackjoiner_9_prep();
__feedbackjoiner_19_prep();

init_ACMSample8BitBlock__196_110__0();
init_Int2Float__199_111__1();
init_Add__202_112__2();
init_DCT_1D__210_114__3();
init_Pre_CollapsedDataParallel_1__216_260__4();
init_DCT_1D__224_122__5();
init_Post_CollapsedDataParallel_2__230_261__6();
init_staticQuantization__237_131__7();
init_Identity__249_138__35();
init_AnonFilter_a0__240_135__10();
init_Identity__243_136__34();
init_IDforNothing__246_137__12();
init_ZigZagOrdering__254_139__14();
init_RunLengthEncoder__262_140__15();
init_RunLengthDecoder__268_141__16();
init_ZigZagUnordering__273_142__17();
init_Identity__285_149__33();
init_AnonFilter_a1__276_146__20();
init_Identity__279_147__32();
init_IDforNothing__282_148__22();
init_staticDeQuantization__292_151__24();
init_iDCT_1D__300_153__25();
init_Pre_CollapsedDataParallel_1__306_262__26();
init_iDCT_1D__314_161__27();
init_Post_CollapsedDataParallel_2__320_263__28();
init_Add__323_169__29();
init_Float2Int__326_170__30();
init_Display8BitIntBlock__331_171__31();

  // ============= Steady State =============

  if (__timer_enabled) {
    tt.start();
  }
  for (int n = 0; n < (__max_iteration  ); n++) {
HEAD_0_1 = 0;
TAIL_0_1 = 0;
    work_ACMSample8BitBlock__196_110__0(1 );
HEAD_1_2 = 0;
TAIL_1_2 = 0;
    work_Int2Float__199_111__1(64 );
HEAD_2_3 = 0;
TAIL_2_3 = 0;
    work_Add__202_112__2(64 );
HEAD_3_4 = 0;
TAIL_3_4 = 0;
    work_DCT_1D__210_114__3(8 );
HEAD_4_5 = 0;
TAIL_4_5 = 0;
    work_Pre_CollapsedDataParallel_1__216_260__4(1 );
HEAD_5_6 = 0;
TAIL_5_6 = 0;
    work_DCT_1D__224_122__5(8 );
HEAD_6_7 = 0;
TAIL_6_7 = 0;
    work_Post_CollapsedDataParallel_2__230_261__6(1 );
HEAD_7_8 = 0;
TAIL_7_8 = 0;
    work_staticQuantization__237_131__7(1 );
HEAD_8_9 = 0;
TAIL_8_9 = 0;
HEAD_8_35 = 0;
TAIL_8_35 = 0;
    __splitter_8_work(1 );
HEAD_35_13 = 0;
TAIL_35_13 = 0;
    Identity__249_138__work__35(63 );
HEAD_9_10 = 0;
TAIL_9_10 = 0;
    __joiner_9_work(1 );
HEAD_10_11 = 0;
TAIL_10_11 = 0;
    work_AnonFilter_a0__240_135__10(1 );
HEAD_11_12 = 0;
TAIL_11_12 = 0;
HEAD_11_34 = 0;
TAIL_11_34 = 0;
    __splitter_11_work(1 );
for (int __y = 0; __y < __PEEK_BUF_SIZE_34_9; __y++) {
  BUFFER_34_9[__y] = BUFFER_34_9[__y + TAIL_34_9];
}
HEAD_34_9 -= TAIL_34_9;
TAIL_34_9 = 0;
    Identity__243_136__work__34(1 );
HEAD_12_13 = 0;
TAIL_12_13 = 0;
    work_IDforNothing__246_137__12(1 );
HEAD_13_14 = 0;
TAIL_13_14 = 0;
    __joiner_13_work(1 );
HEAD_14_15 = 0;
TAIL_14_15 = 0;
    work_ZigZagOrdering__254_139__14(1 );
HEAD_15_16 = 0;
TAIL_15_16 = 0;
    work_RunLengthEncoder__262_140__15(1 );
HEAD_16_17 = 0;
TAIL_16_17 = 0;
    work_RunLengthDecoder__268_141__16(32 );
HEAD_17_18 = 0;
TAIL_17_18 = 0;
    work_ZigZagUnordering__273_142__17(1 );
HEAD_18_19 = 0;
TAIL_18_19 = 0;
HEAD_18_33 = 0;
TAIL_18_33 = 0;
    __splitter_18_work(1 );
HEAD_19_20 = 0;
TAIL_19_20 = 0;
    __joiner_19_work(1 );
HEAD_33_23 = 0;
TAIL_33_23 = 0;
    Identity__285_149__work__33(63 );
HEAD_20_21 = 0;
TAIL_20_21 = 0;
    work_AnonFilter_a1__276_146__20(1 );
HEAD_21_22 = 0;
TAIL_21_22 = 0;
HEAD_21_32 = 0;
TAIL_21_32 = 0;
    __splitter_21_work(1 );
for (int __y = 0; __y < __PEEK_BUF_SIZE_32_19; __y++) {
  BUFFER_32_19[__y] = BUFFER_32_19[__y + TAIL_32_19];
}
HEAD_32_19 -= TAIL_32_19;
TAIL_32_19 = 0;
    Identity__279_147__work__32(1 );
HEAD_22_23 = 0;
TAIL_22_23 = 0;
    work_IDforNothing__282_148__22(1 );
HEAD_23_24 = 0;
TAIL_23_24 = 0;
    __joiner_23_work(1 );
HEAD_24_25 = 0;
TAIL_24_25 = 0;
    work_staticDeQuantization__292_151__24(1 );
HEAD_25_26 = 0;
TAIL_25_26 = 0;
    work_iDCT_1D__300_153__25(8 );
HEAD_26_27 = 0;
TAIL_26_27 = 0;
    work_Pre_CollapsedDataParallel_1__306_262__26(1 );
HEAD_27_28 = 0;
TAIL_27_28 = 0;
    work_iDCT_1D__314_161__27(8 );
HEAD_28_29 = 0;
TAIL_28_29 = 0;
    work_Post_CollapsedDataParallel_2__320_263__28(1 );
HEAD_29_30 = 0;
TAIL_29_30 = 0;
    work_Add__323_169__29(64 );
HEAD_30_31 = 0;
TAIL_30_31 = 0;
    work_Float2Int__326_170__30(64 );
    work_Display8BitIntBlock__331_171__31(8 );
  }
if (__timer_enabled) {
    tt.stop();
    tt.output(stderr);
  }


  return 0;
}
