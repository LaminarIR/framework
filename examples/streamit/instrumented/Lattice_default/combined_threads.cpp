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
volatile double sum;
proc_timer tt("total runtime");


float BUFFER_0_1[__BUF_SIZE_MASK_0_1 + 1];
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
float BUFFER_3_43[__BUF_SIZE_MASK_3_43 + 1];
int HEAD_3_43 = 0;
int TAIL_3_43 = 0;
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
float BUFFER_7_42[__BUF_SIZE_MASK_7_42 + 1];
int HEAD_7_42 = 0;
int TAIL_7_42 = 0;
float BUFFER_8_9[__BUF_SIZE_MASK_8_9 + 1];
int HEAD_8_9 = 0;
int TAIL_8_9 = 0;
float BUFFER_9_10[__BUF_SIZE_MASK_9_10 + 1];
int HEAD_9_10 = 0;
int TAIL_9_10 = 0;
float BUFFER_10_11[__BUF_SIZE_MASK_10_11 + 1];
int HEAD_10_11 = 0;
int TAIL_10_11 = 0;
float BUFFER_11_12[__BUF_SIZE_MASK_11_12 + 1];
int HEAD_11_12 = 0;
int TAIL_11_12 = 0;
float BUFFER_11_41[__BUF_SIZE_MASK_11_41 + 1];
int HEAD_11_41 = 0;
int TAIL_11_41 = 0;
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
float BUFFER_15_40[__BUF_SIZE_MASK_15_40 + 1];
int HEAD_15_40 = 0;
int TAIL_15_40 = 0;
float BUFFER_16_17[__BUF_SIZE_MASK_16_17 + 1];
int HEAD_16_17 = 0;
int TAIL_16_17 = 0;
float BUFFER_17_18[__BUF_SIZE_MASK_17_18 + 1];
int HEAD_17_18 = 0;
int TAIL_17_18 = 0;
float BUFFER_18_19[__BUF_SIZE_MASK_18_19 + 1];
int HEAD_18_19 = 0;
int TAIL_18_19 = 0;
float BUFFER_19_20[__BUF_SIZE_MASK_19_20 + 1];
int HEAD_19_20 = 0;
int TAIL_19_20 = 0;
float BUFFER_19_39[__BUF_SIZE_MASK_19_39 + 1];
int HEAD_19_39 = 0;
int TAIL_19_39 = 0;
float BUFFER_20_21[__BUF_SIZE_MASK_20_21 + 1];
int HEAD_20_21 = 0;
int TAIL_20_21 = 0;
float BUFFER_21_22[__BUF_SIZE_MASK_21_22 + 1];
int HEAD_21_22 = 0;
int TAIL_21_22 = 0;
float BUFFER_22_23[__BUF_SIZE_MASK_22_23 + 1];
int HEAD_22_23 = 0;
int TAIL_22_23 = 0;
float BUFFER_23_24[__BUF_SIZE_MASK_23_24 + 1];
int HEAD_23_24 = 0;
int TAIL_23_24 = 0;
float BUFFER_23_38[__BUF_SIZE_MASK_23_38 + 1];
int HEAD_23_38 = 0;
int TAIL_23_38 = 0;
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
float BUFFER_27_37[__BUF_SIZE_MASK_27_37 + 1];
int HEAD_27_37 = 0;
int TAIL_27_37 = 0;
float BUFFER_28_29[__BUF_SIZE_MASK_28_29 + 1];
int HEAD_28_29 = 0;
int TAIL_28_29 = 0;
float BUFFER_29_30[__BUF_SIZE_MASK_29_30 + 1];
int HEAD_29_30 = 0;
int TAIL_29_30 = 0;
float BUFFER_30_31[__BUF_SIZE_MASK_30_31 + 1];
int HEAD_30_31 = 0;
int TAIL_30_31 = 0;
float BUFFER_31_32[__BUF_SIZE_MASK_31_32 + 1];
int HEAD_31_32 = 0;
int TAIL_31_32 = 0;
float BUFFER_31_36[__BUF_SIZE_MASK_31_36 + 1];
int HEAD_31_36 = 0;
int TAIL_31_36 = 0;
float BUFFER_32_33[__BUF_SIZE_MASK_32_33 + 1];
int HEAD_32_33 = 0;
int TAIL_32_33 = 0;
float BUFFER_33_34[__BUF_SIZE_MASK_33_34 + 1];
int HEAD_33_34 = 0;
int TAIL_33_34 = 0;
float BUFFER_34_35[__BUF_SIZE_MASK_34_35 + 1];
int HEAD_34_35 = 0;
int TAIL_34_35 = 0;
float BUFFER_36_33[__BUF_SIZE_MASK_36_33 + 1];
int HEAD_36_33 = 0;
int TAIL_36_33 = 0;
float BUFFER_37_29[__BUF_SIZE_MASK_37_29 + 1];
int HEAD_37_29 = 0;
int TAIL_37_29 = 0;
float BUFFER_38_25[__BUF_SIZE_MASK_38_25 + 1];
int HEAD_38_25 = 0;
int TAIL_38_25 = 0;
float BUFFER_39_21[__BUF_SIZE_MASK_39_21 + 1];
int HEAD_39_21 = 0;
int TAIL_39_21 = 0;
float BUFFER_40_17[__BUF_SIZE_MASK_40_17 + 1];
int HEAD_40_17 = 0;
int TAIL_40_17 = 0;
float BUFFER_41_13[__BUF_SIZE_MASK_41_13 + 1];
int HEAD_41_13 = 0;
int TAIL_41_13 = 0;
float BUFFER_42_9[__BUF_SIZE_MASK_42_9 + 1];
int HEAD_42_9 = 0;
int TAIL_42_9 = 0;
float BUFFER_43_5[__BUF_SIZE_MASK_43_5 + 1];
int HEAD_43_5 = 0;
int TAIL_43_5 = 0;
void init_Counter__15_31__0();
void work_Counter__15_31__0(int);
#ifdef BUFFER_MERGE
void work_Counter__15_31__0__mod(int ____n, void *____in, float *____out);
void work_Counter__15_31__0__mod2(int ____n, void *____in, float *____out, int s1, int s2);
#endif
void init_AutoGeneratedExpander__19_80__1();
void work_AutoGeneratedExpander__19_80__1(int);
#ifdef BUFFER_MERGE
void work_AutoGeneratedExpander__19_80__1__mod(int ____n, float *____in, float *____out);
void work_AutoGeneratedExpander__19_80__1__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_Identity__22_32__2();
void Identity__22_32__work__2(int);
#ifdef BUFFER_MERGE
void Identity__22_32__work__2__mod(int ____n, float *____in, float *____out);
void Identity__22_32__work__2__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void __splitter_3_work(int);
void init_Identity__25_36__4();
void Identity__25_36__work__4(int);
#ifdef BUFFER_MERGE
void Identity__25_36__work__4__mod(int ____n, float *____in, float *____out);
void Identity__25_36__work__4__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void __joiner_5_work(int);
void init_LatFilt__34_38__6();
void work_LatFilt__34_38__6(int);
#ifdef BUFFER_MERGE
void work_LatFilt__34_38__6__mod(int ____n, float *____in, float *____out);
void work_LatFilt__34_38__6__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void __splitter_7_work(int);
void init_Identity__37_41__8();
void Identity__37_41__work__8(int);
#ifdef BUFFER_MERGE
void Identity__37_41__work__8__mod(int ____n, float *____in, float *____out);
void Identity__37_41__work__8__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void __joiner_9_work(int);
void init_LatFilt__46_43__10();
void work_LatFilt__46_43__10(int);
#ifdef BUFFER_MERGE
void work_LatFilt__46_43__10__mod(int ____n, float *____in, float *____out);
void work_LatFilt__46_43__10__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void __splitter_11_work(int);
void init_Identity__49_46__12();
void Identity__49_46__work__12(int);
#ifdef BUFFER_MERGE
void Identity__49_46__work__12__mod(int ____n, float *____in, float *____out);
void Identity__49_46__work__12__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void __joiner_13_work(int);
void init_LatFilt__58_48__14();
void work_LatFilt__58_48__14(int);
#ifdef BUFFER_MERGE
void work_LatFilt__58_48__14__mod(int ____n, float *____in, float *____out);
void work_LatFilt__58_48__14__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void __splitter_15_work(int);
void init_Identity__61_51__16();
void Identity__61_51__work__16(int);
#ifdef BUFFER_MERGE
void Identity__61_51__work__16__mod(int ____n, float *____in, float *____out);
void Identity__61_51__work__16__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void __joiner_17_work(int);
void init_LatFilt__70_53__18();
void work_LatFilt__70_53__18(int);
#ifdef BUFFER_MERGE
void work_LatFilt__70_53__18__mod(int ____n, float *____in, float *____out);
void work_LatFilt__70_53__18__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void __splitter_19_work(int);
void init_Identity__73_56__20();
void Identity__73_56__work__20(int);
#ifdef BUFFER_MERGE
void Identity__73_56__work__20__mod(int ____n, float *____in, float *____out);
void Identity__73_56__work__20__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void __joiner_21_work(int);
void init_LatFilt__82_58__22();
void work_LatFilt__82_58__22(int);
#ifdef BUFFER_MERGE
void work_LatFilt__82_58__22__mod(int ____n, float *____in, float *____out);
void work_LatFilt__82_58__22__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void __splitter_23_work(int);
void init_Identity__85_61__24();
void Identity__85_61__work__24(int);
#ifdef BUFFER_MERGE
void Identity__85_61__work__24__mod(int ____n, float *____in, float *____out);
void Identity__85_61__work__24__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void __joiner_25_work(int);
void init_LatFilt__94_63__26();
void work_LatFilt__94_63__26(int);
#ifdef BUFFER_MERGE
void work_LatFilt__94_63__26__mod(int ____n, float *____in, float *____out);
void work_LatFilt__94_63__26__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void __splitter_27_work(int);
void init_Identity__97_66__28();
void Identity__97_66__work__28(int);
#ifdef BUFFER_MERGE
void Identity__97_66__work__28__mod(int ____n, float *____in, float *____out);
void Identity__97_66__work__28__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void __joiner_29_work(int);
void init_LatFilt__106_68__30();
void work_LatFilt__106_68__30(int);
#ifdef BUFFER_MERGE
void work_LatFilt__106_68__30__mod(int ____n, float *____in, float *____out);
void work_LatFilt__106_68__30__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void __splitter_31_work(int);
void init_Identity__109_71__32();
void Identity__109_71__work__32(int);
#ifdef BUFFER_MERGE
void Identity__109_71__work__32__mod(int ____n, float *____in, float *____out);
void Identity__109_71__work__32__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void __joiner_33_work(int);
void init_LatFilt__118_73__34();
void work_LatFilt__118_73__34(int);
#ifdef BUFFER_MERGE
void work_LatFilt__118_73__34__mod(int ____n, float *____in, float *____out);
void work_LatFilt__118_73__34__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_LastStage__122_74__35();
void work_LastStage__122_74__35(int);
#ifdef BUFFER_MERGE
void work_LastStage__122_74__35__mod(int ____n, float *____in, void *____out);
void work_LastStage__122_74__35__mod2(int ____n, float *____in, void *____out, int s1, int s2);
#endif
void init_DelayOne__113_72__36();
void work_DelayOne__113_72__36(int);
#ifdef BUFFER_MERGE
void work_DelayOne__113_72__36__mod(int ____n, float *____in, float *____out);
void work_DelayOne__113_72__36__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_DelayOne__101_67__37();
void work_DelayOne__101_67__37(int);
#ifdef BUFFER_MERGE
void work_DelayOne__101_67__37__mod(int ____n, float *____in, float *____out);
void work_DelayOne__101_67__37__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_DelayOne__89_62__38();
void work_DelayOne__89_62__38(int);
#ifdef BUFFER_MERGE
void work_DelayOne__89_62__38__mod(int ____n, float *____in, float *____out);
void work_DelayOne__89_62__38__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_DelayOne__77_57__39();
void work_DelayOne__77_57__39(int);
#ifdef BUFFER_MERGE
void work_DelayOne__77_57__39__mod(int ____n, float *____in, float *____out);
void work_DelayOne__77_57__39__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_DelayOne__65_52__40();
void work_DelayOne__65_52__40(int);
#ifdef BUFFER_MERGE
void work_DelayOne__65_52__40__mod(int ____n, float *____in, float *____out);
void work_DelayOne__65_52__40__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_DelayOne__53_47__41();
void work_DelayOne__53_47__41(int);
#ifdef BUFFER_MERGE
void work_DelayOne__53_47__41__mod(int ____n, float *____in, float *____out);
void work_DelayOne__53_47__41__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_DelayOne__41_42__42();
void work_DelayOne__41_42__42(int);
#ifdef BUFFER_MERGE
void work_DelayOne__41_42__42__mod(int ____n, float *____in, float *____out);
void work_DelayOne__41_42__42__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_DelayOne__29_37__43();
void work_DelayOne__29_37__43(int);
#ifdef BUFFER_MERGE
void work_DelayOne__29_37__43__mod(int ____n, float *____in, float *____out);
void work_DelayOne__29_37__43__mod2(int ____n, float *____in, float *____out, int s1, int s2);
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
// number of phases: 36


