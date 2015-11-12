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

static unsigned int seed;
volatile double sum = 0;

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
int BUFFER_1_25[__BUF_SIZE_MASK_1_25 + 1];
int HEAD_1_25 = 0;
int TAIL_1_25 = 0;
int BUFFER_1_28[__BUF_SIZE_MASK_1_28 + 1];
int HEAD_1_28 = 0;
int TAIL_1_28 = 0;
int BUFFER_1_31[__BUF_SIZE_MASK_1_31 + 1];
int HEAD_1_31 = 0;
int TAIL_1_31 = 0;
int BUFFER_1_34[__BUF_SIZE_MASK_1_34 + 1];
int HEAD_1_34 = 0;
int TAIL_1_34 = 0;
int BUFFER_1_37[__BUF_SIZE_MASK_1_37 + 1];
int HEAD_1_37 = 0;
int TAIL_1_37 = 0;
int BUFFER_1_40[__BUF_SIZE_MASK_1_40 + 1];
int HEAD_1_40 = 0;
int TAIL_1_40 = 0;
int BUFFER_1_43[__BUF_SIZE_MASK_1_43 + 1];
int HEAD_1_43 = 0;
int TAIL_1_43 = 0;
int BUFFER_1_46[__BUF_SIZE_MASK_1_46 + 1];
int HEAD_1_46 = 0;
int TAIL_1_46 = 0;
int BUFFER_1_49[__BUF_SIZE_MASK_1_49 + 1];
int HEAD_1_49 = 0;
int TAIL_1_49 = 0;
int BUFFER_1_52[__BUF_SIZE_MASK_1_52 + 1];
int HEAD_1_52 = 0;
int TAIL_1_52 = 0;
int BUFFER_1_55[__BUF_SIZE_MASK_1_55 + 1];
int HEAD_1_55 = 0;
int TAIL_1_55 = 0;
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
float BUFFER_6_13[__BUF_SIZE_MASK_6_13 + 1];
int HEAD_6_13 = 0;
int TAIL_6_13 = 0;
float BUFFER_6_17[__BUF_SIZE_MASK_6_17 + 1];
int HEAD_6_17 = 0;
int TAIL_6_17 = 0;
float BUFFER_6_21[__BUF_SIZE_MASK_6_21 + 1];
int HEAD_6_21 = 0;
int TAIL_6_21 = 0;
float BUFFER_7_8[__BUF_SIZE_MASK_7_8 + 1];
int HEAD_7_8 = 0;
int TAIL_7_8 = 0;
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
float BUFFER_13_14[__BUF_SIZE_MASK_13_14 + 1];
int HEAD_13_14 = 0;
int TAIL_13_14 = 0;
float BUFFER_14_15[__BUF_SIZE_MASK_14_15 + 1];
int HEAD_14_15 = 0;
int TAIL_14_15 = 0;
float BUFFER_15_16[__BUF_SIZE_MASK_15_16 + 1];
int HEAD_15_16 = 0;
int TAIL_15_16 = 0;
float BUFFER_16_11[__BUF_SIZE_MASK_16_11 + 1];
int HEAD_16_11 = 0;
int TAIL_16_11 = 0;
float BUFFER_17_18[__BUF_SIZE_MASK_17_18 + 1];
int HEAD_17_18 = 0;
int TAIL_17_18 = 0;
float BUFFER_18_19[__BUF_SIZE_MASK_18_19 + 1];
int HEAD_18_19 = 0;
int TAIL_18_19 = 0;
float BUFFER_19_20[__BUF_SIZE_MASK_19_20 + 1];
int HEAD_19_20 = 0;
int TAIL_19_20 = 0;
float BUFFER_20_11[__BUF_SIZE_MASK_20_11 + 1];
int HEAD_20_11 = 0;
int TAIL_20_11 = 0;
float BUFFER_21_22[__BUF_SIZE_MASK_21_22 + 1];
int HEAD_21_22 = 0;
int TAIL_21_22 = 0;
float BUFFER_22_23[__BUF_SIZE_MASK_22_23 + 1];
int HEAD_22_23 = 0;
int TAIL_22_23 = 0;
float BUFFER_23_24[__BUF_SIZE_MASK_23_24 + 1];
int HEAD_23_24 = 0;
int TAIL_23_24 = 0;
float BUFFER_24_11[__BUF_SIZE_MASK_24_11 + 1];
int HEAD_24_11 = 0;
int TAIL_24_11 = 0;
float BUFFER_25_26[__BUF_SIZE_MASK_25_26 + 1];
int HEAD_25_26 = 0;
int TAIL_25_26 = 0;
float BUFFER_26_27[__BUF_SIZE_MASK_26_27 + 1];
int HEAD_26_27 = 0;
int TAIL_26_27 = 0;
float BUFFER_27_5[__BUF_SIZE_MASK_27_5 + 1];
int HEAD_27_5 = 0;
int TAIL_27_5 = 0;
float BUFFER_28_29[__BUF_SIZE_MASK_28_29 + 1];
int HEAD_28_29 = 0;
int TAIL_28_29 = 0;
float BUFFER_29_30[__BUF_SIZE_MASK_29_30 + 1];
int HEAD_29_30 = 0;
int TAIL_29_30 = 0;
float BUFFER_30_5[__BUF_SIZE_MASK_30_5 + 1];
int HEAD_30_5 = 0;
int TAIL_30_5 = 0;
float BUFFER_31_32[__BUF_SIZE_MASK_31_32 + 1];
int HEAD_31_32 = 0;
int TAIL_31_32 = 0;
float BUFFER_32_33[__BUF_SIZE_MASK_32_33 + 1];
int HEAD_32_33 = 0;
int TAIL_32_33 = 0;
float BUFFER_33_5[__BUF_SIZE_MASK_33_5 + 1];
int HEAD_33_5 = 0;
int TAIL_33_5 = 0;
float BUFFER_34_35[__BUF_SIZE_MASK_34_35 + 1];
int HEAD_34_35 = 0;
int TAIL_34_35 = 0;
float BUFFER_35_36[__BUF_SIZE_MASK_35_36 + 1];
int HEAD_35_36 = 0;
int TAIL_35_36 = 0;
float BUFFER_36_5[__BUF_SIZE_MASK_36_5 + 1];
int HEAD_36_5 = 0;
int TAIL_36_5 = 0;
float BUFFER_37_38[__BUF_SIZE_MASK_37_38 + 1];
int HEAD_37_38 = 0;
int TAIL_37_38 = 0;
float BUFFER_38_39[__BUF_SIZE_MASK_38_39 + 1];
int HEAD_38_39 = 0;
int TAIL_38_39 = 0;
float BUFFER_39_5[__BUF_SIZE_MASK_39_5 + 1];
int HEAD_39_5 = 0;
int TAIL_39_5 = 0;
float BUFFER_40_41[__BUF_SIZE_MASK_40_41 + 1];
int HEAD_40_41 = 0;
int TAIL_40_41 = 0;
float BUFFER_41_42[__BUF_SIZE_MASK_41_42 + 1];
int HEAD_41_42 = 0;
int TAIL_41_42 = 0;
float BUFFER_42_5[__BUF_SIZE_MASK_42_5 + 1];
int HEAD_42_5 = 0;
int TAIL_42_5 = 0;
float BUFFER_43_44[__BUF_SIZE_MASK_43_44 + 1];
int HEAD_43_44 = 0;
int TAIL_43_44 = 0;
float BUFFER_44_45[__BUF_SIZE_MASK_44_45 + 1];
int HEAD_44_45 = 0;
int TAIL_44_45 = 0;
float BUFFER_45_5[__BUF_SIZE_MASK_45_5 + 1];
int HEAD_45_5 = 0;
int TAIL_45_5 = 0;
float BUFFER_46_47[__BUF_SIZE_MASK_46_47 + 1];
int HEAD_46_47 = 0;
int TAIL_46_47 = 0;
float BUFFER_47_48[__BUF_SIZE_MASK_47_48 + 1];
int HEAD_47_48 = 0;
int TAIL_47_48 = 0;
float BUFFER_48_5[__BUF_SIZE_MASK_48_5 + 1];
int HEAD_48_5 = 0;
int TAIL_48_5 = 0;
float BUFFER_49_50[__BUF_SIZE_MASK_49_50 + 1];
int HEAD_49_50 = 0;
int TAIL_49_50 = 0;
float BUFFER_50_51[__BUF_SIZE_MASK_50_51 + 1];
int HEAD_50_51 = 0;
int TAIL_50_51 = 0;
float BUFFER_51_5[__BUF_SIZE_MASK_51_5 + 1];
int HEAD_51_5 = 0;
int TAIL_51_5 = 0;
float BUFFER_52_53[__BUF_SIZE_MASK_52_53 + 1];
int HEAD_52_53 = 0;
int TAIL_52_53 = 0;
float BUFFER_53_54[__BUF_SIZE_MASK_53_54 + 1];
int HEAD_53_54 = 0;
int TAIL_53_54 = 0;
float BUFFER_54_5[__BUF_SIZE_MASK_54_5 + 1];
int HEAD_54_5 = 0;
int TAIL_54_5 = 0;
float BUFFER_55_56[__BUF_SIZE_MASK_55_56 + 1];
int HEAD_55_56 = 0;
int TAIL_55_56 = 0;
float BUFFER_56_57[__BUF_SIZE_MASK_56_57 + 1];
int HEAD_56_57 = 0;
int TAIL_56_57 = 0;
float BUFFER_57_5[__BUF_SIZE_MASK_57_5 + 1];
int HEAD_57_5 = 0;
int TAIL_57_5 = 0;
void init_IntSource__3_59__0();
void work_IntSource__3_59__0(int);
#ifdef BUFFER_MERGE
void work_IntSource__3_59__0__mod(int ____n, void *____in, int *____out);
void work_IntSource__3_59__0__mod2(int ____n, void *____in, int *____out, int s1, int s2);
#endif
void __splitter_1_work(int);
void init_InputGenerate__7_62__2();
void work_InputGenerate__7_62__2(int);
#ifdef BUFFER_MERGE
void work_InputGenerate__7_62__2__mod(int ____n, int *____in, float *____out);
void work_InputGenerate__7_62__2__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__22_63__3();
void work_BeamFirFilter__22_63__3(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__22_63__3__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__22_63__3__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__37_64__4();
void work_BeamFirFilter__37_64__4(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__37_64__4__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__37_64__4__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void __joiner_5_work(int);
void __splitter_6_work(int);
void init_BeamForm__423_110__7();
void work_BeamForm__423_110__7(int);
#ifdef BUFFER_MERGE
void work_BeamForm__423_110__7__mod(int ____n, float *____in, float *____out);
void work_BeamForm__423_110__7__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__438_111__8();
void work_BeamFirFilter__438_111__8(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__438_111__8__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__438_111__8__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_Magnitude__446_112__9();
void work_Magnitude__446_112__9(int);
#ifdef BUFFER_MERGE
void work_Magnitude__446_112__9__mod(int ____n, float *____in, float *____out);
void work_Magnitude__446_112__9__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_Detector__452_113__10();
void work_Detector__452_113__10(int);
#ifdef BUFFER_MERGE
void work_Detector__452_113__10__mod(int ____n, float *____in, float *____out);
void work_Detector__452_113__10__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void __joiner_11_work(int);
void init_FloatPrinter__579_129__12();
void work_FloatPrinter__579_129__12(int);
#ifdef BUFFER_MERGE
void work_FloatPrinter__579_129__12__mod(int ____n, float *____in, void *____out);
void work_FloatPrinter__579_129__12__mod2(int ____n, float *____in, void *____out, int s1, int s2);
#endif
void init_BeamForm__464_115__13();
void work_BeamForm__464_115__13(int);
#ifdef BUFFER_MERGE
void work_BeamForm__464_115__13__mod(int ____n, float *____in, float *____out);
void work_BeamForm__464_115__13__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__479_116__14();
void work_BeamFirFilter__479_116__14(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__479_116__14__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__479_116__14__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_Magnitude__487_117__15();
void work_Magnitude__487_117__15(int);
#ifdef BUFFER_MERGE
void work_Magnitude__487_117__15__mod(int ____n, float *____in, float *____out);
void work_Magnitude__487_117__15__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_Detector__493_118__16();
void work_Detector__493_118__16(int);
#ifdef BUFFER_MERGE
void work_Detector__493_118__16__mod(int ____n, float *____in, float *____out);
void work_Detector__493_118__16__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_BeamForm__505_120__17();
void work_BeamForm__505_120__17(int);
#ifdef BUFFER_MERGE
void work_BeamForm__505_120__17__mod(int ____n, float *____in, float *____out);
void work_BeamForm__505_120__17__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__520_121__18();
void work_BeamFirFilter__520_121__18(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__520_121__18__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__520_121__18__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_Magnitude__528_122__19();
void work_Magnitude__528_122__19(int);
#ifdef BUFFER_MERGE
void work_Magnitude__528_122__19__mod(int ____n, float *____in, float *____out);
void work_Magnitude__528_122__19__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_Detector__534_123__20();
void work_Detector__534_123__20(int);
#ifdef BUFFER_MERGE
void work_Detector__534_123__20__mod(int ____n, float *____in, float *____out);
void work_Detector__534_123__20__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_BeamForm__546_125__21();
void work_BeamForm__546_125__21(int);
#ifdef BUFFER_MERGE
void work_BeamForm__546_125__21__mod(int ____n, float *____in, float *____out);
void work_BeamForm__546_125__21__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__561_126__22();
void work_BeamFirFilter__561_126__22(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__561_126__22__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__561_126__22__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_Magnitude__569_127__23();
void work_Magnitude__569_127__23(int);
#ifdef BUFFER_MERGE
void work_Magnitude__569_127__23__mod(int ____n, float *____in, float *____out);
void work_Magnitude__569_127__23__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_Detector__575_128__24();
void work_Detector__575_128__24(int);
#ifdef BUFFER_MERGE
void work_Detector__575_128__24__mod(int ____n, float *____in, float *____out);
void work_Detector__575_128__24__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_InputGenerate__41_66__25();
void work_InputGenerate__41_66__25(int);
#ifdef BUFFER_MERGE
void work_InputGenerate__41_66__25__mod(int ____n, int *____in, float *____out);
void work_InputGenerate__41_66__25__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__56_67__26();
void work_BeamFirFilter__56_67__26(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__56_67__26__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__56_67__26__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__71_68__27();
void work_BeamFirFilter__71_68__27(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__71_68__27__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__71_68__27__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_InputGenerate__75_70__28();
void work_InputGenerate__75_70__28(int);
#ifdef BUFFER_MERGE
void work_InputGenerate__75_70__28__mod(int ____n, int *____in, float *____out);
void work_InputGenerate__75_70__28__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__90_71__29();
void work_BeamFirFilter__90_71__29(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__90_71__29__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__90_71__29__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__105_72__30();
void work_BeamFirFilter__105_72__30(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__105_72__30__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__105_72__30__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_InputGenerate__109_74__31();
void work_InputGenerate__109_74__31(int);
#ifdef BUFFER_MERGE
void work_InputGenerate__109_74__31__mod(int ____n, int *____in, float *____out);
void work_InputGenerate__109_74__31__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__124_75__32();
void work_BeamFirFilter__124_75__32(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__124_75__32__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__124_75__32__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__139_76__33();
void work_BeamFirFilter__139_76__33(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__139_76__33__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__139_76__33__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_InputGenerate__143_78__34();
void work_InputGenerate__143_78__34(int);
#ifdef BUFFER_MERGE
void work_InputGenerate__143_78__34__mod(int ____n, int *____in, float *____out);
void work_InputGenerate__143_78__34__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__158_79__35();
void work_BeamFirFilter__158_79__35(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__158_79__35__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__158_79__35__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__173_80__36();
void work_BeamFirFilter__173_80__36(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__173_80__36__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__173_80__36__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_InputGenerate__177_82__37();
void work_InputGenerate__177_82__37(int);
#ifdef BUFFER_MERGE
void work_InputGenerate__177_82__37__mod(int ____n, int *____in, float *____out);
void work_InputGenerate__177_82__37__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__192_83__38();
void work_BeamFirFilter__192_83__38(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__192_83__38__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__192_83__38__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__207_84__39();
void work_BeamFirFilter__207_84__39(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__207_84__39__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__207_84__39__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_InputGenerate__211_86__40();
void work_InputGenerate__211_86__40(int);
#ifdef BUFFER_MERGE
void work_InputGenerate__211_86__40__mod(int ____n, int *____in, float *____out);
void work_InputGenerate__211_86__40__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__226_87__41();
void work_BeamFirFilter__226_87__41(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__226_87__41__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__226_87__41__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__241_88__42();
void work_BeamFirFilter__241_88__42(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__241_88__42__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__241_88__42__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_InputGenerate__245_90__43();
void work_InputGenerate__245_90__43(int);
#ifdef BUFFER_MERGE
void work_InputGenerate__245_90__43__mod(int ____n, int *____in, float *____out);
void work_InputGenerate__245_90__43__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__260_91__44();
void work_BeamFirFilter__260_91__44(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__260_91__44__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__260_91__44__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__275_92__45();
void work_BeamFirFilter__275_92__45(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__275_92__45__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__275_92__45__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_InputGenerate__279_94__46();
void work_InputGenerate__279_94__46(int);
#ifdef BUFFER_MERGE
void work_InputGenerate__279_94__46__mod(int ____n, int *____in, float *____out);
void work_InputGenerate__279_94__46__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__294_95__47();
void work_BeamFirFilter__294_95__47(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__294_95__47__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__294_95__47__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__309_96__48();
void work_BeamFirFilter__309_96__48(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__309_96__48__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__309_96__48__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_InputGenerate__313_98__49();
void work_InputGenerate__313_98__49(int);
#ifdef BUFFER_MERGE
void work_InputGenerate__313_98__49__mod(int ____n, int *____in, float *____out);
void work_InputGenerate__313_98__49__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__328_99__50();
void work_BeamFirFilter__328_99__50(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__328_99__50__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__328_99__50__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__343_100__51();
void work_BeamFirFilter__343_100__51(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__343_100__51__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__343_100__51__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_InputGenerate__347_102__52();
void work_InputGenerate__347_102__52(int);
#ifdef BUFFER_MERGE
void work_InputGenerate__347_102__52__mod(int ____n, int *____in, float *____out);
void work_InputGenerate__347_102__52__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__362_103__53();
void work_BeamFirFilter__362_103__53(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__362_103__53__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__362_103__53__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__377_104__54();
void work_BeamFirFilter__377_104__54(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__377_104__54__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__377_104__54__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_InputGenerate__381_106__55();
void work_InputGenerate__381_106__55(int);
#ifdef BUFFER_MERGE
void work_InputGenerate__381_106__55__mod(int ____n, int *____in, float *____out);
void work_InputGenerate__381_106__55__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__396_107__56();
void work_BeamFirFilter__396_107__56(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__396_107__56__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__396_107__56__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_BeamFirFilter__411_108__57();
void work_BeamFirFilter__411_108__57(int);
#ifdef BUFFER_MERGE
void work_BeamFirFilter__411_108__57__mod(int ____n, float *____in, float *____out);
void work_BeamFirFilter__411_108__57__mod2(int ____n, float *____in, float *____out, int s1, int s2);
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

init_IntSource__3_59__0();
init_InputGenerate__347_102__52();
init_InputGenerate__75_70__28();
init_InputGenerate__211_86__40();
init_InputGenerate__313_98__49();
init_InputGenerate__41_66__25();
init_InputGenerate__177_82__37();
init_InputGenerate__143_78__34();
init_InputGenerate__279_94__46();
init_InputGenerate__381_106__55();
init_InputGenerate__7_62__2();
init_InputGenerate__109_74__31();
init_InputGenerate__245_90__43();
init_BeamFirFilter__362_103__53();
init_BeamFirFilter__226_87__41();
init_BeamFirFilter__90_71__29();
init_BeamFirFilter__328_99__50();
init_BeamFirFilter__192_83__38();
init_BeamFirFilter__56_67__26();
init_BeamFirFilter__294_95__47();
init_BeamFirFilter__158_79__35();
init_BeamFirFilter__22_63__3();
init_BeamFirFilter__396_107__56();
init_BeamFirFilter__260_91__44();
init_BeamFirFilter__124_75__32();
init_BeamFirFilter__71_68__27();
init_BeamFirFilter__207_84__39();
init_BeamFirFilter__343_100__51();
init_BeamFirFilter__37_64__4();
init_BeamFirFilter__173_80__36();
init_BeamFirFilter__309_96__48();
init_BeamFirFilter__139_76__33();
init_BeamFirFilter__275_92__45();
init_BeamFirFilter__411_108__57();
init_BeamFirFilter__105_72__30();
init_BeamFirFilter__241_88__42();
init_BeamFirFilter__377_104__54();
init_BeamForm__464_115__13();
init_BeamForm__423_110__7();
init_BeamForm__546_125__21();
init_BeamForm__505_120__17();
init_BeamFirFilter__479_116__14();
init_BeamFirFilter__561_126__22();
init_BeamFirFilter__438_111__8();
init_BeamFirFilter__520_121__18();
init_Magnitude__487_117__15();
init_Magnitude__446_112__9();
init_Magnitude__569_127__23();
init_Magnitude__528_122__19();
init_Detector__493_118__16();
init_Detector__452_113__10();
init_Detector__575_128__24();
init_Detector__534_123__20();
init_FloatPrinter__579_129__12();

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
    work_IntSource__3_59__0(72 );
HEAD_1_2 = 0;
TAIL_1_2 = 0;
HEAD_1_25 = 0;
TAIL_1_25 = 0;
HEAD_1_28 = 0;
TAIL_1_28 = 0;
HEAD_1_31 = 0;
TAIL_1_31 = 0;
HEAD_1_34 = 0;
TAIL_1_34 = 0;
HEAD_1_37 = 0;
TAIL_1_37 = 0;
HEAD_1_40 = 0;
TAIL_1_40 = 0;
HEAD_1_43 = 0;
TAIL_1_43 = 0;
HEAD_1_46 = 0;
TAIL_1_46 = 0;
HEAD_1_49 = 0;
TAIL_1_49 = 0;
HEAD_1_52 = 0;
TAIL_1_52 = 0;
HEAD_1_55 = 0;
TAIL_1_55 = 0;
    __splitter_1_work(6 );
HEAD_52_53 = 0;
TAIL_52_53 = 0;
    work_InputGenerate__347_102__52(6 );
HEAD_28_29 = 0;
TAIL_28_29 = 0;
    work_InputGenerate__75_70__28(6 );
HEAD_40_41 = 0;
TAIL_40_41 = 0;
    work_InputGenerate__211_86__40(6 );
HEAD_49_50 = 0;
TAIL_49_50 = 0;
    work_InputGenerate__313_98__49(6 );
HEAD_25_26 = 0;
TAIL_25_26 = 0;
    work_InputGenerate__41_66__25(6 );
HEAD_37_38 = 0;
TAIL_37_38 = 0;
    work_InputGenerate__177_82__37(6 );
HEAD_34_35 = 0;
TAIL_34_35 = 0;
    work_InputGenerate__143_78__34(6 );
HEAD_46_47 = 0;
TAIL_46_47 = 0;
    work_InputGenerate__279_94__46(6 );
HEAD_55_56 = 0;
TAIL_55_56 = 0;
    work_InputGenerate__381_106__55(6 );
HEAD_2_3 = 0;
TAIL_2_3 = 0;
    work_InputGenerate__7_62__2(6 );
HEAD_31_32 = 0;
TAIL_31_32 = 0;
    work_InputGenerate__109_74__31(6 );
HEAD_43_44 = 0;
TAIL_43_44 = 0;
    work_InputGenerate__245_90__43(6 );
HEAD_53_54 = 0;
TAIL_53_54 = 0;
    work_BeamFirFilter__362_103__53(3 );
HEAD_41_42 = 0;
TAIL_41_42 = 0;
    work_BeamFirFilter__226_87__41(3 );
HEAD_29_30 = 0;
TAIL_29_30 = 0;
    work_BeamFirFilter__90_71__29(3 );
HEAD_50_51 = 0;
TAIL_50_51 = 0;
    work_BeamFirFilter__328_99__50(3 );
HEAD_38_39 = 0;
TAIL_38_39 = 0;
    work_BeamFirFilter__192_83__38(3 );
HEAD_26_27 = 0;
TAIL_26_27 = 0;
    work_BeamFirFilter__56_67__26(3 );
HEAD_47_48 = 0;
TAIL_47_48 = 0;
    work_BeamFirFilter__294_95__47(3 );
HEAD_35_36 = 0;
TAIL_35_36 = 0;
    work_BeamFirFilter__158_79__35(3 );
HEAD_3_4 = 0;
TAIL_3_4 = 0;
    work_BeamFirFilter__22_63__3(3 );
HEAD_56_57 = 0;
TAIL_56_57 = 0;
    work_BeamFirFilter__396_107__56(3 );
HEAD_44_45 = 0;
TAIL_44_45 = 0;
    work_BeamFirFilter__260_91__44(3 );
HEAD_32_33 = 0;
TAIL_32_33 = 0;
    work_BeamFirFilter__124_75__32(3 );
HEAD_27_5 = 0;
TAIL_27_5 = 0;
    work_BeamFirFilter__71_68__27(1 );
HEAD_39_5 = 0;
TAIL_39_5 = 0;
    work_BeamFirFilter__207_84__39(1 );
HEAD_51_5 = 0;
TAIL_51_5 = 0;
    work_BeamFirFilter__343_100__51(1 );
HEAD_4_5 = 0;
TAIL_4_5 = 0;
    work_BeamFirFilter__37_64__4(1 );
HEAD_36_5 = 0;
TAIL_36_5 = 0;
    work_BeamFirFilter__173_80__36(1 );
HEAD_48_5 = 0;
TAIL_48_5 = 0;
    work_BeamFirFilter__309_96__48(1 );
HEAD_33_5 = 0;
TAIL_33_5 = 0;
    work_BeamFirFilter__139_76__33(1 );
HEAD_45_5 = 0;
TAIL_45_5 = 0;
    work_BeamFirFilter__275_92__45(1 );
HEAD_57_5 = 0;
TAIL_57_5 = 0;
    work_BeamFirFilter__411_108__57(1 );
HEAD_30_5 = 0;
TAIL_30_5 = 0;
    work_BeamFirFilter__105_72__30(1 );
HEAD_42_5 = 0;
TAIL_42_5 = 0;
    work_BeamFirFilter__241_88__42(1 );
HEAD_54_5 = 0;
TAIL_54_5 = 0;
    work_BeamFirFilter__377_104__54(1 );
HEAD_5_6 = 0;
TAIL_5_6 = 0;
    __joiner_5_work(1 );
HEAD_6_7 = 0;
TAIL_6_7 = 0;
HEAD_6_13 = 0;
TAIL_6_13 = 0;
HEAD_6_17 = 0;
TAIL_6_17 = 0;
HEAD_6_21 = 0;
TAIL_6_21 = 0;
    __splitter_6_work(24 );
HEAD_13_14 = 0;
TAIL_13_14 = 0;
    work_BeamForm__464_115__13(1 );
HEAD_7_8 = 0;
TAIL_7_8 = 0;
    work_BeamForm__423_110__7(1 );
HEAD_21_22 = 0;
TAIL_21_22 = 0;
    work_BeamForm__546_125__21(1 );
HEAD_17_18 = 0;
TAIL_17_18 = 0;
    work_BeamForm__505_120__17(1 );
HEAD_14_15 = 0;
TAIL_14_15 = 0;
    work_BeamFirFilter__479_116__14(1 );
HEAD_22_23 = 0;
TAIL_22_23 = 0;
    work_BeamFirFilter__561_126__22(1 );
HEAD_8_9 = 0;
TAIL_8_9 = 0;
    work_BeamFirFilter__438_111__8(1 );
HEAD_18_19 = 0;
TAIL_18_19 = 0;
    work_BeamFirFilter__520_121__18(1 );
HEAD_15_16 = 0;
TAIL_15_16 = 0;
    work_Magnitude__487_117__15(1 );
HEAD_9_10 = 0;
TAIL_9_10 = 0;
    work_Magnitude__446_112__9(1 );
HEAD_23_24 = 0;
TAIL_23_24 = 0;
    work_Magnitude__569_127__23(1 );
HEAD_19_20 = 0;
TAIL_19_20 = 0;
    work_Magnitude__528_122__19(1 );
HEAD_16_11 = 0;
TAIL_16_11 = 0;
    work_Detector__493_118__16(1 );
HEAD_10_11 = 0;
TAIL_10_11 = 0;
    work_Detector__452_113__10(1 );
HEAD_24_11 = 0;
TAIL_24_11 = 0;
    work_Detector__575_128__24(1 );
HEAD_20_11 = 0;
TAIL_20_11 = 0;
    work_Detector__534_123__20(1 );
HEAD_11_12 = 0;
TAIL_11_12 = 0;
    __joiner_11_work(1 );
    work_FloatPrinter__579_129__12(4 );
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
message *__msg_stack_46;
message *__msg_stack_56;
message *__msg_stack_9;
message *__msg_stack_47;
message *__msg_stack_40;
message *__msg_stack_31;
message *__msg_stack_49;
message *__msg_stack_1;
message *__msg_stack_55;
message *__msg_stack_14;
message *__msg_stack_19;
message *__msg_stack_5;
message *__msg_stack_15;
message *__msg_stack_45;
message *__msg_stack_2;
message *__msg_stack_52;
message *__msg_stack_17;
message *__msg_stack_21;
message *__msg_stack_48;
message *__msg_stack_41;
message *__msg_stack_44;
message *__msg_stack_23;
message *__msg_stack_30;
message *__msg_stack_36;
message *__msg_stack_26;
message *__msg_stack_29;
message *__msg_stack_35;
message *__msg_stack_7;
message *__msg_stack_50;
message *__msg_stack_6;
message *__msg_stack_25;
message *__msg_stack_4;
message *__msg_stack_20;
message *__msg_stack_53;
message *__msg_stack_33;
message *__msg_stack_0;
message *__msg_stack_8;
message *__msg_stack_27;
message *__msg_stack_32;
message *__msg_stack_12;
message *__msg_stack_11;
message *__msg_stack_28;
message *__msg_stack_13;
message *__msg_stack_51;
message *__msg_stack_18;
message *__msg_stack_16;
message *__msg_stack_54;
message *__msg_stack_22;
message *__msg_stack_57;
message *__msg_stack_43;
message *__msg_stack_39;
message *__msg_stack_37;

// end of moved or inserted by concat_cluster_threads.pl

// peek: 0 pop: 0 push 1
// init counts: 0 steady counts: 72

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



int x__0__0 = 0;
void save_file_pointer__0(object_write_buffer *buf);
void load_file_pointer__0(object_write_buffer *buf);

 
void init_IntSource__3_59__0();
inline void check_status__0();

void work_IntSource__3_59__0(int);



inline void __push__0(int data) {
BUFFER_0_1[HEAD_0_1]=data;
HEAD_0_1++;
}



 
void init_IntSource__3_59__0(){
        printf("Enter seed: ");
        scanf ("%d", &seed);
        srand(seed);  
}
void save_file_pointer__0(object_write_buffer *buf) {}
void load_file_pointer__0(object_write_buffer *buf) {}
 
void work_IntSource__3_59__0(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter IntSource

      __push__0(rand_r(&seed));
      // mark end: SIRFilter IntSource

    }
  }
}

// init counts: 0 steady counts: 6

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
BUFFER_1_25[HEAD_1_25]=BUFFER_0_1[TAIL_0_1];
HEAD_1_25++; ; TAIL_0_1++;
BUFFER_1_28[HEAD_1_28]=BUFFER_0_1[TAIL_0_1];
HEAD_1_28++; ; TAIL_0_1++;
BUFFER_1_31[HEAD_1_31]=BUFFER_0_1[TAIL_0_1];
HEAD_1_31++; ; TAIL_0_1++;
BUFFER_1_34[HEAD_1_34]=BUFFER_0_1[TAIL_0_1];
HEAD_1_34++; ; TAIL_0_1++;
BUFFER_1_37[HEAD_1_37]=BUFFER_0_1[TAIL_0_1];
HEAD_1_37++; ; TAIL_0_1++;
BUFFER_1_40[HEAD_1_40]=BUFFER_0_1[TAIL_0_1];
HEAD_1_40++; ; TAIL_0_1++;
BUFFER_1_43[HEAD_1_43]=BUFFER_0_1[TAIL_0_1];
HEAD_1_43++; ; TAIL_0_1++;
BUFFER_1_46[HEAD_1_46]=BUFFER_0_1[TAIL_0_1];
HEAD_1_46++; ; TAIL_0_1++;
BUFFER_1_49[HEAD_1_49]=BUFFER_0_1[TAIL_0_1];
HEAD_1_49++; ; TAIL_0_1++;
BUFFER_1_52[HEAD_1_52]=BUFFER_0_1[TAIL_0_1];
HEAD_1_52++; ; TAIL_0_1++;
BUFFER_1_55[HEAD_1_55]=BUFFER_0_1[TAIL_0_1];
HEAD_1_55++; ; TAIL_0_1++;
  }
}


// peek: 1 pop: 1 push 2
// init counts: 0 steady counts: 6

// ClusterFusion isEliminated: false



int __number_of_iterations_2;
int __counter_2 = 0;
int __steady_2 = 0;
int __tmp_2 = 0;
int __tmp2_2 = 0;
int *__state_flag_2 = NULL;
thread_info *__thread_2 = NULL;



int curSample__4__2 = 0;
void save_peek_buffer__2(object_write_buffer *buf);
void load_peek_buffer__2(object_write_buffer *buf);
void save_file_pointer__2(object_write_buffer *buf);
void load_file_pointer__2(object_write_buffer *buf);

 
void init_InputGenerate__7_62__2();
inline void check_status__2();

void work_InputGenerate__7_62__2(int);


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



inline void __push__2(float data) {
BUFFER_2_3[HEAD_2_3]=data;
HEAD_2_3++;
}



 
void init_InputGenerate__7_62__2(){
  ((curSample__4__2) = 0)/*int*/;
}
void save_file_pointer__2(object_write_buffer *buf) {}
void load_file_pointer__2(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_InputGenerate__7_62__2__mod(int ____n, int *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRFilter InputGenerate

  (*____in++);

  if ((0 && ((curSample__4__2) == 256))) {{
    ((*____out++)=((float)(sqrtf(((double)(((curSample__4__2) * 0)))))));
    ((*____out++)=(((float)(sqrtf(((double)(((curSample__4__2) * 0)))))) + ((float)1.0)));
  } } else {{
    ((*____out++)=(-((float)(sqrtf(((double)(((curSample__4__2) * 0))))))));
    ((*____out++)=(-(((float)(sqrtf(((double)(((curSample__4__2) * 0)))))) + ((float)1.0))));
  }}
  ((curSample__4__2)++);

  if (((curSample__4__2) >= 1024)) {((curSample__4__2) = 0)/*int*/;}
  // mark end: SIRFilter InputGenerate

}}


void work_InputGenerate__7_62__2__mod2(int ____n, int *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRFilter InputGenerate

  (*____in++);

  if ((0 && ((curSample__4__2) == 256))) {{
    ((*____out++)=((float)(sqrtf(((double)(((curSample__4__2) * 0)))))));
    ((*____out++)=(((float)(sqrtf(((double)(((curSample__4__2) * 0)))))) + ((float)1.0)));
  } } else {{
    ((*____out++)=(-((float)(sqrtf(((double)(((curSample__4__2) * 0))))))));
    ((*____out++)=(-(((float)(sqrtf(((double)(((curSample__4__2) * 0)))))) + ((float)1.0))));
  }}
  ((curSample__4__2)++);

  if (((curSample__4__2) >= 1024)) {((curSample__4__2) = 0)/*int*/;}
  // mark end: SIRFilter InputGenerate

}}


#endif // BUFFER_MERGE


 
void work_InputGenerate__7_62__2(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter InputGenerate

      __pop__2();

      if ((0 && ((curSample__4__2) == 256))) {{
        __push__2(((float)(sqrtf(((double)(((curSample__4__2) * 0)))))));
        __push__2((((float)(sqrtf(((double)(((curSample__4__2) * 0)))))) + ((float)1.0)));
      } } else {{
        __push__2((-((float)(sqrtf(((double)(((curSample__4__2) * 0))))))));
        __push__2((-(((float)(sqrtf(((double)(((curSample__4__2) * 0)))))) + ((float)1.0))));
      }}
      ((curSample__4__2)++);

      if (((curSample__4__2) >= 1024)) {((curSample__4__2) = 0)/*int*/;}
      // mark end: SIRFilter InputGenerate

    }
  }
}

// peek: 4 pop: 4 push 2
// init counts: 0 steady counts: 3

// ClusterFusion isEliminated: false



int __number_of_iterations_3;
int __counter_3 = 0;
int __steady_3 = 0;
int __tmp_3 = 0;
int __tmp2_3 = 0;
int *__state_flag_3 = NULL;
thread_info *__thread_3 = NULL;



float real_weight__8__3[64] = {0};
float imag_weight__9__3[64] = {0};
float realBuffer__10__3[64] = {0};
float imagBuffer__11__3[64] = {0};
int count__12__3 = 0;
int pos__13__3 = 0;
void save_peek_buffer__3(object_write_buffer *buf);
void load_peek_buffer__3(object_write_buffer *buf);
void save_file_pointer__3(object_write_buffer *buf);
void load_file_pointer__3(object_write_buffer *buf);

 
void init_BeamFirFilter__22_63__3();
inline void check_status__3();

void work_BeamFirFilter__22_63__3(int);


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



 
void init_BeamFirFilter__22_63__3(){
  int idx__20 = 0;/* int */
  int j__21 = 0;/* int */

  ((pos__13__3) = 0)/*int*/;
  for ((j__21 = 0)/*int*/; (j__21 < 64); (j__21++)) {{
      (idx__20 = (j__21 + 1))/*int*/;
      (((real_weight__8__3)[(int)j__21]) = (((float)(sinf(((double)(idx__20))))) / ((float)(idx__20))))/*float*/;
      (((imag_weight__9__3)[(int)j__21]) = (((float)(cosf(((double)(idx__20))))) / ((float)(idx__20))))/*float*/;
    }
  }
}
void save_file_pointer__3(object_write_buffer *buf) {}
void load_file_pointer__3(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__22_63__3__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__16 = 0.0f;/* float */
  float imag_curr__17 = 0.0f;/* float */
  int i__18 = 0;/* int */
  int modPos__19 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__16 = ((float)0.0))/*float*/;
  (imag_curr__17 = ((float)0.0))/*float*/;
  (((realBuffer__10__3)[(int)(63 - (pos__13__3))]) = (*____in++))/*float*/;
  (((imagBuffer__11__3)[(int)(63 - (pos__13__3))]) = (*____in++))/*float*/;
  (modPos__19 = (63 - (pos__13__3)))/*int*/;
  for ((i__18 = 0)/*int*/; (i__18 < 64); (i__18++)) {{
      (real_curr__16 = (real_curr__16 + ((((realBuffer__10__3)[(int)modPos__19]) * ((real_weight__8__3)[(int)i__18])) + (((imagBuffer__11__3)[(int)modPos__19]) * ((imag_weight__9__3)[(int)i__18])))))/*float*/;
      (imag_curr__17 = (imag_curr__17 + ((((imagBuffer__11__3)[(int)modPos__19]) * ((real_weight__8__3)[(int)i__18])) + (((realBuffer__10__3)[(int)modPos__19]) * ((imag_weight__9__3)[(int)i__18])))))/*float*/;
      (modPos__19 = ((modPos__19 + 1) & 63))/*int*/;
    }
  }
  ((pos__13__3) = (((pos__13__3) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__16);
  ((*____out++)=imag_curr__17);
  for ((i__18 = 2)/*int*/; (i__18 < 4); (i__18++)) {{
      (*____in++);
    }
  }
  ((count__12__3) = ((count__12__3) + 2))/*int*/;

  if (((count__12__3) == 1024)) {{
    ((count__12__3) = 0)/*int*/;
    ((pos__13__3) = 0)/*int*/;
    for ((i__18 = 0)/*int*/; (i__18 < 64); (i__18++)) {{
        (((realBuffer__10__3)[(int)i__18]) = ((float)0.0))/*float*/;
        (((imagBuffer__11__3)[(int)i__18]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__22_63__3__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__16 = 0.0f;/* float */
  float imag_curr__17 = 0.0f;/* float */
  int i__18 = 0;/* int */
  int modPos__19 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__16 = ((float)0.0))/*float*/;
  (imag_curr__17 = ((float)0.0))/*float*/;
  (((realBuffer__10__3)[(int)(63 - (pos__13__3))]) = (*____in++))/*float*/;
  (((imagBuffer__11__3)[(int)(63 - (pos__13__3))]) = (*____in++))/*float*/;
  (modPos__19 = (63 - (pos__13__3)))/*int*/;
  for ((i__18 = 0)/*int*/; (i__18 < 64); (i__18++)) {{
      (real_curr__16 = (real_curr__16 + ((((realBuffer__10__3)[(int)modPos__19]) * ((real_weight__8__3)[(int)i__18])) + (((imagBuffer__11__3)[(int)modPos__19]) * ((imag_weight__9__3)[(int)i__18])))))/*float*/;
      (imag_curr__17 = (imag_curr__17 + ((((imagBuffer__11__3)[(int)modPos__19]) * ((real_weight__8__3)[(int)i__18])) + (((realBuffer__10__3)[(int)modPos__19]) * ((imag_weight__9__3)[(int)i__18])))))/*float*/;
      (modPos__19 = ((modPos__19 + 1) & 63))/*int*/;
    }
  }
  ((pos__13__3) = (((pos__13__3) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__16);
  ((*____out++)=imag_curr__17);
  for ((i__18 = 2)/*int*/; (i__18 < 4); (i__18++)) {{
      (*____in++);
    }
  }
  ((count__12__3) = ((count__12__3) + 2))/*int*/;

  if (((count__12__3) == 1024)) {{
    ((count__12__3) = 0)/*int*/;
    ((pos__13__3) = 0)/*int*/;
    for ((i__18 = 0)/*int*/; (i__18 < 64); (i__18++)) {{
        (((realBuffer__10__3)[(int)i__18]) = ((float)0.0))/*float*/;
        (((imagBuffer__11__3)[(int)i__18]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__22_63__3(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__16 = 0.0f;/* float */
      float imag_curr__17 = 0.0f;/* float */
      int i__18 = 0;/* int */
      int modPos__19 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__16 = ((float)0.0))/*float*/;
      (imag_curr__17 = ((float)0.0))/*float*/;
      (((realBuffer__10__3)[(int)(63 - (pos__13__3))]) = __pop__3())/*float*/;
      (((imagBuffer__11__3)[(int)(63 - (pos__13__3))]) = __pop__3())/*float*/;
      (modPos__19 = (63 - (pos__13__3)))/*int*/;
      for ((i__18 = 0)/*int*/; (i__18 < 64); (i__18++)) {{
          (real_curr__16 = (real_curr__16 + ((((realBuffer__10__3)[(int)modPos__19]) * ((real_weight__8__3)[(int)i__18])) + (((imagBuffer__11__3)[(int)modPos__19]) * ((imag_weight__9__3)[(int)i__18])))))/*float*/;
          (imag_curr__17 = (imag_curr__17 + ((((imagBuffer__11__3)[(int)modPos__19]) * ((real_weight__8__3)[(int)i__18])) + (((realBuffer__10__3)[(int)modPos__19]) * ((imag_weight__9__3)[(int)i__18])))))/*float*/;
          (modPos__19 = ((modPos__19 + 1) & 63))/*int*/;
        }
      }
      ((pos__13__3) = (((pos__13__3) + 1) & 63))/*int*/;
      __push__3(real_curr__16);
      __push__3(imag_curr__17);
      for ((i__18 = 2)/*int*/; (i__18 < 4); (i__18++)) {{
          __pop__3();
        }
      }
      ((count__12__3) = ((count__12__3) + 2))/*int*/;

      if (((count__12__3) == 1024)) {{
        ((count__12__3) = 0)/*int*/;
        ((pos__13__3) = 0)/*int*/;
        for ((i__18 = 0)/*int*/; (i__18 < 64); (i__18++)) {{
            (((realBuffer__10__3)[(int)i__18]) = ((float)0.0))/*float*/;
            (((imagBuffer__11__3)[(int)i__18]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

// peek: 6 pop: 6 push 2
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_4;
int __counter_4 = 0;
int __steady_4 = 0;
int __tmp_4 = 0;
int __tmp2_4 = 0;
int *__state_flag_4 = NULL;
thread_info *__thread_4 = NULL;



float real_weight__23__4[64] = {0};
float imag_weight__24__4[64] = {0};
float realBuffer__25__4[64] = {0};
float imagBuffer__26__4[64] = {0};
int count__27__4 = 0;
int pos__28__4 = 0;
void save_peek_buffer__4(object_write_buffer *buf);
void load_peek_buffer__4(object_write_buffer *buf);
void save_file_pointer__4(object_write_buffer *buf);
void load_file_pointer__4(object_write_buffer *buf);

 
void init_BeamFirFilter__37_64__4();
inline void check_status__4();

void work_BeamFirFilter__37_64__4(int);


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



 
void init_BeamFirFilter__37_64__4(){
  int idx__35 = 0;/* int */
  int j__36 = 0;/* int */

  ((pos__28__4) = 0)/*int*/;
  for ((j__36 = 0)/*int*/; (j__36 < 64); (j__36++)) {{
      (idx__35 = (j__36 + 1))/*int*/;
      (((real_weight__23__4)[(int)j__36]) = (((float)(sinf(((double)(idx__35))))) / ((float)(idx__35))))/*float*/;
      (((imag_weight__24__4)[(int)j__36]) = (((float)(cosf(((double)(idx__35))))) / ((float)(idx__35))))/*float*/;
    }
  }
}
void save_file_pointer__4(object_write_buffer *buf) {}
void load_file_pointer__4(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__37_64__4__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__31 = 0.0f;/* float */
  float imag_curr__32 = 0.0f;/* float */
  int i__33 = 0;/* int */
  int modPos__34 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__31 = ((float)0.0))/*float*/;
  (imag_curr__32 = ((float)0.0))/*float*/;
  (((realBuffer__25__4)[(int)(63 - (pos__28__4))]) = (*____in++))/*float*/;
  (((imagBuffer__26__4)[(int)(63 - (pos__28__4))]) = (*____in++))/*float*/;
  (modPos__34 = (63 - (pos__28__4)))/*int*/;
  for ((i__33 = 0)/*int*/; (i__33 < 64); (i__33++)) {{
      (real_curr__31 = (real_curr__31 + ((((realBuffer__25__4)[(int)modPos__34]) * ((real_weight__23__4)[(int)i__33])) + (((imagBuffer__26__4)[(int)modPos__34]) * ((imag_weight__24__4)[(int)i__33])))))/*float*/;
      (imag_curr__32 = (imag_curr__32 + ((((imagBuffer__26__4)[(int)modPos__34]) * ((real_weight__23__4)[(int)i__33])) + (((realBuffer__25__4)[(int)modPos__34]) * ((imag_weight__24__4)[(int)i__33])))))/*float*/;
      (modPos__34 = ((modPos__34 + 1) & 63))/*int*/;
    }
  }
  ((pos__28__4) = (((pos__28__4) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__31);
  ((*____out++)=imag_curr__32);
  for ((i__33 = 2)/*int*/; (i__33 < 6); (i__33++)) {{
      (*____in++);
    }
  }
  ((count__27__4) = ((count__27__4) + 3))/*int*/;

  if (((count__27__4) == 512)) {{
    ((count__27__4) = 0)/*int*/;
    ((pos__28__4) = 0)/*int*/;
    for ((i__33 = 0)/*int*/; (i__33 < 64); (i__33++)) {{
        (((realBuffer__25__4)[(int)i__33]) = ((float)0.0))/*float*/;
        (((imagBuffer__26__4)[(int)i__33]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__37_64__4__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__31 = 0.0f;/* float */
  float imag_curr__32 = 0.0f;/* float */
  int i__33 = 0;/* int */
  int modPos__34 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__31 = ((float)0.0))/*float*/;
  (imag_curr__32 = ((float)0.0))/*float*/;
  (((realBuffer__25__4)[(int)(63 - (pos__28__4))]) = (*____in++))/*float*/;
  (((imagBuffer__26__4)[(int)(63 - (pos__28__4))]) = (*____in++))/*float*/;
  (modPos__34 = (63 - (pos__28__4)))/*int*/;
  for ((i__33 = 0)/*int*/; (i__33 < 64); (i__33++)) {{
      (real_curr__31 = (real_curr__31 + ((((realBuffer__25__4)[(int)modPos__34]) * ((real_weight__23__4)[(int)i__33])) + (((imagBuffer__26__4)[(int)modPos__34]) * ((imag_weight__24__4)[(int)i__33])))))/*float*/;
      (imag_curr__32 = (imag_curr__32 + ((((imagBuffer__26__4)[(int)modPos__34]) * ((real_weight__23__4)[(int)i__33])) + (((realBuffer__25__4)[(int)modPos__34]) * ((imag_weight__24__4)[(int)i__33])))))/*float*/;
      (modPos__34 = ((modPos__34 + 1) & 63))/*int*/;
    }
  }
  ((pos__28__4) = (((pos__28__4) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__31);
  ((*____out++)=imag_curr__32);
  for ((i__33 = 2)/*int*/; (i__33 < 6); (i__33++)) {{
      (*____in++);
    }
  }
  ((count__27__4) = ((count__27__4) + 3))/*int*/;

  if (((count__27__4) == 512)) {{
    ((count__27__4) = 0)/*int*/;
    ((pos__28__4) = 0)/*int*/;
    for ((i__33 = 0)/*int*/; (i__33 < 64); (i__33++)) {{
        (((realBuffer__25__4)[(int)i__33]) = ((float)0.0))/*float*/;
        (((imagBuffer__26__4)[(int)i__33]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__37_64__4(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__31 = 0.0f;/* float */
      float imag_curr__32 = 0.0f;/* float */
      int i__33 = 0;/* int */
      int modPos__34 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__31 = ((float)0.0))/*float*/;
      (imag_curr__32 = ((float)0.0))/*float*/;
      (((realBuffer__25__4)[(int)(63 - (pos__28__4))]) = __pop__4())/*float*/;
      (((imagBuffer__26__4)[(int)(63 - (pos__28__4))]) = __pop__4())/*float*/;
      (modPos__34 = (63 - (pos__28__4)))/*int*/;
      for ((i__33 = 0)/*int*/; (i__33 < 64); (i__33++)) {{
          (real_curr__31 = (real_curr__31 + ((((realBuffer__25__4)[(int)modPos__34]) * ((real_weight__23__4)[(int)i__33])) + (((imagBuffer__26__4)[(int)modPos__34]) * ((imag_weight__24__4)[(int)i__33])))))/*float*/;
          (imag_curr__32 = (imag_curr__32 + ((((imagBuffer__26__4)[(int)modPos__34]) * ((real_weight__23__4)[(int)i__33])) + (((realBuffer__25__4)[(int)modPos__34]) * ((imag_weight__24__4)[(int)i__33])))))/*float*/;
          (modPos__34 = ((modPos__34 + 1) & 63))/*int*/;
        }
      }
      ((pos__28__4) = (((pos__28__4) + 1) & 63))/*int*/;
      __push__4(real_curr__31);
      __push__4(imag_curr__32);
      for ((i__33 = 2)/*int*/; (i__33 < 6); (i__33++)) {{
          __pop__4();
        }
      }
      ((count__27__4) = ((count__27__4) + 3))/*int*/;

      if (((count__27__4) == 512)) {{
        ((count__27__4) = 0)/*int*/;
        ((pos__28__4) = 0)/*int*/;
        for ((i__33 = 0)/*int*/; (i__33 < 64); (i__33++)) {{
            (((realBuffer__25__4)[(int)i__33]) = ((float)0.0))/*float*/;
            (((imagBuffer__26__4)[(int)i__33]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

// init counts: 0 steady counts: 1

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
BUFFER_5_6[HEAD_5_6]=BUFFER_4_5[TAIL_4_5];
HEAD_5_6++; ; TAIL_4_5++;
BUFFER_5_6[HEAD_5_6]=BUFFER_27_5[TAIL_27_5];
HEAD_5_6++; ; TAIL_27_5++;
BUFFER_5_6[HEAD_5_6]=BUFFER_27_5[TAIL_27_5];
HEAD_5_6++; ; TAIL_27_5++;
BUFFER_5_6[HEAD_5_6]=BUFFER_30_5[TAIL_30_5];
HEAD_5_6++; ; TAIL_30_5++;
BUFFER_5_6[HEAD_5_6]=BUFFER_30_5[TAIL_30_5];
HEAD_5_6++; ; TAIL_30_5++;
BUFFER_5_6[HEAD_5_6]=BUFFER_33_5[TAIL_33_5];
HEAD_5_6++; ; TAIL_33_5++;
BUFFER_5_6[HEAD_5_6]=BUFFER_33_5[TAIL_33_5];
HEAD_5_6++; ; TAIL_33_5++;
BUFFER_5_6[HEAD_5_6]=BUFFER_36_5[TAIL_36_5];
HEAD_5_6++; ; TAIL_36_5++;
BUFFER_5_6[HEAD_5_6]=BUFFER_36_5[TAIL_36_5];
HEAD_5_6++; ; TAIL_36_5++;
BUFFER_5_6[HEAD_5_6]=BUFFER_39_5[TAIL_39_5];
HEAD_5_6++; ; TAIL_39_5++;
BUFFER_5_6[HEAD_5_6]=BUFFER_39_5[TAIL_39_5];
HEAD_5_6++; ; TAIL_39_5++;
BUFFER_5_6[HEAD_5_6]=BUFFER_42_5[TAIL_42_5];
HEAD_5_6++; ; TAIL_42_5++;
BUFFER_5_6[HEAD_5_6]=BUFFER_42_5[TAIL_42_5];
HEAD_5_6++; ; TAIL_42_5++;
BUFFER_5_6[HEAD_5_6]=BUFFER_45_5[TAIL_45_5];
HEAD_5_6++; ; TAIL_45_5++;
BUFFER_5_6[HEAD_5_6]=BUFFER_45_5[TAIL_45_5];
HEAD_5_6++; ; TAIL_45_5++;
BUFFER_5_6[HEAD_5_6]=BUFFER_48_5[TAIL_48_5];
HEAD_5_6++; ; TAIL_48_5++;
BUFFER_5_6[HEAD_5_6]=BUFFER_48_5[TAIL_48_5];
HEAD_5_6++; ; TAIL_48_5++;
BUFFER_5_6[HEAD_5_6]=BUFFER_51_5[TAIL_51_5];
HEAD_5_6++; ; TAIL_51_5++;
BUFFER_5_6[HEAD_5_6]=BUFFER_51_5[TAIL_51_5];
HEAD_5_6++; ; TAIL_51_5++;
BUFFER_5_6[HEAD_5_6]=BUFFER_54_5[TAIL_54_5];
HEAD_5_6++; ; TAIL_54_5++;
BUFFER_5_6[HEAD_5_6]=BUFFER_54_5[TAIL_54_5];
HEAD_5_6++; ; TAIL_54_5++;
BUFFER_5_6[HEAD_5_6]=BUFFER_57_5[TAIL_57_5];
HEAD_5_6++; ; TAIL_57_5++;
BUFFER_5_6[HEAD_5_6]=BUFFER_57_5[TAIL_57_5];
HEAD_5_6++; ; TAIL_57_5++;
  }
}


// init counts: 0 steady counts: 24

// ClusterFusion isEliminated: false



int __number_of_iterations_6;
int __counter_6 = 0;
int __steady_6 = 0;
int __tmp_6 = 0;
int __tmp2_6 = 0;
int *__state_flag_6 = NULL;
thread_info *__thread_6 = NULL;




void __splitter_6_work(int ____n) {
  for (;____n > 0; ____n--) {
  float tmp;
  tmp = BUFFER_5_6[TAIL_5_6]; TAIL_5_6++;
  BUFFER_6_7[HEAD_6_7]=tmp;
HEAD_6_7++; ;
  BUFFER_6_13[HEAD_6_13]=tmp;
HEAD_6_13++; ;
  BUFFER_6_17[HEAD_6_17]=tmp;
HEAD_6_17++; ;
  BUFFER_6_21[HEAD_6_21]=tmp;
HEAD_6_21++; ;
  }
}


// peek: 24 pop: 24 push 2
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_7;
int __counter_7 = 0;
int __steady_7 = 0;
int __tmp_7 = 0;
int __tmp2_7 = 0;
int *__state_flag_7 = NULL;
thread_info *__thread_7 = NULL;



float real_weight__412__7[12] = {0};
float imag_weight__413__7[12] = {0};
void save_peek_buffer__7(object_write_buffer *buf);
void load_peek_buffer__7(object_write_buffer *buf);
void save_file_pointer__7(object_write_buffer *buf);
void load_file_pointer__7(object_write_buffer *buf);

 
void init_BeamForm__423_110__7();
inline void check_status__7();

void work_BeamForm__423_110__7(int);


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



 
void init_BeamForm__423_110__7(){
  int idx__421 = 0;/* int */
  int j__422 = 0;/* int */

  for ((j__422 = 0)/*int*/; (j__422 < 12); (j__422++)) {{
      (idx__421 = (j__422 + 1))/*int*/;
      (((real_weight__412__7)[(int)j__422]) = (((float)(sinf(((double)(idx__421))))) / ((float)((0 + idx__421)))))/*float*/;
      (((imag_weight__413__7)[(int)j__422]) = (((float)(cosf(((double)(idx__421))))) / ((float)((0 + idx__421)))))/*float*/;
    }
  }
}
void save_file_pointer__7(object_write_buffer *buf) {}
void load_file_pointer__7(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamForm__423_110__7__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__416 = 0.0f;/* float */
  float imag_curr__417 = 0.0f;/* float */
  float real_pop__418 = 0.0f;/* float */
  float imag_pop__419 = 0.0f;/* float */
  int i__420 = 0;/* int */

  // mark begin: SIRFilter BeamForm

  (real_curr__416 = ((float)0.0))/*float*/;
  (imag_curr__417 = ((float)0.0))/*float*/;
  for ((i__420 = 0)/*int*/; (i__420 < 12); (i__420++)) {{
      (real_pop__418 = (*____in++))/*float*/;
      (imag_pop__419 = (*____in++))/*float*/;
      (real_curr__416 = (real_curr__416 + ((((real_weight__412__7)[(int)i__420]) * real_pop__418) - (((imag_weight__413__7)[(int)i__420]) * imag_pop__419))))/*float*/;
      (imag_curr__417 = (imag_curr__417 + ((((real_weight__412__7)[(int)i__420]) * imag_pop__419) + (((imag_weight__413__7)[(int)i__420]) * real_pop__418))))/*float*/;
    }
  }
  ((*____out++)=real_curr__416);
  ((*____out++)=imag_curr__417);
  // mark end: SIRFilter BeamForm

}}


void work_BeamForm__423_110__7__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__416 = 0.0f;/* float */
  float imag_curr__417 = 0.0f;/* float */
  float real_pop__418 = 0.0f;/* float */
  float imag_pop__419 = 0.0f;/* float */
  int i__420 = 0;/* int */

  // mark begin: SIRFilter BeamForm

  (real_curr__416 = ((float)0.0))/*float*/;
  (imag_curr__417 = ((float)0.0))/*float*/;
  for ((i__420 = 0)/*int*/; (i__420 < 12); (i__420++)) {{
      (real_pop__418 = (*____in++))/*float*/;
      (imag_pop__419 = (*____in++))/*float*/;
      (real_curr__416 = (real_curr__416 + ((((real_weight__412__7)[(int)i__420]) * real_pop__418) - (((imag_weight__413__7)[(int)i__420]) * imag_pop__419))))/*float*/;
      (imag_curr__417 = (imag_curr__417 + ((((real_weight__412__7)[(int)i__420]) * imag_pop__419) + (((imag_weight__413__7)[(int)i__420]) * real_pop__418))))/*float*/;
    }
  }
  ((*____out++)=real_curr__416);
  ((*____out++)=imag_curr__417);
  // mark end: SIRFilter BeamForm

}}


#endif // BUFFER_MERGE


 
void work_BeamForm__423_110__7(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__416 = 0.0f;/* float */
      float imag_curr__417 = 0.0f;/* float */
      float real_pop__418 = 0.0f;/* float */
      float imag_pop__419 = 0.0f;/* float */
      int i__420 = 0;/* int */

      // mark begin: SIRFilter BeamForm

      (real_curr__416 = ((float)0.0))/*float*/;
      (imag_curr__417 = ((float)0.0))/*float*/;
      for ((i__420 = 0)/*int*/; (i__420 < 12); (i__420++)) {{
          (real_pop__418 = __pop__7())/*float*/;
          (imag_pop__419 = __pop__7())/*float*/;
          (real_curr__416 = (real_curr__416 + ((((real_weight__412__7)[(int)i__420]) * real_pop__418) - (((imag_weight__413__7)[(int)i__420]) * imag_pop__419))))/*float*/;
          (imag_curr__417 = (imag_curr__417 + ((((real_weight__412__7)[(int)i__420]) * imag_pop__419) + (((imag_weight__413__7)[(int)i__420]) * real_pop__418))))/*float*/;
        }
      }
      __push__7(real_curr__416);
      __push__7(imag_curr__417);
      // mark end: SIRFilter BeamForm

    }
  }
}

// peek: 2 pop: 2 push 2
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_8;
int __counter_8 = 0;
int __steady_8 = 0;
int __tmp_8 = 0;
int __tmp2_8 = 0;
int *__state_flag_8 = NULL;
thread_info *__thread_8 = NULL;



float real_weight__424__8[170] = {0};
float imag_weight__425__8[170] = {0};
float realBuffer__426__8[170] = {0};
float imagBuffer__427__8[170] = {0};
int count__428__8 = 0;
int pos__429__8 = 0;
void save_peek_buffer__8(object_write_buffer *buf);
void load_peek_buffer__8(object_write_buffer *buf);
void save_file_pointer__8(object_write_buffer *buf);
void load_file_pointer__8(object_write_buffer *buf);

 
void init_BeamFirFilter__438_111__8();
inline void check_status__8();

void work_BeamFirFilter__438_111__8(int);


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



 
void init_BeamFirFilter__438_111__8(){
  int idx__436 = 0;/* int */
  int j__437 = 0;/* int */

  ((pos__429__8) = 0)/*int*/;
  for ((j__437 = 0)/*int*/; (j__437 < 170); (j__437++)) {{
      (idx__436 = (j__437 + 1))/*int*/;
      (((real_weight__424__8)[(int)j__437]) = (((float)(sinf(((double)(idx__436))))) / ((float)(idx__436))))/*float*/;
      (((imag_weight__425__8)[(int)j__437]) = (((float)(cosf(((double)(idx__436))))) / ((float)(idx__436))))/*float*/;
    }
  }
}
void save_file_pointer__8(object_write_buffer *buf) {}
void load_file_pointer__8(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__438_111__8__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__432 = 0.0f;/* float */
  float imag_curr__433 = 0.0f;/* float */
  int i__434 = 0;/* int */
  int modPos__435 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__432 = ((float)0.0))/*float*/;
  (imag_curr__433 = ((float)0.0))/*float*/;
  (((realBuffer__426__8)[(int)(169 - (pos__429__8))]) = (*____in++))/*float*/;
  (((imagBuffer__427__8)[(int)(169 - (pos__429__8))]) = (*____in++))/*float*/;
  (modPos__435 = (169 - (pos__429__8)))/*int*/;
  for ((i__434 = 0)/*int*/; (i__434 < 170); (i__434++)) {{
      (real_curr__432 = (real_curr__432 + ((((realBuffer__426__8)[(int)modPos__435]) * ((real_weight__424__8)[(int)i__434])) + (((imagBuffer__427__8)[(int)modPos__435]) * ((imag_weight__425__8)[(int)i__434])))))/*float*/;
      (imag_curr__433 = (imag_curr__433 + ((((imagBuffer__427__8)[(int)modPos__435]) * ((real_weight__424__8)[(int)i__434])) + (((realBuffer__426__8)[(int)modPos__435]) * ((imag_weight__425__8)[(int)i__434])))))/*float*/;
      (modPos__435 = ((modPos__435 + 1) & 169))/*int*/;
    }
  }
  ((pos__429__8) = (((pos__429__8) + 1) & 169))/*int*/;
  ((*____out++)=real_curr__432);
  ((*____out++)=imag_curr__433);
  for ((i__434 = 2)/*int*/; (i__434 < 2); (i__434++)) {{
      (*____in++);
    }
  }
  ((count__428__8) = ((count__428__8) + 1))/*int*/;

  if (((count__428__8) == 170)) {{
    ((count__428__8) = 0)/*int*/;
    ((pos__429__8) = 0)/*int*/;
    for ((i__434 = 0)/*int*/; (i__434 < 170); (i__434++)) {{
        (((realBuffer__426__8)[(int)i__434]) = ((float)0.0))/*float*/;
        (((imagBuffer__427__8)[(int)i__434]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__438_111__8__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__432 = 0.0f;/* float */
  float imag_curr__433 = 0.0f;/* float */
  int i__434 = 0;/* int */
  int modPos__435 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__432 = ((float)0.0))/*float*/;
  (imag_curr__433 = ((float)0.0))/*float*/;
  (((realBuffer__426__8)[(int)(169 - (pos__429__8))]) = (*____in++))/*float*/;
  (((imagBuffer__427__8)[(int)(169 - (pos__429__8))]) = (*____in++))/*float*/;
  (modPos__435 = (169 - (pos__429__8)))/*int*/;
  for ((i__434 = 0)/*int*/; (i__434 < 170); (i__434++)) {{
      (real_curr__432 = (real_curr__432 + ((((realBuffer__426__8)[(int)modPos__435]) * ((real_weight__424__8)[(int)i__434])) + (((imagBuffer__427__8)[(int)modPos__435]) * ((imag_weight__425__8)[(int)i__434])))))/*float*/;
      (imag_curr__433 = (imag_curr__433 + ((((imagBuffer__427__8)[(int)modPos__435]) * ((real_weight__424__8)[(int)i__434])) + (((realBuffer__426__8)[(int)modPos__435]) * ((imag_weight__425__8)[(int)i__434])))))/*float*/;
      (modPos__435 = ((modPos__435 + 1) & 169))/*int*/;
    }
  }
  ((pos__429__8) = (((pos__429__8) + 1) & 169))/*int*/;
  ((*____out++)=real_curr__432);
  ((*____out++)=imag_curr__433);
  for ((i__434 = 2)/*int*/; (i__434 < 2); (i__434++)) {{
      (*____in++);
    }
  }
  ((count__428__8) = ((count__428__8) + 1))/*int*/;

  if (((count__428__8) == 170)) {{
    ((count__428__8) = 0)/*int*/;
    ((pos__429__8) = 0)/*int*/;
    for ((i__434 = 0)/*int*/; (i__434 < 170); (i__434++)) {{
        (((realBuffer__426__8)[(int)i__434]) = ((float)0.0))/*float*/;
        (((imagBuffer__427__8)[(int)i__434]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__438_111__8(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__432 = 0.0f;/* float */
      float imag_curr__433 = 0.0f;/* float */
      int i__434 = 0;/* int */
      int modPos__435 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__432 = ((float)0.0))/*float*/;
      (imag_curr__433 = ((float)0.0))/*float*/;
      (((realBuffer__426__8)[(int)(169 - (pos__429__8))]) = __pop__8())/*float*/;
      (((imagBuffer__427__8)[(int)(169 - (pos__429__8))]) = __pop__8())/*float*/;
      (modPos__435 = (169 - (pos__429__8)))/*int*/;
      for ((i__434 = 0)/*int*/; (i__434 < 170); (i__434++)) {{
          (real_curr__432 = (real_curr__432 + ((((realBuffer__426__8)[(int)modPos__435]) * ((real_weight__424__8)[(int)i__434])) + (((imagBuffer__427__8)[(int)modPos__435]) * ((imag_weight__425__8)[(int)i__434])))))/*float*/;
          (imag_curr__433 = (imag_curr__433 + ((((imagBuffer__427__8)[(int)modPos__435]) * ((real_weight__424__8)[(int)i__434])) + (((realBuffer__426__8)[(int)modPos__435]) * ((imag_weight__425__8)[(int)i__434])))))/*float*/;
          (modPos__435 = ((modPos__435 + 1) & 169))/*int*/;
        }
      }
      ((pos__429__8) = (((pos__429__8) + 1) & 169))/*int*/;
      __push__8(real_curr__432);
      __push__8(imag_curr__433);
      for ((i__434 = 2)/*int*/; (i__434 < 2); (i__434++)) {{
          __pop__8();
        }
      }
      ((count__428__8) = ((count__428__8) + 1))/*int*/;

      if (((count__428__8) == 170)) {{
        ((count__428__8) = 0)/*int*/;
        ((pos__429__8) = 0)/*int*/;
        for ((i__434 = 0)/*int*/; (i__434 < 170); (i__434++)) {{
            (((realBuffer__426__8)[(int)i__434]) = ((float)0.0))/*float*/;
            (((imagBuffer__427__8)[(int)i__434]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

// peek: 2 pop: 2 push 1
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

 
float mag__439__9(float real__442, float imag__443); 
void init_Magnitude__446_112__9();
inline void check_status__9();

void work_Magnitude__446_112__9(int);


inline float __pop__9() {
float res=BUFFER_8_9[TAIL_8_9];
TAIL_8_9++;
return res;
}

inline float __pop__9(int n) {
float res=BUFFER_8_9[TAIL_8_9];
TAIL_8_9+=n;

return res;
}

inline float __peek__9(int offs) {
return BUFFER_8_9[TAIL_8_9+offs];
}



inline void __push__9(float data) {
BUFFER_9_10[HEAD_9_10]=data;
HEAD_9_10++;
}



 
float mag__439__9(float real__442, float imag__443){
  return ((float)(((float)(sqrtf(((double)(((real__442 * real__442) + (imag__443 * imag__443)))))))));
}
 
void init_Magnitude__446_112__9(){
}
void save_file_pointer__9(object_write_buffer *buf) {}
void load_file_pointer__9(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Magnitude__446_112__9__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float f1__444 = 0.0f;/* float */
  float f2__445 = 0.0f;/* float */

  // mark begin: SIRFilter Magnitude

  (f1__444 = (*____in++))/*float*/;
  (f2__445 = (*____in++))/*float*/;
  ((*____out++)=mag__439__9(f1__444, f2__445));
  // mark end: SIRFilter Magnitude

}}


void work_Magnitude__446_112__9__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float f1__444 = 0.0f;/* float */
  float f2__445 = 0.0f;/* float */

  // mark begin: SIRFilter Magnitude

  (f1__444 = (*____in++))/*float*/;
  (f2__445 = (*____in++))/*float*/;
  ((*____out++)=mag__439__9(f1__444, f2__445));
  // mark end: SIRFilter Magnitude

}}


#endif // BUFFER_MERGE


 
void work_Magnitude__446_112__9(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float f1__444 = 0.0f;/* float */
      float f2__445 = 0.0f;/* float */

      // mark begin: SIRFilter Magnitude

      (f1__444 = __pop__9())/*float*/;
      (f2__445 = __pop__9())/*float*/;
      __push__9(mag__439__9(f1__444, f2__445));
      // mark end: SIRFilter Magnitude

    }
  }
}

// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_10;
int __counter_10 = 0;
int __steady_10 = 0;
int __tmp_10 = 0;
int __tmp2_10 = 0;
int *__state_flag_10 = NULL;
thread_info *__thread_10 = NULL;



int curSample__447__10 = 0;
void save_peek_buffer__10(object_write_buffer *buf);
void load_peek_buffer__10(object_write_buffer *buf);
void save_file_pointer__10(object_write_buffer *buf);
void load_file_pointer__10(object_write_buffer *buf);

 
void init_Detector__452_113__10();
inline void check_status__10();

void work_Detector__452_113__10(int);


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



 
void init_Detector__452_113__10(){
  ((curSample__447__10) = 0)/*int*/;
}
void save_file_pointer__10(object_write_buffer *buf) {}
void load_file_pointer__10(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Detector__452_113__10__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float inputVal__450 = 0.0f;/* float */
  float outputVal__451 = 0.0f;/* float */

  // mark begin: SIRFilter Detector

  (inputVal__450 = (*____in++))/*float*/;

  if ((0 && (42 == (curSample__447__10)))) {if ((inputVal__450 < ((float)0.1))) {(outputVal__451 = ((float)0.0))/*float*/;
    } else {(outputVal__451 = ((float)1.0))/*float*/;} } else {if ((inputVal__450 < ((float)0.1))) {(outputVal__451 = ((float)0.0))/*float*/;
  } else {(outputVal__451 = ((float)-1.0))/*float*/;}}
  (outputVal__451 = inputVal__450)/*float*/;
  ((curSample__447__10)++);

  if (((curSample__447__10) >= 170)) {((curSample__447__10) = 0)/*int*/;}
  ((*____out++)=outputVal__451);
  // mark end: SIRFilter Detector

}}


void work_Detector__452_113__10__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float inputVal__450 = 0.0f;/* float */
  float outputVal__451 = 0.0f;/* float */

  // mark begin: SIRFilter Detector

  (inputVal__450 = (*____in++))/*float*/;

  if ((0 && (42 == (curSample__447__10)))) {if ((inputVal__450 < ((float)0.1))) {(outputVal__451 = ((float)0.0))/*float*/;
    } else {(outputVal__451 = ((float)1.0))/*float*/;} } else {if ((inputVal__450 < ((float)0.1))) {(outputVal__451 = ((float)0.0))/*float*/;
  } else {(outputVal__451 = ((float)-1.0))/*float*/;}}
  (outputVal__451 = inputVal__450)/*float*/;
  ((curSample__447__10)++);

  if (((curSample__447__10) >= 170)) {((curSample__447__10) = 0)/*int*/;}
  ((*____out++)=outputVal__451);
  // mark end: SIRFilter Detector

}}


#endif // BUFFER_MERGE


 
void work_Detector__452_113__10(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float inputVal__450 = 0.0f;/* float */
      float outputVal__451 = 0.0f;/* float */

      // mark begin: SIRFilter Detector

      (inputVal__450 = __pop__10())/*float*/;

      if ((0 && (42 == (curSample__447__10)))) {if ((inputVal__450 < ((float)0.1))) {(outputVal__451 = ((float)0.0))/*float*/;
        } else {(outputVal__451 = ((float)1.0))/*float*/;} } else {if ((inputVal__450 < ((float)0.1))) {(outputVal__451 = ((float)0.0))/*float*/;
      } else {(outputVal__451 = ((float)-1.0))/*float*/;}}
      (outputVal__451 = inputVal__450)/*float*/;
      ((curSample__447__10)++);

      if (((curSample__447__10) >= 170)) {((curSample__447__10) = 0)/*int*/;}
      __push__10(outputVal__451);
      // mark end: SIRFilter Detector

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




void __joiner_11_work(int ____n) {
  for (;____n > 0; ____n--) {
BUFFER_11_12[HEAD_11_12]=BUFFER_10_11[TAIL_10_11];
HEAD_11_12++; ; TAIL_10_11++;
BUFFER_11_12[HEAD_11_12]=BUFFER_16_11[TAIL_16_11];
HEAD_11_12++; ; TAIL_16_11++;
BUFFER_11_12[HEAD_11_12]=BUFFER_20_11[TAIL_20_11];
HEAD_11_12++; ; TAIL_20_11++;
BUFFER_11_12[HEAD_11_12]=BUFFER_24_11[TAIL_24_11];
HEAD_11_12++; ; TAIL_24_11++;
  }
}


// peek: 1 pop: 1 push 0
// init counts: 0 steady counts: 4

// ClusterFusion isEliminated: false



int __number_of_iterations_12;
int __counter_12 = 0;
int __steady_12 = 0;
int __tmp_12 = 0;
int __tmp2_12 = 0;
int *__state_flag_12 = NULL;
thread_info *__thread_12 = NULL;



float x__576__12 = 0.0f;
void save_peek_buffer__12(object_write_buffer *buf);
void load_peek_buffer__12(object_write_buffer *buf);
void save_file_pointer__12(object_write_buffer *buf);
void load_file_pointer__12(object_write_buffer *buf);

 
void init_FloatPrinter__579_129__12();
inline void check_status__12();

void work_FloatPrinter__579_129__12(int);


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


 
void init_FloatPrinter__579_129__12(){
}
void save_file_pointer__12(object_write_buffer *buf) {}
void load_file_pointer__12(object_write_buffer *buf) {}
 
void work_FloatPrinter__579_129__12(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter FloatPrinter

      ((x__576__12) = __pop__12())/*float*/;
      // mark end: SIRFilter FloatPrinter
      sum+=x__576__12;
    }
  }
}

// peek: 24 pop: 24 push 2
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_13;
int __counter_13 = 0;
int __steady_13 = 0;
int __tmp_13 = 0;
int __tmp2_13 = 0;
int *__state_flag_13 = NULL;
thread_info *__thread_13 = NULL;



float real_weight__453__13[12] = {0};
float imag_weight__454__13[12] = {0};
void save_peek_buffer__13(object_write_buffer *buf);
void load_peek_buffer__13(object_write_buffer *buf);
void save_file_pointer__13(object_write_buffer *buf);
void load_file_pointer__13(object_write_buffer *buf);

 
void init_BeamForm__464_115__13();
inline void check_status__13();

void work_BeamForm__464_115__13(int);


inline float __pop__13() {
float res=BUFFER_6_13[TAIL_6_13];
TAIL_6_13++;
return res;
}

inline float __pop__13(int n) {
float res=BUFFER_6_13[TAIL_6_13];
TAIL_6_13+=n;

return res;
}

inline float __peek__13(int offs) {
return BUFFER_6_13[TAIL_6_13+offs];
}



inline void __push__13(float data) {
BUFFER_13_14[HEAD_13_14]=data;
HEAD_13_14++;
}



 
void init_BeamForm__464_115__13(){
  int idx__462 = 0;/* int */
  int j__463 = 0;/* int */

  for ((j__463 = 0)/*int*/; (j__463 < 12); (j__463++)) {{
      (idx__462 = (j__463 + 1))/*int*/;
      (((real_weight__453__13)[(int)j__463]) = (((float)(sinf(((double)(idx__462))))) / ((float)((1 + idx__462)))))/*float*/;
      (((imag_weight__454__13)[(int)j__463]) = (((float)(cosf(((double)(idx__462))))) / ((float)((1 + idx__462)))))/*float*/;
    }
  }
}
void save_file_pointer__13(object_write_buffer *buf) {}
void load_file_pointer__13(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamForm__464_115__13__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__457 = 0.0f;/* float */
  float imag_curr__458 = 0.0f;/* float */
  float real_pop__459 = 0.0f;/* float */
  float imag_pop__460 = 0.0f;/* float */
  int i__461 = 0;/* int */

  // mark begin: SIRFilter BeamForm

  (real_curr__457 = ((float)0.0))/*float*/;
  (imag_curr__458 = ((float)0.0))/*float*/;
  for ((i__461 = 0)/*int*/; (i__461 < 12); (i__461++)) {{
      (real_pop__459 = (*____in++))/*float*/;
      (imag_pop__460 = (*____in++))/*float*/;
      (real_curr__457 = (real_curr__457 + ((((real_weight__453__13)[(int)i__461]) * real_pop__459) - (((imag_weight__454__13)[(int)i__461]) * imag_pop__460))))/*float*/;
      (imag_curr__458 = (imag_curr__458 + ((((real_weight__453__13)[(int)i__461]) * imag_pop__460) + (((imag_weight__454__13)[(int)i__461]) * real_pop__459))))/*float*/;
    }
  }
  ((*____out++)=real_curr__457);
  ((*____out++)=imag_curr__458);
  // mark end: SIRFilter BeamForm

}}


void work_BeamForm__464_115__13__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__457 = 0.0f;/* float */
  float imag_curr__458 = 0.0f;/* float */
  float real_pop__459 = 0.0f;/* float */
  float imag_pop__460 = 0.0f;/* float */
  int i__461 = 0;/* int */

  // mark begin: SIRFilter BeamForm

  (real_curr__457 = ((float)0.0))/*float*/;
  (imag_curr__458 = ((float)0.0))/*float*/;
  for ((i__461 = 0)/*int*/; (i__461 < 12); (i__461++)) {{
      (real_pop__459 = (*____in++))/*float*/;
      (imag_pop__460 = (*____in++))/*float*/;
      (real_curr__457 = (real_curr__457 + ((((real_weight__453__13)[(int)i__461]) * real_pop__459) - (((imag_weight__454__13)[(int)i__461]) * imag_pop__460))))/*float*/;
      (imag_curr__458 = (imag_curr__458 + ((((real_weight__453__13)[(int)i__461]) * imag_pop__460) + (((imag_weight__454__13)[(int)i__461]) * real_pop__459))))/*float*/;
    }
  }
  ((*____out++)=real_curr__457);
  ((*____out++)=imag_curr__458);
  // mark end: SIRFilter BeamForm

}}


#endif // BUFFER_MERGE


 
void work_BeamForm__464_115__13(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__457 = 0.0f;/* float */
      float imag_curr__458 = 0.0f;/* float */
      float real_pop__459 = 0.0f;/* float */
      float imag_pop__460 = 0.0f;/* float */
      int i__461 = 0;/* int */

      // mark begin: SIRFilter BeamForm

      (real_curr__457 = ((float)0.0))/*float*/;
      (imag_curr__458 = ((float)0.0))/*float*/;
      for ((i__461 = 0)/*int*/; (i__461 < 12); (i__461++)) {{
          (real_pop__459 = __pop__13())/*float*/;
          (imag_pop__460 = __pop__13())/*float*/;
          (real_curr__457 = (real_curr__457 + ((((real_weight__453__13)[(int)i__461]) * real_pop__459) - (((imag_weight__454__13)[(int)i__461]) * imag_pop__460))))/*float*/;
          (imag_curr__458 = (imag_curr__458 + ((((real_weight__453__13)[(int)i__461]) * imag_pop__460) + (((imag_weight__454__13)[(int)i__461]) * real_pop__459))))/*float*/;
        }
      }
      __push__13(real_curr__457);
      __push__13(imag_curr__458);
      // mark end: SIRFilter BeamForm

    }
  }
}

// peek: 2 pop: 2 push 2
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_14;
int __counter_14 = 0;
int __steady_14 = 0;
int __tmp_14 = 0;
int __tmp2_14 = 0;
int *__state_flag_14 = NULL;
thread_info *__thread_14 = NULL;



float real_weight__465__14[170] = {0};
float imag_weight__466__14[170] = {0};
float realBuffer__467__14[170] = {0};
float imagBuffer__468__14[170] = {0};
int count__469__14 = 0;
int pos__470__14 = 0;
void save_peek_buffer__14(object_write_buffer *buf);
void load_peek_buffer__14(object_write_buffer *buf);
void save_file_pointer__14(object_write_buffer *buf);
void load_file_pointer__14(object_write_buffer *buf);

 
void init_BeamFirFilter__479_116__14();
inline void check_status__14();

void work_BeamFirFilter__479_116__14(int);


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



 
void init_BeamFirFilter__479_116__14(){
  int idx__477 = 0;/* int */
  int j__478 = 0;/* int */

  ((pos__470__14) = 0)/*int*/;
  for ((j__478 = 0)/*int*/; (j__478 < 170); (j__478++)) {{
      (idx__477 = (j__478 + 1))/*int*/;
      (((real_weight__465__14)[(int)j__478]) = (((float)(sinf(((double)(idx__477))))) / ((float)(idx__477))))/*float*/;
      (((imag_weight__466__14)[(int)j__478]) = (((float)(cosf(((double)(idx__477))))) / ((float)(idx__477))))/*float*/;
    }
  }
}
void save_file_pointer__14(object_write_buffer *buf) {}
void load_file_pointer__14(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__479_116__14__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__473 = 0.0f;/* float */
  float imag_curr__474 = 0.0f;/* float */
  int i__475 = 0;/* int */
  int modPos__476 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__473 = ((float)0.0))/*float*/;
  (imag_curr__474 = ((float)0.0))/*float*/;
  (((realBuffer__467__14)[(int)(169 - (pos__470__14))]) = (*____in++))/*float*/;
  (((imagBuffer__468__14)[(int)(169 - (pos__470__14))]) = (*____in++))/*float*/;
  (modPos__476 = (169 - (pos__470__14)))/*int*/;
  for ((i__475 = 0)/*int*/; (i__475 < 170); (i__475++)) {{
      (real_curr__473 = (real_curr__473 + ((((realBuffer__467__14)[(int)modPos__476]) * ((real_weight__465__14)[(int)i__475])) + (((imagBuffer__468__14)[(int)modPos__476]) * ((imag_weight__466__14)[(int)i__475])))))/*float*/;
      (imag_curr__474 = (imag_curr__474 + ((((imagBuffer__468__14)[(int)modPos__476]) * ((real_weight__465__14)[(int)i__475])) + (((realBuffer__467__14)[(int)modPos__476]) * ((imag_weight__466__14)[(int)i__475])))))/*float*/;
      (modPos__476 = ((modPos__476 + 1) & 169))/*int*/;
    }
  }
  ((pos__470__14) = (((pos__470__14) + 1) & 169))/*int*/;
  ((*____out++)=real_curr__473);
  ((*____out++)=imag_curr__474);
  for ((i__475 = 2)/*int*/; (i__475 < 2); (i__475++)) {{
      (*____in++);
    }
  }
  ((count__469__14) = ((count__469__14) + 1))/*int*/;

  if (((count__469__14) == 170)) {{
    ((count__469__14) = 0)/*int*/;
    ((pos__470__14) = 0)/*int*/;
    for ((i__475 = 0)/*int*/; (i__475 < 170); (i__475++)) {{
        (((realBuffer__467__14)[(int)i__475]) = ((float)0.0))/*float*/;
        (((imagBuffer__468__14)[(int)i__475]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__479_116__14__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__473 = 0.0f;/* float */
  float imag_curr__474 = 0.0f;/* float */
  int i__475 = 0;/* int */
  int modPos__476 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__473 = ((float)0.0))/*float*/;
  (imag_curr__474 = ((float)0.0))/*float*/;
  (((realBuffer__467__14)[(int)(169 - (pos__470__14))]) = (*____in++))/*float*/;
  (((imagBuffer__468__14)[(int)(169 - (pos__470__14))]) = (*____in++))/*float*/;
  (modPos__476 = (169 - (pos__470__14)))/*int*/;
  for ((i__475 = 0)/*int*/; (i__475 < 170); (i__475++)) {{
      (real_curr__473 = (real_curr__473 + ((((realBuffer__467__14)[(int)modPos__476]) * ((real_weight__465__14)[(int)i__475])) + (((imagBuffer__468__14)[(int)modPos__476]) * ((imag_weight__466__14)[(int)i__475])))))/*float*/;
      (imag_curr__474 = (imag_curr__474 + ((((imagBuffer__468__14)[(int)modPos__476]) * ((real_weight__465__14)[(int)i__475])) + (((realBuffer__467__14)[(int)modPos__476]) * ((imag_weight__466__14)[(int)i__475])))))/*float*/;
      (modPos__476 = ((modPos__476 + 1) & 169))/*int*/;
    }
  }
  ((pos__470__14) = (((pos__470__14) + 1) & 169))/*int*/;
  ((*____out++)=real_curr__473);
  ((*____out++)=imag_curr__474);
  for ((i__475 = 2)/*int*/; (i__475 < 2); (i__475++)) {{
      (*____in++);
    }
  }
  ((count__469__14) = ((count__469__14) + 1))/*int*/;

  if (((count__469__14) == 170)) {{
    ((count__469__14) = 0)/*int*/;
    ((pos__470__14) = 0)/*int*/;
    for ((i__475 = 0)/*int*/; (i__475 < 170); (i__475++)) {{
        (((realBuffer__467__14)[(int)i__475]) = ((float)0.0))/*float*/;
        (((imagBuffer__468__14)[(int)i__475]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__479_116__14(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__473 = 0.0f;/* float */
      float imag_curr__474 = 0.0f;/* float */
      int i__475 = 0;/* int */
      int modPos__476 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__473 = ((float)0.0))/*float*/;
      (imag_curr__474 = ((float)0.0))/*float*/;
      (((realBuffer__467__14)[(int)(169 - (pos__470__14))]) = __pop__14())/*float*/;
      (((imagBuffer__468__14)[(int)(169 - (pos__470__14))]) = __pop__14())/*float*/;
      (modPos__476 = (169 - (pos__470__14)))/*int*/;
      for ((i__475 = 0)/*int*/; (i__475 < 170); (i__475++)) {{
          (real_curr__473 = (real_curr__473 + ((((realBuffer__467__14)[(int)modPos__476]) * ((real_weight__465__14)[(int)i__475])) + (((imagBuffer__468__14)[(int)modPos__476]) * ((imag_weight__466__14)[(int)i__475])))))/*float*/;
          (imag_curr__474 = (imag_curr__474 + ((((imagBuffer__468__14)[(int)modPos__476]) * ((real_weight__465__14)[(int)i__475])) + (((realBuffer__467__14)[(int)modPos__476]) * ((imag_weight__466__14)[(int)i__475])))))/*float*/;
          (modPos__476 = ((modPos__476 + 1) & 169))/*int*/;
        }
      }
      ((pos__470__14) = (((pos__470__14) + 1) & 169))/*int*/;
      __push__14(real_curr__473);
      __push__14(imag_curr__474);
      for ((i__475 = 2)/*int*/; (i__475 < 2); (i__475++)) {{
          __pop__14();
        }
      }
      ((count__469__14) = ((count__469__14) + 1))/*int*/;

      if (((count__469__14) == 170)) {{
        ((count__469__14) = 0)/*int*/;
        ((pos__470__14) = 0)/*int*/;
        for ((i__475 = 0)/*int*/; (i__475 < 170); (i__475++)) {{
            (((realBuffer__467__14)[(int)i__475]) = ((float)0.0))/*float*/;
            (((imagBuffer__468__14)[(int)i__475]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

// peek: 2 pop: 2 push 1
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

 
float mag__480__15(float real__483, float imag__484); 
void init_Magnitude__487_117__15();
inline void check_status__15();

void work_Magnitude__487_117__15(int);


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



 
float mag__480__15(float real__483, float imag__484){
  return ((float)(((float)(sqrtf(((double)(((real__483 * real__483) + (imag__484 * imag__484)))))))));
}
 
void init_Magnitude__487_117__15(){
}
void save_file_pointer__15(object_write_buffer *buf) {}
void load_file_pointer__15(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Magnitude__487_117__15__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float f1__485 = 0.0f;/* float */
  float f2__486 = 0.0f;/* float */

  // mark begin: SIRFilter Magnitude

  (f1__485 = (*____in++))/*float*/;
  (f2__486 = (*____in++))/*float*/;
  ((*____out++)=mag__480__15(f1__485, f2__486));
  // mark end: SIRFilter Magnitude

}}


void work_Magnitude__487_117__15__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float f1__485 = 0.0f;/* float */
  float f2__486 = 0.0f;/* float */

  // mark begin: SIRFilter Magnitude

  (f1__485 = (*____in++))/*float*/;
  (f2__486 = (*____in++))/*float*/;
  ((*____out++)=mag__480__15(f1__485, f2__486));
  // mark end: SIRFilter Magnitude

}}


#endif // BUFFER_MERGE


 
void work_Magnitude__487_117__15(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float f1__485 = 0.0f;/* float */
      float f2__486 = 0.0f;/* float */

      // mark begin: SIRFilter Magnitude

      (f1__485 = __pop__15())/*float*/;
      (f2__486 = __pop__15())/*float*/;
      __push__15(mag__480__15(f1__485, f2__486));
      // mark end: SIRFilter Magnitude

    }
  }
}

// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_16;
int __counter_16 = 0;
int __steady_16 = 0;
int __tmp_16 = 0;
int __tmp2_16 = 0;
int *__state_flag_16 = NULL;
thread_info *__thread_16 = NULL;



int curSample__488__16 = 0;
void save_peek_buffer__16(object_write_buffer *buf);
void load_peek_buffer__16(object_write_buffer *buf);
void save_file_pointer__16(object_write_buffer *buf);
void load_file_pointer__16(object_write_buffer *buf);

 
void init_Detector__493_118__16();
inline void check_status__16();

void work_Detector__493_118__16(int);


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
BUFFER_16_11[HEAD_16_11]=data;
HEAD_16_11++;
}



 
void init_Detector__493_118__16(){
  ((curSample__488__16) = 0)/*int*/;
}
void save_file_pointer__16(object_write_buffer *buf) {}
void load_file_pointer__16(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Detector__493_118__16__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float inputVal__491 = 0.0f;/* float */
  float outputVal__492 = 0.0f;/* float */

  // mark begin: SIRFilter Detector

  (inputVal__491 = (*____in++))/*float*/;

  if ((1 && (42 == (curSample__488__16)))) {if ((inputVal__491 < ((float)0.1))) {(outputVal__492 = ((float)0.0))/*float*/;
    } else {(outputVal__492 = ((float)2.0))/*float*/;} } else {if ((inputVal__491 < ((float)0.1))) {(outputVal__492 = ((float)0.0))/*float*/;
  } else {(outputVal__492 = ((float)-2.0))/*float*/;}}
  (outputVal__492 = inputVal__491)/*float*/;
  ((curSample__488__16)++);

  if (((curSample__488__16) >= 170)) {((curSample__488__16) = 0)/*int*/;}
  ((*____out++)=outputVal__492);
  // mark end: SIRFilter Detector

}}


void work_Detector__493_118__16__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float inputVal__491 = 0.0f;/* float */
  float outputVal__492 = 0.0f;/* float */

  // mark begin: SIRFilter Detector

  (inputVal__491 = (*____in++))/*float*/;

  if ((1 && (42 == (curSample__488__16)))) {if ((inputVal__491 < ((float)0.1))) {(outputVal__492 = ((float)0.0))/*float*/;
    } else {(outputVal__492 = ((float)2.0))/*float*/;} } else {if ((inputVal__491 < ((float)0.1))) {(outputVal__492 = ((float)0.0))/*float*/;
  } else {(outputVal__492 = ((float)-2.0))/*float*/;}}
  (outputVal__492 = inputVal__491)/*float*/;
  ((curSample__488__16)++);

  if (((curSample__488__16) >= 170)) {((curSample__488__16) = 0)/*int*/;}
  ((*____out++)=outputVal__492);
  // mark end: SIRFilter Detector

}}


#endif // BUFFER_MERGE


 
void work_Detector__493_118__16(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float inputVal__491 = 0.0f;/* float */
      float outputVal__492 = 0.0f;/* float */

      // mark begin: SIRFilter Detector

      (inputVal__491 = __pop__16())/*float*/;

      if ((1 && (42 == (curSample__488__16)))) {if ((inputVal__491 < ((float)0.1))) {(outputVal__492 = ((float)0.0))/*float*/;
        } else {(outputVal__492 = ((float)2.0))/*float*/;} } else {if ((inputVal__491 < ((float)0.1))) {(outputVal__492 = ((float)0.0))/*float*/;
      } else {(outputVal__492 = ((float)-2.0))/*float*/;}}
      (outputVal__492 = inputVal__491)/*float*/;
      ((curSample__488__16)++);

      if (((curSample__488__16) >= 170)) {((curSample__488__16) = 0)/*int*/;}
      __push__16(outputVal__492);
      // mark end: SIRFilter Detector

    }
  }
}

// peek: 24 pop: 24 push 2
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_17;
int __counter_17 = 0;
int __steady_17 = 0;
int __tmp_17 = 0;
int __tmp2_17 = 0;
int *__state_flag_17 = NULL;
thread_info *__thread_17 = NULL;



float real_weight__494__17[12] = {0};
float imag_weight__495__17[12] = {0};
void save_peek_buffer__17(object_write_buffer *buf);
void load_peek_buffer__17(object_write_buffer *buf);
void save_file_pointer__17(object_write_buffer *buf);
void load_file_pointer__17(object_write_buffer *buf);

 
void init_BeamForm__505_120__17();
inline void check_status__17();

void work_BeamForm__505_120__17(int);


inline float __pop__17() {
float res=BUFFER_6_17[TAIL_6_17];
TAIL_6_17++;
return res;
}

inline float __pop__17(int n) {
float res=BUFFER_6_17[TAIL_6_17];
TAIL_6_17+=n;

return res;
}

inline float __peek__17(int offs) {
return BUFFER_6_17[TAIL_6_17+offs];
}



inline void __push__17(float data) {
BUFFER_17_18[HEAD_17_18]=data;
HEAD_17_18++;
}



 
void init_BeamForm__505_120__17(){
  int idx__503 = 0;/* int */
  int j__504 = 0;/* int */

  for ((j__504 = 0)/*int*/; (j__504 < 12); (j__504++)) {{
      (idx__503 = (j__504 + 1))/*int*/;
      (((real_weight__494__17)[(int)j__504]) = (((float)(sinf(((double)(idx__503))))) / ((float)((2 + idx__503)))))/*float*/;
      (((imag_weight__495__17)[(int)j__504]) = (((float)(cosf(((double)(idx__503))))) / ((float)((2 + idx__503)))))/*float*/;
    }
  }
}
void save_file_pointer__17(object_write_buffer *buf) {}
void load_file_pointer__17(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamForm__505_120__17__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__498 = 0.0f;/* float */
  float imag_curr__499 = 0.0f;/* float */
  float real_pop__500 = 0.0f;/* float */
  float imag_pop__501 = 0.0f;/* float */
  int i__502 = 0;/* int */

  // mark begin: SIRFilter BeamForm

  (real_curr__498 = ((float)0.0))/*float*/;
  (imag_curr__499 = ((float)0.0))/*float*/;
  for ((i__502 = 0)/*int*/; (i__502 < 12); (i__502++)) {{
      (real_pop__500 = (*____in++))/*float*/;
      (imag_pop__501 = (*____in++))/*float*/;
      (real_curr__498 = (real_curr__498 + ((((real_weight__494__17)[(int)i__502]) * real_pop__500) - (((imag_weight__495__17)[(int)i__502]) * imag_pop__501))))/*float*/;
      (imag_curr__499 = (imag_curr__499 + ((((real_weight__494__17)[(int)i__502]) * imag_pop__501) + (((imag_weight__495__17)[(int)i__502]) * real_pop__500))))/*float*/;
    }
  }
  ((*____out++)=real_curr__498);
  ((*____out++)=imag_curr__499);
  // mark end: SIRFilter BeamForm

}}


void work_BeamForm__505_120__17__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__498 = 0.0f;/* float */
  float imag_curr__499 = 0.0f;/* float */
  float real_pop__500 = 0.0f;/* float */
  float imag_pop__501 = 0.0f;/* float */
  int i__502 = 0;/* int */

  // mark begin: SIRFilter BeamForm

  (real_curr__498 = ((float)0.0))/*float*/;
  (imag_curr__499 = ((float)0.0))/*float*/;
  for ((i__502 = 0)/*int*/; (i__502 < 12); (i__502++)) {{
      (real_pop__500 = (*____in++))/*float*/;
      (imag_pop__501 = (*____in++))/*float*/;
      (real_curr__498 = (real_curr__498 + ((((real_weight__494__17)[(int)i__502]) * real_pop__500) - (((imag_weight__495__17)[(int)i__502]) * imag_pop__501))))/*float*/;
      (imag_curr__499 = (imag_curr__499 + ((((real_weight__494__17)[(int)i__502]) * imag_pop__501) + (((imag_weight__495__17)[(int)i__502]) * real_pop__500))))/*float*/;
    }
  }
  ((*____out++)=real_curr__498);
  ((*____out++)=imag_curr__499);
  // mark end: SIRFilter BeamForm

}}


#endif // BUFFER_MERGE


 
void work_BeamForm__505_120__17(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__498 = 0.0f;/* float */
      float imag_curr__499 = 0.0f;/* float */
      float real_pop__500 = 0.0f;/* float */
      float imag_pop__501 = 0.0f;/* float */
      int i__502 = 0;/* int */

      // mark begin: SIRFilter BeamForm

      (real_curr__498 = ((float)0.0))/*float*/;
      (imag_curr__499 = ((float)0.0))/*float*/;
      for ((i__502 = 0)/*int*/; (i__502 < 12); (i__502++)) {{
          (real_pop__500 = __pop__17())/*float*/;
          (imag_pop__501 = __pop__17())/*float*/;
          (real_curr__498 = (real_curr__498 + ((((real_weight__494__17)[(int)i__502]) * real_pop__500) - (((imag_weight__495__17)[(int)i__502]) * imag_pop__501))))/*float*/;
          (imag_curr__499 = (imag_curr__499 + ((((real_weight__494__17)[(int)i__502]) * imag_pop__501) + (((imag_weight__495__17)[(int)i__502]) * real_pop__500))))/*float*/;
        }
      }
      __push__17(real_curr__498);
      __push__17(imag_curr__499);
      // mark end: SIRFilter BeamForm

    }
  }
}

// peek: 2 pop: 2 push 2
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_18;
int __counter_18 = 0;
int __steady_18 = 0;
int __tmp_18 = 0;
int __tmp2_18 = 0;
int *__state_flag_18 = NULL;
thread_info *__thread_18 = NULL;



float real_weight__506__18[170] = {0};
float imag_weight__507__18[170] = {0};
float realBuffer__508__18[170] = {0};
float imagBuffer__509__18[170] = {0};
int count__510__18 = 0;
int pos__511__18 = 0;
void save_peek_buffer__18(object_write_buffer *buf);
void load_peek_buffer__18(object_write_buffer *buf);
void save_file_pointer__18(object_write_buffer *buf);
void load_file_pointer__18(object_write_buffer *buf);

 
void init_BeamFirFilter__520_121__18();
inline void check_status__18();

void work_BeamFirFilter__520_121__18(int);


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



 
void init_BeamFirFilter__520_121__18(){
  int idx__518 = 0;/* int */
  int j__519 = 0;/* int */

  ((pos__511__18) = 0)/*int*/;
  for ((j__519 = 0)/*int*/; (j__519 < 170); (j__519++)) {{
      (idx__518 = (j__519 + 1))/*int*/;
      (((real_weight__506__18)[(int)j__519]) = (((float)(sinf(((double)(idx__518))))) / ((float)(idx__518))))/*float*/;
      (((imag_weight__507__18)[(int)j__519]) = (((float)(cosf(((double)(idx__518))))) / ((float)(idx__518))))/*float*/;
    }
  }
}
void save_file_pointer__18(object_write_buffer *buf) {}
void load_file_pointer__18(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__520_121__18__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__514 = 0.0f;/* float */
  float imag_curr__515 = 0.0f;/* float */
  int i__516 = 0;/* int */
  int modPos__517 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__514 = ((float)0.0))/*float*/;
  (imag_curr__515 = ((float)0.0))/*float*/;
  (((realBuffer__508__18)[(int)(169 - (pos__511__18))]) = (*____in++))/*float*/;
  (((imagBuffer__509__18)[(int)(169 - (pos__511__18))]) = (*____in++))/*float*/;
  (modPos__517 = (169 - (pos__511__18)))/*int*/;
  for ((i__516 = 0)/*int*/; (i__516 < 170); (i__516++)) {{
      (real_curr__514 = (real_curr__514 + ((((realBuffer__508__18)[(int)modPos__517]) * ((real_weight__506__18)[(int)i__516])) + (((imagBuffer__509__18)[(int)modPos__517]) * ((imag_weight__507__18)[(int)i__516])))))/*float*/;
      (imag_curr__515 = (imag_curr__515 + ((((imagBuffer__509__18)[(int)modPos__517]) * ((real_weight__506__18)[(int)i__516])) + (((realBuffer__508__18)[(int)modPos__517]) * ((imag_weight__507__18)[(int)i__516])))))/*float*/;
      (modPos__517 = ((modPos__517 + 1) & 169))/*int*/;
    }
  }
  ((pos__511__18) = (((pos__511__18) + 1) & 169))/*int*/;
  ((*____out++)=real_curr__514);
  ((*____out++)=imag_curr__515);
  for ((i__516 = 2)/*int*/; (i__516 < 2); (i__516++)) {{
      (*____in++);
    }
  }
  ((count__510__18) = ((count__510__18) + 1))/*int*/;

  if (((count__510__18) == 170)) {{
    ((count__510__18) = 0)/*int*/;
    ((pos__511__18) = 0)/*int*/;
    for ((i__516 = 0)/*int*/; (i__516 < 170); (i__516++)) {{
        (((realBuffer__508__18)[(int)i__516]) = ((float)0.0))/*float*/;
        (((imagBuffer__509__18)[(int)i__516]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__520_121__18__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__514 = 0.0f;/* float */
  float imag_curr__515 = 0.0f;/* float */
  int i__516 = 0;/* int */
  int modPos__517 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__514 = ((float)0.0))/*float*/;
  (imag_curr__515 = ((float)0.0))/*float*/;
  (((realBuffer__508__18)[(int)(169 - (pos__511__18))]) = (*____in++))/*float*/;
  (((imagBuffer__509__18)[(int)(169 - (pos__511__18))]) = (*____in++))/*float*/;
  (modPos__517 = (169 - (pos__511__18)))/*int*/;
  for ((i__516 = 0)/*int*/; (i__516 < 170); (i__516++)) {{
      (real_curr__514 = (real_curr__514 + ((((realBuffer__508__18)[(int)modPos__517]) * ((real_weight__506__18)[(int)i__516])) + (((imagBuffer__509__18)[(int)modPos__517]) * ((imag_weight__507__18)[(int)i__516])))))/*float*/;
      (imag_curr__515 = (imag_curr__515 + ((((imagBuffer__509__18)[(int)modPos__517]) * ((real_weight__506__18)[(int)i__516])) + (((realBuffer__508__18)[(int)modPos__517]) * ((imag_weight__507__18)[(int)i__516])))))/*float*/;
      (modPos__517 = ((modPos__517 + 1) & 169))/*int*/;
    }
  }
  ((pos__511__18) = (((pos__511__18) + 1) & 169))/*int*/;
  ((*____out++)=real_curr__514);
  ((*____out++)=imag_curr__515);
  for ((i__516 = 2)/*int*/; (i__516 < 2); (i__516++)) {{
      (*____in++);
    }
  }
  ((count__510__18) = ((count__510__18) + 1))/*int*/;

  if (((count__510__18) == 170)) {{
    ((count__510__18) = 0)/*int*/;
    ((pos__511__18) = 0)/*int*/;
    for ((i__516 = 0)/*int*/; (i__516 < 170); (i__516++)) {{
        (((realBuffer__508__18)[(int)i__516]) = ((float)0.0))/*float*/;
        (((imagBuffer__509__18)[(int)i__516]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__520_121__18(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__514 = 0.0f;/* float */
      float imag_curr__515 = 0.0f;/* float */
      int i__516 = 0;/* int */
      int modPos__517 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__514 = ((float)0.0))/*float*/;
      (imag_curr__515 = ((float)0.0))/*float*/;
      (((realBuffer__508__18)[(int)(169 - (pos__511__18))]) = __pop__18())/*float*/;
      (((imagBuffer__509__18)[(int)(169 - (pos__511__18))]) = __pop__18())/*float*/;
      (modPos__517 = (169 - (pos__511__18)))/*int*/;
      for ((i__516 = 0)/*int*/; (i__516 < 170); (i__516++)) {{
          (real_curr__514 = (real_curr__514 + ((((realBuffer__508__18)[(int)modPos__517]) * ((real_weight__506__18)[(int)i__516])) + (((imagBuffer__509__18)[(int)modPos__517]) * ((imag_weight__507__18)[(int)i__516])))))/*float*/;
          (imag_curr__515 = (imag_curr__515 + ((((imagBuffer__509__18)[(int)modPos__517]) * ((real_weight__506__18)[(int)i__516])) + (((realBuffer__508__18)[(int)modPos__517]) * ((imag_weight__507__18)[(int)i__516])))))/*float*/;
          (modPos__517 = ((modPos__517 + 1) & 169))/*int*/;
        }
      }
      ((pos__511__18) = (((pos__511__18) + 1) & 169))/*int*/;
      __push__18(real_curr__514);
      __push__18(imag_curr__515);
      for ((i__516 = 2)/*int*/; (i__516 < 2); (i__516++)) {{
          __pop__18();
        }
      }
      ((count__510__18) = ((count__510__18) + 1))/*int*/;

      if (((count__510__18) == 170)) {{
        ((count__510__18) = 0)/*int*/;
        ((pos__511__18) = 0)/*int*/;
        for ((i__516 = 0)/*int*/; (i__516 < 170); (i__516++)) {{
            (((realBuffer__508__18)[(int)i__516]) = ((float)0.0))/*float*/;
            (((imagBuffer__509__18)[(int)i__516]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

// peek: 2 pop: 2 push 1
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

 
float mag__521__19(float real__524, float imag__525); 
void init_Magnitude__528_122__19();
inline void check_status__19();

void work_Magnitude__528_122__19(int);


inline float __pop__19() {
float res=BUFFER_18_19[TAIL_18_19];
TAIL_18_19++;
return res;
}

inline float __pop__19(int n) {
float res=BUFFER_18_19[TAIL_18_19];
TAIL_18_19+=n;

return res;
}

inline float __peek__19(int offs) {
return BUFFER_18_19[TAIL_18_19+offs];
}



inline void __push__19(float data) {
BUFFER_19_20[HEAD_19_20]=data;
HEAD_19_20++;
}



 
float mag__521__19(float real__524, float imag__525){
  return ((float)(((float)(sqrtf(((double)(((real__524 * real__524) + (imag__525 * imag__525)))))))));
}
 
void init_Magnitude__528_122__19(){
}
void save_file_pointer__19(object_write_buffer *buf) {}
void load_file_pointer__19(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Magnitude__528_122__19__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float f1__526 = 0.0f;/* float */
  float f2__527 = 0.0f;/* float */

  // mark begin: SIRFilter Magnitude

  (f1__526 = (*____in++))/*float*/;
  (f2__527 = (*____in++))/*float*/;
  ((*____out++)=mag__521__19(f1__526, f2__527));
  // mark end: SIRFilter Magnitude

}}


void work_Magnitude__528_122__19__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float f1__526 = 0.0f;/* float */
  float f2__527 = 0.0f;/* float */

  // mark begin: SIRFilter Magnitude

  (f1__526 = (*____in++))/*float*/;
  (f2__527 = (*____in++))/*float*/;
  ((*____out++)=mag__521__19(f1__526, f2__527));
  // mark end: SIRFilter Magnitude

}}


#endif // BUFFER_MERGE


 
void work_Magnitude__528_122__19(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float f1__526 = 0.0f;/* float */
      float f2__527 = 0.0f;/* float */

      // mark begin: SIRFilter Magnitude

      (f1__526 = __pop__19())/*float*/;
      (f2__527 = __pop__19())/*float*/;
      __push__19(mag__521__19(f1__526, f2__527));
      // mark end: SIRFilter Magnitude

    }
  }
}

// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_20;
int __counter_20 = 0;
int __steady_20 = 0;
int __tmp_20 = 0;
int __tmp2_20 = 0;
int *__state_flag_20 = NULL;
thread_info *__thread_20 = NULL;



int curSample__529__20 = 0;
void save_peek_buffer__20(object_write_buffer *buf);
void load_peek_buffer__20(object_write_buffer *buf);
void save_file_pointer__20(object_write_buffer *buf);
void load_file_pointer__20(object_write_buffer *buf);

 
void init_Detector__534_123__20();
inline void check_status__20();

void work_Detector__534_123__20(int);


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
BUFFER_20_11[HEAD_20_11]=data;
HEAD_20_11++;
}



 
void init_Detector__534_123__20(){
  ((curSample__529__20) = 0)/*int*/;
}
void save_file_pointer__20(object_write_buffer *buf) {}
void load_file_pointer__20(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Detector__534_123__20__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float inputVal__532 = 0.0f;/* float */
  float outputVal__533 = 0.0f;/* float */

  // mark begin: SIRFilter Detector

  (inputVal__532 = (*____in++))/*float*/;

  if ((0 && (42 == (curSample__529__20)))) {if ((inputVal__532 < ((float)0.1))) {(outputVal__533 = ((float)0.0))/*float*/;
    } else {(outputVal__533 = ((float)3.0))/*float*/;} } else {if ((inputVal__532 < ((float)0.1))) {(outputVal__533 = ((float)0.0))/*float*/;
  } else {(outputVal__533 = ((float)-3.0))/*float*/;}}
  (outputVal__533 = inputVal__532)/*float*/;
  ((curSample__529__20)++);

  if (((curSample__529__20) >= 170)) {((curSample__529__20) = 0)/*int*/;}
  ((*____out++)=outputVal__533);
  // mark end: SIRFilter Detector

}}


void work_Detector__534_123__20__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float inputVal__532 = 0.0f;/* float */
  float outputVal__533 = 0.0f;/* float */

  // mark begin: SIRFilter Detector

  (inputVal__532 = (*____in++))/*float*/;

  if ((0 && (42 == (curSample__529__20)))) {if ((inputVal__532 < ((float)0.1))) {(outputVal__533 = ((float)0.0))/*float*/;
    } else {(outputVal__533 = ((float)3.0))/*float*/;} } else {if ((inputVal__532 < ((float)0.1))) {(outputVal__533 = ((float)0.0))/*float*/;
  } else {(outputVal__533 = ((float)-3.0))/*float*/;}}
  (outputVal__533 = inputVal__532)/*float*/;
  ((curSample__529__20)++);

  if (((curSample__529__20) >= 170)) {((curSample__529__20) = 0)/*int*/;}
  ((*____out++)=outputVal__533);
  // mark end: SIRFilter Detector

}}


#endif // BUFFER_MERGE


 
void work_Detector__534_123__20(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float inputVal__532 = 0.0f;/* float */
      float outputVal__533 = 0.0f;/* float */

      // mark begin: SIRFilter Detector

      (inputVal__532 = __pop__20())/*float*/;

      if ((0 && (42 == (curSample__529__20)))) {if ((inputVal__532 < ((float)0.1))) {(outputVal__533 = ((float)0.0))/*float*/;
        } else {(outputVal__533 = ((float)3.0))/*float*/;} } else {if ((inputVal__532 < ((float)0.1))) {(outputVal__533 = ((float)0.0))/*float*/;
      } else {(outputVal__533 = ((float)-3.0))/*float*/;}}
      (outputVal__533 = inputVal__532)/*float*/;
      ((curSample__529__20)++);

      if (((curSample__529__20) >= 170)) {((curSample__529__20) = 0)/*int*/;}
      __push__20(outputVal__533);
      // mark end: SIRFilter Detector

    }
  }
}

// peek: 24 pop: 24 push 2
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_21;
int __counter_21 = 0;
int __steady_21 = 0;
int __tmp_21 = 0;
int __tmp2_21 = 0;
int *__state_flag_21 = NULL;
thread_info *__thread_21 = NULL;



float real_weight__535__21[12] = {0};
float imag_weight__536__21[12] = {0};
void save_peek_buffer__21(object_write_buffer *buf);
void load_peek_buffer__21(object_write_buffer *buf);
void save_file_pointer__21(object_write_buffer *buf);
void load_file_pointer__21(object_write_buffer *buf);

 
void init_BeamForm__546_125__21();
inline void check_status__21();

void work_BeamForm__546_125__21(int);


inline float __pop__21() {
float res=BUFFER_6_21[TAIL_6_21];
TAIL_6_21++;
return res;
}

inline float __pop__21(int n) {
float res=BUFFER_6_21[TAIL_6_21];
TAIL_6_21+=n;

return res;
}

inline float __peek__21(int offs) {
return BUFFER_6_21[TAIL_6_21+offs];
}



inline void __push__21(float data) {
BUFFER_21_22[HEAD_21_22]=data;
HEAD_21_22++;
}



 
void init_BeamForm__546_125__21(){
  int idx__544 = 0;/* int */
  int j__545 = 0;/* int */

  for ((j__545 = 0)/*int*/; (j__545 < 12); (j__545++)) {{
      (idx__544 = (j__545 + 1))/*int*/;
      (((real_weight__535__21)[(int)j__545]) = (((float)(sinf(((double)(idx__544))))) / ((float)((3 + idx__544)))))/*float*/;
      (((imag_weight__536__21)[(int)j__545]) = (((float)(cosf(((double)(idx__544))))) / ((float)((3 + idx__544)))))/*float*/;
    }
  }
}
void save_file_pointer__21(object_write_buffer *buf) {}
void load_file_pointer__21(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamForm__546_125__21__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__539 = 0.0f;/* float */
  float imag_curr__540 = 0.0f;/* float */
  float real_pop__541 = 0.0f;/* float */
  float imag_pop__542 = 0.0f;/* float */
  int i__543 = 0;/* int */

  // mark begin: SIRFilter BeamForm

  (real_curr__539 = ((float)0.0))/*float*/;
  (imag_curr__540 = ((float)0.0))/*float*/;
  for ((i__543 = 0)/*int*/; (i__543 < 12); (i__543++)) {{
      (real_pop__541 = (*____in++))/*float*/;
      (imag_pop__542 = (*____in++))/*float*/;
      (real_curr__539 = (real_curr__539 + ((((real_weight__535__21)[(int)i__543]) * real_pop__541) - (((imag_weight__536__21)[(int)i__543]) * imag_pop__542))))/*float*/;
      (imag_curr__540 = (imag_curr__540 + ((((real_weight__535__21)[(int)i__543]) * imag_pop__542) + (((imag_weight__536__21)[(int)i__543]) * real_pop__541))))/*float*/;
    }
  }
  ((*____out++)=real_curr__539);
  ((*____out++)=imag_curr__540);
  // mark end: SIRFilter BeamForm

}}


void work_BeamForm__546_125__21__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__539 = 0.0f;/* float */
  float imag_curr__540 = 0.0f;/* float */
  float real_pop__541 = 0.0f;/* float */
  float imag_pop__542 = 0.0f;/* float */
  int i__543 = 0;/* int */

  // mark begin: SIRFilter BeamForm

  (real_curr__539 = ((float)0.0))/*float*/;
  (imag_curr__540 = ((float)0.0))/*float*/;
  for ((i__543 = 0)/*int*/; (i__543 < 12); (i__543++)) {{
      (real_pop__541 = (*____in++))/*float*/;
      (imag_pop__542 = (*____in++))/*float*/;
      (real_curr__539 = (real_curr__539 + ((((real_weight__535__21)[(int)i__543]) * real_pop__541) - (((imag_weight__536__21)[(int)i__543]) * imag_pop__542))))/*float*/;
      (imag_curr__540 = (imag_curr__540 + ((((real_weight__535__21)[(int)i__543]) * imag_pop__542) + (((imag_weight__536__21)[(int)i__543]) * real_pop__541))))/*float*/;
    }
  }
  ((*____out++)=real_curr__539);
  ((*____out++)=imag_curr__540);
  // mark end: SIRFilter BeamForm

}}


#endif // BUFFER_MERGE


 
void work_BeamForm__546_125__21(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__539 = 0.0f;/* float */
      float imag_curr__540 = 0.0f;/* float */
      float real_pop__541 = 0.0f;/* float */
      float imag_pop__542 = 0.0f;/* float */
      int i__543 = 0;/* int */

      // mark begin: SIRFilter BeamForm

      (real_curr__539 = ((float)0.0))/*float*/;
      (imag_curr__540 = ((float)0.0))/*float*/;
      for ((i__543 = 0)/*int*/; (i__543 < 12); (i__543++)) {{
          (real_pop__541 = __pop__21())/*float*/;
          (imag_pop__542 = __pop__21())/*float*/;
          (real_curr__539 = (real_curr__539 + ((((real_weight__535__21)[(int)i__543]) * real_pop__541) - (((imag_weight__536__21)[(int)i__543]) * imag_pop__542))))/*float*/;
          (imag_curr__540 = (imag_curr__540 + ((((real_weight__535__21)[(int)i__543]) * imag_pop__542) + (((imag_weight__536__21)[(int)i__543]) * real_pop__541))))/*float*/;
        }
      }
      __push__21(real_curr__539);
      __push__21(imag_curr__540);
      // mark end: SIRFilter BeamForm

    }
  }
}

// peek: 2 pop: 2 push 2
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_22;
int __counter_22 = 0;
int __steady_22 = 0;
int __tmp_22 = 0;
int __tmp2_22 = 0;
int *__state_flag_22 = NULL;
thread_info *__thread_22 = NULL;



float real_weight__547__22[170] = {0};
float imag_weight__548__22[170] = {0};
float realBuffer__549__22[170] = {0};
float imagBuffer__550__22[170] = {0};
int count__551__22 = 0;
int pos__552__22 = 0;
void save_peek_buffer__22(object_write_buffer *buf);
void load_peek_buffer__22(object_write_buffer *buf);
void save_file_pointer__22(object_write_buffer *buf);
void load_file_pointer__22(object_write_buffer *buf);

 
void init_BeamFirFilter__561_126__22();
inline void check_status__22();

void work_BeamFirFilter__561_126__22(int);


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



 
void init_BeamFirFilter__561_126__22(){
  int idx__559 = 0;/* int */
  int j__560 = 0;/* int */

  ((pos__552__22) = 0)/*int*/;
  for ((j__560 = 0)/*int*/; (j__560 < 170); (j__560++)) {{
      (idx__559 = (j__560 + 1))/*int*/;
      (((real_weight__547__22)[(int)j__560]) = (((float)(sinf(((double)(idx__559))))) / ((float)(idx__559))))/*float*/;
      (((imag_weight__548__22)[(int)j__560]) = (((float)(cosf(((double)(idx__559))))) / ((float)(idx__559))))/*float*/;
    }
  }
}
void save_file_pointer__22(object_write_buffer *buf) {}
void load_file_pointer__22(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__561_126__22__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__555 = 0.0f;/* float */
  float imag_curr__556 = 0.0f;/* float */
  int i__557 = 0;/* int */
  int modPos__558 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__555 = ((float)0.0))/*float*/;
  (imag_curr__556 = ((float)0.0))/*float*/;
  (((realBuffer__549__22)[(int)(169 - (pos__552__22))]) = (*____in++))/*float*/;
  (((imagBuffer__550__22)[(int)(169 - (pos__552__22))]) = (*____in++))/*float*/;
  (modPos__558 = (169 - (pos__552__22)))/*int*/;
  for ((i__557 = 0)/*int*/; (i__557 < 170); (i__557++)) {{
      (real_curr__555 = (real_curr__555 + ((((realBuffer__549__22)[(int)modPos__558]) * ((real_weight__547__22)[(int)i__557])) + (((imagBuffer__550__22)[(int)modPos__558]) * ((imag_weight__548__22)[(int)i__557])))))/*float*/;
      (imag_curr__556 = (imag_curr__556 + ((((imagBuffer__550__22)[(int)modPos__558]) * ((real_weight__547__22)[(int)i__557])) + (((realBuffer__549__22)[(int)modPos__558]) * ((imag_weight__548__22)[(int)i__557])))))/*float*/;
      (modPos__558 = ((modPos__558 + 1) & 169))/*int*/;
    }
  }
  ((pos__552__22) = (((pos__552__22) + 1) & 169))/*int*/;
  ((*____out++)=real_curr__555);
  ((*____out++)=imag_curr__556);
  for ((i__557 = 2)/*int*/; (i__557 < 2); (i__557++)) {{
      (*____in++);
    }
  }
  ((count__551__22) = ((count__551__22) + 1))/*int*/;

  if (((count__551__22) == 170)) {{
    ((count__551__22) = 0)/*int*/;
    ((pos__552__22) = 0)/*int*/;
    for ((i__557 = 0)/*int*/; (i__557 < 170); (i__557++)) {{
        (((realBuffer__549__22)[(int)i__557]) = ((float)0.0))/*float*/;
        (((imagBuffer__550__22)[(int)i__557]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__561_126__22__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__555 = 0.0f;/* float */
  float imag_curr__556 = 0.0f;/* float */
  int i__557 = 0;/* int */
  int modPos__558 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__555 = ((float)0.0))/*float*/;
  (imag_curr__556 = ((float)0.0))/*float*/;
  (((realBuffer__549__22)[(int)(169 - (pos__552__22))]) = (*____in++))/*float*/;
  (((imagBuffer__550__22)[(int)(169 - (pos__552__22))]) = (*____in++))/*float*/;
  (modPos__558 = (169 - (pos__552__22)))/*int*/;
  for ((i__557 = 0)/*int*/; (i__557 < 170); (i__557++)) {{
      (real_curr__555 = (real_curr__555 + ((((realBuffer__549__22)[(int)modPos__558]) * ((real_weight__547__22)[(int)i__557])) + (((imagBuffer__550__22)[(int)modPos__558]) * ((imag_weight__548__22)[(int)i__557])))))/*float*/;
      (imag_curr__556 = (imag_curr__556 + ((((imagBuffer__550__22)[(int)modPos__558]) * ((real_weight__547__22)[(int)i__557])) + (((realBuffer__549__22)[(int)modPos__558]) * ((imag_weight__548__22)[(int)i__557])))))/*float*/;
      (modPos__558 = ((modPos__558 + 1) & 169))/*int*/;
    }
  }
  ((pos__552__22) = (((pos__552__22) + 1) & 169))/*int*/;
  ((*____out++)=real_curr__555);
  ((*____out++)=imag_curr__556);
  for ((i__557 = 2)/*int*/; (i__557 < 2); (i__557++)) {{
      (*____in++);
    }
  }
  ((count__551__22) = ((count__551__22) + 1))/*int*/;

  if (((count__551__22) == 170)) {{
    ((count__551__22) = 0)/*int*/;
    ((pos__552__22) = 0)/*int*/;
    for ((i__557 = 0)/*int*/; (i__557 < 170); (i__557++)) {{
        (((realBuffer__549__22)[(int)i__557]) = ((float)0.0))/*float*/;
        (((imagBuffer__550__22)[(int)i__557]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__561_126__22(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__555 = 0.0f;/* float */
      float imag_curr__556 = 0.0f;/* float */
      int i__557 = 0;/* int */
      int modPos__558 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__555 = ((float)0.0))/*float*/;
      (imag_curr__556 = ((float)0.0))/*float*/;
      (((realBuffer__549__22)[(int)(169 - (pos__552__22))]) = __pop__22())/*float*/;
      (((imagBuffer__550__22)[(int)(169 - (pos__552__22))]) = __pop__22())/*float*/;
      (modPos__558 = (169 - (pos__552__22)))/*int*/;
      for ((i__557 = 0)/*int*/; (i__557 < 170); (i__557++)) {{
          (real_curr__555 = (real_curr__555 + ((((realBuffer__549__22)[(int)modPos__558]) * ((real_weight__547__22)[(int)i__557])) + (((imagBuffer__550__22)[(int)modPos__558]) * ((imag_weight__548__22)[(int)i__557])))))/*float*/;
          (imag_curr__556 = (imag_curr__556 + ((((imagBuffer__550__22)[(int)modPos__558]) * ((real_weight__547__22)[(int)i__557])) + (((realBuffer__549__22)[(int)modPos__558]) * ((imag_weight__548__22)[(int)i__557])))))/*float*/;
          (modPos__558 = ((modPos__558 + 1) & 169))/*int*/;
        }
      }
      ((pos__552__22) = (((pos__552__22) + 1) & 169))/*int*/;
      __push__22(real_curr__555);
      __push__22(imag_curr__556);
      for ((i__557 = 2)/*int*/; (i__557 < 2); (i__557++)) {{
          __pop__22();
        }
      }
      ((count__551__22) = ((count__551__22) + 1))/*int*/;

      if (((count__551__22) == 170)) {{
        ((count__551__22) = 0)/*int*/;
        ((pos__552__22) = 0)/*int*/;
        for ((i__557 = 0)/*int*/; (i__557 < 170); (i__557++)) {{
            (((realBuffer__549__22)[(int)i__557]) = ((float)0.0))/*float*/;
            (((imagBuffer__550__22)[(int)i__557]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

// peek: 2 pop: 2 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_23;
int __counter_23 = 0;
int __steady_23 = 0;
int __tmp_23 = 0;
int __tmp2_23 = 0;
int *__state_flag_23 = NULL;
thread_info *__thread_23 = NULL;



void save_peek_buffer__23(object_write_buffer *buf);
void load_peek_buffer__23(object_write_buffer *buf);
void save_file_pointer__23(object_write_buffer *buf);
void load_file_pointer__23(object_write_buffer *buf);

 
float mag__562__23(float real__565, float imag__566); 
void init_Magnitude__569_127__23();
inline void check_status__23();

void work_Magnitude__569_127__23(int);


inline float __pop__23() {
float res=BUFFER_22_23[TAIL_22_23];
TAIL_22_23++;
return res;
}

inline float __pop__23(int n) {
float res=BUFFER_22_23[TAIL_22_23];
TAIL_22_23+=n;

return res;
}

inline float __peek__23(int offs) {
return BUFFER_22_23[TAIL_22_23+offs];
}



inline void __push__23(float data) {
BUFFER_23_24[HEAD_23_24]=data;
HEAD_23_24++;
}



 
float mag__562__23(float real__565, float imag__566){
  return ((float)(((float)(sqrtf(((double)(((real__565 * real__565) + (imag__566 * imag__566)))))))));
}
 
void init_Magnitude__569_127__23(){
}
void save_file_pointer__23(object_write_buffer *buf) {}
void load_file_pointer__23(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Magnitude__569_127__23__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float f1__567 = 0.0f;/* float */
  float f2__568 = 0.0f;/* float */

  // mark begin: SIRFilter Magnitude

  (f1__567 = (*____in++))/*float*/;
  (f2__568 = (*____in++))/*float*/;
  ((*____out++)=mag__562__23(f1__567, f2__568));
  // mark end: SIRFilter Magnitude

}}


void work_Magnitude__569_127__23__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float f1__567 = 0.0f;/* float */
  float f2__568 = 0.0f;/* float */

  // mark begin: SIRFilter Magnitude

  (f1__567 = (*____in++))/*float*/;
  (f2__568 = (*____in++))/*float*/;
  ((*____out++)=mag__562__23(f1__567, f2__568));
  // mark end: SIRFilter Magnitude

}}


#endif // BUFFER_MERGE


 
void work_Magnitude__569_127__23(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float f1__567 = 0.0f;/* float */
      float f2__568 = 0.0f;/* float */

      // mark begin: SIRFilter Magnitude

      (f1__567 = __pop__23())/*float*/;
      (f2__568 = __pop__23())/*float*/;
      __push__23(mag__562__23(f1__567, f2__568));
      // mark end: SIRFilter Magnitude

    }
  }
}

// peek: 1 pop: 1 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_24;
int __counter_24 = 0;
int __steady_24 = 0;
int __tmp_24 = 0;
int __tmp2_24 = 0;
int *__state_flag_24 = NULL;
thread_info *__thread_24 = NULL;



int curSample__570__24 = 0;
void save_peek_buffer__24(object_write_buffer *buf);
void load_peek_buffer__24(object_write_buffer *buf);
void save_file_pointer__24(object_write_buffer *buf);
void load_file_pointer__24(object_write_buffer *buf);

 
void init_Detector__575_128__24();
inline void check_status__24();

void work_Detector__575_128__24(int);


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
BUFFER_24_11[HEAD_24_11]=data;
HEAD_24_11++;
}



 
void init_Detector__575_128__24(){
  ((curSample__570__24) = 0)/*int*/;
}
void save_file_pointer__24(object_write_buffer *buf) {}
void load_file_pointer__24(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_Detector__575_128__24__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float inputVal__573 = 0.0f;/* float */
  float outputVal__574 = 0.0f;/* float */

  // mark begin: SIRFilter Detector

  (inputVal__573 = (*____in++))/*float*/;

  if ((0 && (42 == (curSample__570__24)))) {if ((inputVal__573 < ((float)0.1))) {(outputVal__574 = ((float)0.0))/*float*/;
    } else {(outputVal__574 = ((float)4.0))/*float*/;} } else {if ((inputVal__573 < ((float)0.1))) {(outputVal__574 = ((float)0.0))/*float*/;
  } else {(outputVal__574 = ((float)-4.0))/*float*/;}}
  (outputVal__574 = inputVal__573)/*float*/;
  ((curSample__570__24)++);

  if (((curSample__570__24) >= 170)) {((curSample__570__24) = 0)/*int*/;}
  ((*____out++)=outputVal__574);
  // mark end: SIRFilter Detector

}}


void work_Detector__575_128__24__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float inputVal__573 = 0.0f;/* float */
  float outputVal__574 = 0.0f;/* float */

  // mark begin: SIRFilter Detector

  (inputVal__573 = (*____in++))/*float*/;

  if ((0 && (42 == (curSample__570__24)))) {if ((inputVal__573 < ((float)0.1))) {(outputVal__574 = ((float)0.0))/*float*/;
    } else {(outputVal__574 = ((float)4.0))/*float*/;} } else {if ((inputVal__573 < ((float)0.1))) {(outputVal__574 = ((float)0.0))/*float*/;
  } else {(outputVal__574 = ((float)-4.0))/*float*/;}}
  (outputVal__574 = inputVal__573)/*float*/;
  ((curSample__570__24)++);

  if (((curSample__570__24) >= 170)) {((curSample__570__24) = 0)/*int*/;}
  ((*____out++)=outputVal__574);
  // mark end: SIRFilter Detector

}}


#endif // BUFFER_MERGE


 
void work_Detector__575_128__24(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float inputVal__573 = 0.0f;/* float */
      float outputVal__574 = 0.0f;/* float */

      // mark begin: SIRFilter Detector

      (inputVal__573 = __pop__24())/*float*/;

      if ((0 && (42 == (curSample__570__24)))) {if ((inputVal__573 < ((float)0.1))) {(outputVal__574 = ((float)0.0))/*float*/;
        } else {(outputVal__574 = ((float)4.0))/*float*/;} } else {if ((inputVal__573 < ((float)0.1))) {(outputVal__574 = ((float)0.0))/*float*/;
      } else {(outputVal__574 = ((float)-4.0))/*float*/;}}
      (outputVal__574 = inputVal__573)/*float*/;
      ((curSample__570__24)++);

      if (((curSample__570__24) >= 170)) {((curSample__570__24) = 0)/*int*/;}
      __push__24(outputVal__574);
      // mark end: SIRFilter Detector

    }
  }
}

// peek: 1 pop: 1 push 2
// init counts: 0 steady counts: 6

// ClusterFusion isEliminated: false



int __number_of_iterations_25;
int __counter_25 = 0;
int __steady_25 = 0;
int __tmp_25 = 0;
int __tmp2_25 = 0;
int *__state_flag_25 = NULL;
thread_info *__thread_25 = NULL;



int curSample__38__25 = 0;
void save_peek_buffer__25(object_write_buffer *buf);
void load_peek_buffer__25(object_write_buffer *buf);
void save_file_pointer__25(object_write_buffer *buf);
void load_file_pointer__25(object_write_buffer *buf);

 
void init_InputGenerate__41_66__25();
inline void check_status__25();

void work_InputGenerate__41_66__25(int);


inline int __pop__25() {
int res=BUFFER_1_25[TAIL_1_25];
TAIL_1_25++;
return res;
}

inline int __pop__25(int n) {
int res=BUFFER_1_25[TAIL_1_25];
TAIL_1_25+=n;

return res;
}

inline int __peek__25(int offs) {
return BUFFER_1_25[TAIL_1_25+offs];
}



inline void __push__25(float data) {
BUFFER_25_26[HEAD_25_26]=data;
HEAD_25_26++;
}



 
void init_InputGenerate__41_66__25(){
  ((curSample__38__25) = 0)/*int*/;
}
void save_file_pointer__25(object_write_buffer *buf) {}
void load_file_pointer__25(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_InputGenerate__41_66__25__mod(int ____n, int *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRFilter InputGenerate

  (*____in++);

  if ((1 && ((curSample__38__25) == 256))) {{
    ((*____out++)=((float)(sqrtf(((double)(((curSample__38__25) * 1)))))));
    ((*____out++)=(((float)(sqrtf(((double)(((curSample__38__25) * 1)))))) + ((float)1.0)));
  } } else {{
    ((*____out++)=(-((float)(sqrtf(((double)(((curSample__38__25) * 1))))))));
    ((*____out++)=(-(((float)(sqrtf(((double)(((curSample__38__25) * 1)))))) + ((float)1.0))));
  }}
  ((curSample__38__25)++);

  if (((curSample__38__25) >= 1024)) {((curSample__38__25) = 0)/*int*/;}
  // mark end: SIRFilter InputGenerate

}}


void work_InputGenerate__41_66__25__mod2(int ____n, int *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRFilter InputGenerate

  (*____in++);

  if ((1 && ((curSample__38__25) == 256))) {{
    ((*____out++)=((float)(sqrtf(((double)(((curSample__38__25) * 1)))))));
    ((*____out++)=(((float)(sqrtf(((double)(((curSample__38__25) * 1)))))) + ((float)1.0)));
  } } else {{
    ((*____out++)=(-((float)(sqrtf(((double)(((curSample__38__25) * 1))))))));
    ((*____out++)=(-(((float)(sqrtf(((double)(((curSample__38__25) * 1)))))) + ((float)1.0))));
  }}
  ((curSample__38__25)++);

  if (((curSample__38__25) >= 1024)) {((curSample__38__25) = 0)/*int*/;}
  // mark end: SIRFilter InputGenerate

}}


#endif // BUFFER_MERGE


 
void work_InputGenerate__41_66__25(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter InputGenerate

      __pop__25();

      if ((1 && ((curSample__38__25) == 256))) {{
        __push__25(((float)(sqrtf(((double)(((curSample__38__25) * 1)))))));
        __push__25((((float)(sqrtf(((double)(((curSample__38__25) * 1)))))) + ((float)1.0)));
      } } else {{
        __push__25((-((float)(sqrtf(((double)(((curSample__38__25) * 1))))))));
        __push__25((-(((float)(sqrtf(((double)(((curSample__38__25) * 1)))))) + ((float)1.0))));
      }}
      ((curSample__38__25)++);

      if (((curSample__38__25) >= 1024)) {((curSample__38__25) = 0)/*int*/;}
      // mark end: SIRFilter InputGenerate

    }
  }
}

// peek: 4 pop: 4 push 2
// init counts: 0 steady counts: 3

// ClusterFusion isEliminated: false



int __number_of_iterations_26;
int __counter_26 = 0;
int __steady_26 = 0;
int __tmp_26 = 0;
int __tmp2_26 = 0;
int *__state_flag_26 = NULL;
thread_info *__thread_26 = NULL;



float real_weight__42__26[64] = {0};
float imag_weight__43__26[64] = {0};
float realBuffer__44__26[64] = {0};
float imagBuffer__45__26[64] = {0};
int count__46__26 = 0;
int pos__47__26 = 0;
void save_peek_buffer__26(object_write_buffer *buf);
void load_peek_buffer__26(object_write_buffer *buf);
void save_file_pointer__26(object_write_buffer *buf);
void load_file_pointer__26(object_write_buffer *buf);

 
void init_BeamFirFilter__56_67__26();
inline void check_status__26();

void work_BeamFirFilter__56_67__26(int);


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



 
void init_BeamFirFilter__56_67__26(){
  int idx__54 = 0;/* int */
  int j__55 = 0;/* int */

  ((pos__47__26) = 0)/*int*/;
  for ((j__55 = 0)/*int*/; (j__55 < 64); (j__55++)) {{
      (idx__54 = (j__55 + 1))/*int*/;
      (((real_weight__42__26)[(int)j__55]) = (((float)(sinf(((double)(idx__54))))) / ((float)(idx__54))))/*float*/;
      (((imag_weight__43__26)[(int)j__55]) = (((float)(cosf(((double)(idx__54))))) / ((float)(idx__54))))/*float*/;
    }
  }
}
void save_file_pointer__26(object_write_buffer *buf) {}
void load_file_pointer__26(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__56_67__26__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__50 = 0.0f;/* float */
  float imag_curr__51 = 0.0f;/* float */
  int i__52 = 0;/* int */
  int modPos__53 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__50 = ((float)0.0))/*float*/;
  (imag_curr__51 = ((float)0.0))/*float*/;
  (((realBuffer__44__26)[(int)(63 - (pos__47__26))]) = (*____in++))/*float*/;
  (((imagBuffer__45__26)[(int)(63 - (pos__47__26))]) = (*____in++))/*float*/;
  (modPos__53 = (63 - (pos__47__26)))/*int*/;
  for ((i__52 = 0)/*int*/; (i__52 < 64); (i__52++)) {{
      (real_curr__50 = (real_curr__50 + ((((realBuffer__44__26)[(int)modPos__53]) * ((real_weight__42__26)[(int)i__52])) + (((imagBuffer__45__26)[(int)modPos__53]) * ((imag_weight__43__26)[(int)i__52])))))/*float*/;
      (imag_curr__51 = (imag_curr__51 + ((((imagBuffer__45__26)[(int)modPos__53]) * ((real_weight__42__26)[(int)i__52])) + (((realBuffer__44__26)[(int)modPos__53]) * ((imag_weight__43__26)[(int)i__52])))))/*float*/;
      (modPos__53 = ((modPos__53 + 1) & 63))/*int*/;
    }
  }
  ((pos__47__26) = (((pos__47__26) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__50);
  ((*____out++)=imag_curr__51);
  for ((i__52 = 2)/*int*/; (i__52 < 4); (i__52++)) {{
      (*____in++);
    }
  }
  ((count__46__26) = ((count__46__26) + 2))/*int*/;

  if (((count__46__26) == 1024)) {{
    ((count__46__26) = 0)/*int*/;
    ((pos__47__26) = 0)/*int*/;
    for ((i__52 = 0)/*int*/; (i__52 < 64); (i__52++)) {{
        (((realBuffer__44__26)[(int)i__52]) = ((float)0.0))/*float*/;
        (((imagBuffer__45__26)[(int)i__52]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__56_67__26__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__50 = 0.0f;/* float */
  float imag_curr__51 = 0.0f;/* float */
  int i__52 = 0;/* int */
  int modPos__53 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__50 = ((float)0.0))/*float*/;
  (imag_curr__51 = ((float)0.0))/*float*/;
  (((realBuffer__44__26)[(int)(63 - (pos__47__26))]) = (*____in++))/*float*/;
  (((imagBuffer__45__26)[(int)(63 - (pos__47__26))]) = (*____in++))/*float*/;
  (modPos__53 = (63 - (pos__47__26)))/*int*/;
  for ((i__52 = 0)/*int*/; (i__52 < 64); (i__52++)) {{
      (real_curr__50 = (real_curr__50 + ((((realBuffer__44__26)[(int)modPos__53]) * ((real_weight__42__26)[(int)i__52])) + (((imagBuffer__45__26)[(int)modPos__53]) * ((imag_weight__43__26)[(int)i__52])))))/*float*/;
      (imag_curr__51 = (imag_curr__51 + ((((imagBuffer__45__26)[(int)modPos__53]) * ((real_weight__42__26)[(int)i__52])) + (((realBuffer__44__26)[(int)modPos__53]) * ((imag_weight__43__26)[(int)i__52])))))/*float*/;
      (modPos__53 = ((modPos__53 + 1) & 63))/*int*/;
    }
  }
  ((pos__47__26) = (((pos__47__26) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__50);
  ((*____out++)=imag_curr__51);
  for ((i__52 = 2)/*int*/; (i__52 < 4); (i__52++)) {{
      (*____in++);
    }
  }
  ((count__46__26) = ((count__46__26) + 2))/*int*/;

  if (((count__46__26) == 1024)) {{
    ((count__46__26) = 0)/*int*/;
    ((pos__47__26) = 0)/*int*/;
    for ((i__52 = 0)/*int*/; (i__52 < 64); (i__52++)) {{
        (((realBuffer__44__26)[(int)i__52]) = ((float)0.0))/*float*/;
        (((imagBuffer__45__26)[(int)i__52]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__56_67__26(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__50 = 0.0f;/* float */
      float imag_curr__51 = 0.0f;/* float */
      int i__52 = 0;/* int */
      int modPos__53 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__50 = ((float)0.0))/*float*/;
      (imag_curr__51 = ((float)0.0))/*float*/;
      (((realBuffer__44__26)[(int)(63 - (pos__47__26))]) = __pop__26())/*float*/;
      (((imagBuffer__45__26)[(int)(63 - (pos__47__26))]) = __pop__26())/*float*/;
      (modPos__53 = (63 - (pos__47__26)))/*int*/;
      for ((i__52 = 0)/*int*/; (i__52 < 64); (i__52++)) {{
          (real_curr__50 = (real_curr__50 + ((((realBuffer__44__26)[(int)modPos__53]) * ((real_weight__42__26)[(int)i__52])) + (((imagBuffer__45__26)[(int)modPos__53]) * ((imag_weight__43__26)[(int)i__52])))))/*float*/;
          (imag_curr__51 = (imag_curr__51 + ((((imagBuffer__45__26)[(int)modPos__53]) * ((real_weight__42__26)[(int)i__52])) + (((realBuffer__44__26)[(int)modPos__53]) * ((imag_weight__43__26)[(int)i__52])))))/*float*/;
          (modPos__53 = ((modPos__53 + 1) & 63))/*int*/;
        }
      }
      ((pos__47__26) = (((pos__47__26) + 1) & 63))/*int*/;
      __push__26(real_curr__50);
      __push__26(imag_curr__51);
      for ((i__52 = 2)/*int*/; (i__52 < 4); (i__52++)) {{
          __pop__26();
        }
      }
      ((count__46__26) = ((count__46__26) + 2))/*int*/;

      if (((count__46__26) == 1024)) {{
        ((count__46__26) = 0)/*int*/;
        ((pos__47__26) = 0)/*int*/;
        for ((i__52 = 0)/*int*/; (i__52 < 64); (i__52++)) {{
            (((realBuffer__44__26)[(int)i__52]) = ((float)0.0))/*float*/;
            (((imagBuffer__45__26)[(int)i__52]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

// peek: 6 pop: 6 push 2
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_27;
int __counter_27 = 0;
int __steady_27 = 0;
int __tmp_27 = 0;
int __tmp2_27 = 0;
int *__state_flag_27 = NULL;
thread_info *__thread_27 = NULL;



float real_weight__57__27[64] = {0};
float imag_weight__58__27[64] = {0};
float realBuffer__59__27[64] = {0};
float imagBuffer__60__27[64] = {0};
int count__61__27 = 0;
int pos__62__27 = 0;
void save_peek_buffer__27(object_write_buffer *buf);
void load_peek_buffer__27(object_write_buffer *buf);
void save_file_pointer__27(object_write_buffer *buf);
void load_file_pointer__27(object_write_buffer *buf);

 
void init_BeamFirFilter__71_68__27();
inline void check_status__27();

void work_BeamFirFilter__71_68__27(int);


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
BUFFER_27_5[HEAD_27_5]=data;
HEAD_27_5++;
}



 
void init_BeamFirFilter__71_68__27(){
  int idx__69 = 0;/* int */
  int j__70 = 0;/* int */

  ((pos__62__27) = 0)/*int*/;
  for ((j__70 = 0)/*int*/; (j__70 < 64); (j__70++)) {{
      (idx__69 = (j__70 + 1))/*int*/;
      (((real_weight__57__27)[(int)j__70]) = (((float)(sinf(((double)(idx__69))))) / ((float)(idx__69))))/*float*/;
      (((imag_weight__58__27)[(int)j__70]) = (((float)(cosf(((double)(idx__69))))) / ((float)(idx__69))))/*float*/;
    }
  }
}
void save_file_pointer__27(object_write_buffer *buf) {}
void load_file_pointer__27(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__71_68__27__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__65 = 0.0f;/* float */
  float imag_curr__66 = 0.0f;/* float */
  int i__67 = 0;/* int */
  int modPos__68 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__65 = ((float)0.0))/*float*/;
  (imag_curr__66 = ((float)0.0))/*float*/;
  (((realBuffer__59__27)[(int)(63 - (pos__62__27))]) = (*____in++))/*float*/;
  (((imagBuffer__60__27)[(int)(63 - (pos__62__27))]) = (*____in++))/*float*/;
  (modPos__68 = (63 - (pos__62__27)))/*int*/;
  for ((i__67 = 0)/*int*/; (i__67 < 64); (i__67++)) {{
      (real_curr__65 = (real_curr__65 + ((((realBuffer__59__27)[(int)modPos__68]) * ((real_weight__57__27)[(int)i__67])) + (((imagBuffer__60__27)[(int)modPos__68]) * ((imag_weight__58__27)[(int)i__67])))))/*float*/;
      (imag_curr__66 = (imag_curr__66 + ((((imagBuffer__60__27)[(int)modPos__68]) * ((real_weight__57__27)[(int)i__67])) + (((realBuffer__59__27)[(int)modPos__68]) * ((imag_weight__58__27)[(int)i__67])))))/*float*/;
      (modPos__68 = ((modPos__68 + 1) & 63))/*int*/;
    }
  }
  ((pos__62__27) = (((pos__62__27) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__65);
  ((*____out++)=imag_curr__66);
  for ((i__67 = 2)/*int*/; (i__67 < 6); (i__67++)) {{
      (*____in++);
    }
  }
  ((count__61__27) = ((count__61__27) + 3))/*int*/;

  if (((count__61__27) == 512)) {{
    ((count__61__27) = 0)/*int*/;
    ((pos__62__27) = 0)/*int*/;
    for ((i__67 = 0)/*int*/; (i__67 < 64); (i__67++)) {{
        (((realBuffer__59__27)[(int)i__67]) = ((float)0.0))/*float*/;
        (((imagBuffer__60__27)[(int)i__67]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__71_68__27__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__65 = 0.0f;/* float */
  float imag_curr__66 = 0.0f;/* float */
  int i__67 = 0;/* int */
  int modPos__68 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__65 = ((float)0.0))/*float*/;
  (imag_curr__66 = ((float)0.0))/*float*/;
  (((realBuffer__59__27)[(int)(63 - (pos__62__27))]) = (*____in++))/*float*/;
  (((imagBuffer__60__27)[(int)(63 - (pos__62__27))]) = (*____in++))/*float*/;
  (modPos__68 = (63 - (pos__62__27)))/*int*/;
  for ((i__67 = 0)/*int*/; (i__67 < 64); (i__67++)) {{
      (real_curr__65 = (real_curr__65 + ((((realBuffer__59__27)[(int)modPos__68]) * ((real_weight__57__27)[(int)i__67])) + (((imagBuffer__60__27)[(int)modPos__68]) * ((imag_weight__58__27)[(int)i__67])))))/*float*/;
      (imag_curr__66 = (imag_curr__66 + ((((imagBuffer__60__27)[(int)modPos__68]) * ((real_weight__57__27)[(int)i__67])) + (((realBuffer__59__27)[(int)modPos__68]) * ((imag_weight__58__27)[(int)i__67])))))/*float*/;
      (modPos__68 = ((modPos__68 + 1) & 63))/*int*/;
    }
  }
  ((pos__62__27) = (((pos__62__27) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__65);
  ((*____out++)=imag_curr__66);
  for ((i__67 = 2)/*int*/; (i__67 < 6); (i__67++)) {{
      (*____in++);
    }
  }
  ((count__61__27) = ((count__61__27) + 3))/*int*/;

  if (((count__61__27) == 512)) {{
    ((count__61__27) = 0)/*int*/;
    ((pos__62__27) = 0)/*int*/;
    for ((i__67 = 0)/*int*/; (i__67 < 64); (i__67++)) {{
        (((realBuffer__59__27)[(int)i__67]) = ((float)0.0))/*float*/;
        (((imagBuffer__60__27)[(int)i__67]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__71_68__27(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__65 = 0.0f;/* float */
      float imag_curr__66 = 0.0f;/* float */
      int i__67 = 0;/* int */
      int modPos__68 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__65 = ((float)0.0))/*float*/;
      (imag_curr__66 = ((float)0.0))/*float*/;
      (((realBuffer__59__27)[(int)(63 - (pos__62__27))]) = __pop__27())/*float*/;
      (((imagBuffer__60__27)[(int)(63 - (pos__62__27))]) = __pop__27())/*float*/;
      (modPos__68 = (63 - (pos__62__27)))/*int*/;
      for ((i__67 = 0)/*int*/; (i__67 < 64); (i__67++)) {{
          (real_curr__65 = (real_curr__65 + ((((realBuffer__59__27)[(int)modPos__68]) * ((real_weight__57__27)[(int)i__67])) + (((imagBuffer__60__27)[(int)modPos__68]) * ((imag_weight__58__27)[(int)i__67])))))/*float*/;
          (imag_curr__66 = (imag_curr__66 + ((((imagBuffer__60__27)[(int)modPos__68]) * ((real_weight__57__27)[(int)i__67])) + (((realBuffer__59__27)[(int)modPos__68]) * ((imag_weight__58__27)[(int)i__67])))))/*float*/;
          (modPos__68 = ((modPos__68 + 1) & 63))/*int*/;
        }
      }
      ((pos__62__27) = (((pos__62__27) + 1) & 63))/*int*/;
      __push__27(real_curr__65);
      __push__27(imag_curr__66);
      for ((i__67 = 2)/*int*/; (i__67 < 6); (i__67++)) {{
          __pop__27();
        }
      }
      ((count__61__27) = ((count__61__27) + 3))/*int*/;

      if (((count__61__27) == 512)) {{
        ((count__61__27) = 0)/*int*/;
        ((pos__62__27) = 0)/*int*/;
        for ((i__67 = 0)/*int*/; (i__67 < 64); (i__67++)) {{
            (((realBuffer__59__27)[(int)i__67]) = ((float)0.0))/*float*/;
            (((imagBuffer__60__27)[(int)i__67]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

// peek: 1 pop: 1 push 2
// init counts: 0 steady counts: 6

// ClusterFusion isEliminated: false



int __number_of_iterations_28;
int __counter_28 = 0;
int __steady_28 = 0;
int __tmp_28 = 0;
int __tmp2_28 = 0;
int *__state_flag_28 = NULL;
thread_info *__thread_28 = NULL;



int curSample__72__28 = 0;
void save_peek_buffer__28(object_write_buffer *buf);
void load_peek_buffer__28(object_write_buffer *buf);
void save_file_pointer__28(object_write_buffer *buf);
void load_file_pointer__28(object_write_buffer *buf);

 
void init_InputGenerate__75_70__28();
inline void check_status__28();

void work_InputGenerate__75_70__28(int);


inline int __pop__28() {
int res=BUFFER_1_28[TAIL_1_28];
TAIL_1_28++;
return res;
}

inline int __pop__28(int n) {
int res=BUFFER_1_28[TAIL_1_28];
TAIL_1_28+=n;

return res;
}

inline int __peek__28(int offs) {
return BUFFER_1_28[TAIL_1_28+offs];
}



inline void __push__28(float data) {
BUFFER_28_29[HEAD_28_29]=data;
HEAD_28_29++;
}



 
void init_InputGenerate__75_70__28(){
  ((curSample__72__28) = 0)/*int*/;
}
void save_file_pointer__28(object_write_buffer *buf) {}
void load_file_pointer__28(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_InputGenerate__75_70__28__mod(int ____n, int *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRFilter InputGenerate

  (*____in++);

  if ((0 && ((curSample__72__28) == 256))) {{
    ((*____out++)=((float)(sqrtf(((double)(((curSample__72__28) * 2)))))));
    ((*____out++)=(((float)(sqrtf(((double)(((curSample__72__28) * 2)))))) + ((float)1.0)));
  } } else {{
    ((*____out++)=(-((float)(sqrtf(((double)(((curSample__72__28) * 2))))))));
    ((*____out++)=(-(((float)(sqrtf(((double)(((curSample__72__28) * 2)))))) + ((float)1.0))));
  }}
  ((curSample__72__28)++);

  if (((curSample__72__28) >= 1024)) {((curSample__72__28) = 0)/*int*/;}
  // mark end: SIRFilter InputGenerate

}}


void work_InputGenerate__75_70__28__mod2(int ____n, int *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRFilter InputGenerate

  (*____in++);

  if ((0 && ((curSample__72__28) == 256))) {{
    ((*____out++)=((float)(sqrtf(((double)(((curSample__72__28) * 2)))))));
    ((*____out++)=(((float)(sqrtf(((double)(((curSample__72__28) * 2)))))) + ((float)1.0)));
  } } else {{
    ((*____out++)=(-((float)(sqrtf(((double)(((curSample__72__28) * 2))))))));
    ((*____out++)=(-(((float)(sqrtf(((double)(((curSample__72__28) * 2)))))) + ((float)1.0))));
  }}
  ((curSample__72__28)++);

  if (((curSample__72__28) >= 1024)) {((curSample__72__28) = 0)/*int*/;}
  // mark end: SIRFilter InputGenerate

}}


#endif // BUFFER_MERGE


 
void work_InputGenerate__75_70__28(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter InputGenerate

      __pop__28();

      if ((0 && ((curSample__72__28) == 256))) {{
        __push__28(((float)(sqrtf(((double)(((curSample__72__28) * 2)))))));
        __push__28((((float)(sqrtf(((double)(((curSample__72__28) * 2)))))) + ((float)1.0)));
      } } else {{
        __push__28((-((float)(sqrtf(((double)(((curSample__72__28) * 2))))))));
        __push__28((-(((float)(sqrtf(((double)(((curSample__72__28) * 2)))))) + ((float)1.0))));
      }}
      ((curSample__72__28)++);

      if (((curSample__72__28) >= 1024)) {((curSample__72__28) = 0)/*int*/;}
      // mark end: SIRFilter InputGenerate

    }
  }
}

// peek: 4 pop: 4 push 2
// init counts: 0 steady counts: 3

// ClusterFusion isEliminated: false



int __number_of_iterations_29;
int __counter_29 = 0;
int __steady_29 = 0;
int __tmp_29 = 0;
int __tmp2_29 = 0;
int *__state_flag_29 = NULL;
thread_info *__thread_29 = NULL;



float real_weight__76__29[64] = {0};
float imag_weight__77__29[64] = {0};
float realBuffer__78__29[64] = {0};
float imagBuffer__79__29[64] = {0};
int count__80__29 = 0;
int pos__81__29 = 0;
void save_peek_buffer__29(object_write_buffer *buf);
void load_peek_buffer__29(object_write_buffer *buf);
void save_file_pointer__29(object_write_buffer *buf);
void load_file_pointer__29(object_write_buffer *buf);

 
void init_BeamFirFilter__90_71__29();
inline void check_status__29();

void work_BeamFirFilter__90_71__29(int);


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



 
void init_BeamFirFilter__90_71__29(){
  int idx__88 = 0;/* int */
  int j__89 = 0;/* int */

  ((pos__81__29) = 0)/*int*/;
  for ((j__89 = 0)/*int*/; (j__89 < 64); (j__89++)) {{
      (idx__88 = (j__89 + 1))/*int*/;
      (((real_weight__76__29)[(int)j__89]) = (((float)(sinf(((double)(idx__88))))) / ((float)(idx__88))))/*float*/;
      (((imag_weight__77__29)[(int)j__89]) = (((float)(cosf(((double)(idx__88))))) / ((float)(idx__88))))/*float*/;
    }
  }
}
void save_file_pointer__29(object_write_buffer *buf) {}
void load_file_pointer__29(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__90_71__29__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__84 = 0.0f;/* float */
  float imag_curr__85 = 0.0f;/* float */
  int i__86 = 0;/* int */
  int modPos__87 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__84 = ((float)0.0))/*float*/;
  (imag_curr__85 = ((float)0.0))/*float*/;
  (((realBuffer__78__29)[(int)(63 - (pos__81__29))]) = (*____in++))/*float*/;
  (((imagBuffer__79__29)[(int)(63 - (pos__81__29))]) = (*____in++))/*float*/;
  (modPos__87 = (63 - (pos__81__29)))/*int*/;
  for ((i__86 = 0)/*int*/; (i__86 < 64); (i__86++)) {{
      (real_curr__84 = (real_curr__84 + ((((realBuffer__78__29)[(int)modPos__87]) * ((real_weight__76__29)[(int)i__86])) + (((imagBuffer__79__29)[(int)modPos__87]) * ((imag_weight__77__29)[(int)i__86])))))/*float*/;
      (imag_curr__85 = (imag_curr__85 + ((((imagBuffer__79__29)[(int)modPos__87]) * ((real_weight__76__29)[(int)i__86])) + (((realBuffer__78__29)[(int)modPos__87]) * ((imag_weight__77__29)[(int)i__86])))))/*float*/;
      (modPos__87 = ((modPos__87 + 1) & 63))/*int*/;
    }
  }
  ((pos__81__29) = (((pos__81__29) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__84);
  ((*____out++)=imag_curr__85);
  for ((i__86 = 2)/*int*/; (i__86 < 4); (i__86++)) {{
      (*____in++);
    }
  }
  ((count__80__29) = ((count__80__29) + 2))/*int*/;

  if (((count__80__29) == 1024)) {{
    ((count__80__29) = 0)/*int*/;
    ((pos__81__29) = 0)/*int*/;
    for ((i__86 = 0)/*int*/; (i__86 < 64); (i__86++)) {{
        (((realBuffer__78__29)[(int)i__86]) = ((float)0.0))/*float*/;
        (((imagBuffer__79__29)[(int)i__86]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__90_71__29__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__84 = 0.0f;/* float */
  float imag_curr__85 = 0.0f;/* float */
  int i__86 = 0;/* int */
  int modPos__87 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__84 = ((float)0.0))/*float*/;
  (imag_curr__85 = ((float)0.0))/*float*/;
  (((realBuffer__78__29)[(int)(63 - (pos__81__29))]) = (*____in++))/*float*/;
  (((imagBuffer__79__29)[(int)(63 - (pos__81__29))]) = (*____in++))/*float*/;
  (modPos__87 = (63 - (pos__81__29)))/*int*/;
  for ((i__86 = 0)/*int*/; (i__86 < 64); (i__86++)) {{
      (real_curr__84 = (real_curr__84 + ((((realBuffer__78__29)[(int)modPos__87]) * ((real_weight__76__29)[(int)i__86])) + (((imagBuffer__79__29)[(int)modPos__87]) * ((imag_weight__77__29)[(int)i__86])))))/*float*/;
      (imag_curr__85 = (imag_curr__85 + ((((imagBuffer__79__29)[(int)modPos__87]) * ((real_weight__76__29)[(int)i__86])) + (((realBuffer__78__29)[(int)modPos__87]) * ((imag_weight__77__29)[(int)i__86])))))/*float*/;
      (modPos__87 = ((modPos__87 + 1) & 63))/*int*/;
    }
  }
  ((pos__81__29) = (((pos__81__29) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__84);
  ((*____out++)=imag_curr__85);
  for ((i__86 = 2)/*int*/; (i__86 < 4); (i__86++)) {{
      (*____in++);
    }
  }
  ((count__80__29) = ((count__80__29) + 2))/*int*/;

  if (((count__80__29) == 1024)) {{
    ((count__80__29) = 0)/*int*/;
    ((pos__81__29) = 0)/*int*/;
    for ((i__86 = 0)/*int*/; (i__86 < 64); (i__86++)) {{
        (((realBuffer__78__29)[(int)i__86]) = ((float)0.0))/*float*/;
        (((imagBuffer__79__29)[(int)i__86]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__90_71__29(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__84 = 0.0f;/* float */
      float imag_curr__85 = 0.0f;/* float */
      int i__86 = 0;/* int */
      int modPos__87 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__84 = ((float)0.0))/*float*/;
      (imag_curr__85 = ((float)0.0))/*float*/;
      (((realBuffer__78__29)[(int)(63 - (pos__81__29))]) = __pop__29())/*float*/;
      (((imagBuffer__79__29)[(int)(63 - (pos__81__29))]) = __pop__29())/*float*/;
      (modPos__87 = (63 - (pos__81__29)))/*int*/;
      for ((i__86 = 0)/*int*/; (i__86 < 64); (i__86++)) {{
          (real_curr__84 = (real_curr__84 + ((((realBuffer__78__29)[(int)modPos__87]) * ((real_weight__76__29)[(int)i__86])) + (((imagBuffer__79__29)[(int)modPos__87]) * ((imag_weight__77__29)[(int)i__86])))))/*float*/;
          (imag_curr__85 = (imag_curr__85 + ((((imagBuffer__79__29)[(int)modPos__87]) * ((real_weight__76__29)[(int)i__86])) + (((realBuffer__78__29)[(int)modPos__87]) * ((imag_weight__77__29)[(int)i__86])))))/*float*/;
          (modPos__87 = ((modPos__87 + 1) & 63))/*int*/;
        }
      }
      ((pos__81__29) = (((pos__81__29) + 1) & 63))/*int*/;
      __push__29(real_curr__84);
      __push__29(imag_curr__85);
      for ((i__86 = 2)/*int*/; (i__86 < 4); (i__86++)) {{
          __pop__29();
        }
      }
      ((count__80__29) = ((count__80__29) + 2))/*int*/;

      if (((count__80__29) == 1024)) {{
        ((count__80__29) = 0)/*int*/;
        ((pos__81__29) = 0)/*int*/;
        for ((i__86 = 0)/*int*/; (i__86 < 64); (i__86++)) {{
            (((realBuffer__78__29)[(int)i__86]) = ((float)0.0))/*float*/;
            (((imagBuffer__79__29)[(int)i__86]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

// peek: 6 pop: 6 push 2
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_30;
int __counter_30 = 0;
int __steady_30 = 0;
int __tmp_30 = 0;
int __tmp2_30 = 0;
int *__state_flag_30 = NULL;
thread_info *__thread_30 = NULL;



float real_weight__91__30[64] = {0};
float imag_weight__92__30[64] = {0};
float realBuffer__93__30[64] = {0};
float imagBuffer__94__30[64] = {0};
int count__95__30 = 0;
int pos__96__30 = 0;
void save_peek_buffer__30(object_write_buffer *buf);
void load_peek_buffer__30(object_write_buffer *buf);
void save_file_pointer__30(object_write_buffer *buf);
void load_file_pointer__30(object_write_buffer *buf);

 
void init_BeamFirFilter__105_72__30();
inline void check_status__30();

void work_BeamFirFilter__105_72__30(int);


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
BUFFER_30_5[HEAD_30_5]=data;
HEAD_30_5++;
}



 
void init_BeamFirFilter__105_72__30(){
  int idx__103 = 0;/* int */
  int j__104 = 0;/* int */

  ((pos__96__30) = 0)/*int*/;
  for ((j__104 = 0)/*int*/; (j__104 < 64); (j__104++)) {{
      (idx__103 = (j__104 + 1))/*int*/;
      (((real_weight__91__30)[(int)j__104]) = (((float)(sinf(((double)(idx__103))))) / ((float)(idx__103))))/*float*/;
      (((imag_weight__92__30)[(int)j__104]) = (((float)(cosf(((double)(idx__103))))) / ((float)(idx__103))))/*float*/;
    }
  }
}
void save_file_pointer__30(object_write_buffer *buf) {}
void load_file_pointer__30(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__105_72__30__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__99 = 0.0f;/* float */
  float imag_curr__100 = 0.0f;/* float */
  int i__101 = 0;/* int */
  int modPos__102 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__99 = ((float)0.0))/*float*/;
  (imag_curr__100 = ((float)0.0))/*float*/;
  (((realBuffer__93__30)[(int)(63 - (pos__96__30))]) = (*____in++))/*float*/;
  (((imagBuffer__94__30)[(int)(63 - (pos__96__30))]) = (*____in++))/*float*/;
  (modPos__102 = (63 - (pos__96__30)))/*int*/;
  for ((i__101 = 0)/*int*/; (i__101 < 64); (i__101++)) {{
      (real_curr__99 = (real_curr__99 + ((((realBuffer__93__30)[(int)modPos__102]) * ((real_weight__91__30)[(int)i__101])) + (((imagBuffer__94__30)[(int)modPos__102]) * ((imag_weight__92__30)[(int)i__101])))))/*float*/;
      (imag_curr__100 = (imag_curr__100 + ((((imagBuffer__94__30)[(int)modPos__102]) * ((real_weight__91__30)[(int)i__101])) + (((realBuffer__93__30)[(int)modPos__102]) * ((imag_weight__92__30)[(int)i__101])))))/*float*/;
      (modPos__102 = ((modPos__102 + 1) & 63))/*int*/;
    }
  }
  ((pos__96__30) = (((pos__96__30) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__99);
  ((*____out++)=imag_curr__100);
  for ((i__101 = 2)/*int*/; (i__101 < 6); (i__101++)) {{
      (*____in++);
    }
  }
  ((count__95__30) = ((count__95__30) + 3))/*int*/;

  if (((count__95__30) == 512)) {{
    ((count__95__30) = 0)/*int*/;
    ((pos__96__30) = 0)/*int*/;
    for ((i__101 = 0)/*int*/; (i__101 < 64); (i__101++)) {{
        (((realBuffer__93__30)[(int)i__101]) = ((float)0.0))/*float*/;
        (((imagBuffer__94__30)[(int)i__101]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__105_72__30__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__99 = 0.0f;/* float */
  float imag_curr__100 = 0.0f;/* float */
  int i__101 = 0;/* int */
  int modPos__102 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__99 = ((float)0.0))/*float*/;
  (imag_curr__100 = ((float)0.0))/*float*/;
  (((realBuffer__93__30)[(int)(63 - (pos__96__30))]) = (*____in++))/*float*/;
  (((imagBuffer__94__30)[(int)(63 - (pos__96__30))]) = (*____in++))/*float*/;
  (modPos__102 = (63 - (pos__96__30)))/*int*/;
  for ((i__101 = 0)/*int*/; (i__101 < 64); (i__101++)) {{
      (real_curr__99 = (real_curr__99 + ((((realBuffer__93__30)[(int)modPos__102]) * ((real_weight__91__30)[(int)i__101])) + (((imagBuffer__94__30)[(int)modPos__102]) * ((imag_weight__92__30)[(int)i__101])))))/*float*/;
      (imag_curr__100 = (imag_curr__100 + ((((imagBuffer__94__30)[(int)modPos__102]) * ((real_weight__91__30)[(int)i__101])) + (((realBuffer__93__30)[(int)modPos__102]) * ((imag_weight__92__30)[(int)i__101])))))/*float*/;
      (modPos__102 = ((modPos__102 + 1) & 63))/*int*/;
    }
  }
  ((pos__96__30) = (((pos__96__30) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__99);
  ((*____out++)=imag_curr__100);
  for ((i__101 = 2)/*int*/; (i__101 < 6); (i__101++)) {{
      (*____in++);
    }
  }
  ((count__95__30) = ((count__95__30) + 3))/*int*/;

  if (((count__95__30) == 512)) {{
    ((count__95__30) = 0)/*int*/;
    ((pos__96__30) = 0)/*int*/;
    for ((i__101 = 0)/*int*/; (i__101 < 64); (i__101++)) {{
        (((realBuffer__93__30)[(int)i__101]) = ((float)0.0))/*float*/;
        (((imagBuffer__94__30)[(int)i__101]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__105_72__30(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__99 = 0.0f;/* float */
      float imag_curr__100 = 0.0f;/* float */
      int i__101 = 0;/* int */
      int modPos__102 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__99 = ((float)0.0))/*float*/;
      (imag_curr__100 = ((float)0.0))/*float*/;
      (((realBuffer__93__30)[(int)(63 - (pos__96__30))]) = __pop__30())/*float*/;
      (((imagBuffer__94__30)[(int)(63 - (pos__96__30))]) = __pop__30())/*float*/;
      (modPos__102 = (63 - (pos__96__30)))/*int*/;
      for ((i__101 = 0)/*int*/; (i__101 < 64); (i__101++)) {{
          (real_curr__99 = (real_curr__99 + ((((realBuffer__93__30)[(int)modPos__102]) * ((real_weight__91__30)[(int)i__101])) + (((imagBuffer__94__30)[(int)modPos__102]) * ((imag_weight__92__30)[(int)i__101])))))/*float*/;
          (imag_curr__100 = (imag_curr__100 + ((((imagBuffer__94__30)[(int)modPos__102]) * ((real_weight__91__30)[(int)i__101])) + (((realBuffer__93__30)[(int)modPos__102]) * ((imag_weight__92__30)[(int)i__101])))))/*float*/;
          (modPos__102 = ((modPos__102 + 1) & 63))/*int*/;
        }
      }
      ((pos__96__30) = (((pos__96__30) + 1) & 63))/*int*/;
      __push__30(real_curr__99);
      __push__30(imag_curr__100);
      for ((i__101 = 2)/*int*/; (i__101 < 6); (i__101++)) {{
          __pop__30();
        }
      }
      ((count__95__30) = ((count__95__30) + 3))/*int*/;

      if (((count__95__30) == 512)) {{
        ((count__95__30) = 0)/*int*/;
        ((pos__96__30) = 0)/*int*/;
        for ((i__101 = 0)/*int*/; (i__101 < 64); (i__101++)) {{
            (((realBuffer__93__30)[(int)i__101]) = ((float)0.0))/*float*/;
            (((imagBuffer__94__30)[(int)i__101]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

// peek: 1 pop: 1 push 2
// init counts: 0 steady counts: 6

// ClusterFusion isEliminated: false



int __number_of_iterations_31;
int __counter_31 = 0;
int __steady_31 = 0;
int __tmp_31 = 0;
int __tmp2_31 = 0;
int *__state_flag_31 = NULL;
thread_info *__thread_31 = NULL;



int curSample__106__31 = 0;
void save_peek_buffer__31(object_write_buffer *buf);
void load_peek_buffer__31(object_write_buffer *buf);
void save_file_pointer__31(object_write_buffer *buf);
void load_file_pointer__31(object_write_buffer *buf);

 
void init_InputGenerate__109_74__31();
inline void check_status__31();

void work_InputGenerate__109_74__31(int);


inline int __pop__31() {
int res=BUFFER_1_31[TAIL_1_31];
TAIL_1_31++;
return res;
}

inline int __pop__31(int n) {
int res=BUFFER_1_31[TAIL_1_31];
TAIL_1_31+=n;

return res;
}

inline int __peek__31(int offs) {
return BUFFER_1_31[TAIL_1_31+offs];
}



inline void __push__31(float data) {
BUFFER_31_32[HEAD_31_32]=data;
HEAD_31_32++;
}



 
void init_InputGenerate__109_74__31(){
  ((curSample__106__31) = 0)/*int*/;
}
void save_file_pointer__31(object_write_buffer *buf) {}
void load_file_pointer__31(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_InputGenerate__109_74__31__mod(int ____n, int *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRFilter InputGenerate

  (*____in++);

  if ((0 && ((curSample__106__31) == 256))) {{
    ((*____out++)=((float)(sqrtf(((double)(((curSample__106__31) * 3)))))));
    ((*____out++)=(((float)(sqrtf(((double)(((curSample__106__31) * 3)))))) + ((float)1.0)));
  } } else {{
    ((*____out++)=(-((float)(sqrtf(((double)(((curSample__106__31) * 3))))))));
    ((*____out++)=(-(((float)(sqrtf(((double)(((curSample__106__31) * 3)))))) + ((float)1.0))));
  }}
  ((curSample__106__31)++);

  if (((curSample__106__31) >= 1024)) {((curSample__106__31) = 0)/*int*/;}
  // mark end: SIRFilter InputGenerate

}}


void work_InputGenerate__109_74__31__mod2(int ____n, int *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRFilter InputGenerate

  (*____in++);

  if ((0 && ((curSample__106__31) == 256))) {{
    ((*____out++)=((float)(sqrtf(((double)(((curSample__106__31) * 3)))))));
    ((*____out++)=(((float)(sqrtf(((double)(((curSample__106__31) * 3)))))) + ((float)1.0)));
  } } else {{
    ((*____out++)=(-((float)(sqrtf(((double)(((curSample__106__31) * 3))))))));
    ((*____out++)=(-(((float)(sqrtf(((double)(((curSample__106__31) * 3)))))) + ((float)1.0))));
  }}
  ((curSample__106__31)++);

  if (((curSample__106__31) >= 1024)) {((curSample__106__31) = 0)/*int*/;}
  // mark end: SIRFilter InputGenerate

}}


#endif // BUFFER_MERGE


 
void work_InputGenerate__109_74__31(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter InputGenerate

      __pop__31();

      if ((0 && ((curSample__106__31) == 256))) {{
        __push__31(((float)(sqrtf(((double)(((curSample__106__31) * 3)))))));
        __push__31((((float)(sqrtf(((double)(((curSample__106__31) * 3)))))) + ((float)1.0)));
      } } else {{
        __push__31((-((float)(sqrtf(((double)(((curSample__106__31) * 3))))))));
        __push__31((-(((float)(sqrtf(((double)(((curSample__106__31) * 3)))))) + ((float)1.0))));
      }}
      ((curSample__106__31)++);

      if (((curSample__106__31) >= 1024)) {((curSample__106__31) = 0)/*int*/;}
      // mark end: SIRFilter InputGenerate

    }
  }
}

// peek: 4 pop: 4 push 2
// init counts: 0 steady counts: 3

// ClusterFusion isEliminated: false



int __number_of_iterations_32;
int __counter_32 = 0;
int __steady_32 = 0;
int __tmp_32 = 0;
int __tmp2_32 = 0;
int *__state_flag_32 = NULL;
thread_info *__thread_32 = NULL;



float real_weight__110__32[64] = {0};
float imag_weight__111__32[64] = {0};
float realBuffer__112__32[64] = {0};
float imagBuffer__113__32[64] = {0};
int count__114__32 = 0;
int pos__115__32 = 0;
void save_peek_buffer__32(object_write_buffer *buf);
void load_peek_buffer__32(object_write_buffer *buf);
void save_file_pointer__32(object_write_buffer *buf);
void load_file_pointer__32(object_write_buffer *buf);

 
void init_BeamFirFilter__124_75__32();
inline void check_status__32();

void work_BeamFirFilter__124_75__32(int);


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



 
void init_BeamFirFilter__124_75__32(){
  int idx__122 = 0;/* int */
  int j__123 = 0;/* int */

  ((pos__115__32) = 0)/*int*/;
  for ((j__123 = 0)/*int*/; (j__123 < 64); (j__123++)) {{
      (idx__122 = (j__123 + 1))/*int*/;
      (((real_weight__110__32)[(int)j__123]) = (((float)(sinf(((double)(idx__122))))) / ((float)(idx__122))))/*float*/;
      (((imag_weight__111__32)[(int)j__123]) = (((float)(cosf(((double)(idx__122))))) / ((float)(idx__122))))/*float*/;
    }
  }
}
void save_file_pointer__32(object_write_buffer *buf) {}
void load_file_pointer__32(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__124_75__32__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__118 = 0.0f;/* float */
  float imag_curr__119 = 0.0f;/* float */
  int i__120 = 0;/* int */
  int modPos__121 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__118 = ((float)0.0))/*float*/;
  (imag_curr__119 = ((float)0.0))/*float*/;
  (((realBuffer__112__32)[(int)(63 - (pos__115__32))]) = (*____in++))/*float*/;
  (((imagBuffer__113__32)[(int)(63 - (pos__115__32))]) = (*____in++))/*float*/;
  (modPos__121 = (63 - (pos__115__32)))/*int*/;
  for ((i__120 = 0)/*int*/; (i__120 < 64); (i__120++)) {{
      (real_curr__118 = (real_curr__118 + ((((realBuffer__112__32)[(int)modPos__121]) * ((real_weight__110__32)[(int)i__120])) + (((imagBuffer__113__32)[(int)modPos__121]) * ((imag_weight__111__32)[(int)i__120])))))/*float*/;
      (imag_curr__119 = (imag_curr__119 + ((((imagBuffer__113__32)[(int)modPos__121]) * ((real_weight__110__32)[(int)i__120])) + (((realBuffer__112__32)[(int)modPos__121]) * ((imag_weight__111__32)[(int)i__120])))))/*float*/;
      (modPos__121 = ((modPos__121 + 1) & 63))/*int*/;
    }
  }
  ((pos__115__32) = (((pos__115__32) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__118);
  ((*____out++)=imag_curr__119);
  for ((i__120 = 2)/*int*/; (i__120 < 4); (i__120++)) {{
      (*____in++);
    }
  }
  ((count__114__32) = ((count__114__32) + 2))/*int*/;

  if (((count__114__32) == 1024)) {{
    ((count__114__32) = 0)/*int*/;
    ((pos__115__32) = 0)/*int*/;
    for ((i__120 = 0)/*int*/; (i__120 < 64); (i__120++)) {{
        (((realBuffer__112__32)[(int)i__120]) = ((float)0.0))/*float*/;
        (((imagBuffer__113__32)[(int)i__120]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__124_75__32__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__118 = 0.0f;/* float */
  float imag_curr__119 = 0.0f;/* float */
  int i__120 = 0;/* int */
  int modPos__121 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__118 = ((float)0.0))/*float*/;
  (imag_curr__119 = ((float)0.0))/*float*/;
  (((realBuffer__112__32)[(int)(63 - (pos__115__32))]) = (*____in++))/*float*/;
  (((imagBuffer__113__32)[(int)(63 - (pos__115__32))]) = (*____in++))/*float*/;
  (modPos__121 = (63 - (pos__115__32)))/*int*/;
  for ((i__120 = 0)/*int*/; (i__120 < 64); (i__120++)) {{
      (real_curr__118 = (real_curr__118 + ((((realBuffer__112__32)[(int)modPos__121]) * ((real_weight__110__32)[(int)i__120])) + (((imagBuffer__113__32)[(int)modPos__121]) * ((imag_weight__111__32)[(int)i__120])))))/*float*/;
      (imag_curr__119 = (imag_curr__119 + ((((imagBuffer__113__32)[(int)modPos__121]) * ((real_weight__110__32)[(int)i__120])) + (((realBuffer__112__32)[(int)modPos__121]) * ((imag_weight__111__32)[(int)i__120])))))/*float*/;
      (modPos__121 = ((modPos__121 + 1) & 63))/*int*/;
    }
  }
  ((pos__115__32) = (((pos__115__32) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__118);
  ((*____out++)=imag_curr__119);
  for ((i__120 = 2)/*int*/; (i__120 < 4); (i__120++)) {{
      (*____in++);
    }
  }
  ((count__114__32) = ((count__114__32) + 2))/*int*/;

  if (((count__114__32) == 1024)) {{
    ((count__114__32) = 0)/*int*/;
    ((pos__115__32) = 0)/*int*/;
    for ((i__120 = 0)/*int*/; (i__120 < 64); (i__120++)) {{
        (((realBuffer__112__32)[(int)i__120]) = ((float)0.0))/*float*/;
        (((imagBuffer__113__32)[(int)i__120]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__124_75__32(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__118 = 0.0f;/* float */
      float imag_curr__119 = 0.0f;/* float */
      int i__120 = 0;/* int */
      int modPos__121 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__118 = ((float)0.0))/*float*/;
      (imag_curr__119 = ((float)0.0))/*float*/;
      (((realBuffer__112__32)[(int)(63 - (pos__115__32))]) = __pop__32())/*float*/;
      (((imagBuffer__113__32)[(int)(63 - (pos__115__32))]) = __pop__32())/*float*/;
      (modPos__121 = (63 - (pos__115__32)))/*int*/;
      for ((i__120 = 0)/*int*/; (i__120 < 64); (i__120++)) {{
          (real_curr__118 = (real_curr__118 + ((((realBuffer__112__32)[(int)modPos__121]) * ((real_weight__110__32)[(int)i__120])) + (((imagBuffer__113__32)[(int)modPos__121]) * ((imag_weight__111__32)[(int)i__120])))))/*float*/;
          (imag_curr__119 = (imag_curr__119 + ((((imagBuffer__113__32)[(int)modPos__121]) * ((real_weight__110__32)[(int)i__120])) + (((realBuffer__112__32)[(int)modPos__121]) * ((imag_weight__111__32)[(int)i__120])))))/*float*/;
          (modPos__121 = ((modPos__121 + 1) & 63))/*int*/;
        }
      }
      ((pos__115__32) = (((pos__115__32) + 1) & 63))/*int*/;
      __push__32(real_curr__118);
      __push__32(imag_curr__119);
      for ((i__120 = 2)/*int*/; (i__120 < 4); (i__120++)) {{
          __pop__32();
        }
      }
      ((count__114__32) = ((count__114__32) + 2))/*int*/;

      if (((count__114__32) == 1024)) {{
        ((count__114__32) = 0)/*int*/;
        ((pos__115__32) = 0)/*int*/;
        for ((i__120 = 0)/*int*/; (i__120 < 64); (i__120++)) {{
            (((realBuffer__112__32)[(int)i__120]) = ((float)0.0))/*float*/;
            (((imagBuffer__113__32)[(int)i__120]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

// peek: 6 pop: 6 push 2
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_33;
int __counter_33 = 0;
int __steady_33 = 0;
int __tmp_33 = 0;
int __tmp2_33 = 0;
int *__state_flag_33 = NULL;
thread_info *__thread_33 = NULL;



float real_weight__125__33[64] = {0};
float imag_weight__126__33[64] = {0};
float realBuffer__127__33[64] = {0};
float imagBuffer__128__33[64] = {0};
int count__129__33 = 0;
int pos__130__33 = 0;
void save_peek_buffer__33(object_write_buffer *buf);
void load_peek_buffer__33(object_write_buffer *buf);
void save_file_pointer__33(object_write_buffer *buf);
void load_file_pointer__33(object_write_buffer *buf);

 
void init_BeamFirFilter__139_76__33();
inline void check_status__33();

void work_BeamFirFilter__139_76__33(int);


inline float __pop__33() {
float res=BUFFER_32_33[TAIL_32_33];
TAIL_32_33++;
return res;
}

inline float __pop__33(int n) {
float res=BUFFER_32_33[TAIL_32_33];
TAIL_32_33+=n;

return res;
}

inline float __peek__33(int offs) {
return BUFFER_32_33[TAIL_32_33+offs];
}



inline void __push__33(float data) {
BUFFER_33_5[HEAD_33_5]=data;
HEAD_33_5++;
}



 
void init_BeamFirFilter__139_76__33(){
  int idx__137 = 0;/* int */
  int j__138 = 0;/* int */

  ((pos__130__33) = 0)/*int*/;
  for ((j__138 = 0)/*int*/; (j__138 < 64); (j__138++)) {{
      (idx__137 = (j__138 + 1))/*int*/;
      (((real_weight__125__33)[(int)j__138]) = (((float)(sinf(((double)(idx__137))))) / ((float)(idx__137))))/*float*/;
      (((imag_weight__126__33)[(int)j__138]) = (((float)(cosf(((double)(idx__137))))) / ((float)(idx__137))))/*float*/;
    }
  }
}
void save_file_pointer__33(object_write_buffer *buf) {}
void load_file_pointer__33(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__139_76__33__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__133 = 0.0f;/* float */
  float imag_curr__134 = 0.0f;/* float */
  int i__135 = 0;/* int */
  int modPos__136 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__133 = ((float)0.0))/*float*/;
  (imag_curr__134 = ((float)0.0))/*float*/;
  (((realBuffer__127__33)[(int)(63 - (pos__130__33))]) = (*____in++))/*float*/;
  (((imagBuffer__128__33)[(int)(63 - (pos__130__33))]) = (*____in++))/*float*/;
  (modPos__136 = (63 - (pos__130__33)))/*int*/;
  for ((i__135 = 0)/*int*/; (i__135 < 64); (i__135++)) {{
      (real_curr__133 = (real_curr__133 + ((((realBuffer__127__33)[(int)modPos__136]) * ((real_weight__125__33)[(int)i__135])) + (((imagBuffer__128__33)[(int)modPos__136]) * ((imag_weight__126__33)[(int)i__135])))))/*float*/;
      (imag_curr__134 = (imag_curr__134 + ((((imagBuffer__128__33)[(int)modPos__136]) * ((real_weight__125__33)[(int)i__135])) + (((realBuffer__127__33)[(int)modPos__136]) * ((imag_weight__126__33)[(int)i__135])))))/*float*/;
      (modPos__136 = ((modPos__136 + 1) & 63))/*int*/;
    }
  }
  ((pos__130__33) = (((pos__130__33) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__133);
  ((*____out++)=imag_curr__134);
  for ((i__135 = 2)/*int*/; (i__135 < 6); (i__135++)) {{
      (*____in++);
    }
  }
  ((count__129__33) = ((count__129__33) + 3))/*int*/;

  if (((count__129__33) == 512)) {{
    ((count__129__33) = 0)/*int*/;
    ((pos__130__33) = 0)/*int*/;
    for ((i__135 = 0)/*int*/; (i__135 < 64); (i__135++)) {{
        (((realBuffer__127__33)[(int)i__135]) = ((float)0.0))/*float*/;
        (((imagBuffer__128__33)[(int)i__135]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__139_76__33__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__133 = 0.0f;/* float */
  float imag_curr__134 = 0.0f;/* float */
  int i__135 = 0;/* int */
  int modPos__136 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__133 = ((float)0.0))/*float*/;
  (imag_curr__134 = ((float)0.0))/*float*/;
  (((realBuffer__127__33)[(int)(63 - (pos__130__33))]) = (*____in++))/*float*/;
  (((imagBuffer__128__33)[(int)(63 - (pos__130__33))]) = (*____in++))/*float*/;
  (modPos__136 = (63 - (pos__130__33)))/*int*/;
  for ((i__135 = 0)/*int*/; (i__135 < 64); (i__135++)) {{
      (real_curr__133 = (real_curr__133 + ((((realBuffer__127__33)[(int)modPos__136]) * ((real_weight__125__33)[(int)i__135])) + (((imagBuffer__128__33)[(int)modPos__136]) * ((imag_weight__126__33)[(int)i__135])))))/*float*/;
      (imag_curr__134 = (imag_curr__134 + ((((imagBuffer__128__33)[(int)modPos__136]) * ((real_weight__125__33)[(int)i__135])) + (((realBuffer__127__33)[(int)modPos__136]) * ((imag_weight__126__33)[(int)i__135])))))/*float*/;
      (modPos__136 = ((modPos__136 + 1) & 63))/*int*/;
    }
  }
  ((pos__130__33) = (((pos__130__33) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__133);
  ((*____out++)=imag_curr__134);
  for ((i__135 = 2)/*int*/; (i__135 < 6); (i__135++)) {{
      (*____in++);
    }
  }
  ((count__129__33) = ((count__129__33) + 3))/*int*/;

  if (((count__129__33) == 512)) {{
    ((count__129__33) = 0)/*int*/;
    ((pos__130__33) = 0)/*int*/;
    for ((i__135 = 0)/*int*/; (i__135 < 64); (i__135++)) {{
        (((realBuffer__127__33)[(int)i__135]) = ((float)0.0))/*float*/;
        (((imagBuffer__128__33)[(int)i__135]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__139_76__33(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__133 = 0.0f;/* float */
      float imag_curr__134 = 0.0f;/* float */
      int i__135 = 0;/* int */
      int modPos__136 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__133 = ((float)0.0))/*float*/;
      (imag_curr__134 = ((float)0.0))/*float*/;
      (((realBuffer__127__33)[(int)(63 - (pos__130__33))]) = __pop__33())/*float*/;
      (((imagBuffer__128__33)[(int)(63 - (pos__130__33))]) = __pop__33())/*float*/;
      (modPos__136 = (63 - (pos__130__33)))/*int*/;
      for ((i__135 = 0)/*int*/; (i__135 < 64); (i__135++)) {{
          (real_curr__133 = (real_curr__133 + ((((realBuffer__127__33)[(int)modPos__136]) * ((real_weight__125__33)[(int)i__135])) + (((imagBuffer__128__33)[(int)modPos__136]) * ((imag_weight__126__33)[(int)i__135])))))/*float*/;
          (imag_curr__134 = (imag_curr__134 + ((((imagBuffer__128__33)[(int)modPos__136]) * ((real_weight__125__33)[(int)i__135])) + (((realBuffer__127__33)[(int)modPos__136]) * ((imag_weight__126__33)[(int)i__135])))))/*float*/;
          (modPos__136 = ((modPos__136 + 1) & 63))/*int*/;
        }
      }
      ((pos__130__33) = (((pos__130__33) + 1) & 63))/*int*/;
      __push__33(real_curr__133);
      __push__33(imag_curr__134);
      for ((i__135 = 2)/*int*/; (i__135 < 6); (i__135++)) {{
          __pop__33();
        }
      }
      ((count__129__33) = ((count__129__33) + 3))/*int*/;

      if (((count__129__33) == 512)) {{
        ((count__129__33) = 0)/*int*/;
        ((pos__130__33) = 0)/*int*/;
        for ((i__135 = 0)/*int*/; (i__135 < 64); (i__135++)) {{
            (((realBuffer__127__33)[(int)i__135]) = ((float)0.0))/*float*/;
            (((imagBuffer__128__33)[(int)i__135]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

// peek: 1 pop: 1 push 2
// init counts: 0 steady counts: 6

// ClusterFusion isEliminated: false



int __number_of_iterations_34;
int __counter_34 = 0;
int __steady_34 = 0;
int __tmp_34 = 0;
int __tmp2_34 = 0;
int *__state_flag_34 = NULL;
thread_info *__thread_34 = NULL;



int curSample__140__34 = 0;
void save_peek_buffer__34(object_write_buffer *buf);
void load_peek_buffer__34(object_write_buffer *buf);
void save_file_pointer__34(object_write_buffer *buf);
void load_file_pointer__34(object_write_buffer *buf);

 
void init_InputGenerate__143_78__34();
inline void check_status__34();

void work_InputGenerate__143_78__34(int);


inline int __pop__34() {
int res=BUFFER_1_34[TAIL_1_34];
TAIL_1_34++;
return res;
}

inline int __pop__34(int n) {
int res=BUFFER_1_34[TAIL_1_34];
TAIL_1_34+=n;

return res;
}

inline int __peek__34(int offs) {
return BUFFER_1_34[TAIL_1_34+offs];
}



inline void __push__34(float data) {
BUFFER_34_35[HEAD_34_35]=data;
HEAD_34_35++;
}



 
void init_InputGenerate__143_78__34(){
  ((curSample__140__34) = 0)/*int*/;
}
void save_file_pointer__34(object_write_buffer *buf) {}
void load_file_pointer__34(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_InputGenerate__143_78__34__mod(int ____n, int *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRFilter InputGenerate

  (*____in++);

  if ((0 && ((curSample__140__34) == 256))) {{
    ((*____out++)=((float)(sqrtf(((double)(((curSample__140__34) * 4)))))));
    ((*____out++)=(((float)(sqrtf(((double)(((curSample__140__34) * 4)))))) + ((float)1.0)));
  } } else {{
    ((*____out++)=(-((float)(sqrtf(((double)(((curSample__140__34) * 4))))))));
    ((*____out++)=(-(((float)(sqrtf(((double)(((curSample__140__34) * 4)))))) + ((float)1.0))));
  }}
  ((curSample__140__34)++);

  if (((curSample__140__34) >= 1024)) {((curSample__140__34) = 0)/*int*/;}
  // mark end: SIRFilter InputGenerate

}}


void work_InputGenerate__143_78__34__mod2(int ____n, int *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRFilter InputGenerate

  (*____in++);

  if ((0 && ((curSample__140__34) == 256))) {{
    ((*____out++)=((float)(sqrtf(((double)(((curSample__140__34) * 4)))))));
    ((*____out++)=(((float)(sqrtf(((double)(((curSample__140__34) * 4)))))) + ((float)1.0)));
  } } else {{
    ((*____out++)=(-((float)(sqrtf(((double)(((curSample__140__34) * 4))))))));
    ((*____out++)=(-(((float)(sqrtf(((double)(((curSample__140__34) * 4)))))) + ((float)1.0))));
  }}
  ((curSample__140__34)++);

  if (((curSample__140__34) >= 1024)) {((curSample__140__34) = 0)/*int*/;}
  // mark end: SIRFilter InputGenerate

}}


#endif // BUFFER_MERGE


 
void work_InputGenerate__143_78__34(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter InputGenerate

      __pop__34();

      if ((0 && ((curSample__140__34) == 256))) {{
        __push__34(((float)(sqrtf(((double)(((curSample__140__34) * 4)))))));
        __push__34((((float)(sqrtf(((double)(((curSample__140__34) * 4)))))) + ((float)1.0)));
      } } else {{
        __push__34((-((float)(sqrtf(((double)(((curSample__140__34) * 4))))))));
        __push__34((-(((float)(sqrtf(((double)(((curSample__140__34) * 4)))))) + ((float)1.0))));
      }}
      ((curSample__140__34)++);

      if (((curSample__140__34) >= 1024)) {((curSample__140__34) = 0)/*int*/;}
      // mark end: SIRFilter InputGenerate

    }
  }
}

// peek: 4 pop: 4 push 2
// init counts: 0 steady counts: 3

// ClusterFusion isEliminated: false



int __number_of_iterations_35;
int __counter_35 = 0;
int __steady_35 = 0;
int __tmp_35 = 0;
int __tmp2_35 = 0;
int *__state_flag_35 = NULL;
thread_info *__thread_35 = NULL;



float real_weight__144__35[64] = {0};
float imag_weight__145__35[64] = {0};
float realBuffer__146__35[64] = {0};
float imagBuffer__147__35[64] = {0};
int count__148__35 = 0;
int pos__149__35 = 0;
void save_peek_buffer__35(object_write_buffer *buf);
void load_peek_buffer__35(object_write_buffer *buf);
void save_file_pointer__35(object_write_buffer *buf);
void load_file_pointer__35(object_write_buffer *buf);

 
void init_BeamFirFilter__158_79__35();
inline void check_status__35();

void work_BeamFirFilter__158_79__35(int);


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



inline void __push__35(float data) {
BUFFER_35_36[HEAD_35_36]=data;
HEAD_35_36++;
}



 
void init_BeamFirFilter__158_79__35(){
  int idx__156 = 0;/* int */
  int j__157 = 0;/* int */

  ((pos__149__35) = 0)/*int*/;
  for ((j__157 = 0)/*int*/; (j__157 < 64); (j__157++)) {{
      (idx__156 = (j__157 + 1))/*int*/;
      (((real_weight__144__35)[(int)j__157]) = (((float)(sinf(((double)(idx__156))))) / ((float)(idx__156))))/*float*/;
      (((imag_weight__145__35)[(int)j__157]) = (((float)(cosf(((double)(idx__156))))) / ((float)(idx__156))))/*float*/;
    }
  }
}
void save_file_pointer__35(object_write_buffer *buf) {}
void load_file_pointer__35(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__158_79__35__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__152 = 0.0f;/* float */
  float imag_curr__153 = 0.0f;/* float */
  int i__154 = 0;/* int */
  int modPos__155 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__152 = ((float)0.0))/*float*/;
  (imag_curr__153 = ((float)0.0))/*float*/;
  (((realBuffer__146__35)[(int)(63 - (pos__149__35))]) = (*____in++))/*float*/;
  (((imagBuffer__147__35)[(int)(63 - (pos__149__35))]) = (*____in++))/*float*/;
  (modPos__155 = (63 - (pos__149__35)))/*int*/;
  for ((i__154 = 0)/*int*/; (i__154 < 64); (i__154++)) {{
      (real_curr__152 = (real_curr__152 + ((((realBuffer__146__35)[(int)modPos__155]) * ((real_weight__144__35)[(int)i__154])) + (((imagBuffer__147__35)[(int)modPos__155]) * ((imag_weight__145__35)[(int)i__154])))))/*float*/;
      (imag_curr__153 = (imag_curr__153 + ((((imagBuffer__147__35)[(int)modPos__155]) * ((real_weight__144__35)[(int)i__154])) + (((realBuffer__146__35)[(int)modPos__155]) * ((imag_weight__145__35)[(int)i__154])))))/*float*/;
      (modPos__155 = ((modPos__155 + 1) & 63))/*int*/;
    }
  }
  ((pos__149__35) = (((pos__149__35) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__152);
  ((*____out++)=imag_curr__153);
  for ((i__154 = 2)/*int*/; (i__154 < 4); (i__154++)) {{
      (*____in++);
    }
  }
  ((count__148__35) = ((count__148__35) + 2))/*int*/;

  if (((count__148__35) == 1024)) {{
    ((count__148__35) = 0)/*int*/;
    ((pos__149__35) = 0)/*int*/;
    for ((i__154 = 0)/*int*/; (i__154 < 64); (i__154++)) {{
        (((realBuffer__146__35)[(int)i__154]) = ((float)0.0))/*float*/;
        (((imagBuffer__147__35)[(int)i__154]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__158_79__35__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__152 = 0.0f;/* float */
  float imag_curr__153 = 0.0f;/* float */
  int i__154 = 0;/* int */
  int modPos__155 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__152 = ((float)0.0))/*float*/;
  (imag_curr__153 = ((float)0.0))/*float*/;
  (((realBuffer__146__35)[(int)(63 - (pos__149__35))]) = (*____in++))/*float*/;
  (((imagBuffer__147__35)[(int)(63 - (pos__149__35))]) = (*____in++))/*float*/;
  (modPos__155 = (63 - (pos__149__35)))/*int*/;
  for ((i__154 = 0)/*int*/; (i__154 < 64); (i__154++)) {{
      (real_curr__152 = (real_curr__152 + ((((realBuffer__146__35)[(int)modPos__155]) * ((real_weight__144__35)[(int)i__154])) + (((imagBuffer__147__35)[(int)modPos__155]) * ((imag_weight__145__35)[(int)i__154])))))/*float*/;
      (imag_curr__153 = (imag_curr__153 + ((((imagBuffer__147__35)[(int)modPos__155]) * ((real_weight__144__35)[(int)i__154])) + (((realBuffer__146__35)[(int)modPos__155]) * ((imag_weight__145__35)[(int)i__154])))))/*float*/;
      (modPos__155 = ((modPos__155 + 1) & 63))/*int*/;
    }
  }
  ((pos__149__35) = (((pos__149__35) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__152);
  ((*____out++)=imag_curr__153);
  for ((i__154 = 2)/*int*/; (i__154 < 4); (i__154++)) {{
      (*____in++);
    }
  }
  ((count__148__35) = ((count__148__35) + 2))/*int*/;

  if (((count__148__35) == 1024)) {{
    ((count__148__35) = 0)/*int*/;
    ((pos__149__35) = 0)/*int*/;
    for ((i__154 = 0)/*int*/; (i__154 < 64); (i__154++)) {{
        (((realBuffer__146__35)[(int)i__154]) = ((float)0.0))/*float*/;
        (((imagBuffer__147__35)[(int)i__154]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__158_79__35(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__152 = 0.0f;/* float */
      float imag_curr__153 = 0.0f;/* float */
      int i__154 = 0;/* int */
      int modPos__155 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__152 = ((float)0.0))/*float*/;
      (imag_curr__153 = ((float)0.0))/*float*/;
      (((realBuffer__146__35)[(int)(63 - (pos__149__35))]) = __pop__35())/*float*/;
      (((imagBuffer__147__35)[(int)(63 - (pos__149__35))]) = __pop__35())/*float*/;
      (modPos__155 = (63 - (pos__149__35)))/*int*/;
      for ((i__154 = 0)/*int*/; (i__154 < 64); (i__154++)) {{
          (real_curr__152 = (real_curr__152 + ((((realBuffer__146__35)[(int)modPos__155]) * ((real_weight__144__35)[(int)i__154])) + (((imagBuffer__147__35)[(int)modPos__155]) * ((imag_weight__145__35)[(int)i__154])))))/*float*/;
          (imag_curr__153 = (imag_curr__153 + ((((imagBuffer__147__35)[(int)modPos__155]) * ((real_weight__144__35)[(int)i__154])) + (((realBuffer__146__35)[(int)modPos__155]) * ((imag_weight__145__35)[(int)i__154])))))/*float*/;
          (modPos__155 = ((modPos__155 + 1) & 63))/*int*/;
        }
      }
      ((pos__149__35) = (((pos__149__35) + 1) & 63))/*int*/;
      __push__35(real_curr__152);
      __push__35(imag_curr__153);
      for ((i__154 = 2)/*int*/; (i__154 < 4); (i__154++)) {{
          __pop__35();
        }
      }
      ((count__148__35) = ((count__148__35) + 2))/*int*/;

      if (((count__148__35) == 1024)) {{
        ((count__148__35) = 0)/*int*/;
        ((pos__149__35) = 0)/*int*/;
        for ((i__154 = 0)/*int*/; (i__154 < 64); (i__154++)) {{
            (((realBuffer__146__35)[(int)i__154]) = ((float)0.0))/*float*/;
            (((imagBuffer__147__35)[(int)i__154]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

// peek: 6 pop: 6 push 2
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_36;
int __counter_36 = 0;
int __steady_36 = 0;
int __tmp_36 = 0;
int __tmp2_36 = 0;
int *__state_flag_36 = NULL;
thread_info *__thread_36 = NULL;



float real_weight__159__36[64] = {0};
float imag_weight__160__36[64] = {0};
float realBuffer__161__36[64] = {0};
float imagBuffer__162__36[64] = {0};
int count__163__36 = 0;
int pos__164__36 = 0;
void save_peek_buffer__36(object_write_buffer *buf);
void load_peek_buffer__36(object_write_buffer *buf);
void save_file_pointer__36(object_write_buffer *buf);
void load_file_pointer__36(object_write_buffer *buf);

 
void init_BeamFirFilter__173_80__36();
inline void check_status__36();

void work_BeamFirFilter__173_80__36(int);


inline float __pop__36() {
float res=BUFFER_35_36[TAIL_35_36];
TAIL_35_36++;
return res;
}

inline float __pop__36(int n) {
float res=BUFFER_35_36[TAIL_35_36];
TAIL_35_36+=n;

return res;
}

inline float __peek__36(int offs) {
return BUFFER_35_36[TAIL_35_36+offs];
}



inline void __push__36(float data) {
BUFFER_36_5[HEAD_36_5]=data;
HEAD_36_5++;
}



 
void init_BeamFirFilter__173_80__36(){
  int idx__171 = 0;/* int */
  int j__172 = 0;/* int */

  ((pos__164__36) = 0)/*int*/;
  for ((j__172 = 0)/*int*/; (j__172 < 64); (j__172++)) {{
      (idx__171 = (j__172 + 1))/*int*/;
      (((real_weight__159__36)[(int)j__172]) = (((float)(sinf(((double)(idx__171))))) / ((float)(idx__171))))/*float*/;
      (((imag_weight__160__36)[(int)j__172]) = (((float)(cosf(((double)(idx__171))))) / ((float)(idx__171))))/*float*/;
    }
  }
}
void save_file_pointer__36(object_write_buffer *buf) {}
void load_file_pointer__36(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__173_80__36__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__167 = 0.0f;/* float */
  float imag_curr__168 = 0.0f;/* float */
  int i__169 = 0;/* int */
  int modPos__170 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__167 = ((float)0.0))/*float*/;
  (imag_curr__168 = ((float)0.0))/*float*/;
  (((realBuffer__161__36)[(int)(63 - (pos__164__36))]) = (*____in++))/*float*/;
  (((imagBuffer__162__36)[(int)(63 - (pos__164__36))]) = (*____in++))/*float*/;
  (modPos__170 = (63 - (pos__164__36)))/*int*/;
  for ((i__169 = 0)/*int*/; (i__169 < 64); (i__169++)) {{
      (real_curr__167 = (real_curr__167 + ((((realBuffer__161__36)[(int)modPos__170]) * ((real_weight__159__36)[(int)i__169])) + (((imagBuffer__162__36)[(int)modPos__170]) * ((imag_weight__160__36)[(int)i__169])))))/*float*/;
      (imag_curr__168 = (imag_curr__168 + ((((imagBuffer__162__36)[(int)modPos__170]) * ((real_weight__159__36)[(int)i__169])) + (((realBuffer__161__36)[(int)modPos__170]) * ((imag_weight__160__36)[(int)i__169])))))/*float*/;
      (modPos__170 = ((modPos__170 + 1) & 63))/*int*/;
    }
  }
  ((pos__164__36) = (((pos__164__36) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__167);
  ((*____out++)=imag_curr__168);
  for ((i__169 = 2)/*int*/; (i__169 < 6); (i__169++)) {{
      (*____in++);
    }
  }
  ((count__163__36) = ((count__163__36) + 3))/*int*/;

  if (((count__163__36) == 512)) {{
    ((count__163__36) = 0)/*int*/;
    ((pos__164__36) = 0)/*int*/;
    for ((i__169 = 0)/*int*/; (i__169 < 64); (i__169++)) {{
        (((realBuffer__161__36)[(int)i__169]) = ((float)0.0))/*float*/;
        (((imagBuffer__162__36)[(int)i__169]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__173_80__36__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__167 = 0.0f;/* float */
  float imag_curr__168 = 0.0f;/* float */
  int i__169 = 0;/* int */
  int modPos__170 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__167 = ((float)0.0))/*float*/;
  (imag_curr__168 = ((float)0.0))/*float*/;
  (((realBuffer__161__36)[(int)(63 - (pos__164__36))]) = (*____in++))/*float*/;
  (((imagBuffer__162__36)[(int)(63 - (pos__164__36))]) = (*____in++))/*float*/;
  (modPos__170 = (63 - (pos__164__36)))/*int*/;
  for ((i__169 = 0)/*int*/; (i__169 < 64); (i__169++)) {{
      (real_curr__167 = (real_curr__167 + ((((realBuffer__161__36)[(int)modPos__170]) * ((real_weight__159__36)[(int)i__169])) + (((imagBuffer__162__36)[(int)modPos__170]) * ((imag_weight__160__36)[(int)i__169])))))/*float*/;
      (imag_curr__168 = (imag_curr__168 + ((((imagBuffer__162__36)[(int)modPos__170]) * ((real_weight__159__36)[(int)i__169])) + (((realBuffer__161__36)[(int)modPos__170]) * ((imag_weight__160__36)[(int)i__169])))))/*float*/;
      (modPos__170 = ((modPos__170 + 1) & 63))/*int*/;
    }
  }
  ((pos__164__36) = (((pos__164__36) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__167);
  ((*____out++)=imag_curr__168);
  for ((i__169 = 2)/*int*/; (i__169 < 6); (i__169++)) {{
      (*____in++);
    }
  }
  ((count__163__36) = ((count__163__36) + 3))/*int*/;

  if (((count__163__36) == 512)) {{
    ((count__163__36) = 0)/*int*/;
    ((pos__164__36) = 0)/*int*/;
    for ((i__169 = 0)/*int*/; (i__169 < 64); (i__169++)) {{
        (((realBuffer__161__36)[(int)i__169]) = ((float)0.0))/*float*/;
        (((imagBuffer__162__36)[(int)i__169]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__173_80__36(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__167 = 0.0f;/* float */
      float imag_curr__168 = 0.0f;/* float */
      int i__169 = 0;/* int */
      int modPos__170 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__167 = ((float)0.0))/*float*/;
      (imag_curr__168 = ((float)0.0))/*float*/;
      (((realBuffer__161__36)[(int)(63 - (pos__164__36))]) = __pop__36())/*float*/;
      (((imagBuffer__162__36)[(int)(63 - (pos__164__36))]) = __pop__36())/*float*/;
      (modPos__170 = (63 - (pos__164__36)))/*int*/;
      for ((i__169 = 0)/*int*/; (i__169 < 64); (i__169++)) {{
          (real_curr__167 = (real_curr__167 + ((((realBuffer__161__36)[(int)modPos__170]) * ((real_weight__159__36)[(int)i__169])) + (((imagBuffer__162__36)[(int)modPos__170]) * ((imag_weight__160__36)[(int)i__169])))))/*float*/;
          (imag_curr__168 = (imag_curr__168 + ((((imagBuffer__162__36)[(int)modPos__170]) * ((real_weight__159__36)[(int)i__169])) + (((realBuffer__161__36)[(int)modPos__170]) * ((imag_weight__160__36)[(int)i__169])))))/*float*/;
          (modPos__170 = ((modPos__170 + 1) & 63))/*int*/;
        }
      }
      ((pos__164__36) = (((pos__164__36) + 1) & 63))/*int*/;
      __push__36(real_curr__167);
      __push__36(imag_curr__168);
      for ((i__169 = 2)/*int*/; (i__169 < 6); (i__169++)) {{
          __pop__36();
        }
      }
      ((count__163__36) = ((count__163__36) + 3))/*int*/;

      if (((count__163__36) == 512)) {{
        ((count__163__36) = 0)/*int*/;
        ((pos__164__36) = 0)/*int*/;
        for ((i__169 = 0)/*int*/; (i__169 < 64); (i__169++)) {{
            (((realBuffer__161__36)[(int)i__169]) = ((float)0.0))/*float*/;
            (((imagBuffer__162__36)[(int)i__169]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

// peek: 1 pop: 1 push 2
// init counts: 0 steady counts: 6

// ClusterFusion isEliminated: false



int __number_of_iterations_37;
int __counter_37 = 0;
int __steady_37 = 0;
int __tmp_37 = 0;
int __tmp2_37 = 0;
int *__state_flag_37 = NULL;
thread_info *__thread_37 = NULL;



int curSample__174__37 = 0;
void save_peek_buffer__37(object_write_buffer *buf);
void load_peek_buffer__37(object_write_buffer *buf);
void save_file_pointer__37(object_write_buffer *buf);
void load_file_pointer__37(object_write_buffer *buf);

 
void init_InputGenerate__177_82__37();
inline void check_status__37();

void work_InputGenerate__177_82__37(int);


inline int __pop__37() {
int res=BUFFER_1_37[TAIL_1_37];
TAIL_1_37++;
return res;
}

inline int __pop__37(int n) {
int res=BUFFER_1_37[TAIL_1_37];
TAIL_1_37+=n;

return res;
}

inline int __peek__37(int offs) {
return BUFFER_1_37[TAIL_1_37+offs];
}



inline void __push__37(float data) {
BUFFER_37_38[HEAD_37_38]=data;
HEAD_37_38++;
}



 
void init_InputGenerate__177_82__37(){
  ((curSample__174__37) = 0)/*int*/;
}
void save_file_pointer__37(object_write_buffer *buf) {}
void load_file_pointer__37(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_InputGenerate__177_82__37__mod(int ____n, int *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRFilter InputGenerate

  (*____in++);

  if ((0 && ((curSample__174__37) == 256))) {{
    ((*____out++)=((float)(sqrtf(((double)(((curSample__174__37) * 5)))))));
    ((*____out++)=(((float)(sqrtf(((double)(((curSample__174__37) * 5)))))) + ((float)1.0)));
  } } else {{
    ((*____out++)=(-((float)(sqrtf(((double)(((curSample__174__37) * 5))))))));
    ((*____out++)=(-(((float)(sqrtf(((double)(((curSample__174__37) * 5)))))) + ((float)1.0))));
  }}
  ((curSample__174__37)++);

  if (((curSample__174__37) >= 1024)) {((curSample__174__37) = 0)/*int*/;}
  // mark end: SIRFilter InputGenerate

}}


void work_InputGenerate__177_82__37__mod2(int ____n, int *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRFilter InputGenerate

  (*____in++);

  if ((0 && ((curSample__174__37) == 256))) {{
    ((*____out++)=((float)(sqrtf(((double)(((curSample__174__37) * 5)))))));
    ((*____out++)=(((float)(sqrtf(((double)(((curSample__174__37) * 5)))))) + ((float)1.0)));
  } } else {{
    ((*____out++)=(-((float)(sqrtf(((double)(((curSample__174__37) * 5))))))));
    ((*____out++)=(-(((float)(sqrtf(((double)(((curSample__174__37) * 5)))))) + ((float)1.0))));
  }}
  ((curSample__174__37)++);

  if (((curSample__174__37) >= 1024)) {((curSample__174__37) = 0)/*int*/;}
  // mark end: SIRFilter InputGenerate

}}


#endif // BUFFER_MERGE


 
void work_InputGenerate__177_82__37(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter InputGenerate

      __pop__37();

      if ((0 && ((curSample__174__37) == 256))) {{
        __push__37(((float)(sqrtf(((double)(((curSample__174__37) * 5)))))));
        __push__37((((float)(sqrtf(((double)(((curSample__174__37) * 5)))))) + ((float)1.0)));
      } } else {{
        __push__37((-((float)(sqrtf(((double)(((curSample__174__37) * 5))))))));
        __push__37((-(((float)(sqrtf(((double)(((curSample__174__37) * 5)))))) + ((float)1.0))));
      }}
      ((curSample__174__37)++);

      if (((curSample__174__37) >= 1024)) {((curSample__174__37) = 0)/*int*/;}
      // mark end: SIRFilter InputGenerate

    }
  }
}

// peek: 4 pop: 4 push 2
// init counts: 0 steady counts: 3

// ClusterFusion isEliminated: false



int __number_of_iterations_38;
int __counter_38 = 0;
int __steady_38 = 0;
int __tmp_38 = 0;
int __tmp2_38 = 0;
int *__state_flag_38 = NULL;
thread_info *__thread_38 = NULL;



float real_weight__178__38[64] = {0};
float imag_weight__179__38[64] = {0};
float realBuffer__180__38[64] = {0};
float imagBuffer__181__38[64] = {0};
int count__182__38 = 0;
int pos__183__38 = 0;
void save_peek_buffer__38(object_write_buffer *buf);
void load_peek_buffer__38(object_write_buffer *buf);
void save_file_pointer__38(object_write_buffer *buf);
void load_file_pointer__38(object_write_buffer *buf);

 
void init_BeamFirFilter__192_83__38();
inline void check_status__38();

void work_BeamFirFilter__192_83__38(int);


inline float __pop__38() {
float res=BUFFER_37_38[TAIL_37_38];
TAIL_37_38++;
return res;
}

inline float __pop__38(int n) {
float res=BUFFER_37_38[TAIL_37_38];
TAIL_37_38+=n;

return res;
}

inline float __peek__38(int offs) {
return BUFFER_37_38[TAIL_37_38+offs];
}



inline void __push__38(float data) {
BUFFER_38_39[HEAD_38_39]=data;
HEAD_38_39++;
}



 
void init_BeamFirFilter__192_83__38(){
  int idx__190 = 0;/* int */
  int j__191 = 0;/* int */

  ((pos__183__38) = 0)/*int*/;
  for ((j__191 = 0)/*int*/; (j__191 < 64); (j__191++)) {{
      (idx__190 = (j__191 + 1))/*int*/;
      (((real_weight__178__38)[(int)j__191]) = (((float)(sinf(((double)(idx__190))))) / ((float)(idx__190))))/*float*/;
      (((imag_weight__179__38)[(int)j__191]) = (((float)(cosf(((double)(idx__190))))) / ((float)(idx__190))))/*float*/;
    }
  }
}
void save_file_pointer__38(object_write_buffer *buf) {}
void load_file_pointer__38(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__192_83__38__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__186 = 0.0f;/* float */
  float imag_curr__187 = 0.0f;/* float */
  int i__188 = 0;/* int */
  int modPos__189 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__186 = ((float)0.0))/*float*/;
  (imag_curr__187 = ((float)0.0))/*float*/;
  (((realBuffer__180__38)[(int)(63 - (pos__183__38))]) = (*____in++))/*float*/;
  (((imagBuffer__181__38)[(int)(63 - (pos__183__38))]) = (*____in++))/*float*/;
  (modPos__189 = (63 - (pos__183__38)))/*int*/;
  for ((i__188 = 0)/*int*/; (i__188 < 64); (i__188++)) {{
      (real_curr__186 = (real_curr__186 + ((((realBuffer__180__38)[(int)modPos__189]) * ((real_weight__178__38)[(int)i__188])) + (((imagBuffer__181__38)[(int)modPos__189]) * ((imag_weight__179__38)[(int)i__188])))))/*float*/;
      (imag_curr__187 = (imag_curr__187 + ((((imagBuffer__181__38)[(int)modPos__189]) * ((real_weight__178__38)[(int)i__188])) + (((realBuffer__180__38)[(int)modPos__189]) * ((imag_weight__179__38)[(int)i__188])))))/*float*/;
      (modPos__189 = ((modPos__189 + 1) & 63))/*int*/;
    }
  }
  ((pos__183__38) = (((pos__183__38) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__186);
  ((*____out++)=imag_curr__187);
  for ((i__188 = 2)/*int*/; (i__188 < 4); (i__188++)) {{
      (*____in++);
    }
  }
  ((count__182__38) = ((count__182__38) + 2))/*int*/;

  if (((count__182__38) == 1024)) {{
    ((count__182__38) = 0)/*int*/;
    ((pos__183__38) = 0)/*int*/;
    for ((i__188 = 0)/*int*/; (i__188 < 64); (i__188++)) {{
        (((realBuffer__180__38)[(int)i__188]) = ((float)0.0))/*float*/;
        (((imagBuffer__181__38)[(int)i__188]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__192_83__38__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__186 = 0.0f;/* float */
  float imag_curr__187 = 0.0f;/* float */
  int i__188 = 0;/* int */
  int modPos__189 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__186 = ((float)0.0))/*float*/;
  (imag_curr__187 = ((float)0.0))/*float*/;
  (((realBuffer__180__38)[(int)(63 - (pos__183__38))]) = (*____in++))/*float*/;
  (((imagBuffer__181__38)[(int)(63 - (pos__183__38))]) = (*____in++))/*float*/;
  (modPos__189 = (63 - (pos__183__38)))/*int*/;
  for ((i__188 = 0)/*int*/; (i__188 < 64); (i__188++)) {{
      (real_curr__186 = (real_curr__186 + ((((realBuffer__180__38)[(int)modPos__189]) * ((real_weight__178__38)[(int)i__188])) + (((imagBuffer__181__38)[(int)modPos__189]) * ((imag_weight__179__38)[(int)i__188])))))/*float*/;
      (imag_curr__187 = (imag_curr__187 + ((((imagBuffer__181__38)[(int)modPos__189]) * ((real_weight__178__38)[(int)i__188])) + (((realBuffer__180__38)[(int)modPos__189]) * ((imag_weight__179__38)[(int)i__188])))))/*float*/;
      (modPos__189 = ((modPos__189 + 1) & 63))/*int*/;
    }
  }
  ((pos__183__38) = (((pos__183__38) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__186);
  ((*____out++)=imag_curr__187);
  for ((i__188 = 2)/*int*/; (i__188 < 4); (i__188++)) {{
      (*____in++);
    }
  }
  ((count__182__38) = ((count__182__38) + 2))/*int*/;

  if (((count__182__38) == 1024)) {{
    ((count__182__38) = 0)/*int*/;
    ((pos__183__38) = 0)/*int*/;
    for ((i__188 = 0)/*int*/; (i__188 < 64); (i__188++)) {{
        (((realBuffer__180__38)[(int)i__188]) = ((float)0.0))/*float*/;
        (((imagBuffer__181__38)[(int)i__188]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__192_83__38(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__186 = 0.0f;/* float */
      float imag_curr__187 = 0.0f;/* float */
      int i__188 = 0;/* int */
      int modPos__189 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__186 = ((float)0.0))/*float*/;
      (imag_curr__187 = ((float)0.0))/*float*/;
      (((realBuffer__180__38)[(int)(63 - (pos__183__38))]) = __pop__38())/*float*/;
      (((imagBuffer__181__38)[(int)(63 - (pos__183__38))]) = __pop__38())/*float*/;
      (modPos__189 = (63 - (pos__183__38)))/*int*/;
      for ((i__188 = 0)/*int*/; (i__188 < 64); (i__188++)) {{
          (real_curr__186 = (real_curr__186 + ((((realBuffer__180__38)[(int)modPos__189]) * ((real_weight__178__38)[(int)i__188])) + (((imagBuffer__181__38)[(int)modPos__189]) * ((imag_weight__179__38)[(int)i__188])))))/*float*/;
          (imag_curr__187 = (imag_curr__187 + ((((imagBuffer__181__38)[(int)modPos__189]) * ((real_weight__178__38)[(int)i__188])) + (((realBuffer__180__38)[(int)modPos__189]) * ((imag_weight__179__38)[(int)i__188])))))/*float*/;
          (modPos__189 = ((modPos__189 + 1) & 63))/*int*/;
        }
      }
      ((pos__183__38) = (((pos__183__38) + 1) & 63))/*int*/;
      __push__38(real_curr__186);
      __push__38(imag_curr__187);
      for ((i__188 = 2)/*int*/; (i__188 < 4); (i__188++)) {{
          __pop__38();
        }
      }
      ((count__182__38) = ((count__182__38) + 2))/*int*/;

      if (((count__182__38) == 1024)) {{
        ((count__182__38) = 0)/*int*/;
        ((pos__183__38) = 0)/*int*/;
        for ((i__188 = 0)/*int*/; (i__188 < 64); (i__188++)) {{
            (((realBuffer__180__38)[(int)i__188]) = ((float)0.0))/*float*/;
            (((imagBuffer__181__38)[(int)i__188]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

// peek: 6 pop: 6 push 2
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_39;
int __counter_39 = 0;
int __steady_39 = 0;
int __tmp_39 = 0;
int __tmp2_39 = 0;
int *__state_flag_39 = NULL;
thread_info *__thread_39 = NULL;



float real_weight__193__39[64] = {0};
float imag_weight__194__39[64] = {0};
float realBuffer__195__39[64] = {0};
float imagBuffer__196__39[64] = {0};
int count__197__39 = 0;
int pos__198__39 = 0;
void save_peek_buffer__39(object_write_buffer *buf);
void load_peek_buffer__39(object_write_buffer *buf);
void save_file_pointer__39(object_write_buffer *buf);
void load_file_pointer__39(object_write_buffer *buf);

 
void init_BeamFirFilter__207_84__39();
inline void check_status__39();

void work_BeamFirFilter__207_84__39(int);


inline float __pop__39() {
float res=BUFFER_38_39[TAIL_38_39];
TAIL_38_39++;
return res;
}

inline float __pop__39(int n) {
float res=BUFFER_38_39[TAIL_38_39];
TAIL_38_39+=n;

return res;
}

inline float __peek__39(int offs) {
return BUFFER_38_39[TAIL_38_39+offs];
}



inline void __push__39(float data) {
BUFFER_39_5[HEAD_39_5]=data;
HEAD_39_5++;
}



 
void init_BeamFirFilter__207_84__39(){
  int idx__205 = 0;/* int */
  int j__206 = 0;/* int */

  ((pos__198__39) = 0)/*int*/;
  for ((j__206 = 0)/*int*/; (j__206 < 64); (j__206++)) {{
      (idx__205 = (j__206 + 1))/*int*/;
      (((real_weight__193__39)[(int)j__206]) = (((float)(sinf(((double)(idx__205))))) / ((float)(idx__205))))/*float*/;
      (((imag_weight__194__39)[(int)j__206]) = (((float)(cosf(((double)(idx__205))))) / ((float)(idx__205))))/*float*/;
    }
  }
}
void save_file_pointer__39(object_write_buffer *buf) {}
void load_file_pointer__39(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__207_84__39__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__201 = 0.0f;/* float */
  float imag_curr__202 = 0.0f;/* float */
  int i__203 = 0;/* int */
  int modPos__204 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__201 = ((float)0.0))/*float*/;
  (imag_curr__202 = ((float)0.0))/*float*/;
  (((realBuffer__195__39)[(int)(63 - (pos__198__39))]) = (*____in++))/*float*/;
  (((imagBuffer__196__39)[(int)(63 - (pos__198__39))]) = (*____in++))/*float*/;
  (modPos__204 = (63 - (pos__198__39)))/*int*/;
  for ((i__203 = 0)/*int*/; (i__203 < 64); (i__203++)) {{
      (real_curr__201 = (real_curr__201 + ((((realBuffer__195__39)[(int)modPos__204]) * ((real_weight__193__39)[(int)i__203])) + (((imagBuffer__196__39)[(int)modPos__204]) * ((imag_weight__194__39)[(int)i__203])))))/*float*/;
      (imag_curr__202 = (imag_curr__202 + ((((imagBuffer__196__39)[(int)modPos__204]) * ((real_weight__193__39)[(int)i__203])) + (((realBuffer__195__39)[(int)modPos__204]) * ((imag_weight__194__39)[(int)i__203])))))/*float*/;
      (modPos__204 = ((modPos__204 + 1) & 63))/*int*/;
    }
  }
  ((pos__198__39) = (((pos__198__39) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__201);
  ((*____out++)=imag_curr__202);
  for ((i__203 = 2)/*int*/; (i__203 < 6); (i__203++)) {{
      (*____in++);
    }
  }
  ((count__197__39) = ((count__197__39) + 3))/*int*/;

  if (((count__197__39) == 512)) {{
    ((count__197__39) = 0)/*int*/;
    ((pos__198__39) = 0)/*int*/;
    for ((i__203 = 0)/*int*/; (i__203 < 64); (i__203++)) {{
        (((realBuffer__195__39)[(int)i__203]) = ((float)0.0))/*float*/;
        (((imagBuffer__196__39)[(int)i__203]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__207_84__39__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__201 = 0.0f;/* float */
  float imag_curr__202 = 0.0f;/* float */
  int i__203 = 0;/* int */
  int modPos__204 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__201 = ((float)0.0))/*float*/;
  (imag_curr__202 = ((float)0.0))/*float*/;
  (((realBuffer__195__39)[(int)(63 - (pos__198__39))]) = (*____in++))/*float*/;
  (((imagBuffer__196__39)[(int)(63 - (pos__198__39))]) = (*____in++))/*float*/;
  (modPos__204 = (63 - (pos__198__39)))/*int*/;
  for ((i__203 = 0)/*int*/; (i__203 < 64); (i__203++)) {{
      (real_curr__201 = (real_curr__201 + ((((realBuffer__195__39)[(int)modPos__204]) * ((real_weight__193__39)[(int)i__203])) + (((imagBuffer__196__39)[(int)modPos__204]) * ((imag_weight__194__39)[(int)i__203])))))/*float*/;
      (imag_curr__202 = (imag_curr__202 + ((((imagBuffer__196__39)[(int)modPos__204]) * ((real_weight__193__39)[(int)i__203])) + (((realBuffer__195__39)[(int)modPos__204]) * ((imag_weight__194__39)[(int)i__203])))))/*float*/;
      (modPos__204 = ((modPos__204 + 1) & 63))/*int*/;
    }
  }
  ((pos__198__39) = (((pos__198__39) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__201);
  ((*____out++)=imag_curr__202);
  for ((i__203 = 2)/*int*/; (i__203 < 6); (i__203++)) {{
      (*____in++);
    }
  }
  ((count__197__39) = ((count__197__39) + 3))/*int*/;

  if (((count__197__39) == 512)) {{
    ((count__197__39) = 0)/*int*/;
    ((pos__198__39) = 0)/*int*/;
    for ((i__203 = 0)/*int*/; (i__203 < 64); (i__203++)) {{
        (((realBuffer__195__39)[(int)i__203]) = ((float)0.0))/*float*/;
        (((imagBuffer__196__39)[(int)i__203]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__207_84__39(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__201 = 0.0f;/* float */
      float imag_curr__202 = 0.0f;/* float */
      int i__203 = 0;/* int */
      int modPos__204 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__201 = ((float)0.0))/*float*/;
      (imag_curr__202 = ((float)0.0))/*float*/;
      (((realBuffer__195__39)[(int)(63 - (pos__198__39))]) = __pop__39())/*float*/;
      (((imagBuffer__196__39)[(int)(63 - (pos__198__39))]) = __pop__39())/*float*/;
      (modPos__204 = (63 - (pos__198__39)))/*int*/;
      for ((i__203 = 0)/*int*/; (i__203 < 64); (i__203++)) {{
          (real_curr__201 = (real_curr__201 + ((((realBuffer__195__39)[(int)modPos__204]) * ((real_weight__193__39)[(int)i__203])) + (((imagBuffer__196__39)[(int)modPos__204]) * ((imag_weight__194__39)[(int)i__203])))))/*float*/;
          (imag_curr__202 = (imag_curr__202 + ((((imagBuffer__196__39)[(int)modPos__204]) * ((real_weight__193__39)[(int)i__203])) + (((realBuffer__195__39)[(int)modPos__204]) * ((imag_weight__194__39)[(int)i__203])))))/*float*/;
          (modPos__204 = ((modPos__204 + 1) & 63))/*int*/;
        }
      }
      ((pos__198__39) = (((pos__198__39) + 1) & 63))/*int*/;
      __push__39(real_curr__201);
      __push__39(imag_curr__202);
      for ((i__203 = 2)/*int*/; (i__203 < 6); (i__203++)) {{
          __pop__39();
        }
      }
      ((count__197__39) = ((count__197__39) + 3))/*int*/;

      if (((count__197__39) == 512)) {{
        ((count__197__39) = 0)/*int*/;
        ((pos__198__39) = 0)/*int*/;
        for ((i__203 = 0)/*int*/; (i__203 < 64); (i__203++)) {{
            (((realBuffer__195__39)[(int)i__203]) = ((float)0.0))/*float*/;
            (((imagBuffer__196__39)[(int)i__203]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

// peek: 1 pop: 1 push 2
// init counts: 0 steady counts: 6

// ClusterFusion isEliminated: false



int __number_of_iterations_40;
int __counter_40 = 0;
int __steady_40 = 0;
int __tmp_40 = 0;
int __tmp2_40 = 0;
int *__state_flag_40 = NULL;
thread_info *__thread_40 = NULL;



int curSample__208__40 = 0;
void save_peek_buffer__40(object_write_buffer *buf);
void load_peek_buffer__40(object_write_buffer *buf);
void save_file_pointer__40(object_write_buffer *buf);
void load_file_pointer__40(object_write_buffer *buf);

 
void init_InputGenerate__211_86__40();
inline void check_status__40();

void work_InputGenerate__211_86__40(int);


inline int __pop__40() {
int res=BUFFER_1_40[TAIL_1_40];
TAIL_1_40++;
return res;
}

inline int __pop__40(int n) {
int res=BUFFER_1_40[TAIL_1_40];
TAIL_1_40+=n;

return res;
}

inline int __peek__40(int offs) {
return BUFFER_1_40[TAIL_1_40+offs];
}



inline void __push__40(float data) {
BUFFER_40_41[HEAD_40_41]=data;
HEAD_40_41++;
}



 
void init_InputGenerate__211_86__40(){
  ((curSample__208__40) = 0)/*int*/;
}
void save_file_pointer__40(object_write_buffer *buf) {}
void load_file_pointer__40(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_InputGenerate__211_86__40__mod(int ____n, int *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRFilter InputGenerate

  (*____in++);

  if ((0 && ((curSample__208__40) == 256))) {{
    ((*____out++)=((float)(sqrtf(((double)(((curSample__208__40) * 6)))))));
    ((*____out++)=(((float)(sqrtf(((double)(((curSample__208__40) * 6)))))) + ((float)1.0)));
  } } else {{
    ((*____out++)=(-((float)(sqrtf(((double)(((curSample__208__40) * 6))))))));
    ((*____out++)=(-(((float)(sqrtf(((double)(((curSample__208__40) * 6)))))) + ((float)1.0))));
  }}
  ((curSample__208__40)++);

  if (((curSample__208__40) >= 1024)) {((curSample__208__40) = 0)/*int*/;}
  // mark end: SIRFilter InputGenerate

}}


void work_InputGenerate__211_86__40__mod2(int ____n, int *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRFilter InputGenerate

  (*____in++);

  if ((0 && ((curSample__208__40) == 256))) {{
    ((*____out++)=((float)(sqrtf(((double)(((curSample__208__40) * 6)))))));
    ((*____out++)=(((float)(sqrtf(((double)(((curSample__208__40) * 6)))))) + ((float)1.0)));
  } } else {{
    ((*____out++)=(-((float)(sqrtf(((double)(((curSample__208__40) * 6))))))));
    ((*____out++)=(-(((float)(sqrtf(((double)(((curSample__208__40) * 6)))))) + ((float)1.0))));
  }}
  ((curSample__208__40)++);

  if (((curSample__208__40) >= 1024)) {((curSample__208__40) = 0)/*int*/;}
  // mark end: SIRFilter InputGenerate

}}


#endif // BUFFER_MERGE


 
void work_InputGenerate__211_86__40(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter InputGenerate

      __pop__40();

      if ((0 && ((curSample__208__40) == 256))) {{
        __push__40(((float)(sqrtf(((double)(((curSample__208__40) * 6)))))));
        __push__40((((float)(sqrtf(((double)(((curSample__208__40) * 6)))))) + ((float)1.0)));
      } } else {{
        __push__40((-((float)(sqrtf(((double)(((curSample__208__40) * 6))))))));
        __push__40((-(((float)(sqrtf(((double)(((curSample__208__40) * 6)))))) + ((float)1.0))));
      }}
      ((curSample__208__40)++);

      if (((curSample__208__40) >= 1024)) {((curSample__208__40) = 0)/*int*/;}
      // mark end: SIRFilter InputGenerate

    }
  }
}

// peek: 4 pop: 4 push 2
// init counts: 0 steady counts: 3

// ClusterFusion isEliminated: false



int __number_of_iterations_41;
int __counter_41 = 0;
int __steady_41 = 0;
int __tmp_41 = 0;
int __tmp2_41 = 0;
int *__state_flag_41 = NULL;
thread_info *__thread_41 = NULL;



float real_weight__212__41[64] = {0};
float imag_weight__213__41[64] = {0};
float realBuffer__214__41[64] = {0};
float imagBuffer__215__41[64] = {0};
int count__216__41 = 0;
int pos__217__41 = 0;
void save_peek_buffer__41(object_write_buffer *buf);
void load_peek_buffer__41(object_write_buffer *buf);
void save_file_pointer__41(object_write_buffer *buf);
void load_file_pointer__41(object_write_buffer *buf);

 
void init_BeamFirFilter__226_87__41();
inline void check_status__41();

void work_BeamFirFilter__226_87__41(int);


inline float __pop__41() {
float res=BUFFER_40_41[TAIL_40_41];
TAIL_40_41++;
return res;
}

inline float __pop__41(int n) {
float res=BUFFER_40_41[TAIL_40_41];
TAIL_40_41+=n;

return res;
}

inline float __peek__41(int offs) {
return BUFFER_40_41[TAIL_40_41+offs];
}



inline void __push__41(float data) {
BUFFER_41_42[HEAD_41_42]=data;
HEAD_41_42++;
}



 
void init_BeamFirFilter__226_87__41(){
  int idx__224 = 0;/* int */
  int j__225 = 0;/* int */

  ((pos__217__41) = 0)/*int*/;
  for ((j__225 = 0)/*int*/; (j__225 < 64); (j__225++)) {{
      (idx__224 = (j__225 + 1))/*int*/;
      (((real_weight__212__41)[(int)j__225]) = (((float)(sinf(((double)(idx__224))))) / ((float)(idx__224))))/*float*/;
      (((imag_weight__213__41)[(int)j__225]) = (((float)(cosf(((double)(idx__224))))) / ((float)(idx__224))))/*float*/;
    }
  }
}
void save_file_pointer__41(object_write_buffer *buf) {}
void load_file_pointer__41(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__226_87__41__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__220 = 0.0f;/* float */
  float imag_curr__221 = 0.0f;/* float */
  int i__222 = 0;/* int */
  int modPos__223 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__220 = ((float)0.0))/*float*/;
  (imag_curr__221 = ((float)0.0))/*float*/;
  (((realBuffer__214__41)[(int)(63 - (pos__217__41))]) = (*____in++))/*float*/;
  (((imagBuffer__215__41)[(int)(63 - (pos__217__41))]) = (*____in++))/*float*/;
  (modPos__223 = (63 - (pos__217__41)))/*int*/;
  for ((i__222 = 0)/*int*/; (i__222 < 64); (i__222++)) {{
      (real_curr__220 = (real_curr__220 + ((((realBuffer__214__41)[(int)modPos__223]) * ((real_weight__212__41)[(int)i__222])) + (((imagBuffer__215__41)[(int)modPos__223]) * ((imag_weight__213__41)[(int)i__222])))))/*float*/;
      (imag_curr__221 = (imag_curr__221 + ((((imagBuffer__215__41)[(int)modPos__223]) * ((real_weight__212__41)[(int)i__222])) + (((realBuffer__214__41)[(int)modPos__223]) * ((imag_weight__213__41)[(int)i__222])))))/*float*/;
      (modPos__223 = ((modPos__223 + 1) & 63))/*int*/;
    }
  }
  ((pos__217__41) = (((pos__217__41) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__220);
  ((*____out++)=imag_curr__221);
  for ((i__222 = 2)/*int*/; (i__222 < 4); (i__222++)) {{
      (*____in++);
    }
  }
  ((count__216__41) = ((count__216__41) + 2))/*int*/;

  if (((count__216__41) == 1024)) {{
    ((count__216__41) = 0)/*int*/;
    ((pos__217__41) = 0)/*int*/;
    for ((i__222 = 0)/*int*/; (i__222 < 64); (i__222++)) {{
        (((realBuffer__214__41)[(int)i__222]) = ((float)0.0))/*float*/;
        (((imagBuffer__215__41)[(int)i__222]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__226_87__41__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__220 = 0.0f;/* float */
  float imag_curr__221 = 0.0f;/* float */
  int i__222 = 0;/* int */
  int modPos__223 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__220 = ((float)0.0))/*float*/;
  (imag_curr__221 = ((float)0.0))/*float*/;
  (((realBuffer__214__41)[(int)(63 - (pos__217__41))]) = (*____in++))/*float*/;
  (((imagBuffer__215__41)[(int)(63 - (pos__217__41))]) = (*____in++))/*float*/;
  (modPos__223 = (63 - (pos__217__41)))/*int*/;
  for ((i__222 = 0)/*int*/; (i__222 < 64); (i__222++)) {{
      (real_curr__220 = (real_curr__220 + ((((realBuffer__214__41)[(int)modPos__223]) * ((real_weight__212__41)[(int)i__222])) + (((imagBuffer__215__41)[(int)modPos__223]) * ((imag_weight__213__41)[(int)i__222])))))/*float*/;
      (imag_curr__221 = (imag_curr__221 + ((((imagBuffer__215__41)[(int)modPos__223]) * ((real_weight__212__41)[(int)i__222])) + (((realBuffer__214__41)[(int)modPos__223]) * ((imag_weight__213__41)[(int)i__222])))))/*float*/;
      (modPos__223 = ((modPos__223 + 1) & 63))/*int*/;
    }
  }
  ((pos__217__41) = (((pos__217__41) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__220);
  ((*____out++)=imag_curr__221);
  for ((i__222 = 2)/*int*/; (i__222 < 4); (i__222++)) {{
      (*____in++);
    }
  }
  ((count__216__41) = ((count__216__41) + 2))/*int*/;

  if (((count__216__41) == 1024)) {{
    ((count__216__41) = 0)/*int*/;
    ((pos__217__41) = 0)/*int*/;
    for ((i__222 = 0)/*int*/; (i__222 < 64); (i__222++)) {{
        (((realBuffer__214__41)[(int)i__222]) = ((float)0.0))/*float*/;
        (((imagBuffer__215__41)[(int)i__222]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__226_87__41(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__220 = 0.0f;/* float */
      float imag_curr__221 = 0.0f;/* float */
      int i__222 = 0;/* int */
      int modPos__223 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__220 = ((float)0.0))/*float*/;
      (imag_curr__221 = ((float)0.0))/*float*/;
      (((realBuffer__214__41)[(int)(63 - (pos__217__41))]) = __pop__41())/*float*/;
      (((imagBuffer__215__41)[(int)(63 - (pos__217__41))]) = __pop__41())/*float*/;
      (modPos__223 = (63 - (pos__217__41)))/*int*/;
      for ((i__222 = 0)/*int*/; (i__222 < 64); (i__222++)) {{
          (real_curr__220 = (real_curr__220 + ((((realBuffer__214__41)[(int)modPos__223]) * ((real_weight__212__41)[(int)i__222])) + (((imagBuffer__215__41)[(int)modPos__223]) * ((imag_weight__213__41)[(int)i__222])))))/*float*/;
          (imag_curr__221 = (imag_curr__221 + ((((imagBuffer__215__41)[(int)modPos__223]) * ((real_weight__212__41)[(int)i__222])) + (((realBuffer__214__41)[(int)modPos__223]) * ((imag_weight__213__41)[(int)i__222])))))/*float*/;
          (modPos__223 = ((modPos__223 + 1) & 63))/*int*/;
        }
      }
      ((pos__217__41) = (((pos__217__41) + 1) & 63))/*int*/;
      __push__41(real_curr__220);
      __push__41(imag_curr__221);
      for ((i__222 = 2)/*int*/; (i__222 < 4); (i__222++)) {{
          __pop__41();
        }
      }
      ((count__216__41) = ((count__216__41) + 2))/*int*/;

      if (((count__216__41) == 1024)) {{
        ((count__216__41) = 0)/*int*/;
        ((pos__217__41) = 0)/*int*/;
        for ((i__222 = 0)/*int*/; (i__222 < 64); (i__222++)) {{
            (((realBuffer__214__41)[(int)i__222]) = ((float)0.0))/*float*/;
            (((imagBuffer__215__41)[(int)i__222]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

// peek: 6 pop: 6 push 2
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_42;
int __counter_42 = 0;
int __steady_42 = 0;
int __tmp_42 = 0;
int __tmp2_42 = 0;
int *__state_flag_42 = NULL;
thread_info *__thread_42 = NULL;



float real_weight__227__42[64] = {0};
float imag_weight__228__42[64] = {0};
float realBuffer__229__42[64] = {0};
float imagBuffer__230__42[64] = {0};
int count__231__42 = 0;
int pos__232__42 = 0;
void save_peek_buffer__42(object_write_buffer *buf);
void load_peek_buffer__42(object_write_buffer *buf);
void save_file_pointer__42(object_write_buffer *buf);
void load_file_pointer__42(object_write_buffer *buf);

 
void init_BeamFirFilter__241_88__42();
inline void check_status__42();

void work_BeamFirFilter__241_88__42(int);


inline float __pop__42() {
float res=BUFFER_41_42[TAIL_41_42];
TAIL_41_42++;
return res;
}

inline float __pop__42(int n) {
float res=BUFFER_41_42[TAIL_41_42];
TAIL_41_42+=n;

return res;
}

inline float __peek__42(int offs) {
return BUFFER_41_42[TAIL_41_42+offs];
}



inline void __push__42(float data) {
BUFFER_42_5[HEAD_42_5]=data;
HEAD_42_5++;
}



 
void init_BeamFirFilter__241_88__42(){
  int idx__239 = 0;/* int */
  int j__240 = 0;/* int */

  ((pos__232__42) = 0)/*int*/;
  for ((j__240 = 0)/*int*/; (j__240 < 64); (j__240++)) {{
      (idx__239 = (j__240 + 1))/*int*/;
      (((real_weight__227__42)[(int)j__240]) = (((float)(sinf(((double)(idx__239))))) / ((float)(idx__239))))/*float*/;
      (((imag_weight__228__42)[(int)j__240]) = (((float)(cosf(((double)(idx__239))))) / ((float)(idx__239))))/*float*/;
    }
  }
}
void save_file_pointer__42(object_write_buffer *buf) {}
void load_file_pointer__42(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__241_88__42__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__235 = 0.0f;/* float */
  float imag_curr__236 = 0.0f;/* float */
  int i__237 = 0;/* int */
  int modPos__238 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__235 = ((float)0.0))/*float*/;
  (imag_curr__236 = ((float)0.0))/*float*/;
  (((realBuffer__229__42)[(int)(63 - (pos__232__42))]) = (*____in++))/*float*/;
  (((imagBuffer__230__42)[(int)(63 - (pos__232__42))]) = (*____in++))/*float*/;
  (modPos__238 = (63 - (pos__232__42)))/*int*/;
  for ((i__237 = 0)/*int*/; (i__237 < 64); (i__237++)) {{
      (real_curr__235 = (real_curr__235 + ((((realBuffer__229__42)[(int)modPos__238]) * ((real_weight__227__42)[(int)i__237])) + (((imagBuffer__230__42)[(int)modPos__238]) * ((imag_weight__228__42)[(int)i__237])))))/*float*/;
      (imag_curr__236 = (imag_curr__236 + ((((imagBuffer__230__42)[(int)modPos__238]) * ((real_weight__227__42)[(int)i__237])) + (((realBuffer__229__42)[(int)modPos__238]) * ((imag_weight__228__42)[(int)i__237])))))/*float*/;
      (modPos__238 = ((modPos__238 + 1) & 63))/*int*/;
    }
  }
  ((pos__232__42) = (((pos__232__42) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__235);
  ((*____out++)=imag_curr__236);
  for ((i__237 = 2)/*int*/; (i__237 < 6); (i__237++)) {{
      (*____in++);
    }
  }
  ((count__231__42) = ((count__231__42) + 3))/*int*/;

  if (((count__231__42) == 512)) {{
    ((count__231__42) = 0)/*int*/;
    ((pos__232__42) = 0)/*int*/;
    for ((i__237 = 0)/*int*/; (i__237 < 64); (i__237++)) {{
        (((realBuffer__229__42)[(int)i__237]) = ((float)0.0))/*float*/;
        (((imagBuffer__230__42)[(int)i__237]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__241_88__42__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__235 = 0.0f;/* float */
  float imag_curr__236 = 0.0f;/* float */
  int i__237 = 0;/* int */
  int modPos__238 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__235 = ((float)0.0))/*float*/;
  (imag_curr__236 = ((float)0.0))/*float*/;
  (((realBuffer__229__42)[(int)(63 - (pos__232__42))]) = (*____in++))/*float*/;
  (((imagBuffer__230__42)[(int)(63 - (pos__232__42))]) = (*____in++))/*float*/;
  (modPos__238 = (63 - (pos__232__42)))/*int*/;
  for ((i__237 = 0)/*int*/; (i__237 < 64); (i__237++)) {{
      (real_curr__235 = (real_curr__235 + ((((realBuffer__229__42)[(int)modPos__238]) * ((real_weight__227__42)[(int)i__237])) + (((imagBuffer__230__42)[(int)modPos__238]) * ((imag_weight__228__42)[(int)i__237])))))/*float*/;
      (imag_curr__236 = (imag_curr__236 + ((((imagBuffer__230__42)[(int)modPos__238]) * ((real_weight__227__42)[(int)i__237])) + (((realBuffer__229__42)[(int)modPos__238]) * ((imag_weight__228__42)[(int)i__237])))))/*float*/;
      (modPos__238 = ((modPos__238 + 1) & 63))/*int*/;
    }
  }
  ((pos__232__42) = (((pos__232__42) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__235);
  ((*____out++)=imag_curr__236);
  for ((i__237 = 2)/*int*/; (i__237 < 6); (i__237++)) {{
      (*____in++);
    }
  }
  ((count__231__42) = ((count__231__42) + 3))/*int*/;

  if (((count__231__42) == 512)) {{
    ((count__231__42) = 0)/*int*/;
    ((pos__232__42) = 0)/*int*/;
    for ((i__237 = 0)/*int*/; (i__237 < 64); (i__237++)) {{
        (((realBuffer__229__42)[(int)i__237]) = ((float)0.0))/*float*/;
        (((imagBuffer__230__42)[(int)i__237]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__241_88__42(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__235 = 0.0f;/* float */
      float imag_curr__236 = 0.0f;/* float */
      int i__237 = 0;/* int */
      int modPos__238 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__235 = ((float)0.0))/*float*/;
      (imag_curr__236 = ((float)0.0))/*float*/;
      (((realBuffer__229__42)[(int)(63 - (pos__232__42))]) = __pop__42())/*float*/;
      (((imagBuffer__230__42)[(int)(63 - (pos__232__42))]) = __pop__42())/*float*/;
      (modPos__238 = (63 - (pos__232__42)))/*int*/;
      for ((i__237 = 0)/*int*/; (i__237 < 64); (i__237++)) {{
          (real_curr__235 = (real_curr__235 + ((((realBuffer__229__42)[(int)modPos__238]) * ((real_weight__227__42)[(int)i__237])) + (((imagBuffer__230__42)[(int)modPos__238]) * ((imag_weight__228__42)[(int)i__237])))))/*float*/;
          (imag_curr__236 = (imag_curr__236 + ((((imagBuffer__230__42)[(int)modPos__238]) * ((real_weight__227__42)[(int)i__237])) + (((realBuffer__229__42)[(int)modPos__238]) * ((imag_weight__228__42)[(int)i__237])))))/*float*/;
          (modPos__238 = ((modPos__238 + 1) & 63))/*int*/;
        }
      }
      ((pos__232__42) = (((pos__232__42) + 1) & 63))/*int*/;
      __push__42(real_curr__235);
      __push__42(imag_curr__236);
      for ((i__237 = 2)/*int*/; (i__237 < 6); (i__237++)) {{
          __pop__42();
        }
      }
      ((count__231__42) = ((count__231__42) + 3))/*int*/;

      if (((count__231__42) == 512)) {{
        ((count__231__42) = 0)/*int*/;
        ((pos__232__42) = 0)/*int*/;
        for ((i__237 = 0)/*int*/; (i__237 < 64); (i__237++)) {{
            (((realBuffer__229__42)[(int)i__237]) = ((float)0.0))/*float*/;
            (((imagBuffer__230__42)[(int)i__237]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

// peek: 1 pop: 1 push 2
// init counts: 0 steady counts: 6

// ClusterFusion isEliminated: false



int __number_of_iterations_43;
int __counter_43 = 0;
int __steady_43 = 0;
int __tmp_43 = 0;
int __tmp2_43 = 0;
int *__state_flag_43 = NULL;
thread_info *__thread_43 = NULL;



int curSample__242__43 = 0;
void save_peek_buffer__43(object_write_buffer *buf);
void load_peek_buffer__43(object_write_buffer *buf);
void save_file_pointer__43(object_write_buffer *buf);
void load_file_pointer__43(object_write_buffer *buf);

 
void init_InputGenerate__245_90__43();
inline void check_status__43();

void work_InputGenerate__245_90__43(int);


inline int __pop__43() {
int res=BUFFER_1_43[TAIL_1_43];
TAIL_1_43++;
return res;
}

inline int __pop__43(int n) {
int res=BUFFER_1_43[TAIL_1_43];
TAIL_1_43+=n;

return res;
}

inline int __peek__43(int offs) {
return BUFFER_1_43[TAIL_1_43+offs];
}



inline void __push__43(float data) {
BUFFER_43_44[HEAD_43_44]=data;
HEAD_43_44++;
}



 
void init_InputGenerate__245_90__43(){
  ((curSample__242__43) = 0)/*int*/;
}
void save_file_pointer__43(object_write_buffer *buf) {}
void load_file_pointer__43(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_InputGenerate__245_90__43__mod(int ____n, int *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRFilter InputGenerate

  (*____in++);

  if ((0 && ((curSample__242__43) == 256))) {{
    ((*____out++)=((float)(sqrtf(((double)(((curSample__242__43) * 7)))))));
    ((*____out++)=(((float)(sqrtf(((double)(((curSample__242__43) * 7)))))) + ((float)1.0)));
  } } else {{
    ((*____out++)=(-((float)(sqrtf(((double)(((curSample__242__43) * 7))))))));
    ((*____out++)=(-(((float)(sqrtf(((double)(((curSample__242__43) * 7)))))) + ((float)1.0))));
  }}
  ((curSample__242__43)++);

  if (((curSample__242__43) >= 1024)) {((curSample__242__43) = 0)/*int*/;}
  // mark end: SIRFilter InputGenerate

}}


void work_InputGenerate__245_90__43__mod2(int ____n, int *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRFilter InputGenerate

  (*____in++);

  if ((0 && ((curSample__242__43) == 256))) {{
    ((*____out++)=((float)(sqrtf(((double)(((curSample__242__43) * 7)))))));
    ((*____out++)=(((float)(sqrtf(((double)(((curSample__242__43) * 7)))))) + ((float)1.0)));
  } } else {{
    ((*____out++)=(-((float)(sqrtf(((double)(((curSample__242__43) * 7))))))));
    ((*____out++)=(-(((float)(sqrtf(((double)(((curSample__242__43) * 7)))))) + ((float)1.0))));
  }}
  ((curSample__242__43)++);

  if (((curSample__242__43) >= 1024)) {((curSample__242__43) = 0)/*int*/;}
  // mark end: SIRFilter InputGenerate

}}


#endif // BUFFER_MERGE


 
void work_InputGenerate__245_90__43(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter InputGenerate

      __pop__43();

      if ((0 && ((curSample__242__43) == 256))) {{
        __push__43(((float)(sqrtf(((double)(((curSample__242__43) * 7)))))));
        __push__43((((float)(sqrtf(((double)(((curSample__242__43) * 7)))))) + ((float)1.0)));
      } } else {{
        __push__43((-((float)(sqrtf(((double)(((curSample__242__43) * 7))))))));
        __push__43((-(((float)(sqrtf(((double)(((curSample__242__43) * 7)))))) + ((float)1.0))));
      }}
      ((curSample__242__43)++);

      if (((curSample__242__43) >= 1024)) {((curSample__242__43) = 0)/*int*/;}
      // mark end: SIRFilter InputGenerate

    }
  }
}

// peek: 4 pop: 4 push 2
// init counts: 0 steady counts: 3

// ClusterFusion isEliminated: false



int __number_of_iterations_44;
int __counter_44 = 0;
int __steady_44 = 0;
int __tmp_44 = 0;
int __tmp2_44 = 0;
int *__state_flag_44 = NULL;
thread_info *__thread_44 = NULL;



float real_weight__246__44[64] = {0};
float imag_weight__247__44[64] = {0};
float realBuffer__248__44[64] = {0};
float imagBuffer__249__44[64] = {0};
int count__250__44 = 0;
int pos__251__44 = 0;
void save_peek_buffer__44(object_write_buffer *buf);
void load_peek_buffer__44(object_write_buffer *buf);
void save_file_pointer__44(object_write_buffer *buf);
void load_file_pointer__44(object_write_buffer *buf);

 
void init_BeamFirFilter__260_91__44();
inline void check_status__44();

void work_BeamFirFilter__260_91__44(int);


inline float __pop__44() {
float res=BUFFER_43_44[TAIL_43_44];
TAIL_43_44++;
return res;
}

inline float __pop__44(int n) {
float res=BUFFER_43_44[TAIL_43_44];
TAIL_43_44+=n;

return res;
}

inline float __peek__44(int offs) {
return BUFFER_43_44[TAIL_43_44+offs];
}



inline void __push__44(float data) {
BUFFER_44_45[HEAD_44_45]=data;
HEAD_44_45++;
}



 
void init_BeamFirFilter__260_91__44(){
  int idx__258 = 0;/* int */
  int j__259 = 0;/* int */

  ((pos__251__44) = 0)/*int*/;
  for ((j__259 = 0)/*int*/; (j__259 < 64); (j__259++)) {{
      (idx__258 = (j__259 + 1))/*int*/;
      (((real_weight__246__44)[(int)j__259]) = (((float)(sinf(((double)(idx__258))))) / ((float)(idx__258))))/*float*/;
      (((imag_weight__247__44)[(int)j__259]) = (((float)(cosf(((double)(idx__258))))) / ((float)(idx__258))))/*float*/;
    }
  }
}
void save_file_pointer__44(object_write_buffer *buf) {}
void load_file_pointer__44(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__260_91__44__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__254 = 0.0f;/* float */
  float imag_curr__255 = 0.0f;/* float */
  int i__256 = 0;/* int */
  int modPos__257 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__254 = ((float)0.0))/*float*/;
  (imag_curr__255 = ((float)0.0))/*float*/;
  (((realBuffer__248__44)[(int)(63 - (pos__251__44))]) = (*____in++))/*float*/;
  (((imagBuffer__249__44)[(int)(63 - (pos__251__44))]) = (*____in++))/*float*/;
  (modPos__257 = (63 - (pos__251__44)))/*int*/;
  for ((i__256 = 0)/*int*/; (i__256 < 64); (i__256++)) {{
      (real_curr__254 = (real_curr__254 + ((((realBuffer__248__44)[(int)modPos__257]) * ((real_weight__246__44)[(int)i__256])) + (((imagBuffer__249__44)[(int)modPos__257]) * ((imag_weight__247__44)[(int)i__256])))))/*float*/;
      (imag_curr__255 = (imag_curr__255 + ((((imagBuffer__249__44)[(int)modPos__257]) * ((real_weight__246__44)[(int)i__256])) + (((realBuffer__248__44)[(int)modPos__257]) * ((imag_weight__247__44)[(int)i__256])))))/*float*/;
      (modPos__257 = ((modPos__257 + 1) & 63))/*int*/;
    }
  }
  ((pos__251__44) = (((pos__251__44) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__254);
  ((*____out++)=imag_curr__255);
  for ((i__256 = 2)/*int*/; (i__256 < 4); (i__256++)) {{
      (*____in++);
    }
  }
  ((count__250__44) = ((count__250__44) + 2))/*int*/;

  if (((count__250__44) == 1024)) {{
    ((count__250__44) = 0)/*int*/;
    ((pos__251__44) = 0)/*int*/;
    for ((i__256 = 0)/*int*/; (i__256 < 64); (i__256++)) {{
        (((realBuffer__248__44)[(int)i__256]) = ((float)0.0))/*float*/;
        (((imagBuffer__249__44)[(int)i__256]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__260_91__44__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__254 = 0.0f;/* float */
  float imag_curr__255 = 0.0f;/* float */
  int i__256 = 0;/* int */
  int modPos__257 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__254 = ((float)0.0))/*float*/;
  (imag_curr__255 = ((float)0.0))/*float*/;
  (((realBuffer__248__44)[(int)(63 - (pos__251__44))]) = (*____in++))/*float*/;
  (((imagBuffer__249__44)[(int)(63 - (pos__251__44))]) = (*____in++))/*float*/;
  (modPos__257 = (63 - (pos__251__44)))/*int*/;
  for ((i__256 = 0)/*int*/; (i__256 < 64); (i__256++)) {{
      (real_curr__254 = (real_curr__254 + ((((realBuffer__248__44)[(int)modPos__257]) * ((real_weight__246__44)[(int)i__256])) + (((imagBuffer__249__44)[(int)modPos__257]) * ((imag_weight__247__44)[(int)i__256])))))/*float*/;
      (imag_curr__255 = (imag_curr__255 + ((((imagBuffer__249__44)[(int)modPos__257]) * ((real_weight__246__44)[(int)i__256])) + (((realBuffer__248__44)[(int)modPos__257]) * ((imag_weight__247__44)[(int)i__256])))))/*float*/;
      (modPos__257 = ((modPos__257 + 1) & 63))/*int*/;
    }
  }
  ((pos__251__44) = (((pos__251__44) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__254);
  ((*____out++)=imag_curr__255);
  for ((i__256 = 2)/*int*/; (i__256 < 4); (i__256++)) {{
      (*____in++);
    }
  }
  ((count__250__44) = ((count__250__44) + 2))/*int*/;

  if (((count__250__44) == 1024)) {{
    ((count__250__44) = 0)/*int*/;
    ((pos__251__44) = 0)/*int*/;
    for ((i__256 = 0)/*int*/; (i__256 < 64); (i__256++)) {{
        (((realBuffer__248__44)[(int)i__256]) = ((float)0.0))/*float*/;
        (((imagBuffer__249__44)[(int)i__256]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__260_91__44(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__254 = 0.0f;/* float */
      float imag_curr__255 = 0.0f;/* float */
      int i__256 = 0;/* int */
      int modPos__257 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__254 = ((float)0.0))/*float*/;
      (imag_curr__255 = ((float)0.0))/*float*/;
      (((realBuffer__248__44)[(int)(63 - (pos__251__44))]) = __pop__44())/*float*/;
      (((imagBuffer__249__44)[(int)(63 - (pos__251__44))]) = __pop__44())/*float*/;
      (modPos__257 = (63 - (pos__251__44)))/*int*/;
      for ((i__256 = 0)/*int*/; (i__256 < 64); (i__256++)) {{
          (real_curr__254 = (real_curr__254 + ((((realBuffer__248__44)[(int)modPos__257]) * ((real_weight__246__44)[(int)i__256])) + (((imagBuffer__249__44)[(int)modPos__257]) * ((imag_weight__247__44)[(int)i__256])))))/*float*/;
          (imag_curr__255 = (imag_curr__255 + ((((imagBuffer__249__44)[(int)modPos__257]) * ((real_weight__246__44)[(int)i__256])) + (((realBuffer__248__44)[(int)modPos__257]) * ((imag_weight__247__44)[(int)i__256])))))/*float*/;
          (modPos__257 = ((modPos__257 + 1) & 63))/*int*/;
        }
      }
      ((pos__251__44) = (((pos__251__44) + 1) & 63))/*int*/;
      __push__44(real_curr__254);
      __push__44(imag_curr__255);
      for ((i__256 = 2)/*int*/; (i__256 < 4); (i__256++)) {{
          __pop__44();
        }
      }
      ((count__250__44) = ((count__250__44) + 2))/*int*/;

      if (((count__250__44) == 1024)) {{
        ((count__250__44) = 0)/*int*/;
        ((pos__251__44) = 0)/*int*/;
        for ((i__256 = 0)/*int*/; (i__256 < 64); (i__256++)) {{
            (((realBuffer__248__44)[(int)i__256]) = ((float)0.0))/*float*/;
            (((imagBuffer__249__44)[(int)i__256]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

// peek: 6 pop: 6 push 2
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_45;
int __counter_45 = 0;
int __steady_45 = 0;
int __tmp_45 = 0;
int __tmp2_45 = 0;
int *__state_flag_45 = NULL;
thread_info *__thread_45 = NULL;



float real_weight__261__45[64] = {0};
float imag_weight__262__45[64] = {0};
float realBuffer__263__45[64] = {0};
float imagBuffer__264__45[64] = {0};
int count__265__45 = 0;
int pos__266__45 = 0;
void save_peek_buffer__45(object_write_buffer *buf);
void load_peek_buffer__45(object_write_buffer *buf);
void save_file_pointer__45(object_write_buffer *buf);
void load_file_pointer__45(object_write_buffer *buf);

 
void init_BeamFirFilter__275_92__45();
inline void check_status__45();

void work_BeamFirFilter__275_92__45(int);


inline float __pop__45() {
float res=BUFFER_44_45[TAIL_44_45];
TAIL_44_45++;
return res;
}

inline float __pop__45(int n) {
float res=BUFFER_44_45[TAIL_44_45];
TAIL_44_45+=n;

return res;
}

inline float __peek__45(int offs) {
return BUFFER_44_45[TAIL_44_45+offs];
}



inline void __push__45(float data) {
BUFFER_45_5[HEAD_45_5]=data;
HEAD_45_5++;
}



 
void init_BeamFirFilter__275_92__45(){
  int idx__273 = 0;/* int */
  int j__274 = 0;/* int */

  ((pos__266__45) = 0)/*int*/;
  for ((j__274 = 0)/*int*/; (j__274 < 64); (j__274++)) {{
      (idx__273 = (j__274 + 1))/*int*/;
      (((real_weight__261__45)[(int)j__274]) = (((float)(sinf(((double)(idx__273))))) / ((float)(idx__273))))/*float*/;
      (((imag_weight__262__45)[(int)j__274]) = (((float)(cosf(((double)(idx__273))))) / ((float)(idx__273))))/*float*/;
    }
  }
}
void save_file_pointer__45(object_write_buffer *buf) {}
void load_file_pointer__45(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__275_92__45__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__269 = 0.0f;/* float */
  float imag_curr__270 = 0.0f;/* float */
  int i__271 = 0;/* int */
  int modPos__272 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__269 = ((float)0.0))/*float*/;
  (imag_curr__270 = ((float)0.0))/*float*/;
  (((realBuffer__263__45)[(int)(63 - (pos__266__45))]) = (*____in++))/*float*/;
  (((imagBuffer__264__45)[(int)(63 - (pos__266__45))]) = (*____in++))/*float*/;
  (modPos__272 = (63 - (pos__266__45)))/*int*/;
  for ((i__271 = 0)/*int*/; (i__271 < 64); (i__271++)) {{
      (real_curr__269 = (real_curr__269 + ((((realBuffer__263__45)[(int)modPos__272]) * ((real_weight__261__45)[(int)i__271])) + (((imagBuffer__264__45)[(int)modPos__272]) * ((imag_weight__262__45)[(int)i__271])))))/*float*/;
      (imag_curr__270 = (imag_curr__270 + ((((imagBuffer__264__45)[(int)modPos__272]) * ((real_weight__261__45)[(int)i__271])) + (((realBuffer__263__45)[(int)modPos__272]) * ((imag_weight__262__45)[(int)i__271])))))/*float*/;
      (modPos__272 = ((modPos__272 + 1) & 63))/*int*/;
    }
  }
  ((pos__266__45) = (((pos__266__45) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__269);
  ((*____out++)=imag_curr__270);
  for ((i__271 = 2)/*int*/; (i__271 < 6); (i__271++)) {{
      (*____in++);
    }
  }
  ((count__265__45) = ((count__265__45) + 3))/*int*/;

  if (((count__265__45) == 512)) {{
    ((count__265__45) = 0)/*int*/;
    ((pos__266__45) = 0)/*int*/;
    for ((i__271 = 0)/*int*/; (i__271 < 64); (i__271++)) {{
        (((realBuffer__263__45)[(int)i__271]) = ((float)0.0))/*float*/;
        (((imagBuffer__264__45)[(int)i__271]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__275_92__45__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__269 = 0.0f;/* float */
  float imag_curr__270 = 0.0f;/* float */
  int i__271 = 0;/* int */
  int modPos__272 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__269 = ((float)0.0))/*float*/;
  (imag_curr__270 = ((float)0.0))/*float*/;
  (((realBuffer__263__45)[(int)(63 - (pos__266__45))]) = (*____in++))/*float*/;
  (((imagBuffer__264__45)[(int)(63 - (pos__266__45))]) = (*____in++))/*float*/;
  (modPos__272 = (63 - (pos__266__45)))/*int*/;
  for ((i__271 = 0)/*int*/; (i__271 < 64); (i__271++)) {{
      (real_curr__269 = (real_curr__269 + ((((realBuffer__263__45)[(int)modPos__272]) * ((real_weight__261__45)[(int)i__271])) + (((imagBuffer__264__45)[(int)modPos__272]) * ((imag_weight__262__45)[(int)i__271])))))/*float*/;
      (imag_curr__270 = (imag_curr__270 + ((((imagBuffer__264__45)[(int)modPos__272]) * ((real_weight__261__45)[(int)i__271])) + (((realBuffer__263__45)[(int)modPos__272]) * ((imag_weight__262__45)[(int)i__271])))))/*float*/;
      (modPos__272 = ((modPos__272 + 1) & 63))/*int*/;
    }
  }
  ((pos__266__45) = (((pos__266__45) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__269);
  ((*____out++)=imag_curr__270);
  for ((i__271 = 2)/*int*/; (i__271 < 6); (i__271++)) {{
      (*____in++);
    }
  }
  ((count__265__45) = ((count__265__45) + 3))/*int*/;

  if (((count__265__45) == 512)) {{
    ((count__265__45) = 0)/*int*/;
    ((pos__266__45) = 0)/*int*/;
    for ((i__271 = 0)/*int*/; (i__271 < 64); (i__271++)) {{
        (((realBuffer__263__45)[(int)i__271]) = ((float)0.0))/*float*/;
        (((imagBuffer__264__45)[(int)i__271]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__275_92__45(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__269 = 0.0f;/* float */
      float imag_curr__270 = 0.0f;/* float */
      int i__271 = 0;/* int */
      int modPos__272 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__269 = ((float)0.0))/*float*/;
      (imag_curr__270 = ((float)0.0))/*float*/;
      (((realBuffer__263__45)[(int)(63 - (pos__266__45))]) = __pop__45())/*float*/;
      (((imagBuffer__264__45)[(int)(63 - (pos__266__45))]) = __pop__45())/*float*/;
      (modPos__272 = (63 - (pos__266__45)))/*int*/;
      for ((i__271 = 0)/*int*/; (i__271 < 64); (i__271++)) {{
          (real_curr__269 = (real_curr__269 + ((((realBuffer__263__45)[(int)modPos__272]) * ((real_weight__261__45)[(int)i__271])) + (((imagBuffer__264__45)[(int)modPos__272]) * ((imag_weight__262__45)[(int)i__271])))))/*float*/;
          (imag_curr__270 = (imag_curr__270 + ((((imagBuffer__264__45)[(int)modPos__272]) * ((real_weight__261__45)[(int)i__271])) + (((realBuffer__263__45)[(int)modPos__272]) * ((imag_weight__262__45)[(int)i__271])))))/*float*/;
          (modPos__272 = ((modPos__272 + 1) & 63))/*int*/;
        }
      }
      ((pos__266__45) = (((pos__266__45) + 1) & 63))/*int*/;
      __push__45(real_curr__269);
      __push__45(imag_curr__270);
      for ((i__271 = 2)/*int*/; (i__271 < 6); (i__271++)) {{
          __pop__45();
        }
      }
      ((count__265__45) = ((count__265__45) + 3))/*int*/;

      if (((count__265__45) == 512)) {{
        ((count__265__45) = 0)/*int*/;
        ((pos__266__45) = 0)/*int*/;
        for ((i__271 = 0)/*int*/; (i__271 < 64); (i__271++)) {{
            (((realBuffer__263__45)[(int)i__271]) = ((float)0.0))/*float*/;
            (((imagBuffer__264__45)[(int)i__271]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

// peek: 1 pop: 1 push 2
// init counts: 0 steady counts: 6

// ClusterFusion isEliminated: false



int __number_of_iterations_46;
int __counter_46 = 0;
int __steady_46 = 0;
int __tmp_46 = 0;
int __tmp2_46 = 0;
int *__state_flag_46 = NULL;
thread_info *__thread_46 = NULL;



int curSample__276__46 = 0;
void save_peek_buffer__46(object_write_buffer *buf);
void load_peek_buffer__46(object_write_buffer *buf);
void save_file_pointer__46(object_write_buffer *buf);
void load_file_pointer__46(object_write_buffer *buf);

 
void init_InputGenerate__279_94__46();
inline void check_status__46();

void work_InputGenerate__279_94__46(int);


inline int __pop__46() {
int res=BUFFER_1_46[TAIL_1_46];
TAIL_1_46++;
return res;
}

inline int __pop__46(int n) {
int res=BUFFER_1_46[TAIL_1_46];
TAIL_1_46+=n;

return res;
}

inline int __peek__46(int offs) {
return BUFFER_1_46[TAIL_1_46+offs];
}



inline void __push__46(float data) {
BUFFER_46_47[HEAD_46_47]=data;
HEAD_46_47++;
}



 
void init_InputGenerate__279_94__46(){
  ((curSample__276__46) = 0)/*int*/;
}
void save_file_pointer__46(object_write_buffer *buf) {}
void load_file_pointer__46(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_InputGenerate__279_94__46__mod(int ____n, int *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRFilter InputGenerate

  (*____in++);

  if ((0 && ((curSample__276__46) == 256))) {{
    ((*____out++)=((float)(sqrtf(((double)(((curSample__276__46) * 8)))))));
    ((*____out++)=(((float)(sqrtf(((double)(((curSample__276__46) * 8)))))) + ((float)1.0)));
  } } else {{
    ((*____out++)=(-((float)(sqrtf(((double)(((curSample__276__46) * 8))))))));
    ((*____out++)=(-(((float)(sqrtf(((double)(((curSample__276__46) * 8)))))) + ((float)1.0))));
  }}
  ((curSample__276__46)++);

  if (((curSample__276__46) >= 1024)) {((curSample__276__46) = 0)/*int*/;}
  // mark end: SIRFilter InputGenerate

}}


void work_InputGenerate__279_94__46__mod2(int ____n, int *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRFilter InputGenerate

  (*____in++);

  if ((0 && ((curSample__276__46) == 256))) {{
    ((*____out++)=((float)(sqrtf(((double)(((curSample__276__46) * 8)))))));
    ((*____out++)=(((float)(sqrtf(((double)(((curSample__276__46) * 8)))))) + ((float)1.0)));
  } } else {{
    ((*____out++)=(-((float)(sqrtf(((double)(((curSample__276__46) * 8))))))));
    ((*____out++)=(-(((float)(sqrtf(((double)(((curSample__276__46) * 8)))))) + ((float)1.0))));
  }}
  ((curSample__276__46)++);

  if (((curSample__276__46) >= 1024)) {((curSample__276__46) = 0)/*int*/;}
  // mark end: SIRFilter InputGenerate

}}


#endif // BUFFER_MERGE


 
void work_InputGenerate__279_94__46(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter InputGenerate

      __pop__46();

      if ((0 && ((curSample__276__46) == 256))) {{
        __push__46(((float)(sqrtf(((double)(((curSample__276__46) * 8)))))));
        __push__46((((float)(sqrtf(((double)(((curSample__276__46) * 8)))))) + ((float)1.0)));
      } } else {{
        __push__46((-((float)(sqrtf(((double)(((curSample__276__46) * 8))))))));
        __push__46((-(((float)(sqrtf(((double)(((curSample__276__46) * 8)))))) + ((float)1.0))));
      }}
      ((curSample__276__46)++);

      if (((curSample__276__46) >= 1024)) {((curSample__276__46) = 0)/*int*/;}
      // mark end: SIRFilter InputGenerate

    }
  }
}

// peek: 4 pop: 4 push 2
// init counts: 0 steady counts: 3

// ClusterFusion isEliminated: false



int __number_of_iterations_47;
int __counter_47 = 0;
int __steady_47 = 0;
int __tmp_47 = 0;
int __tmp2_47 = 0;
int *__state_flag_47 = NULL;
thread_info *__thread_47 = NULL;



float real_weight__280__47[64] = {0};
float imag_weight__281__47[64] = {0};
float realBuffer__282__47[64] = {0};
float imagBuffer__283__47[64] = {0};
int count__284__47 = 0;
int pos__285__47 = 0;
void save_peek_buffer__47(object_write_buffer *buf);
void load_peek_buffer__47(object_write_buffer *buf);
void save_file_pointer__47(object_write_buffer *buf);
void load_file_pointer__47(object_write_buffer *buf);

 
void init_BeamFirFilter__294_95__47();
inline void check_status__47();

void work_BeamFirFilter__294_95__47(int);


inline float __pop__47() {
float res=BUFFER_46_47[TAIL_46_47];
TAIL_46_47++;
return res;
}

inline float __pop__47(int n) {
float res=BUFFER_46_47[TAIL_46_47];
TAIL_46_47+=n;

return res;
}

inline float __peek__47(int offs) {
return BUFFER_46_47[TAIL_46_47+offs];
}



inline void __push__47(float data) {
BUFFER_47_48[HEAD_47_48]=data;
HEAD_47_48++;
}



 
void init_BeamFirFilter__294_95__47(){
  int idx__292 = 0;/* int */
  int j__293 = 0;/* int */

  ((pos__285__47) = 0)/*int*/;
  for ((j__293 = 0)/*int*/; (j__293 < 64); (j__293++)) {{
      (idx__292 = (j__293 + 1))/*int*/;
      (((real_weight__280__47)[(int)j__293]) = (((float)(sinf(((double)(idx__292))))) / ((float)(idx__292))))/*float*/;
      (((imag_weight__281__47)[(int)j__293]) = (((float)(cosf(((double)(idx__292))))) / ((float)(idx__292))))/*float*/;
    }
  }
}
void save_file_pointer__47(object_write_buffer *buf) {}
void load_file_pointer__47(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__294_95__47__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__288 = 0.0f;/* float */
  float imag_curr__289 = 0.0f;/* float */
  int i__290 = 0;/* int */
  int modPos__291 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__288 = ((float)0.0))/*float*/;
  (imag_curr__289 = ((float)0.0))/*float*/;
  (((realBuffer__282__47)[(int)(63 - (pos__285__47))]) = (*____in++))/*float*/;
  (((imagBuffer__283__47)[(int)(63 - (pos__285__47))]) = (*____in++))/*float*/;
  (modPos__291 = (63 - (pos__285__47)))/*int*/;
  for ((i__290 = 0)/*int*/; (i__290 < 64); (i__290++)) {{
      (real_curr__288 = (real_curr__288 + ((((realBuffer__282__47)[(int)modPos__291]) * ((real_weight__280__47)[(int)i__290])) + (((imagBuffer__283__47)[(int)modPos__291]) * ((imag_weight__281__47)[(int)i__290])))))/*float*/;
      (imag_curr__289 = (imag_curr__289 + ((((imagBuffer__283__47)[(int)modPos__291]) * ((real_weight__280__47)[(int)i__290])) + (((realBuffer__282__47)[(int)modPos__291]) * ((imag_weight__281__47)[(int)i__290])))))/*float*/;
      (modPos__291 = ((modPos__291 + 1) & 63))/*int*/;
    }
  }
  ((pos__285__47) = (((pos__285__47) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__288);
  ((*____out++)=imag_curr__289);
  for ((i__290 = 2)/*int*/; (i__290 < 4); (i__290++)) {{
      (*____in++);
    }
  }
  ((count__284__47) = ((count__284__47) + 2))/*int*/;

  if (((count__284__47) == 1024)) {{
    ((count__284__47) = 0)/*int*/;
    ((pos__285__47) = 0)/*int*/;
    for ((i__290 = 0)/*int*/; (i__290 < 64); (i__290++)) {{
        (((realBuffer__282__47)[(int)i__290]) = ((float)0.0))/*float*/;
        (((imagBuffer__283__47)[(int)i__290]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__294_95__47__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__288 = 0.0f;/* float */
  float imag_curr__289 = 0.0f;/* float */
  int i__290 = 0;/* int */
  int modPos__291 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__288 = ((float)0.0))/*float*/;
  (imag_curr__289 = ((float)0.0))/*float*/;
  (((realBuffer__282__47)[(int)(63 - (pos__285__47))]) = (*____in++))/*float*/;
  (((imagBuffer__283__47)[(int)(63 - (pos__285__47))]) = (*____in++))/*float*/;
  (modPos__291 = (63 - (pos__285__47)))/*int*/;
  for ((i__290 = 0)/*int*/; (i__290 < 64); (i__290++)) {{
      (real_curr__288 = (real_curr__288 + ((((realBuffer__282__47)[(int)modPos__291]) * ((real_weight__280__47)[(int)i__290])) + (((imagBuffer__283__47)[(int)modPos__291]) * ((imag_weight__281__47)[(int)i__290])))))/*float*/;
      (imag_curr__289 = (imag_curr__289 + ((((imagBuffer__283__47)[(int)modPos__291]) * ((real_weight__280__47)[(int)i__290])) + (((realBuffer__282__47)[(int)modPos__291]) * ((imag_weight__281__47)[(int)i__290])))))/*float*/;
      (modPos__291 = ((modPos__291 + 1) & 63))/*int*/;
    }
  }
  ((pos__285__47) = (((pos__285__47) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__288);
  ((*____out++)=imag_curr__289);
  for ((i__290 = 2)/*int*/; (i__290 < 4); (i__290++)) {{
      (*____in++);
    }
  }
  ((count__284__47) = ((count__284__47) + 2))/*int*/;

  if (((count__284__47) == 1024)) {{
    ((count__284__47) = 0)/*int*/;
    ((pos__285__47) = 0)/*int*/;
    for ((i__290 = 0)/*int*/; (i__290 < 64); (i__290++)) {{
        (((realBuffer__282__47)[(int)i__290]) = ((float)0.0))/*float*/;
        (((imagBuffer__283__47)[(int)i__290]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__294_95__47(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__288 = 0.0f;/* float */
      float imag_curr__289 = 0.0f;/* float */
      int i__290 = 0;/* int */
      int modPos__291 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__288 = ((float)0.0))/*float*/;
      (imag_curr__289 = ((float)0.0))/*float*/;
      (((realBuffer__282__47)[(int)(63 - (pos__285__47))]) = __pop__47())/*float*/;
      (((imagBuffer__283__47)[(int)(63 - (pos__285__47))]) = __pop__47())/*float*/;
      (modPos__291 = (63 - (pos__285__47)))/*int*/;
      for ((i__290 = 0)/*int*/; (i__290 < 64); (i__290++)) {{
          (real_curr__288 = (real_curr__288 + ((((realBuffer__282__47)[(int)modPos__291]) * ((real_weight__280__47)[(int)i__290])) + (((imagBuffer__283__47)[(int)modPos__291]) * ((imag_weight__281__47)[(int)i__290])))))/*float*/;
          (imag_curr__289 = (imag_curr__289 + ((((imagBuffer__283__47)[(int)modPos__291]) * ((real_weight__280__47)[(int)i__290])) + (((realBuffer__282__47)[(int)modPos__291]) * ((imag_weight__281__47)[(int)i__290])))))/*float*/;
          (modPos__291 = ((modPos__291 + 1) & 63))/*int*/;
        }
      }
      ((pos__285__47) = (((pos__285__47) + 1) & 63))/*int*/;
      __push__47(real_curr__288);
      __push__47(imag_curr__289);
      for ((i__290 = 2)/*int*/; (i__290 < 4); (i__290++)) {{
          __pop__47();
        }
      }
      ((count__284__47) = ((count__284__47) + 2))/*int*/;

      if (((count__284__47) == 1024)) {{
        ((count__284__47) = 0)/*int*/;
        ((pos__285__47) = 0)/*int*/;
        for ((i__290 = 0)/*int*/; (i__290 < 64); (i__290++)) {{
            (((realBuffer__282__47)[(int)i__290]) = ((float)0.0))/*float*/;
            (((imagBuffer__283__47)[(int)i__290]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

// peek: 6 pop: 6 push 2
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_48;
int __counter_48 = 0;
int __steady_48 = 0;
int __tmp_48 = 0;
int __tmp2_48 = 0;
int *__state_flag_48 = NULL;
thread_info *__thread_48 = NULL;



float real_weight__295__48[64] = {0};
float imag_weight__296__48[64] = {0};
float realBuffer__297__48[64] = {0};
float imagBuffer__298__48[64] = {0};
int count__299__48 = 0;
int pos__300__48 = 0;
void save_peek_buffer__48(object_write_buffer *buf);
void load_peek_buffer__48(object_write_buffer *buf);
void save_file_pointer__48(object_write_buffer *buf);
void load_file_pointer__48(object_write_buffer *buf);

 
void init_BeamFirFilter__309_96__48();
inline void check_status__48();

void work_BeamFirFilter__309_96__48(int);


inline float __pop__48() {
float res=BUFFER_47_48[TAIL_47_48];
TAIL_47_48++;
return res;
}

inline float __pop__48(int n) {
float res=BUFFER_47_48[TAIL_47_48];
TAIL_47_48+=n;

return res;
}

inline float __peek__48(int offs) {
return BUFFER_47_48[TAIL_47_48+offs];
}



inline void __push__48(float data) {
BUFFER_48_5[HEAD_48_5]=data;
HEAD_48_5++;
}



 
void init_BeamFirFilter__309_96__48(){
  int idx__307 = 0;/* int */
  int j__308 = 0;/* int */

  ((pos__300__48) = 0)/*int*/;
  for ((j__308 = 0)/*int*/; (j__308 < 64); (j__308++)) {{
      (idx__307 = (j__308 + 1))/*int*/;
      (((real_weight__295__48)[(int)j__308]) = (((float)(sinf(((double)(idx__307))))) / ((float)(idx__307))))/*float*/;
      (((imag_weight__296__48)[(int)j__308]) = (((float)(cosf(((double)(idx__307))))) / ((float)(idx__307))))/*float*/;
    }
  }
}
void save_file_pointer__48(object_write_buffer *buf) {}
void load_file_pointer__48(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__309_96__48__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__303 = 0.0f;/* float */
  float imag_curr__304 = 0.0f;/* float */
  int i__305 = 0;/* int */
  int modPos__306 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__303 = ((float)0.0))/*float*/;
  (imag_curr__304 = ((float)0.0))/*float*/;
  (((realBuffer__297__48)[(int)(63 - (pos__300__48))]) = (*____in++))/*float*/;
  (((imagBuffer__298__48)[(int)(63 - (pos__300__48))]) = (*____in++))/*float*/;
  (modPos__306 = (63 - (pos__300__48)))/*int*/;
  for ((i__305 = 0)/*int*/; (i__305 < 64); (i__305++)) {{
      (real_curr__303 = (real_curr__303 + ((((realBuffer__297__48)[(int)modPos__306]) * ((real_weight__295__48)[(int)i__305])) + (((imagBuffer__298__48)[(int)modPos__306]) * ((imag_weight__296__48)[(int)i__305])))))/*float*/;
      (imag_curr__304 = (imag_curr__304 + ((((imagBuffer__298__48)[(int)modPos__306]) * ((real_weight__295__48)[(int)i__305])) + (((realBuffer__297__48)[(int)modPos__306]) * ((imag_weight__296__48)[(int)i__305])))))/*float*/;
      (modPos__306 = ((modPos__306 + 1) & 63))/*int*/;
    }
  }
  ((pos__300__48) = (((pos__300__48) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__303);
  ((*____out++)=imag_curr__304);
  for ((i__305 = 2)/*int*/; (i__305 < 6); (i__305++)) {{
      (*____in++);
    }
  }
  ((count__299__48) = ((count__299__48) + 3))/*int*/;

  if (((count__299__48) == 512)) {{
    ((count__299__48) = 0)/*int*/;
    ((pos__300__48) = 0)/*int*/;
    for ((i__305 = 0)/*int*/; (i__305 < 64); (i__305++)) {{
        (((realBuffer__297__48)[(int)i__305]) = ((float)0.0))/*float*/;
        (((imagBuffer__298__48)[(int)i__305]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__309_96__48__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__303 = 0.0f;/* float */
  float imag_curr__304 = 0.0f;/* float */
  int i__305 = 0;/* int */
  int modPos__306 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__303 = ((float)0.0))/*float*/;
  (imag_curr__304 = ((float)0.0))/*float*/;
  (((realBuffer__297__48)[(int)(63 - (pos__300__48))]) = (*____in++))/*float*/;
  (((imagBuffer__298__48)[(int)(63 - (pos__300__48))]) = (*____in++))/*float*/;
  (modPos__306 = (63 - (pos__300__48)))/*int*/;
  for ((i__305 = 0)/*int*/; (i__305 < 64); (i__305++)) {{
      (real_curr__303 = (real_curr__303 + ((((realBuffer__297__48)[(int)modPos__306]) * ((real_weight__295__48)[(int)i__305])) + (((imagBuffer__298__48)[(int)modPos__306]) * ((imag_weight__296__48)[(int)i__305])))))/*float*/;
      (imag_curr__304 = (imag_curr__304 + ((((imagBuffer__298__48)[(int)modPos__306]) * ((real_weight__295__48)[(int)i__305])) + (((realBuffer__297__48)[(int)modPos__306]) * ((imag_weight__296__48)[(int)i__305])))))/*float*/;
      (modPos__306 = ((modPos__306 + 1) & 63))/*int*/;
    }
  }
  ((pos__300__48) = (((pos__300__48) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__303);
  ((*____out++)=imag_curr__304);
  for ((i__305 = 2)/*int*/; (i__305 < 6); (i__305++)) {{
      (*____in++);
    }
  }
  ((count__299__48) = ((count__299__48) + 3))/*int*/;

  if (((count__299__48) == 512)) {{
    ((count__299__48) = 0)/*int*/;
    ((pos__300__48) = 0)/*int*/;
    for ((i__305 = 0)/*int*/; (i__305 < 64); (i__305++)) {{
        (((realBuffer__297__48)[(int)i__305]) = ((float)0.0))/*float*/;
        (((imagBuffer__298__48)[(int)i__305]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__309_96__48(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__303 = 0.0f;/* float */
      float imag_curr__304 = 0.0f;/* float */
      int i__305 = 0;/* int */
      int modPos__306 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__303 = ((float)0.0))/*float*/;
      (imag_curr__304 = ((float)0.0))/*float*/;
      (((realBuffer__297__48)[(int)(63 - (pos__300__48))]) = __pop__48())/*float*/;
      (((imagBuffer__298__48)[(int)(63 - (pos__300__48))]) = __pop__48())/*float*/;
      (modPos__306 = (63 - (pos__300__48)))/*int*/;
      for ((i__305 = 0)/*int*/; (i__305 < 64); (i__305++)) {{
          (real_curr__303 = (real_curr__303 + ((((realBuffer__297__48)[(int)modPos__306]) * ((real_weight__295__48)[(int)i__305])) + (((imagBuffer__298__48)[(int)modPos__306]) * ((imag_weight__296__48)[(int)i__305])))))/*float*/;
          (imag_curr__304 = (imag_curr__304 + ((((imagBuffer__298__48)[(int)modPos__306]) * ((real_weight__295__48)[(int)i__305])) + (((realBuffer__297__48)[(int)modPos__306]) * ((imag_weight__296__48)[(int)i__305])))))/*float*/;
          (modPos__306 = ((modPos__306 + 1) & 63))/*int*/;
        }
      }
      ((pos__300__48) = (((pos__300__48) + 1) & 63))/*int*/;
      __push__48(real_curr__303);
      __push__48(imag_curr__304);
      for ((i__305 = 2)/*int*/; (i__305 < 6); (i__305++)) {{
          __pop__48();
        }
      }
      ((count__299__48) = ((count__299__48) + 3))/*int*/;

      if (((count__299__48) == 512)) {{
        ((count__299__48) = 0)/*int*/;
        ((pos__300__48) = 0)/*int*/;
        for ((i__305 = 0)/*int*/; (i__305 < 64); (i__305++)) {{
            (((realBuffer__297__48)[(int)i__305]) = ((float)0.0))/*float*/;
            (((imagBuffer__298__48)[(int)i__305]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

// peek: 1 pop: 1 push 2
// init counts: 0 steady counts: 6

// ClusterFusion isEliminated: false



int __number_of_iterations_49;
int __counter_49 = 0;
int __steady_49 = 0;
int __tmp_49 = 0;
int __tmp2_49 = 0;
int *__state_flag_49 = NULL;
thread_info *__thread_49 = NULL;



int curSample__310__49 = 0;
void save_peek_buffer__49(object_write_buffer *buf);
void load_peek_buffer__49(object_write_buffer *buf);
void save_file_pointer__49(object_write_buffer *buf);
void load_file_pointer__49(object_write_buffer *buf);

 
void init_InputGenerate__313_98__49();
inline void check_status__49();

void work_InputGenerate__313_98__49(int);


inline int __pop__49() {
int res=BUFFER_1_49[TAIL_1_49];
TAIL_1_49++;
return res;
}

inline int __pop__49(int n) {
int res=BUFFER_1_49[TAIL_1_49];
TAIL_1_49+=n;

return res;
}

inline int __peek__49(int offs) {
return BUFFER_1_49[TAIL_1_49+offs];
}



inline void __push__49(float data) {
BUFFER_49_50[HEAD_49_50]=data;
HEAD_49_50++;
}



 
void init_InputGenerate__313_98__49(){
  ((curSample__310__49) = 0)/*int*/;
}
void save_file_pointer__49(object_write_buffer *buf) {}
void load_file_pointer__49(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_InputGenerate__313_98__49__mod(int ____n, int *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRFilter InputGenerate

  (*____in++);

  if ((0 && ((curSample__310__49) == 256))) {{
    ((*____out++)=((float)(sqrtf(((double)(((curSample__310__49) * 9)))))));
    ((*____out++)=(((float)(sqrtf(((double)(((curSample__310__49) * 9)))))) + ((float)1.0)));
  } } else {{
    ((*____out++)=(-((float)(sqrtf(((double)(((curSample__310__49) * 9))))))));
    ((*____out++)=(-(((float)(sqrtf(((double)(((curSample__310__49) * 9)))))) + ((float)1.0))));
  }}
  ((curSample__310__49)++);

  if (((curSample__310__49) >= 1024)) {((curSample__310__49) = 0)/*int*/;}
  // mark end: SIRFilter InputGenerate

}}


void work_InputGenerate__313_98__49__mod2(int ____n, int *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRFilter InputGenerate

  (*____in++);

  if ((0 && ((curSample__310__49) == 256))) {{
    ((*____out++)=((float)(sqrtf(((double)(((curSample__310__49) * 9)))))));
    ((*____out++)=(((float)(sqrtf(((double)(((curSample__310__49) * 9)))))) + ((float)1.0)));
  } } else {{
    ((*____out++)=(-((float)(sqrtf(((double)(((curSample__310__49) * 9))))))));
    ((*____out++)=(-(((float)(sqrtf(((double)(((curSample__310__49) * 9)))))) + ((float)1.0))));
  }}
  ((curSample__310__49)++);

  if (((curSample__310__49) >= 1024)) {((curSample__310__49) = 0)/*int*/;}
  // mark end: SIRFilter InputGenerate

}}


#endif // BUFFER_MERGE


 
void work_InputGenerate__313_98__49(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter InputGenerate

      __pop__49();

      if ((0 && ((curSample__310__49) == 256))) {{
        __push__49(((float)(sqrtf(((double)(((curSample__310__49) * 9)))))));
        __push__49((((float)(sqrtf(((double)(((curSample__310__49) * 9)))))) + ((float)1.0)));
      } } else {{
        __push__49((-((float)(sqrtf(((double)(((curSample__310__49) * 9))))))));
        __push__49((-(((float)(sqrtf(((double)(((curSample__310__49) * 9)))))) + ((float)1.0))));
      }}
      ((curSample__310__49)++);

      if (((curSample__310__49) >= 1024)) {((curSample__310__49) = 0)/*int*/;}
      // mark end: SIRFilter InputGenerate

    }
  }
}

// peek: 4 pop: 4 push 2
// init counts: 0 steady counts: 3

// ClusterFusion isEliminated: false



int __number_of_iterations_50;
int __counter_50 = 0;
int __steady_50 = 0;
int __tmp_50 = 0;
int __tmp2_50 = 0;
int *__state_flag_50 = NULL;
thread_info *__thread_50 = NULL;



float real_weight__314__50[64] = {0};
float imag_weight__315__50[64] = {0};
float realBuffer__316__50[64] = {0};
float imagBuffer__317__50[64] = {0};
int count__318__50 = 0;
int pos__319__50 = 0;
void save_peek_buffer__50(object_write_buffer *buf);
void load_peek_buffer__50(object_write_buffer *buf);
void save_file_pointer__50(object_write_buffer *buf);
void load_file_pointer__50(object_write_buffer *buf);

 
void init_BeamFirFilter__328_99__50();
inline void check_status__50();

void work_BeamFirFilter__328_99__50(int);


inline float __pop__50() {
float res=BUFFER_49_50[TAIL_49_50];
TAIL_49_50++;
return res;
}

inline float __pop__50(int n) {
float res=BUFFER_49_50[TAIL_49_50];
TAIL_49_50+=n;

return res;
}

inline float __peek__50(int offs) {
return BUFFER_49_50[TAIL_49_50+offs];
}



inline void __push__50(float data) {
BUFFER_50_51[HEAD_50_51]=data;
HEAD_50_51++;
}



 
void init_BeamFirFilter__328_99__50(){
  int idx__326 = 0;/* int */
  int j__327 = 0;/* int */

  ((pos__319__50) = 0)/*int*/;
  for ((j__327 = 0)/*int*/; (j__327 < 64); (j__327++)) {{
      (idx__326 = (j__327 + 1))/*int*/;
      (((real_weight__314__50)[(int)j__327]) = (((float)(sinf(((double)(idx__326))))) / ((float)(idx__326))))/*float*/;
      (((imag_weight__315__50)[(int)j__327]) = (((float)(cosf(((double)(idx__326))))) / ((float)(idx__326))))/*float*/;
    }
  }
}
void save_file_pointer__50(object_write_buffer *buf) {}
void load_file_pointer__50(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__328_99__50__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__322 = 0.0f;/* float */
  float imag_curr__323 = 0.0f;/* float */
  int i__324 = 0;/* int */
  int modPos__325 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__322 = ((float)0.0))/*float*/;
  (imag_curr__323 = ((float)0.0))/*float*/;
  (((realBuffer__316__50)[(int)(63 - (pos__319__50))]) = (*____in++))/*float*/;
  (((imagBuffer__317__50)[(int)(63 - (pos__319__50))]) = (*____in++))/*float*/;
  (modPos__325 = (63 - (pos__319__50)))/*int*/;
  for ((i__324 = 0)/*int*/; (i__324 < 64); (i__324++)) {{
      (real_curr__322 = (real_curr__322 + ((((realBuffer__316__50)[(int)modPos__325]) * ((real_weight__314__50)[(int)i__324])) + (((imagBuffer__317__50)[(int)modPos__325]) * ((imag_weight__315__50)[(int)i__324])))))/*float*/;
      (imag_curr__323 = (imag_curr__323 + ((((imagBuffer__317__50)[(int)modPos__325]) * ((real_weight__314__50)[(int)i__324])) + (((realBuffer__316__50)[(int)modPos__325]) * ((imag_weight__315__50)[(int)i__324])))))/*float*/;
      (modPos__325 = ((modPos__325 + 1) & 63))/*int*/;
    }
  }
  ((pos__319__50) = (((pos__319__50) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__322);
  ((*____out++)=imag_curr__323);
  for ((i__324 = 2)/*int*/; (i__324 < 4); (i__324++)) {{
      (*____in++);
    }
  }
  ((count__318__50) = ((count__318__50) + 2))/*int*/;

  if (((count__318__50) == 1024)) {{
    ((count__318__50) = 0)/*int*/;
    ((pos__319__50) = 0)/*int*/;
    for ((i__324 = 0)/*int*/; (i__324 < 64); (i__324++)) {{
        (((realBuffer__316__50)[(int)i__324]) = ((float)0.0))/*float*/;
        (((imagBuffer__317__50)[(int)i__324]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__328_99__50__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__322 = 0.0f;/* float */
  float imag_curr__323 = 0.0f;/* float */
  int i__324 = 0;/* int */
  int modPos__325 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__322 = ((float)0.0))/*float*/;
  (imag_curr__323 = ((float)0.0))/*float*/;
  (((realBuffer__316__50)[(int)(63 - (pos__319__50))]) = (*____in++))/*float*/;
  (((imagBuffer__317__50)[(int)(63 - (pos__319__50))]) = (*____in++))/*float*/;
  (modPos__325 = (63 - (pos__319__50)))/*int*/;
  for ((i__324 = 0)/*int*/; (i__324 < 64); (i__324++)) {{
      (real_curr__322 = (real_curr__322 + ((((realBuffer__316__50)[(int)modPos__325]) * ((real_weight__314__50)[(int)i__324])) + (((imagBuffer__317__50)[(int)modPos__325]) * ((imag_weight__315__50)[(int)i__324])))))/*float*/;
      (imag_curr__323 = (imag_curr__323 + ((((imagBuffer__317__50)[(int)modPos__325]) * ((real_weight__314__50)[(int)i__324])) + (((realBuffer__316__50)[(int)modPos__325]) * ((imag_weight__315__50)[(int)i__324])))))/*float*/;
      (modPos__325 = ((modPos__325 + 1) & 63))/*int*/;
    }
  }
  ((pos__319__50) = (((pos__319__50) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__322);
  ((*____out++)=imag_curr__323);
  for ((i__324 = 2)/*int*/; (i__324 < 4); (i__324++)) {{
      (*____in++);
    }
  }
  ((count__318__50) = ((count__318__50) + 2))/*int*/;

  if (((count__318__50) == 1024)) {{
    ((count__318__50) = 0)/*int*/;
    ((pos__319__50) = 0)/*int*/;
    for ((i__324 = 0)/*int*/; (i__324 < 64); (i__324++)) {{
        (((realBuffer__316__50)[(int)i__324]) = ((float)0.0))/*float*/;
        (((imagBuffer__317__50)[(int)i__324]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__328_99__50(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__322 = 0.0f;/* float */
      float imag_curr__323 = 0.0f;/* float */
      int i__324 = 0;/* int */
      int modPos__325 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__322 = ((float)0.0))/*float*/;
      (imag_curr__323 = ((float)0.0))/*float*/;
      (((realBuffer__316__50)[(int)(63 - (pos__319__50))]) = __pop__50())/*float*/;
      (((imagBuffer__317__50)[(int)(63 - (pos__319__50))]) = __pop__50())/*float*/;
      (modPos__325 = (63 - (pos__319__50)))/*int*/;
      for ((i__324 = 0)/*int*/; (i__324 < 64); (i__324++)) {{
          (real_curr__322 = (real_curr__322 + ((((realBuffer__316__50)[(int)modPos__325]) * ((real_weight__314__50)[(int)i__324])) + (((imagBuffer__317__50)[(int)modPos__325]) * ((imag_weight__315__50)[(int)i__324])))))/*float*/;
          (imag_curr__323 = (imag_curr__323 + ((((imagBuffer__317__50)[(int)modPos__325]) * ((real_weight__314__50)[(int)i__324])) + (((realBuffer__316__50)[(int)modPos__325]) * ((imag_weight__315__50)[(int)i__324])))))/*float*/;
          (modPos__325 = ((modPos__325 + 1) & 63))/*int*/;
        }
      }
      ((pos__319__50) = (((pos__319__50) + 1) & 63))/*int*/;
      __push__50(real_curr__322);
      __push__50(imag_curr__323);
      for ((i__324 = 2)/*int*/; (i__324 < 4); (i__324++)) {{
          __pop__50();
        }
      }
      ((count__318__50) = ((count__318__50) + 2))/*int*/;

      if (((count__318__50) == 1024)) {{
        ((count__318__50) = 0)/*int*/;
        ((pos__319__50) = 0)/*int*/;
        for ((i__324 = 0)/*int*/; (i__324 < 64); (i__324++)) {{
            (((realBuffer__316__50)[(int)i__324]) = ((float)0.0))/*float*/;
            (((imagBuffer__317__50)[(int)i__324]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

// peek: 6 pop: 6 push 2
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_51;
int __counter_51 = 0;
int __steady_51 = 0;
int __tmp_51 = 0;
int __tmp2_51 = 0;
int *__state_flag_51 = NULL;
thread_info *__thread_51 = NULL;



float real_weight__329__51[64] = {0};
float imag_weight__330__51[64] = {0};
float realBuffer__331__51[64] = {0};
float imagBuffer__332__51[64] = {0};
int count__333__51 = 0;
int pos__334__51 = 0;
void save_peek_buffer__51(object_write_buffer *buf);
void load_peek_buffer__51(object_write_buffer *buf);
void save_file_pointer__51(object_write_buffer *buf);
void load_file_pointer__51(object_write_buffer *buf);

 
void init_BeamFirFilter__343_100__51();
inline void check_status__51();

void work_BeamFirFilter__343_100__51(int);


inline float __pop__51() {
float res=BUFFER_50_51[TAIL_50_51];
TAIL_50_51++;
return res;
}

inline float __pop__51(int n) {
float res=BUFFER_50_51[TAIL_50_51];
TAIL_50_51+=n;

return res;
}

inline float __peek__51(int offs) {
return BUFFER_50_51[TAIL_50_51+offs];
}



inline void __push__51(float data) {
BUFFER_51_5[HEAD_51_5]=data;
HEAD_51_5++;
}



 
void init_BeamFirFilter__343_100__51(){
  int idx__341 = 0;/* int */
  int j__342 = 0;/* int */

  ((pos__334__51) = 0)/*int*/;
  for ((j__342 = 0)/*int*/; (j__342 < 64); (j__342++)) {{
      (idx__341 = (j__342 + 1))/*int*/;
      (((real_weight__329__51)[(int)j__342]) = (((float)(sinf(((double)(idx__341))))) / ((float)(idx__341))))/*float*/;
      (((imag_weight__330__51)[(int)j__342]) = (((float)(cosf(((double)(idx__341))))) / ((float)(idx__341))))/*float*/;
    }
  }
}
void save_file_pointer__51(object_write_buffer *buf) {}
void load_file_pointer__51(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__343_100__51__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__337 = 0.0f;/* float */
  float imag_curr__338 = 0.0f;/* float */
  int i__339 = 0;/* int */
  int modPos__340 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__337 = ((float)0.0))/*float*/;
  (imag_curr__338 = ((float)0.0))/*float*/;
  (((realBuffer__331__51)[(int)(63 - (pos__334__51))]) = (*____in++))/*float*/;
  (((imagBuffer__332__51)[(int)(63 - (pos__334__51))]) = (*____in++))/*float*/;
  (modPos__340 = (63 - (pos__334__51)))/*int*/;
  for ((i__339 = 0)/*int*/; (i__339 < 64); (i__339++)) {{
      (real_curr__337 = (real_curr__337 + ((((realBuffer__331__51)[(int)modPos__340]) * ((real_weight__329__51)[(int)i__339])) + (((imagBuffer__332__51)[(int)modPos__340]) * ((imag_weight__330__51)[(int)i__339])))))/*float*/;
      (imag_curr__338 = (imag_curr__338 + ((((imagBuffer__332__51)[(int)modPos__340]) * ((real_weight__329__51)[(int)i__339])) + (((realBuffer__331__51)[(int)modPos__340]) * ((imag_weight__330__51)[(int)i__339])))))/*float*/;
      (modPos__340 = ((modPos__340 + 1) & 63))/*int*/;
    }
  }
  ((pos__334__51) = (((pos__334__51) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__337);
  ((*____out++)=imag_curr__338);
  for ((i__339 = 2)/*int*/; (i__339 < 6); (i__339++)) {{
      (*____in++);
    }
  }
  ((count__333__51) = ((count__333__51) + 3))/*int*/;

  if (((count__333__51) == 512)) {{
    ((count__333__51) = 0)/*int*/;
    ((pos__334__51) = 0)/*int*/;
    for ((i__339 = 0)/*int*/; (i__339 < 64); (i__339++)) {{
        (((realBuffer__331__51)[(int)i__339]) = ((float)0.0))/*float*/;
        (((imagBuffer__332__51)[(int)i__339]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__343_100__51__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__337 = 0.0f;/* float */
  float imag_curr__338 = 0.0f;/* float */
  int i__339 = 0;/* int */
  int modPos__340 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__337 = ((float)0.0))/*float*/;
  (imag_curr__338 = ((float)0.0))/*float*/;
  (((realBuffer__331__51)[(int)(63 - (pos__334__51))]) = (*____in++))/*float*/;
  (((imagBuffer__332__51)[(int)(63 - (pos__334__51))]) = (*____in++))/*float*/;
  (modPos__340 = (63 - (pos__334__51)))/*int*/;
  for ((i__339 = 0)/*int*/; (i__339 < 64); (i__339++)) {{
      (real_curr__337 = (real_curr__337 + ((((realBuffer__331__51)[(int)modPos__340]) * ((real_weight__329__51)[(int)i__339])) + (((imagBuffer__332__51)[(int)modPos__340]) * ((imag_weight__330__51)[(int)i__339])))))/*float*/;
      (imag_curr__338 = (imag_curr__338 + ((((imagBuffer__332__51)[(int)modPos__340]) * ((real_weight__329__51)[(int)i__339])) + (((realBuffer__331__51)[(int)modPos__340]) * ((imag_weight__330__51)[(int)i__339])))))/*float*/;
      (modPos__340 = ((modPos__340 + 1) & 63))/*int*/;
    }
  }
  ((pos__334__51) = (((pos__334__51) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__337);
  ((*____out++)=imag_curr__338);
  for ((i__339 = 2)/*int*/; (i__339 < 6); (i__339++)) {{
      (*____in++);
    }
  }
  ((count__333__51) = ((count__333__51) + 3))/*int*/;

  if (((count__333__51) == 512)) {{
    ((count__333__51) = 0)/*int*/;
    ((pos__334__51) = 0)/*int*/;
    for ((i__339 = 0)/*int*/; (i__339 < 64); (i__339++)) {{
        (((realBuffer__331__51)[(int)i__339]) = ((float)0.0))/*float*/;
        (((imagBuffer__332__51)[(int)i__339]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__343_100__51(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__337 = 0.0f;/* float */
      float imag_curr__338 = 0.0f;/* float */
      int i__339 = 0;/* int */
      int modPos__340 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__337 = ((float)0.0))/*float*/;
      (imag_curr__338 = ((float)0.0))/*float*/;
      (((realBuffer__331__51)[(int)(63 - (pos__334__51))]) = __pop__51())/*float*/;
      (((imagBuffer__332__51)[(int)(63 - (pos__334__51))]) = __pop__51())/*float*/;
      (modPos__340 = (63 - (pos__334__51)))/*int*/;
      for ((i__339 = 0)/*int*/; (i__339 < 64); (i__339++)) {{
          (real_curr__337 = (real_curr__337 + ((((realBuffer__331__51)[(int)modPos__340]) * ((real_weight__329__51)[(int)i__339])) + (((imagBuffer__332__51)[(int)modPos__340]) * ((imag_weight__330__51)[(int)i__339])))))/*float*/;
          (imag_curr__338 = (imag_curr__338 + ((((imagBuffer__332__51)[(int)modPos__340]) * ((real_weight__329__51)[(int)i__339])) + (((realBuffer__331__51)[(int)modPos__340]) * ((imag_weight__330__51)[(int)i__339])))))/*float*/;
          (modPos__340 = ((modPos__340 + 1) & 63))/*int*/;
        }
      }
      ((pos__334__51) = (((pos__334__51) + 1) & 63))/*int*/;
      __push__51(real_curr__337);
      __push__51(imag_curr__338);
      for ((i__339 = 2)/*int*/; (i__339 < 6); (i__339++)) {{
          __pop__51();
        }
      }
      ((count__333__51) = ((count__333__51) + 3))/*int*/;

      if (((count__333__51) == 512)) {{
        ((count__333__51) = 0)/*int*/;
        ((pos__334__51) = 0)/*int*/;
        for ((i__339 = 0)/*int*/; (i__339 < 64); (i__339++)) {{
            (((realBuffer__331__51)[(int)i__339]) = ((float)0.0))/*float*/;
            (((imagBuffer__332__51)[(int)i__339]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

// peek: 1 pop: 1 push 2
// init counts: 0 steady counts: 6

// ClusterFusion isEliminated: false



int __number_of_iterations_52;
int __counter_52 = 0;
int __steady_52 = 0;
int __tmp_52 = 0;
int __tmp2_52 = 0;
int *__state_flag_52 = NULL;
thread_info *__thread_52 = NULL;



int curSample__344__52 = 0;
void save_peek_buffer__52(object_write_buffer *buf);
void load_peek_buffer__52(object_write_buffer *buf);
void save_file_pointer__52(object_write_buffer *buf);
void load_file_pointer__52(object_write_buffer *buf);

 
void init_InputGenerate__347_102__52();
inline void check_status__52();

void work_InputGenerate__347_102__52(int);


inline int __pop__52() {
int res=BUFFER_1_52[TAIL_1_52];
TAIL_1_52++;
return res;
}

inline int __pop__52(int n) {
int res=BUFFER_1_52[TAIL_1_52];
TAIL_1_52+=n;

return res;
}

inline int __peek__52(int offs) {
return BUFFER_1_52[TAIL_1_52+offs];
}



inline void __push__52(float data) {
BUFFER_52_53[HEAD_52_53]=data;
HEAD_52_53++;
}



 
void init_InputGenerate__347_102__52(){
  ((curSample__344__52) = 0)/*int*/;
}
void save_file_pointer__52(object_write_buffer *buf) {}
void load_file_pointer__52(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_InputGenerate__347_102__52__mod(int ____n, int *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRFilter InputGenerate

  (*____in++);

  if ((0 && ((curSample__344__52) == 256))) {{
    ((*____out++)=((float)(sqrtf(((double)(((curSample__344__52) * 10)))))));
    ((*____out++)=(((float)(sqrtf(((double)(((curSample__344__52) * 10)))))) + ((float)1.0)));
  } } else {{
    ((*____out++)=(-((float)(sqrtf(((double)(((curSample__344__52) * 10))))))));
    ((*____out++)=(-(((float)(sqrtf(((double)(((curSample__344__52) * 10)))))) + ((float)1.0))));
  }}
  ((curSample__344__52)++);

  if (((curSample__344__52) >= 1024)) {((curSample__344__52) = 0)/*int*/;}
  // mark end: SIRFilter InputGenerate

}}


void work_InputGenerate__347_102__52__mod2(int ____n, int *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRFilter InputGenerate

  (*____in++);

  if ((0 && ((curSample__344__52) == 256))) {{
    ((*____out++)=((float)(sqrtf(((double)(((curSample__344__52) * 10)))))));
    ((*____out++)=(((float)(sqrtf(((double)(((curSample__344__52) * 10)))))) + ((float)1.0)));
  } } else {{
    ((*____out++)=(-((float)(sqrtf(((double)(((curSample__344__52) * 10))))))));
    ((*____out++)=(-(((float)(sqrtf(((double)(((curSample__344__52) * 10)))))) + ((float)1.0))));
  }}
  ((curSample__344__52)++);

  if (((curSample__344__52) >= 1024)) {((curSample__344__52) = 0)/*int*/;}
  // mark end: SIRFilter InputGenerate

}}


#endif // BUFFER_MERGE


 
void work_InputGenerate__347_102__52(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter InputGenerate

      __pop__52();

      if ((0 && ((curSample__344__52) == 256))) {{
        __push__52(((float)(sqrtf(((double)(((curSample__344__52) * 10)))))));
        __push__52((((float)(sqrtf(((double)(((curSample__344__52) * 10)))))) + ((float)1.0)));
      } } else {{
        __push__52((-((float)(sqrtf(((double)(((curSample__344__52) * 10))))))));
        __push__52((-(((float)(sqrtf(((double)(((curSample__344__52) * 10)))))) + ((float)1.0))));
      }}
      ((curSample__344__52)++);

      if (((curSample__344__52) >= 1024)) {((curSample__344__52) = 0)/*int*/;}
      // mark end: SIRFilter InputGenerate

    }
  }
}

// peek: 4 pop: 4 push 2
// init counts: 0 steady counts: 3

// ClusterFusion isEliminated: false



int __number_of_iterations_53;
int __counter_53 = 0;
int __steady_53 = 0;
int __tmp_53 = 0;
int __tmp2_53 = 0;
int *__state_flag_53 = NULL;
thread_info *__thread_53 = NULL;



float real_weight__348__53[64] = {0};
float imag_weight__349__53[64] = {0};
float realBuffer__350__53[64] = {0};
float imagBuffer__351__53[64] = {0};
int count__352__53 = 0;
int pos__353__53 = 0;
void save_peek_buffer__53(object_write_buffer *buf);
void load_peek_buffer__53(object_write_buffer *buf);
void save_file_pointer__53(object_write_buffer *buf);
void load_file_pointer__53(object_write_buffer *buf);

 
void init_BeamFirFilter__362_103__53();
inline void check_status__53();

void work_BeamFirFilter__362_103__53(int);


inline float __pop__53() {
float res=BUFFER_52_53[TAIL_52_53];
TAIL_52_53++;
return res;
}

inline float __pop__53(int n) {
float res=BUFFER_52_53[TAIL_52_53];
TAIL_52_53+=n;

return res;
}

inline float __peek__53(int offs) {
return BUFFER_52_53[TAIL_52_53+offs];
}



inline void __push__53(float data) {
BUFFER_53_54[HEAD_53_54]=data;
HEAD_53_54++;
}



 
void init_BeamFirFilter__362_103__53(){
  int idx__360 = 0;/* int */
  int j__361 = 0;/* int */

  ((pos__353__53) = 0)/*int*/;
  for ((j__361 = 0)/*int*/; (j__361 < 64); (j__361++)) {{
      (idx__360 = (j__361 + 1))/*int*/;
      (((real_weight__348__53)[(int)j__361]) = (((float)(sinf(((double)(idx__360))))) / ((float)(idx__360))))/*float*/;
      (((imag_weight__349__53)[(int)j__361]) = (((float)(cosf(((double)(idx__360))))) / ((float)(idx__360))))/*float*/;
    }
  }
}
void save_file_pointer__53(object_write_buffer *buf) {}
void load_file_pointer__53(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__362_103__53__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__356 = 0.0f;/* float */
  float imag_curr__357 = 0.0f;/* float */
  int i__358 = 0;/* int */
  int modPos__359 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__356 = ((float)0.0))/*float*/;
  (imag_curr__357 = ((float)0.0))/*float*/;
  (((realBuffer__350__53)[(int)(63 - (pos__353__53))]) = (*____in++))/*float*/;
  (((imagBuffer__351__53)[(int)(63 - (pos__353__53))]) = (*____in++))/*float*/;
  (modPos__359 = (63 - (pos__353__53)))/*int*/;
  for ((i__358 = 0)/*int*/; (i__358 < 64); (i__358++)) {{
      (real_curr__356 = (real_curr__356 + ((((realBuffer__350__53)[(int)modPos__359]) * ((real_weight__348__53)[(int)i__358])) + (((imagBuffer__351__53)[(int)modPos__359]) * ((imag_weight__349__53)[(int)i__358])))))/*float*/;
      (imag_curr__357 = (imag_curr__357 + ((((imagBuffer__351__53)[(int)modPos__359]) * ((real_weight__348__53)[(int)i__358])) + (((realBuffer__350__53)[(int)modPos__359]) * ((imag_weight__349__53)[(int)i__358])))))/*float*/;
      (modPos__359 = ((modPos__359 + 1) & 63))/*int*/;
    }
  }
  ((pos__353__53) = (((pos__353__53) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__356);
  ((*____out++)=imag_curr__357);
  for ((i__358 = 2)/*int*/; (i__358 < 4); (i__358++)) {{
      (*____in++);
    }
  }
  ((count__352__53) = ((count__352__53) + 2))/*int*/;

  if (((count__352__53) == 1024)) {{
    ((count__352__53) = 0)/*int*/;
    ((pos__353__53) = 0)/*int*/;
    for ((i__358 = 0)/*int*/; (i__358 < 64); (i__358++)) {{
        (((realBuffer__350__53)[(int)i__358]) = ((float)0.0))/*float*/;
        (((imagBuffer__351__53)[(int)i__358]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__362_103__53__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__356 = 0.0f;/* float */
  float imag_curr__357 = 0.0f;/* float */
  int i__358 = 0;/* int */
  int modPos__359 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__356 = ((float)0.0))/*float*/;
  (imag_curr__357 = ((float)0.0))/*float*/;
  (((realBuffer__350__53)[(int)(63 - (pos__353__53))]) = (*____in++))/*float*/;
  (((imagBuffer__351__53)[(int)(63 - (pos__353__53))]) = (*____in++))/*float*/;
  (modPos__359 = (63 - (pos__353__53)))/*int*/;
  for ((i__358 = 0)/*int*/; (i__358 < 64); (i__358++)) {{
      (real_curr__356 = (real_curr__356 + ((((realBuffer__350__53)[(int)modPos__359]) * ((real_weight__348__53)[(int)i__358])) + (((imagBuffer__351__53)[(int)modPos__359]) * ((imag_weight__349__53)[(int)i__358])))))/*float*/;
      (imag_curr__357 = (imag_curr__357 + ((((imagBuffer__351__53)[(int)modPos__359]) * ((real_weight__348__53)[(int)i__358])) + (((realBuffer__350__53)[(int)modPos__359]) * ((imag_weight__349__53)[(int)i__358])))))/*float*/;
      (modPos__359 = ((modPos__359 + 1) & 63))/*int*/;
    }
  }
  ((pos__353__53) = (((pos__353__53) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__356);
  ((*____out++)=imag_curr__357);
  for ((i__358 = 2)/*int*/; (i__358 < 4); (i__358++)) {{
      (*____in++);
    }
  }
  ((count__352__53) = ((count__352__53) + 2))/*int*/;

  if (((count__352__53) == 1024)) {{
    ((count__352__53) = 0)/*int*/;
    ((pos__353__53) = 0)/*int*/;
    for ((i__358 = 0)/*int*/; (i__358 < 64); (i__358++)) {{
        (((realBuffer__350__53)[(int)i__358]) = ((float)0.0))/*float*/;
        (((imagBuffer__351__53)[(int)i__358]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__362_103__53(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__356 = 0.0f;/* float */
      float imag_curr__357 = 0.0f;/* float */
      int i__358 = 0;/* int */
      int modPos__359 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__356 = ((float)0.0))/*float*/;
      (imag_curr__357 = ((float)0.0))/*float*/;
      (((realBuffer__350__53)[(int)(63 - (pos__353__53))]) = __pop__53())/*float*/;
      (((imagBuffer__351__53)[(int)(63 - (pos__353__53))]) = __pop__53())/*float*/;
      (modPos__359 = (63 - (pos__353__53)))/*int*/;
      for ((i__358 = 0)/*int*/; (i__358 < 64); (i__358++)) {{
          (real_curr__356 = (real_curr__356 + ((((realBuffer__350__53)[(int)modPos__359]) * ((real_weight__348__53)[(int)i__358])) + (((imagBuffer__351__53)[(int)modPos__359]) * ((imag_weight__349__53)[(int)i__358])))))/*float*/;
          (imag_curr__357 = (imag_curr__357 + ((((imagBuffer__351__53)[(int)modPos__359]) * ((real_weight__348__53)[(int)i__358])) + (((realBuffer__350__53)[(int)modPos__359]) * ((imag_weight__349__53)[(int)i__358])))))/*float*/;
          (modPos__359 = ((modPos__359 + 1) & 63))/*int*/;
        }
      }
      ((pos__353__53) = (((pos__353__53) + 1) & 63))/*int*/;
      __push__53(real_curr__356);
      __push__53(imag_curr__357);
      for ((i__358 = 2)/*int*/; (i__358 < 4); (i__358++)) {{
          __pop__53();
        }
      }
      ((count__352__53) = ((count__352__53) + 2))/*int*/;

      if (((count__352__53) == 1024)) {{
        ((count__352__53) = 0)/*int*/;
        ((pos__353__53) = 0)/*int*/;
        for ((i__358 = 0)/*int*/; (i__358 < 64); (i__358++)) {{
            (((realBuffer__350__53)[(int)i__358]) = ((float)0.0))/*float*/;
            (((imagBuffer__351__53)[(int)i__358]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

// peek: 6 pop: 6 push 2
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_54;
int __counter_54 = 0;
int __steady_54 = 0;
int __tmp_54 = 0;
int __tmp2_54 = 0;
int *__state_flag_54 = NULL;
thread_info *__thread_54 = NULL;



float real_weight__363__54[64] = {0};
float imag_weight__364__54[64] = {0};
float realBuffer__365__54[64] = {0};
float imagBuffer__366__54[64] = {0};
int count__367__54 = 0;
int pos__368__54 = 0;
void save_peek_buffer__54(object_write_buffer *buf);
void load_peek_buffer__54(object_write_buffer *buf);
void save_file_pointer__54(object_write_buffer *buf);
void load_file_pointer__54(object_write_buffer *buf);

 
void init_BeamFirFilter__377_104__54();
inline void check_status__54();

void work_BeamFirFilter__377_104__54(int);


inline float __pop__54() {
float res=BUFFER_53_54[TAIL_53_54];
TAIL_53_54++;
return res;
}

inline float __pop__54(int n) {
float res=BUFFER_53_54[TAIL_53_54];
TAIL_53_54+=n;

return res;
}

inline float __peek__54(int offs) {
return BUFFER_53_54[TAIL_53_54+offs];
}



inline void __push__54(float data) {
BUFFER_54_5[HEAD_54_5]=data;
HEAD_54_5++;
}



 
void init_BeamFirFilter__377_104__54(){
  int idx__375 = 0;/* int */
  int j__376 = 0;/* int */

  ((pos__368__54) = 0)/*int*/;
  for ((j__376 = 0)/*int*/; (j__376 < 64); (j__376++)) {{
      (idx__375 = (j__376 + 1))/*int*/;
      (((real_weight__363__54)[(int)j__376]) = (((float)(sinf(((double)(idx__375))))) / ((float)(idx__375))))/*float*/;
      (((imag_weight__364__54)[(int)j__376]) = (((float)(cosf(((double)(idx__375))))) / ((float)(idx__375))))/*float*/;
    }
  }
}
void save_file_pointer__54(object_write_buffer *buf) {}
void load_file_pointer__54(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__377_104__54__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__371 = 0.0f;/* float */
  float imag_curr__372 = 0.0f;/* float */
  int i__373 = 0;/* int */
  int modPos__374 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__371 = ((float)0.0))/*float*/;
  (imag_curr__372 = ((float)0.0))/*float*/;
  (((realBuffer__365__54)[(int)(63 - (pos__368__54))]) = (*____in++))/*float*/;
  (((imagBuffer__366__54)[(int)(63 - (pos__368__54))]) = (*____in++))/*float*/;
  (modPos__374 = (63 - (pos__368__54)))/*int*/;
  for ((i__373 = 0)/*int*/; (i__373 < 64); (i__373++)) {{
      (real_curr__371 = (real_curr__371 + ((((realBuffer__365__54)[(int)modPos__374]) * ((real_weight__363__54)[(int)i__373])) + (((imagBuffer__366__54)[(int)modPos__374]) * ((imag_weight__364__54)[(int)i__373])))))/*float*/;
      (imag_curr__372 = (imag_curr__372 + ((((imagBuffer__366__54)[(int)modPos__374]) * ((real_weight__363__54)[(int)i__373])) + (((realBuffer__365__54)[(int)modPos__374]) * ((imag_weight__364__54)[(int)i__373])))))/*float*/;
      (modPos__374 = ((modPos__374 + 1) & 63))/*int*/;
    }
  }
  ((pos__368__54) = (((pos__368__54) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__371);
  ((*____out++)=imag_curr__372);
  for ((i__373 = 2)/*int*/; (i__373 < 6); (i__373++)) {{
      (*____in++);
    }
  }
  ((count__367__54) = ((count__367__54) + 3))/*int*/;

  if (((count__367__54) == 512)) {{
    ((count__367__54) = 0)/*int*/;
    ((pos__368__54) = 0)/*int*/;
    for ((i__373 = 0)/*int*/; (i__373 < 64); (i__373++)) {{
        (((realBuffer__365__54)[(int)i__373]) = ((float)0.0))/*float*/;
        (((imagBuffer__366__54)[(int)i__373]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__377_104__54__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__371 = 0.0f;/* float */
  float imag_curr__372 = 0.0f;/* float */
  int i__373 = 0;/* int */
  int modPos__374 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__371 = ((float)0.0))/*float*/;
  (imag_curr__372 = ((float)0.0))/*float*/;
  (((realBuffer__365__54)[(int)(63 - (pos__368__54))]) = (*____in++))/*float*/;
  (((imagBuffer__366__54)[(int)(63 - (pos__368__54))]) = (*____in++))/*float*/;
  (modPos__374 = (63 - (pos__368__54)))/*int*/;
  for ((i__373 = 0)/*int*/; (i__373 < 64); (i__373++)) {{
      (real_curr__371 = (real_curr__371 + ((((realBuffer__365__54)[(int)modPos__374]) * ((real_weight__363__54)[(int)i__373])) + (((imagBuffer__366__54)[(int)modPos__374]) * ((imag_weight__364__54)[(int)i__373])))))/*float*/;
      (imag_curr__372 = (imag_curr__372 + ((((imagBuffer__366__54)[(int)modPos__374]) * ((real_weight__363__54)[(int)i__373])) + (((realBuffer__365__54)[(int)modPos__374]) * ((imag_weight__364__54)[(int)i__373])))))/*float*/;
      (modPos__374 = ((modPos__374 + 1) & 63))/*int*/;
    }
  }
  ((pos__368__54) = (((pos__368__54) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__371);
  ((*____out++)=imag_curr__372);
  for ((i__373 = 2)/*int*/; (i__373 < 6); (i__373++)) {{
      (*____in++);
    }
  }
  ((count__367__54) = ((count__367__54) + 3))/*int*/;

  if (((count__367__54) == 512)) {{
    ((count__367__54) = 0)/*int*/;
    ((pos__368__54) = 0)/*int*/;
    for ((i__373 = 0)/*int*/; (i__373 < 64); (i__373++)) {{
        (((realBuffer__365__54)[(int)i__373]) = ((float)0.0))/*float*/;
        (((imagBuffer__366__54)[(int)i__373]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__377_104__54(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__371 = 0.0f;/* float */
      float imag_curr__372 = 0.0f;/* float */
      int i__373 = 0;/* int */
      int modPos__374 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__371 = ((float)0.0))/*float*/;
      (imag_curr__372 = ((float)0.0))/*float*/;
      (((realBuffer__365__54)[(int)(63 - (pos__368__54))]) = __pop__54())/*float*/;
      (((imagBuffer__366__54)[(int)(63 - (pos__368__54))]) = __pop__54())/*float*/;
      (modPos__374 = (63 - (pos__368__54)))/*int*/;
      for ((i__373 = 0)/*int*/; (i__373 < 64); (i__373++)) {{
          (real_curr__371 = (real_curr__371 + ((((realBuffer__365__54)[(int)modPos__374]) * ((real_weight__363__54)[(int)i__373])) + (((imagBuffer__366__54)[(int)modPos__374]) * ((imag_weight__364__54)[(int)i__373])))))/*float*/;
          (imag_curr__372 = (imag_curr__372 + ((((imagBuffer__366__54)[(int)modPos__374]) * ((real_weight__363__54)[(int)i__373])) + (((realBuffer__365__54)[(int)modPos__374]) * ((imag_weight__364__54)[(int)i__373])))))/*float*/;
          (modPos__374 = ((modPos__374 + 1) & 63))/*int*/;
        }
      }
      ((pos__368__54) = (((pos__368__54) + 1) & 63))/*int*/;
      __push__54(real_curr__371);
      __push__54(imag_curr__372);
      for ((i__373 = 2)/*int*/; (i__373 < 6); (i__373++)) {{
          __pop__54();
        }
      }
      ((count__367__54) = ((count__367__54) + 3))/*int*/;

      if (((count__367__54) == 512)) {{
        ((count__367__54) = 0)/*int*/;
        ((pos__368__54) = 0)/*int*/;
        for ((i__373 = 0)/*int*/; (i__373 < 64); (i__373++)) {{
            (((realBuffer__365__54)[(int)i__373]) = ((float)0.0))/*float*/;
            (((imagBuffer__366__54)[(int)i__373]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

// peek: 1 pop: 1 push 2
// init counts: 0 steady counts: 6

// ClusterFusion isEliminated: false



int __number_of_iterations_55;
int __counter_55 = 0;
int __steady_55 = 0;
int __tmp_55 = 0;
int __tmp2_55 = 0;
int *__state_flag_55 = NULL;
thread_info *__thread_55 = NULL;



int curSample__378__55 = 0;
void save_peek_buffer__55(object_write_buffer *buf);
void load_peek_buffer__55(object_write_buffer *buf);
void save_file_pointer__55(object_write_buffer *buf);
void load_file_pointer__55(object_write_buffer *buf);

 
void init_InputGenerate__381_106__55();
inline void check_status__55();

void work_InputGenerate__381_106__55(int);


inline int __pop__55() {
int res=BUFFER_1_55[TAIL_1_55];
TAIL_1_55++;
return res;
}

inline int __pop__55(int n) {
int res=BUFFER_1_55[TAIL_1_55];
TAIL_1_55+=n;

return res;
}

inline int __peek__55(int offs) {
return BUFFER_1_55[TAIL_1_55+offs];
}



inline void __push__55(float data) {
BUFFER_55_56[HEAD_55_56]=data;
HEAD_55_56++;
}



 
void init_InputGenerate__381_106__55(){
  ((curSample__378__55) = 0)/*int*/;
}
void save_file_pointer__55(object_write_buffer *buf) {}
void load_file_pointer__55(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_InputGenerate__381_106__55__mod(int ____n, int *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  // mark begin: SIRFilter InputGenerate

  (*____in++);

  if ((0 && ((curSample__378__55) == 256))) {{
    ((*____out++)=((float)(sqrtf(((double)(((curSample__378__55) * 11)))))));
    ((*____out++)=(((float)(sqrtf(((double)(((curSample__378__55) * 11)))))) + ((float)1.0)));
  } } else {{
    ((*____out++)=(-((float)(sqrtf(((double)(((curSample__378__55) * 11))))))));
    ((*____out++)=(-(((float)(sqrtf(((double)(((curSample__378__55) * 11)))))) + ((float)1.0))));
  }}
  ((curSample__378__55)++);

  if (((curSample__378__55) >= 1024)) {((curSample__378__55) = 0)/*int*/;}
  // mark end: SIRFilter InputGenerate

}}


void work_InputGenerate__381_106__55__mod2(int ____n, int *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  // mark begin: SIRFilter InputGenerate

  (*____in++);

  if ((0 && ((curSample__378__55) == 256))) {{
    ((*____out++)=((float)(sqrtf(((double)(((curSample__378__55) * 11)))))));
    ((*____out++)=(((float)(sqrtf(((double)(((curSample__378__55) * 11)))))) + ((float)1.0)));
  } } else {{
    ((*____out++)=(-((float)(sqrtf(((double)(((curSample__378__55) * 11))))))));
    ((*____out++)=(-(((float)(sqrtf(((double)(((curSample__378__55) * 11)))))) + ((float)1.0))));
  }}
  ((curSample__378__55)++);

  if (((curSample__378__55) >= 1024)) {((curSample__378__55) = 0)/*int*/;}
  // mark end: SIRFilter InputGenerate

}}


#endif // BUFFER_MERGE


 
void work_InputGenerate__381_106__55(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter InputGenerate

      __pop__55();

      if ((0 && ((curSample__378__55) == 256))) {{
        __push__55(((float)(sqrtf(((double)(((curSample__378__55) * 11)))))));
        __push__55((((float)(sqrtf(((double)(((curSample__378__55) * 11)))))) + ((float)1.0)));
      } } else {{
        __push__55((-((float)(sqrtf(((double)(((curSample__378__55) * 11))))))));
        __push__55((-(((float)(sqrtf(((double)(((curSample__378__55) * 11)))))) + ((float)1.0))));
      }}
      ((curSample__378__55)++);

      if (((curSample__378__55) >= 1024)) {((curSample__378__55) = 0)/*int*/;}
      // mark end: SIRFilter InputGenerate

    }
  }
}

// peek: 4 pop: 4 push 2
// init counts: 0 steady counts: 3

// ClusterFusion isEliminated: false



int __number_of_iterations_56;
int __counter_56 = 0;
int __steady_56 = 0;
int __tmp_56 = 0;
int __tmp2_56 = 0;
int *__state_flag_56 = NULL;
thread_info *__thread_56 = NULL;



float real_weight__382__56[64] = {0};
float imag_weight__383__56[64] = {0};
float realBuffer__384__56[64] = {0};
float imagBuffer__385__56[64] = {0};
int count__386__56 = 0;
int pos__387__56 = 0;
void save_peek_buffer__56(object_write_buffer *buf);
void load_peek_buffer__56(object_write_buffer *buf);
void save_file_pointer__56(object_write_buffer *buf);
void load_file_pointer__56(object_write_buffer *buf);

 
void init_BeamFirFilter__396_107__56();
inline void check_status__56();

void work_BeamFirFilter__396_107__56(int);


inline float __pop__56() {
float res=BUFFER_55_56[TAIL_55_56];
TAIL_55_56++;
return res;
}

inline float __pop__56(int n) {
float res=BUFFER_55_56[TAIL_55_56];
TAIL_55_56+=n;

return res;
}

inline float __peek__56(int offs) {
return BUFFER_55_56[TAIL_55_56+offs];
}



inline void __push__56(float data) {
BUFFER_56_57[HEAD_56_57]=data;
HEAD_56_57++;
}



 
void init_BeamFirFilter__396_107__56(){
  int idx__394 = 0;/* int */
  int j__395 = 0;/* int */

  ((pos__387__56) = 0)/*int*/;
  for ((j__395 = 0)/*int*/; (j__395 < 64); (j__395++)) {{
      (idx__394 = (j__395 + 1))/*int*/;
      (((real_weight__382__56)[(int)j__395]) = (((float)(sinf(((double)(idx__394))))) / ((float)(idx__394))))/*float*/;
      (((imag_weight__383__56)[(int)j__395]) = (((float)(cosf(((double)(idx__394))))) / ((float)(idx__394))))/*float*/;
    }
  }
}
void save_file_pointer__56(object_write_buffer *buf) {}
void load_file_pointer__56(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__396_107__56__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__390 = 0.0f;/* float */
  float imag_curr__391 = 0.0f;/* float */
  int i__392 = 0;/* int */
  int modPos__393 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__390 = ((float)0.0))/*float*/;
  (imag_curr__391 = ((float)0.0))/*float*/;
  (((realBuffer__384__56)[(int)(63 - (pos__387__56))]) = (*____in++))/*float*/;
  (((imagBuffer__385__56)[(int)(63 - (pos__387__56))]) = (*____in++))/*float*/;
  (modPos__393 = (63 - (pos__387__56)))/*int*/;
  for ((i__392 = 0)/*int*/; (i__392 < 64); (i__392++)) {{
      (real_curr__390 = (real_curr__390 + ((((realBuffer__384__56)[(int)modPos__393]) * ((real_weight__382__56)[(int)i__392])) + (((imagBuffer__385__56)[(int)modPos__393]) * ((imag_weight__383__56)[(int)i__392])))))/*float*/;
      (imag_curr__391 = (imag_curr__391 + ((((imagBuffer__385__56)[(int)modPos__393]) * ((real_weight__382__56)[(int)i__392])) + (((realBuffer__384__56)[(int)modPos__393]) * ((imag_weight__383__56)[(int)i__392])))))/*float*/;
      (modPos__393 = ((modPos__393 + 1) & 63))/*int*/;
    }
  }
  ((pos__387__56) = (((pos__387__56) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__390);
  ((*____out++)=imag_curr__391);
  for ((i__392 = 2)/*int*/; (i__392 < 4); (i__392++)) {{
      (*____in++);
    }
  }
  ((count__386__56) = ((count__386__56) + 2))/*int*/;

  if (((count__386__56) == 1024)) {{
    ((count__386__56) = 0)/*int*/;
    ((pos__387__56) = 0)/*int*/;
    for ((i__392 = 0)/*int*/; (i__392 < 64); (i__392++)) {{
        (((realBuffer__384__56)[(int)i__392]) = ((float)0.0))/*float*/;
        (((imagBuffer__385__56)[(int)i__392]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__396_107__56__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__390 = 0.0f;/* float */
  float imag_curr__391 = 0.0f;/* float */
  int i__392 = 0;/* int */
  int modPos__393 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__390 = ((float)0.0))/*float*/;
  (imag_curr__391 = ((float)0.0))/*float*/;
  (((realBuffer__384__56)[(int)(63 - (pos__387__56))]) = (*____in++))/*float*/;
  (((imagBuffer__385__56)[(int)(63 - (pos__387__56))]) = (*____in++))/*float*/;
  (modPos__393 = (63 - (pos__387__56)))/*int*/;
  for ((i__392 = 0)/*int*/; (i__392 < 64); (i__392++)) {{
      (real_curr__390 = (real_curr__390 + ((((realBuffer__384__56)[(int)modPos__393]) * ((real_weight__382__56)[(int)i__392])) + (((imagBuffer__385__56)[(int)modPos__393]) * ((imag_weight__383__56)[(int)i__392])))))/*float*/;
      (imag_curr__391 = (imag_curr__391 + ((((imagBuffer__385__56)[(int)modPos__393]) * ((real_weight__382__56)[(int)i__392])) + (((realBuffer__384__56)[(int)modPos__393]) * ((imag_weight__383__56)[(int)i__392])))))/*float*/;
      (modPos__393 = ((modPos__393 + 1) & 63))/*int*/;
    }
  }
  ((pos__387__56) = (((pos__387__56) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__390);
  ((*____out++)=imag_curr__391);
  for ((i__392 = 2)/*int*/; (i__392 < 4); (i__392++)) {{
      (*____in++);
    }
  }
  ((count__386__56) = ((count__386__56) + 2))/*int*/;

  if (((count__386__56) == 1024)) {{
    ((count__386__56) = 0)/*int*/;
    ((pos__387__56) = 0)/*int*/;
    for ((i__392 = 0)/*int*/; (i__392 < 64); (i__392++)) {{
        (((realBuffer__384__56)[(int)i__392]) = ((float)0.0))/*float*/;
        (((imagBuffer__385__56)[(int)i__392]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__396_107__56(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__390 = 0.0f;/* float */
      float imag_curr__391 = 0.0f;/* float */
      int i__392 = 0;/* int */
      int modPos__393 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__390 = ((float)0.0))/*float*/;
      (imag_curr__391 = ((float)0.0))/*float*/;
      (((realBuffer__384__56)[(int)(63 - (pos__387__56))]) = __pop__56())/*float*/;
      (((imagBuffer__385__56)[(int)(63 - (pos__387__56))]) = __pop__56())/*float*/;
      (modPos__393 = (63 - (pos__387__56)))/*int*/;
      for ((i__392 = 0)/*int*/; (i__392 < 64); (i__392++)) {{
          (real_curr__390 = (real_curr__390 + ((((realBuffer__384__56)[(int)modPos__393]) * ((real_weight__382__56)[(int)i__392])) + (((imagBuffer__385__56)[(int)modPos__393]) * ((imag_weight__383__56)[(int)i__392])))))/*float*/;
          (imag_curr__391 = (imag_curr__391 + ((((imagBuffer__385__56)[(int)modPos__393]) * ((real_weight__382__56)[(int)i__392])) + (((realBuffer__384__56)[(int)modPos__393]) * ((imag_weight__383__56)[(int)i__392])))))/*float*/;
          (modPos__393 = ((modPos__393 + 1) & 63))/*int*/;
        }
      }
      ((pos__387__56) = (((pos__387__56) + 1) & 63))/*int*/;
      __push__56(real_curr__390);
      __push__56(imag_curr__391);
      for ((i__392 = 2)/*int*/; (i__392 < 4); (i__392++)) {{
          __pop__56();
        }
      }
      ((count__386__56) = ((count__386__56) + 2))/*int*/;

      if (((count__386__56) == 1024)) {{
        ((count__386__56) = 0)/*int*/;
        ((pos__387__56) = 0)/*int*/;
        for ((i__392 = 0)/*int*/; (i__392 < 64); (i__392++)) {{
            (((realBuffer__384__56)[(int)i__392]) = ((float)0.0))/*float*/;
            (((imagBuffer__385__56)[(int)i__392]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

// peek: 6 pop: 6 push 2
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_57;
int __counter_57 = 0;
int __steady_57 = 0;
int __tmp_57 = 0;
int __tmp2_57 = 0;
int *__state_flag_57 = NULL;
thread_info *__thread_57 = NULL;



float real_weight__397__57[64] = {0};
float imag_weight__398__57[64] = {0};
float realBuffer__399__57[64] = {0};
float imagBuffer__400__57[64] = {0};
int count__401__57 = 0;
int pos__402__57 = 0;
void save_peek_buffer__57(object_write_buffer *buf);
void load_peek_buffer__57(object_write_buffer *buf);
void save_file_pointer__57(object_write_buffer *buf);
void load_file_pointer__57(object_write_buffer *buf);

 
void init_BeamFirFilter__411_108__57();
inline void check_status__57();

void work_BeamFirFilter__411_108__57(int);


inline float __pop__57() {
float res=BUFFER_56_57[TAIL_56_57];
TAIL_56_57++;
return res;
}

inline float __pop__57(int n) {
float res=BUFFER_56_57[TAIL_56_57];
TAIL_56_57+=n;

return res;
}

inline float __peek__57(int offs) {
return BUFFER_56_57[TAIL_56_57+offs];
}



inline void __push__57(float data) {
BUFFER_57_5[HEAD_57_5]=data;
HEAD_57_5++;
}



 
void init_BeamFirFilter__411_108__57(){
  int idx__409 = 0;/* int */
  int j__410 = 0;/* int */

  ((pos__402__57) = 0)/*int*/;
  for ((j__410 = 0)/*int*/; (j__410 < 64); (j__410++)) {{
      (idx__409 = (j__410 + 1))/*int*/;
      (((real_weight__397__57)[(int)j__410]) = (((float)(sinf(((double)(idx__409))))) / ((float)(idx__409))))/*float*/;
      (((imag_weight__398__57)[(int)j__410]) = (((float)(cosf(((double)(idx__409))))) / ((float)(idx__409))))/*float*/;
    }
  }
}
void save_file_pointer__57(object_write_buffer *buf) {}
void load_file_pointer__57(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_BeamFirFilter__411_108__57__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float real_curr__405 = 0.0f;/* float */
  float imag_curr__406 = 0.0f;/* float */
  int i__407 = 0;/* int */
  int modPos__408 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__405 = ((float)0.0))/*float*/;
  (imag_curr__406 = ((float)0.0))/*float*/;
  (((realBuffer__399__57)[(int)(63 - (pos__402__57))]) = (*____in++))/*float*/;
  (((imagBuffer__400__57)[(int)(63 - (pos__402__57))]) = (*____in++))/*float*/;
  (modPos__408 = (63 - (pos__402__57)))/*int*/;
  for ((i__407 = 0)/*int*/; (i__407 < 64); (i__407++)) {{
      (real_curr__405 = (real_curr__405 + ((((realBuffer__399__57)[(int)modPos__408]) * ((real_weight__397__57)[(int)i__407])) + (((imagBuffer__400__57)[(int)modPos__408]) * ((imag_weight__398__57)[(int)i__407])))))/*float*/;
      (imag_curr__406 = (imag_curr__406 + ((((imagBuffer__400__57)[(int)modPos__408]) * ((real_weight__397__57)[(int)i__407])) + (((realBuffer__399__57)[(int)modPos__408]) * ((imag_weight__398__57)[(int)i__407])))))/*float*/;
      (modPos__408 = ((modPos__408 + 1) & 63))/*int*/;
    }
  }
  ((pos__402__57) = (((pos__402__57) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__405);
  ((*____out++)=imag_curr__406);
  for ((i__407 = 2)/*int*/; (i__407 < 6); (i__407++)) {{
      (*____in++);
    }
  }
  ((count__401__57) = ((count__401__57) + 3))/*int*/;

  if (((count__401__57) == 512)) {{
    ((count__401__57) = 0)/*int*/;
    ((pos__402__57) = 0)/*int*/;
    for ((i__407 = 0)/*int*/; (i__407 < 64); (i__407++)) {{
        (((realBuffer__399__57)[(int)i__407]) = ((float)0.0))/*float*/;
        (((imagBuffer__400__57)[(int)i__407]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


void work_BeamFirFilter__411_108__57__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float real_curr__405 = 0.0f;/* float */
  float imag_curr__406 = 0.0f;/* float */
  int i__407 = 0;/* int */
  int modPos__408 = 0;/* int */

  // mark begin: SIRFilter BeamFirFilter

  (real_curr__405 = ((float)0.0))/*float*/;
  (imag_curr__406 = ((float)0.0))/*float*/;
  (((realBuffer__399__57)[(int)(63 - (pos__402__57))]) = (*____in++))/*float*/;
  (((imagBuffer__400__57)[(int)(63 - (pos__402__57))]) = (*____in++))/*float*/;
  (modPos__408 = (63 - (pos__402__57)))/*int*/;
  for ((i__407 = 0)/*int*/; (i__407 < 64); (i__407++)) {{
      (real_curr__405 = (real_curr__405 + ((((realBuffer__399__57)[(int)modPos__408]) * ((real_weight__397__57)[(int)i__407])) + (((imagBuffer__400__57)[(int)modPos__408]) * ((imag_weight__398__57)[(int)i__407])))))/*float*/;
      (imag_curr__406 = (imag_curr__406 + ((((imagBuffer__400__57)[(int)modPos__408]) * ((real_weight__397__57)[(int)i__407])) + (((realBuffer__399__57)[(int)modPos__408]) * ((imag_weight__398__57)[(int)i__407])))))/*float*/;
      (modPos__408 = ((modPos__408 + 1) & 63))/*int*/;
    }
  }
  ((pos__402__57) = (((pos__402__57) + 1) & 63))/*int*/;
  ((*____out++)=real_curr__405);
  ((*____out++)=imag_curr__406);
  for ((i__407 = 2)/*int*/; (i__407 < 6); (i__407++)) {{
      (*____in++);
    }
  }
  ((count__401__57) = ((count__401__57) + 3))/*int*/;

  if (((count__401__57) == 512)) {{
    ((count__401__57) = 0)/*int*/;
    ((pos__402__57) = 0)/*int*/;
    for ((i__407 = 0)/*int*/; (i__407 < 64); (i__407++)) {{
        (((realBuffer__399__57)[(int)i__407]) = ((float)0.0))/*float*/;
        (((imagBuffer__400__57)[(int)i__407]) = ((float)0.0))/*float*/;
      }
    }
  }}
  // mark end: SIRFilter BeamFirFilter

}}


#endif // BUFFER_MERGE


 
void work_BeamFirFilter__411_108__57(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float real_curr__405 = 0.0f;/* float */
      float imag_curr__406 = 0.0f;/* float */
      int i__407 = 0;/* int */
      int modPos__408 = 0;/* int */

      // mark begin: SIRFilter BeamFirFilter

      (real_curr__405 = ((float)0.0))/*float*/;
      (imag_curr__406 = ((float)0.0))/*float*/;
      (((realBuffer__399__57)[(int)(63 - (pos__402__57))]) = __pop__57())/*float*/;
      (((imagBuffer__400__57)[(int)(63 - (pos__402__57))]) = __pop__57())/*float*/;
      (modPos__408 = (63 - (pos__402__57)))/*int*/;
      for ((i__407 = 0)/*int*/; (i__407 < 64); (i__407++)) {{
          (real_curr__405 = (real_curr__405 + ((((realBuffer__399__57)[(int)modPos__408]) * ((real_weight__397__57)[(int)i__407])) + (((imagBuffer__400__57)[(int)modPos__408]) * ((imag_weight__398__57)[(int)i__407])))))/*float*/;
          (imag_curr__406 = (imag_curr__406 + ((((imagBuffer__400__57)[(int)modPos__408]) * ((real_weight__397__57)[(int)i__407])) + (((realBuffer__399__57)[(int)modPos__408]) * ((imag_weight__398__57)[(int)i__407])))))/*float*/;
          (modPos__408 = ((modPos__408 + 1) & 63))/*int*/;
        }
      }
      ((pos__402__57) = (((pos__402__57) + 1) & 63))/*int*/;
      __push__57(real_curr__405);
      __push__57(imag_curr__406);
      for ((i__407 = 2)/*int*/; (i__407 < 6); (i__407++)) {{
          __pop__57();
        }
      }
      ((count__401__57) = ((count__401__57) + 3))/*int*/;

      if (((count__401__57) == 512)) {{
        ((count__401__57) = 0)/*int*/;
        ((pos__402__57) = 0)/*int*/;
        for ((i__407 = 0)/*int*/; (i__407 < 64); (i__407++)) {{
            (((realBuffer__399__57)[(int)i__407]) = ((float)0.0))/*float*/;
            (((imagBuffer__400__57)[(int)i__407]) = ((float)0.0))/*float*/;
          }
        }
      }}
      // mark end: SIRFilter BeamFirFilter

    }
  }
}

