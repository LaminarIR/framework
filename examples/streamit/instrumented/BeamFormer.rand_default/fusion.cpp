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
extern void init_IntSource__3_59__0();
extern void work_IntSource__3_59__0(int);
#ifdef BUFFER_MERGE
extern void work_IntSource__3_59__0__mod(int ____n, void *____in, int *____out);
extern void work_IntSource__3_59__0__mod2(int ____n, void *____in, int *____out, int s1, int s2);
#endif
extern void __splitter_1_work(int);
extern void init_InputGenerate__7_62__2();
extern void work_InputGenerate__7_62__2(int);
#ifdef BUFFER_MERGE
extern void work_InputGenerate__7_62__2__mod(int ____n, int *____in, float *____out);
extern void work_InputGenerate__7_62__2__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__22_63__3();
extern void work_BeamFirFilter__22_63__3(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__22_63__3__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__22_63__3__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__37_64__4();
extern void work_BeamFirFilter__37_64__4(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__37_64__4__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__37_64__4__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void __joiner_5_work(int);
extern void __splitter_6_work(int);
extern void init_BeamForm__423_110__7();
extern void work_BeamForm__423_110__7(int);
#ifdef BUFFER_MERGE
extern void work_BeamForm__423_110__7__mod(int ____n, float *____in, float *____out);
extern void work_BeamForm__423_110__7__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__438_111__8();
extern void work_BeamFirFilter__438_111__8(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__438_111__8__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__438_111__8__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_Magnitude__446_112__9();
extern void work_Magnitude__446_112__9(int);
#ifdef BUFFER_MERGE
extern void work_Magnitude__446_112__9__mod(int ____n, float *____in, float *____out);
extern void work_Magnitude__446_112__9__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_Detector__452_113__10();
extern void work_Detector__452_113__10(int);
#ifdef BUFFER_MERGE
extern void work_Detector__452_113__10__mod(int ____n, float *____in, float *____out);
extern void work_Detector__452_113__10__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void __joiner_11_work(int);
extern void init_FloatPrinter__579_129__12();
extern void work_FloatPrinter__579_129__12(int);
#ifdef BUFFER_MERGE
extern void work_FloatPrinter__579_129__12__mod(int ____n, float *____in, void *____out);
extern void work_FloatPrinter__579_129__12__mod2(int ____n, float *____in, void *____out, int s1, int s2);
#endif
extern void init_BeamForm__464_115__13();
extern void work_BeamForm__464_115__13(int);
#ifdef BUFFER_MERGE
extern void work_BeamForm__464_115__13__mod(int ____n, float *____in, float *____out);
extern void work_BeamForm__464_115__13__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__479_116__14();
extern void work_BeamFirFilter__479_116__14(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__479_116__14__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__479_116__14__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_Magnitude__487_117__15();
extern void work_Magnitude__487_117__15(int);
#ifdef BUFFER_MERGE
extern void work_Magnitude__487_117__15__mod(int ____n, float *____in, float *____out);
extern void work_Magnitude__487_117__15__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_Detector__493_118__16();
extern void work_Detector__493_118__16(int);
#ifdef BUFFER_MERGE
extern void work_Detector__493_118__16__mod(int ____n, float *____in, float *____out);
extern void work_Detector__493_118__16__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamForm__505_120__17();
extern void work_BeamForm__505_120__17(int);
#ifdef BUFFER_MERGE
extern void work_BeamForm__505_120__17__mod(int ____n, float *____in, float *____out);
extern void work_BeamForm__505_120__17__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__520_121__18();
extern void work_BeamFirFilter__520_121__18(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__520_121__18__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__520_121__18__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_Magnitude__528_122__19();
extern void work_Magnitude__528_122__19(int);
#ifdef BUFFER_MERGE
extern void work_Magnitude__528_122__19__mod(int ____n, float *____in, float *____out);
extern void work_Magnitude__528_122__19__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_Detector__534_123__20();
extern void work_Detector__534_123__20(int);
#ifdef BUFFER_MERGE
extern void work_Detector__534_123__20__mod(int ____n, float *____in, float *____out);
extern void work_Detector__534_123__20__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamForm__546_125__21();
extern void work_BeamForm__546_125__21(int);
#ifdef BUFFER_MERGE
extern void work_BeamForm__546_125__21__mod(int ____n, float *____in, float *____out);
extern void work_BeamForm__546_125__21__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__561_126__22();
extern void work_BeamFirFilter__561_126__22(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__561_126__22__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__561_126__22__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_Magnitude__569_127__23();
extern void work_Magnitude__569_127__23(int);
#ifdef BUFFER_MERGE
extern void work_Magnitude__569_127__23__mod(int ____n, float *____in, float *____out);
extern void work_Magnitude__569_127__23__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_Detector__575_128__24();
extern void work_Detector__575_128__24(int);
#ifdef BUFFER_MERGE
extern void work_Detector__575_128__24__mod(int ____n, float *____in, float *____out);
extern void work_Detector__575_128__24__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_InputGenerate__41_66__25();
extern void work_InputGenerate__41_66__25(int);
#ifdef BUFFER_MERGE
extern void work_InputGenerate__41_66__25__mod(int ____n, int *____in, float *____out);
extern void work_InputGenerate__41_66__25__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__56_67__26();
extern void work_BeamFirFilter__56_67__26(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__56_67__26__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__56_67__26__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__71_68__27();
extern void work_BeamFirFilter__71_68__27(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__71_68__27__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__71_68__27__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_InputGenerate__75_70__28();
extern void work_InputGenerate__75_70__28(int);
#ifdef BUFFER_MERGE
extern void work_InputGenerate__75_70__28__mod(int ____n, int *____in, float *____out);
extern void work_InputGenerate__75_70__28__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__90_71__29();
extern void work_BeamFirFilter__90_71__29(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__90_71__29__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__90_71__29__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__105_72__30();
extern void work_BeamFirFilter__105_72__30(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__105_72__30__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__105_72__30__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_InputGenerate__109_74__31();
extern void work_InputGenerate__109_74__31(int);
#ifdef BUFFER_MERGE
extern void work_InputGenerate__109_74__31__mod(int ____n, int *____in, float *____out);
extern void work_InputGenerate__109_74__31__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__124_75__32();
extern void work_BeamFirFilter__124_75__32(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__124_75__32__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__124_75__32__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__139_76__33();
extern void work_BeamFirFilter__139_76__33(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__139_76__33__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__139_76__33__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_InputGenerate__143_78__34();
extern void work_InputGenerate__143_78__34(int);
#ifdef BUFFER_MERGE
extern void work_InputGenerate__143_78__34__mod(int ____n, int *____in, float *____out);
extern void work_InputGenerate__143_78__34__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__158_79__35();
extern void work_BeamFirFilter__158_79__35(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__158_79__35__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__158_79__35__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__173_80__36();
extern void work_BeamFirFilter__173_80__36(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__173_80__36__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__173_80__36__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_InputGenerate__177_82__37();
extern void work_InputGenerate__177_82__37(int);
#ifdef BUFFER_MERGE
extern void work_InputGenerate__177_82__37__mod(int ____n, int *____in, float *____out);
extern void work_InputGenerate__177_82__37__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__192_83__38();
extern void work_BeamFirFilter__192_83__38(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__192_83__38__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__192_83__38__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__207_84__39();
extern void work_BeamFirFilter__207_84__39(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__207_84__39__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__207_84__39__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_InputGenerate__211_86__40();
extern void work_InputGenerate__211_86__40(int);
#ifdef BUFFER_MERGE
extern void work_InputGenerate__211_86__40__mod(int ____n, int *____in, float *____out);
extern void work_InputGenerate__211_86__40__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__226_87__41();
extern void work_BeamFirFilter__226_87__41(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__226_87__41__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__226_87__41__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__241_88__42();
extern void work_BeamFirFilter__241_88__42(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__241_88__42__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__241_88__42__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_InputGenerate__245_90__43();
extern void work_InputGenerate__245_90__43(int);
#ifdef BUFFER_MERGE
extern void work_InputGenerate__245_90__43__mod(int ____n, int *____in, float *____out);
extern void work_InputGenerate__245_90__43__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__260_91__44();
extern void work_BeamFirFilter__260_91__44(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__260_91__44__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__260_91__44__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__275_92__45();
extern void work_BeamFirFilter__275_92__45(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__275_92__45__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__275_92__45__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_InputGenerate__279_94__46();
extern void work_InputGenerate__279_94__46(int);
#ifdef BUFFER_MERGE
extern void work_InputGenerate__279_94__46__mod(int ____n, int *____in, float *____out);
extern void work_InputGenerate__279_94__46__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__294_95__47();
extern void work_BeamFirFilter__294_95__47(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__294_95__47__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__294_95__47__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__309_96__48();
extern void work_BeamFirFilter__309_96__48(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__309_96__48__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__309_96__48__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_InputGenerate__313_98__49();
extern void work_InputGenerate__313_98__49(int);
#ifdef BUFFER_MERGE
extern void work_InputGenerate__313_98__49__mod(int ____n, int *____in, float *____out);
extern void work_InputGenerate__313_98__49__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__328_99__50();
extern void work_BeamFirFilter__328_99__50(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__328_99__50__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__328_99__50__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__343_100__51();
extern void work_BeamFirFilter__343_100__51(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__343_100__51__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__343_100__51__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_InputGenerate__347_102__52();
extern void work_InputGenerate__347_102__52(int);
#ifdef BUFFER_MERGE
extern void work_InputGenerate__347_102__52__mod(int ____n, int *____in, float *____out);
extern void work_InputGenerate__347_102__52__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__362_103__53();
extern void work_BeamFirFilter__362_103__53(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__362_103__53__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__362_103__53__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__377_104__54();
extern void work_BeamFirFilter__377_104__54(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__377_104__54__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__377_104__54__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_InputGenerate__381_106__55();
extern void work_InputGenerate__381_106__55(int);
#ifdef BUFFER_MERGE
extern void work_InputGenerate__381_106__55__mod(int ____n, int *____in, float *____out);
extern void work_InputGenerate__381_106__55__mod2(int ____n, int *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__396_107__56();
extern void work_BeamFirFilter__396_107__56(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__396_107__56__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__396_107__56__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
extern void init_BeamFirFilter__411_108__57();
extern void work_BeamFirFilter__411_108__57(int);
#ifdef BUFFER_MERGE
extern void work_BeamFirFilter__411_108__57__mod(int ____n, float *____in, float *____out);
extern void work_BeamFirFilter__411_108__57__mod2(int ____n, float *____in, float *____out, int s1, int s2);
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

  // ============= Steady State =============

  if (__timer_enabled) {
    tt.start();
  }
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
if (__timer_enabled) {
    tt.stop();
    tt.output(stderr);
  }


  return 0;
}