#ifdef MEASURE_BY_RTS
RTS_init(1,1);
#else //default ``MEASURE_BY_PAPI''
PAPI_HW_COUNTER_init(1,1);
#endif
  // ============= Initialization =============

init_Counter__15_31__0();
init_AutoGeneratedExpander__19_80__1();
init_Identity__22_32__2();
init_Identity__25_36__4();
init_DelayOne__29_37__43();
init_LatFilt__34_38__6();
init_DelayOne__41_42__42();
init_Identity__37_41__8();
init_LatFilt__46_43__10();
init_Identity__49_46__12();
init_DelayOne__53_47__41();
init_LatFilt__58_48__14();
init_Identity__61_51__16();
init_DelayOne__65_52__40();
init_LatFilt__70_53__18();
init_DelayOne__77_57__39();
init_Identity__73_56__20();
init_LatFilt__82_58__22();
init_DelayOne__89_62__38();
init_Identity__85_61__24();
init_LatFilt__94_63__26();
init_Identity__97_66__28();
init_DelayOne__101_67__37();
init_LatFilt__106_68__30();
init_Identity__109_71__32();
init_DelayOne__113_72__36();
init_LatFilt__118_73__34();
init_LastStage__122_74__35();

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
    work_Counter__15_31__0(1 );
HEAD_1_2 = 0;
TAIL_1_2 = 0;
    work_AutoGeneratedExpander__19_80__1(1 );
HEAD_2_3 = 0;
TAIL_2_3 = 0;
    Identity__22_32__work__2(2 );
HEAD_3_4 = 0;
TAIL_3_4 = 0;
HEAD_3_43 = 0;
TAIL_3_43 = 0;
    __splitter_3_work(2 );
HEAD_4_5 = 0;
TAIL_4_5 = 0;
    Identity__25_36__work__4(2 );
HEAD_43_5 = 0;
TAIL_43_5 = 0;
    work_DelayOne__29_37__43(2 );
HEAD_5_6 = 0;
TAIL_5_6 = 0;
    __joiner_5_work(2 );
HEAD_6_7 = 0;
TAIL_6_7 = 0;
    work_LatFilt__34_38__6(2 );
HEAD_7_8 = 0;
TAIL_7_8 = 0;
HEAD_7_42 = 0;
TAIL_7_42 = 0;
    __splitter_7_work(4 );
HEAD_42_9 = 0;
TAIL_42_9 = 0;
    work_DelayOne__41_42__42(4 );
HEAD_8_9 = 0;
TAIL_8_9 = 0;
    Identity__37_41__work__8(4 );
HEAD_9_10 = 0;
TAIL_9_10 = 0;
    __joiner_9_work(4 );
HEAD_10_11 = 0;
TAIL_10_11 = 0;
    work_LatFilt__46_43__10(4 );
HEAD_11_12 = 0;
TAIL_11_12 = 0;
HEAD_11_41 = 0;
TAIL_11_41 = 0;
    __splitter_11_work(8 );
HEAD_12_13 = 0;
TAIL_12_13 = 0;
    Identity__49_46__work__12(8 );
HEAD_41_13 = 0;
TAIL_41_13 = 0;
    work_DelayOne__53_47__41(8 );
HEAD_13_14 = 0;
TAIL_13_14 = 0;
    __joiner_13_work(8 );
HEAD_14_15 = 0;
TAIL_14_15 = 0;
    work_LatFilt__58_48__14(8 );
HEAD_15_16 = 0;
TAIL_15_16 = 0;
HEAD_15_40 = 0;
TAIL_15_40 = 0;
    __splitter_15_work(16 );
HEAD_16_17 = 0;
TAIL_16_17 = 0;
    Identity__61_51__work__16(16 );
HEAD_40_17 = 0;
TAIL_40_17 = 0;
    work_DelayOne__65_52__40(16 );
HEAD_17_18 = 0;
TAIL_17_18 = 0;
    __joiner_17_work(16 );
HEAD_18_19 = 0;
TAIL_18_19 = 0;
    work_LatFilt__70_53__18(16 );
HEAD_19_20 = 0;
TAIL_19_20 = 0;
HEAD_19_39 = 0;
TAIL_19_39 = 0;
    __splitter_19_work(32 );
HEAD_39_21 = 0;
TAIL_39_21 = 0;
    work_DelayOne__77_57__39(32 );
HEAD_20_21 = 0;
TAIL_20_21 = 0;
    Identity__73_56__work__20(32 );
HEAD_21_22 = 0;
TAIL_21_22 = 0;
    __joiner_21_work(32 );
HEAD_22_23 = 0;
TAIL_22_23 = 0;
    work_LatFilt__82_58__22(32 );
HEAD_23_24 = 0;
TAIL_23_24 = 0;
HEAD_23_38 = 0;
TAIL_23_38 = 0;
    __splitter_23_work(64 );
HEAD_38_25 = 0;
TAIL_38_25 = 0;
    work_DelayOne__89_62__38(64 );
HEAD_24_25 = 0;
TAIL_24_25 = 0;
    Identity__85_61__work__24(64 );
HEAD_25_26 = 0;
TAIL_25_26 = 0;
    __joiner_25_work(64 );
HEAD_26_27 = 0;
TAIL_26_27 = 0;
    work_LatFilt__94_63__26(64 );
HEAD_27_28 = 0;
TAIL_27_28 = 0;
HEAD_27_37 = 0;
TAIL_27_37 = 0;
    __splitter_27_work(128 );
HEAD_28_29 = 0;
TAIL_28_29 = 0;
    Identity__97_66__work__28(128 );
HEAD_37_29 = 0;
TAIL_37_29 = 0;
    work_DelayOne__101_67__37(128 );
HEAD_29_30 = 0;
TAIL_29_30 = 0;
    __joiner_29_work(128 );
HEAD_30_31 = 0;
TAIL_30_31 = 0;
    work_LatFilt__106_68__30(128 );
HEAD_31_32 = 0;
TAIL_31_32 = 0;
HEAD_31_36 = 0;
TAIL_31_36 = 0;
    __splitter_31_work(256 );
HEAD_32_33 = 0;
TAIL_32_33 = 0;
    Identity__109_71__work__32(256 );
HEAD_36_33 = 0;
TAIL_36_33 = 0;
    work_DelayOne__113_72__36(256 );
HEAD_33_34 = 0;
TAIL_33_34 = 0;
    __joiner_33_work(256 );
HEAD_34_35 = 0;
TAIL_34_35 = 0;
    work_LatFilt__118_73__34(256 );
    work_LastStage__122_74__35(256 );
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
message *__msg_stack_34;
message *__msg_stack_38;
message *__msg_stack_3;
message *__msg_stack_10;
message *__msg_stack_24;
message *__msg_stack_42;
message *__msg_stack_9;
message *__msg_stack_40;
message *__msg_stack_31;
message *__msg_stack_1;
message *__msg_stack_14;
message *__msg_stack_19;
message *__msg_stack_5;
message *__msg_stack_15;
message *__msg_stack_2;
message *__msg_stack_17;
message *__msg_stack_21;
message *__msg_stack_41;
message *__msg_stack_23;
message *__msg_stack_30;
message *__msg_stack_36;
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
message *__msg_stack_43;
message *__msg_stack_39;
message *__msg_stack_37;

