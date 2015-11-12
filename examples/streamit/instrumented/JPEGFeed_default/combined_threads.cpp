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

volatile long long sum;

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
void __feedbackjoiner_9_prep();
void __feedbackjoiner_19_prep();
void init_ACMSample8BitBlock__196_110__0();
void work_ACMSample8BitBlock__196_110__0(int);
#ifdef BUFFER_MERGE
void work_ACMSample8BitBlock__196_110__0__mod(int ____n, void *____in, int *____out);
void work_ACMSample8BitBlock__196_110__0__mod2(int ____n, void *____in, int *____out, int s1, int s2);
#endif
void init_Int2Float__199_111__1();
void work_Int2Float__199_111__1(int);
#ifdef BUFFER_MERGE
void work_Int2Float__199_111__1__mod(int ____n, int *____in, float *____out);
void work_Int2Float__199_111__1__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
void init_Add__202_112__2();
void work_Add__202_112__2(int);
#ifdef BUFFER_MERGE
void work_Add__202_112__2__mod(int ____n, float *____in, float *____out);
void work_Add__202_112__2__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_DCT_1D__210_114__3();
void work_DCT_1D__210_114__3(int);
#ifdef BUFFER_MERGE
void work_DCT_1D__210_114__3__mod(int ____n, float *____in, float *____out);
void work_DCT_1D__210_114__3__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_Pre_CollapsedDataParallel_1__216_260__4();
void work_Pre_CollapsedDataParallel_1__216_260__4(int);
#ifdef BUFFER_MERGE
void work_Pre_CollapsedDataParallel_1__216_260__4__mod(int ____n, float *____in, float *____out);
void work_Pre_CollapsedDataParallel_1__216_260__4__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_DCT_1D__224_122__5();
void work_DCT_1D__224_122__5(int);
#ifdef BUFFER_MERGE
void work_DCT_1D__224_122__5__mod(int ____n, float *____in, float *____out);
void work_DCT_1D__224_122__5__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_Post_CollapsedDataParallel_2__230_261__6();
void work_Post_CollapsedDataParallel_2__230_261__6(int);
#ifdef BUFFER_MERGE
void work_Post_CollapsedDataParallel_2__230_261__6__mod(int ____n, float *____in, float *____out);
void work_Post_CollapsedDataParallel_2__230_261__6__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_staticQuantization__237_131__7();
void work_staticQuantization__237_131__7(int);
#ifdef BUFFER_MERGE
void work_staticQuantization__237_131__7__mod(int ____n, float *____in, int *____out);
void work_staticQuantization__237_131__7__mod2(int ____n, float *____in, int *____out, int s1, int s2);
#endif
void __splitter_8_work(int);
void __joiner_9_work(int);
void init_AnonFilter_a0__240_135__10();
void work_AnonFilter_a0__240_135__10(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__240_135__10__mod(int ____n, int *____in, int *____out);
void work_AnonFilter_a0__240_135__10__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void __splitter_11_work(int);
void init_IDforNothing__246_137__12();
void work_IDforNothing__246_137__12(int);
#ifdef BUFFER_MERGE
void work_IDforNothing__246_137__12__mod(int ____n, int *____in, int *____out);
void work_IDforNothing__246_137__12__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void __joiner_13_work(int);
void init_ZigZagOrdering__254_139__14();
void work_ZigZagOrdering__254_139__14(int);
#ifdef BUFFER_MERGE
void work_ZigZagOrdering__254_139__14__mod(int ____n, int *____in, int *____out);
void work_ZigZagOrdering__254_139__14__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_RunLengthEncoder__262_140__15();
void work_RunLengthEncoder__262_140__15(int);
#ifdef BUFFER_MERGE
void work_RunLengthEncoder__262_140__15__mod(int ____n, int *____in, int *____out);
void work_RunLengthEncoder__262_140__15__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_RunLengthDecoder__268_141__16();
void work_RunLengthDecoder__268_141__16(int);
#ifdef BUFFER_MERGE
void work_RunLengthDecoder__268_141__16__mod(int ____n, int *____in, int *____out);
void work_RunLengthDecoder__268_141__16__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_ZigZagUnordering__273_142__17();
void work_ZigZagUnordering__273_142__17(int);
#ifdef BUFFER_MERGE
void work_ZigZagUnordering__273_142__17__mod(int ____n, int *____in, int *____out);
void work_ZigZagUnordering__273_142__17__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void __splitter_18_work(int);
void __joiner_19_work(int);
void init_AnonFilter_a1__276_146__20();
void work_AnonFilter_a1__276_146__20(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a1__276_146__20__mod(int ____n, int *____in, int *____out);
void work_AnonFilter_a1__276_146__20__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void __splitter_21_work(int);
void init_IDforNothing__282_148__22();
void work_IDforNothing__282_148__22(int);
#ifdef BUFFER_MERGE
void work_IDforNothing__282_148__22__mod(int ____n, int *____in, int *____out);
void work_IDforNothing__282_148__22__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void __joiner_23_work(int);
void init_staticDeQuantization__292_151__24();
void work_staticDeQuantization__292_151__24(int);
#ifdef BUFFER_MERGE
void work_staticDeQuantization__292_151__24__mod(int ____n, int *____in, float *____out);
void work_staticDeQuantization__292_151__24__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
void init_iDCT_1D__300_153__25();
void work_iDCT_1D__300_153__25(int);
#ifdef BUFFER_MERGE
void work_iDCT_1D__300_153__25__mod(int ____n, float *____in, float *____out);
void work_iDCT_1D__300_153__25__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_Pre_CollapsedDataParallel_1__306_262__26();
void work_Pre_CollapsedDataParallel_1__306_262__26(int);
#ifdef BUFFER_MERGE
void work_Pre_CollapsedDataParallel_1__306_262__26__mod(int ____n, float *____in, float *____out);
void work_Pre_CollapsedDataParallel_1__306_262__26__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_iDCT_1D__314_161__27();
void work_iDCT_1D__314_161__27(int);
#ifdef BUFFER_MERGE
void work_iDCT_1D__314_161__27__mod(int ____n, float *____in, float *____out);
void work_iDCT_1D__314_161__27__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_Post_CollapsedDataParallel_2__320_263__28();
void work_Post_CollapsedDataParallel_2__320_263__28(int);
#ifdef BUFFER_MERGE
void work_Post_CollapsedDataParallel_2__320_263__28__mod(int ____n, float *____in, float *____out);
void work_Post_CollapsedDataParallel_2__320_263__28__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_Add__323_169__29();
void work_Add__323_169__29(int);
#ifdef BUFFER_MERGE
void work_Add__323_169__29__mod(int ____n, float *____in, float *____out);
void work_Add__323_169__29__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_Float2Int__326_170__30();
void work_Float2Int__326_170__30(int);
#ifdef BUFFER_MERGE
void work_Float2Int__326_170__30__mod(int ____n, float *____in, int *____out);
void work_Float2Int__326_170__30__mod2(int ____n, float *____in, int *____out, int s1, int s2);
#endif
void init_Display8BitIntBlock__331_171__31();
void work_Display8BitIntBlock__331_171__31(int);
#ifdef BUFFER_MERGE
void work_Display8BitIntBlock__331_171__31__mod(int ____n, int *____in, void *____out);
void work_Display8BitIntBlock__331_171__31__mod2(int ____n, int *____in, void *____out, int s1, int s2);
#endif
void init_Identity__279_147__32();
void Identity__279_147__work__32(int);
#ifdef BUFFER_MERGE
void Identity__279_147__work__32__mod(int ____n, int *____in, int *____out);
void Identity__279_147__work__32__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Identity__285_149__33();
void Identity__285_149__work__33(int);
#ifdef BUFFER_MERGE
void Identity__285_149__work__33__mod(int ____n, int *____in, int *____out);
void Identity__285_149__work__33__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Identity__243_136__34();
void Identity__243_136__work__34(int);
#ifdef BUFFER_MERGE
void Identity__243_136__work__34__mod(int ____n, int *____in, int *____out);
void Identity__243_136__work__34__mod2(int ____n, int *____in, int *____out, int s1, int s2);
#endif
void init_Identity__249_138__35();
void Identity__249_138__work__35(int);
#ifdef BUFFER_MERGE
void Identity__249_138__work__35__mod(int ____n, int *____in, int *____out);
void Identity__249_138__work__35__mod2(int ____n, int *____in, int *____out, int s1, int s2);
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

#ifdef MEASURE_BY_RTS
RTS_init(1,1);
#else //default ``MEASURE_BY_PAPI''
PAPI_HW_COUNTER_init(1,1);
#endif

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
message *__msg_stack_34;
message *__msg_stack_3;
message *__msg_stack_10;
message *__msg_stack_24;
message *__msg_stack_9;
message *__msg_stack_31;
message *__msg_stack_1;
message *__msg_stack_14;
message *__msg_stack_19;
message *__msg_stack_5;
message *__msg_stack_15;
message *__msg_stack_2;
message *__msg_stack_17;
message *__msg_stack_21;
message *__msg_stack_23;
message *__msg_stack_30;
message *__msg_stack_26;
message *__msg_stack_29;
message *__msg_stack_35;
message *__msg_stack_7;
message *__msg_stack_6;
message *__msg_stack_25;
message *__msg_stack_4;
message *__msg_stack_20;
message *__msg_stack_33;
message *__msg_stack_0;
message *__msg_stack_8;
message *__msg_stack_27;
message *__msg_stack_32;
message *__msg_stack_12;
message *__msg_stack_11;
message *__msg_stack_28;
message *__msg_stack_13;
message *__msg_stack_18;
message *__msg_stack_16;
message *__msg_stack_22;

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



int out__192__0[64] = {139, 144, 149, 153, 155, 155, 155, 155, 144, 151, 153, 156, 149, 156, 156, 156, 150, 155, 160, 163, 158, 156, 156, 156, 159, 161, 162, 160, 160, 159, 159, 159, 159, 160, 161, 162, 162, 155, 155, 155, 161, 161, 161, 161, 160, 157, 157, 157, 162, 162, 161, 163, 162, 157, 157, 157, 162, 162, 161, 161, 163, 158, 158, 158};
void save_file_pointer__0(object_write_buffer *buf);
void load_file_pointer__0(object_write_buffer *buf);


void init_ACMSample8BitBlock__196_110__0();
inline void check_status__0();

void work_ACMSample8BitBlock__196_110__0(int);



inline void __push__0(int data) {
    BUFFER_0_1[HEAD_0_1]=data;
    HEAD_0_1++;
}




void init_ACMSample8BitBlock__196_110__0(){
}
void save_file_pointer__0(object_write_buffer *buf) {}
void load_file_pointer__0(object_write_buffer *buf) {}

void work_ACMSample8BitBlock__196_110__0(int ____n){
    for (
            ; (0 < ____n); (____n--)) {{
        int i__195 = 0;/* int */

        // mark begin: SIRFilter ACMSample8BitBlock

        for ((i__195 = 0)/*int*/; (i__195 < 64); (i__195++)) {{
            __push__0(((out__192__0)[(int)i__195]));
        }
        }
        // mark end: SIRFilter ACMSample8BitBlock

    }
    }
}

// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 64

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


void init_Int2Float__199_111__1();
inline void check_status__1();

void work_Int2Float__199_111__1(int);


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



inline void __push__1(float data) {
    BUFFER_1_2[HEAD_1_2]=data;
    HEAD_1_2++;
}




void init_Int2Float__199_111__1(){
}
void save_file_pointer__1(object_write_buffer *buf) {}
void load_file_pointer__1(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Int2Float__199_111__1__mod(int ____n, int *____in, float *____out) {
    for (; (0 < ____n); ____n--)
    {
        // mark begin: SIRFilter Int2Float

        ((*____out++)=((float)((*____in++))));
        // mark end: SIRFilter Int2Float

    }}


void work_Int2Float__199_111__1__mod2(int ____n, int *____in, float *____out, int s1, int s2) {
    for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
    {
        // mark begin: SIRFilter Int2Float

        ((*____out++)=((float)((*____in++))));
        // mark end: SIRFilter Int2Float

    }}


#endif // BUFFER_MERGE



void work_Int2Float__199_111__1(int ____n){
    for (
            ; (0 < ____n); (____n--)) {{
        // mark begin: SIRFilter Int2Float

        __push__1(((float)(__pop__1())));
        // mark end: SIRFilter Int2Float

    }
    }
}

// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 64

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


void init_Add__202_112__2();
inline void check_status__2();

void work_Add__202_112__2(int);


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




void init_Add__202_112__2(){
}
void save_file_pointer__2(object_write_buffer *buf) {}
void load_file_pointer__2(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Add__202_112__2__mod(int ____n, float *____in, float *____out) {
    for (; (0 < ____n); ____n--)
    {
        // mark begin: SIRFilter Add

        ((*____out++)=((*____in++) + ((float)-128.0)));
        // mark end: SIRFilter Add

    }}


void work_Add__202_112__2__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
    for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
    {
        // mark begin: SIRFilter Add

        ((*____out++)=((*____in++) + ((float)-128.0)));
        // mark end: SIRFilter Add

    }}


#endif // BUFFER_MERGE



void work_Add__202_112__2(int ____n){
    for (
            ; (0 < ____n); (____n--)) {{
        // mark begin: SIRFilter Add

        __push__2((__pop__2() + ((float)-128.0)));
        // mark end: SIRFilter Add

    }
    }
}

// peek: 8 pop: 8 push 8
// init counts: 0 steady counts: 8

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


void init_DCT_1D__210_114__3();
inline void check_status__3();

void work_DCT_1D__210_114__3(int);


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




void init_DCT_1D__210_114__3(){
}
void save_file_pointer__3(object_write_buffer *buf) {}
void load_file_pointer__3(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_DCT_1D__210_114__3__mod(int ____n, float *____in, float *____out) {
    for (; (0 < ____n); ____n--)
    {
        float Cu__205 = 0.0f;/* float */
        float tempsum__206 = 0.0f;/* float */
        int x__conflict__0__207 = 0;/* int */
        int u__208 = 0;/* int */
        int x__209 = 0;/* int */

        // mark begin: SIRFilter DCT_1D

        for ((u__208 = 0)/*int*/; (u__208 < 8); (u__208++)) {{

            if ((u__208 == 0)) {(Cu__205 = ((float)0.70710677))/*float*/;
            } else {(Cu__205 = ((float)1.0))/*float*/;}
            (tempsum__206 = ((float)0.0))/*float*/;
            for ((x__conflict__0__207 = 0)/*int*/; (x__conflict__0__207 < 8); (x__conflict__0__207++)) {{
                (tempsum__206 = (tempsum__206 + ((*(____in+x__conflict__0__207)) * ((float)(cosf(((double)((((((float)(u__208)) * ((float)3.1415927)) * ((((float)2.0) * ((float)(x__conflict__0__207))) + ((float)1.0))) / ((float)16.0))))))))))/*float*/;
            }
            }
            ((*____out++)=((((float)0.5) * Cu__205) * tempsum__206));
        }
        }
        for ((x__209 = 0)/*int*/; (x__209 < 8); (x__209++)) {{
            (*____in++);
        }
        }
        // mark end: SIRFilter DCT_1D

    }}


void work_DCT_1D__210_114__3__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
    for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
    {
        float Cu__205 = 0.0f;/* float */
        float tempsum__206 = 0.0f;/* float */
        int x__conflict__0__207 = 0;/* int */
        int u__208 = 0;/* int */
        int x__209 = 0;/* int */

        // mark begin: SIRFilter DCT_1D

        for ((u__208 = 0)/*int*/; (u__208 < 8); (u__208++)) {{

            if ((u__208 == 0)) {(Cu__205 = ((float)0.70710677))/*float*/;
            } else {(Cu__205 = ((float)1.0))/*float*/;}
            (tempsum__206 = ((float)0.0))/*float*/;
            for ((x__conflict__0__207 = 0)/*int*/; (x__conflict__0__207 < 8); (x__conflict__0__207++)) {{
                (tempsum__206 = (tempsum__206 + ((*(____in+x__conflict__0__207)) * ((float)(cosf(((double)((((((float)(u__208)) * ((float)3.1415927)) * ((((float)2.0) * ((float)(x__conflict__0__207))) + ((float)1.0))) / ((float)16.0))))))))))/*float*/;
            }
            }
            ((*____out++)=((((float)0.5) * Cu__205) * tempsum__206));
        }
        }
        for ((x__209 = 0)/*int*/; (x__209 < 8); (x__209++)) {{
            (*____in++);
        }
        }
        // mark end: SIRFilter DCT_1D

    }}


#endif // BUFFER_MERGE



void work_DCT_1D__210_114__3(int ____n){
    for (
            ; (0 < ____n); (____n--)) {{
        float Cu__205 = 0.0f;/* float */
        float tempsum__206 = 0.0f;/* float */
        int x__conflict__0__207 = 0;/* int */
        int u__208 = 0;/* int */
        int x__209 = 0;/* int */

        // mark begin: SIRFilter DCT_1D

        for ((u__208 = 0)/*int*/; (u__208 < 8); (u__208++)) {{

            if ((u__208 == 0)) {(Cu__205 = ((float)0.70710677))/*float*/;
            } else {(Cu__205 = ((float)1.0))/*float*/;}
            (tempsum__206 = ((float)0.0))/*float*/;
            for ((x__conflict__0__207 = 0)/*int*/; (x__conflict__0__207 < 8); (x__conflict__0__207++)) {{
                (tempsum__206 = (tempsum__206 + (__peek__3(x__conflict__0__207) * ((float)(cosf(((double)((((((float)(u__208)) * ((float)3.1415927)) * ((((float)2.0) * ((float)(x__conflict__0__207))) + ((float)1.0))) / ((float)16.0))))))))))/*float*/;
            }
            }
            __push__3(((((float)0.5) * Cu__205) * tempsum__206));
        }
        }
        for ((x__209 = 0)/*int*/; (x__209 < 8); (x__209++)) {{
            __pop__3();
        }
        }
        // mark end: SIRFilter DCT_1D

    }
    }
}

// peek: 64 pop: 64 push 64
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


void init_Pre_CollapsedDataParallel_1__216_260__4();
inline void check_status__4();

void work_Pre_CollapsedDataParallel_1__216_260__4(int);


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




void init_Pre_CollapsedDataParallel_1__216_260__4(){
}
void save_file_pointer__4(object_write_buffer *buf) {}
void load_file_pointer__4(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Pre_CollapsedDataParallel_1__216_260__4__mod(int ____n, float *____in, float *____out) {
    for (; (0 < ____n); ____n--)
    {
        int _k__213 = 0;/* int */
        int iTimesSumOfWeights_Plus_PartialSum_k__214 = 0;/* int */
        int _i__215 = 0;/* int */

        // mark begin: SIRFilter Pre_CollapsedDataParallel_1

        // mark begin: SIRSplitter WEIGHTED_ROUND_ROBIN_Splitter

        for ((_k__213 = 0)/*int*/; (_k__213 < 8); (_k__213++)) {{
            (iTimesSumOfWeights_Plus_PartialSum_k__214 = _k__213)/*int*/;
            for ((_i__215 = 0)/*int*/; (_i__215 < 8); (_i__215++)) {{
                ((*____out++)=(*(____in+(iTimesSumOfWeights_Plus_PartialSum_k__214 + 0))));
                (iTimesSumOfWeights_Plus_PartialSum_k__214 = (iTimesSumOfWeights_Plus_PartialSum_k__214 + 8))/*int*/;
            }
            }
        }
        }
        assert(false);__pop__4(64);
        // mark end: SIRSplitter WEIGHTED_ROUND_ROBIN_Splitter

        // mark end: SIRFilter Pre_CollapsedDataParallel_1

    }}


void work_Pre_CollapsedDataParallel_1__216_260__4__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
    for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
    {
        int _k__213 = 0;/* int */
        int iTimesSumOfWeights_Plus_PartialSum_k__214 = 0;/* int */
        int _i__215 = 0;/* int */

        // mark begin: SIRFilter Pre_CollapsedDataParallel_1

        // mark begin: SIRSplitter WEIGHTED_ROUND_ROBIN_Splitter

        for ((_k__213 = 0)/*int*/; (_k__213 < 8); (_k__213++)) {{
            (iTimesSumOfWeights_Plus_PartialSum_k__214 = _k__213)/*int*/;
            for ((_i__215 = 0)/*int*/; (_i__215 < 8); (_i__215++)) {{
                ((*____out++)=(*(____in+(iTimesSumOfWeights_Plus_PartialSum_k__214 + 0))));
                (iTimesSumOfWeights_Plus_PartialSum_k__214 = (iTimesSumOfWeights_Plus_PartialSum_k__214 + 8))/*int*/;
            }
            }
        }
        }
        assert(false);__pop__4(64);
        // mark end: SIRSplitter WEIGHTED_ROUND_ROBIN_Splitter

        // mark end: SIRFilter Pre_CollapsedDataParallel_1

    }}


#endif // BUFFER_MERGE



void work_Pre_CollapsedDataParallel_1__216_260__4(int ____n){
    for (
            ; (0 < ____n); (____n--)) {{
        int _k__213 = 0;/* int */
        int iTimesSumOfWeights_Plus_PartialSum_k__214 = 0;/* int */
        int _i__215 = 0;/* int */

        // mark begin: SIRFilter Pre_CollapsedDataParallel_1

        // mark begin: SIRSplitter WEIGHTED_ROUND_ROBIN_Splitter

        for ((_k__213 = 0)/*int*/; (_k__213 < 8); (_k__213++)) {{
            (iTimesSumOfWeights_Plus_PartialSum_k__214 = _k__213)/*int*/;
            for ((_i__215 = 0)/*int*/; (_i__215 < 8); (_i__215++)) {{
                __push__4(__peek__4((iTimesSumOfWeights_Plus_PartialSum_k__214 + 0)));
                (iTimesSumOfWeights_Plus_PartialSum_k__214 = (iTimesSumOfWeights_Plus_PartialSum_k__214 + 8))/*int*/;
            }
            }
        }
        }
        __pop__4(64);
        // mark end: SIRSplitter WEIGHTED_ROUND_ROBIN_Splitter

        // mark end: SIRFilter Pre_CollapsedDataParallel_1

    }
    }
}

// peek: 8 pop: 8 push 8
// init counts: 0 steady counts: 8

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


void init_DCT_1D__224_122__5();
inline void check_status__5();

void work_DCT_1D__224_122__5(int);


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




void init_DCT_1D__224_122__5(){
}
void save_file_pointer__5(object_write_buffer *buf) {}
void load_file_pointer__5(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_DCT_1D__224_122__5__mod(int ____n, float *____in, float *____out) {
    for (; (0 < ____n); ____n--)
    {
        float Cu__219 = 0.0f;/* float */
        float tempsum__220 = 0.0f;/* float */
        int x__conflict__0__221 = 0;/* int */
        int u__222 = 0;/* int */
        int x__223 = 0;/* int */

        // mark begin: SIRFilter DCT_1D

        for ((u__222 = 0)/*int*/; (u__222 < 8); (u__222++)) {{

            if ((u__222 == 0)) {(Cu__219 = ((float)0.70710677))/*float*/;
            } else {(Cu__219 = ((float)1.0))/*float*/;}
            (tempsum__220 = ((float)0.0))/*float*/;
            for ((x__conflict__0__221 = 0)/*int*/; (x__conflict__0__221 < 8); (x__conflict__0__221++)) {{
                (tempsum__220 = (tempsum__220 + ((*(____in+x__conflict__0__221)) * ((float)(cosf(((double)((((((float)(u__222)) * ((float)3.1415927)) * ((((float)2.0) * ((float)(x__conflict__0__221))) + ((float)1.0))) / ((float)16.0))))))))))/*float*/;
            }
            }
            ((*____out++)=((((float)0.5) * Cu__219) * tempsum__220));
        }
        }
        for ((x__223 = 0)/*int*/; (x__223 < 8); (x__223++)) {{
            (*____in++);
        }
        }
        // mark end: SIRFilter DCT_1D

    }}


void work_DCT_1D__224_122__5__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
    for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
    {
        float Cu__219 = 0.0f;/* float */
        float tempsum__220 = 0.0f;/* float */
        int x__conflict__0__221 = 0;/* int */
        int u__222 = 0;/* int */
        int x__223 = 0;/* int */

        // mark begin: SIRFilter DCT_1D

        for ((u__222 = 0)/*int*/; (u__222 < 8); (u__222++)) {{

            if ((u__222 == 0)) {(Cu__219 = ((float)0.70710677))/*float*/;
            } else {(Cu__219 = ((float)1.0))/*float*/;}
            (tempsum__220 = ((float)0.0))/*float*/;
            for ((x__conflict__0__221 = 0)/*int*/; (x__conflict__0__221 < 8); (x__conflict__0__221++)) {{
                (tempsum__220 = (tempsum__220 + ((*(____in+x__conflict__0__221)) * ((float)(cosf(((double)((((((float)(u__222)) * ((float)3.1415927)) * ((((float)2.0) * ((float)(x__conflict__0__221))) + ((float)1.0))) / ((float)16.0))))))))))/*float*/;
            }
            }
            ((*____out++)=((((float)0.5) * Cu__219) * tempsum__220));
        }
        }
        for ((x__223 = 0)/*int*/; (x__223 < 8); (x__223++)) {{
            (*____in++);
        }
        }
        // mark end: SIRFilter DCT_1D

    }}


#endif // BUFFER_MERGE



void work_DCT_1D__224_122__5(int ____n){
    for (
            ; (0 < ____n); (____n--)) {{
        float Cu__219 = 0.0f;/* float */
        float tempsum__220 = 0.0f;/* float */
        int x__conflict__0__221 = 0;/* int */
        int u__222 = 0;/* int */
        int x__223 = 0;/* int */

        // mark begin: SIRFilter DCT_1D

        for ((u__222 = 0)/*int*/; (u__222 < 8); (u__222++)) {{

            if ((u__222 == 0)) {(Cu__219 = ((float)0.70710677))/*float*/;
            } else {(Cu__219 = ((float)1.0))/*float*/;}
            (tempsum__220 = ((float)0.0))/*float*/;
            for ((x__conflict__0__221 = 0)/*int*/; (x__conflict__0__221 < 8); (x__conflict__0__221++)) {{
                (tempsum__220 = (tempsum__220 + (__peek__5(x__conflict__0__221) * ((float)(cosf(((double)((((((float)(u__222)) * ((float)3.1415927)) * ((((float)2.0) * ((float)(x__conflict__0__221))) + ((float)1.0))) / ((float)16.0))))))))))/*float*/;
            }
            }
            __push__5(((((float)0.5) * Cu__219) * tempsum__220));
        }
        }
        for ((x__223 = 0)/*int*/; (x__223 < 8); (x__223++)) {{
            __pop__5();
        }
        }
        // mark end: SIRFilter DCT_1D

    }
    }
}

// peek: 64 pop: 64 push 64
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


void init_Post_CollapsedDataParallel_2__230_261__6();
inline void check_status__6();

void work_Post_CollapsedDataParallel_2__230_261__6(int);


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




void init_Post_CollapsedDataParallel_2__230_261__6(){
}
void save_file_pointer__6(object_write_buffer *buf) {}
void load_file_pointer__6(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Post_CollapsedDataParallel_2__230_261__6__mod(int ____n, float *____in, float *____out) {
    for (; (0 < ____n); ____n--)
    {
        int _k__227 = 0;/* int */
        int partialSum_i__228 = 0;/* int */
        int _i__229 = 0;/* int */

        // mark begin: SIRFilter Post_CollapsedDataParallel_2

        // mark begin: SIRJoiner WEIGHTED_ROUND_ROBIN_Joiner

        for ((_k__227 = 0)/*int*/; (_k__227 < 8); (_k__227++)) {{
            (partialSum_i__228 = 0)/*int*/;
            for ((_i__229 = 0)/*int*/; (_i__229 < 8); (_i__229++)) {{
                ((*____out++)=(*(____in+(_k__227 + (partialSum_i__228 + 0)))));
                (partialSum_i__228 = (partialSum_i__228 + 8))/*int*/;
            }
            }
        }
        }
        assert(false);__pop__6(64);
        // mark end: SIRJoiner WEIGHTED_ROUND_ROBIN_Joiner

        // mark end: SIRFilter Post_CollapsedDataParallel_2

    }}


void work_Post_CollapsedDataParallel_2__230_261__6__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
    for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
    {
        int _k__227 = 0;/* int */
        int partialSum_i__228 = 0;/* int */
        int _i__229 = 0;/* int */

        // mark begin: SIRFilter Post_CollapsedDataParallel_2

        // mark begin: SIRJoiner WEIGHTED_ROUND_ROBIN_Joiner

        for ((_k__227 = 0)/*int*/; (_k__227 < 8); (_k__227++)) {{
            (partialSum_i__228 = 0)/*int*/;
            for ((_i__229 = 0)/*int*/; (_i__229 < 8); (_i__229++)) {{
                ((*____out++)=(*(____in+(_k__227 + (partialSum_i__228 + 0)))));
                (partialSum_i__228 = (partialSum_i__228 + 8))/*int*/;
            }
            }
        }
        }
        assert(false);__pop__6(64);
        // mark end: SIRJoiner WEIGHTED_ROUND_ROBIN_Joiner

        // mark end: SIRFilter Post_CollapsedDataParallel_2

    }}


#endif // BUFFER_MERGE



void work_Post_CollapsedDataParallel_2__230_261__6(int ____n){
    for (
            ; (0 < ____n); (____n--)) {{
        int _k__227 = 0;/* int */
        int partialSum_i__228 = 0;/* int */
        int _i__229 = 0;/* int */

        // mark begin: SIRFilter Post_CollapsedDataParallel_2

        // mark begin: SIRJoiner WEIGHTED_ROUND_ROBIN_Joiner

        for ((_k__227 = 0)/*int*/; (_k__227 < 8); (_k__227++)) {{
            (partialSum_i__228 = 0)/*int*/;
            for ((_i__229 = 0)/*int*/; (_i__229 < 8); (_i__229++)) {{
                __push__6(__peek__6((_k__227 + (partialSum_i__228 + 0))));
                (partialSum_i__228 = (partialSum_i__228 + 8))/*int*/;
            }
            }
        }
        }
        __pop__6(64);
        // mark end: SIRJoiner WEIGHTED_ROUND_ROBIN_Joiner

        // mark end: SIRFilter Post_CollapsedDataParallel_2

    }
    }
}

// peek: 64 pop: 64 push 64
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_7;
int __counter_7 = 0;
int __steady_7 = 0;
int __tmp_7 = 0;
int __tmp2_7 = 0;
int *__state_flag_7 = NULL;
thread_info *__thread_7 = NULL;



int quantizationTable__231__7[64] = {0};
void save_peek_buffer__7(object_write_buffer *buf);
void load_peek_buffer__7(object_write_buffer *buf);
void save_file_pointer__7(object_write_buffer *buf);
void load_file_pointer__7(object_write_buffer *buf);


void init_staticQuantization__237_131__7();
inline void check_status__7();

void work_staticQuantization__237_131__7(int);


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



inline void __push__7(int data) {
    BUFFER_7_8[HEAD_7_8]=data;
    HEAD_7_8++;
}




void init_staticQuantization__237_131__7(){
    int _param_quantizationTable__236[64] = {0};/* int[64] */

    ((_param_quantizationTable__236[(int)0]) = 16)/*int*/;
    ((_param_quantizationTable__236[(int)1]) = 11)/*int*/;
    ((_param_quantizationTable__236[(int)2]) = 10)/*int*/;
    ((_param_quantizationTable__236[(int)3]) = 16)/*int*/;
    ((_param_quantizationTable__236[(int)4]) = 24)/*int*/;
    ((_param_quantizationTable__236[(int)5]) = 40)/*int*/;
    ((_param_quantizationTable__236[(int)6]) = 51)/*int*/;
    ((_param_quantizationTable__236[(int)7]) = 61)/*int*/;
    ((_param_quantizationTable__236[(int)8]) = 12)/*int*/;
    ((_param_quantizationTable__236[(int)9]) = 12)/*int*/;
    ((_param_quantizationTable__236[(int)10]) = 14)/*int*/;
    ((_param_quantizationTable__236[(int)11]) = 19)/*int*/;
    ((_param_quantizationTable__236[(int)12]) = 26)/*int*/;
    ((_param_quantizationTable__236[(int)13]) = 58)/*int*/;
    ((_param_quantizationTable__236[(int)14]) = 60)/*int*/;
    ((_param_quantizationTable__236[(int)15]) = 55)/*int*/;
    ((_param_quantizationTable__236[(int)16]) = 14)/*int*/;
    ((_param_quantizationTable__236[(int)17]) = 13)/*int*/;
    ((_param_quantizationTable__236[(int)18]) = 16)/*int*/;
    ((_param_quantizationTable__236[(int)19]) = 24)/*int*/;
    ((_param_quantizationTable__236[(int)20]) = 40)/*int*/;
    ((_param_quantizationTable__236[(int)21]) = 57)/*int*/;
    ((_param_quantizationTable__236[(int)22]) = 69)/*int*/;
    ((_param_quantizationTable__236[(int)23]) = 56)/*int*/;
    ((_param_quantizationTable__236[(int)24]) = 14)/*int*/;
    ((_param_quantizationTable__236[(int)25]) = 17)/*int*/;
    ((_param_quantizationTable__236[(int)26]) = 22)/*int*/;
    ((_param_quantizationTable__236[(int)27]) = 29)/*int*/;
    ((_param_quantizationTable__236[(int)28]) = 51)/*int*/;
    ((_param_quantizationTable__236[(int)29]) = 87)/*int*/;
    ((_param_quantizationTable__236[(int)30]) = 80)/*int*/;
    ((_param_quantizationTable__236[(int)31]) = 62)/*int*/;
    ((_param_quantizationTable__236[(int)32]) = 18)/*int*/;
    ((_param_quantizationTable__236[(int)33]) = 22)/*int*/;
    ((_param_quantizationTable__236[(int)34]) = 37)/*int*/;
    ((_param_quantizationTable__236[(int)35]) = 56)/*int*/;
    ((_param_quantizationTable__236[(int)36]) = 68)/*int*/;
    ((_param_quantizationTable__236[(int)37]) = 109)/*int*/;
    ((_param_quantizationTable__236[(int)38]) = 103)/*int*/;
    ((_param_quantizationTable__236[(int)39]) = 77)/*int*/;
    ((_param_quantizationTable__236[(int)40]) = 24)/*int*/;
    ((_param_quantizationTable__236[(int)41]) = 35)/*int*/;
    ((_param_quantizationTable__236[(int)42]) = 55)/*int*/;
    ((_param_quantizationTable__236[(int)43]) = 64)/*int*/;
    ((_param_quantizationTable__236[(int)44]) = 81)/*int*/;
    ((_param_quantizationTable__236[(int)45]) = 104)/*int*/;
    ((_param_quantizationTable__236[(int)46]) = 113)/*int*/;
    ((_param_quantizationTable__236[(int)47]) = 92)/*int*/;
    ((_param_quantizationTable__236[(int)48]) = 49)/*int*/;
    ((_param_quantizationTable__236[(int)49]) = 64)/*int*/;
    ((_param_quantizationTable__236[(int)50]) = 78)/*int*/;
    ((_param_quantizationTable__236[(int)51]) = 87)/*int*/;
    ((_param_quantizationTable__236[(int)52]) = 103)/*int*/;
    ((_param_quantizationTable__236[(int)53]) = 121)/*int*/;
    ((_param_quantizationTable__236[(int)54]) = 120)/*int*/;
    ((_param_quantizationTable__236[(int)55]) = 101)/*int*/;
    ((_param_quantizationTable__236[(int)56]) = 72)/*int*/;
    ((_param_quantizationTable__236[(int)57]) = 92)/*int*/;
    ((_param_quantizationTable__236[(int)58]) = 95)/*int*/;
    ((_param_quantizationTable__236[(int)59]) = 98)/*int*/;
    ((_param_quantizationTable__236[(int)60]) = 112)/*int*/;
    ((_param_quantizationTable__236[(int)61]) = 100)/*int*/;
    ((_param_quantizationTable__236[(int)62]) = 103)/*int*/;
    ((_param_quantizationTable__236[(int)63]) = 99)/*int*/;
    ((_param_quantizationTable__236[(int)0]) = 16)/*int*/;
    ((_param_quantizationTable__236[(int)1]) = 11)/*int*/;
    ((_param_quantizationTable__236[(int)2]) = 10)/*int*/;
    ((_param_quantizationTable__236[(int)3]) = 16)/*int*/;
    ((_param_quantizationTable__236[(int)4]) = 24)/*int*/;
    ((_param_quantizationTable__236[(int)5]) = 40)/*int*/;
    ((_param_quantizationTable__236[(int)6]) = 51)/*int*/;
    ((_param_quantizationTable__236[(int)7]) = 61)/*int*/;
    ((_param_quantizationTable__236[(int)8]) = 12)/*int*/;
    ((_param_quantizationTable__236[(int)9]) = 12)/*int*/;
    ((_param_quantizationTable__236[(int)10]) = 14)/*int*/;
    ((_param_quantizationTable__236[(int)11]) = 19)/*int*/;
    ((_param_quantizationTable__236[(int)12]) = 26)/*int*/;
    ((_param_quantizationTable__236[(int)13]) = 58)/*int*/;
    ((_param_quantizationTable__236[(int)14]) = 60)/*int*/;
    ((_param_quantizationTable__236[(int)15]) = 55)/*int*/;
    ((_param_quantizationTable__236[(int)16]) = 14)/*int*/;
    ((_param_quantizationTable__236[(int)17]) = 13)/*int*/;
    ((_param_quantizationTable__236[(int)18]) = 16)/*int*/;
    ((_param_quantizationTable__236[(int)19]) = 24)/*int*/;
    ((_param_quantizationTable__236[(int)20]) = 40)/*int*/;
    ((_param_quantizationTable__236[(int)21]) = 57)/*int*/;
    ((_param_quantizationTable__236[(int)22]) = 69)/*int*/;
    ((_param_quantizationTable__236[(int)23]) = 56)/*int*/;
    ((_param_quantizationTable__236[(int)24]) = 14)/*int*/;
    ((_param_quantizationTable__236[(int)25]) = 17)/*int*/;
    ((_param_quantizationTable__236[(int)26]) = 22)/*int*/;
    ((_param_quantizationTable__236[(int)27]) = 29)/*int*/;
    ((_param_quantizationTable__236[(int)28]) = 51)/*int*/;
    ((_param_quantizationTable__236[(int)29]) = 87)/*int*/;
    ((_param_quantizationTable__236[(int)30]) = 80)/*int*/;
    ((_param_quantizationTable__236[(int)31]) = 62)/*int*/;
    ((_param_quantizationTable__236[(int)32]) = 18)/*int*/;
    ((_param_quantizationTable__236[(int)33]) = 22)/*int*/;
    ((_param_quantizationTable__236[(int)34]) = 37)/*int*/;
    ((_param_quantizationTable__236[(int)35]) = 56)/*int*/;
    ((_param_quantizationTable__236[(int)36]) = 68)/*int*/;
    ((_param_quantizationTable__236[(int)37]) = 109)/*int*/;
    ((_param_quantizationTable__236[(int)38]) = 103)/*int*/;
    ((_param_quantizationTable__236[(int)39]) = 77)/*int*/;
    ((_param_quantizationTable__236[(int)40]) = 24)/*int*/;
    ((_param_quantizationTable__236[(int)41]) = 35)/*int*/;
    ((_param_quantizationTable__236[(int)42]) = 55)/*int*/;
    ((_param_quantizationTable__236[(int)43]) = 64)/*int*/;
    ((_param_quantizationTable__236[(int)44]) = 81)/*int*/;
    ((_param_quantizationTable__236[(int)45]) = 104)/*int*/;
    ((_param_quantizationTable__236[(int)46]) = 113)/*int*/;
    ((_param_quantizationTable__236[(int)47]) = 92)/*int*/;
    ((_param_quantizationTable__236[(int)48]) = 49)/*int*/;
    ((_param_quantizationTable__236[(int)49]) = 64)/*int*/;
    ((_param_quantizationTable__236[(int)50]) = 78)/*int*/;
    ((_param_quantizationTable__236[(int)51]) = 87)/*int*/;
    ((_param_quantizationTable__236[(int)52]) = 103)/*int*/;
    ((_param_quantizationTable__236[(int)53]) = 121)/*int*/;
    ((_param_quantizationTable__236[(int)54]) = 120)/*int*/;
    ((_param_quantizationTable__236[(int)55]) = 101)/*int*/;
    ((_param_quantizationTable__236[(int)56]) = 72)/*int*/;
    ((_param_quantizationTable__236[(int)57]) = 92)/*int*/;
    ((_param_quantizationTable__236[(int)58]) = 95)/*int*/;
    ((_param_quantizationTable__236[(int)59]) = 98)/*int*/;
    ((_param_quantizationTable__236[(int)60]) = 112)/*int*/;
    ((_param_quantizationTable__236[(int)61]) = 100)/*int*/;
    ((_param_quantizationTable__236[(int)62]) = 103)/*int*/;
    ((_param_quantizationTable__236[(int)63]) = 99)/*int*/;
    {
        int __ARRAY_COPY__0;
        for (__ARRAY_COPY__0 = 0; __ARRAY_COPY__0 < 64; __ARRAY_COPY__0++)
            (quantizationTable__231__7)[__ARRAY_COPY__0] = _param_quantizationTable__236[__ARRAY_COPY__0];
    }
    ;
}
void save_file_pointer__7(object_write_buffer *buf) {}
void load_file_pointer__7(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_staticQuantization__237_131__7__mod(int ____n, float *____in, int *____out) {
    for (; (0 < ____n); ____n--)
    {
        float val__234 = 0.0f;/* float */
        int i__235 = 0;/* int */

        // mark begin: SIRFilter staticQuantization

        for ((i__235 = 0)/*int*/; (i__235 < 64); (i__235++)) {{
            (val__234 = ((*____in++) / ((float)(((quantizationTable__231__7)[(int)i__235])))))/*float*/;
            ((*____out++)=((int)(((float)(floorf((((float)0.5) + val__234)))))));
        }
        }
        // mark end: SIRFilter staticQuantization

    }}


void work_staticQuantization__237_131__7__mod2(int ____n, float *____in, int *____out, int s1, int s2) {
    for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
    {
        float val__234 = 0.0f;/* float */
        int i__235 = 0;/* int */

        // mark begin: SIRFilter staticQuantization

        for ((i__235 = 0)/*int*/; (i__235 < 64); (i__235++)) {{
            (val__234 = ((*____in++) / ((float)(((quantizationTable__231__7)[(int)i__235])))))/*float*/;
            ((*____out++)=((int)(((float)(floorf((((float)0.5) + val__234)))))));
        }
        }
        // mark end: SIRFilter staticQuantization

    }}


#endif // BUFFER_MERGE



void work_staticQuantization__237_131__7(int ____n){
    for (
            ; (0 < ____n); (____n--)) {{
        float val__234 = 0.0f;/* float */
        int i__235 = 0;/* int */

        // mark begin: SIRFilter staticQuantization

        for ((i__235 = 0)/*int*/; (i__235 < 64); (i__235++)) {{
            (val__234 = (__pop__7() / ((float)(((quantizationTable__231__7)[(int)i__235])))))/*float*/;
            __push__7(((int)(((float)(floorf((((float)0.5) + val__234)))))));
        }
        }
        // mark end: SIRFilter staticQuantization

    }
    }
}

// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_8;
int __counter_8 = 0;
int __steady_8 = 0;
int __tmp_8 = 0;
int __tmp2_8 = 0;
int *__state_flag_8 = NULL;
thread_info *__thread_8 = NULL;




void __splitter_8_work(int ____n) {
    for (;____n > 0; ____n--) {
        BUFFER_8_9[HEAD_8_9]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_9++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
        BUFFER_8_35[HEAD_8_35]=BUFFER_7_8[TAIL_7_8];
        HEAD_8_35++; ; TAIL_7_8++;
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



//delay = 1

int __Init_Path_9(int i){
    if ((i == 0)) {return 0;}

    return 0;
}


void __feedbackjoiner_9_prep() {
    for (int __i = 0; __i < 1; __i++) {
        BUFFER_34_9[HEAD_34_9] = __Init_Path_9(__i); HEAD_34_9++;
    }
}

void __joiner_9_work(int ____n) {
    for (;____n > 0; ____n--) {
        BUFFER_9_10[HEAD_9_10]=BUFFER_8_9[TAIL_8_9];
        HEAD_9_10++; ; TAIL_8_9++;
        BUFFER_9_10[HEAD_9_10]=BUFFER_34_9[TAIL_34_9];
        HEAD_9_10++; ; TAIL_34_9++;
    }
}


// peek: 2 pop: 2 push 2
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


void init_AnonFilter_a0__240_135__10();
inline void check_status__10();

void work_AnonFilter_a0__240_135__10(int);


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




void init_AnonFilter_a0__240_135__10(){
}
void save_file_pointer__10(object_write_buffer *buf) {}
void load_file_pointer__10(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__240_135__10__mod(int ____n, int *____in, int *____out) {
    for (; (0 < ____n); ____n--)
    {
        // mark begin: SIRFilter AnonFilter_a0

        ((*____out++)=((*(____in+0)) - (*(____in+1))));
        ((*____out++)=(*(____in+0)));
        assert(false);__pop__10(2);
        // mark end: SIRFilter AnonFilter_a0

    }}


void work_AnonFilter_a0__240_135__10__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
    for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
    {
        // mark begin: SIRFilter AnonFilter_a0

        ((*____out++)=((*(____in+0)) - (*(____in+1))));
        ((*____out++)=(*(____in+0)));
        assert(false);__pop__10(2);
        // mark end: SIRFilter AnonFilter_a0

    }}


#endif // BUFFER_MERGE



void work_AnonFilter_a0__240_135__10(int ____n){
    for (
            ; (0 < ____n); (____n--)) {{
        // mark begin: SIRFilter AnonFilter_a0

        __push__10((__peek__10(0) - __peek__10(1)));
        __push__10(__peek__10(0));
        __pop__10(2);
        // mark end: SIRFilter AnonFilter_a0

    }
    }
}

// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_11;
int __counter_11 = 0;
int __steady_11 = 0;
int __tmp_11 = 0;
int __tmp2_11 = 0;
int *__state_flag_11 = NULL;
thread_info *__thread_11 = NULL;




void __splitter_11_work(int ____n) {
    for (;____n > 0; ____n--) {
        BUFFER_11_12[HEAD_11_12]=BUFFER_10_11[TAIL_10_11];
        HEAD_11_12++; ; TAIL_10_11++;
        BUFFER_11_34[HEAD_11_34]=BUFFER_10_11[TAIL_10_11];
        HEAD_11_34++; ; TAIL_10_11++;
    }
}


// peek: 1 pop: 1 push 1
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


void init_IDforNothing__246_137__12();
inline void check_status__12();

void work_IDforNothing__246_137__12(int);


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



inline void __push__12(int data) {
    BUFFER_12_13[HEAD_12_13]=data;
    HEAD_12_13++;
}




void init_IDforNothing__246_137__12(){
}
void save_file_pointer__12(object_write_buffer *buf) {}
void load_file_pointer__12(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_IDforNothing__246_137__12__mod(int ____n, int *____in, int *____out) {
    for (; (0 < ____n); ____n--)
    {
        // mark begin: SIRFilter IDforNothing

        ((*____out++)=(*____in++));
        // mark end: SIRFilter IDforNothing

    }}


void work_IDforNothing__246_137__12__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
    for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
    {
        // mark begin: SIRFilter IDforNothing

        ((*____out++)=(*____in++));
        // mark end: SIRFilter IDforNothing

    }}


#endif // BUFFER_MERGE



void work_IDforNothing__246_137__12(int ____n){
    for (
            ; (0 < ____n); (____n--)) {{
        // mark begin: SIRFilter IDforNothing

        __push__12(__pop__12());
        // mark end: SIRFilter IDforNothing

    }
    }
}

// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_13;
int __counter_13 = 0;
int __steady_13 = 0;
int __tmp_13 = 0;
int __tmp2_13 = 0;
int *__state_flag_13 = NULL;
thread_info *__thread_13 = NULL;




void __joiner_13_work(int ____n) {
    for (;____n > 0; ____n--) {
        BUFFER_13_14[HEAD_13_14]=BUFFER_12_13[TAIL_12_13];
        HEAD_13_14++; ; TAIL_12_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
        BUFFER_13_14[HEAD_13_14]=BUFFER_35_13[TAIL_35_13];
        HEAD_13_14++; ; TAIL_35_13++;
    }
}


// peek: 64 pop: 64 push 64
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_14;
int __counter_14 = 0;
int __steady_14 = 0;
int __tmp_14 = 0;
int __tmp2_14 = 0;
int *__state_flag_14 = NULL;
thread_info *__thread_14 = NULL;



int Ordering__250__14[64] = {0, 1, 8, 16, 9, 2, 3, 10, 17, 24, 32, 25, 18, 11, 4, 5, 12, 19, 26, 33, 40, 48, 41, 34, 27, 20, 13, 6, 7, 14, 21, 28, 35, 42, 49, 56, 57, 50, 43, 36, 29, 22, 15, 23, 30, 37, 44, 51, 58, 59, 52, 45, 38, 31, 39, 46, 53, 60, 61, 54, 47, 55, 62, 63};
void save_peek_buffer__14(object_write_buffer *buf);
void load_peek_buffer__14(object_write_buffer *buf);
void save_file_pointer__14(object_write_buffer *buf);
void load_file_pointer__14(object_write_buffer *buf);


void init_ZigZagOrdering__254_139__14();
inline void check_status__14();

void work_ZigZagOrdering__254_139__14(int);


inline int __pop__14() {
    int res=BUFFER_13_14[TAIL_13_14];
    TAIL_13_14++;
    return res;
}

inline int __pop__14(int n) {
    int res=BUFFER_13_14[TAIL_13_14];
    TAIL_13_14+=n;

    return res;
}

inline int __peek__14(int offs) {
    return BUFFER_13_14[TAIL_13_14+offs];
}



inline void __push__14(int data) {
    BUFFER_14_15[HEAD_14_15]=data;
    HEAD_14_15++;
}




void init_ZigZagOrdering__254_139__14(){
}
void save_file_pointer__14(object_write_buffer *buf) {}
void load_file_pointer__14(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_ZigZagOrdering__254_139__14__mod(int ____n, int *____in, int *____out) {
    for (; (0 < ____n); ____n--)
    {
        int i__253 = 0;/* int */

        // mark begin: SIRFilter ZigZagOrdering

        for ((i__253 = 0)/*int*/; (i__253 < 64); (i__253++)) {{
            ((*____out++)=(*(____in+((Ordering__250__14)[(int)i__253]))));
        }
        }
        assert(false);__pop__14(64);
        // mark end: SIRFilter ZigZagOrdering

    }}


void work_ZigZagOrdering__254_139__14__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
    for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
    {
        int i__253 = 0;/* int */

        // mark begin: SIRFilter ZigZagOrdering

        for ((i__253 = 0)/*int*/; (i__253 < 64); (i__253++)) {{
            ((*____out++)=(*(____in+((Ordering__250__14)[(int)i__253]))));
        }
        }
        assert(false);__pop__14(64);
        // mark end: SIRFilter ZigZagOrdering

    }}


#endif // BUFFER_MERGE



void work_ZigZagOrdering__254_139__14(int ____n){
    for (
            ; (0 < ____n); (____n--)) {{
        int i__253 = 0;/* int */

        // mark begin: SIRFilter ZigZagOrdering

        for ((i__253 = 0)/*int*/; (i__253 < 64); (i__253++)) {{
            __push__14(__peek__14(((Ordering__250__14)[(int)i__253])));
        }
        }
        __pop__14(64);
        // mark end: SIRFilter ZigZagOrdering

    }
    }
}

// peek: 64 pop: 64 push 64
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


void init_RunLengthEncoder__262_140__15();
inline void check_status__15();

void work_RunLengthEncoder__262_140__15(int);


inline int __pop__15() {
    int res=BUFFER_14_15[TAIL_14_15];
    TAIL_14_15++;
    return res;
}

inline int __pop__15(int n) {
    int res=BUFFER_14_15[TAIL_14_15];
    TAIL_14_15+=n;

    return res;
}

inline int __peek__15(int offs) {
    return BUFFER_14_15[TAIL_14_15+offs];
}



inline void __push__15(int data) {
    BUFFER_15_16[HEAD_15_16]=data;
    HEAD_15_16++;
}




void init_RunLengthEncoder__262_140__15(){
}
void save_file_pointer__15(object_write_buffer *buf) {}
void load_file_pointer__15(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_RunLengthEncoder__262_140__15__mod(int ____n, int *____in, int *____out) {
    for (; (0 < ____n); ____n--)
    {
        int lastInt__257 = 0;/* int */
        int count__258 = 0;/* int */
        int starting__259;/* boolean */
        int curInt__260 = 0;/* int */
        int i__261 = 0;/* int */

        // mark begin: SIRFilter RunLengthEncoder

        (lastInt__257 = 0)/*int*/;
        (count__258 = 0)/*int*/;
        (starting__259 = 1)/*boolean*/;
        for ((i__261 = 0)/*int*/; (i__261 < 64); (i__261++)) {{
            (curInt__260 = (*(____in+i__261)))/*int*/;
            if (starting__259) {{
                (lastInt__257 = curInt__260)/*int*/;
                (count__258 = 1)/*int*/;
                (starting__259 = 0)/*boolean*/;
            } } else {if ((curInt__260 == lastInt__257)) {(count__258++); } else {{
                ((*____out++)=count__258);
                ((*____out++)=lastInt__257);
                (lastInt__257 = curInt__260)/*int*/;
                (count__258 = 1)/*int*/;
            }}}
        }
        }
        ((*____out++)=count__258);
        ((*____out++)=lastInt__257);
        assert(false);__pop__15(64);
        // mark end: SIRFilter RunLengthEncoder

    }}


void work_RunLengthEncoder__262_140__15__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
    for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
    {
        int lastInt__257 = 0;/* int */
        int count__258 = 0;/* int */
        int starting__259;/* boolean */
        int curInt__260 = 0;/* int */
        int i__261 = 0;/* int */

        // mark begin: SIRFilter RunLengthEncoder

        (lastInt__257 = 0)/*int*/;
        (count__258 = 0)/*int*/;
        (starting__259 = 1)/*boolean*/;
        for ((i__261 = 0)/*int*/; (i__261 < 64); (i__261++)) {{
            (curInt__260 = (*(____in+i__261)))/*int*/;
            if (starting__259) {{
                (lastInt__257 = curInt__260)/*int*/;
                (count__258 = 1)/*int*/;
                (starting__259 = 0)/*boolean*/;
            } } else {if ((curInt__260 == lastInt__257)) {(count__258++); } else {{
                ((*____out++)=count__258);
                ((*____out++)=lastInt__257);
                (lastInt__257 = curInt__260)/*int*/;
                (count__258 = 1)/*int*/;
            }}}
        }
        }
        ((*____out++)=count__258);
        ((*____out++)=lastInt__257);
        assert(false);__pop__15(64);
        // mark end: SIRFilter RunLengthEncoder

    }}


#endif // BUFFER_MERGE



void work_RunLengthEncoder__262_140__15(int ____n){
    for (
            ; (0 < ____n); (____n--)) {{
        int lastInt__257 = 0;/* int */
        int count__258 = 0;/* int */
        int starting__259;/* boolean */
        int curInt__260 = 0;/* int */
        int i__261 = 0;/* int */
        int push_counter = 0;
        int i = 0;

        // mark begin: SIRFilter RunLengthEncoder

        (lastInt__257 = 0)/*int*/;
        (count__258 = 0)/*int*/;
        (starting__259 = 1)/*boolean*/;
        for ((i__261 = 0)/*int*/; (i__261 < 64); (i__261++)) {{
            (curInt__260 = __peek__15(i__261))/*int*/;
            if (starting__259) {{
                (lastInt__257 = curInt__260)/*int*/;
                (count__258 = 1)/*int*/;
                (starting__259 = 0)/*boolean*/;
            } } else {if ((curInt__260 == lastInt__257)) {(count__258++); } else {{
                __push__15(count__258);
                push_counter++;
                __push__15(lastInt__257);
                push_counter++;
                (lastInt__257 = curInt__260)/*int*/;
                (count__258 = 1)/*int*/;
            }}}
        }
        }
        __push__15(count__258);
        push_counter++;
        __push__15(lastInt__257);
        push_counter++;
        for(i = 0; i < 64-push_counter+1; i++){
            __push__15(0);
        }
        __pop__15(64);
        // mark end: SIRFilter RunLengthEncoder

    }
    }
}

// peek: 2 pop: 2 push 2
// init counts: 0 steady counts: 32

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


void init_RunLengthDecoder__268_141__16();
inline void check_status__16();

void work_RunLengthDecoder__268_141__16(int);


inline int __pop__16() {
    int res=BUFFER_15_16[TAIL_15_16];
    TAIL_15_16++;
    return res;
}

inline int __pop__16(int n) {
    int res=BUFFER_15_16[TAIL_15_16];
    TAIL_15_16+=n;

    return res;
}

inline int __peek__16(int offs) {
    return BUFFER_15_16[TAIL_15_16+offs];
}



inline void __push__16(int data) {
    BUFFER_16_17[HEAD_16_17]=data;
    HEAD_16_17++;
}




void init_RunLengthDecoder__268_141__16(){
}
void save_file_pointer__16(object_write_buffer *buf) {}
void load_file_pointer__16(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_RunLengthDecoder__268_141__16__mod(int ____n, int *____in, int *____out) {
    for (; (0 < ____n); ____n--)
    {
        int count__265 = 0;/* int */
        int curInt__266 = 0;/* int */
        int i__267 = 0;/* int */

        // mark begin: SIRFilter RunLengthDecoder

        (count__265 = (*____in++))/*int*/;
        (curInt__266 = (*____in++))/*int*/;
        for ((i__267 = 0)/*int*/; (i__267 < count__265); (i__267++)) {{
            ((*____out++)=curInt__266);
        }
        }
        // mark end: SIRFilter RunLengthDecoder

    }}


void work_RunLengthDecoder__268_141__16__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
    for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
    {
        int count__265 = 0;/* int */
        int curInt__266 = 0;/* int */
        int i__267 = 0;/* int */

        // mark begin: SIRFilter RunLengthDecoder

        (count__265 = (*____in++))/*int*/;
        (curInt__266 = (*____in++))/*int*/;
        for ((i__267 = 0)/*int*/; (i__267 < count__265); (i__267++)) {{
            ((*____out++)=curInt__266);
        }
        }
        // mark end: SIRFilter RunLengthDecoder

    }}


#endif // BUFFER_MERGE



void work_RunLengthDecoder__268_141__16(int ____n){
    for (
            ; (0 < ____n); (____n--)) {{
        int count__265 = 0;/* int */
        int curInt__266 = 0;/* int */
        int i__267 = 0;/* int */

        // mark begin: SIRFilter RunLengthDecoder

        (count__265 = __pop__16())/*int*/;
        (curInt__266 = __pop__16())/*int*/;
        for ((i__267 = 0)/*int*/; (i__267 < count__265); (i__267++)) {{
            __push__16(curInt__266);
        }
        }
        // mark end: SIRFilter RunLengthDecoder

    }
    }
}

// peek: 64 pop: 64 push 64
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_17;
int __counter_17 = 0;
int __steady_17 = 0;
int __tmp_17 = 0;
int __tmp2_17 = 0;
int *__state_flag_17 = NULL;
thread_info *__thread_17 = NULL;



int Ordering__269__17[64] = {0, 1, 5, 6, 14, 15, 27, 28, 2, 4, 7, 13, 16, 26, 29, 42, 3, 8, 12, 17, 25, 30, 41, 43, 9, 11, 18, 24, 31, 40, 44, 53, 10, 19, 23, 32, 39, 45, 52, 54, 20, 22, 33, 38, 46, 51, 55, 60, 21, 34, 37, 47, 50, 56, 59, 61, 35, 36, 48, 49, 57, 58, 62, 63};
void save_peek_buffer__17(object_write_buffer *buf);
void load_peek_buffer__17(object_write_buffer *buf);
void save_file_pointer__17(object_write_buffer *buf);
void load_file_pointer__17(object_write_buffer *buf);


void init_ZigZagUnordering__273_142__17();
inline void check_status__17();

void work_ZigZagUnordering__273_142__17(int);


inline int __pop__17() {
    int res=BUFFER_16_17[TAIL_16_17];
    TAIL_16_17++;
    return res;
}

inline int __pop__17(int n) {
    int res=BUFFER_16_17[TAIL_16_17];
    TAIL_16_17+=n;

    return res;
}

inline int __peek__17(int offs) {
    return BUFFER_16_17[TAIL_16_17+offs];
}



inline void __push__17(int data) {
    BUFFER_17_18[HEAD_17_18]=data;
    HEAD_17_18++;
}




void init_ZigZagUnordering__273_142__17(){
}
void save_file_pointer__17(object_write_buffer *buf) {}
void load_file_pointer__17(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_ZigZagUnordering__273_142__17__mod(int ____n, int *____in, int *____out) {
    for (; (0 < ____n); ____n--)
    {
        int i__272 = 0;/* int */

        // mark begin: SIRFilter ZigZagUnordering

        for ((i__272 = 0)/*int*/; (i__272 < 64); (i__272++)) {{
            ((*____out++)=(*(____in+((Ordering__269__17)[(int)i__272]))));
        }
        }
        assert(false);__pop__17(64);
        // mark end: SIRFilter ZigZagUnordering

    }}


void work_ZigZagUnordering__273_142__17__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
    for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
    {
        int i__272 = 0;/* int */

        // mark begin: SIRFilter ZigZagUnordering

        for ((i__272 = 0)/*int*/; (i__272 < 64); (i__272++)) {{
            ((*____out++)=(*(____in+((Ordering__269__17)[(int)i__272]))));
        }
        }
        assert(false);__pop__17(64);
        // mark end: SIRFilter ZigZagUnordering

    }}


#endif // BUFFER_MERGE



void work_ZigZagUnordering__273_142__17(int ____n){
    for (
            ; (0 < ____n); (____n--)) {{
        int i__272 = 0;/* int */

        // mark begin: SIRFilter ZigZagUnordering

        for ((i__272 = 0)/*int*/; (i__272 < 64); (i__272++)) {{
            __push__17(__peek__17(((Ordering__269__17)[(int)i__272])));
        }
        }
        __pop__17(64);
        // mark end: SIRFilter ZigZagUnordering

    }
    }
}

// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_18;
int __counter_18 = 0;
int __steady_18 = 0;
int __tmp_18 = 0;
int __tmp2_18 = 0;
int *__state_flag_18 = NULL;
thread_info *__thread_18 = NULL;




void __splitter_18_work(int ____n) {
    for (;____n > 0; ____n--) {
        BUFFER_18_19[HEAD_18_19]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_19++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
        BUFFER_18_33[HEAD_18_33]=BUFFER_17_18[TAIL_17_18];
        HEAD_18_33++; ; TAIL_17_18++;
    }
}


// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_19;
int __counter_19 = 0;
int __steady_19 = 0;
int __tmp_19 = 0;
int __tmp2_19 = 0;
int *__state_flag_19 = NULL;
thread_info *__thread_19 = NULL;



//delay = 1

int __Init_Path_19(int i){
    if ((i == 0)) {return 0;}

    return 0;
}


void __feedbackjoiner_19_prep() {
    for (int __i = 0; __i < 1; __i++) {
        BUFFER_32_19[HEAD_32_19] = __Init_Path_19(__i); HEAD_32_19++;
    }
}

void __joiner_19_work(int ____n) {
    for (;____n > 0; ____n--) {
        BUFFER_19_20[HEAD_19_20]=BUFFER_18_19[TAIL_18_19];
        HEAD_19_20++; ; TAIL_18_19++;
        BUFFER_19_20[HEAD_19_20]=BUFFER_32_19[TAIL_32_19];
        HEAD_19_20++; ; TAIL_32_19++;
    }
}


// peek: 2 pop: 2 push 2
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


void init_AnonFilter_a1__276_146__20();
inline void check_status__20();

void work_AnonFilter_a1__276_146__20(int);


inline int __pop__20() {
    int res=BUFFER_19_20[TAIL_19_20];
    TAIL_19_20++;
    return res;
}

inline int __pop__20(int n) {
    int res=BUFFER_19_20[TAIL_19_20];
    TAIL_19_20+=n;

    return res;
}

inline int __peek__20(int offs) {
    return BUFFER_19_20[TAIL_19_20+offs];
}



inline void __push__20(int data) {
    BUFFER_20_21[HEAD_20_21]=data;
    HEAD_20_21++;
}




void init_AnonFilter_a1__276_146__20(){
}
void save_file_pointer__20(object_write_buffer *buf) {}
void load_file_pointer__20(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a1__276_146__20__mod(int ____n, int *____in, int *____out) {
    for (; (0 < ____n); ____n--)
    {
        // mark begin: SIRFilter AnonFilter_a1

        ((*____out++)=((*(____in+0)) + (*(____in+1))));
        ((*____out++)=(*(____in+1)));
        assert(false);__pop__20(2);
        // mark end: SIRFilter AnonFilter_a1

    }}


void work_AnonFilter_a1__276_146__20__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
    for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
    {
        // mark begin: SIRFilter AnonFilter_a1

        ((*____out++)=((*(____in+0)) + (*(____in+1))));
        ((*____out++)=(*(____in+1)));
        assert(false);__pop__20(2);
        // mark end: SIRFilter AnonFilter_a1

    }}


#endif // BUFFER_MERGE



void work_AnonFilter_a1__276_146__20(int ____n){
    for (
            ; (0 < ____n); (____n--)) {{
        // mark begin: SIRFilter AnonFilter_a1

        __push__20((__peek__20(0) + __peek__20(1)));
        __push__20(__peek__20(1));
        __pop__20(2);
        // mark end: SIRFilter AnonFilter_a1

    }
    }
}

// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_21;
int __counter_21 = 0;
int __steady_21 = 0;
int __tmp_21 = 0;
int __tmp2_21 = 0;
int *__state_flag_21 = NULL;
thread_info *__thread_21 = NULL;




void __splitter_21_work(int ____n) {
    for (;____n > 0; ____n--) {
        BUFFER_21_22[HEAD_21_22]=BUFFER_20_21[TAIL_20_21];
        HEAD_21_22++; ; TAIL_20_21++;
        BUFFER_21_32[HEAD_21_32]=BUFFER_20_21[TAIL_20_21];
        HEAD_21_32++; ; TAIL_20_21++;
    }
}


// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_22;
int __counter_22 = 0;
int __steady_22 = 0;
int __tmp_22 = 0;
int __tmp2_22 = 0;
int *__state_flag_22 = NULL;
thread_info *__thread_22 = NULL;



void save_peek_buffer__22(object_write_buffer *buf);
void load_peek_buffer__22(object_write_buffer *buf);
void save_file_pointer__22(object_write_buffer *buf);
void load_file_pointer__22(object_write_buffer *buf);


void init_IDforNothing__282_148__22();
inline void check_status__22();

void work_IDforNothing__282_148__22(int);


inline int __pop__22() {
    int res=BUFFER_21_22[TAIL_21_22];
    TAIL_21_22++;
    return res;
}

inline int __pop__22(int n) {
    int res=BUFFER_21_22[TAIL_21_22];
    TAIL_21_22+=n;

    return res;
}

inline int __peek__22(int offs) {
    return BUFFER_21_22[TAIL_21_22+offs];
}



inline void __push__22(int data) {
    BUFFER_22_23[HEAD_22_23]=data;
    HEAD_22_23++;
}




void init_IDforNothing__282_148__22(){
}
void save_file_pointer__22(object_write_buffer *buf) {}
void load_file_pointer__22(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_IDforNothing__282_148__22__mod(int ____n, int *____in, int *____out) {
    for (; (0 < ____n); ____n--)
    {
        // mark begin: SIRFilter IDforNothing

        ((*____out++)=(*____in++));
        // mark end: SIRFilter IDforNothing

    }}


void work_IDforNothing__282_148__22__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
    for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
    {
        // mark begin: SIRFilter IDforNothing

        ((*____out++)=(*____in++));
        // mark end: SIRFilter IDforNothing

    }}


#endif // BUFFER_MERGE



void work_IDforNothing__282_148__22(int ____n){
    for (
            ; (0 < ____n); (____n--)) {{
        // mark begin: SIRFilter IDforNothing

        __push__22(__pop__22());
        // mark end: SIRFilter IDforNothing

    }
    }
}

// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_23;
int __counter_23 = 0;
int __steady_23 = 0;
int __tmp_23 = 0;
int __tmp2_23 = 0;
int *__state_flag_23 = NULL;
thread_info *__thread_23 = NULL;




void __joiner_23_work(int ____n) {
    for (;____n > 0; ____n--) {
        BUFFER_23_24[HEAD_23_24]=BUFFER_22_23[TAIL_22_23];
        HEAD_23_24++; ; TAIL_22_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
        BUFFER_23_24[HEAD_23_24]=BUFFER_33_23[TAIL_33_23];
        HEAD_23_24++; ; TAIL_33_23++;
    }
}


// peek: 64 pop: 64 push 64
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_24;
int __counter_24 = 0;
int __steady_24 = 0;
int __tmp_24 = 0;
int __tmp2_24 = 0;
int *__state_flag_24 = NULL;
thread_info *__thread_24 = NULL;



int quantizationTable__286__24[64] = {0};
void save_peek_buffer__24(object_write_buffer *buf);
void load_peek_buffer__24(object_write_buffer *buf);
void save_file_pointer__24(object_write_buffer *buf);
void load_file_pointer__24(object_write_buffer *buf);


void init_staticDeQuantization__292_151__24();
inline void check_status__24();

void work_staticDeQuantization__292_151__24(int);


inline int __pop__24() {
    int res=BUFFER_23_24[TAIL_23_24];
    TAIL_23_24++;
    return res;
}

inline int __pop__24(int n) {
    int res=BUFFER_23_24[TAIL_23_24];
    TAIL_23_24+=n;

    return res;
}

inline int __peek__24(int offs) {
    return BUFFER_23_24[TAIL_23_24+offs];
}



inline void __push__24(float data) {
    BUFFER_24_25[HEAD_24_25]=data;
    HEAD_24_25++;
}




void init_staticDeQuantization__292_151__24(){
    int _param_quantizationTable__291[64] = {0};/* int[64] */

    ((_param_quantizationTable__291[(int)0]) = 16)/*int*/;
    ((_param_quantizationTable__291[(int)1]) = 11)/*int*/;
    ((_param_quantizationTable__291[(int)2]) = 10)/*int*/;
    ((_param_quantizationTable__291[(int)3]) = 16)/*int*/;
    ((_param_quantizationTable__291[(int)4]) = 24)/*int*/;
    ((_param_quantizationTable__291[(int)5]) = 40)/*int*/;
    ((_param_quantizationTable__291[(int)6]) = 51)/*int*/;
    ((_param_quantizationTable__291[(int)7]) = 61)/*int*/;
    ((_param_quantizationTable__291[(int)8]) = 12)/*int*/;
    ((_param_quantizationTable__291[(int)9]) = 12)/*int*/;
    ((_param_quantizationTable__291[(int)10]) = 14)/*int*/;
    ((_param_quantizationTable__291[(int)11]) = 19)/*int*/;
    ((_param_quantizationTable__291[(int)12]) = 26)/*int*/;
    ((_param_quantizationTable__291[(int)13]) = 58)/*int*/;
    ((_param_quantizationTable__291[(int)14]) = 60)/*int*/;
    ((_param_quantizationTable__291[(int)15]) = 55)/*int*/;
    ((_param_quantizationTable__291[(int)16]) = 14)/*int*/;
    ((_param_quantizationTable__291[(int)17]) = 13)/*int*/;
    ((_param_quantizationTable__291[(int)18]) = 16)/*int*/;
    ((_param_quantizationTable__291[(int)19]) = 24)/*int*/;
    ((_param_quantizationTable__291[(int)20]) = 40)/*int*/;
    ((_param_quantizationTable__291[(int)21]) = 57)/*int*/;
    ((_param_quantizationTable__291[(int)22]) = 69)/*int*/;
    ((_param_quantizationTable__291[(int)23]) = 56)/*int*/;
    ((_param_quantizationTable__291[(int)24]) = 14)/*int*/;
    ((_param_quantizationTable__291[(int)25]) = 17)/*int*/;
    ((_param_quantizationTable__291[(int)26]) = 22)/*int*/;
    ((_param_quantizationTable__291[(int)27]) = 29)/*int*/;
    ((_param_quantizationTable__291[(int)28]) = 51)/*int*/;
    ((_param_quantizationTable__291[(int)29]) = 87)/*int*/;
    ((_param_quantizationTable__291[(int)30]) = 80)/*int*/;
    ((_param_quantizationTable__291[(int)31]) = 62)/*int*/;
    ((_param_quantizationTable__291[(int)32]) = 18)/*int*/;
    ((_param_quantizationTable__291[(int)33]) = 22)/*int*/;
    ((_param_quantizationTable__291[(int)34]) = 37)/*int*/;
    ((_param_quantizationTable__291[(int)35]) = 56)/*int*/;
    ((_param_quantizationTable__291[(int)36]) = 68)/*int*/;
    ((_param_quantizationTable__291[(int)37]) = 109)/*int*/;
    ((_param_quantizationTable__291[(int)38]) = 103)/*int*/;
    ((_param_quantizationTable__291[(int)39]) = 77)/*int*/;
    ((_param_quantizationTable__291[(int)40]) = 24)/*int*/;
    ((_param_quantizationTable__291[(int)41]) = 35)/*int*/;
    ((_param_quantizationTable__291[(int)42]) = 55)/*int*/;
    ((_param_quantizationTable__291[(int)43]) = 64)/*int*/;
    ((_param_quantizationTable__291[(int)44]) = 81)/*int*/;
    ((_param_quantizationTable__291[(int)45]) = 104)/*int*/;
    ((_param_quantizationTable__291[(int)46]) = 113)/*int*/;
    ((_param_quantizationTable__291[(int)47]) = 92)/*int*/;
    ((_param_quantizationTable__291[(int)48]) = 49)/*int*/;
    ((_param_quantizationTable__291[(int)49]) = 64)/*int*/;
    ((_param_quantizationTable__291[(int)50]) = 78)/*int*/;
    ((_param_quantizationTable__291[(int)51]) = 87)/*int*/;
    ((_param_quantizationTable__291[(int)52]) = 103)/*int*/;
    ((_param_quantizationTable__291[(int)53]) = 121)/*int*/;
    ((_param_quantizationTable__291[(int)54]) = 120)/*int*/;
    ((_param_quantizationTable__291[(int)55]) = 101)/*int*/;
    ((_param_quantizationTable__291[(int)56]) = 72)/*int*/;
    ((_param_quantizationTable__291[(int)57]) = 92)/*int*/;
    ((_param_quantizationTable__291[(int)58]) = 95)/*int*/;
    ((_param_quantizationTable__291[(int)59]) = 98)/*int*/;
    ((_param_quantizationTable__291[(int)60]) = 112)/*int*/;
    ((_param_quantizationTable__291[(int)61]) = 100)/*int*/;
    ((_param_quantizationTable__291[(int)62]) = 103)/*int*/;
    ((_param_quantizationTable__291[(int)63]) = 99)/*int*/;
    ((_param_quantizationTable__291[(int)0]) = 16)/*int*/;
    ((_param_quantizationTable__291[(int)1]) = 11)/*int*/;
    ((_param_quantizationTable__291[(int)2]) = 10)/*int*/;
    ((_param_quantizationTable__291[(int)3]) = 16)/*int*/;
    ((_param_quantizationTable__291[(int)4]) = 24)/*int*/;
    ((_param_quantizationTable__291[(int)5]) = 40)/*int*/;
    ((_param_quantizationTable__291[(int)6]) = 51)/*int*/;
    ((_param_quantizationTable__291[(int)7]) = 61)/*int*/;
    ((_param_quantizationTable__291[(int)8]) = 12)/*int*/;
    ((_param_quantizationTable__291[(int)9]) = 12)/*int*/;
    ((_param_quantizationTable__291[(int)10]) = 14)/*int*/;
    ((_param_quantizationTable__291[(int)11]) = 19)/*int*/;
    ((_param_quantizationTable__291[(int)12]) = 26)/*int*/;
    ((_param_quantizationTable__291[(int)13]) = 58)/*int*/;
    ((_param_quantizationTable__291[(int)14]) = 60)/*int*/;
    ((_param_quantizationTable__291[(int)15]) = 55)/*int*/;
    ((_param_quantizationTable__291[(int)16]) = 14)/*int*/;
    ((_param_quantizationTable__291[(int)17]) = 13)/*int*/;
    ((_param_quantizationTable__291[(int)18]) = 16)/*int*/;
    ((_param_quantizationTable__291[(int)19]) = 24)/*int*/;
    ((_param_quantizationTable__291[(int)20]) = 40)/*int*/;
    ((_param_quantizationTable__291[(int)21]) = 57)/*int*/;
    ((_param_quantizationTable__291[(int)22]) = 69)/*int*/;
    ((_param_quantizationTable__291[(int)23]) = 56)/*int*/;
    ((_param_quantizationTable__291[(int)24]) = 14)/*int*/;
    ((_param_quantizationTable__291[(int)25]) = 17)/*int*/;
    ((_param_quantizationTable__291[(int)26]) = 22)/*int*/;
    ((_param_quantizationTable__291[(int)27]) = 29)/*int*/;
    ((_param_quantizationTable__291[(int)28]) = 51)/*int*/;
    ((_param_quantizationTable__291[(int)29]) = 87)/*int*/;
    ((_param_quantizationTable__291[(int)30]) = 80)/*int*/;
    ((_param_quantizationTable__291[(int)31]) = 62)/*int*/;
    ((_param_quantizationTable__291[(int)32]) = 18)/*int*/;
    ((_param_quantizationTable__291[(int)33]) = 22)/*int*/;
    ((_param_quantizationTable__291[(int)34]) = 37)/*int*/;
    ((_param_quantizationTable__291[(int)35]) = 56)/*int*/;
    ((_param_quantizationTable__291[(int)36]) = 68)/*int*/;
    ((_param_quantizationTable__291[(int)37]) = 109)/*int*/;
    ((_param_quantizationTable__291[(int)38]) = 103)/*int*/;
    ((_param_quantizationTable__291[(int)39]) = 77)/*int*/;
    ((_param_quantizationTable__291[(int)40]) = 24)/*int*/;
    ((_param_quantizationTable__291[(int)41]) = 35)/*int*/;
    ((_param_quantizationTable__291[(int)42]) = 55)/*int*/;
    ((_param_quantizationTable__291[(int)43]) = 64)/*int*/;
    ((_param_quantizationTable__291[(int)44]) = 81)/*int*/;
    ((_param_quantizationTable__291[(int)45]) = 104)/*int*/;
    ((_param_quantizationTable__291[(int)46]) = 113)/*int*/;
    ((_param_quantizationTable__291[(int)47]) = 92)/*int*/;
    ((_param_quantizationTable__291[(int)48]) = 49)/*int*/;
    ((_param_quantizationTable__291[(int)49]) = 64)/*int*/;
    ((_param_quantizationTable__291[(int)50]) = 78)/*int*/;
    ((_param_quantizationTable__291[(int)51]) = 87)/*int*/;
    ((_param_quantizationTable__291[(int)52]) = 103)/*int*/;
    ((_param_quantizationTable__291[(int)53]) = 121)/*int*/;
    ((_param_quantizationTable__291[(int)54]) = 120)/*int*/;
    ((_param_quantizationTable__291[(int)55]) = 101)/*int*/;
    ((_param_quantizationTable__291[(int)56]) = 72)/*int*/;
    ((_param_quantizationTable__291[(int)57]) = 92)/*int*/;
    ((_param_quantizationTable__291[(int)58]) = 95)/*int*/;
    ((_param_quantizationTable__291[(int)59]) = 98)/*int*/;
    ((_param_quantizationTable__291[(int)60]) = 112)/*int*/;
    ((_param_quantizationTable__291[(int)61]) = 100)/*int*/;
    ((_param_quantizationTable__291[(int)62]) = 103)/*int*/;
    ((_param_quantizationTable__291[(int)63]) = 99)/*int*/;
    {
        int __ARRAY_COPY__0;
        for (__ARRAY_COPY__0 = 0; __ARRAY_COPY__0 < 64; __ARRAY_COPY__0++)
            (quantizationTable__286__24)[__ARRAY_COPY__0] = _param_quantizationTable__291[__ARRAY_COPY__0];
    }
    ;
}
void save_file_pointer__24(object_write_buffer *buf) {}
void load_file_pointer__24(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_staticDeQuantization__292_151__24__mod(int ____n, int *____in, float *____out) {
    for (; (0 < ____n); ____n--)
    {
        int val__289 = 0;/* int */
        int i__290 = 0;/* int */

        // mark begin: SIRFilter staticDeQuantization

        for ((i__290 = 0)/*int*/; (i__290 < 64); (i__290++)) {{
            (val__289 = ((*____in++) * ((quantizationTable__286__24)[(int)i__290])))/*int*/;
            ((*____out++)=((float)(val__289)));
        }
        }
        // mark end: SIRFilter staticDeQuantization

    }}


void work_staticDeQuantization__292_151__24__mod2(int ____n, int *____in, float *____out, int s1, int s2) {
    for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
    {
        int val__289 = 0;/* int */
        int i__290 = 0;/* int */

        // mark begin: SIRFilter staticDeQuantization

        for ((i__290 = 0)/*int*/; (i__290 < 64); (i__290++)) {{
            (val__289 = ((*____in++) * ((quantizationTable__286__24)[(int)i__290])))/*int*/;
            ((*____out++)=((float)(val__289)));
        }
        }
        // mark end: SIRFilter staticDeQuantization

    }}


#endif // BUFFER_MERGE



void work_staticDeQuantization__292_151__24(int ____n){
    for (
            ; (0 < ____n); (____n--)) {{
        int val__289 = 0;/* int */
        int i__290 = 0;/* int */

        // mark begin: SIRFilter staticDeQuantization

        for ((i__290 = 0)/*int*/; (i__290 < 64); (i__290++)) {{
            (val__289 = (__pop__24() * ((quantizationTable__286__24)[(int)i__290])))/*int*/;
            __push__24(((float)(val__289)));
        }
        }
        // mark end: SIRFilter staticDeQuantization

    }
    }
}

// peek: 8 pop: 8 push 8
// init counts: 0 steady counts: 8

// ClusterFusion isEliminated: false



int __number_of_iterations_25;
int __counter_25 = 0;
int __steady_25 = 0;
int __tmp_25 = 0;
int __tmp2_25 = 0;
int *__state_flag_25 = NULL;
thread_info *__thread_25 = NULL;



void save_peek_buffer__25(object_write_buffer *buf);
void load_peek_buffer__25(object_write_buffer *buf);
void save_file_pointer__25(object_write_buffer *buf);
void load_file_pointer__25(object_write_buffer *buf);


void init_iDCT_1D__300_153__25();
inline void check_status__25();

void work_iDCT_1D__300_153__25(int);


inline float __pop__25() {
    float res=BUFFER_24_25[TAIL_24_25];
    TAIL_24_25++;
    return res;
}

inline float __pop__25(int n) {
    float res=BUFFER_24_25[TAIL_24_25];
    TAIL_24_25+=n;

    return res;
}

inline float __peek__25(int offs) {
    return BUFFER_24_25[TAIL_24_25+offs];
}



inline void __push__25(float data) {
    BUFFER_25_26[HEAD_25_26]=data;
    HEAD_25_26++;
}




void init_iDCT_1D__300_153__25(){
}
void save_file_pointer__25(object_write_buffer *buf) {}
void load_file_pointer__25(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_iDCT_1D__300_153__25__mod(int ____n, float *____in, float *____out) {
    for (; (0 < ____n); ____n--)
    {
        float Cu__295 = 0.0f;/* float */
        float tempsum__296 = 0.0f;/* float */
        int u__297 = 0;/* int */
        int x__conflict__0__298 = 0;/* int */
        int x__299 = 0;/* int */

        // mark begin: SIRFilter iDCT_1D

        for ((x__conflict__0__298 = 0)/*int*/; (x__conflict__0__298 < 8); (x__conflict__0__298++)) {{
            (tempsum__296 = ((float)0.0))/*float*/;
            for ((u__297 = 0)/*int*/; (u__297 < 8); (u__297++)) {{

                if ((u__297 == 0)) {(Cu__295 = ((float)0.70710677))/*float*/;
                } else {(Cu__295 = ((float)1.0))/*float*/;}
                (tempsum__296 = (tempsum__296 + ((Cu__295 * (*(____in+u__297))) * ((float)(cosf(((double)((((((float)(u__297)) * ((float)3.1415927)) * ((((float)2.0) * ((float)(x__conflict__0__298))) + ((float)1.0))) / ((float)16.0))))))))))/*float*/;
            }
            }
            ((*____out++)=(((float)0.5) * tempsum__296));
        }
        }
        for ((x__299 = 0)/*int*/; (x__299 < 8); (x__299++)) {{
            (*____in++);
        }
        }
        // mark end: SIRFilter iDCT_1D

    }}


void work_iDCT_1D__300_153__25__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
    for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
    {
        float Cu__295 = 0.0f;/* float */
        float tempsum__296 = 0.0f;/* float */
        int u__297 = 0;/* int */
        int x__conflict__0__298 = 0;/* int */
        int x__299 = 0;/* int */

        // mark begin: SIRFilter iDCT_1D

        for ((x__conflict__0__298 = 0)/*int*/; (x__conflict__0__298 < 8); (x__conflict__0__298++)) {{
            (tempsum__296 = ((float)0.0))/*float*/;
            for ((u__297 = 0)/*int*/; (u__297 < 8); (u__297++)) {{

                if ((u__297 == 0)) {(Cu__295 = ((float)0.70710677))/*float*/;
                } else {(Cu__295 = ((float)1.0))/*float*/;}
                (tempsum__296 = (tempsum__296 + ((Cu__295 * (*(____in+u__297))) * ((float)(cosf(((double)((((((float)(u__297)) * ((float)3.1415927)) * ((((float)2.0) * ((float)(x__conflict__0__298))) + ((float)1.0))) / ((float)16.0))))))))))/*float*/;
            }
            }
            ((*____out++)=(((float)0.5) * tempsum__296));
        }
        }
        for ((x__299 = 0)/*int*/; (x__299 < 8); (x__299++)) {{
            (*____in++);
        }
        }
        // mark end: SIRFilter iDCT_1D

    }}


#endif // BUFFER_MERGE



void work_iDCT_1D__300_153__25(int ____n){
    for (
            ; (0 < ____n); (____n--)) {{
        float Cu__295 = 0.0f;/* float */
        float tempsum__296 = 0.0f;/* float */
        int u__297 = 0;/* int */
        int x__conflict__0__298 = 0;/* int */
        int x__299 = 0;/* int */

        // mark begin: SIRFilter iDCT_1D

        for ((x__conflict__0__298 = 0)/*int*/; (x__conflict__0__298 < 8); (x__conflict__0__298++)) {{
            (tempsum__296 = ((float)0.0))/*float*/;
            for ((u__297 = 0)/*int*/; (u__297 < 8); (u__297++)) {{

                if ((u__297 == 0)) {(Cu__295 = ((float)0.70710677))/*float*/;
                } else {(Cu__295 = ((float)1.0))/*float*/;}
                (tempsum__296 = (tempsum__296 + ((Cu__295 * __peek__25(u__297)) * ((float)(cosf(((double)((((((float)(u__297)) * ((float)3.1415927)) * ((((float)2.0) * ((float)(x__conflict__0__298))) + ((float)1.0))) / ((float)16.0))))))))))/*float*/;
            }
            }
            __push__25((((float)0.5) * tempsum__296));
        }
        }
        for ((x__299 = 0)/*int*/; (x__299 < 8); (x__299++)) {{
            __pop__25();
        }
        }
        // mark end: SIRFilter iDCT_1D

    }
    }
}

// peek: 64 pop: 64 push 64
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_26;
int __counter_26 = 0;
int __steady_26 = 0;
int __tmp_26 = 0;
int __tmp2_26 = 0;
int *__state_flag_26 = NULL;
thread_info *__thread_26 = NULL;



void save_peek_buffer__26(object_write_buffer *buf);
void load_peek_buffer__26(object_write_buffer *buf);
void save_file_pointer__26(object_write_buffer *buf);
void load_file_pointer__26(object_write_buffer *buf);


void init_Pre_CollapsedDataParallel_1__306_262__26();
inline void check_status__26();

void work_Pre_CollapsedDataParallel_1__306_262__26(int);


inline float __pop__26() {
    float res=BUFFER_25_26[TAIL_25_26];
    TAIL_25_26++;
    return res;
}

inline float __pop__26(int n) {
    float res=BUFFER_25_26[TAIL_25_26];
    TAIL_25_26+=n;

    return res;
}

inline float __peek__26(int offs) {
    return BUFFER_25_26[TAIL_25_26+offs];
}



inline void __push__26(float data) {
    BUFFER_26_27[HEAD_26_27]=data;
    HEAD_26_27++;
}




void init_Pre_CollapsedDataParallel_1__306_262__26(){
}
void save_file_pointer__26(object_write_buffer *buf) {}
void load_file_pointer__26(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Pre_CollapsedDataParallel_1__306_262__26__mod(int ____n, float *____in, float *____out) {
    for (; (0 < ____n); ____n--)
    {
        int _k__303 = 0;/* int */
        int iTimesSumOfWeights_Plus_PartialSum_k__304 = 0;/* int */
        int _i__305 = 0;/* int */

        // mark begin: SIRFilter Pre_CollapsedDataParallel_1

        // mark begin: SIRSplitter WEIGHTED_ROUND_ROBIN_Splitter

        for ((_k__303 = 0)/*int*/; (_k__303 < 8); (_k__303++)) {{
            (iTimesSumOfWeights_Plus_PartialSum_k__304 = _k__303)/*int*/;
            for ((_i__305 = 0)/*int*/; (_i__305 < 8); (_i__305++)) {{
                ((*____out++)=(*(____in+(iTimesSumOfWeights_Plus_PartialSum_k__304 + 0))));
                (iTimesSumOfWeights_Plus_PartialSum_k__304 = (iTimesSumOfWeights_Plus_PartialSum_k__304 + 8))/*int*/;
            }
            }
        }
        }
        assert(false);__pop__26(64);
        // mark end: SIRSplitter WEIGHTED_ROUND_ROBIN_Splitter

        // mark end: SIRFilter Pre_CollapsedDataParallel_1

    }}


void work_Pre_CollapsedDataParallel_1__306_262__26__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
    for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
    {
        int _k__303 = 0;/* int */
        int iTimesSumOfWeights_Plus_PartialSum_k__304 = 0;/* int */
        int _i__305 = 0;/* int */

        // mark begin: SIRFilter Pre_CollapsedDataParallel_1

        // mark begin: SIRSplitter WEIGHTED_ROUND_ROBIN_Splitter

        for ((_k__303 = 0)/*int*/; (_k__303 < 8); (_k__303++)) {{
            (iTimesSumOfWeights_Plus_PartialSum_k__304 = _k__303)/*int*/;
            for ((_i__305 = 0)/*int*/; (_i__305 < 8); (_i__305++)) {{
                ((*____out++)=(*(____in+(iTimesSumOfWeights_Plus_PartialSum_k__304 + 0))));
                (iTimesSumOfWeights_Plus_PartialSum_k__304 = (iTimesSumOfWeights_Plus_PartialSum_k__304 + 8))/*int*/;
            }
            }
        }
        }
        assert(false);__pop__26(64);
        // mark end: SIRSplitter WEIGHTED_ROUND_ROBIN_Splitter

        // mark end: SIRFilter Pre_CollapsedDataParallel_1

    }}


#endif // BUFFER_MERGE



void work_Pre_CollapsedDataParallel_1__306_262__26(int ____n){
    for (
            ; (0 < ____n); (____n--)) {{
        int _k__303 = 0;/* int */
        int iTimesSumOfWeights_Plus_PartialSum_k__304 = 0;/* int */
        int _i__305 = 0;/* int */

        // mark begin: SIRFilter Pre_CollapsedDataParallel_1

        // mark begin: SIRSplitter WEIGHTED_ROUND_ROBIN_Splitter

        for ((_k__303 = 0)/*int*/; (_k__303 < 8); (_k__303++)) {{
            (iTimesSumOfWeights_Plus_PartialSum_k__304 = _k__303)/*int*/;
            for ((_i__305 = 0)/*int*/; (_i__305 < 8); (_i__305++)) {{
                __push__26(__peek__26((iTimesSumOfWeights_Plus_PartialSum_k__304 + 0)));
                (iTimesSumOfWeights_Plus_PartialSum_k__304 = (iTimesSumOfWeights_Plus_PartialSum_k__304 + 8))/*int*/;
            }
            }
        }
        }
        __pop__26(64);
        // mark end: SIRSplitter WEIGHTED_ROUND_ROBIN_Splitter

        // mark end: SIRFilter Pre_CollapsedDataParallel_1

    }
    }
}

// peek: 8 pop: 8 push 8
// init counts: 0 steady counts: 8

// ClusterFusion isEliminated: false



int __number_of_iterations_27;
int __counter_27 = 0;
int __steady_27 = 0;
int __tmp_27 = 0;
int __tmp2_27 = 0;
int *__state_flag_27 = NULL;
thread_info *__thread_27 = NULL;



void save_peek_buffer__27(object_write_buffer *buf);
void load_peek_buffer__27(object_write_buffer *buf);
void save_file_pointer__27(object_write_buffer *buf);
void load_file_pointer__27(object_write_buffer *buf);


void init_iDCT_1D__314_161__27();
inline void check_status__27();

void work_iDCT_1D__314_161__27(int);


inline float __pop__27() {
    float res=BUFFER_26_27[TAIL_26_27];
    TAIL_26_27++;
    return res;
}

inline float __pop__27(int n) {
    float res=BUFFER_26_27[TAIL_26_27];
    TAIL_26_27+=n;

    return res;
}

inline float __peek__27(int offs) {
    return BUFFER_26_27[TAIL_26_27+offs];
}



inline void __push__27(float data) {
    BUFFER_27_28[HEAD_27_28]=data;
    HEAD_27_28++;
}




void init_iDCT_1D__314_161__27(){
}
void save_file_pointer__27(object_write_buffer *buf) {}
void load_file_pointer__27(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_iDCT_1D__314_161__27__mod(int ____n, float *____in, float *____out) {
    for (; (0 < ____n); ____n--)
    {
        float Cu__309 = 0.0f;/* float */
        float tempsum__310 = 0.0f;/* float */
        int u__311 = 0;/* int */
        int x__conflict__0__312 = 0;/* int */
        int x__313 = 0;/* int */

        // mark begin: SIRFilter iDCT_1D

        for ((x__conflict__0__312 = 0)/*int*/; (x__conflict__0__312 < 8); (x__conflict__0__312++)) {{
            (tempsum__310 = ((float)0.0))/*float*/;
            for ((u__311 = 0)/*int*/; (u__311 < 8); (u__311++)) {{

                if ((u__311 == 0)) {(Cu__309 = ((float)0.70710677))/*float*/;
                } else {(Cu__309 = ((float)1.0))/*float*/;}
                (tempsum__310 = (tempsum__310 + ((Cu__309 * (*(____in+u__311))) * ((float)(cosf(((double)((((((float)(u__311)) * ((float)3.1415927)) * ((((float)2.0) * ((float)(x__conflict__0__312))) + ((float)1.0))) / ((float)16.0))))))))))/*float*/;
            }
            }
            ((*____out++)=(((float)0.5) * tempsum__310));
        }
        }
        for ((x__313 = 0)/*int*/; (x__313 < 8); (x__313++)) {{
            (*____in++);
        }
        }
        // mark end: SIRFilter iDCT_1D

    }}


void work_iDCT_1D__314_161__27__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
    for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
    {
        float Cu__309 = 0.0f;/* float */
        float tempsum__310 = 0.0f;/* float */
        int u__311 = 0;/* int */
        int x__conflict__0__312 = 0;/* int */
        int x__313 = 0;/* int */

        // mark begin: SIRFilter iDCT_1D

        for ((x__conflict__0__312 = 0)/*int*/; (x__conflict__0__312 < 8); (x__conflict__0__312++)) {{
            (tempsum__310 = ((float)0.0))/*float*/;
            for ((u__311 = 0)/*int*/; (u__311 < 8); (u__311++)) {{

                if ((u__311 == 0)) {(Cu__309 = ((float)0.70710677))/*float*/;
                } else {(Cu__309 = ((float)1.0))/*float*/;}
                (tempsum__310 = (tempsum__310 + ((Cu__309 * (*(____in+u__311))) * ((float)(cosf(((double)((((((float)(u__311)) * ((float)3.1415927)) * ((((float)2.0) * ((float)(x__conflict__0__312))) + ((float)1.0))) / ((float)16.0))))))))))/*float*/;
            }
            }
            ((*____out++)=(((float)0.5) * tempsum__310));
        }
        }
        for ((x__313 = 0)/*int*/; (x__313 < 8); (x__313++)) {{
            (*____in++);
        }
        }
        // mark end: SIRFilter iDCT_1D

    }}


#endif // BUFFER_MERGE



void work_iDCT_1D__314_161__27(int ____n){
    for (
            ; (0 < ____n); (____n--)) {{
        float Cu__309 = 0.0f;/* float */
        float tempsum__310 = 0.0f;/* float */
        int u__311 = 0;/* int */
        int x__conflict__0__312 = 0;/* int */
        int x__313 = 0;/* int */

        // mark begin: SIRFilter iDCT_1D

        for ((x__conflict__0__312 = 0)/*int*/; (x__conflict__0__312 < 8); (x__conflict__0__312++)) {{
            (tempsum__310 = ((float)0.0))/*float*/;
            for ((u__311 = 0)/*int*/; (u__311 < 8); (u__311++)) {{

                if ((u__311 == 0)) {(Cu__309 = ((float)0.70710677))/*float*/;
                } else {(Cu__309 = ((float)1.0))/*float*/;}
                (tempsum__310 = (tempsum__310 + ((Cu__309 * __peek__27(u__311)) * ((float)(cosf(((double)((((((float)(u__311)) * ((float)3.1415927)) * ((((float)2.0) * ((float)(x__conflict__0__312))) + ((float)1.0))) / ((float)16.0))))))))))/*float*/;
            }
            }
            __push__27((((float)0.5) * tempsum__310));
        }
        }
        for ((x__313 = 0)/*int*/; (x__313 < 8); (x__313++)) {{
            __pop__27();
        }
        }
        // mark end: SIRFilter iDCT_1D

    }
    }
}

// peek: 64 pop: 64 push 64
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_28;
int __counter_28 = 0;
int __steady_28 = 0;
int __tmp_28 = 0;
int __tmp2_28 = 0;
int *__state_flag_28 = NULL;
thread_info *__thread_28 = NULL;



void save_peek_buffer__28(object_write_buffer *buf);
void load_peek_buffer__28(object_write_buffer *buf);
void save_file_pointer__28(object_write_buffer *buf);
void load_file_pointer__28(object_write_buffer *buf);


void init_Post_CollapsedDataParallel_2__320_263__28();
inline void check_status__28();

void work_Post_CollapsedDataParallel_2__320_263__28(int);


inline float __pop__28() {
    float res=BUFFER_27_28[TAIL_27_28];
    TAIL_27_28++;
    return res;
}

inline float __pop__28(int n) {
    float res=BUFFER_27_28[TAIL_27_28];
    TAIL_27_28+=n;

    return res;
}

inline float __peek__28(int offs) {
    return BUFFER_27_28[TAIL_27_28+offs];
}



inline void __push__28(float data) {
    BUFFER_28_29[HEAD_28_29]=data;
    HEAD_28_29++;
}




void init_Post_CollapsedDataParallel_2__320_263__28(){
}
void save_file_pointer__28(object_write_buffer *buf) {}
void load_file_pointer__28(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Post_CollapsedDataParallel_2__320_263__28__mod(int ____n, float *____in, float *____out) {
    for (; (0 < ____n); ____n--)
    {
        int _k__317 = 0;/* int */
        int partialSum_i__318 = 0;/* int */
        int _i__319 = 0;/* int */

        // mark begin: SIRFilter Post_CollapsedDataParallel_2

        // mark begin: SIRJoiner WEIGHTED_ROUND_ROBIN_Joiner

        for ((_k__317 = 0)/*int*/; (_k__317 < 8); (_k__317++)) {{
            (partialSum_i__318 = 0)/*int*/;
            for ((_i__319 = 0)/*int*/; (_i__319 < 8); (_i__319++)) {{
                ((*____out++)=(*(____in+(_k__317 + (partialSum_i__318 + 0)))));
                (partialSum_i__318 = (partialSum_i__318 + 8))/*int*/;
            }
            }
        }
        }
        assert(false);__pop__28(64);
        // mark end: SIRJoiner WEIGHTED_ROUND_ROBIN_Joiner

        // mark end: SIRFilter Post_CollapsedDataParallel_2

    }}


void work_Post_CollapsedDataParallel_2__320_263__28__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
    for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
    {
        int _k__317 = 0;/* int */
        int partialSum_i__318 = 0;/* int */
        int _i__319 = 0;/* int */

        // mark begin: SIRFilter Post_CollapsedDataParallel_2

        // mark begin: SIRJoiner WEIGHTED_ROUND_ROBIN_Joiner

        for ((_k__317 = 0)/*int*/; (_k__317 < 8); (_k__317++)) {{
            (partialSum_i__318 = 0)/*int*/;
            for ((_i__319 = 0)/*int*/; (_i__319 < 8); (_i__319++)) {{
                ((*____out++)=(*(____in+(_k__317 + (partialSum_i__318 + 0)))));
                (partialSum_i__318 = (partialSum_i__318 + 8))/*int*/;
            }
            }
        }
        }
        assert(false);__pop__28(64);
        // mark end: SIRJoiner WEIGHTED_ROUND_ROBIN_Joiner

        // mark end: SIRFilter Post_CollapsedDataParallel_2

    }}


#endif // BUFFER_MERGE



void work_Post_CollapsedDataParallel_2__320_263__28(int ____n){
    for (
            ; (0 < ____n); (____n--)) {{
        int _k__317 = 0;/* int */
        int partialSum_i__318 = 0;/* int */
        int _i__319 = 0;/* int */

        // mark begin: SIRFilter Post_CollapsedDataParallel_2

        // mark begin: SIRJoiner WEIGHTED_ROUND_ROBIN_Joiner

        for ((_k__317 = 0)/*int*/; (_k__317 < 8); (_k__317++)) {{
            (partialSum_i__318 = 0)/*int*/;
            for ((_i__319 = 0)/*int*/; (_i__319 < 8); (_i__319++)) {{
                __push__28(__peek__28((_k__317 + (partialSum_i__318 + 0))));
                (partialSum_i__318 = (partialSum_i__318 + 8))/*int*/;
            }
            }
        }
        }
        __pop__28(64);
        // mark end: SIRJoiner WEIGHTED_ROUND_ROBIN_Joiner

        // mark end: SIRFilter Post_CollapsedDataParallel_2

    }
    }
}

// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 64

// ClusterFusion isEliminated: false



int __number_of_iterations_29;
int __counter_29 = 0;
int __steady_29 = 0;
int __tmp_29 = 0;
int __tmp2_29 = 0;
int *__state_flag_29 = NULL;
thread_info *__thread_29 = NULL;



void save_peek_buffer__29(object_write_buffer *buf);
void load_peek_buffer__29(object_write_buffer *buf);
void save_file_pointer__29(object_write_buffer *buf);
void load_file_pointer__29(object_write_buffer *buf);


void init_Add__323_169__29();
inline void check_status__29();

void work_Add__323_169__29(int);


inline float __pop__29() {
    float res=BUFFER_28_29[TAIL_28_29];
    TAIL_28_29++;
    return res;
}

inline float __pop__29(int n) {
    float res=BUFFER_28_29[TAIL_28_29];
    TAIL_28_29+=n;

    return res;
}

inline float __peek__29(int offs) {
    return BUFFER_28_29[TAIL_28_29+offs];
}



inline void __push__29(float data) {
    BUFFER_29_30[HEAD_29_30]=data;
    HEAD_29_30++;
}




void init_Add__323_169__29(){
}
void save_file_pointer__29(object_write_buffer *buf) {}
void load_file_pointer__29(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Add__323_169__29__mod(int ____n, float *____in, float *____out) {
    for (; (0 < ____n); ____n--)
    {
        // mark begin: SIRFilter Add

        ((*____out++)=((*____in++) + ((float)128.0)));
        // mark end: SIRFilter Add

    }}


void work_Add__323_169__29__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
    for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
    {
        // mark begin: SIRFilter Add

        ((*____out++)=((*____in++) + ((float)128.0)));
        // mark end: SIRFilter Add

    }}


#endif // BUFFER_MERGE



void work_Add__323_169__29(int ____n){
    for (
            ; (0 < ____n); (____n--)) {{
        // mark begin: SIRFilter Add

        __push__29((__pop__29() + ((float)128.0)));
        // mark end: SIRFilter Add

    }
    }
}

// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 64

// ClusterFusion isEliminated: false



int __number_of_iterations_30;
int __counter_30 = 0;
int __steady_30 = 0;
int __tmp_30 = 0;
int __tmp2_30 = 0;
int *__state_flag_30 = NULL;
thread_info *__thread_30 = NULL;



void save_peek_buffer__30(object_write_buffer *buf);
void load_peek_buffer__30(object_write_buffer *buf);
void save_file_pointer__30(object_write_buffer *buf);
void load_file_pointer__30(object_write_buffer *buf);


void init_Float2Int__326_170__30();
inline void check_status__30();

void work_Float2Int__326_170__30(int);


inline float __pop__30() {
    float res=BUFFER_29_30[TAIL_29_30];
    TAIL_29_30++;
    return res;
}

inline float __pop__30(int n) {
    float res=BUFFER_29_30[TAIL_29_30];
    TAIL_29_30+=n;

    return res;
}

inline float __peek__30(int offs) {
    return BUFFER_29_30[TAIL_29_30+offs];
}



inline void __push__30(int data) {
    BUFFER_30_31[HEAD_30_31]=data;
    HEAD_30_31++;
}




void init_Float2Int__326_170__30(){
}
void save_file_pointer__30(object_write_buffer *buf) {}
void load_file_pointer__30(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Float2Int__326_170__30__mod(int ____n, float *____in, int *____out) {
    for (; (0 < ____n); ____n--)
    {
        // mark begin: SIRFilter Float2Int

        ((*____out++)=((int)(((float)(floorf((((float)0.5) + (*____in++))))))));
        // mark end: SIRFilter Float2Int

    }}


void work_Float2Int__326_170__30__mod2(int ____n, float *____in, int *____out, int s1, int s2) {
    for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
    {
        // mark begin: SIRFilter Float2Int

        ((*____out++)=((int)(((float)(floorf((((float)0.5) + (*____in++))))))));
        // mark end: SIRFilter Float2Int

    }}


#endif // BUFFER_MERGE



void work_Float2Int__326_170__30(int ____n){
    for (
            ; (0 < ____n); (____n--)) {{
        // mark begin: SIRFilter Float2Int

        __push__30(((int)(((float)(floorf((((float)0.5) + __pop__30())))))));
        // mark end: SIRFilter Float2Int

    }
    }
}

// peek: 8 pop: 8 push 0
// init counts: 0 steady counts: 8

// ClusterFusion isEliminated: false



int __number_of_iterations_31;
int __counter_31 = 0;
int __steady_31 = 0;
int __tmp_31 = 0;
int __tmp2_31 = 0;
int *__state_flag_31 = NULL;
thread_info *__thread_31 = NULL;



void save_peek_buffer__31(object_write_buffer *buf);
void load_peek_buffer__31(object_write_buffer *buf);
void save_file_pointer__31(object_write_buffer *buf);
void load_file_pointer__31(object_write_buffer *buf);


void init_Display8BitIntBlock__331_171__31();
inline void check_status__31();

void work_Display8BitIntBlock__331_171__31(int);


inline int __pop__31() {
    int res=BUFFER_30_31[TAIL_30_31];
    TAIL_30_31++;
    return res;
}

inline int __pop__31(int n) {
    int res=BUFFER_30_31[TAIL_30_31];
    TAIL_30_31+=n;

    return res;
}

inline int __peek__31(int offs) {
    return BUFFER_30_31[TAIL_30_31+offs];
}



void init_Display8BitIntBlock__331_171__31(){
}
void save_file_pointer__31(object_write_buffer *buf) {}
void load_file_pointer__31(object_write_buffer *buf) {}

void work_Display8BitIntBlock__331_171__31(int ____n){
    for (
            ; (0 < ____n); (____n--)) {{
        int x__329[8] = {0};/* int[8] */
        int i__330 = 0;/* int */

        // mark begin: SIRFilter Display8BitIntBlock

        ((x__329[(int)0]) = __pop__31())/*int*/;
        ((x__329[(int)1]) = __pop__31())/*int*/;
        ((x__329[(int)2]) = __pop__31())/*int*/;
        ((x__329[(int)3]) = __pop__31())/*int*/;
        ((x__329[(int)4]) = __pop__31())/*int*/;
        ((x__329[(int)5]) = __pop__31())/*int*/;
        ((x__329[(int)6]) = __pop__31())/*int*/;
        ((x__329[(int)7]) = __pop__31())/*int*/;

        // TIMER_PRINT_CODE: __print_sink__ += (int)("NEWLINE"); 
        //printf( "%s", "NEWLINE"); printf("\n");

        for ((i__330 = 0)/*int*/; (i__330 < 8); (i__330++)) {{

            // TIMER_PRINT_CODE: __print_sink__ += (int)((x__329[(int)i__330])); 
            //printf( "%d", (x__329[(int)i__330])); printf("\n");
            sum+=x__329[(int)i__330];
        }
        }
        // mark end: SIRFilter Display8BitIntBlock

    }
    }
}

// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_32;
int __counter_32 = 0;
int __steady_32 = 0;
int __tmp_32 = 0;
int __tmp2_32 = 0;
int *__state_flag_32 = NULL;
thread_info *__thread_32 = NULL;



void save_peek_buffer__32(object_write_buffer *buf);
void load_peek_buffer__32(object_write_buffer *buf);
void save_file_pointer__32(object_write_buffer *buf);
void load_file_pointer__32(object_write_buffer *buf);


void init_Identity__279_147__32();
inline void check_status__32();

void Identity__279_147__work__32(int);


inline int __pop__32() {
    int res=BUFFER_21_32[TAIL_21_32];
    TAIL_21_32++;
    return res;
}

inline int __pop__32(int n) {
    int res=BUFFER_21_32[TAIL_21_32];
    TAIL_21_32+=n;

    return res;
}

inline int __peek__32(int offs) {
    return BUFFER_21_32[TAIL_21_32+offs];
}



inline void __push__32(int data) {
    BUFFER_32_19[HEAD_32_19]=data;
    HEAD_32_19++;
}



void init_Identity__279_147__32() {
}

void save_file_pointer__32(object_write_buffer *buf) {}
void load_file_pointer__32(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Identity__279_147__32__mod(int ____n, int *____in, int *____out) {
    for (; (0 < ____n); ____n--)
    {
        // mark begin: SIRIdentity Identity

        ((*____out++)=(*____in++));
        // mark end: SIRIdentity Identity

    }}


void work_Identity__279_147__32__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
    for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
    {
        // mark begin: SIRIdentity Identity

        ((*____out++)=(*____in++));
        // mark end: SIRIdentity Identity

    }}


#endif // BUFFER_MERGE


void Identity__279_147__work__32(int ____n) {
    for (; 0 < ____n; ____n--) {
        __push__32(__pop__32());
    }
}


// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 63

// ClusterFusion isEliminated: false



int __number_of_iterations_33;
int __counter_33 = 0;
int __steady_33 = 0;
int __tmp_33 = 0;
int __tmp2_33 = 0;
int *__state_flag_33 = NULL;
thread_info *__thread_33 = NULL;



void save_peek_buffer__33(object_write_buffer *buf);
void load_peek_buffer__33(object_write_buffer *buf);
void save_file_pointer__33(object_write_buffer *buf);
void load_file_pointer__33(object_write_buffer *buf);


void init_Identity__285_149__33();
inline void check_status__33();

void Identity__285_149__work__33(int);


inline int __pop__33() {
    int res=BUFFER_18_33[TAIL_18_33];
    TAIL_18_33++;
    return res;
}

inline int __pop__33(int n) {
    int res=BUFFER_18_33[TAIL_18_33];
    TAIL_18_33+=n;

    return res;
}

inline int __peek__33(int offs) {
    return BUFFER_18_33[TAIL_18_33+offs];
}



inline void __push__33(int data) {
    BUFFER_33_23[HEAD_33_23]=data;
    HEAD_33_23++;
}



void init_Identity__285_149__33() {
}

void save_file_pointer__33(object_write_buffer *buf) {}
void load_file_pointer__33(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Identity__285_149__33__mod(int ____n, int *____in, int *____out) {
    for (; (0 < ____n); ____n--)
    {
        // mark begin: SIRIdentity Identity

        ((*____out++)=(*____in++));
        // mark end: SIRIdentity Identity

    }}


void work_Identity__285_149__33__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
    for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
    {
        // mark begin: SIRIdentity Identity

        ((*____out++)=(*____in++));
        // mark end: SIRIdentity Identity

    }}


#endif // BUFFER_MERGE


void Identity__285_149__work__33(int ____n) {
    for (; 0 < ____n; ____n--) {
        __push__33(__pop__33());
    }
}


// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_34;
int __counter_34 = 0;
int __steady_34 = 0;
int __tmp_34 = 0;
int __tmp2_34 = 0;
int *__state_flag_34 = NULL;
thread_info *__thread_34 = NULL;



void save_peek_buffer__34(object_write_buffer *buf);
void load_peek_buffer__34(object_write_buffer *buf);
void save_file_pointer__34(object_write_buffer *buf);
void load_file_pointer__34(object_write_buffer *buf);


void init_Identity__243_136__34();
inline void check_status__34();

void Identity__243_136__work__34(int);


inline int __pop__34() {
    int res=BUFFER_11_34[TAIL_11_34];
    TAIL_11_34++;
    return res;
}

inline int __pop__34(int n) {
    int res=BUFFER_11_34[TAIL_11_34];
    TAIL_11_34+=n;

    return res;
}

inline int __peek__34(int offs) {
    return BUFFER_11_34[TAIL_11_34+offs];
}



inline void __push__34(int data) {
    BUFFER_34_9[HEAD_34_9]=data;
    HEAD_34_9++;
}



void init_Identity__243_136__34() {
}

void save_file_pointer__34(object_write_buffer *buf) {}
void load_file_pointer__34(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Identity__243_136__34__mod(int ____n, int *____in, int *____out) {
    for (; (0 < ____n); ____n--)
    {
        // mark begin: SIRIdentity Identity

        ((*____out++)=(*____in++));
        // mark end: SIRIdentity Identity

    }}


void work_Identity__243_136__34__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
    for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
    {
        // mark begin: SIRIdentity Identity

        ((*____out++)=(*____in++));
        // mark end: SIRIdentity Identity

    }}


#endif // BUFFER_MERGE


void Identity__243_136__work__34(int ____n) {
    for (; 0 < ____n; ____n--) {
        __push__34(__pop__34());
    }
}


// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 63

// ClusterFusion isEliminated: false



int __number_of_iterations_35;
int __counter_35 = 0;
int __steady_35 = 0;
int __tmp_35 = 0;
int __tmp2_35 = 0;
int *__state_flag_35 = NULL;
thread_info *__thread_35 = NULL;



void save_peek_buffer__35(object_write_buffer *buf);
void load_peek_buffer__35(object_write_buffer *buf);
void save_file_pointer__35(object_write_buffer *buf);
void load_file_pointer__35(object_write_buffer *buf);


void init_Identity__249_138__35();
inline void check_status__35();

void Identity__249_138__work__35(int);


inline int __pop__35() {
    int res=BUFFER_8_35[TAIL_8_35];
    TAIL_8_35++;
    return res;
}

inline int __pop__35(int n) {
    int res=BUFFER_8_35[TAIL_8_35];
    TAIL_8_35+=n;

    return res;
}

inline int __peek__35(int offs) {
    return BUFFER_8_35[TAIL_8_35+offs];
}



inline void __push__35(int data) {
    BUFFER_35_13[HEAD_35_13]=data;
    HEAD_35_13++;
}



void init_Identity__249_138__35() {
}

void save_file_pointer__35(object_write_buffer *buf) {}
void load_file_pointer__35(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Identity__249_138__35__mod(int ____n, int *____in, int *____out) {
    for (; (0 < ____n); ____n--)
    {
        // mark begin: SIRIdentity Identity

        ((*____out++)=(*____in++));
        // mark end: SIRIdentity Identity

    }}


void work_Identity__249_138__35__mod2(int ____n, int *____in, int *____out, int s1, int s2) {
    for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
    {
        // mark begin: SIRIdentity Identity

        ((*____out++)=(*____in++));
        // mark end: SIRIdentity Identity

    }}


#endif // BUFFER_MERGE


void Identity__249_138__work__35(int ____n) {
    for (; 0 < ____n; ____n--) {
        __push__35(__pop__35());
    }
}