// end of moved or inserted by concat_cluster_threads.pl

// peek: 0 pop: 0 push 1
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



float i__12__0 = 0.0f;
void save_file_pointer__0(object_write_buffer *buf);
void load_file_pointer__0(object_write_buffer *buf);

 
void init_Counter__15_31__0();
inline void check_status__0();

void work_Counter__15_31__0(int);



inline void __push__0(float data) {
BUFFER_0_1[HEAD_0_1]=data;
HEAD_0_1++;
}



 
void init_Counter__15_31__0(){
  ((i__12__0) = ((float)1.0))/*float*/;
}
void save_file_pointer__0(object_write_buffer *buf) {}
void load_file_pointer__0(object_write_buffer *buf) {}
 
void work_Counter__15_31__0(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter Counter

      __push__0((i__12__0));
      ((i__12__0) = ((float)0.0))/*float*/;
      // mark end: SIRFilter Counter

    }
  }
}

// peek: 1 pop: 1 push 2
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

 
void init_AutoGeneratedExpander__19_80__1();
inline void check_status__1();

void work_AutoGeneratedExpander__19_80__1(int);


inline float __pop__1() {
float res=BUFFER_0_1[TAIL_0_1];
TAIL_0_1++;
return res;
}

inline float __pop__1(int n) {
float res=BUFFER_0_1[TAIL_0_1];
TAIL_0_1+=n;

return res;
}

inline float __peek__1(int offs) {
return BUFFER_0_1[TAIL_0_1+offs];
}



inline void __push__1(float data) {
BUFFER_1_2[HEAD_1_2]=data;
HEAD_1_2++;
}



 
void init_AutoGeneratedExpander__19_80__1(){
}
void save_file_pointer__1(object_write_buffer *buf) {}
void load_file_pointer__1(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AutoGeneratedExpander__19_80__1__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  int streamItVar3__18 = 0;/* int */

  // mark begin: SIRFilter AutoGeneratedExpander

  for ((streamItVar3__18 = 0)/*int*/; (streamItVar3__18 < 2); (streamItVar3__18++)) {{
      ((*____out++)=(*(____in+0)));
    }
  }
  (*____in++);
  // mark end: SIRFilter AutoGeneratedExpander

}}


void work_AutoGeneratedExpander__19_80__1__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  int streamItVar3__18 = 0;/* int */

  // mark begin: SIRFilter AutoGeneratedExpander

  for ((streamItVar3__18 = 0)/*int*/; (streamItVar3__18 < 2); (streamItVar3__18++)) {{
      ((*____out++)=(*(____in+0)));
    }
  }
  (*____in++);
  // mark end: SIRFilter AutoGeneratedExpander

}}


#endif // BUFFER_MERGE


 
void work_AutoGeneratedExpander__19_80__1(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      int streamItVar3__18 = 0;/* int */

      // mark begin: SIRFilter AutoGeneratedExpander

      for ((streamItVar3__18 = 0)/*int*/; (streamItVar3__18 < 2); (streamItVar3__18++)) {{
          __push__1(__peek__1(0));
        }
      }
      __pop__1();
      // mark end: SIRFilter AutoGeneratedExpander

    }
  }
}

// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 2

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

 
void init_Identity__22_32__2();
inline void check_status__2();

void Identity__22_32__work__2(int);


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



void init_Identity__22_32__2() {
}

void save_file_pointer__2(object_write_buffer *buf) {}
void load_file_pointer__2(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Identity__22_32__2__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRIdentity Identity

  ((*____out++)=(*____in++));
  // mark end: SIRIdentity Identity

}}


void work_Identity__22_32__2__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRIdentity Identity

  ((*____out++)=(*____in++));
  // mark end: SIRIdentity Identity

}}


#endif // BUFFER_MERGE


void Identity__22_32__work__2(int ____n) {
    for (; 0 < ____n; ____n--) {
        __push__2(__pop__2());
    }
}


// init counts: 0 steady counts: 2

// ClusterFusion isEliminated: false



int __number_of_iterations_3;
int __counter_3 = 0;
int __steady_3 = 0;
int __tmp_3 = 0;
int __tmp2_3 = 0;
int *__state_flag_3 = NULL;
thread_info *__thread_3 = NULL;




void __splitter_3_work(int ____n) {
  for (;____n > 0; ____n--) {
  float tmp;
  tmp = BUFFER_2_3[TAIL_2_3]; TAIL_2_3++;
  BUFFER_3_4[HEAD_3_4]=tmp;
HEAD_3_4++; ;
  BUFFER_3_43[HEAD_3_43]=tmp;
HEAD_3_43++; ;
  }
}


// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 2

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

 
void init_Identity__25_36__4();
inline void check_status__4();

void Identity__25_36__work__4(int);


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



void init_Identity__25_36__4() {
}

void save_file_pointer__4(object_write_buffer *buf) {}
void load_file_pointer__4(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Identity__25_36__4__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRIdentity Identity

  ((*____out++)=(*____in++));
  // mark end: SIRIdentity Identity

}}


void work_Identity__25_36__4__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRIdentity Identity

  ((*____out++)=(*____in++));
  // mark end: SIRIdentity Identity

}}


#endif // BUFFER_MERGE


void Identity__25_36__work__4(int ____n) {
    for (; 0 < ____n; ____n--) {
        __push__4(__pop__4());
    }
}


// init counts: 0 steady counts: 2

// ClusterFusion isEliminated: false



int __number_of_iterations_5;
int __counter_5 = 0;
int __steady_5 = 0;
int __tmp_5 = 0;
int __tmp2_5 = 0;
int *__state_flag_5 = NULL;
thread_info *__thread_5 = NULL;




void __joiner_5_work(int ____n) {
  for (;____n > 0; ____n--) {
BUFFER_5_6[HEAD_5_6]=BUFFER_4_5[TAIL_4_5];
HEAD_5_6++; ; TAIL_4_5++;
BUFFER_5_6[HEAD_5_6]=BUFFER_43_5[TAIL_43_5];
HEAD_5_6++; ; TAIL_43_5++;
  }
}


// peek: 2 pop: 2 push 2
// init counts: 0 steady counts: 2

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

 
void init_LatFilt__34_38__6();
inline void check_status__6();

void work_LatFilt__34_38__6(int);


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



 
void init_LatFilt__34_38__6(){
}
void save_file_pointer__6(object_write_buffer *buf) {}
void load_file_pointer__6(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_LatFilt__34_38__6__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float e_i__32 = 0.0f;/* float */
  float e_bar_i__33 = 0.0f;/* float */

  // mark begin: SIRFilter LatFilt

  (e_i__32 = ((float)0.0))/*float*/;
  (e_bar_i__33 = ((float)0.0))/*float*/;
  (e_i__32 = ((*(____in+0)) - (((float)2.0) * (*(____in+1)))))/*float*/;
  (e_bar_i__33 = ((*(____in+1)) - (((float)2.0) * (*(____in+0)))))/*float*/;
  ((*____out++)=e_i__32);
  ((*____out++)=e_bar_i__33);
  assert(false);__pop__6(2);
  // mark end: SIRFilter LatFilt

}}


void work_LatFilt__34_38__6__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float e_i__32 = 0.0f;/* float */
  float e_bar_i__33 = 0.0f;/* float */

  // mark begin: SIRFilter LatFilt

  (e_i__32 = ((float)0.0))/*float*/;
  (e_bar_i__33 = ((float)0.0))/*float*/;
  (e_i__32 = ((*(____in+0)) - (((float)2.0) * (*(____in+1)))))/*float*/;
  (e_bar_i__33 = ((*(____in+1)) - (((float)2.0) * (*(____in+0)))))/*float*/;
  ((*____out++)=e_i__32);
  ((*____out++)=e_bar_i__33);
  assert(false);__pop__6(2);
  // mark end: SIRFilter LatFilt

}}


#endif // BUFFER_MERGE


 
void work_LatFilt__34_38__6(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float e_i__32 = 0.0f;/* float */
      float e_bar_i__33 = 0.0f;/* float */

      // mark begin: SIRFilter LatFilt

      (e_i__32 = ((float)0.0))/*float*/;
      (e_bar_i__33 = ((float)0.0))/*float*/;
      (e_i__32 = (__peek__6(0) - (((float)2.0) * __peek__6(1))))/*float*/;
      (e_bar_i__33 = (__peek__6(1) - (((float)2.0) * __peek__6(0))))/*float*/;
      __push__6(e_i__32);
      __push__6(e_bar_i__33);
      __pop__6(2);
      // mark end: SIRFilter LatFilt

    }
  }
}

// init counts: 0 steady counts: 4

// ClusterFusion isEliminated: false



int __number_of_iterations_7;
int __counter_7 = 0;
int __steady_7 = 0;
int __tmp_7 = 0;
int __tmp2_7 = 0;
int *__state_flag_7 = NULL;
thread_info *__thread_7 = NULL;




void __splitter_7_work(int ____n) {
  for (;____n > 0; ____n--) {
  float tmp;
  tmp = BUFFER_6_7[TAIL_6_7]; TAIL_6_7++;
  BUFFER_7_8[HEAD_7_8]=tmp;
HEAD_7_8++; ;
  BUFFER_7_42[HEAD_7_42]=tmp;
HEAD_7_42++; ;
  }
}


// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 4

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

 
void init_Identity__37_41__8();
inline void check_status__8();

void Identity__37_41__work__8(int);


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



void init_Identity__37_41__8() {
}

void save_file_pointer__8(object_write_buffer *buf) {}
void load_file_pointer__8(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Identity__37_41__8__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRIdentity Identity

  ((*____out++)=(*____in++));
  // mark end: SIRIdentity Identity

}}


void work_Identity__37_41__8__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRIdentity Identity

  ((*____out++)=(*____in++));
  // mark end: SIRIdentity Identity

}}


#endif // BUFFER_MERGE


void Identity__37_41__work__8(int ____n) {
    for (; 0 < ____n; ____n--) {
        __push__8(__pop__8());
    }
}


// init counts: 0 steady counts: 4

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
BUFFER_9_10[HEAD_9_10]=BUFFER_42_9[TAIL_42_9];
HEAD_9_10++; ; TAIL_42_9++;
  }
}


// peek: 2 pop: 2 push 2
// init counts: 0 steady counts: 4

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

 
void init_LatFilt__46_43__10();
inline void check_status__10();

void work_LatFilt__46_43__10(int);


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



inline void __push__10(float data) {
BUFFER_10_11[HEAD_10_11]=data;
HEAD_10_11++;
}



 
void init_LatFilt__46_43__10(){
}
void save_file_pointer__10(object_write_buffer *buf) {}
void load_file_pointer__10(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_LatFilt__46_43__10__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float e_i__44 = 0.0f;/* float */
  float e_bar_i__45 = 0.0f;/* float */

  // mark begin: SIRFilter LatFilt

  (e_i__44 = ((float)0.0))/*float*/;
  (e_bar_i__45 = ((float)0.0))/*float*/;
  (e_i__44 = ((*(____in+0)) - (((float)3.0) * (*(____in+1)))))/*float*/;
  (e_bar_i__45 = ((*(____in+1)) - (((float)3.0) * (*(____in+0)))))/*float*/;
  ((*____out++)=e_i__44);
  ((*____out++)=e_bar_i__45);
  assert(false);__pop__10(2);
  // mark end: SIRFilter LatFilt

}}


void work_LatFilt__46_43__10__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float e_i__44 = 0.0f;/* float */
  float e_bar_i__45 = 0.0f;/* float */

  // mark begin: SIRFilter LatFilt

  (e_i__44 = ((float)0.0))/*float*/;
  (e_bar_i__45 = ((float)0.0))/*float*/;
  (e_i__44 = ((*(____in+0)) - (((float)3.0) * (*(____in+1)))))/*float*/;
  (e_bar_i__45 = ((*(____in+1)) - (((float)3.0) * (*(____in+0)))))/*float*/;
  ((*____out++)=e_i__44);
  ((*____out++)=e_bar_i__45);
  assert(false);__pop__10(2);
  // mark end: SIRFilter LatFilt

}}


#endif // BUFFER_MERGE


 
void work_LatFilt__46_43__10(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float e_i__44 = 0.0f;/* float */
      float e_bar_i__45 = 0.0f;/* float */

      // mark begin: SIRFilter LatFilt

      (e_i__44 = ((float)0.0))/*float*/;
      (e_bar_i__45 = ((float)0.0))/*float*/;
      (e_i__44 = (__peek__10(0) - (((float)3.0) * __peek__10(1))))/*float*/;
      (e_bar_i__45 = (__peek__10(1) - (((float)3.0) * __peek__10(0))))/*float*/;
      __push__10(e_i__44);
      __push__10(e_bar_i__45);
      __pop__10(2);
      // mark end: SIRFilter LatFilt

    }
  }
}

// init counts: 0 steady counts: 8

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
  float tmp;
  tmp = BUFFER_10_11[TAIL_10_11]; TAIL_10_11++;
  BUFFER_11_12[HEAD_11_12]=tmp;
HEAD_11_12++; ;
  BUFFER_11_41[HEAD_11_41]=tmp;
HEAD_11_41++; ;
  }
}


// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 8

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

 
void init_Identity__49_46__12();
inline void check_status__12();

void Identity__49_46__work__12(int);


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



void init_Identity__49_46__12() {
}

void save_file_pointer__12(object_write_buffer *buf) {}
void load_file_pointer__12(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Identity__49_46__12__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRIdentity Identity

  ((*____out++)=(*____in++));
  // mark end: SIRIdentity Identity

}}


void work_Identity__49_46__12__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRIdentity Identity

  ((*____out++)=(*____in++));
  // mark end: SIRIdentity Identity

}}


#endif // BUFFER_MERGE


void Identity__49_46__work__12(int ____n) {
    for (; 0 < ____n; ____n--) {
        __push__12(__pop__12());
    }
}


// init counts: 0 steady counts: 8

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
BUFFER_13_14[HEAD_13_14]=BUFFER_41_13[TAIL_41_13];
HEAD_13_14++; ; TAIL_41_13++;
  }
}


// peek: 2 pop: 2 push 2
// init counts: 0 steady counts: 8

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

 
void init_LatFilt__58_48__14();
inline void check_status__14();

void work_LatFilt__58_48__14(int);


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



 
void init_LatFilt__58_48__14(){
}
void save_file_pointer__14(object_write_buffer *buf) {}
void load_file_pointer__14(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_LatFilt__58_48__14__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float e_i__56 = 0.0f;/* float */
  float e_bar_i__57 = 0.0f;/* float */

  // mark begin: SIRFilter LatFilt

  (e_i__56 = ((float)0.0))/*float*/;
  (e_bar_i__57 = ((float)0.0))/*float*/;
  (e_i__56 = ((*(____in+0)) - (((float)4.0) * (*(____in+1)))))/*float*/;
  (e_bar_i__57 = ((*(____in+1)) - (((float)4.0) * (*(____in+0)))))/*float*/;
  ((*____out++)=e_i__56);
  ((*____out++)=e_bar_i__57);
  assert(false);__pop__14(2);
  // mark end: SIRFilter LatFilt

}}


void work_LatFilt__58_48__14__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float e_i__56 = 0.0f;/* float */
  float e_bar_i__57 = 0.0f;/* float */

  // mark begin: SIRFilter LatFilt

  (e_i__56 = ((float)0.0))/*float*/;
  (e_bar_i__57 = ((float)0.0))/*float*/;
  (e_i__56 = ((*(____in+0)) - (((float)4.0) * (*(____in+1)))))/*float*/;
  (e_bar_i__57 = ((*(____in+1)) - (((float)4.0) * (*(____in+0)))))/*float*/;
  ((*____out++)=e_i__56);
  ((*____out++)=e_bar_i__57);
  assert(false);__pop__14(2);
  // mark end: SIRFilter LatFilt

}}


#endif // BUFFER_MERGE


 
void work_LatFilt__58_48__14(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float e_i__56 = 0.0f;/* float */
      float e_bar_i__57 = 0.0f;/* float */

      // mark begin: SIRFilter LatFilt

      (e_i__56 = ((float)0.0))/*float*/;
      (e_bar_i__57 = ((float)0.0))/*float*/;
      (e_i__56 = (__peek__14(0) - (((float)4.0) * __peek__14(1))))/*float*/;
      (e_bar_i__57 = (__peek__14(1) - (((float)4.0) * __peek__14(0))))/*float*/;
      __push__14(e_i__56);
      __push__14(e_bar_i__57);
      __pop__14(2);
      // mark end: SIRFilter LatFilt

    }
  }
}

// init counts: 0 steady counts: 16

// ClusterFusion isEliminated: false



int __number_of_iterations_15;
int __counter_15 = 0;
int __steady_15 = 0;
int __tmp_15 = 0;
int __tmp2_15 = 0;
int *__state_flag_15 = NULL;
thread_info *__thread_15 = NULL;




void __splitter_15_work(int ____n) {
  for (;____n > 0; ____n--) {
  float tmp;
  tmp = BUFFER_14_15[TAIL_14_15]; TAIL_14_15++;
  BUFFER_15_16[HEAD_15_16]=tmp;
HEAD_15_16++; ;
  BUFFER_15_40[HEAD_15_40]=tmp;
HEAD_15_40++; ;
  }
}


// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 16

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

 
void init_Identity__61_51__16();
inline void check_status__16();

void Identity__61_51__work__16(int);


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



void init_Identity__61_51__16() {
}

void save_file_pointer__16(object_write_buffer *buf) {}
void load_file_pointer__16(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Identity__61_51__16__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRIdentity Identity

  ((*____out++)=(*____in++));
  // mark end: SIRIdentity Identity

}}


void work_Identity__61_51__16__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRIdentity Identity

  ((*____out++)=(*____in++));
  // mark end: SIRIdentity Identity

}}


#endif // BUFFER_MERGE


void Identity__61_51__work__16(int ____n) {
    for (; 0 < ____n; ____n--) {
        __push__16(__pop__16());
    }
}


// init counts: 0 steady counts: 16

// ClusterFusion isEliminated: false



int __number_of_iterations_17;
int __counter_17 = 0;
int __steady_17 = 0;
int __tmp_17 = 0;
int __tmp2_17 = 0;
int *__state_flag_17 = NULL;
thread_info *__thread_17 = NULL;




void __joiner_17_work(int ____n) {
  for (;____n > 0; ____n--) {
BUFFER_17_18[HEAD_17_18]=BUFFER_16_17[TAIL_16_17];
HEAD_17_18++; ; TAIL_16_17++;
BUFFER_17_18[HEAD_17_18]=BUFFER_40_17[TAIL_40_17];
HEAD_17_18++; ; TAIL_40_17++;
  }
}


// peek: 2 pop: 2 push 2
// init counts: 0 steady counts: 16

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

 
void init_LatFilt__70_53__18();
inline void check_status__18();

void work_LatFilt__70_53__18(int);


inline float __pop__18() {
float res=BUFFER_17_18[TAIL_17_18];
TAIL_17_18++;
return res;
}

inline float __pop__18(int n) {
float res=BUFFER_17_18[TAIL_17_18];
TAIL_17_18+=n;

return res;
}

inline float __peek__18(int offs) {
return BUFFER_17_18[TAIL_17_18+offs];
}



inline void __push__18(float data) {
BUFFER_18_19[HEAD_18_19]=data;
HEAD_18_19++;
}



 
void init_LatFilt__70_53__18(){
}
void save_file_pointer__18(object_write_buffer *buf) {}
void load_file_pointer__18(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_LatFilt__70_53__18__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float e_i__68 = 0.0f;/* float */
  float e_bar_i__69 = 0.0f;/* float */

  // mark begin: SIRFilter LatFilt

  (e_i__68 = ((float)0.0))/*float*/;
  (e_bar_i__69 = ((float)0.0))/*float*/;
  (e_i__68 = ((*(____in+0)) - (((float)5.0) * (*(____in+1)))))/*float*/;
  (e_bar_i__69 = ((*(____in+1)) - (((float)5.0) * (*(____in+0)))))/*float*/;
  ((*____out++)=e_i__68);
  ((*____out++)=e_bar_i__69);
  assert(false);__pop__18(2);
  // mark end: SIRFilter LatFilt

}}


void work_LatFilt__70_53__18__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float e_i__68 = 0.0f;/* float */
  float e_bar_i__69 = 0.0f;/* float */

  // mark begin: SIRFilter LatFilt

  (e_i__68 = ((float)0.0))/*float*/;
  (e_bar_i__69 = ((float)0.0))/*float*/;
  (e_i__68 = ((*(____in+0)) - (((float)5.0) * (*(____in+1)))))/*float*/;
  (e_bar_i__69 = ((*(____in+1)) - (((float)5.0) * (*(____in+0)))))/*float*/;
  ((*____out++)=e_i__68);
  ((*____out++)=e_bar_i__69);
  assert(false);__pop__18(2);
  // mark end: SIRFilter LatFilt

}}


#endif // BUFFER_MERGE


 
void work_LatFilt__70_53__18(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float e_i__68 = 0.0f;/* float */
      float e_bar_i__69 = 0.0f;/* float */

      // mark begin: SIRFilter LatFilt

      (e_i__68 = ((float)0.0))/*float*/;
      (e_bar_i__69 = ((float)0.0))/*float*/;
      (e_i__68 = (__peek__18(0) - (((float)5.0) * __peek__18(1))))/*float*/;
      (e_bar_i__69 = (__peek__18(1) - (((float)5.0) * __peek__18(0))))/*float*/;
      __push__18(e_i__68);
      __push__18(e_bar_i__69);
      __pop__18(2);
      // mark end: SIRFilter LatFilt

    }
  }
}

// init counts: 0 steady counts: 32

// ClusterFusion isEliminated: false



int __number_of_iterations_19;
int __counter_19 = 0;
int __steady_19 = 0;
int __tmp_19 = 0;
int __tmp2_19 = 0;
int *__state_flag_19 = NULL;
thread_info *__thread_19 = NULL;




void __splitter_19_work(int ____n) {
  for (;____n > 0; ____n--) {
  float tmp;
  tmp = BUFFER_18_19[TAIL_18_19]; TAIL_18_19++;
  BUFFER_19_20[HEAD_19_20]=tmp;
HEAD_19_20++; ;
  BUFFER_19_39[HEAD_19_39]=tmp;
HEAD_19_39++; ;
  }
}


// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 32

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

 
void init_Identity__73_56__20();
inline void check_status__20();

void Identity__73_56__work__20(int);


inline float __pop__20() {
float res=BUFFER_19_20[TAIL_19_20];
TAIL_19_20++;
return res;
}

inline float __pop__20(int n) {
float res=BUFFER_19_20[TAIL_19_20];
TAIL_19_20+=n;

return res;
}

inline float __peek__20(int offs) {
return BUFFER_19_20[TAIL_19_20+offs];
}



inline void __push__20(float data) {
BUFFER_20_21[HEAD_20_21]=data;
HEAD_20_21++;
}



void init_Identity__73_56__20() {
}

void save_file_pointer__20(object_write_buffer *buf) {}
void load_file_pointer__20(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Identity__73_56__20__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRIdentity Identity

  ((*____out++)=(*____in++));
  // mark end: SIRIdentity Identity

}}


void work_Identity__73_56__20__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRIdentity Identity

  ((*____out++)=(*____in++));
  // mark end: SIRIdentity Identity

}}


#endif // BUFFER_MERGE


void Identity__73_56__work__20(int ____n) {
    for (; 0 < ____n; ____n--) {
        __push__20(__pop__20());
    }
}


// init counts: 0 steady counts: 32

// ClusterFusion isEliminated: false



int __number_of_iterations_21;
int __counter_21 = 0;
int __steady_21 = 0;
int __tmp_21 = 0;
int __tmp2_21 = 0;
int *__state_flag_21 = NULL;
thread_info *__thread_21 = NULL;




void __joiner_21_work(int ____n) {
  for (;____n > 0; ____n--) {
BUFFER_21_22[HEAD_21_22]=BUFFER_20_21[TAIL_20_21];
HEAD_21_22++; ; TAIL_20_21++;
BUFFER_21_22[HEAD_21_22]=BUFFER_39_21[TAIL_39_21];
HEAD_21_22++; ; TAIL_39_21++;
  }
}


// peek: 2 pop: 2 push 2
// init counts: 0 steady counts: 32

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

 
void init_LatFilt__82_58__22();
inline void check_status__22();

void work_LatFilt__82_58__22(int);


inline float __pop__22() {
float res=BUFFER_21_22[TAIL_21_22];
TAIL_21_22++;
return res;
}

inline float __pop__22(int n) {
float res=BUFFER_21_22[TAIL_21_22];
TAIL_21_22+=n;

return res;
}

inline float __peek__22(int offs) {
return BUFFER_21_22[TAIL_21_22+offs];
}



inline void __push__22(float data) {
BUFFER_22_23[HEAD_22_23]=data;
HEAD_22_23++;
}



 
void init_LatFilt__82_58__22(){
}
void save_file_pointer__22(object_write_buffer *buf) {}
void load_file_pointer__22(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_LatFilt__82_58__22__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float e_i__80 = 0.0f;/* float */
  float e_bar_i__81 = 0.0f;/* float */

  // mark begin: SIRFilter LatFilt

  (e_i__80 = ((float)0.0))/*float*/;
  (e_bar_i__81 = ((float)0.0))/*float*/;
  (e_i__80 = ((*(____in+0)) - (((float)6.0) * (*(____in+1)))))/*float*/;
  (e_bar_i__81 = ((*(____in+1)) - (((float)6.0) * (*(____in+0)))))/*float*/;
  ((*____out++)=e_i__80);
  ((*____out++)=e_bar_i__81);
  assert(false);__pop__22(2);
  // mark end: SIRFilter LatFilt

}}


void work_LatFilt__82_58__22__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float e_i__80 = 0.0f;/* float */
  float e_bar_i__81 = 0.0f;/* float */

  // mark begin: SIRFilter LatFilt

  (e_i__80 = ((float)0.0))/*float*/;
  (e_bar_i__81 = ((float)0.0))/*float*/;
  (e_i__80 = ((*(____in+0)) - (((float)6.0) * (*(____in+1)))))/*float*/;
  (e_bar_i__81 = ((*(____in+1)) - (((float)6.0) * (*(____in+0)))))/*float*/;
  ((*____out++)=e_i__80);
  ((*____out++)=e_bar_i__81);
  assert(false);__pop__22(2);
  // mark end: SIRFilter LatFilt

}}


#endif // BUFFER_MERGE


 
void work_LatFilt__82_58__22(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float e_i__80 = 0.0f;/* float */
      float e_bar_i__81 = 0.0f;/* float */

      // mark begin: SIRFilter LatFilt

      (e_i__80 = ((float)0.0))/*float*/;
      (e_bar_i__81 = ((float)0.0))/*float*/;
      (e_i__80 = (__peek__22(0) - (((float)6.0) * __peek__22(1))))/*float*/;
      (e_bar_i__81 = (__peek__22(1) - (((float)6.0) * __peek__22(0))))/*float*/;
      __push__22(e_i__80);
      __push__22(e_bar_i__81);
      __pop__22(2);
      // mark end: SIRFilter LatFilt

    }
  }
}

// init counts: 0 steady counts: 64

// ClusterFusion isEliminated: false



int __number_of_iterations_23;
int __counter_23 = 0;
int __steady_23 = 0;
int __tmp_23 = 0;
int __tmp2_23 = 0;
int *__state_flag_23 = NULL;
thread_info *__thread_23 = NULL;




void __splitter_23_work(int ____n) {
  for (;____n > 0; ____n--) {
  float tmp;
  tmp = BUFFER_22_23[TAIL_22_23]; TAIL_22_23++;
  BUFFER_23_24[HEAD_23_24]=tmp;
HEAD_23_24++; ;
  BUFFER_23_38[HEAD_23_38]=tmp;
HEAD_23_38++; ;
  }
}


// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 64

// ClusterFusion isEliminated: false



int __number_of_iterations_24;
int __counter_24 = 0;
int __steady_24 = 0;
int __tmp_24 = 0;
int __tmp2_24 = 0;
int *__state_flag_24 = NULL;
thread_info *__thread_24 = NULL;



void save_peek_buffer__24(object_write_buffer *buf);
void load_peek_buffer__24(object_write_buffer *buf);
void save_file_pointer__24(object_write_buffer *buf);
void load_file_pointer__24(object_write_buffer *buf);

 
void init_Identity__85_61__24();
inline void check_status__24();

void Identity__85_61__work__24(int);


inline float __pop__24() {
float res=BUFFER_23_24[TAIL_23_24];
TAIL_23_24++;
return res;
}

inline float __pop__24(int n) {
float res=BUFFER_23_24[TAIL_23_24];
TAIL_23_24+=n;

return res;
}

inline float __peek__24(int offs) {
return BUFFER_23_24[TAIL_23_24+offs];
}



inline void __push__24(float data) {
BUFFER_24_25[HEAD_24_25]=data;
HEAD_24_25++;
}



void init_Identity__85_61__24() {
}

void save_file_pointer__24(object_write_buffer *buf) {}
void load_file_pointer__24(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Identity__85_61__24__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRIdentity Identity

  ((*____out++)=(*____in++));
  // mark end: SIRIdentity Identity

}}


void work_Identity__85_61__24__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRIdentity Identity

  ((*____out++)=(*____in++));
  // mark end: SIRIdentity Identity

}}


#endif // BUFFER_MERGE


void Identity__85_61__work__24(int ____n) {
    for (; 0 < ____n; ____n--) {
        __push__24(__pop__24());
    }
}


// init counts: 0 steady counts: 64

// ClusterFusion isEliminated: false



int __number_of_iterations_25;
int __counter_25 = 0;
int __steady_25 = 0;
int __tmp_25 = 0;
int __tmp2_25 = 0;
int *__state_flag_25 = NULL;
thread_info *__thread_25 = NULL;




void __joiner_25_work(int ____n) {
  for (;____n > 0; ____n--) {
BUFFER_25_26[HEAD_25_26]=BUFFER_24_25[TAIL_24_25];
HEAD_25_26++; ; TAIL_24_25++;
BUFFER_25_26[HEAD_25_26]=BUFFER_38_25[TAIL_38_25];
HEAD_25_26++; ; TAIL_38_25++;
  }
}


// peek: 2 pop: 2 push 2
// init counts: 0 steady counts: 64

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

 
void init_LatFilt__94_63__26();
inline void check_status__26();

void work_LatFilt__94_63__26(int);


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



 
void init_LatFilt__94_63__26(){
}
void save_file_pointer__26(object_write_buffer *buf) {}
void load_file_pointer__26(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_LatFilt__94_63__26__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float e_i__92 = 0.0f;/* float */
  float e_bar_i__93 = 0.0f;/* float */

  // mark begin: SIRFilter LatFilt

  (e_i__92 = ((float)0.0))/*float*/;
  (e_bar_i__93 = ((float)0.0))/*float*/;
  (e_i__92 = ((*(____in+0)) - (((float)7.0) * (*(____in+1)))))/*float*/;
  (e_bar_i__93 = ((*(____in+1)) - (((float)7.0) * (*(____in+0)))))/*float*/;
  ((*____out++)=e_i__92);
  ((*____out++)=e_bar_i__93);
  assert(false);__pop__26(2);
  // mark end: SIRFilter LatFilt

}}


void work_LatFilt__94_63__26__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float e_i__92 = 0.0f;/* float */
  float e_bar_i__93 = 0.0f;/* float */

  // mark begin: SIRFilter LatFilt

  (e_i__92 = ((float)0.0))/*float*/;
  (e_bar_i__93 = ((float)0.0))/*float*/;
  (e_i__92 = ((*(____in+0)) - (((float)7.0) * (*(____in+1)))))/*float*/;
  (e_bar_i__93 = ((*(____in+1)) - (((float)7.0) * (*(____in+0)))))/*float*/;
  ((*____out++)=e_i__92);
  ((*____out++)=e_bar_i__93);
  assert(false);__pop__26(2);
  // mark end: SIRFilter LatFilt

}}


#endif // BUFFER_MERGE


 
void work_LatFilt__94_63__26(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float e_i__92 = 0.0f;/* float */
      float e_bar_i__93 = 0.0f;/* float */

      // mark begin: SIRFilter LatFilt

      (e_i__92 = ((float)0.0))/*float*/;
      (e_bar_i__93 = ((float)0.0))/*float*/;
      (e_i__92 = (__peek__26(0) - (((float)7.0) * __peek__26(1))))/*float*/;
      (e_bar_i__93 = (__peek__26(1) - (((float)7.0) * __peek__26(0))))/*float*/;
      __push__26(e_i__92);
      __push__26(e_bar_i__93);
      __pop__26(2);
      // mark end: SIRFilter LatFilt

    }
  }
}

// init counts: 0 steady counts: 128

// ClusterFusion isEliminated: false



int __number_of_iterations_27;
int __counter_27 = 0;
int __steady_27 = 0;
int __tmp_27 = 0;
int __tmp2_27 = 0;
int *__state_flag_27 = NULL;
thread_info *__thread_27 = NULL;




void __splitter_27_work(int ____n) {
  for (;____n > 0; ____n--) {
  float tmp;
  tmp = BUFFER_26_27[TAIL_26_27]; TAIL_26_27++;
  BUFFER_27_28[HEAD_27_28]=tmp;
HEAD_27_28++; ;
  BUFFER_27_37[HEAD_27_37]=tmp;
HEAD_27_37++; ;
  }
}


// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 128

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

 
void init_Identity__97_66__28();
inline void check_status__28();

void Identity__97_66__work__28(int);


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



void init_Identity__97_66__28() {
}

void save_file_pointer__28(object_write_buffer *buf) {}
void load_file_pointer__28(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Identity__97_66__28__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRIdentity Identity

  ((*____out++)=(*____in++));
  // mark end: SIRIdentity Identity

}}


void work_Identity__97_66__28__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRIdentity Identity

  ((*____out++)=(*____in++));
  // mark end: SIRIdentity Identity

}}


#endif // BUFFER_MERGE


void Identity__97_66__work__28(int ____n) {
    for (; 0 < ____n; ____n--) {
        __push__28(__pop__28());
    }
}


// init counts: 0 steady counts: 128

// ClusterFusion isEliminated: false



int __number_of_iterations_29;
int __counter_29 = 0;
int __steady_29 = 0;
int __tmp_29 = 0;
int __tmp2_29 = 0;
int *__state_flag_29 = NULL;
thread_info *__thread_29 = NULL;




void __joiner_29_work(int ____n) {
  for (;____n > 0; ____n--) {
BUFFER_29_30[HEAD_29_30]=BUFFER_28_29[TAIL_28_29];
HEAD_29_30++; ; TAIL_28_29++;
BUFFER_29_30[HEAD_29_30]=BUFFER_37_29[TAIL_37_29];
HEAD_29_30++; ; TAIL_37_29++;
  }
}


// peek: 2 pop: 2 push 2
// init counts: 0 steady counts: 128

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

 
void init_LatFilt__106_68__30();
inline void check_status__30();

void work_LatFilt__106_68__30(int);


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



inline void __push__30(float data) {
BUFFER_30_31[HEAD_30_31]=data;
HEAD_30_31++;
}



 
void init_LatFilt__106_68__30(){
}
void save_file_pointer__30(object_write_buffer *buf) {}
void load_file_pointer__30(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_LatFilt__106_68__30__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float e_i__104 = 0.0f;/* float */
  float e_bar_i__105 = 0.0f;/* float */

  // mark begin: SIRFilter LatFilt

  (e_i__104 = ((float)0.0))/*float*/;
  (e_bar_i__105 = ((float)0.0))/*float*/;
  (e_i__104 = ((*(____in+0)) - (((float)8.0) * (*(____in+1)))))/*float*/;
  (e_bar_i__105 = ((*(____in+1)) - (((float)8.0) * (*(____in+0)))))/*float*/;
  ((*____out++)=e_i__104);
  ((*____out++)=e_bar_i__105);
  assert(false);__pop__30(2);
  // mark end: SIRFilter LatFilt

}}


void work_LatFilt__106_68__30__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float e_i__104 = 0.0f;/* float */
  float e_bar_i__105 = 0.0f;/* float */

  // mark begin: SIRFilter LatFilt

  (e_i__104 = ((float)0.0))/*float*/;
  (e_bar_i__105 = ((float)0.0))/*float*/;
  (e_i__104 = ((*(____in+0)) - (((float)8.0) * (*(____in+1)))))/*float*/;
  (e_bar_i__105 = ((*(____in+1)) - (((float)8.0) * (*(____in+0)))))/*float*/;
  ((*____out++)=e_i__104);
  ((*____out++)=e_bar_i__105);
  assert(false);__pop__30(2);
  // mark end: SIRFilter LatFilt

}}


#endif // BUFFER_MERGE


 
void work_LatFilt__106_68__30(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float e_i__104 = 0.0f;/* float */
      float e_bar_i__105 = 0.0f;/* float */

      // mark begin: SIRFilter LatFilt

      (e_i__104 = ((float)0.0))/*float*/;
      (e_bar_i__105 = ((float)0.0))/*float*/;
      (e_i__104 = (__peek__30(0) - (((float)8.0) * __peek__30(1))))/*float*/;
      (e_bar_i__105 = (__peek__30(1) - (((float)8.0) * __peek__30(0))))/*float*/;
      __push__30(e_i__104);
      __push__30(e_bar_i__105);
      __pop__30(2);
      // mark end: SIRFilter LatFilt

    }
  }
}

// init counts: 0 steady counts: 256

// ClusterFusion isEliminated: false



int __number_of_iterations_31;
int __counter_31 = 0;
int __steady_31 = 0;
int __tmp_31 = 0;
int __tmp2_31 = 0;
int *__state_flag_31 = NULL;
thread_info *__thread_31 = NULL;




void __splitter_31_work(int ____n) {
  for (;____n > 0; ____n--) {
  float tmp;
  tmp = BUFFER_30_31[TAIL_30_31]; TAIL_30_31++;
  BUFFER_31_32[HEAD_31_32]=tmp;
HEAD_31_32++; ;
  BUFFER_31_36[HEAD_31_36]=tmp;
HEAD_31_36++; ;
  }
}


// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 256

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

 
void init_Identity__109_71__32();
inline void check_status__32();

void Identity__109_71__work__32(int);


inline float __pop__32() {
float res=BUFFER_31_32[TAIL_31_32];
TAIL_31_32++;
return res;
}

inline float __pop__32(int n) {
float res=BUFFER_31_32[TAIL_31_32];
TAIL_31_32+=n;

return res;
}

inline float __peek__32(int offs) {
return BUFFER_31_32[TAIL_31_32+offs];
}



inline void __push__32(float data) {
BUFFER_32_33[HEAD_32_33]=data;
HEAD_32_33++;
}



void init_Identity__109_71__32() {
}

void save_file_pointer__32(object_write_buffer *buf) {}
void load_file_pointer__32(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Identity__109_71__32__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRIdentity Identity

  ((*____out++)=(*____in++));
  // mark end: SIRIdentity Identity

}}


void work_Identity__109_71__32__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRIdentity Identity

  ((*____out++)=(*____in++));
  // mark end: SIRIdentity Identity

}}


#endif // BUFFER_MERGE


void Identity__109_71__work__32(int ____n) {
    for (; 0 < ____n; ____n--) {
        __push__32(__pop__32());
    }
}


// init counts: 0 steady counts: 256

// ClusterFusion isEliminated: false



int __number_of_iterations_33;
int __counter_33 = 0;
int __steady_33 = 0;
int __tmp_33 = 0;
int __tmp2_33 = 0;
int *__state_flag_33 = NULL;
thread_info *__thread_33 = NULL;




void __joiner_33_work(int ____n) {
  for (;____n > 0; ____n--) {
BUFFER_33_34[HEAD_33_34]=BUFFER_32_33[TAIL_32_33];
HEAD_33_34++; ; TAIL_32_33++;
BUFFER_33_34[HEAD_33_34]=BUFFER_36_33[TAIL_36_33];
HEAD_33_34++; ; TAIL_36_33++;
  }
}


// peek: 2 pop: 2 push 2
// init counts: 0 steady counts: 256

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

 
void init_LatFilt__118_73__34();
inline void check_status__34();

void work_LatFilt__118_73__34(int);


inline float __pop__34() {
float res=BUFFER_33_34[TAIL_33_34];
TAIL_33_34++;
return res;
}

inline float __pop__34(int n) {
float res=BUFFER_33_34[TAIL_33_34];
TAIL_33_34+=n;

return res;
}

inline float __peek__34(int offs) {
return BUFFER_33_34[TAIL_33_34+offs];
}



inline void __push__34(float data) {
BUFFER_34_35[HEAD_34_35]=data;
HEAD_34_35++;
}



 
void init_LatFilt__118_73__34(){
}
void save_file_pointer__34(object_write_buffer *buf) {}
void load_file_pointer__34(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_LatFilt__118_73__34__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float e_i__116 = 0.0f;/* float */
  float e_bar_i__117 = 0.0f;/* float */

  // mark begin: SIRFilter LatFilt

  (e_i__116 = ((float)0.0))/*float*/;
  (e_bar_i__117 = ((float)0.0))/*float*/;
  (e_i__116 = ((*(____in+0)) - (((float)9.0) * (*(____in+1)))))/*float*/;
  (e_bar_i__117 = ((*(____in+1)) - (((float)9.0) * (*(____in+0)))))/*float*/;
  ((*____out++)=e_i__116);
  ((*____out++)=e_bar_i__117);
  assert(false);__pop__34(2);
  // mark end: SIRFilter LatFilt

}}


void work_LatFilt__118_73__34__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float e_i__116 = 0.0f;/* float */
  float e_bar_i__117 = 0.0f;/* float */

  // mark begin: SIRFilter LatFilt

  (e_i__116 = ((float)0.0))/*float*/;
  (e_bar_i__117 = ((float)0.0))/*float*/;
  (e_i__116 = ((*(____in+0)) - (((float)9.0) * (*(____in+1)))))/*float*/;
  (e_bar_i__117 = ((*(____in+1)) - (((float)9.0) * (*(____in+0)))))/*float*/;
  ((*____out++)=e_i__116);
  ((*____out++)=e_bar_i__117);
  assert(false);__pop__34(2);
  // mark end: SIRFilter LatFilt

}}


#endif // BUFFER_MERGE


 
void work_LatFilt__118_73__34(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float e_i__116 = 0.0f;/* float */
      float e_bar_i__117 = 0.0f;/* float */

      // mark begin: SIRFilter LatFilt

      (e_i__116 = ((float)0.0))/*float*/;
      (e_bar_i__117 = ((float)0.0))/*float*/;
      (e_i__116 = (__peek__34(0) - (((float)9.0) * __peek__34(1))))/*float*/;
      (e_bar_i__117 = (__peek__34(1) - (((float)9.0) * __peek__34(0))))/*float*/;
      __push__34(e_i__116);
      __push__34(e_bar_i__117);
      __pop__34(2);
      // mark end: SIRFilter LatFilt

    }
  }
}

// peek: 2 pop: 2 push 0
// init counts: 0 steady counts: 256

// ClusterFusion isEliminated: false



int __number_of_iterations_35;
int __counter_35 = 0;
int __steady_35 = 0;
int __tmp_35 = 0;
int __tmp2_35 = 0;
int *__state_flag_35 = NULL;
thread_info *__thread_35 = NULL;



float x__119__35 = 0.0f;
void save_peek_buffer__35(object_write_buffer *buf);
void load_peek_buffer__35(object_write_buffer *buf);
void save_file_pointer__35(object_write_buffer *buf);
void load_file_pointer__35(object_write_buffer *buf);

 
void init_LastStage__122_74__35();
inline void check_status__35();

void work_LastStage__122_74__35(int);


inline float __pop__35() {
float res=BUFFER_34_35[TAIL_34_35];
TAIL_34_35++;
return res;
}

inline float __pop__35(int n) {
float res=BUFFER_34_35[TAIL_34_35];
TAIL_34_35+=n;

return res;
}

inline float __peek__35(int offs) {
return BUFFER_34_35[TAIL_34_35+offs];
}


 
void init_LastStage__122_74__35(){
}
void save_file_pointer__35(object_write_buffer *buf) {}
void load_file_pointer__35(object_write_buffer *buf) {}
 
void work_LastStage__122_74__35(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter LastStage

      ((x__119__35) = __pop__35())/*float*/;
      sum+=x__119__35;
      ((x__119__35) = __pop__35())/*float*/;
      sum+=x__119__35;
      // mark end: SIRFilter LastStage

    }
  }
}

// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 256

// ClusterFusion isEliminated: false



int __number_of_iterations_36;
int __counter_36 = 0;
int __steady_36 = 0;
int __tmp_36 = 0;
int __tmp2_36 = 0;
int *__state_flag_36 = NULL;
thread_info *__thread_36 = NULL;



float last__110__36 = 0.0f;
void save_peek_buffer__36(object_write_buffer *buf);
void load_peek_buffer__36(object_write_buffer *buf);
void save_file_pointer__36(object_write_buffer *buf);
void load_file_pointer__36(object_write_buffer *buf);

 
void init_DelayOne__113_72__36();
inline void check_status__36();

void work_DelayOne__113_72__36(int);


inline float __pop__36() {
float res=BUFFER_31_36[TAIL_31_36];
TAIL_31_36++;
return res;
}

inline float __pop__36(int n) {
float res=BUFFER_31_36[TAIL_31_36];
TAIL_31_36+=n;

return res;
}

inline float __peek__36(int offs) {
return BUFFER_31_36[TAIL_31_36+offs];
}



inline void __push__36(float data) {
BUFFER_36_33[HEAD_36_33]=data;
HEAD_36_33++;
}



 
void init_DelayOne__113_72__36(){
  ((last__110__36) = ((float)0.0))/*float*/;
}
void save_file_pointer__36(object_write_buffer *buf) {}
void load_file_pointer__36(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_DelayOne__113_72__36__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRFilter DelayOne

  ((*____out++)=(last__110__36));
  ((last__110__36) = (*____in++))/*float*/;
  // mark end: SIRFilter DelayOne

}}


void work_DelayOne__113_72__36__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRFilter DelayOne

  ((*____out++)=(last__110__36));
  ((last__110__36) = (*____in++))/*float*/;
  // mark end: SIRFilter DelayOne

}}


#endif // BUFFER_MERGE


 
void work_DelayOne__113_72__36(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter DelayOne

      __push__36((last__110__36));
      ((last__110__36) = __pop__36())/*float*/;
      // mark end: SIRFilter DelayOne

    }
  }
}

// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 128

// ClusterFusion isEliminated: false



int __number_of_iterations_37;
int __counter_37 = 0;
int __steady_37 = 0;
int __tmp_37 = 0;
int __tmp2_37 = 0;
int *__state_flag_37 = NULL;
thread_info *__thread_37 = NULL;



float last__98__37 = 0.0f;
void save_peek_buffer__37(object_write_buffer *buf);
void load_peek_buffer__37(object_write_buffer *buf);
void save_file_pointer__37(object_write_buffer *buf);
void load_file_pointer__37(object_write_buffer *buf);

 
void init_DelayOne__101_67__37();
inline void check_status__37();

void work_DelayOne__101_67__37(int);


inline float __pop__37() {
float res=BUFFER_27_37[TAIL_27_37];
TAIL_27_37++;
return res;
}

inline float __pop__37(int n) {
float res=BUFFER_27_37[TAIL_27_37];
TAIL_27_37+=n;

return res;
}

inline float __peek__37(int offs) {
return BUFFER_27_37[TAIL_27_37+offs];
}



inline void __push__37(float data) {
BUFFER_37_29[HEAD_37_29]=data;
HEAD_37_29++;
}



 
void init_DelayOne__101_67__37(){
  ((last__98__37) = ((float)0.0))/*float*/;
}
void save_file_pointer__37(object_write_buffer *buf) {}
void load_file_pointer__37(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_DelayOne__101_67__37__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRFilter DelayOne

  ((*____out++)=(last__98__37));
  ((last__98__37) = (*____in++))/*float*/;
  // mark end: SIRFilter DelayOne

}}


void work_DelayOne__101_67__37__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRFilter DelayOne

  ((*____out++)=(last__98__37));
  ((last__98__37) = (*____in++))/*float*/;
  // mark end: SIRFilter DelayOne

}}


#endif // BUFFER_MERGE


 
void work_DelayOne__101_67__37(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter DelayOne

      __push__37((last__98__37));
      ((last__98__37) = __pop__37())/*float*/;
      // mark end: SIRFilter DelayOne

    }
  }
}

// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 64

// ClusterFusion isEliminated: false



int __number_of_iterations_38;
int __counter_38 = 0;
int __steady_38 = 0;
int __tmp_38 = 0;
int __tmp2_38 = 0;
int *__state_flag_38 = NULL;
thread_info *__thread_38 = NULL;



float last__86__38 = 0.0f;
void save_peek_buffer__38(object_write_buffer *buf);
void load_peek_buffer__38(object_write_buffer *buf);
void save_file_pointer__38(object_write_buffer *buf);
void load_file_pointer__38(object_write_buffer *buf);

 
void init_DelayOne__89_62__38();
inline void check_status__38();

void work_DelayOne__89_62__38(int);


inline float __pop__38() {
float res=BUFFER_23_38[TAIL_23_38];
TAIL_23_38++;
return res;
}

inline float __pop__38(int n) {
float res=BUFFER_23_38[TAIL_23_38];
TAIL_23_38+=n;

return res;
}

inline float __peek__38(int offs) {
return BUFFER_23_38[TAIL_23_38+offs];
}



inline void __push__38(float data) {
BUFFER_38_25[HEAD_38_25]=data;
HEAD_38_25++;
}



 
void init_DelayOne__89_62__38(){
  ((last__86__38) = ((float)0.0))/*float*/;
}
void save_file_pointer__38(object_write_buffer *buf) {}
void load_file_pointer__38(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_DelayOne__89_62__38__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRFilter DelayOne

  ((*____out++)=(last__86__38));
  ((last__86__38) = (*____in++))/*float*/;
  // mark end: SIRFilter DelayOne

}}


void work_DelayOne__89_62__38__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRFilter DelayOne

  ((*____out++)=(last__86__38));
  ((last__86__38) = (*____in++))/*float*/;
  // mark end: SIRFilter DelayOne

}}


#endif // BUFFER_MERGE


 
void work_DelayOne__89_62__38(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter DelayOne

      __push__38((last__86__38));
      ((last__86__38) = __pop__38())/*float*/;
      // mark end: SIRFilter DelayOne

    }
  }
}

// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 32

// ClusterFusion isEliminated: false



int __number_of_iterations_39;
int __counter_39 = 0;
int __steady_39 = 0;
int __tmp_39 = 0;
int __tmp2_39 = 0;
int *__state_flag_39 = NULL;
thread_info *__thread_39 = NULL;



float last__74__39 = 0.0f;
void save_peek_buffer__39(object_write_buffer *buf);
void load_peek_buffer__39(object_write_buffer *buf);
void save_file_pointer__39(object_write_buffer *buf);
void load_file_pointer__39(object_write_buffer *buf);

 
void init_DelayOne__77_57__39();
inline void check_status__39();

void work_DelayOne__77_57__39(int);


inline float __pop__39() {
float res=BUFFER_19_39[TAIL_19_39];
TAIL_19_39++;
return res;
}

inline float __pop__39(int n) {
float res=BUFFER_19_39[TAIL_19_39];
TAIL_19_39+=n;

return res;
}

inline float __peek__39(int offs) {
return BUFFER_19_39[TAIL_19_39+offs];
}



inline void __push__39(float data) {
BUFFER_39_21[HEAD_39_21]=data;
HEAD_39_21++;
}



 
void init_DelayOne__77_57__39(){
  ((last__74__39) = ((float)0.0))/*float*/;
}
void save_file_pointer__39(object_write_buffer *buf) {}
void load_file_pointer__39(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_DelayOne__77_57__39__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRFilter DelayOne

  ((*____out++)=(last__74__39));
  ((last__74__39) = (*____in++))/*float*/;
  // mark end: SIRFilter DelayOne

}}


void work_DelayOne__77_57__39__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRFilter DelayOne

  ((*____out++)=(last__74__39));
  ((last__74__39) = (*____in++))/*float*/;
  // mark end: SIRFilter DelayOne

}}


#endif // BUFFER_MERGE


 
void work_DelayOne__77_57__39(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter DelayOne

      __push__39((last__74__39));
      ((last__74__39) = __pop__39())/*float*/;
      // mark end: SIRFilter DelayOne

    }
  }
}

// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 16

// ClusterFusion isEliminated: false



int __number_of_iterations_40;
int __counter_40 = 0;
int __steady_40 = 0;
int __tmp_40 = 0;
int __tmp2_40 = 0;
int *__state_flag_40 = NULL;
thread_info *__thread_40 = NULL;



float last__62__40 = 0.0f;
void save_peek_buffer__40(object_write_buffer *buf);
void load_peek_buffer__40(object_write_buffer *buf);
void save_file_pointer__40(object_write_buffer *buf);
void load_file_pointer__40(object_write_buffer *buf);

 
void init_DelayOne__65_52__40();
inline void check_status__40();

void work_DelayOne__65_52__40(int);


inline float __pop__40() {
float res=BUFFER_15_40[TAIL_15_40];
TAIL_15_40++;
return res;
}

inline float __pop__40(int n) {
float res=BUFFER_15_40[TAIL_15_40];
TAIL_15_40+=n;

return res;
}

inline float __peek__40(int offs) {
return BUFFER_15_40[TAIL_15_40+offs];
}



inline void __push__40(float data) {
BUFFER_40_17[HEAD_40_17]=data;
HEAD_40_17++;
}



 
void init_DelayOne__65_52__40(){
  ((last__62__40) = ((float)0.0))/*float*/;
}
void save_file_pointer__40(object_write_buffer *buf) {}
void load_file_pointer__40(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_DelayOne__65_52__40__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRFilter DelayOne

  ((*____out++)=(last__62__40));
  ((last__62__40) = (*____in++))/*float*/;
  // mark end: SIRFilter DelayOne

}}


void work_DelayOne__65_52__40__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRFilter DelayOne

  ((*____out++)=(last__62__40));
  ((last__62__40) = (*____in++))/*float*/;
  // mark end: SIRFilter DelayOne

}}


#endif // BUFFER_MERGE


 
void work_DelayOne__65_52__40(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter DelayOne

      __push__40((last__62__40));
      ((last__62__40) = __pop__40())/*float*/;
      // mark end: SIRFilter DelayOne

    }
  }
}

// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 8

// ClusterFusion isEliminated: false



int __number_of_iterations_41;
int __counter_41 = 0;
int __steady_41 = 0;
int __tmp_41 = 0;
int __tmp2_41 = 0;
int *__state_flag_41 = NULL;
thread_info *__thread_41 = NULL;



float last__50__41 = 0.0f;
void save_peek_buffer__41(object_write_buffer *buf);
void load_peek_buffer__41(object_write_buffer *buf);
void save_file_pointer__41(object_write_buffer *buf);
void load_file_pointer__41(object_write_buffer *buf);

 
void init_DelayOne__53_47__41();
inline void check_status__41();

void work_DelayOne__53_47__41(int);


inline float __pop__41() {
float res=BUFFER_11_41[TAIL_11_41];
TAIL_11_41++;
return res;
}

inline float __pop__41(int n) {
float res=BUFFER_11_41[TAIL_11_41];
TAIL_11_41+=n;

return res;
}

inline float __peek__41(int offs) {
return BUFFER_11_41[TAIL_11_41+offs];
}



inline void __push__41(float data) {
BUFFER_41_13[HEAD_41_13]=data;
HEAD_41_13++;
}



 
void init_DelayOne__53_47__41(){
  ((last__50__41) = ((float)0.0))/*float*/;
}
void save_file_pointer__41(object_write_buffer *buf) {}
void load_file_pointer__41(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_DelayOne__53_47__41__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRFilter DelayOne

  ((*____out++)=(last__50__41));
  ((last__50__41) = (*____in++))/*float*/;
  // mark end: SIRFilter DelayOne

}}


void work_DelayOne__53_47__41__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRFilter DelayOne

  ((*____out++)=(last__50__41));
  ((last__50__41) = (*____in++))/*float*/;
  // mark end: SIRFilter DelayOne

}}


#endif // BUFFER_MERGE


 
void work_DelayOne__53_47__41(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter DelayOne

      __push__41((last__50__41));
      ((last__50__41) = __pop__41())/*float*/;
      // mark end: SIRFilter DelayOne

    }
  }
}

// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 4

// ClusterFusion isEliminated: false



int __number_of_iterations_42;
int __counter_42 = 0;
int __steady_42 = 0;
int __tmp_42 = 0;
int __tmp2_42 = 0;
int *__state_flag_42 = NULL;
thread_info *__thread_42 = NULL;



float last__38__42 = 0.0f;
void save_peek_buffer__42(object_write_buffer *buf);
void load_peek_buffer__42(object_write_buffer *buf);
void save_file_pointer__42(object_write_buffer *buf);
void load_file_pointer__42(object_write_buffer *buf);

 
void init_DelayOne__41_42__42();
inline void check_status__42();

void work_DelayOne__41_42__42(int);


inline float __pop__42() {
float res=BUFFER_7_42[TAIL_7_42];
TAIL_7_42++;
return res;
}

inline float __pop__42(int n) {
float res=BUFFER_7_42[TAIL_7_42];
TAIL_7_42+=n;

return res;
}

inline float __peek__42(int offs) {
return BUFFER_7_42[TAIL_7_42+offs];
}



inline void __push__42(float data) {
BUFFER_42_9[HEAD_42_9]=data;
HEAD_42_9++;
}



 
void init_DelayOne__41_42__42(){
  ((last__38__42) = ((float)0.0))/*float*/;
}
void save_file_pointer__42(object_write_buffer *buf) {}
void load_file_pointer__42(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_DelayOne__41_42__42__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRFilter DelayOne

  ((*____out++)=(last__38__42));
  ((last__38__42) = (*____in++))/*float*/;
  // mark end: SIRFilter DelayOne

}}


void work_DelayOne__41_42__42__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRFilter DelayOne

  ((*____out++)=(last__38__42));
  ((last__38__42) = (*____in++))/*float*/;
  // mark end: SIRFilter DelayOne

}}


#endif // BUFFER_MERGE


 
void work_DelayOne__41_42__42(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter DelayOne

      __push__42((last__38__42));
      ((last__38__42) = __pop__42())/*float*/;
      // mark end: SIRFilter DelayOne

    }
  }
}

// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 2

// ClusterFusion isEliminated: false



int __number_of_iterations_43;
int __counter_43 = 0;
int __steady_43 = 0;
int __tmp_43 = 0;
int __tmp2_43 = 0;
int *__state_flag_43 = NULL;
thread_info *__thread_43 = NULL;



float last__26__43 = 0.0f;
void save_peek_buffer__43(object_write_buffer *buf);
void load_peek_buffer__43(object_write_buffer *buf);
void save_file_pointer__43(object_write_buffer *buf);
void load_file_pointer__43(object_write_buffer *buf);

 
void init_DelayOne__29_37__43();
inline void check_status__43();

void work_DelayOne__29_37__43(int);


inline float __pop__43() {
float res=BUFFER_3_43[TAIL_3_43];
TAIL_3_43++;
return res;
}

inline float __pop__43(int n) {
float res=BUFFER_3_43[TAIL_3_43];
TAIL_3_43+=n;

return res;
}

inline float __peek__43(int offs) {
return BUFFER_3_43[TAIL_3_43+offs];
}



inline void __push__43(float data) {
BUFFER_43_5[HEAD_43_5]=data;
HEAD_43_5++;
}



 
void init_DelayOne__29_37__43(){
  ((last__26__43) = ((float)0.0))/*float*/;
}
void save_file_pointer__43(object_write_buffer *buf) {}
void load_file_pointer__43(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_DelayOne__29_37__43__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRFilter DelayOne

  ((*____out++)=(last__26__43));
  ((last__26__43) = (*____in++))/*float*/;
  // mark end: SIRFilter DelayOne

}}


void work_DelayOne__29_37__43__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRFilter DelayOne

  ((*____out++)=(last__26__43));
  ((last__26__43) = (*____in++))/*float*/;
  // mark end: SIRFilter DelayOne

}}


#endif // BUFFER_MERGE


 
void work_DelayOne__29_37__43(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter DelayOne

      __push__43((last__26__43));
      ((last__26__43) = __pop__43())/*float*/;
      // mark end: SIRFilter DelayOne

    }
  }
}

