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
volatile double finalSum=0;
proc_timer tt("total runtime");


float BUFFER_0_1[__BUF_SIZE_MASK_0_1 + 1];
int HEAD_0_1 = 0;
int TAIL_0_1 = 0;
float BUFFER_1_2[__BUF_SIZE_MASK_1_2 + 1];
int HEAD_1_2 = 0;
int TAIL_1_2 = 0;
float BUFFER_1_5[__BUF_SIZE_MASK_1_5 + 1];
int HEAD_1_5 = 0;
int TAIL_1_5 = 0;
float BUFFER_1_6[__BUF_SIZE_MASK_1_6 + 1];
int HEAD_1_6 = 0;
int TAIL_1_6 = 0;
float BUFFER_1_7[__BUF_SIZE_MASK_1_7 + 1];
int HEAD_1_7 = 0;
int TAIL_1_7 = 0;
float BUFFER_1_8[__BUF_SIZE_MASK_1_8 + 1];
int HEAD_1_8 = 0;
int TAIL_1_8 = 0;
float BUFFER_1_9[__BUF_SIZE_MASK_1_9 + 1];
int HEAD_1_9 = 0;
int TAIL_1_9 = 0;
float BUFFER_1_10[__BUF_SIZE_MASK_1_10 + 1];
int HEAD_1_10 = 0;
int TAIL_1_10 = 0;
float BUFFER_1_11[__BUF_SIZE_MASK_1_11 + 1];
int HEAD_1_11 = 0;
int TAIL_1_11 = 0;
float BUFFER_1_12[__BUF_SIZE_MASK_1_12 + 1];
int HEAD_1_12 = 0;
int TAIL_1_12 = 0;
float BUFFER_1_13[__BUF_SIZE_MASK_1_13 + 1];
int HEAD_1_13 = 0;
int TAIL_1_13 = 0;
float BUFFER_1_14[__BUF_SIZE_MASK_1_14 + 1];
int HEAD_1_14 = 0;
int TAIL_1_14 = 0;
float BUFFER_1_15[__BUF_SIZE_MASK_1_15 + 1];
int HEAD_1_15 = 0;
int TAIL_1_15 = 0;
float BUFFER_1_16[__BUF_SIZE_MASK_1_16 + 1];
int HEAD_1_16 = 0;
int TAIL_1_16 = 0;
float BUFFER_1_17[__BUF_SIZE_MASK_1_17 + 1];
int HEAD_1_17 = 0;
int TAIL_1_17 = 0;
float BUFFER_1_18[__BUF_SIZE_MASK_1_18 + 1];
int HEAD_1_18 = 0;
int TAIL_1_18 = 0;
float BUFFER_1_19[__BUF_SIZE_MASK_1_19 + 1];
int HEAD_1_19 = 0;
int TAIL_1_19 = 0;
float BUFFER_1_20[__BUF_SIZE_MASK_1_20 + 1];
int HEAD_1_20 = 0;
int TAIL_1_20 = 0;
float BUFFER_1_21[__BUF_SIZE_MASK_1_21 + 1];
int HEAD_1_21 = 0;
int TAIL_1_21 = 0;
float BUFFER_1_22[__BUF_SIZE_MASK_1_22 + 1];
int HEAD_1_22 = 0;
int TAIL_1_22 = 0;
float BUFFER_1_23[__BUF_SIZE_MASK_1_23 + 1];
int HEAD_1_23 = 0;
int TAIL_1_23 = 0;
float BUFFER_1_24[__BUF_SIZE_MASK_1_24 + 1];
int HEAD_1_24 = 0;
int TAIL_1_24 = 0;
float BUFFER_1_25[__BUF_SIZE_MASK_1_25 + 1];
int HEAD_1_25 = 0;
int TAIL_1_25 = 0;
float BUFFER_1_26[__BUF_SIZE_MASK_1_26 + 1];
int HEAD_1_26 = 0;
int TAIL_1_26 = 0;
float BUFFER_1_27[__BUF_SIZE_MASK_1_27 + 1];
int HEAD_1_27 = 0;
int TAIL_1_27 = 0;
float BUFFER_1_28[__BUF_SIZE_MASK_1_28 + 1];
int HEAD_1_28 = 0;
int TAIL_1_28 = 0;
float BUFFER_1_29[__BUF_SIZE_MASK_1_29 + 1];
int HEAD_1_29 = 0;
int TAIL_1_29 = 0;
float BUFFER_1_30[__BUF_SIZE_MASK_1_30 + 1];
int HEAD_1_30 = 0;
int TAIL_1_30 = 0;
float BUFFER_1_31[__BUF_SIZE_MASK_1_31 + 1];
int HEAD_1_31 = 0;
int TAIL_1_31 = 0;
float BUFFER_1_32[__BUF_SIZE_MASK_1_32 + 1];
int HEAD_1_32 = 0;
int TAIL_1_32 = 0;
float BUFFER_1_33[__BUF_SIZE_MASK_1_33 + 1];
int HEAD_1_33 = 0;
int TAIL_1_33 = 0;
float BUFFER_1_34[__BUF_SIZE_MASK_1_34 + 1];
int HEAD_1_34 = 0;
int TAIL_1_34 = 0;
float BUFFER_1_35[__BUF_SIZE_MASK_1_35 + 1];
int HEAD_1_35 = 0;
int TAIL_1_35 = 0;
float BUFFER_1_36[__BUF_SIZE_MASK_1_36 + 1];
int HEAD_1_36 = 0;
int TAIL_1_36 = 0;
float BUFFER_1_37[__BUF_SIZE_MASK_1_37 + 1];
int HEAD_1_37 = 0;
int TAIL_1_37 = 0;
float BUFFER_1_38[__BUF_SIZE_MASK_1_38 + 1];
int HEAD_1_38 = 0;
int TAIL_1_38 = 0;
float BUFFER_1_39[__BUF_SIZE_MASK_1_39 + 1];
int HEAD_1_39 = 0;
int TAIL_1_39 = 0;
float BUFFER_1_40[__BUF_SIZE_MASK_1_40 + 1];
int HEAD_1_40 = 0;
int TAIL_1_40 = 0;
float BUFFER_1_41[__BUF_SIZE_MASK_1_41 + 1];
int HEAD_1_41 = 0;
int TAIL_1_41 = 0;
float BUFFER_1_42[__BUF_SIZE_MASK_1_42 + 1];
int HEAD_1_42 = 0;
int TAIL_1_42 = 0;
float BUFFER_1_43[__BUF_SIZE_MASK_1_43 + 1];
int HEAD_1_43 = 0;
int TAIL_1_43 = 0;
float BUFFER_1_44[__BUF_SIZE_MASK_1_44 + 1];
int HEAD_1_44 = 0;
int TAIL_1_44 = 0;
float BUFFER_1_45[__BUF_SIZE_MASK_1_45 + 1];
int HEAD_1_45 = 0;
int TAIL_1_45 = 0;
float BUFFER_1_46[__BUF_SIZE_MASK_1_46 + 1];
int HEAD_1_46 = 0;
int TAIL_1_46 = 0;
float BUFFER_1_47[__BUF_SIZE_MASK_1_47 + 1];
int HEAD_1_47 = 0;
int TAIL_1_47 = 0;
float BUFFER_1_48[__BUF_SIZE_MASK_1_48 + 1];
int HEAD_1_48 = 0;
int TAIL_1_48 = 0;
float BUFFER_1_49[__BUF_SIZE_MASK_1_49 + 1];
int HEAD_1_49 = 0;
int TAIL_1_49 = 0;
float BUFFER_1_50[__BUF_SIZE_MASK_1_50 + 1];
int HEAD_1_50 = 0;
int TAIL_1_50 = 0;
float BUFFER_1_51[__BUF_SIZE_MASK_1_51 + 1];
int HEAD_1_51 = 0;
int TAIL_1_51 = 0;
float BUFFER_1_52[__BUF_SIZE_MASK_1_52 + 1];
int HEAD_1_52 = 0;
int TAIL_1_52 = 0;
float BUFFER_1_53[__BUF_SIZE_MASK_1_53 + 1];
int HEAD_1_53 = 0;
int TAIL_1_53 = 0;
float BUFFER_1_54[__BUF_SIZE_MASK_1_54 + 1];
int HEAD_1_54 = 0;
int TAIL_1_54 = 0;
float BUFFER_1_55[__BUF_SIZE_MASK_1_55 + 1];
int HEAD_1_55 = 0;
int TAIL_1_55 = 0;
float BUFFER_1_56[__BUF_SIZE_MASK_1_56 + 1];
int HEAD_1_56 = 0;
int TAIL_1_56 = 0;
float BUFFER_1_57[__BUF_SIZE_MASK_1_57 + 1];
int HEAD_1_57 = 0;
int TAIL_1_57 = 0;
float BUFFER_1_58[__BUF_SIZE_MASK_1_58 + 1];
int HEAD_1_58 = 0;
int TAIL_1_58 = 0;
float BUFFER_1_59[__BUF_SIZE_MASK_1_59 + 1];
int HEAD_1_59 = 0;
int TAIL_1_59 = 0;
float BUFFER_1_60[__BUF_SIZE_MASK_1_60 + 1];
int HEAD_1_60 = 0;
int TAIL_1_60 = 0;
float BUFFER_1_61[__BUF_SIZE_MASK_1_61 + 1];
int HEAD_1_61 = 0;
int TAIL_1_61 = 0;
float BUFFER_1_62[__BUF_SIZE_MASK_1_62 + 1];
int HEAD_1_62 = 0;
int TAIL_1_62 = 0;
float BUFFER_1_63[__BUF_SIZE_MASK_1_63 + 1];
int HEAD_1_63 = 0;
int TAIL_1_63 = 0;
float BUFFER_1_64[__BUF_SIZE_MASK_1_64 + 1];
int HEAD_1_64 = 0;
int TAIL_1_64 = 0;
float BUFFER_1_65[__BUF_SIZE_MASK_1_65 + 1];
int HEAD_1_65 = 0;
int TAIL_1_65 = 0;
float BUFFER_1_66[__BUF_SIZE_MASK_1_66 + 1];
int HEAD_1_66 = 0;
int TAIL_1_66 = 0;
float BUFFER_1_67[__BUF_SIZE_MASK_1_67 + 1];
int HEAD_1_67 = 0;
int TAIL_1_67 = 0;
float BUFFER_1_68[__BUF_SIZE_MASK_1_68 + 1];
int HEAD_1_68 = 0;
int TAIL_1_68 = 0;
float BUFFER_1_69[__BUF_SIZE_MASK_1_69 + 1];
int HEAD_1_69 = 0;
int TAIL_1_69 = 0;
float BUFFER_1_70[__BUF_SIZE_MASK_1_70 + 1];
int HEAD_1_70 = 0;
int TAIL_1_70 = 0;
float BUFFER_1_71[__BUF_SIZE_MASK_1_71 + 1];
int HEAD_1_71 = 0;
int TAIL_1_71 = 0;
float BUFFER_1_72[__BUF_SIZE_MASK_1_72 + 1];
int HEAD_1_72 = 0;
int TAIL_1_72 = 0;
float BUFFER_1_73[__BUF_SIZE_MASK_1_73 + 1];
int HEAD_1_73 = 0;
int TAIL_1_73 = 0;
float BUFFER_1_74[__BUF_SIZE_MASK_1_74 + 1];
int HEAD_1_74 = 0;
int TAIL_1_74 = 0;
float BUFFER_1_75[__BUF_SIZE_MASK_1_75 + 1];
int HEAD_1_75 = 0;
int TAIL_1_75 = 0;
float BUFFER_1_76[__BUF_SIZE_MASK_1_76 + 1];
int HEAD_1_76 = 0;
int TAIL_1_76 = 0;
float BUFFER_1_77[__BUF_SIZE_MASK_1_77 + 1];
int HEAD_1_77 = 0;
int TAIL_1_77 = 0;
float BUFFER_1_78[__BUF_SIZE_MASK_1_78 + 1];
int HEAD_1_78 = 0;
int TAIL_1_78 = 0;
float BUFFER_1_79[__BUF_SIZE_MASK_1_79 + 1];
int HEAD_1_79 = 0;
int TAIL_1_79 = 0;
float BUFFER_1_80[__BUF_SIZE_MASK_1_80 + 1];
int HEAD_1_80 = 0;
int TAIL_1_80 = 0;
float BUFFER_1_81[__BUF_SIZE_MASK_1_81 + 1];
int HEAD_1_81 = 0;
int TAIL_1_81 = 0;
float BUFFER_1_82[__BUF_SIZE_MASK_1_82 + 1];
int HEAD_1_82 = 0;
int TAIL_1_82 = 0;
float BUFFER_1_83[__BUF_SIZE_MASK_1_83 + 1];
int HEAD_1_83 = 0;
int TAIL_1_83 = 0;
float BUFFER_1_84[__BUF_SIZE_MASK_1_84 + 1];
int HEAD_1_84 = 0;
int TAIL_1_84 = 0;
float BUFFER_1_85[__BUF_SIZE_MASK_1_85 + 1];
int HEAD_1_85 = 0;
int TAIL_1_85 = 0;
float BUFFER_1_86[__BUF_SIZE_MASK_1_86 + 1];
int HEAD_1_86 = 0;
int TAIL_1_86 = 0;
float BUFFER_1_87[__BUF_SIZE_MASK_1_87 + 1];
int HEAD_1_87 = 0;
int TAIL_1_87 = 0;
float BUFFER_1_88[__BUF_SIZE_MASK_1_88 + 1];
int HEAD_1_88 = 0;
int TAIL_1_88 = 0;
float BUFFER_1_89[__BUF_SIZE_MASK_1_89 + 1];
int HEAD_1_89 = 0;
int TAIL_1_89 = 0;
float BUFFER_1_90[__BUF_SIZE_MASK_1_90 + 1];
int HEAD_1_90 = 0;
int TAIL_1_90 = 0;
float BUFFER_1_91[__BUF_SIZE_MASK_1_91 + 1];
int HEAD_1_91 = 0;
int TAIL_1_91 = 0;
float BUFFER_1_92[__BUF_SIZE_MASK_1_92 + 1];
int HEAD_1_92 = 0;
int TAIL_1_92 = 0;
float BUFFER_1_93[__BUF_SIZE_MASK_1_93 + 1];
int HEAD_1_93 = 0;
int TAIL_1_93 = 0;
float BUFFER_1_94[__BUF_SIZE_MASK_1_94 + 1];
int HEAD_1_94 = 0;
int TAIL_1_94 = 0;
float BUFFER_1_95[__BUF_SIZE_MASK_1_95 + 1];
int HEAD_1_95 = 0;
int TAIL_1_95 = 0;
float BUFFER_1_96[__BUF_SIZE_MASK_1_96 + 1];
int HEAD_1_96 = 0;
int TAIL_1_96 = 0;
float BUFFER_1_97[__BUF_SIZE_MASK_1_97 + 1];
int HEAD_1_97 = 0;
int TAIL_1_97 = 0;
float BUFFER_1_98[__BUF_SIZE_MASK_1_98 + 1];
int HEAD_1_98 = 0;
int TAIL_1_98 = 0;
float BUFFER_1_99[__BUF_SIZE_MASK_1_99 + 1];
int HEAD_1_99 = 0;
int TAIL_1_99 = 0;
float BUFFER_1_100[__BUF_SIZE_MASK_1_100 + 1];
int HEAD_1_100 = 0;
int TAIL_1_100 = 0;
float BUFFER_1_101[__BUF_SIZE_MASK_1_101 + 1];
int HEAD_1_101 = 0;
int TAIL_1_101 = 0;
float BUFFER_1_102[__BUF_SIZE_MASK_1_102 + 1];
int HEAD_1_102 = 0;
int TAIL_1_102 = 0;
float BUFFER_1_103[__BUF_SIZE_MASK_1_103 + 1];
int HEAD_1_103 = 0;
int TAIL_1_103 = 0;
float BUFFER_1_104[__BUF_SIZE_MASK_1_104 + 1];
int HEAD_1_104 = 0;
int TAIL_1_104 = 0;
float BUFFER_1_105[__BUF_SIZE_MASK_1_105 + 1];
int HEAD_1_105 = 0;
int TAIL_1_105 = 0;
float BUFFER_1_106[__BUF_SIZE_MASK_1_106 + 1];
int HEAD_1_106 = 0;
int TAIL_1_106 = 0;
float BUFFER_1_107[__BUF_SIZE_MASK_1_107 + 1];
int HEAD_1_107 = 0;
int TAIL_1_107 = 0;
float BUFFER_1_108[__BUF_SIZE_MASK_1_108 + 1];
int HEAD_1_108 = 0;
int TAIL_1_108 = 0;
float BUFFER_1_109[__BUF_SIZE_MASK_1_109 + 1];
int HEAD_1_109 = 0;
int TAIL_1_109 = 0;
float BUFFER_1_110[__BUF_SIZE_MASK_1_110 + 1];
int HEAD_1_110 = 0;
int TAIL_1_110 = 0;
float BUFFER_1_111[__BUF_SIZE_MASK_1_111 + 1];
int HEAD_1_111 = 0;
int TAIL_1_111 = 0;
float BUFFER_1_112[__BUF_SIZE_MASK_1_112 + 1];
int HEAD_1_112 = 0;
int TAIL_1_112 = 0;
float BUFFER_1_113[__BUF_SIZE_MASK_1_113 + 1];
int HEAD_1_113 = 0;
int TAIL_1_113 = 0;
float BUFFER_1_114[__BUF_SIZE_MASK_1_114 + 1];
int HEAD_1_114 = 0;
int TAIL_1_114 = 0;
float BUFFER_1_115[__BUF_SIZE_MASK_1_115 + 1];
int HEAD_1_115 = 0;
int TAIL_1_115 = 0;
float BUFFER_1_116[__BUF_SIZE_MASK_1_116 + 1];
int HEAD_1_116 = 0;
int TAIL_1_116 = 0;
float BUFFER_1_117[__BUF_SIZE_MASK_1_117 + 1];
int HEAD_1_117 = 0;
int TAIL_1_117 = 0;
float BUFFER_1_118[__BUF_SIZE_MASK_1_118 + 1];
int HEAD_1_118 = 0;
int TAIL_1_118 = 0;
float BUFFER_1_119[__BUF_SIZE_MASK_1_119 + 1];
int HEAD_1_119 = 0;
int TAIL_1_119 = 0;
float BUFFER_1_120[__BUF_SIZE_MASK_1_120 + 1];
int HEAD_1_120 = 0;
int TAIL_1_120 = 0;
float BUFFER_1_121[__BUF_SIZE_MASK_1_121 + 1];
int HEAD_1_121 = 0;
int TAIL_1_121 = 0;
float BUFFER_1_122[__BUF_SIZE_MASK_1_122 + 1];
int HEAD_1_122 = 0;
int TAIL_1_122 = 0;
float BUFFER_1_123[__BUF_SIZE_MASK_1_123 + 1];
int HEAD_1_123 = 0;
int TAIL_1_123 = 0;
float BUFFER_1_124[__BUF_SIZE_MASK_1_124 + 1];
int HEAD_1_124 = 0;
int TAIL_1_124 = 0;
float BUFFER_1_125[__BUF_SIZE_MASK_1_125 + 1];
int HEAD_1_125 = 0;
int TAIL_1_125 = 0;
float BUFFER_1_126[__BUF_SIZE_MASK_1_126 + 1];
int HEAD_1_126 = 0;
int TAIL_1_126 = 0;
float BUFFER_1_127[__BUF_SIZE_MASK_1_127 + 1];
int HEAD_1_127 = 0;
int TAIL_1_127 = 0;
float BUFFER_1_128[__BUF_SIZE_MASK_1_128 + 1];
int HEAD_1_128 = 0;
int TAIL_1_128 = 0;
float BUFFER_1_129[__BUF_SIZE_MASK_1_129 + 1];
int HEAD_1_129 = 0;
int TAIL_1_129 = 0;
float BUFFER_1_130[__BUF_SIZE_MASK_1_130 + 1];
int HEAD_1_130 = 0;
int TAIL_1_130 = 0;
float BUFFER_1_131[__BUF_SIZE_MASK_1_131 + 1];
int HEAD_1_131 = 0;
int TAIL_1_131 = 0;
float BUFFER_2_3[__BUF_SIZE_MASK_2_3 + 1];
int HEAD_2_3 = 0;
int TAIL_2_3 = 0;
float BUFFER_3_4[__BUF_SIZE_MASK_3_4 + 1];
int HEAD_3_4 = 0;
int TAIL_3_4 = 0;
float BUFFER_5_3[__BUF_SIZE_MASK_5_3 + 1];
int HEAD_5_3 = 0;
int TAIL_5_3 = 0;
float BUFFER_6_3[__BUF_SIZE_MASK_6_3 + 1];
int HEAD_6_3 = 0;
int TAIL_6_3 = 0;
float BUFFER_7_3[__BUF_SIZE_MASK_7_3 + 1];
int HEAD_7_3 = 0;
int TAIL_7_3 = 0;
float BUFFER_8_3[__BUF_SIZE_MASK_8_3 + 1];
int HEAD_8_3 = 0;
int TAIL_8_3 = 0;
float BUFFER_9_3[__BUF_SIZE_MASK_9_3 + 1];
int HEAD_9_3 = 0;
int TAIL_9_3 = 0;
float BUFFER_10_3[__BUF_SIZE_MASK_10_3 + 1];
int HEAD_10_3 = 0;
int TAIL_10_3 = 0;
float BUFFER_11_3[__BUF_SIZE_MASK_11_3 + 1];
int HEAD_11_3 = 0;
int TAIL_11_3 = 0;
float BUFFER_12_3[__BUF_SIZE_MASK_12_3 + 1];
int HEAD_12_3 = 0;
int TAIL_12_3 = 0;
float BUFFER_13_3[__BUF_SIZE_MASK_13_3 + 1];
int HEAD_13_3 = 0;
int TAIL_13_3 = 0;
float BUFFER_14_3[__BUF_SIZE_MASK_14_3 + 1];
int HEAD_14_3 = 0;
int TAIL_14_3 = 0;
float BUFFER_15_3[__BUF_SIZE_MASK_15_3 + 1];
int HEAD_15_3 = 0;
int TAIL_15_3 = 0;
float BUFFER_16_3[__BUF_SIZE_MASK_16_3 + 1];
int HEAD_16_3 = 0;
int TAIL_16_3 = 0;
float BUFFER_17_3[__BUF_SIZE_MASK_17_3 + 1];
int HEAD_17_3 = 0;
int TAIL_17_3 = 0;
float BUFFER_18_3[__BUF_SIZE_MASK_18_3 + 1];
int HEAD_18_3 = 0;
int TAIL_18_3 = 0;
float BUFFER_19_3[__BUF_SIZE_MASK_19_3 + 1];
int HEAD_19_3 = 0;
int TAIL_19_3 = 0;
float BUFFER_20_3[__BUF_SIZE_MASK_20_3 + 1];
int HEAD_20_3 = 0;
int TAIL_20_3 = 0;
float BUFFER_21_3[__BUF_SIZE_MASK_21_3 + 1];
int HEAD_21_3 = 0;
int TAIL_21_3 = 0;
float BUFFER_22_3[__BUF_SIZE_MASK_22_3 + 1];
int HEAD_22_3 = 0;
int TAIL_22_3 = 0;
float BUFFER_23_3[__BUF_SIZE_MASK_23_3 + 1];
int HEAD_23_3 = 0;
int TAIL_23_3 = 0;
float BUFFER_24_3[__BUF_SIZE_MASK_24_3 + 1];
int HEAD_24_3 = 0;
int TAIL_24_3 = 0;
float BUFFER_25_3[__BUF_SIZE_MASK_25_3 + 1];
int HEAD_25_3 = 0;
int TAIL_25_3 = 0;
float BUFFER_26_3[__BUF_SIZE_MASK_26_3 + 1];
int HEAD_26_3 = 0;
int TAIL_26_3 = 0;
float BUFFER_27_3[__BUF_SIZE_MASK_27_3 + 1];
int HEAD_27_3 = 0;
int TAIL_27_3 = 0;
float BUFFER_28_3[__BUF_SIZE_MASK_28_3 + 1];
int HEAD_28_3 = 0;
int TAIL_28_3 = 0;
float BUFFER_29_3[__BUF_SIZE_MASK_29_3 + 1];
int HEAD_29_3 = 0;
int TAIL_29_3 = 0;
float BUFFER_30_3[__BUF_SIZE_MASK_30_3 + 1];
int HEAD_30_3 = 0;
int TAIL_30_3 = 0;
float BUFFER_31_3[__BUF_SIZE_MASK_31_3 + 1];
int HEAD_31_3 = 0;
int TAIL_31_3 = 0;
float BUFFER_32_3[__BUF_SIZE_MASK_32_3 + 1];
int HEAD_32_3 = 0;
int TAIL_32_3 = 0;
float BUFFER_33_3[__BUF_SIZE_MASK_33_3 + 1];
int HEAD_33_3 = 0;
int TAIL_33_3 = 0;
float BUFFER_34_3[__BUF_SIZE_MASK_34_3 + 1];
int HEAD_34_3 = 0;
int TAIL_34_3 = 0;
float BUFFER_35_3[__BUF_SIZE_MASK_35_3 + 1];
int HEAD_35_3 = 0;
int TAIL_35_3 = 0;
float BUFFER_36_3[__BUF_SIZE_MASK_36_3 + 1];
int HEAD_36_3 = 0;
int TAIL_36_3 = 0;
float BUFFER_37_3[__BUF_SIZE_MASK_37_3 + 1];
int HEAD_37_3 = 0;
int TAIL_37_3 = 0;
float BUFFER_38_3[__BUF_SIZE_MASK_38_3 + 1];
int HEAD_38_3 = 0;
int TAIL_38_3 = 0;
float BUFFER_39_3[__BUF_SIZE_MASK_39_3 + 1];
int HEAD_39_3 = 0;
int TAIL_39_3 = 0;
float BUFFER_40_3[__BUF_SIZE_MASK_40_3 + 1];
int HEAD_40_3 = 0;
int TAIL_40_3 = 0;
float BUFFER_41_3[__BUF_SIZE_MASK_41_3 + 1];
int HEAD_41_3 = 0;
int TAIL_41_3 = 0;
float BUFFER_42_3[__BUF_SIZE_MASK_42_3 + 1];
int HEAD_42_3 = 0;
int TAIL_42_3 = 0;
float BUFFER_43_3[__BUF_SIZE_MASK_43_3 + 1];
int HEAD_43_3 = 0;
int TAIL_43_3 = 0;
float BUFFER_44_3[__BUF_SIZE_MASK_44_3 + 1];
int HEAD_44_3 = 0;
int TAIL_44_3 = 0;
float BUFFER_45_3[__BUF_SIZE_MASK_45_3 + 1];
int HEAD_45_3 = 0;
int TAIL_45_3 = 0;
float BUFFER_46_3[__BUF_SIZE_MASK_46_3 + 1];
int HEAD_46_3 = 0;
int TAIL_46_3 = 0;
float BUFFER_47_3[__BUF_SIZE_MASK_47_3 + 1];
int HEAD_47_3 = 0;
int TAIL_47_3 = 0;
float BUFFER_48_3[__BUF_SIZE_MASK_48_3 + 1];
int HEAD_48_3 = 0;
int TAIL_48_3 = 0;
float BUFFER_49_3[__BUF_SIZE_MASK_49_3 + 1];
int HEAD_49_3 = 0;
int TAIL_49_3 = 0;
float BUFFER_50_3[__BUF_SIZE_MASK_50_3 + 1];
int HEAD_50_3 = 0;
int TAIL_50_3 = 0;
float BUFFER_51_3[__BUF_SIZE_MASK_51_3 + 1];
int HEAD_51_3 = 0;
int TAIL_51_3 = 0;
float BUFFER_52_3[__BUF_SIZE_MASK_52_3 + 1];
int HEAD_52_3 = 0;
int TAIL_52_3 = 0;
float BUFFER_53_3[__BUF_SIZE_MASK_53_3 + 1];
int HEAD_53_3 = 0;
int TAIL_53_3 = 0;
float BUFFER_54_3[__BUF_SIZE_MASK_54_3 + 1];
int HEAD_54_3 = 0;
int TAIL_54_3 = 0;
float BUFFER_55_3[__BUF_SIZE_MASK_55_3 + 1];
int HEAD_55_3 = 0;
int TAIL_55_3 = 0;
float BUFFER_56_3[__BUF_SIZE_MASK_56_3 + 1];
int HEAD_56_3 = 0;
int TAIL_56_3 = 0;
float BUFFER_57_3[__BUF_SIZE_MASK_57_3 + 1];
int HEAD_57_3 = 0;
int TAIL_57_3 = 0;
float BUFFER_58_3[__BUF_SIZE_MASK_58_3 + 1];
int HEAD_58_3 = 0;
int TAIL_58_3 = 0;
float BUFFER_59_3[__BUF_SIZE_MASK_59_3 + 1];
int HEAD_59_3 = 0;
int TAIL_59_3 = 0;
float BUFFER_60_3[__BUF_SIZE_MASK_60_3 + 1];
int HEAD_60_3 = 0;
int TAIL_60_3 = 0;
float BUFFER_61_3[__BUF_SIZE_MASK_61_3 + 1];
int HEAD_61_3 = 0;
int TAIL_61_3 = 0;
float BUFFER_62_3[__BUF_SIZE_MASK_62_3 + 1];
int HEAD_62_3 = 0;
int TAIL_62_3 = 0;
float BUFFER_63_3[__BUF_SIZE_MASK_63_3 + 1];
int HEAD_63_3 = 0;
int TAIL_63_3 = 0;
float BUFFER_64_3[__BUF_SIZE_MASK_64_3 + 1];
int HEAD_64_3 = 0;
int TAIL_64_3 = 0;
float BUFFER_65_3[__BUF_SIZE_MASK_65_3 + 1];
int HEAD_65_3 = 0;
int TAIL_65_3 = 0;
float BUFFER_66_3[__BUF_SIZE_MASK_66_3 + 1];
int HEAD_66_3 = 0;
int TAIL_66_3 = 0;
float BUFFER_67_3[__BUF_SIZE_MASK_67_3 + 1];
int HEAD_67_3 = 0;
int TAIL_67_3 = 0;
float BUFFER_68_3[__BUF_SIZE_MASK_68_3 + 1];
int HEAD_68_3 = 0;
int TAIL_68_3 = 0;
float BUFFER_69_3[__BUF_SIZE_MASK_69_3 + 1];
int HEAD_69_3 = 0;
int TAIL_69_3 = 0;
float BUFFER_70_3[__BUF_SIZE_MASK_70_3 + 1];
int HEAD_70_3 = 0;
int TAIL_70_3 = 0;
float BUFFER_71_3[__BUF_SIZE_MASK_71_3 + 1];
int HEAD_71_3 = 0;
int TAIL_71_3 = 0;
float BUFFER_72_3[__BUF_SIZE_MASK_72_3 + 1];
int HEAD_72_3 = 0;
int TAIL_72_3 = 0;
float BUFFER_73_3[__BUF_SIZE_MASK_73_3 + 1];
int HEAD_73_3 = 0;
int TAIL_73_3 = 0;
float BUFFER_74_3[__BUF_SIZE_MASK_74_3 + 1];
int HEAD_74_3 = 0;
int TAIL_74_3 = 0;
float BUFFER_75_3[__BUF_SIZE_MASK_75_3 + 1];
int HEAD_75_3 = 0;
int TAIL_75_3 = 0;
float BUFFER_76_3[__BUF_SIZE_MASK_76_3 + 1];
int HEAD_76_3 = 0;
int TAIL_76_3 = 0;
float BUFFER_77_3[__BUF_SIZE_MASK_77_3 + 1];
int HEAD_77_3 = 0;
int TAIL_77_3 = 0;
float BUFFER_78_3[__BUF_SIZE_MASK_78_3 + 1];
int HEAD_78_3 = 0;
int TAIL_78_3 = 0;
float BUFFER_79_3[__BUF_SIZE_MASK_79_3 + 1];
int HEAD_79_3 = 0;
int TAIL_79_3 = 0;
float BUFFER_80_3[__BUF_SIZE_MASK_80_3 + 1];
int HEAD_80_3 = 0;
int TAIL_80_3 = 0;
float BUFFER_81_3[__BUF_SIZE_MASK_81_3 + 1];
int HEAD_81_3 = 0;
int TAIL_81_3 = 0;
float BUFFER_82_3[__BUF_SIZE_MASK_82_3 + 1];
int HEAD_82_3 = 0;
int TAIL_82_3 = 0;
float BUFFER_83_3[__BUF_SIZE_MASK_83_3 + 1];
int HEAD_83_3 = 0;
int TAIL_83_3 = 0;
float BUFFER_84_3[__BUF_SIZE_MASK_84_3 + 1];
int HEAD_84_3 = 0;
int TAIL_84_3 = 0;
float BUFFER_85_3[__BUF_SIZE_MASK_85_3 + 1];
int HEAD_85_3 = 0;
int TAIL_85_3 = 0;
float BUFFER_86_3[__BUF_SIZE_MASK_86_3 + 1];
int HEAD_86_3 = 0;
int TAIL_86_3 = 0;
float BUFFER_87_3[__BUF_SIZE_MASK_87_3 + 1];
int HEAD_87_3 = 0;
int TAIL_87_3 = 0;
float BUFFER_88_3[__BUF_SIZE_MASK_88_3 + 1];
int HEAD_88_3 = 0;
int TAIL_88_3 = 0;
float BUFFER_89_3[__BUF_SIZE_MASK_89_3 + 1];
int HEAD_89_3 = 0;
int TAIL_89_3 = 0;
float BUFFER_90_3[__BUF_SIZE_MASK_90_3 + 1];
int HEAD_90_3 = 0;
int TAIL_90_3 = 0;
float BUFFER_91_3[__BUF_SIZE_MASK_91_3 + 1];
int HEAD_91_3 = 0;
int TAIL_91_3 = 0;
float BUFFER_92_3[__BUF_SIZE_MASK_92_3 + 1];
int HEAD_92_3 = 0;
int TAIL_92_3 = 0;
float BUFFER_93_3[__BUF_SIZE_MASK_93_3 + 1];
int HEAD_93_3 = 0;
int TAIL_93_3 = 0;
float BUFFER_94_3[__BUF_SIZE_MASK_94_3 + 1];
int HEAD_94_3 = 0;
int TAIL_94_3 = 0;
float BUFFER_95_3[__BUF_SIZE_MASK_95_3 + 1];
int HEAD_95_3 = 0;
int TAIL_95_3 = 0;
float BUFFER_96_3[__BUF_SIZE_MASK_96_3 + 1];
int HEAD_96_3 = 0;
int TAIL_96_3 = 0;
float BUFFER_97_3[__BUF_SIZE_MASK_97_3 + 1];
int HEAD_97_3 = 0;
int TAIL_97_3 = 0;
float BUFFER_98_3[__BUF_SIZE_MASK_98_3 + 1];
int HEAD_98_3 = 0;
int TAIL_98_3 = 0;
float BUFFER_99_3[__BUF_SIZE_MASK_99_3 + 1];
int HEAD_99_3 = 0;
int TAIL_99_3 = 0;
float BUFFER_100_3[__BUF_SIZE_MASK_100_3 + 1];
int HEAD_100_3 = 0;
int TAIL_100_3 = 0;
float BUFFER_101_3[__BUF_SIZE_MASK_101_3 + 1];
int HEAD_101_3 = 0;
int TAIL_101_3 = 0;
float BUFFER_102_3[__BUF_SIZE_MASK_102_3 + 1];
int HEAD_102_3 = 0;
int TAIL_102_3 = 0;
float BUFFER_103_3[__BUF_SIZE_MASK_103_3 + 1];
int HEAD_103_3 = 0;
int TAIL_103_3 = 0;
float BUFFER_104_3[__BUF_SIZE_MASK_104_3 + 1];
int HEAD_104_3 = 0;
int TAIL_104_3 = 0;
float BUFFER_105_3[__BUF_SIZE_MASK_105_3 + 1];
int HEAD_105_3 = 0;
int TAIL_105_3 = 0;
float BUFFER_106_3[__BUF_SIZE_MASK_106_3 + 1];
int HEAD_106_3 = 0;
int TAIL_106_3 = 0;
float BUFFER_107_3[__BUF_SIZE_MASK_107_3 + 1];
int HEAD_107_3 = 0;
int TAIL_107_3 = 0;
float BUFFER_108_3[__BUF_SIZE_MASK_108_3 + 1];
int HEAD_108_3 = 0;
int TAIL_108_3 = 0;
float BUFFER_109_3[__BUF_SIZE_MASK_109_3 + 1];
int HEAD_109_3 = 0;
int TAIL_109_3 = 0;
float BUFFER_110_3[__BUF_SIZE_MASK_110_3 + 1];
int HEAD_110_3 = 0;
int TAIL_110_3 = 0;
float BUFFER_111_3[__BUF_SIZE_MASK_111_3 + 1];
int HEAD_111_3 = 0;
int TAIL_111_3 = 0;
float BUFFER_112_3[__BUF_SIZE_MASK_112_3 + 1];
int HEAD_112_3 = 0;
int TAIL_112_3 = 0;
float BUFFER_113_3[__BUF_SIZE_MASK_113_3 + 1];
int HEAD_113_3 = 0;
int TAIL_113_3 = 0;
float BUFFER_114_3[__BUF_SIZE_MASK_114_3 + 1];
int HEAD_114_3 = 0;
int TAIL_114_3 = 0;
float BUFFER_115_3[__BUF_SIZE_MASK_115_3 + 1];
int HEAD_115_3 = 0;
int TAIL_115_3 = 0;
float BUFFER_116_3[__BUF_SIZE_MASK_116_3 + 1];
int HEAD_116_3 = 0;
int TAIL_116_3 = 0;
float BUFFER_117_3[__BUF_SIZE_MASK_117_3 + 1];
int HEAD_117_3 = 0;
int TAIL_117_3 = 0;
float BUFFER_118_3[__BUF_SIZE_MASK_118_3 + 1];
int HEAD_118_3 = 0;
int TAIL_118_3 = 0;
float BUFFER_119_3[__BUF_SIZE_MASK_119_3 + 1];
int HEAD_119_3 = 0;
int TAIL_119_3 = 0;
float BUFFER_120_3[__BUF_SIZE_MASK_120_3 + 1];
int HEAD_120_3 = 0;
int TAIL_120_3 = 0;
float BUFFER_121_3[__BUF_SIZE_MASK_121_3 + 1];
int HEAD_121_3 = 0;
int TAIL_121_3 = 0;
float BUFFER_122_3[__BUF_SIZE_MASK_122_3 + 1];
int HEAD_122_3 = 0;
int TAIL_122_3 = 0;
float BUFFER_123_3[__BUF_SIZE_MASK_123_3 + 1];
int HEAD_123_3 = 0;
int TAIL_123_3 = 0;
float BUFFER_124_3[__BUF_SIZE_MASK_124_3 + 1];
int HEAD_124_3 = 0;
int TAIL_124_3 = 0;
float BUFFER_125_3[__BUF_SIZE_MASK_125_3 + 1];
int HEAD_125_3 = 0;
int TAIL_125_3 = 0;
float BUFFER_126_3[__BUF_SIZE_MASK_126_3 + 1];
int HEAD_126_3 = 0;
int TAIL_126_3 = 0;
float BUFFER_127_3[__BUF_SIZE_MASK_127_3 + 1];
int HEAD_127_3 = 0;
int TAIL_127_3 = 0;
float BUFFER_128_3[__BUF_SIZE_MASK_128_3 + 1];
int HEAD_128_3 = 0;
int TAIL_128_3 = 0;
float BUFFER_129_3[__BUF_SIZE_MASK_129_3 + 1];
int HEAD_129_3 = 0;
int TAIL_129_3 = 0;
float BUFFER_130_3[__BUF_SIZE_MASK_130_3 + 1];
int HEAD_130_3 = 0;
int TAIL_130_3 = 0;
float BUFFER_131_3[__BUF_SIZE_MASK_131_3 + 1];
int HEAD_131_3 = 0;
int TAIL_131_3 = 0;
void init_OneSource__3_10__0();
void work_OneSource__3_10__0(int);
#ifdef BUFFER_MERGE
void work_OneSource__3_10__0__mod(int ____n, void *____in, float *____out);
void work_OneSource__3_10__0__mod2(int ____n, void *____in, float *____out, int s1, int s2);
#endif
void __splitter_1_work(int);
void init_AnonFilter_a0__9_11__2();
void work_AnonFilter_a0__9_11__2(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__9_11__2__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__9_11__2__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void __joiner_3_work(int);
void init_FloatPrinter__775_139__4();
void work_FloatPrinter__775_139__4(int);
#ifdef BUFFER_MERGE
void work_FloatPrinter__775_139__4__mod(int ____n, float *____in, void *____out);
void work_FloatPrinter__775_139__4__mod2(int ____n, float *____in, void *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__15_12__5();
void work_AnonFilter_a0__15_12__5(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__15_12__5__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__15_12__5__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__21_13__6();
void work_AnonFilter_a0__21_13__6(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__21_13__6__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__21_13__6__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__27_14__7();
void work_AnonFilter_a0__27_14__7(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__27_14__7__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__27_14__7__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__33_15__8();
void work_AnonFilter_a0__33_15__8(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__33_15__8__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__33_15__8__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__39_16__9();
void work_AnonFilter_a0__39_16__9(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__39_16__9__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__39_16__9__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__45_17__10();
void work_AnonFilter_a0__45_17__10(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__45_17__10__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__45_17__10__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__51_18__11();
void work_AnonFilter_a0__51_18__11(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__51_18__11__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__51_18__11__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__57_19__12();
void work_AnonFilter_a0__57_19__12(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__57_19__12__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__57_19__12__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__63_20__13();
void work_AnonFilter_a0__63_20__13(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__63_20__13__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__63_20__13__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__69_21__14();
void work_AnonFilter_a0__69_21__14(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__69_21__14__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__69_21__14__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__75_22__15();
void work_AnonFilter_a0__75_22__15(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__75_22__15__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__75_22__15__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__81_23__16();
void work_AnonFilter_a0__81_23__16(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__81_23__16__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__81_23__16__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__87_24__17();
void work_AnonFilter_a0__87_24__17(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__87_24__17__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__87_24__17__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__93_25__18();
void work_AnonFilter_a0__93_25__18(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__93_25__18__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__93_25__18__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__99_26__19();
void work_AnonFilter_a0__99_26__19(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__99_26__19__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__99_26__19__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__105_27__20();
void work_AnonFilter_a0__105_27__20(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__105_27__20__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__105_27__20__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__111_28__21();
void work_AnonFilter_a0__111_28__21(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__111_28__21__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__111_28__21__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__117_29__22();
void work_AnonFilter_a0__117_29__22(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__117_29__22__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__117_29__22__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__123_30__23();
void work_AnonFilter_a0__123_30__23(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__123_30__23__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__123_30__23__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__129_31__24();
void work_AnonFilter_a0__129_31__24(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__129_31__24__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__129_31__24__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__135_32__25();
void work_AnonFilter_a0__135_32__25(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__135_32__25__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__135_32__25__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__141_33__26();
void work_AnonFilter_a0__141_33__26(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__141_33__26__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__141_33__26__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__147_34__27();
void work_AnonFilter_a0__147_34__27(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__147_34__27__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__147_34__27__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__153_35__28();
void work_AnonFilter_a0__153_35__28(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__153_35__28__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__153_35__28__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__159_36__29();
void work_AnonFilter_a0__159_36__29(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__159_36__29__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__159_36__29__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__165_37__30();
void work_AnonFilter_a0__165_37__30(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__165_37__30__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__165_37__30__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__171_38__31();
void work_AnonFilter_a0__171_38__31(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__171_38__31__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__171_38__31__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__177_39__32();
void work_AnonFilter_a0__177_39__32(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__177_39__32__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__177_39__32__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__183_40__33();
void work_AnonFilter_a0__183_40__33(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__183_40__33__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__183_40__33__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__189_41__34();
void work_AnonFilter_a0__189_41__34(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__189_41__34__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__189_41__34__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__195_42__35();
void work_AnonFilter_a0__195_42__35(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__195_42__35__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__195_42__35__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__201_43__36();
void work_AnonFilter_a0__201_43__36(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__201_43__36__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__201_43__36__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__207_44__37();
void work_AnonFilter_a0__207_44__37(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__207_44__37__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__207_44__37__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__213_45__38();
void work_AnonFilter_a0__213_45__38(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__213_45__38__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__213_45__38__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__219_46__39();
void work_AnonFilter_a0__219_46__39(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__219_46__39__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__219_46__39__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__225_47__40();
void work_AnonFilter_a0__225_47__40(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__225_47__40__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__225_47__40__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__231_48__41();
void work_AnonFilter_a0__231_48__41(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__231_48__41__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__231_48__41__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__237_49__42();
void work_AnonFilter_a0__237_49__42(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__237_49__42__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__237_49__42__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__243_50__43();
void work_AnonFilter_a0__243_50__43(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__243_50__43__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__243_50__43__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__249_51__44();
void work_AnonFilter_a0__249_51__44(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__249_51__44__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__249_51__44__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__255_52__45();
void work_AnonFilter_a0__255_52__45(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__255_52__45__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__255_52__45__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__261_53__46();
void work_AnonFilter_a0__261_53__46(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__261_53__46__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__261_53__46__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__267_54__47();
void work_AnonFilter_a0__267_54__47(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__267_54__47__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__267_54__47__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__273_55__48();
void work_AnonFilter_a0__273_55__48(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__273_55__48__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__273_55__48__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__279_56__49();
void work_AnonFilter_a0__279_56__49(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__279_56__49__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__279_56__49__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__285_57__50();
void work_AnonFilter_a0__285_57__50(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__285_57__50__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__285_57__50__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__291_58__51();
void work_AnonFilter_a0__291_58__51(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__291_58__51__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__291_58__51__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__297_59__52();
void work_AnonFilter_a0__297_59__52(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__297_59__52__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__297_59__52__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__303_60__53();
void work_AnonFilter_a0__303_60__53(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__303_60__53__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__303_60__53__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__309_61__54();
void work_AnonFilter_a0__309_61__54(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__309_61__54__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__309_61__54__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__315_62__55();
void work_AnonFilter_a0__315_62__55(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__315_62__55__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__315_62__55__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__321_63__56();
void work_AnonFilter_a0__321_63__56(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__321_63__56__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__321_63__56__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__327_64__57();
void work_AnonFilter_a0__327_64__57(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__327_64__57__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__327_64__57__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__333_65__58();
void work_AnonFilter_a0__333_65__58(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__333_65__58__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__333_65__58__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__339_66__59();
void work_AnonFilter_a0__339_66__59(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__339_66__59__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__339_66__59__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__345_67__60();
void work_AnonFilter_a0__345_67__60(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__345_67__60__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__345_67__60__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__351_68__61();
void work_AnonFilter_a0__351_68__61(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__351_68__61__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__351_68__61__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__357_69__62();
void work_AnonFilter_a0__357_69__62(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__357_69__62__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__357_69__62__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__363_70__63();
void work_AnonFilter_a0__363_70__63(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__363_70__63__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__363_70__63__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__369_71__64();
void work_AnonFilter_a0__369_71__64(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__369_71__64__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__369_71__64__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__375_72__65();
void work_AnonFilter_a0__375_72__65(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__375_72__65__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__375_72__65__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__381_73__66();
void work_AnonFilter_a0__381_73__66(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__381_73__66__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__381_73__66__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__387_74__67();
void work_AnonFilter_a0__387_74__67(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__387_74__67__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__387_74__67__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__393_75__68();
void work_AnonFilter_a0__393_75__68(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__393_75__68__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__393_75__68__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__399_76__69();
void work_AnonFilter_a0__399_76__69(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__399_76__69__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__399_76__69__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__405_77__70();
void work_AnonFilter_a0__405_77__70(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__405_77__70__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__405_77__70__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__411_78__71();
void work_AnonFilter_a0__411_78__71(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__411_78__71__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__411_78__71__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__417_79__72();
void work_AnonFilter_a0__417_79__72(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__417_79__72__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__417_79__72__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__423_80__73();
void work_AnonFilter_a0__423_80__73(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__423_80__73__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__423_80__73__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__429_81__74();
void work_AnonFilter_a0__429_81__74(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__429_81__74__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__429_81__74__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__435_82__75();
void work_AnonFilter_a0__435_82__75(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__435_82__75__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__435_82__75__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__441_83__76();
void work_AnonFilter_a0__441_83__76(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__441_83__76__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__441_83__76__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__447_84__77();
void work_AnonFilter_a0__447_84__77(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__447_84__77__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__447_84__77__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__453_85__78();
void work_AnonFilter_a0__453_85__78(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__453_85__78__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__453_85__78__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__459_86__79();
void work_AnonFilter_a0__459_86__79(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__459_86__79__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__459_86__79__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__465_87__80();
void work_AnonFilter_a0__465_87__80(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__465_87__80__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__465_87__80__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__471_88__81();
void work_AnonFilter_a0__471_88__81(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__471_88__81__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__471_88__81__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__477_89__82();
void work_AnonFilter_a0__477_89__82(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__477_89__82__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__477_89__82__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__483_90__83();
void work_AnonFilter_a0__483_90__83(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__483_90__83__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__483_90__83__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__489_91__84();
void work_AnonFilter_a0__489_91__84(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__489_91__84__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__489_91__84__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__495_92__85();
void work_AnonFilter_a0__495_92__85(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__495_92__85__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__495_92__85__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__501_93__86();
void work_AnonFilter_a0__501_93__86(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__501_93__86__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__501_93__86__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__507_94__87();
void work_AnonFilter_a0__507_94__87(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__507_94__87__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__507_94__87__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__513_95__88();
void work_AnonFilter_a0__513_95__88(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__513_95__88__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__513_95__88__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__519_96__89();
void work_AnonFilter_a0__519_96__89(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__519_96__89__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__519_96__89__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__525_97__90();
void work_AnonFilter_a0__525_97__90(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__525_97__90__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__525_97__90__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__531_98__91();
void work_AnonFilter_a0__531_98__91(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__531_98__91__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__531_98__91__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__537_99__92();
void work_AnonFilter_a0__537_99__92(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__537_99__92__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__537_99__92__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__543_100__93();
void work_AnonFilter_a0__543_100__93(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__543_100__93__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__543_100__93__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__549_101__94();
void work_AnonFilter_a0__549_101__94(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__549_101__94__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__549_101__94__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__555_102__95();
void work_AnonFilter_a0__555_102__95(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__555_102__95__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__555_102__95__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__561_103__96();
void work_AnonFilter_a0__561_103__96(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__561_103__96__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__561_103__96__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__567_104__97();
void work_AnonFilter_a0__567_104__97(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__567_104__97__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__567_104__97__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__573_105__98();
void work_AnonFilter_a0__573_105__98(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__573_105__98__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__573_105__98__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__579_106__99();
void work_AnonFilter_a0__579_106__99(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__579_106__99__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__579_106__99__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__585_107__100();
void work_AnonFilter_a0__585_107__100(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__585_107__100__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__585_107__100__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__591_108__101();
void work_AnonFilter_a0__591_108__101(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__591_108__101__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__591_108__101__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__597_109__102();
void work_AnonFilter_a0__597_109__102(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__597_109__102__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__597_109__102__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__603_110__103();
void work_AnonFilter_a0__603_110__103(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__603_110__103__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__603_110__103__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__609_111__104();
void work_AnonFilter_a0__609_111__104(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__609_111__104__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__609_111__104__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__615_112__105();
void work_AnonFilter_a0__615_112__105(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__615_112__105__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__615_112__105__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__621_113__106();
void work_AnonFilter_a0__621_113__106(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__621_113__106__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__621_113__106__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__627_114__107();
void work_AnonFilter_a0__627_114__107(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__627_114__107__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__627_114__107__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__633_115__108();
void work_AnonFilter_a0__633_115__108(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__633_115__108__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__633_115__108__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__639_116__109();
void work_AnonFilter_a0__639_116__109(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__639_116__109__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__639_116__109__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__645_117__110();
void work_AnonFilter_a0__645_117__110(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__645_117__110__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__645_117__110__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__651_118__111();
void work_AnonFilter_a0__651_118__111(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__651_118__111__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__651_118__111__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__657_119__112();
void work_AnonFilter_a0__657_119__112(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__657_119__112__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__657_119__112__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__663_120__113();
void work_AnonFilter_a0__663_120__113(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__663_120__113__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__663_120__113__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__669_121__114();
void work_AnonFilter_a0__669_121__114(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__669_121__114__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__669_121__114__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__675_122__115();
void work_AnonFilter_a0__675_122__115(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__675_122__115__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__675_122__115__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__681_123__116();
void work_AnonFilter_a0__681_123__116(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__681_123__116__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__681_123__116__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__687_124__117();
void work_AnonFilter_a0__687_124__117(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__687_124__117__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__687_124__117__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__693_125__118();
void work_AnonFilter_a0__693_125__118(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__693_125__118__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__693_125__118__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__699_126__119();
void work_AnonFilter_a0__699_126__119(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__699_126__119__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__699_126__119__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__705_127__120();
void work_AnonFilter_a0__705_127__120(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__705_127__120__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__705_127__120__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__711_128__121();
void work_AnonFilter_a0__711_128__121(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__711_128__121__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__711_128__121__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__717_129__122();
void work_AnonFilter_a0__717_129__122(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__717_129__122__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__717_129__122__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__723_130__123();
void work_AnonFilter_a0__723_130__123(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__723_130__123__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__723_130__123__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__729_131__124();
void work_AnonFilter_a0__729_131__124(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__729_131__124__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__729_131__124__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__735_132__125();
void work_AnonFilter_a0__735_132__125(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__735_132__125__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__735_132__125__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__741_133__126();
void work_AnonFilter_a0__741_133__126(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__741_133__126__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__741_133__126__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__747_134__127();
void work_AnonFilter_a0__747_134__127(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__747_134__127__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__747_134__127__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__753_135__128();
void work_AnonFilter_a0__753_135__128(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__753_135__128__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__753_135__128__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__759_136__129();
void work_AnonFilter_a0__759_136__129(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__759_136__129__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__759_136__129__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__765_137__130();
void work_AnonFilter_a0__765_137__130(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__765_137__130__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__765_137__130__mod2(int ____n, float *____in, float *____out, int s1, int s2);
#endif
void init_AnonFilter_a0__771_138__131();
void work_AnonFilter_a0__771_138__131(int);
#ifdef BUFFER_MERGE
void work_AnonFilter_a0__771_138__131__mod(int ____n, float *____in, float *____out);
void work_AnonFilter_a0__771_138__131__mod2(int ____n, float *____in, float *____out, int s1, int s2);
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
// number of phases: 5


  // ============= Initialization =============

init_OneSource__3_10__0();
init_AnonFilter_a0__9_11__2();
init_AnonFilter_a0__15_12__5();
init_AnonFilter_a0__21_13__6();
init_AnonFilter_a0__27_14__7();
init_AnonFilter_a0__33_15__8();
init_AnonFilter_a0__45_17__10();
init_AnonFilter_a0__39_16__9();
init_AnonFilter_a0__57_19__12();
init_AnonFilter_a0__51_18__11();
init_AnonFilter_a0__69_21__14();
init_AnonFilter_a0__63_20__13();
init_AnonFilter_a0__81_23__16();
init_AnonFilter_a0__75_22__15();
init_AnonFilter_a0__93_25__18();
init_AnonFilter_a0__87_24__17();
init_AnonFilter_a0__105_27__20();
init_AnonFilter_a0__99_26__19();
init_AnonFilter_a0__117_29__22();
init_AnonFilter_a0__111_28__21();
init_AnonFilter_a0__129_31__24();
init_AnonFilter_a0__123_30__23();
init_AnonFilter_a0__147_34__27();
init_AnonFilter_a0__153_35__28();
init_AnonFilter_a0__135_32__25();
init_AnonFilter_a0__141_33__26();
init_AnonFilter_a0__171_38__31();
init_AnonFilter_a0__177_39__32();
init_AnonFilter_a0__159_36__29();
init_AnonFilter_a0__165_37__30();
init_AnonFilter_a0__195_42__35();
init_AnonFilter_a0__201_43__36();
init_AnonFilter_a0__183_40__33();
init_AnonFilter_a0__189_41__34();
init_AnonFilter_a0__219_46__39();
init_AnonFilter_a0__225_47__40();
init_AnonFilter_a0__207_44__37();
init_AnonFilter_a0__213_45__38();
init_AnonFilter_a0__249_51__44();
init_AnonFilter_a0__243_50__43();
init_AnonFilter_a0__237_49__42();
init_AnonFilter_a0__231_48__41();
init_AnonFilter_a0__273_55__48();
init_AnonFilter_a0__267_54__47();
init_AnonFilter_a0__261_53__46();
init_AnonFilter_a0__255_52__45();
init_AnonFilter_a0__297_59__52();
init_AnonFilter_a0__291_58__51();
init_AnonFilter_a0__285_57__50();
init_AnonFilter_a0__279_56__49();
init_AnonFilter_a0__321_63__56();
init_AnonFilter_a0__315_62__55();
init_AnonFilter_a0__309_61__54();
init_AnonFilter_a0__303_60__53();
init_AnonFilter_a0__351_68__61();
init_AnonFilter_a0__357_69__62();
init_AnonFilter_a0__363_70__63();
init_AnonFilter_a0__369_71__64();
init_AnonFilter_a0__327_64__57();
init_AnonFilter_a0__333_65__58();
init_AnonFilter_a0__339_66__59();
init_AnonFilter_a0__345_67__60();
init_AnonFilter_a0__399_76__69();
init_AnonFilter_a0__405_77__70();
init_AnonFilter_a0__411_78__71();
init_AnonFilter_a0__417_79__72();
init_AnonFilter_a0__375_72__65();
init_AnonFilter_a0__381_73__66();
init_AnonFilter_a0__387_74__67();
init_AnonFilter_a0__393_75__68();
init_AnonFilter_a0__453_85__78();
init_AnonFilter_a0__447_84__77();
init_AnonFilter_a0__465_87__80();
init_AnonFilter_a0__459_86__79();
init_AnonFilter_a0__429_81__74();
init_AnonFilter_a0__423_80__73();
init_AnonFilter_a0__441_83__76();
init_AnonFilter_a0__435_82__75();
init_AnonFilter_a0__501_93__86();
init_AnonFilter_a0__495_92__85();
init_AnonFilter_a0__513_95__88();
init_AnonFilter_a0__507_94__87();
init_AnonFilter_a0__477_89__82();
init_AnonFilter_a0__471_88__81();
init_AnonFilter_a0__489_91__84();
init_AnonFilter_a0__483_90__83();
init_AnonFilter_a0__555_102__95();
init_AnonFilter_a0__561_103__96();
init_AnonFilter_a0__543_100__93();
init_AnonFilter_a0__549_101__94();
init_AnonFilter_a0__531_98__91();
init_AnonFilter_a0__537_99__92();
init_AnonFilter_a0__519_96__89();
init_AnonFilter_a0__525_97__90();
init_AnonFilter_a0__603_110__103();
init_AnonFilter_a0__609_111__104();
init_AnonFilter_a0__591_108__101();
init_AnonFilter_a0__597_109__102();
init_AnonFilter_a0__579_106__99();
init_AnonFilter_a0__585_107__100();
init_AnonFilter_a0__567_104__97();
init_AnonFilter_a0__573_105__98();
init_AnonFilter_a0__657_119__112();
init_AnonFilter_a0__651_118__111();
init_AnonFilter_a0__645_117__110();
init_AnonFilter_a0__639_116__109();
init_AnonFilter_a0__633_115__108();
init_AnonFilter_a0__627_114__107();
init_AnonFilter_a0__621_113__106();
init_AnonFilter_a0__615_112__105();
init_AnonFilter_a0__705_127__120();
init_AnonFilter_a0__699_126__119();
init_AnonFilter_a0__693_125__118();
init_AnonFilter_a0__687_124__117();
init_AnonFilter_a0__681_123__116();
init_AnonFilter_a0__675_122__115();
init_AnonFilter_a0__669_121__114();
init_AnonFilter_a0__663_120__113();
init_AnonFilter_a0__765_137__130();
init_AnonFilter_a0__759_136__129();
init_AnonFilter_a0__771_138__131();
init_AnonFilter_a0__717_129__122();
init_AnonFilter_a0__711_128__121();
init_AnonFilter_a0__729_131__124();
init_AnonFilter_a0__723_130__123();
init_AnonFilter_a0__741_133__126();
init_AnonFilter_a0__735_132__125();
init_AnonFilter_a0__753_135__128();
init_AnonFilter_a0__747_134__127();
init_FloatPrinter__775_139__4();

#ifdef MEASURE_BY_RTS
RTS_init(1,1);
#else //default ``MEASURE_BY_PAPI''
PAPI_HW_COUNTER_init(1,1);
#endif

  // ============= Steady State =============

#ifdef MEASURE_BY_RTS
RTS_thread_init(0,1);
#else //default ``MEASURE_BY_PAPI''
PAPI_HW_COUNTER_thread_init(0,1);
PAPI_HW_COUNTER_open(0);
#endif
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
    work_OneSource__3_10__0(32 );
HEAD_1_2 = 0;
TAIL_1_2 = 0;
HEAD_1_5 = 0;
TAIL_1_5 = 0;
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
HEAD_1_22 = 0;
TAIL_1_22 = 0;
HEAD_1_23 = 0;
TAIL_1_23 = 0;
HEAD_1_24 = 0;
TAIL_1_24 = 0;
HEAD_1_25 = 0;
TAIL_1_25 = 0;
HEAD_1_26 = 0;
TAIL_1_26 = 0;
HEAD_1_27 = 0;
TAIL_1_27 = 0;
HEAD_1_28 = 0;
TAIL_1_28 = 0;
HEAD_1_29 = 0;
TAIL_1_29 = 0;
HEAD_1_30 = 0;
TAIL_1_30 = 0;
HEAD_1_31 = 0;
TAIL_1_31 = 0;
HEAD_1_32 = 0;
TAIL_1_32 = 0;
HEAD_1_33 = 0;
TAIL_1_33 = 0;
HEAD_1_34 = 0;
TAIL_1_34 = 0;
HEAD_1_35 = 0;
TAIL_1_35 = 0;
HEAD_1_36 = 0;
TAIL_1_36 = 0;
HEAD_1_37 = 0;
TAIL_1_37 = 0;
HEAD_1_38 = 0;
TAIL_1_38 = 0;
HEAD_1_39 = 0;
TAIL_1_39 = 0;
HEAD_1_40 = 0;
TAIL_1_40 = 0;
HEAD_1_41 = 0;
TAIL_1_41 = 0;
HEAD_1_42 = 0;
TAIL_1_42 = 0;
HEAD_1_43 = 0;
TAIL_1_43 = 0;
HEAD_1_44 = 0;
TAIL_1_44 = 0;
HEAD_1_45 = 0;
TAIL_1_45 = 0;
HEAD_1_46 = 0;
TAIL_1_46 = 0;
HEAD_1_47 = 0;
TAIL_1_47 = 0;
HEAD_1_48 = 0;
TAIL_1_48 = 0;
HEAD_1_49 = 0;
TAIL_1_49 = 0;
HEAD_1_50 = 0;
TAIL_1_50 = 0;
HEAD_1_51 = 0;
TAIL_1_51 = 0;
HEAD_1_52 = 0;
TAIL_1_52 = 0;
HEAD_1_53 = 0;
TAIL_1_53 = 0;
HEAD_1_54 = 0;
TAIL_1_54 = 0;
HEAD_1_55 = 0;
TAIL_1_55 = 0;
HEAD_1_56 = 0;
TAIL_1_56 = 0;
HEAD_1_57 = 0;
TAIL_1_57 = 0;
HEAD_1_58 = 0;
TAIL_1_58 = 0;
HEAD_1_59 = 0;
TAIL_1_59 = 0;
HEAD_1_60 = 0;
TAIL_1_60 = 0;
HEAD_1_61 = 0;
TAIL_1_61 = 0;
HEAD_1_62 = 0;
TAIL_1_62 = 0;
HEAD_1_63 = 0;
TAIL_1_63 = 0;
HEAD_1_64 = 0;
TAIL_1_64 = 0;
HEAD_1_65 = 0;
TAIL_1_65 = 0;
HEAD_1_66 = 0;
TAIL_1_66 = 0;
HEAD_1_67 = 0;
TAIL_1_67 = 0;
HEAD_1_68 = 0;
TAIL_1_68 = 0;
HEAD_1_69 = 0;
TAIL_1_69 = 0;
HEAD_1_70 = 0;
TAIL_1_70 = 0;
HEAD_1_71 = 0;
TAIL_1_71 = 0;
HEAD_1_72 = 0;
TAIL_1_72 = 0;
HEAD_1_73 = 0;
TAIL_1_73 = 0;
HEAD_1_74 = 0;
TAIL_1_74 = 0;
HEAD_1_75 = 0;
TAIL_1_75 = 0;
HEAD_1_76 = 0;
TAIL_1_76 = 0;
HEAD_1_77 = 0;
TAIL_1_77 = 0;
HEAD_1_78 = 0;
TAIL_1_78 = 0;
HEAD_1_79 = 0;
TAIL_1_79 = 0;
HEAD_1_80 = 0;
TAIL_1_80 = 0;
HEAD_1_81 = 0;
TAIL_1_81 = 0;
HEAD_1_82 = 0;
TAIL_1_82 = 0;
HEAD_1_83 = 0;
TAIL_1_83 = 0;
HEAD_1_84 = 0;
TAIL_1_84 = 0;
HEAD_1_85 = 0;
TAIL_1_85 = 0;
HEAD_1_86 = 0;
TAIL_1_86 = 0;
HEAD_1_87 = 0;
TAIL_1_87 = 0;
HEAD_1_88 = 0;
TAIL_1_88 = 0;
HEAD_1_89 = 0;
TAIL_1_89 = 0;
HEAD_1_90 = 0;
TAIL_1_90 = 0;
HEAD_1_91 = 0;
TAIL_1_91 = 0;
HEAD_1_92 = 0;
TAIL_1_92 = 0;
HEAD_1_93 = 0;
TAIL_1_93 = 0;
HEAD_1_94 = 0;
TAIL_1_94 = 0;
HEAD_1_95 = 0;
TAIL_1_95 = 0;
HEAD_1_96 = 0;
TAIL_1_96 = 0;
HEAD_1_97 = 0;
TAIL_1_97 = 0;
HEAD_1_98 = 0;
TAIL_1_98 = 0;
HEAD_1_99 = 0;
TAIL_1_99 = 0;
HEAD_1_100 = 0;
TAIL_1_100 = 0;
HEAD_1_101 = 0;
TAIL_1_101 = 0;
HEAD_1_102 = 0;
TAIL_1_102 = 0;
HEAD_1_103 = 0;
TAIL_1_103 = 0;
HEAD_1_104 = 0;
TAIL_1_104 = 0;
HEAD_1_105 = 0;
TAIL_1_105 = 0;
HEAD_1_106 = 0;
TAIL_1_106 = 0;
HEAD_1_107 = 0;
TAIL_1_107 = 0;
HEAD_1_108 = 0;
TAIL_1_108 = 0;
HEAD_1_109 = 0;
TAIL_1_109 = 0;
HEAD_1_110 = 0;
TAIL_1_110 = 0;
HEAD_1_111 = 0;
TAIL_1_111 = 0;
HEAD_1_112 = 0;
TAIL_1_112 = 0;
HEAD_1_113 = 0;
TAIL_1_113 = 0;
HEAD_1_114 = 0;
TAIL_1_114 = 0;
HEAD_1_115 = 0;
TAIL_1_115 = 0;
HEAD_1_116 = 0;
TAIL_1_116 = 0;
HEAD_1_117 = 0;
TAIL_1_117 = 0;
HEAD_1_118 = 0;
TAIL_1_118 = 0;
HEAD_1_119 = 0;
TAIL_1_119 = 0;
HEAD_1_120 = 0;
TAIL_1_120 = 0;
HEAD_1_121 = 0;
TAIL_1_121 = 0;
HEAD_1_122 = 0;
TAIL_1_122 = 0;
HEAD_1_123 = 0;
TAIL_1_123 = 0;
HEAD_1_124 = 0;
TAIL_1_124 = 0;
HEAD_1_125 = 0;
TAIL_1_125 = 0;
HEAD_1_126 = 0;
TAIL_1_126 = 0;
HEAD_1_127 = 0;
TAIL_1_127 = 0;
HEAD_1_128 = 0;
TAIL_1_128 = 0;
HEAD_1_129 = 0;
TAIL_1_129 = 0;
HEAD_1_130 = 0;
TAIL_1_130 = 0;
HEAD_1_131 = 0;
TAIL_1_131 = 0;
    __splitter_1_work(32 );
HEAD_2_3 = 0;
TAIL_2_3 = 0;
    work_AnonFilter_a0__9_11__2(1 );
HEAD_5_3 = 0;
TAIL_5_3 = 0;
    work_AnonFilter_a0__15_12__5(1 );
HEAD_6_3 = 0;
TAIL_6_3 = 0;
    work_AnonFilter_a0__21_13__6(1 );
HEAD_7_3 = 0;
TAIL_7_3 = 0;
    work_AnonFilter_a0__27_14__7(1 );
HEAD_8_3 = 0;
TAIL_8_3 = 0;
    work_AnonFilter_a0__33_15__8(1 );
HEAD_10_3 = 0;
TAIL_10_3 = 0;
    work_AnonFilter_a0__45_17__10(1 );
HEAD_9_3 = 0;
TAIL_9_3 = 0;
    work_AnonFilter_a0__39_16__9(1 );
HEAD_12_3 = 0;
TAIL_12_3 = 0;
    work_AnonFilter_a0__57_19__12(1 );
HEAD_11_3 = 0;
TAIL_11_3 = 0;
    work_AnonFilter_a0__51_18__11(1 );
HEAD_14_3 = 0;
TAIL_14_3 = 0;
    work_AnonFilter_a0__69_21__14(1 );
HEAD_13_3 = 0;
TAIL_13_3 = 0;
    work_AnonFilter_a0__63_20__13(1 );
HEAD_16_3 = 0;
TAIL_16_3 = 0;
    work_AnonFilter_a0__81_23__16(1 );
HEAD_15_3 = 0;
TAIL_15_3 = 0;
    work_AnonFilter_a0__75_22__15(1 );
HEAD_18_3 = 0;
TAIL_18_3 = 0;
    work_AnonFilter_a0__93_25__18(1 );
HEAD_17_3 = 0;
TAIL_17_3 = 0;
    work_AnonFilter_a0__87_24__17(1 );
HEAD_20_3 = 0;
TAIL_20_3 = 0;
    work_AnonFilter_a0__105_27__20(1 );
HEAD_19_3 = 0;
TAIL_19_3 = 0;
    work_AnonFilter_a0__99_26__19(1 );
HEAD_22_3 = 0;
TAIL_22_3 = 0;
    work_AnonFilter_a0__117_29__22(1 );
HEAD_21_3 = 0;
TAIL_21_3 = 0;
    work_AnonFilter_a0__111_28__21(1 );
HEAD_24_3 = 0;
TAIL_24_3 = 0;
    work_AnonFilter_a0__129_31__24(1 );
HEAD_23_3 = 0;
TAIL_23_3 = 0;
    work_AnonFilter_a0__123_30__23(1 );
HEAD_27_3 = 0;
TAIL_27_3 = 0;
    work_AnonFilter_a0__147_34__27(1 );
HEAD_28_3 = 0;
TAIL_28_3 = 0;
    work_AnonFilter_a0__153_35__28(1 );
HEAD_25_3 = 0;
TAIL_25_3 = 0;
    work_AnonFilter_a0__135_32__25(1 );
HEAD_26_3 = 0;
TAIL_26_3 = 0;
    work_AnonFilter_a0__141_33__26(1 );
HEAD_31_3 = 0;
TAIL_31_3 = 0;
    work_AnonFilter_a0__171_38__31(1 );
HEAD_32_3 = 0;
TAIL_32_3 = 0;
    work_AnonFilter_a0__177_39__32(1 );
HEAD_29_3 = 0;
TAIL_29_3 = 0;
    work_AnonFilter_a0__159_36__29(1 );
HEAD_30_3 = 0;
TAIL_30_3 = 0;
    work_AnonFilter_a0__165_37__30(1 );
HEAD_35_3 = 0;
TAIL_35_3 = 0;
    work_AnonFilter_a0__195_42__35(1 );
HEAD_36_3 = 0;
TAIL_36_3 = 0;
    work_AnonFilter_a0__201_43__36(1 );
HEAD_33_3 = 0;
TAIL_33_3 = 0;
    work_AnonFilter_a0__183_40__33(1 );
HEAD_34_3 = 0;
TAIL_34_3 = 0;
    work_AnonFilter_a0__189_41__34(1 );
HEAD_39_3 = 0;
TAIL_39_3 = 0;
    work_AnonFilter_a0__219_46__39(1 );
HEAD_40_3 = 0;
TAIL_40_3 = 0;
    work_AnonFilter_a0__225_47__40(1 );
HEAD_37_3 = 0;
TAIL_37_3 = 0;
    work_AnonFilter_a0__207_44__37(1 );
HEAD_38_3 = 0;
TAIL_38_3 = 0;
    work_AnonFilter_a0__213_45__38(1 );
HEAD_44_3 = 0;
TAIL_44_3 = 0;
    work_AnonFilter_a0__249_51__44(1 );
HEAD_43_3 = 0;
TAIL_43_3 = 0;
    work_AnonFilter_a0__243_50__43(1 );
HEAD_42_3 = 0;
TAIL_42_3 = 0;
    work_AnonFilter_a0__237_49__42(1 );
HEAD_41_3 = 0;
TAIL_41_3 = 0;
    work_AnonFilter_a0__231_48__41(1 );
HEAD_48_3 = 0;
TAIL_48_3 = 0;
    work_AnonFilter_a0__273_55__48(1 );
HEAD_47_3 = 0;
TAIL_47_3 = 0;
    work_AnonFilter_a0__267_54__47(1 );
HEAD_46_3 = 0;
TAIL_46_3 = 0;
    work_AnonFilter_a0__261_53__46(1 );
HEAD_45_3 = 0;
TAIL_45_3 = 0;
    work_AnonFilter_a0__255_52__45(1 );
HEAD_52_3 = 0;
TAIL_52_3 = 0;
    work_AnonFilter_a0__297_59__52(1 );
HEAD_51_3 = 0;
TAIL_51_3 = 0;
    work_AnonFilter_a0__291_58__51(1 );
HEAD_50_3 = 0;
TAIL_50_3 = 0;
    work_AnonFilter_a0__285_57__50(1 );
HEAD_49_3 = 0;
TAIL_49_3 = 0;
    work_AnonFilter_a0__279_56__49(1 );
HEAD_56_3 = 0;
TAIL_56_3 = 0;
    work_AnonFilter_a0__321_63__56(1 );
HEAD_55_3 = 0;
TAIL_55_3 = 0;
    work_AnonFilter_a0__315_62__55(1 );
HEAD_54_3 = 0;
TAIL_54_3 = 0;
    work_AnonFilter_a0__309_61__54(1 );
HEAD_53_3 = 0;
TAIL_53_3 = 0;
    work_AnonFilter_a0__303_60__53(1 );
HEAD_61_3 = 0;
TAIL_61_3 = 0;
    work_AnonFilter_a0__351_68__61(1 );
HEAD_62_3 = 0;
TAIL_62_3 = 0;
    work_AnonFilter_a0__357_69__62(1 );
HEAD_63_3 = 0;
TAIL_63_3 = 0;
    work_AnonFilter_a0__363_70__63(1 );
HEAD_64_3 = 0;
TAIL_64_3 = 0;
    work_AnonFilter_a0__369_71__64(1 );
HEAD_57_3 = 0;
TAIL_57_3 = 0;
    work_AnonFilter_a0__327_64__57(1 );
HEAD_58_3 = 0;
TAIL_58_3 = 0;
    work_AnonFilter_a0__333_65__58(1 );
HEAD_59_3 = 0;
TAIL_59_3 = 0;
    work_AnonFilter_a0__339_66__59(1 );
HEAD_60_3 = 0;
TAIL_60_3 = 0;
    work_AnonFilter_a0__345_67__60(1 );
HEAD_69_3 = 0;
TAIL_69_3 = 0;
    work_AnonFilter_a0__399_76__69(1 );
HEAD_70_3 = 0;
TAIL_70_3 = 0;
    work_AnonFilter_a0__405_77__70(1 );
HEAD_71_3 = 0;
TAIL_71_3 = 0;
    work_AnonFilter_a0__411_78__71(1 );
HEAD_72_3 = 0;
TAIL_72_3 = 0;
    work_AnonFilter_a0__417_79__72(1 );
HEAD_65_3 = 0;
TAIL_65_3 = 0;
    work_AnonFilter_a0__375_72__65(1 );
HEAD_66_3 = 0;
TAIL_66_3 = 0;
    work_AnonFilter_a0__381_73__66(1 );
HEAD_67_3 = 0;
TAIL_67_3 = 0;
    work_AnonFilter_a0__387_74__67(1 );
HEAD_68_3 = 0;
TAIL_68_3 = 0;
    work_AnonFilter_a0__393_75__68(1 );
HEAD_78_3 = 0;
TAIL_78_3 = 0;
    work_AnonFilter_a0__453_85__78(1 );
HEAD_77_3 = 0;
TAIL_77_3 = 0;
    work_AnonFilter_a0__447_84__77(1 );
HEAD_80_3 = 0;
TAIL_80_3 = 0;
    work_AnonFilter_a0__465_87__80(1 );
HEAD_79_3 = 0;
TAIL_79_3 = 0;
    work_AnonFilter_a0__459_86__79(1 );
HEAD_74_3 = 0;
TAIL_74_3 = 0;
    work_AnonFilter_a0__429_81__74(1 );
HEAD_73_3 = 0;
TAIL_73_3 = 0;
    work_AnonFilter_a0__423_80__73(1 );
HEAD_76_3 = 0;
TAIL_76_3 = 0;
    work_AnonFilter_a0__441_83__76(1 );
HEAD_75_3 = 0;
TAIL_75_3 = 0;
    work_AnonFilter_a0__435_82__75(1 );
HEAD_86_3 = 0;
TAIL_86_3 = 0;
    work_AnonFilter_a0__501_93__86(1 );
HEAD_85_3 = 0;
TAIL_85_3 = 0;
    work_AnonFilter_a0__495_92__85(1 );
HEAD_88_3 = 0;
TAIL_88_3 = 0;
    work_AnonFilter_a0__513_95__88(1 );
HEAD_87_3 = 0;
TAIL_87_3 = 0;
    work_AnonFilter_a0__507_94__87(1 );
HEAD_82_3 = 0;
TAIL_82_3 = 0;
    work_AnonFilter_a0__477_89__82(1 );
HEAD_81_3 = 0;
TAIL_81_3 = 0;
    work_AnonFilter_a0__471_88__81(1 );
HEAD_84_3 = 0;
TAIL_84_3 = 0;
    work_AnonFilter_a0__489_91__84(1 );
HEAD_83_3 = 0;
TAIL_83_3 = 0;
    work_AnonFilter_a0__483_90__83(1 );
HEAD_95_3 = 0;
TAIL_95_3 = 0;
    work_AnonFilter_a0__555_102__95(1 );
HEAD_96_3 = 0;
TAIL_96_3 = 0;
    work_AnonFilter_a0__561_103__96(1 );
HEAD_93_3 = 0;
TAIL_93_3 = 0;
    work_AnonFilter_a0__543_100__93(1 );
HEAD_94_3 = 0;
TAIL_94_3 = 0;
    work_AnonFilter_a0__549_101__94(1 );
HEAD_91_3 = 0;
TAIL_91_3 = 0;
    work_AnonFilter_a0__531_98__91(1 );
HEAD_92_3 = 0;
TAIL_92_3 = 0;
    work_AnonFilter_a0__537_99__92(1 );
HEAD_89_3 = 0;
TAIL_89_3 = 0;
    work_AnonFilter_a0__519_96__89(1 );
HEAD_90_3 = 0;
TAIL_90_3 = 0;
    work_AnonFilter_a0__525_97__90(1 );
HEAD_103_3 = 0;
TAIL_103_3 = 0;
    work_AnonFilter_a0__603_110__103(1 );
HEAD_104_3 = 0;
TAIL_104_3 = 0;
    work_AnonFilter_a0__609_111__104(1 );
HEAD_101_3 = 0;
TAIL_101_3 = 0;
    work_AnonFilter_a0__591_108__101(1 );
HEAD_102_3 = 0;
TAIL_102_3 = 0;
    work_AnonFilter_a0__597_109__102(1 );
HEAD_99_3 = 0;
TAIL_99_3 = 0;
    work_AnonFilter_a0__579_106__99(1 );
HEAD_100_3 = 0;
TAIL_100_3 = 0;
    work_AnonFilter_a0__585_107__100(1 );
HEAD_97_3 = 0;
TAIL_97_3 = 0;
    work_AnonFilter_a0__567_104__97(1 );
HEAD_98_3 = 0;
TAIL_98_3 = 0;
    work_AnonFilter_a0__573_105__98(1 );
HEAD_112_3 = 0;
TAIL_112_3 = 0;
    work_AnonFilter_a0__657_119__112(1 );
HEAD_111_3 = 0;
TAIL_111_3 = 0;
    work_AnonFilter_a0__651_118__111(1 );
HEAD_110_3 = 0;
TAIL_110_3 = 0;
    work_AnonFilter_a0__645_117__110(1 );
HEAD_109_3 = 0;
TAIL_109_3 = 0;
    work_AnonFilter_a0__639_116__109(1 );
HEAD_108_3 = 0;
TAIL_108_3 = 0;
    work_AnonFilter_a0__633_115__108(1 );
HEAD_107_3 = 0;
TAIL_107_3 = 0;
    work_AnonFilter_a0__627_114__107(1 );
HEAD_106_3 = 0;
TAIL_106_3 = 0;
    work_AnonFilter_a0__621_113__106(1 );
HEAD_105_3 = 0;
TAIL_105_3 = 0;
    work_AnonFilter_a0__615_112__105(1 );
HEAD_120_3 = 0;
TAIL_120_3 = 0;
    work_AnonFilter_a0__705_127__120(1 );
HEAD_119_3 = 0;
TAIL_119_3 = 0;
    work_AnonFilter_a0__699_126__119(1 );
HEAD_118_3 = 0;
TAIL_118_3 = 0;
    work_AnonFilter_a0__693_125__118(1 );
HEAD_117_3 = 0;
TAIL_117_3 = 0;
    work_AnonFilter_a0__687_124__117(1 );
HEAD_116_3 = 0;
TAIL_116_3 = 0;
    work_AnonFilter_a0__681_123__116(1 );
HEAD_115_3 = 0;
TAIL_115_3 = 0;
    work_AnonFilter_a0__675_122__115(1 );
HEAD_114_3 = 0;
TAIL_114_3 = 0;
    work_AnonFilter_a0__669_121__114(1 );
HEAD_113_3 = 0;
TAIL_113_3 = 0;
    work_AnonFilter_a0__663_120__113(1 );
HEAD_130_3 = 0;
TAIL_130_3 = 0;
    work_AnonFilter_a0__765_137__130(1 );
HEAD_129_3 = 0;
TAIL_129_3 = 0;
    work_AnonFilter_a0__759_136__129(1 );
HEAD_131_3 = 0;
TAIL_131_3 = 0;
    work_AnonFilter_a0__771_138__131(1 );
HEAD_122_3 = 0;
TAIL_122_3 = 0;
    work_AnonFilter_a0__717_129__122(1 );
HEAD_121_3 = 0;
TAIL_121_3 = 0;
    work_AnonFilter_a0__711_128__121(1 );
HEAD_124_3 = 0;
TAIL_124_3 = 0;
    work_AnonFilter_a0__729_131__124(1 );
HEAD_123_3 = 0;
TAIL_123_3 = 0;
    work_AnonFilter_a0__723_130__123(1 );
HEAD_126_3 = 0;
TAIL_126_3 = 0;
    work_AnonFilter_a0__741_133__126(1 );
HEAD_125_3 = 0;
TAIL_125_3 = 0;
    work_AnonFilter_a0__735_132__125(1 );
HEAD_128_3 = 0;
TAIL_128_3 = 0;
    work_AnonFilter_a0__753_135__128(1 );
HEAD_127_3 = 0;
TAIL_127_3 = 0;
    work_AnonFilter_a0__747_134__127(1 );
HEAD_3_4 = 0;
TAIL_3_4 = 0;
    __joiner_3_work(1 );
    work_FloatPrinter__775_139__4(128 );
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
  printf("%f\n", finalSum);

  return 0;
}

// moved or inserted by concat_cluster_threads.pl
#include <message.h>
message *__msg_stack_128;
message *__msg_stack_105;
message *__msg_stack_10;
message *__msg_stack_123;
message *__msg_stack_81;
message *__msg_stack_42;
message *__msg_stack_91;
message *__msg_stack_56;
message *__msg_stack_9;
message *__msg_stack_47;
message *__msg_stack_31;
message *__msg_stack_107;
message *__msg_stack_102;
message *__msg_stack_19;
message *__msg_stack_66;
message *__msg_stack_52;
message *__msg_stack_118;
message *__msg_stack_23;
message *__msg_stack_87;
message *__msg_stack_6;
message *__msg_stack_78;
message *__msg_stack_127;
message *__msg_stack_63;
message *__msg_stack_0;
message *__msg_stack_27;
message *__msg_stack_86;
message *__msg_stack_76;
message *__msg_stack_75;
message *__msg_stack_11;
message *__msg_stack_13;
message *__msg_stack_125;
message *__msg_stack_85;
message *__msg_stack_82;
message *__msg_stack_43;
message *__msg_stack_114;
message *__msg_stack_60;
message *__msg_stack_37;
message *__msg_stack_34;
message *__msg_stack_113;
message *__msg_stack_112;
message *__msg_stack_64;
message *__msg_stack_106;
message *__msg_stack_46;
message *__msg_stack_59;
message *__msg_stack_121;
message *__msg_stack_116;
message *__msg_stack_1;
message *__msg_stack_111;
message *__msg_stack_15;
message *__msg_stack_120;
message *__msg_stack_21;
message *__msg_stack_41;
message *__msg_stack_48;
message *__msg_stack_58;
message *__msg_stack_36;
message *__msg_stack_26;
message *__msg_stack_89;
message *__msg_stack_35;
message *__msg_stack_50;
message *__msg_stack_126;
message *__msg_stack_110;
message *__msg_stack_98;
message *__msg_stack_80;
message *__msg_stack_33;
message *__msg_stack_94;
message *__msg_stack_12;
message *__msg_stack_115;
message *__msg_stack_28;
message *__msg_stack_16;
message *__msg_stack_39;
message *__msg_stack_122;
message *__msg_stack_117;
message *__msg_stack_97;
message *__msg_stack_38;
message *__msg_stack_103;
message *__msg_stack_70;
message *__msg_stack_84;
message *__msg_stack_83;
message *__msg_stack_95;
message *__msg_stack_65;
message *__msg_stack_14;
message *__msg_stack_55;
message *__msg_stack_101;
message *__msg_stack_77;
message *__msg_stack_2;
message *__msg_stack_100;
message *__msg_stack_73;
message *__msg_stack_30;
message *__msg_stack_7;
message *__msg_stack_90;
message *__msg_stack_131;
message *__msg_stack_18;
message *__msg_stack_119;
message *__msg_stack_124;
message *__msg_stack_93;
message *__msg_stack_57;
message *__msg_stack_108;
message *__msg_stack_3;
message *__msg_stack_24;
message *__msg_stack_40;
message *__msg_stack_69;
message *__msg_stack_99;
message *__msg_stack_49;
message *__msg_stack_5;
message *__msg_stack_67;
message *__msg_stack_45;
message *__msg_stack_71;
message *__msg_stack_17;
message *__msg_stack_92;
message *__msg_stack_74;
message *__msg_stack_44;
message *__msg_stack_29;
message *__msg_stack_62;
message *__msg_stack_68;
message *__msg_stack_25;
message *__msg_stack_129;
message *__msg_stack_4;
message *__msg_stack_20;
message *__msg_stack_53;
message *__msg_stack_61;
message *__msg_stack_8;
message *__msg_stack_32;
message *__msg_stack_109;
message *__msg_stack_104;
message *__msg_stack_130;
message *__msg_stack_51;
message *__msg_stack_54;
message *__msg_stack_22;
message *__msg_stack_79;
message *__msg_stack_88;
message *__msg_stack_72;
message *__msg_stack_96;

// end of moved or inserted by concat_cluster_threads.pl

// peek: 0 pop: 0 push 1
// init counts: 0 steady counts: 32

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



float n__0__0 = 0.0f;
void save_file_pointer__0(object_write_buffer *buf);
void load_file_pointer__0(object_write_buffer *buf);

 
void init_OneSource__3_10__0();
inline void check_status__0();

void work_OneSource__3_10__0(int);



inline void __push__0(float data) {
BUFFER_0_1[HEAD_0_1]=data;
HEAD_0_1++;
}



 
void init_OneSource__3_10__0(){
  ((n__0__0) = ((float)0.0))/*float*/;
}
void save_file_pointer__0(object_write_buffer *buf) {}
void load_file_pointer__0(object_write_buffer *buf) {}
 
void work_OneSource__3_10__0(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter OneSource

      __push__0((n__0__0));
      ((n__0__0) = ((n__0__0) + ((float)0.01)))/*float*/;
      // mark end: SIRFilter OneSource

    }
  }
}

// init counts: 0 steady counts: 32

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
  float tmp;
  tmp = BUFFER_0_1[TAIL_0_1]; TAIL_0_1++;
  BUFFER_1_2[HEAD_1_2]=tmp;
HEAD_1_2++; ;
  BUFFER_1_5[HEAD_1_5]=tmp;
HEAD_1_5++; ;
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
  BUFFER_1_22[HEAD_1_22]=tmp;
HEAD_1_22++; ;
  BUFFER_1_23[HEAD_1_23]=tmp;
HEAD_1_23++; ;
  BUFFER_1_24[HEAD_1_24]=tmp;
HEAD_1_24++; ;
  BUFFER_1_25[HEAD_1_25]=tmp;
HEAD_1_25++; ;
  BUFFER_1_26[HEAD_1_26]=tmp;
HEAD_1_26++; ;
  BUFFER_1_27[HEAD_1_27]=tmp;
HEAD_1_27++; ;
  BUFFER_1_28[HEAD_1_28]=tmp;
HEAD_1_28++; ;
  BUFFER_1_29[HEAD_1_29]=tmp;
HEAD_1_29++; ;
  BUFFER_1_30[HEAD_1_30]=tmp;
HEAD_1_30++; ;
  BUFFER_1_31[HEAD_1_31]=tmp;
HEAD_1_31++; ;
  BUFFER_1_32[HEAD_1_32]=tmp;
HEAD_1_32++; ;
  BUFFER_1_33[HEAD_1_33]=tmp;
HEAD_1_33++; ;
  BUFFER_1_34[HEAD_1_34]=tmp;
HEAD_1_34++; ;
  BUFFER_1_35[HEAD_1_35]=tmp;
HEAD_1_35++; ;
  BUFFER_1_36[HEAD_1_36]=tmp;
HEAD_1_36++; ;
  BUFFER_1_37[HEAD_1_37]=tmp;
HEAD_1_37++; ;
  BUFFER_1_38[HEAD_1_38]=tmp;
HEAD_1_38++; ;
  BUFFER_1_39[HEAD_1_39]=tmp;
HEAD_1_39++; ;
  BUFFER_1_40[HEAD_1_40]=tmp;
HEAD_1_40++; ;
  BUFFER_1_41[HEAD_1_41]=tmp;
HEAD_1_41++; ;
  BUFFER_1_42[HEAD_1_42]=tmp;
HEAD_1_42++; ;
  BUFFER_1_43[HEAD_1_43]=tmp;
HEAD_1_43++; ;
  BUFFER_1_44[HEAD_1_44]=tmp;
HEAD_1_44++; ;
  BUFFER_1_45[HEAD_1_45]=tmp;
HEAD_1_45++; ;
  BUFFER_1_46[HEAD_1_46]=tmp;
HEAD_1_46++; ;
  BUFFER_1_47[HEAD_1_47]=tmp;
HEAD_1_47++; ;
  BUFFER_1_48[HEAD_1_48]=tmp;
HEAD_1_48++; ;
  BUFFER_1_49[HEAD_1_49]=tmp;
HEAD_1_49++; ;
  BUFFER_1_50[HEAD_1_50]=tmp;
HEAD_1_50++; ;
  BUFFER_1_51[HEAD_1_51]=tmp;
HEAD_1_51++; ;
  BUFFER_1_52[HEAD_1_52]=tmp;
HEAD_1_52++; ;
  BUFFER_1_53[HEAD_1_53]=tmp;
HEAD_1_53++; ;
  BUFFER_1_54[HEAD_1_54]=tmp;
HEAD_1_54++; ;
  BUFFER_1_55[HEAD_1_55]=tmp;
HEAD_1_55++; ;
  BUFFER_1_56[HEAD_1_56]=tmp;
HEAD_1_56++; ;
  BUFFER_1_57[HEAD_1_57]=tmp;
HEAD_1_57++; ;
  BUFFER_1_58[HEAD_1_58]=tmp;
HEAD_1_58++; ;
  BUFFER_1_59[HEAD_1_59]=tmp;
HEAD_1_59++; ;
  BUFFER_1_60[HEAD_1_60]=tmp;
HEAD_1_60++; ;
  BUFFER_1_61[HEAD_1_61]=tmp;
HEAD_1_61++; ;
  BUFFER_1_62[HEAD_1_62]=tmp;
HEAD_1_62++; ;
  BUFFER_1_63[HEAD_1_63]=tmp;
HEAD_1_63++; ;
  BUFFER_1_64[HEAD_1_64]=tmp;
HEAD_1_64++; ;
  BUFFER_1_65[HEAD_1_65]=tmp;
HEAD_1_65++; ;
  BUFFER_1_66[HEAD_1_66]=tmp;
HEAD_1_66++; ;
  BUFFER_1_67[HEAD_1_67]=tmp;
HEAD_1_67++; ;
  BUFFER_1_68[HEAD_1_68]=tmp;
HEAD_1_68++; ;
  BUFFER_1_69[HEAD_1_69]=tmp;
HEAD_1_69++; ;
  BUFFER_1_70[HEAD_1_70]=tmp;
HEAD_1_70++; ;
  BUFFER_1_71[HEAD_1_71]=tmp;
HEAD_1_71++; ;
  BUFFER_1_72[HEAD_1_72]=tmp;
HEAD_1_72++; ;
  BUFFER_1_73[HEAD_1_73]=tmp;
HEAD_1_73++; ;
  BUFFER_1_74[HEAD_1_74]=tmp;
HEAD_1_74++; ;
  BUFFER_1_75[HEAD_1_75]=tmp;
HEAD_1_75++; ;
  BUFFER_1_76[HEAD_1_76]=tmp;
HEAD_1_76++; ;
  BUFFER_1_77[HEAD_1_77]=tmp;
HEAD_1_77++; ;
  BUFFER_1_78[HEAD_1_78]=tmp;
HEAD_1_78++; ;
  BUFFER_1_79[HEAD_1_79]=tmp;
HEAD_1_79++; ;
  BUFFER_1_80[HEAD_1_80]=tmp;
HEAD_1_80++; ;
  BUFFER_1_81[HEAD_1_81]=tmp;
HEAD_1_81++; ;
  BUFFER_1_82[HEAD_1_82]=tmp;
HEAD_1_82++; ;
  BUFFER_1_83[HEAD_1_83]=tmp;
HEAD_1_83++; ;
  BUFFER_1_84[HEAD_1_84]=tmp;
HEAD_1_84++; ;
  BUFFER_1_85[HEAD_1_85]=tmp;
HEAD_1_85++; ;
  BUFFER_1_86[HEAD_1_86]=tmp;
HEAD_1_86++; ;
  BUFFER_1_87[HEAD_1_87]=tmp;
HEAD_1_87++; ;
  BUFFER_1_88[HEAD_1_88]=tmp;
HEAD_1_88++; ;
  BUFFER_1_89[HEAD_1_89]=tmp;
HEAD_1_89++; ;
  BUFFER_1_90[HEAD_1_90]=tmp;
HEAD_1_90++; ;
  BUFFER_1_91[HEAD_1_91]=tmp;
HEAD_1_91++; ;
  BUFFER_1_92[HEAD_1_92]=tmp;
HEAD_1_92++; ;
  BUFFER_1_93[HEAD_1_93]=tmp;
HEAD_1_93++; ;
  BUFFER_1_94[HEAD_1_94]=tmp;
HEAD_1_94++; ;
  BUFFER_1_95[HEAD_1_95]=tmp;
HEAD_1_95++; ;
  BUFFER_1_96[HEAD_1_96]=tmp;
HEAD_1_96++; ;
  BUFFER_1_97[HEAD_1_97]=tmp;
HEAD_1_97++; ;
  BUFFER_1_98[HEAD_1_98]=tmp;
HEAD_1_98++; ;
  BUFFER_1_99[HEAD_1_99]=tmp;
HEAD_1_99++; ;
  BUFFER_1_100[HEAD_1_100]=tmp;
HEAD_1_100++; ;
  BUFFER_1_101[HEAD_1_101]=tmp;
HEAD_1_101++; ;
  BUFFER_1_102[HEAD_1_102]=tmp;
HEAD_1_102++; ;
  BUFFER_1_103[HEAD_1_103]=tmp;
HEAD_1_103++; ;
  BUFFER_1_104[HEAD_1_104]=tmp;
HEAD_1_104++; ;
  BUFFER_1_105[HEAD_1_105]=tmp;
HEAD_1_105++; ;
  BUFFER_1_106[HEAD_1_106]=tmp;
HEAD_1_106++; ;
  BUFFER_1_107[HEAD_1_107]=tmp;
HEAD_1_107++; ;
  BUFFER_1_108[HEAD_1_108]=tmp;
HEAD_1_108++; ;
  BUFFER_1_109[HEAD_1_109]=tmp;
HEAD_1_109++; ;
  BUFFER_1_110[HEAD_1_110]=tmp;
HEAD_1_110++; ;
  BUFFER_1_111[HEAD_1_111]=tmp;
HEAD_1_111++; ;
  BUFFER_1_112[HEAD_1_112]=tmp;
HEAD_1_112++; ;
  BUFFER_1_113[HEAD_1_113]=tmp;
HEAD_1_113++; ;
  BUFFER_1_114[HEAD_1_114]=tmp;
HEAD_1_114++; ;
  BUFFER_1_115[HEAD_1_115]=tmp;
HEAD_1_115++; ;
  BUFFER_1_116[HEAD_1_116]=tmp;
HEAD_1_116++; ;
  BUFFER_1_117[HEAD_1_117]=tmp;
HEAD_1_117++; ;
  BUFFER_1_118[HEAD_1_118]=tmp;
HEAD_1_118++; ;
  BUFFER_1_119[HEAD_1_119]=tmp;
HEAD_1_119++; ;
  BUFFER_1_120[HEAD_1_120]=tmp;
HEAD_1_120++; ;
  BUFFER_1_121[HEAD_1_121]=tmp;
HEAD_1_121++; ;
  BUFFER_1_122[HEAD_1_122]=tmp;
HEAD_1_122++; ;
  BUFFER_1_123[HEAD_1_123]=tmp;
HEAD_1_123++; ;
  BUFFER_1_124[HEAD_1_124]=tmp;
HEAD_1_124++; ;
  BUFFER_1_125[HEAD_1_125]=tmp;
HEAD_1_125++; ;
  BUFFER_1_126[HEAD_1_126]=tmp;
HEAD_1_126++; ;
  BUFFER_1_127[HEAD_1_127]=tmp;
HEAD_1_127++; ;
  BUFFER_1_128[HEAD_1_128]=tmp;
HEAD_1_128++; ;
  BUFFER_1_129[HEAD_1_129]=tmp;
HEAD_1_129++; ;
  BUFFER_1_130[HEAD_1_130]=tmp;
HEAD_1_130++; ;
  BUFFER_1_131[HEAD_1_131]=tmp;
HEAD_1_131++; ;
  }
}


// peek: 32 pop: 32 push 1
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

 
void init_AnonFilter_a0__9_11__2();
inline void check_status__2();

void work_AnonFilter_a0__9_11__2(int);


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



 
void init_AnonFilter_a0__9_11__2(){
}
void save_file_pointer__2(object_write_buffer *buf) {}
void load_file_pointer__2(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__9_11__2__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__6 = 0.0f;/* float */
  int i__conflict__0__7 = 0;/* int */
  int i__8 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__6 = ((float)0.0))/*float*/;
  for ((i__conflict__0__7 = 0)/*int*/; (i__conflict__0__7 < 32); (i__conflict__0__7++)) {{
      (sum__6 = (sum__6 + ((*(____in+i__conflict__0__7)) * (*(____in+(i__conflict__0__7 + 0))))))/*float*/;
    }
  }
  for ((i__8 = 0)/*int*/; (i__8 < 32); (i__8++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__6);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__9_11__2__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__6 = 0.0f;/* float */
  int i__conflict__0__7 = 0;/* int */
  int i__8 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__6 = ((float)0.0))/*float*/;
  for ((i__conflict__0__7 = 0)/*int*/; (i__conflict__0__7 < 32); (i__conflict__0__7++)) {{
      (sum__6 = (sum__6 + ((*(____in+i__conflict__0__7)) * (*(____in+(i__conflict__0__7 + 0))))))/*float*/;
    }
  }
  for ((i__8 = 0)/*int*/; (i__8 < 32); (i__8++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__6);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__9_11__2(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__6 = 0.0f;/* float */
      int i__conflict__0__7 = 0;/* int */
      int i__8 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__6 = ((float)0.0))/*float*/;
      for ((i__conflict__0__7 = 0)/*int*/; (i__conflict__0__7 < 32); (i__conflict__0__7++)) {{
          (sum__6 = (sum__6 + (__peek__2(i__conflict__0__7) * __peek__2((i__conflict__0__7 + 0)))))/*float*/;
        }
      }
      for ((i__8 = 0)/*int*/; (i__8 < 32); (i__8++)) {{
          __pop__2();
        }
      }
      __push__2(sum__6);
      // mark end: SIRFilter AnonFilter_a0

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
BUFFER_3_4[HEAD_3_4]=BUFFER_5_3[TAIL_5_3];
HEAD_3_4++; ; TAIL_5_3++;
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
BUFFER_3_4[HEAD_3_4]=BUFFER_22_3[TAIL_22_3];
HEAD_3_4++; ; TAIL_22_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_23_3[TAIL_23_3];
HEAD_3_4++; ; TAIL_23_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_24_3[TAIL_24_3];
HEAD_3_4++; ; TAIL_24_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_25_3[TAIL_25_3];
HEAD_3_4++; ; TAIL_25_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_26_3[TAIL_26_3];
HEAD_3_4++; ; TAIL_26_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_27_3[TAIL_27_3];
HEAD_3_4++; ; TAIL_27_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_28_3[TAIL_28_3];
HEAD_3_4++; ; TAIL_28_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_29_3[TAIL_29_3];
HEAD_3_4++; ; TAIL_29_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_30_3[TAIL_30_3];
HEAD_3_4++; ; TAIL_30_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_31_3[TAIL_31_3];
HEAD_3_4++; ; TAIL_31_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_32_3[TAIL_32_3];
HEAD_3_4++; ; TAIL_32_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_33_3[TAIL_33_3];
HEAD_3_4++; ; TAIL_33_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_34_3[TAIL_34_3];
HEAD_3_4++; ; TAIL_34_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_35_3[TAIL_35_3];
HEAD_3_4++; ; TAIL_35_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_36_3[TAIL_36_3];
HEAD_3_4++; ; TAIL_36_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_37_3[TAIL_37_3];
HEAD_3_4++; ; TAIL_37_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_38_3[TAIL_38_3];
HEAD_3_4++; ; TAIL_38_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_39_3[TAIL_39_3];
HEAD_3_4++; ; TAIL_39_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_40_3[TAIL_40_3];
HEAD_3_4++; ; TAIL_40_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_41_3[TAIL_41_3];
HEAD_3_4++; ; TAIL_41_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_42_3[TAIL_42_3];
HEAD_3_4++; ; TAIL_42_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_43_3[TAIL_43_3];
HEAD_3_4++; ; TAIL_43_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_44_3[TAIL_44_3];
HEAD_3_4++; ; TAIL_44_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_45_3[TAIL_45_3];
HEAD_3_4++; ; TAIL_45_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_46_3[TAIL_46_3];
HEAD_3_4++; ; TAIL_46_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_47_3[TAIL_47_3];
HEAD_3_4++; ; TAIL_47_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_48_3[TAIL_48_3];
HEAD_3_4++; ; TAIL_48_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_49_3[TAIL_49_3];
HEAD_3_4++; ; TAIL_49_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_50_3[TAIL_50_3];
HEAD_3_4++; ; TAIL_50_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_51_3[TAIL_51_3];
HEAD_3_4++; ; TAIL_51_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_52_3[TAIL_52_3];
HEAD_3_4++; ; TAIL_52_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_53_3[TAIL_53_3];
HEAD_3_4++; ; TAIL_53_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_54_3[TAIL_54_3];
HEAD_3_4++; ; TAIL_54_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_55_3[TAIL_55_3];
HEAD_3_4++; ; TAIL_55_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_56_3[TAIL_56_3];
HEAD_3_4++; ; TAIL_56_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_57_3[TAIL_57_3];
HEAD_3_4++; ; TAIL_57_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_58_3[TAIL_58_3];
HEAD_3_4++; ; TAIL_58_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_59_3[TAIL_59_3];
HEAD_3_4++; ; TAIL_59_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_60_3[TAIL_60_3];
HEAD_3_4++; ; TAIL_60_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_61_3[TAIL_61_3];
HEAD_3_4++; ; TAIL_61_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_62_3[TAIL_62_3];
HEAD_3_4++; ; TAIL_62_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_63_3[TAIL_63_3];
HEAD_3_4++; ; TAIL_63_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_64_3[TAIL_64_3];
HEAD_3_4++; ; TAIL_64_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_65_3[TAIL_65_3];
HEAD_3_4++; ; TAIL_65_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_66_3[TAIL_66_3];
HEAD_3_4++; ; TAIL_66_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_67_3[TAIL_67_3];
HEAD_3_4++; ; TAIL_67_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_68_3[TAIL_68_3];
HEAD_3_4++; ; TAIL_68_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_69_3[TAIL_69_3];
HEAD_3_4++; ; TAIL_69_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_70_3[TAIL_70_3];
HEAD_3_4++; ; TAIL_70_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_71_3[TAIL_71_3];
HEAD_3_4++; ; TAIL_71_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_72_3[TAIL_72_3];
HEAD_3_4++; ; TAIL_72_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_73_3[TAIL_73_3];
HEAD_3_4++; ; TAIL_73_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_74_3[TAIL_74_3];
HEAD_3_4++; ; TAIL_74_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_75_3[TAIL_75_3];
HEAD_3_4++; ; TAIL_75_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_76_3[TAIL_76_3];
HEAD_3_4++; ; TAIL_76_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_77_3[TAIL_77_3];
HEAD_3_4++; ; TAIL_77_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_78_3[TAIL_78_3];
HEAD_3_4++; ; TAIL_78_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_79_3[TAIL_79_3];
HEAD_3_4++; ; TAIL_79_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_80_3[TAIL_80_3];
HEAD_3_4++; ; TAIL_80_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_81_3[TAIL_81_3];
HEAD_3_4++; ; TAIL_81_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_82_3[TAIL_82_3];
HEAD_3_4++; ; TAIL_82_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_83_3[TAIL_83_3];
HEAD_3_4++; ; TAIL_83_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_84_3[TAIL_84_3];
HEAD_3_4++; ; TAIL_84_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_85_3[TAIL_85_3];
HEAD_3_4++; ; TAIL_85_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_86_3[TAIL_86_3];
HEAD_3_4++; ; TAIL_86_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_87_3[TAIL_87_3];
HEAD_3_4++; ; TAIL_87_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_88_3[TAIL_88_3];
HEAD_3_4++; ; TAIL_88_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_89_3[TAIL_89_3];
HEAD_3_4++; ; TAIL_89_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_90_3[TAIL_90_3];
HEAD_3_4++; ; TAIL_90_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_91_3[TAIL_91_3];
HEAD_3_4++; ; TAIL_91_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_92_3[TAIL_92_3];
HEAD_3_4++; ; TAIL_92_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_93_3[TAIL_93_3];
HEAD_3_4++; ; TAIL_93_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_94_3[TAIL_94_3];
HEAD_3_4++; ; TAIL_94_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_95_3[TAIL_95_3];
HEAD_3_4++; ; TAIL_95_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_96_3[TAIL_96_3];
HEAD_3_4++; ; TAIL_96_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_97_3[TAIL_97_3];
HEAD_3_4++; ; TAIL_97_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_98_3[TAIL_98_3];
HEAD_3_4++; ; TAIL_98_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_99_3[TAIL_99_3];
HEAD_3_4++; ; TAIL_99_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_100_3[TAIL_100_3];
HEAD_3_4++; ; TAIL_100_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_101_3[TAIL_101_3];
HEAD_3_4++; ; TAIL_101_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_102_3[TAIL_102_3];
HEAD_3_4++; ; TAIL_102_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_103_3[TAIL_103_3];
HEAD_3_4++; ; TAIL_103_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_104_3[TAIL_104_3];
HEAD_3_4++; ; TAIL_104_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_105_3[TAIL_105_3];
HEAD_3_4++; ; TAIL_105_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_106_3[TAIL_106_3];
HEAD_3_4++; ; TAIL_106_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_107_3[TAIL_107_3];
HEAD_3_4++; ; TAIL_107_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_108_3[TAIL_108_3];
HEAD_3_4++; ; TAIL_108_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_109_3[TAIL_109_3];
HEAD_3_4++; ; TAIL_109_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_110_3[TAIL_110_3];
HEAD_3_4++; ; TAIL_110_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_111_3[TAIL_111_3];
HEAD_3_4++; ; TAIL_111_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_112_3[TAIL_112_3];
HEAD_3_4++; ; TAIL_112_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_113_3[TAIL_113_3];
HEAD_3_4++; ; TAIL_113_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_114_3[TAIL_114_3];
HEAD_3_4++; ; TAIL_114_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_115_3[TAIL_115_3];
HEAD_3_4++; ; TAIL_115_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_116_3[TAIL_116_3];
HEAD_3_4++; ; TAIL_116_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_117_3[TAIL_117_3];
HEAD_3_4++; ; TAIL_117_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_118_3[TAIL_118_3];
HEAD_3_4++; ; TAIL_118_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_119_3[TAIL_119_3];
HEAD_3_4++; ; TAIL_119_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_120_3[TAIL_120_3];
HEAD_3_4++; ; TAIL_120_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_121_3[TAIL_121_3];
HEAD_3_4++; ; TAIL_121_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_122_3[TAIL_122_3];
HEAD_3_4++; ; TAIL_122_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_123_3[TAIL_123_3];
HEAD_3_4++; ; TAIL_123_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_124_3[TAIL_124_3];
HEAD_3_4++; ; TAIL_124_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_125_3[TAIL_125_3];
HEAD_3_4++; ; TAIL_125_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_126_3[TAIL_126_3];
HEAD_3_4++; ; TAIL_126_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_127_3[TAIL_127_3];
HEAD_3_4++; ; TAIL_127_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_128_3[TAIL_128_3];
HEAD_3_4++; ; TAIL_128_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_129_3[TAIL_129_3];
HEAD_3_4++; ; TAIL_129_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_130_3[TAIL_130_3];
HEAD_3_4++; ; TAIL_130_3++;
BUFFER_3_4[HEAD_3_4]=BUFFER_131_3[TAIL_131_3];
HEAD_3_4++; ; TAIL_131_3++;
  }
}


// peek: 1 pop: 1 push 0
// init counts: 0 steady counts: 128

// ClusterFusion isEliminated: false



int __number_of_iterations_4;
int __counter_4 = 0;
int __steady_4 = 0;
int __tmp_4 = 0;
int __tmp2_4 = 0;
int *__state_flag_4 = NULL;
thread_info *__thread_4 = NULL;



float x__772__4 = 0.0f;
void save_peek_buffer__4(object_write_buffer *buf);
void load_peek_buffer__4(object_write_buffer *buf);
void save_file_pointer__4(object_write_buffer *buf);
void load_file_pointer__4(object_write_buffer *buf);

 
void init_FloatPrinter__775_139__4();
inline void check_status__4();

void work_FloatPrinter__775_139__4(int);


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


 
void init_FloatPrinter__775_139__4(){
}
void save_file_pointer__4(object_write_buffer *buf) {}
void load_file_pointer__4(object_write_buffer *buf) {}
 
void work_FloatPrinter__775_139__4(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      // mark begin: SIRFilter FloatPrinter

      ((x__772__4) = __pop__4())/*float*/;
      finalSum+=x__772__4;
      // mark end: SIRFilter FloatPrinter

    }
  }
}

// peek: 32 pop: 32 push 1
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

 
void init_AnonFilter_a0__15_12__5();
inline void check_status__5();

void work_AnonFilter_a0__15_12__5(int);


inline float __pop__5() {
float res=BUFFER_1_5[TAIL_1_5];
TAIL_1_5++;
return res;
}

inline float __pop__5(int n) {
float res=BUFFER_1_5[TAIL_1_5];
TAIL_1_5+=n;

return res;
}

inline float __peek__5(int offs) {
return BUFFER_1_5[TAIL_1_5+offs];
}



inline void __push__5(float data) {
BUFFER_5_3[HEAD_5_3]=data;
HEAD_5_3++;
}



 
void init_AnonFilter_a0__15_12__5(){
}
void save_file_pointer__5(object_write_buffer *buf) {}
void load_file_pointer__5(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__15_12__5__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__12 = 0.0f;/* float */
  int i__conflict__0__13 = 0;/* int */
  int i__14 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__12 = ((float)0.0))/*float*/;
  for ((i__conflict__0__13 = 0)/*int*/; (i__conflict__0__13 < 31); (i__conflict__0__13++)) {{
      (sum__12 = (sum__12 + ((*(____in+i__conflict__0__13)) * (*(____in+(i__conflict__0__13 + 1))))))/*float*/;
    }
  }
  for ((i__14 = 0)/*int*/; (i__14 < 32); (i__14++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__12);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__15_12__5__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__12 = 0.0f;/* float */
  int i__conflict__0__13 = 0;/* int */
  int i__14 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__12 = ((float)0.0))/*float*/;
  for ((i__conflict__0__13 = 0)/*int*/; (i__conflict__0__13 < 31); (i__conflict__0__13++)) {{
      (sum__12 = (sum__12 + ((*(____in+i__conflict__0__13)) * (*(____in+(i__conflict__0__13 + 1))))))/*float*/;
    }
  }
  for ((i__14 = 0)/*int*/; (i__14 < 32); (i__14++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__12);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__15_12__5(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__12 = 0.0f;/* float */
      int i__conflict__0__13 = 0;/* int */
      int i__14 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__12 = ((float)0.0))/*float*/;
      for ((i__conflict__0__13 = 0)/*int*/; (i__conflict__0__13 < 31); (i__conflict__0__13++)) {{
          (sum__12 = (sum__12 + (__peek__5(i__conflict__0__13) * __peek__5((i__conflict__0__13 + 1)))))/*float*/;
        }
      }
      for ((i__14 = 0)/*int*/; (i__14 < 32); (i__14++)) {{
          __pop__5();
        }
      }
      __push__5(sum__12);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
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

 
void init_AnonFilter_a0__21_13__6();
inline void check_status__6();

void work_AnonFilter_a0__21_13__6(int);


inline float __pop__6() {
float res=BUFFER_1_6[TAIL_1_6];
TAIL_1_6++;
return res;
}

inline float __pop__6(int n) {
float res=BUFFER_1_6[TAIL_1_6];
TAIL_1_6+=n;

return res;
}

inline float __peek__6(int offs) {
return BUFFER_1_6[TAIL_1_6+offs];
}



inline void __push__6(float data) {
BUFFER_6_3[HEAD_6_3]=data;
HEAD_6_3++;
}



 
void init_AnonFilter_a0__21_13__6(){
}
void save_file_pointer__6(object_write_buffer *buf) {}
void load_file_pointer__6(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__21_13__6__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__18 = 0.0f;/* float */
  int i__conflict__0__19 = 0;/* int */
  int i__20 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__18 = ((float)0.0))/*float*/;
  for ((i__conflict__0__19 = 0)/*int*/; (i__conflict__0__19 < 30); (i__conflict__0__19++)) {{
      (sum__18 = (sum__18 + ((*(____in+i__conflict__0__19)) * (*(____in+(i__conflict__0__19 + 2))))))/*float*/;
    }
  }
  for ((i__20 = 0)/*int*/; (i__20 < 32); (i__20++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__18);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__21_13__6__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__18 = 0.0f;/* float */
  int i__conflict__0__19 = 0;/* int */
  int i__20 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__18 = ((float)0.0))/*float*/;
  for ((i__conflict__0__19 = 0)/*int*/; (i__conflict__0__19 < 30); (i__conflict__0__19++)) {{
      (sum__18 = (sum__18 + ((*(____in+i__conflict__0__19)) * (*(____in+(i__conflict__0__19 + 2))))))/*float*/;
    }
  }
  for ((i__20 = 0)/*int*/; (i__20 < 32); (i__20++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__18);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__21_13__6(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__18 = 0.0f;/* float */
      int i__conflict__0__19 = 0;/* int */
      int i__20 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__18 = ((float)0.0))/*float*/;
      for ((i__conflict__0__19 = 0)/*int*/; (i__conflict__0__19 < 30); (i__conflict__0__19++)) {{
          (sum__18 = (sum__18 + (__peek__6(i__conflict__0__19) * __peek__6((i__conflict__0__19 + 2)))))/*float*/;
        }
      }
      for ((i__20 = 0)/*int*/; (i__20 < 32); (i__20++)) {{
          __pop__6();
        }
      }
      __push__6(sum__18);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
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

 
void init_AnonFilter_a0__27_14__7();
inline void check_status__7();

void work_AnonFilter_a0__27_14__7(int);


inline float __pop__7() {
float res=BUFFER_1_7[TAIL_1_7];
TAIL_1_7++;
return res;
}

inline float __pop__7(int n) {
float res=BUFFER_1_7[TAIL_1_7];
TAIL_1_7+=n;

return res;
}

inline float __peek__7(int offs) {
return BUFFER_1_7[TAIL_1_7+offs];
}



inline void __push__7(float data) {
BUFFER_7_3[HEAD_7_3]=data;
HEAD_7_3++;
}



 
void init_AnonFilter_a0__27_14__7(){
}
void save_file_pointer__7(object_write_buffer *buf) {}
void load_file_pointer__7(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__27_14__7__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__24 = 0.0f;/* float */
  int i__conflict__0__25 = 0;/* int */
  int i__26 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__24 = ((float)0.0))/*float*/;
  for ((i__conflict__0__25 = 0)/*int*/; (i__conflict__0__25 < 29); (i__conflict__0__25++)) {{
      (sum__24 = (sum__24 + ((*(____in+i__conflict__0__25)) * (*(____in+(i__conflict__0__25 + 3))))))/*float*/;
    }
  }
  for ((i__26 = 0)/*int*/; (i__26 < 32); (i__26++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__24);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__27_14__7__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__24 = 0.0f;/* float */
  int i__conflict__0__25 = 0;/* int */
  int i__26 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__24 = ((float)0.0))/*float*/;
  for ((i__conflict__0__25 = 0)/*int*/; (i__conflict__0__25 < 29); (i__conflict__0__25++)) {{
      (sum__24 = (sum__24 + ((*(____in+i__conflict__0__25)) * (*(____in+(i__conflict__0__25 + 3))))))/*float*/;
    }
  }
  for ((i__26 = 0)/*int*/; (i__26 < 32); (i__26++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__24);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__27_14__7(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__24 = 0.0f;/* float */
      int i__conflict__0__25 = 0;/* int */
      int i__26 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__24 = ((float)0.0))/*float*/;
      for ((i__conflict__0__25 = 0)/*int*/; (i__conflict__0__25 < 29); (i__conflict__0__25++)) {{
          (sum__24 = (sum__24 + (__peek__7(i__conflict__0__25) * __peek__7((i__conflict__0__25 + 3)))))/*float*/;
        }
      }
      for ((i__26 = 0)/*int*/; (i__26 < 32); (i__26++)) {{
          __pop__7();
        }
      }
      __push__7(sum__24);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
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

 
void init_AnonFilter_a0__33_15__8();
inline void check_status__8();

void work_AnonFilter_a0__33_15__8(int);


inline float __pop__8() {
float res=BUFFER_1_8[TAIL_1_8];
TAIL_1_8++;
return res;
}

inline float __pop__8(int n) {
float res=BUFFER_1_8[TAIL_1_8];
TAIL_1_8+=n;

return res;
}

inline float __peek__8(int offs) {
return BUFFER_1_8[TAIL_1_8+offs];
}



inline void __push__8(float data) {
BUFFER_8_3[HEAD_8_3]=data;
HEAD_8_3++;
}



 
void init_AnonFilter_a0__33_15__8(){
}
void save_file_pointer__8(object_write_buffer *buf) {}
void load_file_pointer__8(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__33_15__8__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__30 = 0.0f;/* float */
  int i__conflict__0__31 = 0;/* int */
  int i__32 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__30 = ((float)0.0))/*float*/;
  for ((i__conflict__0__31 = 0)/*int*/; (i__conflict__0__31 < 28); (i__conflict__0__31++)) {{
      (sum__30 = (sum__30 + ((*(____in+i__conflict__0__31)) * (*(____in+(i__conflict__0__31 + 4))))))/*float*/;
    }
  }
  for ((i__32 = 0)/*int*/; (i__32 < 32); (i__32++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__30);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__33_15__8__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__30 = 0.0f;/* float */
  int i__conflict__0__31 = 0;/* int */
  int i__32 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__30 = ((float)0.0))/*float*/;
  for ((i__conflict__0__31 = 0)/*int*/; (i__conflict__0__31 < 28); (i__conflict__0__31++)) {{
      (sum__30 = (sum__30 + ((*(____in+i__conflict__0__31)) * (*(____in+(i__conflict__0__31 + 4))))))/*float*/;
    }
  }
  for ((i__32 = 0)/*int*/; (i__32 < 32); (i__32++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__30);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__33_15__8(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__30 = 0.0f;/* float */
      int i__conflict__0__31 = 0;/* int */
      int i__32 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__30 = ((float)0.0))/*float*/;
      for ((i__conflict__0__31 = 0)/*int*/; (i__conflict__0__31 < 28); (i__conflict__0__31++)) {{
          (sum__30 = (sum__30 + (__peek__8(i__conflict__0__31) * __peek__8((i__conflict__0__31 + 4)))))/*float*/;
        }
      }
      for ((i__32 = 0)/*int*/; (i__32 < 32); (i__32++)) {{
          __pop__8();
        }
      }
      __push__8(sum__30);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
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

 
void init_AnonFilter_a0__39_16__9();
inline void check_status__9();

void work_AnonFilter_a0__39_16__9(int);


inline float __pop__9() {
float res=BUFFER_1_9[TAIL_1_9];
TAIL_1_9++;
return res;
}

inline float __pop__9(int n) {
float res=BUFFER_1_9[TAIL_1_9];
TAIL_1_9+=n;

return res;
}

inline float __peek__9(int offs) {
return BUFFER_1_9[TAIL_1_9+offs];
}



inline void __push__9(float data) {
BUFFER_9_3[HEAD_9_3]=data;
HEAD_9_3++;
}



 
void init_AnonFilter_a0__39_16__9(){
}
void save_file_pointer__9(object_write_buffer *buf) {}
void load_file_pointer__9(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__39_16__9__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__36 = 0.0f;/* float */
  int i__conflict__0__37 = 0;/* int */
  int i__38 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__36 = ((float)0.0))/*float*/;
  for ((i__conflict__0__37 = 0)/*int*/; (i__conflict__0__37 < 27); (i__conflict__0__37++)) {{
      (sum__36 = (sum__36 + ((*(____in+i__conflict__0__37)) * (*(____in+(i__conflict__0__37 + 5))))))/*float*/;
    }
  }
  for ((i__38 = 0)/*int*/; (i__38 < 32); (i__38++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__36);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__39_16__9__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__36 = 0.0f;/* float */
  int i__conflict__0__37 = 0;/* int */
  int i__38 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__36 = ((float)0.0))/*float*/;
  for ((i__conflict__0__37 = 0)/*int*/; (i__conflict__0__37 < 27); (i__conflict__0__37++)) {{
      (sum__36 = (sum__36 + ((*(____in+i__conflict__0__37)) * (*(____in+(i__conflict__0__37 + 5))))))/*float*/;
    }
  }
  for ((i__38 = 0)/*int*/; (i__38 < 32); (i__38++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__36);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__39_16__9(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__36 = 0.0f;/* float */
      int i__conflict__0__37 = 0;/* int */
      int i__38 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__36 = ((float)0.0))/*float*/;
      for ((i__conflict__0__37 = 0)/*int*/; (i__conflict__0__37 < 27); (i__conflict__0__37++)) {{
          (sum__36 = (sum__36 + (__peek__9(i__conflict__0__37) * __peek__9((i__conflict__0__37 + 5)))))/*float*/;
        }
      }
      for ((i__38 = 0)/*int*/; (i__38 < 32); (i__38++)) {{
          __pop__9();
        }
      }
      __push__9(sum__36);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
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

 
void init_AnonFilter_a0__45_17__10();
inline void check_status__10();

void work_AnonFilter_a0__45_17__10(int);


inline float __pop__10() {
float res=BUFFER_1_10[TAIL_1_10];
TAIL_1_10++;
return res;
}

inline float __pop__10(int n) {
float res=BUFFER_1_10[TAIL_1_10];
TAIL_1_10+=n;

return res;
}

inline float __peek__10(int offs) {
return BUFFER_1_10[TAIL_1_10+offs];
}



inline void __push__10(float data) {
BUFFER_10_3[HEAD_10_3]=data;
HEAD_10_3++;
}



 
void init_AnonFilter_a0__45_17__10(){
}
void save_file_pointer__10(object_write_buffer *buf) {}
void load_file_pointer__10(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__45_17__10__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__42 = 0.0f;/* float */
  int i__conflict__0__43 = 0;/* int */
  int i__44 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__42 = ((float)0.0))/*float*/;
  for ((i__conflict__0__43 = 0)/*int*/; (i__conflict__0__43 < 26); (i__conflict__0__43++)) {{
      (sum__42 = (sum__42 + ((*(____in+i__conflict__0__43)) * (*(____in+(i__conflict__0__43 + 6))))))/*float*/;
    }
  }
  for ((i__44 = 0)/*int*/; (i__44 < 32); (i__44++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__42);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__45_17__10__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__42 = 0.0f;/* float */
  int i__conflict__0__43 = 0;/* int */
  int i__44 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__42 = ((float)0.0))/*float*/;
  for ((i__conflict__0__43 = 0)/*int*/; (i__conflict__0__43 < 26); (i__conflict__0__43++)) {{
      (sum__42 = (sum__42 + ((*(____in+i__conflict__0__43)) * (*(____in+(i__conflict__0__43 + 6))))))/*float*/;
    }
  }
  for ((i__44 = 0)/*int*/; (i__44 < 32); (i__44++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__42);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__45_17__10(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__42 = 0.0f;/* float */
      int i__conflict__0__43 = 0;/* int */
      int i__44 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__42 = ((float)0.0))/*float*/;
      for ((i__conflict__0__43 = 0)/*int*/; (i__conflict__0__43 < 26); (i__conflict__0__43++)) {{
          (sum__42 = (sum__42 + (__peek__10(i__conflict__0__43) * __peek__10((i__conflict__0__43 + 6)))))/*float*/;
        }
      }
      for ((i__44 = 0)/*int*/; (i__44 < 32); (i__44++)) {{
          __pop__10();
        }
      }
      __push__10(sum__42);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
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

 
void init_AnonFilter_a0__51_18__11();
inline void check_status__11();

void work_AnonFilter_a0__51_18__11(int);


inline float __pop__11() {
float res=BUFFER_1_11[TAIL_1_11];
TAIL_1_11++;
return res;
}

inline float __pop__11(int n) {
float res=BUFFER_1_11[TAIL_1_11];
TAIL_1_11+=n;

return res;
}

inline float __peek__11(int offs) {
return BUFFER_1_11[TAIL_1_11+offs];
}



inline void __push__11(float data) {
BUFFER_11_3[HEAD_11_3]=data;
HEAD_11_3++;
}



 
void init_AnonFilter_a0__51_18__11(){
}
void save_file_pointer__11(object_write_buffer *buf) {}
void load_file_pointer__11(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__51_18__11__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__48 = 0.0f;/* float */
  int i__conflict__0__49 = 0;/* int */
  int i__50 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__48 = ((float)0.0))/*float*/;
  for ((i__conflict__0__49 = 0)/*int*/; (i__conflict__0__49 < 25); (i__conflict__0__49++)) {{
      (sum__48 = (sum__48 + ((*(____in+i__conflict__0__49)) * (*(____in+(i__conflict__0__49 + 7))))))/*float*/;
    }
  }
  for ((i__50 = 0)/*int*/; (i__50 < 32); (i__50++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__48);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__51_18__11__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__48 = 0.0f;/* float */
  int i__conflict__0__49 = 0;/* int */
  int i__50 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__48 = ((float)0.0))/*float*/;
  for ((i__conflict__0__49 = 0)/*int*/; (i__conflict__0__49 < 25); (i__conflict__0__49++)) {{
      (sum__48 = (sum__48 + ((*(____in+i__conflict__0__49)) * (*(____in+(i__conflict__0__49 + 7))))))/*float*/;
    }
  }
  for ((i__50 = 0)/*int*/; (i__50 < 32); (i__50++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__48);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__51_18__11(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__48 = 0.0f;/* float */
      int i__conflict__0__49 = 0;/* int */
      int i__50 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__48 = ((float)0.0))/*float*/;
      for ((i__conflict__0__49 = 0)/*int*/; (i__conflict__0__49 < 25); (i__conflict__0__49++)) {{
          (sum__48 = (sum__48 + (__peek__11(i__conflict__0__49) * __peek__11((i__conflict__0__49 + 7)))))/*float*/;
        }
      }
      for ((i__50 = 0)/*int*/; (i__50 < 32); (i__50++)) {{
          __pop__11();
        }
      }
      __push__11(sum__48);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
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

 
void init_AnonFilter_a0__57_19__12();
inline void check_status__12();

void work_AnonFilter_a0__57_19__12(int);


inline float __pop__12() {
float res=BUFFER_1_12[TAIL_1_12];
TAIL_1_12++;
return res;
}

inline float __pop__12(int n) {
float res=BUFFER_1_12[TAIL_1_12];
TAIL_1_12+=n;

return res;
}

inline float __peek__12(int offs) {
return BUFFER_1_12[TAIL_1_12+offs];
}



inline void __push__12(float data) {
BUFFER_12_3[HEAD_12_3]=data;
HEAD_12_3++;
}



 
void init_AnonFilter_a0__57_19__12(){
}
void save_file_pointer__12(object_write_buffer *buf) {}
void load_file_pointer__12(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__57_19__12__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__54 = 0.0f;/* float */
  int i__conflict__0__55 = 0;/* int */
  int i__56 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__54 = ((float)0.0))/*float*/;
  for ((i__conflict__0__55 = 0)/*int*/; (i__conflict__0__55 < 24); (i__conflict__0__55++)) {{
      (sum__54 = (sum__54 + ((*(____in+i__conflict__0__55)) * (*(____in+(i__conflict__0__55 + 8))))))/*float*/;
    }
  }
  for ((i__56 = 0)/*int*/; (i__56 < 32); (i__56++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__54);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__57_19__12__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__54 = 0.0f;/* float */
  int i__conflict__0__55 = 0;/* int */
  int i__56 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__54 = ((float)0.0))/*float*/;
  for ((i__conflict__0__55 = 0)/*int*/; (i__conflict__0__55 < 24); (i__conflict__0__55++)) {{
      (sum__54 = (sum__54 + ((*(____in+i__conflict__0__55)) * (*(____in+(i__conflict__0__55 + 8))))))/*float*/;
    }
  }
  for ((i__56 = 0)/*int*/; (i__56 < 32); (i__56++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__54);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__57_19__12(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__54 = 0.0f;/* float */
      int i__conflict__0__55 = 0;/* int */
      int i__56 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__54 = ((float)0.0))/*float*/;
      for ((i__conflict__0__55 = 0)/*int*/; (i__conflict__0__55 < 24); (i__conflict__0__55++)) {{
          (sum__54 = (sum__54 + (__peek__12(i__conflict__0__55) * __peek__12((i__conflict__0__55 + 8)))))/*float*/;
        }
      }
      for ((i__56 = 0)/*int*/; (i__56 < 32); (i__56++)) {{
          __pop__12();
        }
      }
      __push__12(sum__54);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
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

 
void init_AnonFilter_a0__63_20__13();
inline void check_status__13();

void work_AnonFilter_a0__63_20__13(int);


inline float __pop__13() {
float res=BUFFER_1_13[TAIL_1_13];
TAIL_1_13++;
return res;
}

inline float __pop__13(int n) {
float res=BUFFER_1_13[TAIL_1_13];
TAIL_1_13+=n;

return res;
}

inline float __peek__13(int offs) {
return BUFFER_1_13[TAIL_1_13+offs];
}



inline void __push__13(float data) {
BUFFER_13_3[HEAD_13_3]=data;
HEAD_13_3++;
}



 
void init_AnonFilter_a0__63_20__13(){
}
void save_file_pointer__13(object_write_buffer *buf) {}
void load_file_pointer__13(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__63_20__13__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__60 = 0.0f;/* float */
  int i__conflict__0__61 = 0;/* int */
  int i__62 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__60 = ((float)0.0))/*float*/;
  for ((i__conflict__0__61 = 0)/*int*/; (i__conflict__0__61 < 23); (i__conflict__0__61++)) {{
      (sum__60 = (sum__60 + ((*(____in+i__conflict__0__61)) * (*(____in+(i__conflict__0__61 + 9))))))/*float*/;
    }
  }
  for ((i__62 = 0)/*int*/; (i__62 < 32); (i__62++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__60);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__63_20__13__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__60 = 0.0f;/* float */
  int i__conflict__0__61 = 0;/* int */
  int i__62 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__60 = ((float)0.0))/*float*/;
  for ((i__conflict__0__61 = 0)/*int*/; (i__conflict__0__61 < 23); (i__conflict__0__61++)) {{
      (sum__60 = (sum__60 + ((*(____in+i__conflict__0__61)) * (*(____in+(i__conflict__0__61 + 9))))))/*float*/;
    }
  }
  for ((i__62 = 0)/*int*/; (i__62 < 32); (i__62++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__60);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__63_20__13(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__60 = 0.0f;/* float */
      int i__conflict__0__61 = 0;/* int */
      int i__62 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__60 = ((float)0.0))/*float*/;
      for ((i__conflict__0__61 = 0)/*int*/; (i__conflict__0__61 < 23); (i__conflict__0__61++)) {{
          (sum__60 = (sum__60 + (__peek__13(i__conflict__0__61) * __peek__13((i__conflict__0__61 + 9)))))/*float*/;
        }
      }
      for ((i__62 = 0)/*int*/; (i__62 < 32); (i__62++)) {{
          __pop__13();
        }
      }
      __push__13(sum__60);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
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

 
void init_AnonFilter_a0__69_21__14();
inline void check_status__14();

void work_AnonFilter_a0__69_21__14(int);


inline float __pop__14() {
float res=BUFFER_1_14[TAIL_1_14];
TAIL_1_14++;
return res;
}

inline float __pop__14(int n) {
float res=BUFFER_1_14[TAIL_1_14];
TAIL_1_14+=n;

return res;
}

inline float __peek__14(int offs) {
return BUFFER_1_14[TAIL_1_14+offs];
}



inline void __push__14(float data) {
BUFFER_14_3[HEAD_14_3]=data;
HEAD_14_3++;
}



 
void init_AnonFilter_a0__69_21__14(){
}
void save_file_pointer__14(object_write_buffer *buf) {}
void load_file_pointer__14(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__69_21__14__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__66 = 0.0f;/* float */
  int i__conflict__0__67 = 0;/* int */
  int i__68 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__66 = ((float)0.0))/*float*/;
  for ((i__conflict__0__67 = 0)/*int*/; (i__conflict__0__67 < 22); (i__conflict__0__67++)) {{
      (sum__66 = (sum__66 + ((*(____in+i__conflict__0__67)) * (*(____in+(i__conflict__0__67 + 10))))))/*float*/;
    }
  }
  for ((i__68 = 0)/*int*/; (i__68 < 32); (i__68++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__66);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__69_21__14__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__66 = 0.0f;/* float */
  int i__conflict__0__67 = 0;/* int */
  int i__68 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__66 = ((float)0.0))/*float*/;
  for ((i__conflict__0__67 = 0)/*int*/; (i__conflict__0__67 < 22); (i__conflict__0__67++)) {{
      (sum__66 = (sum__66 + ((*(____in+i__conflict__0__67)) * (*(____in+(i__conflict__0__67 + 10))))))/*float*/;
    }
  }
  for ((i__68 = 0)/*int*/; (i__68 < 32); (i__68++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__66);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__69_21__14(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__66 = 0.0f;/* float */
      int i__conflict__0__67 = 0;/* int */
      int i__68 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__66 = ((float)0.0))/*float*/;
      for ((i__conflict__0__67 = 0)/*int*/; (i__conflict__0__67 < 22); (i__conflict__0__67++)) {{
          (sum__66 = (sum__66 + (__peek__14(i__conflict__0__67) * __peek__14((i__conflict__0__67 + 10)))))/*float*/;
        }
      }
      for ((i__68 = 0)/*int*/; (i__68 < 32); (i__68++)) {{
          __pop__14();
        }
      }
      __push__14(sum__66);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
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

 
void init_AnonFilter_a0__75_22__15();
inline void check_status__15();

void work_AnonFilter_a0__75_22__15(int);


inline float __pop__15() {
float res=BUFFER_1_15[TAIL_1_15];
TAIL_1_15++;
return res;
}

inline float __pop__15(int n) {
float res=BUFFER_1_15[TAIL_1_15];
TAIL_1_15+=n;

return res;
}

inline float __peek__15(int offs) {
return BUFFER_1_15[TAIL_1_15+offs];
}



inline void __push__15(float data) {
BUFFER_15_3[HEAD_15_3]=data;
HEAD_15_3++;
}



 
void init_AnonFilter_a0__75_22__15(){
}
void save_file_pointer__15(object_write_buffer *buf) {}
void load_file_pointer__15(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__75_22__15__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__72 = 0.0f;/* float */
  int i__conflict__0__73 = 0;/* int */
  int i__74 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__72 = ((float)0.0))/*float*/;
  for ((i__conflict__0__73 = 0)/*int*/; (i__conflict__0__73 < 21); (i__conflict__0__73++)) {{
      (sum__72 = (sum__72 + ((*(____in+i__conflict__0__73)) * (*(____in+(i__conflict__0__73 + 11))))))/*float*/;
    }
  }
  for ((i__74 = 0)/*int*/; (i__74 < 32); (i__74++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__72);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__75_22__15__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__72 = 0.0f;/* float */
  int i__conflict__0__73 = 0;/* int */
  int i__74 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__72 = ((float)0.0))/*float*/;
  for ((i__conflict__0__73 = 0)/*int*/; (i__conflict__0__73 < 21); (i__conflict__0__73++)) {{
      (sum__72 = (sum__72 + ((*(____in+i__conflict__0__73)) * (*(____in+(i__conflict__0__73 + 11))))))/*float*/;
    }
  }
  for ((i__74 = 0)/*int*/; (i__74 < 32); (i__74++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__72);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__75_22__15(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__72 = 0.0f;/* float */
      int i__conflict__0__73 = 0;/* int */
      int i__74 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__72 = ((float)0.0))/*float*/;
      for ((i__conflict__0__73 = 0)/*int*/; (i__conflict__0__73 < 21); (i__conflict__0__73++)) {{
          (sum__72 = (sum__72 + (__peek__15(i__conflict__0__73) * __peek__15((i__conflict__0__73 + 11)))))/*float*/;
        }
      }
      for ((i__74 = 0)/*int*/; (i__74 < 32); (i__74++)) {{
          __pop__15();
        }
      }
      __push__15(sum__72);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
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

 
void init_AnonFilter_a0__81_23__16();
inline void check_status__16();

void work_AnonFilter_a0__81_23__16(int);


inline float __pop__16() {
float res=BUFFER_1_16[TAIL_1_16];
TAIL_1_16++;
return res;
}

inline float __pop__16(int n) {
float res=BUFFER_1_16[TAIL_1_16];
TAIL_1_16+=n;

return res;
}

inline float __peek__16(int offs) {
return BUFFER_1_16[TAIL_1_16+offs];
}



inline void __push__16(float data) {
BUFFER_16_3[HEAD_16_3]=data;
HEAD_16_3++;
}



 
void init_AnonFilter_a0__81_23__16(){
}
void save_file_pointer__16(object_write_buffer *buf) {}
void load_file_pointer__16(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__81_23__16__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__78 = 0.0f;/* float */
  int i__conflict__0__79 = 0;/* int */
  int i__80 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__78 = ((float)0.0))/*float*/;
  for ((i__conflict__0__79 = 0)/*int*/; (i__conflict__0__79 < 20); (i__conflict__0__79++)) {{
      (sum__78 = (sum__78 + ((*(____in+i__conflict__0__79)) * (*(____in+(i__conflict__0__79 + 12))))))/*float*/;
    }
  }
  for ((i__80 = 0)/*int*/; (i__80 < 32); (i__80++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__78);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__81_23__16__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__78 = 0.0f;/* float */
  int i__conflict__0__79 = 0;/* int */
  int i__80 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__78 = ((float)0.0))/*float*/;
  for ((i__conflict__0__79 = 0)/*int*/; (i__conflict__0__79 < 20); (i__conflict__0__79++)) {{
      (sum__78 = (sum__78 + ((*(____in+i__conflict__0__79)) * (*(____in+(i__conflict__0__79 + 12))))))/*float*/;
    }
  }
  for ((i__80 = 0)/*int*/; (i__80 < 32); (i__80++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__78);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__81_23__16(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__78 = 0.0f;/* float */
      int i__conflict__0__79 = 0;/* int */
      int i__80 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__78 = ((float)0.0))/*float*/;
      for ((i__conflict__0__79 = 0)/*int*/; (i__conflict__0__79 < 20); (i__conflict__0__79++)) {{
          (sum__78 = (sum__78 + (__peek__16(i__conflict__0__79) * __peek__16((i__conflict__0__79 + 12)))))/*float*/;
        }
      }
      for ((i__80 = 0)/*int*/; (i__80 < 32); (i__80++)) {{
          __pop__16();
        }
      }
      __push__16(sum__78);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
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

 
void init_AnonFilter_a0__87_24__17();
inline void check_status__17();

void work_AnonFilter_a0__87_24__17(int);


inline float __pop__17() {
float res=BUFFER_1_17[TAIL_1_17];
TAIL_1_17++;
return res;
}

inline float __pop__17(int n) {
float res=BUFFER_1_17[TAIL_1_17];
TAIL_1_17+=n;

return res;
}

inline float __peek__17(int offs) {
return BUFFER_1_17[TAIL_1_17+offs];
}



inline void __push__17(float data) {
BUFFER_17_3[HEAD_17_3]=data;
HEAD_17_3++;
}



 
void init_AnonFilter_a0__87_24__17(){
}
void save_file_pointer__17(object_write_buffer *buf) {}
void load_file_pointer__17(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__87_24__17__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__84 = 0.0f;/* float */
  int i__conflict__0__85 = 0;/* int */
  int i__86 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__84 = ((float)0.0))/*float*/;
  for ((i__conflict__0__85 = 0)/*int*/; (i__conflict__0__85 < 19); (i__conflict__0__85++)) {{
      (sum__84 = (sum__84 + ((*(____in+i__conflict__0__85)) * (*(____in+(i__conflict__0__85 + 13))))))/*float*/;
    }
  }
  for ((i__86 = 0)/*int*/; (i__86 < 32); (i__86++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__84);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__87_24__17__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__84 = 0.0f;/* float */
  int i__conflict__0__85 = 0;/* int */
  int i__86 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__84 = ((float)0.0))/*float*/;
  for ((i__conflict__0__85 = 0)/*int*/; (i__conflict__0__85 < 19); (i__conflict__0__85++)) {{
      (sum__84 = (sum__84 + ((*(____in+i__conflict__0__85)) * (*(____in+(i__conflict__0__85 + 13))))))/*float*/;
    }
  }
  for ((i__86 = 0)/*int*/; (i__86 < 32); (i__86++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__84);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__87_24__17(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__84 = 0.0f;/* float */
      int i__conflict__0__85 = 0;/* int */
      int i__86 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__84 = ((float)0.0))/*float*/;
      for ((i__conflict__0__85 = 0)/*int*/; (i__conflict__0__85 < 19); (i__conflict__0__85++)) {{
          (sum__84 = (sum__84 + (__peek__17(i__conflict__0__85) * __peek__17((i__conflict__0__85 + 13)))))/*float*/;
        }
      }
      for ((i__86 = 0)/*int*/; (i__86 < 32); (i__86++)) {{
          __pop__17();
        }
      }
      __push__17(sum__84);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
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

 
void init_AnonFilter_a0__93_25__18();
inline void check_status__18();

void work_AnonFilter_a0__93_25__18(int);


inline float __pop__18() {
float res=BUFFER_1_18[TAIL_1_18];
TAIL_1_18++;
return res;
}

inline float __pop__18(int n) {
float res=BUFFER_1_18[TAIL_1_18];
TAIL_1_18+=n;

return res;
}

inline float __peek__18(int offs) {
return BUFFER_1_18[TAIL_1_18+offs];
}



inline void __push__18(float data) {
BUFFER_18_3[HEAD_18_3]=data;
HEAD_18_3++;
}



 
void init_AnonFilter_a0__93_25__18(){
}
void save_file_pointer__18(object_write_buffer *buf) {}
void load_file_pointer__18(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__93_25__18__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__90 = 0.0f;/* float */
  int i__conflict__0__91 = 0;/* int */
  int i__92 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__90 = ((float)0.0))/*float*/;
  for ((i__conflict__0__91 = 0)/*int*/; (i__conflict__0__91 < 18); (i__conflict__0__91++)) {{
      (sum__90 = (sum__90 + ((*(____in+i__conflict__0__91)) * (*(____in+(i__conflict__0__91 + 14))))))/*float*/;
    }
  }
  for ((i__92 = 0)/*int*/; (i__92 < 32); (i__92++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__90);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__93_25__18__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__90 = 0.0f;/* float */
  int i__conflict__0__91 = 0;/* int */
  int i__92 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__90 = ((float)0.0))/*float*/;
  for ((i__conflict__0__91 = 0)/*int*/; (i__conflict__0__91 < 18); (i__conflict__0__91++)) {{
      (sum__90 = (sum__90 + ((*(____in+i__conflict__0__91)) * (*(____in+(i__conflict__0__91 + 14))))))/*float*/;
    }
  }
  for ((i__92 = 0)/*int*/; (i__92 < 32); (i__92++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__90);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__93_25__18(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__90 = 0.0f;/* float */
      int i__conflict__0__91 = 0;/* int */
      int i__92 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__90 = ((float)0.0))/*float*/;
      for ((i__conflict__0__91 = 0)/*int*/; (i__conflict__0__91 < 18); (i__conflict__0__91++)) {{
          (sum__90 = (sum__90 + (__peek__18(i__conflict__0__91) * __peek__18((i__conflict__0__91 + 14)))))/*float*/;
        }
      }
      for ((i__92 = 0)/*int*/; (i__92 < 32); (i__92++)) {{
          __pop__18();
        }
      }
      __push__18(sum__90);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
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

 
void init_AnonFilter_a0__99_26__19();
inline void check_status__19();

void work_AnonFilter_a0__99_26__19(int);


inline float __pop__19() {
float res=BUFFER_1_19[TAIL_1_19];
TAIL_1_19++;
return res;
}

inline float __pop__19(int n) {
float res=BUFFER_1_19[TAIL_1_19];
TAIL_1_19+=n;

return res;
}

inline float __peek__19(int offs) {
return BUFFER_1_19[TAIL_1_19+offs];
}



inline void __push__19(float data) {
BUFFER_19_3[HEAD_19_3]=data;
HEAD_19_3++;
}



 
void init_AnonFilter_a0__99_26__19(){
}
void save_file_pointer__19(object_write_buffer *buf) {}
void load_file_pointer__19(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__99_26__19__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__96 = 0.0f;/* float */
  int i__conflict__0__97 = 0;/* int */
  int i__98 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__96 = ((float)0.0))/*float*/;
  for ((i__conflict__0__97 = 0)/*int*/; (i__conflict__0__97 < 17); (i__conflict__0__97++)) {{
      (sum__96 = (sum__96 + ((*(____in+i__conflict__0__97)) * (*(____in+(i__conflict__0__97 + 15))))))/*float*/;
    }
  }
  for ((i__98 = 0)/*int*/; (i__98 < 32); (i__98++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__96);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__99_26__19__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__96 = 0.0f;/* float */
  int i__conflict__0__97 = 0;/* int */
  int i__98 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__96 = ((float)0.0))/*float*/;
  for ((i__conflict__0__97 = 0)/*int*/; (i__conflict__0__97 < 17); (i__conflict__0__97++)) {{
      (sum__96 = (sum__96 + ((*(____in+i__conflict__0__97)) * (*(____in+(i__conflict__0__97 + 15))))))/*float*/;
    }
  }
  for ((i__98 = 0)/*int*/; (i__98 < 32); (i__98++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__96);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__99_26__19(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__96 = 0.0f;/* float */
      int i__conflict__0__97 = 0;/* int */
      int i__98 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__96 = ((float)0.0))/*float*/;
      for ((i__conflict__0__97 = 0)/*int*/; (i__conflict__0__97 < 17); (i__conflict__0__97++)) {{
          (sum__96 = (sum__96 + (__peek__19(i__conflict__0__97) * __peek__19((i__conflict__0__97 + 15)))))/*float*/;
        }
      }
      for ((i__98 = 0)/*int*/; (i__98 < 32); (i__98++)) {{
          __pop__19();
        }
      }
      __push__19(sum__96);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
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

 
void init_AnonFilter_a0__105_27__20();
inline void check_status__20();

void work_AnonFilter_a0__105_27__20(int);


inline float __pop__20() {
float res=BUFFER_1_20[TAIL_1_20];
TAIL_1_20++;
return res;
}

inline float __pop__20(int n) {
float res=BUFFER_1_20[TAIL_1_20];
TAIL_1_20+=n;

return res;
}

inline float __peek__20(int offs) {
return BUFFER_1_20[TAIL_1_20+offs];
}



inline void __push__20(float data) {
BUFFER_20_3[HEAD_20_3]=data;
HEAD_20_3++;
}



 
void init_AnonFilter_a0__105_27__20(){
}
void save_file_pointer__20(object_write_buffer *buf) {}
void load_file_pointer__20(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__105_27__20__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__102 = 0.0f;/* float */
  int i__conflict__0__103 = 0;/* int */
  int i__104 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__102 = ((float)0.0))/*float*/;
  for ((i__conflict__0__103 = 0)/*int*/; (i__conflict__0__103 < 16); (i__conflict__0__103++)) {{
      (sum__102 = (sum__102 + ((*(____in+i__conflict__0__103)) * (*(____in+(i__conflict__0__103 + 16))))))/*float*/;
    }
  }
  for ((i__104 = 0)/*int*/; (i__104 < 32); (i__104++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__102);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__105_27__20__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__102 = 0.0f;/* float */
  int i__conflict__0__103 = 0;/* int */
  int i__104 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__102 = ((float)0.0))/*float*/;
  for ((i__conflict__0__103 = 0)/*int*/; (i__conflict__0__103 < 16); (i__conflict__0__103++)) {{
      (sum__102 = (sum__102 + ((*(____in+i__conflict__0__103)) * (*(____in+(i__conflict__0__103 + 16))))))/*float*/;
    }
  }
  for ((i__104 = 0)/*int*/; (i__104 < 32); (i__104++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__102);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__105_27__20(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__102 = 0.0f;/* float */
      int i__conflict__0__103 = 0;/* int */
      int i__104 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__102 = ((float)0.0))/*float*/;
      for ((i__conflict__0__103 = 0)/*int*/; (i__conflict__0__103 < 16); (i__conflict__0__103++)) {{
          (sum__102 = (sum__102 + (__peek__20(i__conflict__0__103) * __peek__20((i__conflict__0__103 + 16)))))/*float*/;
        }
      }
      for ((i__104 = 0)/*int*/; (i__104 < 32); (i__104++)) {{
          __pop__20();
        }
      }
      __push__20(sum__102);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

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

 
void init_AnonFilter_a0__111_28__21();
inline void check_status__21();

void work_AnonFilter_a0__111_28__21(int);


inline float __pop__21() {
float res=BUFFER_1_21[TAIL_1_21];
TAIL_1_21++;
return res;
}

inline float __pop__21(int n) {
float res=BUFFER_1_21[TAIL_1_21];
TAIL_1_21+=n;

return res;
}

inline float __peek__21(int offs) {
return BUFFER_1_21[TAIL_1_21+offs];
}



inline void __push__21(float data) {
BUFFER_21_3[HEAD_21_3]=data;
HEAD_21_3++;
}



 
void init_AnonFilter_a0__111_28__21(){
}
void save_file_pointer__21(object_write_buffer *buf) {}
void load_file_pointer__21(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__111_28__21__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__108 = 0.0f;/* float */
  int i__conflict__0__109 = 0;/* int */
  int i__110 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__108 = ((float)0.0))/*float*/;
  for ((i__conflict__0__109 = 0)/*int*/; (i__conflict__0__109 < 15); (i__conflict__0__109++)) {{
      (sum__108 = (sum__108 + ((*(____in+i__conflict__0__109)) * (*(____in+(i__conflict__0__109 + 17))))))/*float*/;
    }
  }
  for ((i__110 = 0)/*int*/; (i__110 < 32); (i__110++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__108);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__111_28__21__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__108 = 0.0f;/* float */
  int i__conflict__0__109 = 0;/* int */
  int i__110 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__108 = ((float)0.0))/*float*/;
  for ((i__conflict__0__109 = 0)/*int*/; (i__conflict__0__109 < 15); (i__conflict__0__109++)) {{
      (sum__108 = (sum__108 + ((*(____in+i__conflict__0__109)) * (*(____in+(i__conflict__0__109 + 17))))))/*float*/;
    }
  }
  for ((i__110 = 0)/*int*/; (i__110 < 32); (i__110++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__108);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__111_28__21(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__108 = 0.0f;/* float */
      int i__conflict__0__109 = 0;/* int */
      int i__110 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__108 = ((float)0.0))/*float*/;
      for ((i__conflict__0__109 = 0)/*int*/; (i__conflict__0__109 < 15); (i__conflict__0__109++)) {{
          (sum__108 = (sum__108 + (__peek__21(i__conflict__0__109) * __peek__21((i__conflict__0__109 + 17)))))/*float*/;
        }
      }
      for ((i__110 = 0)/*int*/; (i__110 < 32); (i__110++)) {{
          __pop__21();
        }
      }
      __push__21(sum__108);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
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

 
void init_AnonFilter_a0__117_29__22();
inline void check_status__22();

void work_AnonFilter_a0__117_29__22(int);


inline float __pop__22() {
float res=BUFFER_1_22[TAIL_1_22];
TAIL_1_22++;
return res;
}

inline float __pop__22(int n) {
float res=BUFFER_1_22[TAIL_1_22];
TAIL_1_22+=n;

return res;
}

inline float __peek__22(int offs) {
return BUFFER_1_22[TAIL_1_22+offs];
}



inline void __push__22(float data) {
BUFFER_22_3[HEAD_22_3]=data;
HEAD_22_3++;
}



 
void init_AnonFilter_a0__117_29__22(){
}
void save_file_pointer__22(object_write_buffer *buf) {}
void load_file_pointer__22(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__117_29__22__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__114 = 0.0f;/* float */
  int i__conflict__0__115 = 0;/* int */
  int i__116 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__114 = ((float)0.0))/*float*/;
  for ((i__conflict__0__115 = 0)/*int*/; (i__conflict__0__115 < 14); (i__conflict__0__115++)) {{
      (sum__114 = (sum__114 + ((*(____in+i__conflict__0__115)) * (*(____in+(i__conflict__0__115 + 18))))))/*float*/;
    }
  }
  for ((i__116 = 0)/*int*/; (i__116 < 32); (i__116++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__114);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__117_29__22__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__114 = 0.0f;/* float */
  int i__conflict__0__115 = 0;/* int */
  int i__116 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__114 = ((float)0.0))/*float*/;
  for ((i__conflict__0__115 = 0)/*int*/; (i__conflict__0__115 < 14); (i__conflict__0__115++)) {{
      (sum__114 = (sum__114 + ((*(____in+i__conflict__0__115)) * (*(____in+(i__conflict__0__115 + 18))))))/*float*/;
    }
  }
  for ((i__116 = 0)/*int*/; (i__116 < 32); (i__116++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__114);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__117_29__22(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__114 = 0.0f;/* float */
      int i__conflict__0__115 = 0;/* int */
      int i__116 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__114 = ((float)0.0))/*float*/;
      for ((i__conflict__0__115 = 0)/*int*/; (i__conflict__0__115 < 14); (i__conflict__0__115++)) {{
          (sum__114 = (sum__114 + (__peek__22(i__conflict__0__115) * __peek__22((i__conflict__0__115 + 18)))))/*float*/;
        }
      }
      for ((i__116 = 0)/*int*/; (i__116 < 32); (i__116++)) {{
          __pop__22();
        }
      }
      __push__22(sum__114);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
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

 
void init_AnonFilter_a0__123_30__23();
inline void check_status__23();

void work_AnonFilter_a0__123_30__23(int);


inline float __pop__23() {
float res=BUFFER_1_23[TAIL_1_23];
TAIL_1_23++;
return res;
}

inline float __pop__23(int n) {
float res=BUFFER_1_23[TAIL_1_23];
TAIL_1_23+=n;

return res;
}

inline float __peek__23(int offs) {
return BUFFER_1_23[TAIL_1_23+offs];
}



inline void __push__23(float data) {
BUFFER_23_3[HEAD_23_3]=data;
HEAD_23_3++;
}



 
void init_AnonFilter_a0__123_30__23(){
}
void save_file_pointer__23(object_write_buffer *buf) {}
void load_file_pointer__23(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__123_30__23__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__120 = 0.0f;/* float */
  int i__conflict__0__121 = 0;/* int */
  int i__122 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__120 = ((float)0.0))/*float*/;
  for ((i__conflict__0__121 = 0)/*int*/; (i__conflict__0__121 < 13); (i__conflict__0__121++)) {{
      (sum__120 = (sum__120 + ((*(____in+i__conflict__0__121)) * (*(____in+(i__conflict__0__121 + 19))))))/*float*/;
    }
  }
  for ((i__122 = 0)/*int*/; (i__122 < 32); (i__122++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__120);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__123_30__23__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__120 = 0.0f;/* float */
  int i__conflict__0__121 = 0;/* int */
  int i__122 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__120 = ((float)0.0))/*float*/;
  for ((i__conflict__0__121 = 0)/*int*/; (i__conflict__0__121 < 13); (i__conflict__0__121++)) {{
      (sum__120 = (sum__120 + ((*(____in+i__conflict__0__121)) * (*(____in+(i__conflict__0__121 + 19))))))/*float*/;
    }
  }
  for ((i__122 = 0)/*int*/; (i__122 < 32); (i__122++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__120);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__123_30__23(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__120 = 0.0f;/* float */
      int i__conflict__0__121 = 0;/* int */
      int i__122 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__120 = ((float)0.0))/*float*/;
      for ((i__conflict__0__121 = 0)/*int*/; (i__conflict__0__121 < 13); (i__conflict__0__121++)) {{
          (sum__120 = (sum__120 + (__peek__23(i__conflict__0__121) * __peek__23((i__conflict__0__121 + 19)))))/*float*/;
        }
      }
      for ((i__122 = 0)/*int*/; (i__122 < 32); (i__122++)) {{
          __pop__23();
        }
      }
      __push__23(sum__120);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

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

 
void init_AnonFilter_a0__129_31__24();
inline void check_status__24();

void work_AnonFilter_a0__129_31__24(int);


inline float __pop__24() {
float res=BUFFER_1_24[TAIL_1_24];
TAIL_1_24++;
return res;
}

inline float __pop__24(int n) {
float res=BUFFER_1_24[TAIL_1_24];
TAIL_1_24+=n;

return res;
}

inline float __peek__24(int offs) {
return BUFFER_1_24[TAIL_1_24+offs];
}



inline void __push__24(float data) {
BUFFER_24_3[HEAD_24_3]=data;
HEAD_24_3++;
}



 
void init_AnonFilter_a0__129_31__24(){
}
void save_file_pointer__24(object_write_buffer *buf) {}
void load_file_pointer__24(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__129_31__24__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__126 = 0.0f;/* float */
  int i__conflict__0__127 = 0;/* int */
  int i__128 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__126 = ((float)0.0))/*float*/;
  for ((i__conflict__0__127 = 0)/*int*/; (i__conflict__0__127 < 12); (i__conflict__0__127++)) {{
      (sum__126 = (sum__126 + ((*(____in+i__conflict__0__127)) * (*(____in+(i__conflict__0__127 + 20))))))/*float*/;
    }
  }
  for ((i__128 = 0)/*int*/; (i__128 < 32); (i__128++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__126);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__129_31__24__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__126 = 0.0f;/* float */
  int i__conflict__0__127 = 0;/* int */
  int i__128 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__126 = ((float)0.0))/*float*/;
  for ((i__conflict__0__127 = 0)/*int*/; (i__conflict__0__127 < 12); (i__conflict__0__127++)) {{
      (sum__126 = (sum__126 + ((*(____in+i__conflict__0__127)) * (*(____in+(i__conflict__0__127 + 20))))))/*float*/;
    }
  }
  for ((i__128 = 0)/*int*/; (i__128 < 32); (i__128++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__126);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__129_31__24(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__126 = 0.0f;/* float */
      int i__conflict__0__127 = 0;/* int */
      int i__128 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__126 = ((float)0.0))/*float*/;
      for ((i__conflict__0__127 = 0)/*int*/; (i__conflict__0__127 < 12); (i__conflict__0__127++)) {{
          (sum__126 = (sum__126 + (__peek__24(i__conflict__0__127) * __peek__24((i__conflict__0__127 + 20)))))/*float*/;
        }
      }
      for ((i__128 = 0)/*int*/; (i__128 < 32); (i__128++)) {{
          __pop__24();
        }
      }
      __push__24(sum__126);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

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

 
void init_AnonFilter_a0__135_32__25();
inline void check_status__25();

void work_AnonFilter_a0__135_32__25(int);


inline float __pop__25() {
float res=BUFFER_1_25[TAIL_1_25];
TAIL_1_25++;
return res;
}

inline float __pop__25(int n) {
float res=BUFFER_1_25[TAIL_1_25];
TAIL_1_25+=n;

return res;
}

inline float __peek__25(int offs) {
return BUFFER_1_25[TAIL_1_25+offs];
}



inline void __push__25(float data) {
BUFFER_25_3[HEAD_25_3]=data;
HEAD_25_3++;
}



 
void init_AnonFilter_a0__135_32__25(){
}
void save_file_pointer__25(object_write_buffer *buf) {}
void load_file_pointer__25(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__135_32__25__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__132 = 0.0f;/* float */
  int i__conflict__0__133 = 0;/* int */
  int i__134 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__132 = ((float)0.0))/*float*/;
  for ((i__conflict__0__133 = 0)/*int*/; (i__conflict__0__133 < 11); (i__conflict__0__133++)) {{
      (sum__132 = (sum__132 + ((*(____in+i__conflict__0__133)) * (*(____in+(i__conflict__0__133 + 21))))))/*float*/;
    }
  }
  for ((i__134 = 0)/*int*/; (i__134 < 32); (i__134++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__132);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__135_32__25__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__132 = 0.0f;/* float */
  int i__conflict__0__133 = 0;/* int */
  int i__134 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__132 = ((float)0.0))/*float*/;
  for ((i__conflict__0__133 = 0)/*int*/; (i__conflict__0__133 < 11); (i__conflict__0__133++)) {{
      (sum__132 = (sum__132 + ((*(____in+i__conflict__0__133)) * (*(____in+(i__conflict__0__133 + 21))))))/*float*/;
    }
  }
  for ((i__134 = 0)/*int*/; (i__134 < 32); (i__134++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__132);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__135_32__25(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__132 = 0.0f;/* float */
      int i__conflict__0__133 = 0;/* int */
      int i__134 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__132 = ((float)0.0))/*float*/;
      for ((i__conflict__0__133 = 0)/*int*/; (i__conflict__0__133 < 11); (i__conflict__0__133++)) {{
          (sum__132 = (sum__132 + (__peek__25(i__conflict__0__133) * __peek__25((i__conflict__0__133 + 21)))))/*float*/;
        }
      }
      for ((i__134 = 0)/*int*/; (i__134 < 32); (i__134++)) {{
          __pop__25();
        }
      }
      __push__25(sum__132);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
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

 
void init_AnonFilter_a0__141_33__26();
inline void check_status__26();

void work_AnonFilter_a0__141_33__26(int);


inline float __pop__26() {
float res=BUFFER_1_26[TAIL_1_26];
TAIL_1_26++;
return res;
}

inline float __pop__26(int n) {
float res=BUFFER_1_26[TAIL_1_26];
TAIL_1_26+=n;

return res;
}

inline float __peek__26(int offs) {
return BUFFER_1_26[TAIL_1_26+offs];
}



inline void __push__26(float data) {
BUFFER_26_3[HEAD_26_3]=data;
HEAD_26_3++;
}



 
void init_AnonFilter_a0__141_33__26(){
}
void save_file_pointer__26(object_write_buffer *buf) {}
void load_file_pointer__26(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__141_33__26__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__138 = 0.0f;/* float */
  int i__conflict__0__139 = 0;/* int */
  int i__140 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__138 = ((float)0.0))/*float*/;
  for ((i__conflict__0__139 = 0)/*int*/; (i__conflict__0__139 < 10); (i__conflict__0__139++)) {{
      (sum__138 = (sum__138 + ((*(____in+i__conflict__0__139)) * (*(____in+(i__conflict__0__139 + 22))))))/*float*/;
    }
  }
  for ((i__140 = 0)/*int*/; (i__140 < 32); (i__140++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__138);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__141_33__26__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__138 = 0.0f;/* float */
  int i__conflict__0__139 = 0;/* int */
  int i__140 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__138 = ((float)0.0))/*float*/;
  for ((i__conflict__0__139 = 0)/*int*/; (i__conflict__0__139 < 10); (i__conflict__0__139++)) {{
      (sum__138 = (sum__138 + ((*(____in+i__conflict__0__139)) * (*(____in+(i__conflict__0__139 + 22))))))/*float*/;
    }
  }
  for ((i__140 = 0)/*int*/; (i__140 < 32); (i__140++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__138);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__141_33__26(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__138 = 0.0f;/* float */
      int i__conflict__0__139 = 0;/* int */
      int i__140 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__138 = ((float)0.0))/*float*/;
      for ((i__conflict__0__139 = 0)/*int*/; (i__conflict__0__139 < 10); (i__conflict__0__139++)) {{
          (sum__138 = (sum__138 + (__peek__26(i__conflict__0__139) * __peek__26((i__conflict__0__139 + 22)))))/*float*/;
        }
      }
      for ((i__140 = 0)/*int*/; (i__140 < 32); (i__140++)) {{
          __pop__26();
        }
      }
      __push__26(sum__138);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

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

 
void init_AnonFilter_a0__147_34__27();
inline void check_status__27();

void work_AnonFilter_a0__147_34__27(int);


inline float __pop__27() {
float res=BUFFER_1_27[TAIL_1_27];
TAIL_1_27++;
return res;
}

inline float __pop__27(int n) {
float res=BUFFER_1_27[TAIL_1_27];
TAIL_1_27+=n;

return res;
}

inline float __peek__27(int offs) {
return BUFFER_1_27[TAIL_1_27+offs];
}



inline void __push__27(float data) {
BUFFER_27_3[HEAD_27_3]=data;
HEAD_27_3++;
}



 
void init_AnonFilter_a0__147_34__27(){
}
void save_file_pointer__27(object_write_buffer *buf) {}
void load_file_pointer__27(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__147_34__27__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__144 = 0.0f;/* float */
  int i__conflict__0__145 = 0;/* int */
  int i__146 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__144 = ((float)0.0))/*float*/;
  for ((i__conflict__0__145 = 0)/*int*/; (i__conflict__0__145 < 9); (i__conflict__0__145++)) {{
      (sum__144 = (sum__144 + ((*(____in+i__conflict__0__145)) * (*(____in+(i__conflict__0__145 + 23))))))/*float*/;
    }
  }
  for ((i__146 = 0)/*int*/; (i__146 < 32); (i__146++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__144);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__147_34__27__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__144 = 0.0f;/* float */
  int i__conflict__0__145 = 0;/* int */
  int i__146 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__144 = ((float)0.0))/*float*/;
  for ((i__conflict__0__145 = 0)/*int*/; (i__conflict__0__145 < 9); (i__conflict__0__145++)) {{
      (sum__144 = (sum__144 + ((*(____in+i__conflict__0__145)) * (*(____in+(i__conflict__0__145 + 23))))))/*float*/;
    }
  }
  for ((i__146 = 0)/*int*/; (i__146 < 32); (i__146++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__144);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__147_34__27(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__144 = 0.0f;/* float */
      int i__conflict__0__145 = 0;/* int */
      int i__146 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__144 = ((float)0.0))/*float*/;
      for ((i__conflict__0__145 = 0)/*int*/; (i__conflict__0__145 < 9); (i__conflict__0__145++)) {{
          (sum__144 = (sum__144 + (__peek__27(i__conflict__0__145) * __peek__27((i__conflict__0__145 + 23)))))/*float*/;
        }
      }
      for ((i__146 = 0)/*int*/; (i__146 < 32); (i__146++)) {{
          __pop__27();
        }
      }
      __push__27(sum__144);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
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

 
void init_AnonFilter_a0__153_35__28();
inline void check_status__28();

void work_AnonFilter_a0__153_35__28(int);


inline float __pop__28() {
float res=BUFFER_1_28[TAIL_1_28];
TAIL_1_28++;
return res;
}

inline float __pop__28(int n) {
float res=BUFFER_1_28[TAIL_1_28];
TAIL_1_28+=n;

return res;
}

inline float __peek__28(int offs) {
return BUFFER_1_28[TAIL_1_28+offs];
}



inline void __push__28(float data) {
BUFFER_28_3[HEAD_28_3]=data;
HEAD_28_3++;
}



 
void init_AnonFilter_a0__153_35__28(){
}
void save_file_pointer__28(object_write_buffer *buf) {}
void load_file_pointer__28(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__153_35__28__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__150 = 0.0f;/* float */
  int i__conflict__0__151 = 0;/* int */
  int i__152 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__150 = ((float)0.0))/*float*/;
  for ((i__conflict__0__151 = 0)/*int*/; (i__conflict__0__151 < 8); (i__conflict__0__151++)) {{
      (sum__150 = (sum__150 + ((*(____in+i__conflict__0__151)) * (*(____in+(i__conflict__0__151 + 24))))))/*float*/;
    }
  }
  for ((i__152 = 0)/*int*/; (i__152 < 32); (i__152++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__150);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__153_35__28__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__150 = 0.0f;/* float */
  int i__conflict__0__151 = 0;/* int */
  int i__152 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__150 = ((float)0.0))/*float*/;
  for ((i__conflict__0__151 = 0)/*int*/; (i__conflict__0__151 < 8); (i__conflict__0__151++)) {{
      (sum__150 = (sum__150 + ((*(____in+i__conflict__0__151)) * (*(____in+(i__conflict__0__151 + 24))))))/*float*/;
    }
  }
  for ((i__152 = 0)/*int*/; (i__152 < 32); (i__152++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__150);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__153_35__28(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__150 = 0.0f;/* float */
      int i__conflict__0__151 = 0;/* int */
      int i__152 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__150 = ((float)0.0))/*float*/;
      for ((i__conflict__0__151 = 0)/*int*/; (i__conflict__0__151 < 8); (i__conflict__0__151++)) {{
          (sum__150 = (sum__150 + (__peek__28(i__conflict__0__151) * __peek__28((i__conflict__0__151 + 24)))))/*float*/;
        }
      }
      for ((i__152 = 0)/*int*/; (i__152 < 32); (i__152++)) {{
          __pop__28();
        }
      }
      __push__28(sum__150);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

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

 
void init_AnonFilter_a0__159_36__29();
inline void check_status__29();

void work_AnonFilter_a0__159_36__29(int);


inline float __pop__29() {
float res=BUFFER_1_29[TAIL_1_29];
TAIL_1_29++;
return res;
}

inline float __pop__29(int n) {
float res=BUFFER_1_29[TAIL_1_29];
TAIL_1_29+=n;

return res;
}

inline float __peek__29(int offs) {
return BUFFER_1_29[TAIL_1_29+offs];
}



inline void __push__29(float data) {
BUFFER_29_3[HEAD_29_3]=data;
HEAD_29_3++;
}



 
void init_AnonFilter_a0__159_36__29(){
}
void save_file_pointer__29(object_write_buffer *buf) {}
void load_file_pointer__29(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__159_36__29__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__156 = 0.0f;/* float */
  int i__conflict__0__157 = 0;/* int */
  int i__158 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__156 = ((float)0.0))/*float*/;
  for ((i__conflict__0__157 = 0)/*int*/; (i__conflict__0__157 < 7); (i__conflict__0__157++)) {{
      (sum__156 = (sum__156 + ((*(____in+i__conflict__0__157)) * (*(____in+(i__conflict__0__157 + 25))))))/*float*/;
    }
  }
  for ((i__158 = 0)/*int*/; (i__158 < 32); (i__158++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__156);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__159_36__29__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__156 = 0.0f;/* float */
  int i__conflict__0__157 = 0;/* int */
  int i__158 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__156 = ((float)0.0))/*float*/;
  for ((i__conflict__0__157 = 0)/*int*/; (i__conflict__0__157 < 7); (i__conflict__0__157++)) {{
      (sum__156 = (sum__156 + ((*(____in+i__conflict__0__157)) * (*(____in+(i__conflict__0__157 + 25))))))/*float*/;
    }
  }
  for ((i__158 = 0)/*int*/; (i__158 < 32); (i__158++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__156);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__159_36__29(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__156 = 0.0f;/* float */
      int i__conflict__0__157 = 0;/* int */
      int i__158 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__156 = ((float)0.0))/*float*/;
      for ((i__conflict__0__157 = 0)/*int*/; (i__conflict__0__157 < 7); (i__conflict__0__157++)) {{
          (sum__156 = (sum__156 + (__peek__29(i__conflict__0__157) * __peek__29((i__conflict__0__157 + 25)))))/*float*/;
        }
      }
      for ((i__158 = 0)/*int*/; (i__158 < 32); (i__158++)) {{
          __pop__29();
        }
      }
      __push__29(sum__156);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

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

 
void init_AnonFilter_a0__165_37__30();
inline void check_status__30();

void work_AnonFilter_a0__165_37__30(int);


inline float __pop__30() {
float res=BUFFER_1_30[TAIL_1_30];
TAIL_1_30++;
return res;
}

inline float __pop__30(int n) {
float res=BUFFER_1_30[TAIL_1_30];
TAIL_1_30+=n;

return res;
}

inline float __peek__30(int offs) {
return BUFFER_1_30[TAIL_1_30+offs];
}



inline void __push__30(float data) {
BUFFER_30_3[HEAD_30_3]=data;
HEAD_30_3++;
}



 
void init_AnonFilter_a0__165_37__30(){
}
void save_file_pointer__30(object_write_buffer *buf) {}
void load_file_pointer__30(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__165_37__30__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__162 = 0.0f;/* float */
  int i__conflict__0__163 = 0;/* int */
  int i__164 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__162 = ((float)0.0))/*float*/;
  for ((i__conflict__0__163 = 0)/*int*/; (i__conflict__0__163 < 6); (i__conflict__0__163++)) {{
      (sum__162 = (sum__162 + ((*(____in+i__conflict__0__163)) * (*(____in+(i__conflict__0__163 + 26))))))/*float*/;
    }
  }
  for ((i__164 = 0)/*int*/; (i__164 < 32); (i__164++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__162);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__165_37__30__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__162 = 0.0f;/* float */
  int i__conflict__0__163 = 0;/* int */
  int i__164 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__162 = ((float)0.0))/*float*/;
  for ((i__conflict__0__163 = 0)/*int*/; (i__conflict__0__163 < 6); (i__conflict__0__163++)) {{
      (sum__162 = (sum__162 + ((*(____in+i__conflict__0__163)) * (*(____in+(i__conflict__0__163 + 26))))))/*float*/;
    }
  }
  for ((i__164 = 0)/*int*/; (i__164 < 32); (i__164++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__162);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__165_37__30(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__162 = 0.0f;/* float */
      int i__conflict__0__163 = 0;/* int */
      int i__164 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__162 = ((float)0.0))/*float*/;
      for ((i__conflict__0__163 = 0)/*int*/; (i__conflict__0__163 < 6); (i__conflict__0__163++)) {{
          (sum__162 = (sum__162 + (__peek__30(i__conflict__0__163) * __peek__30((i__conflict__0__163 + 26)))))/*float*/;
        }
      }
      for ((i__164 = 0)/*int*/; (i__164 < 32); (i__164++)) {{
          __pop__30();
        }
      }
      __push__30(sum__162);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

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

 
void init_AnonFilter_a0__171_38__31();
inline void check_status__31();

void work_AnonFilter_a0__171_38__31(int);


inline float __pop__31() {
float res=BUFFER_1_31[TAIL_1_31];
TAIL_1_31++;
return res;
}

inline float __pop__31(int n) {
float res=BUFFER_1_31[TAIL_1_31];
TAIL_1_31+=n;

return res;
}

inline float __peek__31(int offs) {
return BUFFER_1_31[TAIL_1_31+offs];
}



inline void __push__31(float data) {
BUFFER_31_3[HEAD_31_3]=data;
HEAD_31_3++;
}



 
void init_AnonFilter_a0__171_38__31(){
}
void save_file_pointer__31(object_write_buffer *buf) {}
void load_file_pointer__31(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__171_38__31__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__168 = 0.0f;/* float */
  int i__conflict__0__169 = 0;/* int */
  int i__170 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__168 = ((float)0.0))/*float*/;
  for ((i__conflict__0__169 = 0)/*int*/; (i__conflict__0__169 < 5); (i__conflict__0__169++)) {{
      (sum__168 = (sum__168 + ((*(____in+i__conflict__0__169)) * (*(____in+(i__conflict__0__169 + 27))))))/*float*/;
    }
  }
  for ((i__170 = 0)/*int*/; (i__170 < 32); (i__170++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__168);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__171_38__31__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__168 = 0.0f;/* float */
  int i__conflict__0__169 = 0;/* int */
  int i__170 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__168 = ((float)0.0))/*float*/;
  for ((i__conflict__0__169 = 0)/*int*/; (i__conflict__0__169 < 5); (i__conflict__0__169++)) {{
      (sum__168 = (sum__168 + ((*(____in+i__conflict__0__169)) * (*(____in+(i__conflict__0__169 + 27))))))/*float*/;
    }
  }
  for ((i__170 = 0)/*int*/; (i__170 < 32); (i__170++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__168);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__171_38__31(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__168 = 0.0f;/* float */
      int i__conflict__0__169 = 0;/* int */
      int i__170 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__168 = ((float)0.0))/*float*/;
      for ((i__conflict__0__169 = 0)/*int*/; (i__conflict__0__169 < 5); (i__conflict__0__169++)) {{
          (sum__168 = (sum__168 + (__peek__31(i__conflict__0__169) * __peek__31((i__conflict__0__169 + 27)))))/*float*/;
        }
      }
      for ((i__170 = 0)/*int*/; (i__170 < 32); (i__170++)) {{
          __pop__31();
        }
      }
      __push__31(sum__168);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
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

 
void init_AnonFilter_a0__177_39__32();
inline void check_status__32();

void work_AnonFilter_a0__177_39__32(int);


inline float __pop__32() {
float res=BUFFER_1_32[TAIL_1_32];
TAIL_1_32++;
return res;
}

inline float __pop__32(int n) {
float res=BUFFER_1_32[TAIL_1_32];
TAIL_1_32+=n;

return res;
}

inline float __peek__32(int offs) {
return BUFFER_1_32[TAIL_1_32+offs];
}



inline void __push__32(float data) {
BUFFER_32_3[HEAD_32_3]=data;
HEAD_32_3++;
}



 
void init_AnonFilter_a0__177_39__32(){
}
void save_file_pointer__32(object_write_buffer *buf) {}
void load_file_pointer__32(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__177_39__32__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__174 = 0.0f;/* float */
  int i__conflict__0__175 = 0;/* int */
  int i__176 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__174 = ((float)0.0))/*float*/;
  for ((i__conflict__0__175 = 0)/*int*/; (i__conflict__0__175 < 4); (i__conflict__0__175++)) {{
      (sum__174 = (sum__174 + ((*(____in+i__conflict__0__175)) * (*(____in+(i__conflict__0__175 + 28))))))/*float*/;
    }
  }
  for ((i__176 = 0)/*int*/; (i__176 < 32); (i__176++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__174);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__177_39__32__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__174 = 0.0f;/* float */
  int i__conflict__0__175 = 0;/* int */
  int i__176 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__174 = ((float)0.0))/*float*/;
  for ((i__conflict__0__175 = 0)/*int*/; (i__conflict__0__175 < 4); (i__conflict__0__175++)) {{
      (sum__174 = (sum__174 + ((*(____in+i__conflict__0__175)) * (*(____in+(i__conflict__0__175 + 28))))))/*float*/;
    }
  }
  for ((i__176 = 0)/*int*/; (i__176 < 32); (i__176++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__174);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__177_39__32(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__174 = 0.0f;/* float */
      int i__conflict__0__175 = 0;/* int */
      int i__176 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__174 = ((float)0.0))/*float*/;
      for ((i__conflict__0__175 = 0)/*int*/; (i__conflict__0__175 < 4); (i__conflict__0__175++)) {{
          (sum__174 = (sum__174 + (__peek__32(i__conflict__0__175) * __peek__32((i__conflict__0__175 + 28)))))/*float*/;
        }
      }
      for ((i__176 = 0)/*int*/; (i__176 < 32); (i__176++)) {{
          __pop__32();
        }
      }
      __push__32(sum__174);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

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

 
void init_AnonFilter_a0__183_40__33();
inline void check_status__33();

void work_AnonFilter_a0__183_40__33(int);


inline float __pop__33() {
float res=BUFFER_1_33[TAIL_1_33];
TAIL_1_33++;
return res;
}

inline float __pop__33(int n) {
float res=BUFFER_1_33[TAIL_1_33];
TAIL_1_33+=n;

return res;
}

inline float __peek__33(int offs) {
return BUFFER_1_33[TAIL_1_33+offs];
}



inline void __push__33(float data) {
BUFFER_33_3[HEAD_33_3]=data;
HEAD_33_3++;
}



 
void init_AnonFilter_a0__183_40__33(){
}
void save_file_pointer__33(object_write_buffer *buf) {}
void load_file_pointer__33(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__183_40__33__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__180 = 0.0f;/* float */
  int i__conflict__0__181 = 0;/* int */
  int i__182 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__180 = ((float)0.0))/*float*/;
  for ((i__conflict__0__181 = 0)/*int*/; (i__conflict__0__181 < 3); (i__conflict__0__181++)) {{
      (sum__180 = (sum__180 + ((*(____in+i__conflict__0__181)) * (*(____in+(i__conflict__0__181 + 29))))))/*float*/;
    }
  }
  for ((i__182 = 0)/*int*/; (i__182 < 32); (i__182++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__180);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__183_40__33__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__180 = 0.0f;/* float */
  int i__conflict__0__181 = 0;/* int */
  int i__182 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__180 = ((float)0.0))/*float*/;
  for ((i__conflict__0__181 = 0)/*int*/; (i__conflict__0__181 < 3); (i__conflict__0__181++)) {{
      (sum__180 = (sum__180 + ((*(____in+i__conflict__0__181)) * (*(____in+(i__conflict__0__181 + 29))))))/*float*/;
    }
  }
  for ((i__182 = 0)/*int*/; (i__182 < 32); (i__182++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__180);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__183_40__33(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__180 = 0.0f;/* float */
      int i__conflict__0__181 = 0;/* int */
      int i__182 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__180 = ((float)0.0))/*float*/;
      for ((i__conflict__0__181 = 0)/*int*/; (i__conflict__0__181 < 3); (i__conflict__0__181++)) {{
          (sum__180 = (sum__180 + (__peek__33(i__conflict__0__181) * __peek__33((i__conflict__0__181 + 29)))))/*float*/;
        }
      }
      for ((i__182 = 0)/*int*/; (i__182 < 32); (i__182++)) {{
          __pop__33();
        }
      }
      __push__33(sum__180);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
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

 
void init_AnonFilter_a0__189_41__34();
inline void check_status__34();

void work_AnonFilter_a0__189_41__34(int);


inline float __pop__34() {
float res=BUFFER_1_34[TAIL_1_34];
TAIL_1_34++;
return res;
}

inline float __pop__34(int n) {
float res=BUFFER_1_34[TAIL_1_34];
TAIL_1_34+=n;

return res;
}

inline float __peek__34(int offs) {
return BUFFER_1_34[TAIL_1_34+offs];
}



inline void __push__34(float data) {
BUFFER_34_3[HEAD_34_3]=data;
HEAD_34_3++;
}



 
void init_AnonFilter_a0__189_41__34(){
}
void save_file_pointer__34(object_write_buffer *buf) {}
void load_file_pointer__34(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__189_41__34__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__186 = 0.0f;/* float */
  int i__conflict__0__187 = 0;/* int */
  int i__188 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__186 = ((float)0.0))/*float*/;
  for ((i__conflict__0__187 = 0)/*int*/; (i__conflict__0__187 < 2); (i__conflict__0__187++)) {{
      (sum__186 = (sum__186 + ((*(____in+i__conflict__0__187)) * (*(____in+(i__conflict__0__187 + 30))))))/*float*/;
    }
  }
  for ((i__188 = 0)/*int*/; (i__188 < 32); (i__188++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__186);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__189_41__34__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__186 = 0.0f;/* float */
  int i__conflict__0__187 = 0;/* int */
  int i__188 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__186 = ((float)0.0))/*float*/;
  for ((i__conflict__0__187 = 0)/*int*/; (i__conflict__0__187 < 2); (i__conflict__0__187++)) {{
      (sum__186 = (sum__186 + ((*(____in+i__conflict__0__187)) * (*(____in+(i__conflict__0__187 + 30))))))/*float*/;
    }
  }
  for ((i__188 = 0)/*int*/; (i__188 < 32); (i__188++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__186);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__189_41__34(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__186 = 0.0f;/* float */
      int i__conflict__0__187 = 0;/* int */
      int i__188 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__186 = ((float)0.0))/*float*/;
      for ((i__conflict__0__187 = 0)/*int*/; (i__conflict__0__187 < 2); (i__conflict__0__187++)) {{
          (sum__186 = (sum__186 + (__peek__34(i__conflict__0__187) * __peek__34((i__conflict__0__187 + 30)))))/*float*/;
        }
      }
      for ((i__188 = 0)/*int*/; (i__188 < 32); (i__188++)) {{
          __pop__34();
        }
      }
      __push__34(sum__186);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

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

 
void init_AnonFilter_a0__195_42__35();
inline void check_status__35();

void work_AnonFilter_a0__195_42__35(int);


inline float __pop__35() {
float res=BUFFER_1_35[TAIL_1_35];
TAIL_1_35++;
return res;
}

inline float __pop__35(int n) {
float res=BUFFER_1_35[TAIL_1_35];
TAIL_1_35+=n;

return res;
}

inline float __peek__35(int offs) {
return BUFFER_1_35[TAIL_1_35+offs];
}



inline void __push__35(float data) {
BUFFER_35_3[HEAD_35_3]=data;
HEAD_35_3++;
}



 
void init_AnonFilter_a0__195_42__35(){
}
void save_file_pointer__35(object_write_buffer *buf) {}
void load_file_pointer__35(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__195_42__35__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__192 = 0.0f;/* float */
  int i__conflict__0__193 = 0;/* int */
  int i__194 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__192 = ((float)0.0))/*float*/;
  for ((i__conflict__0__193 = 0)/*int*/; (i__conflict__0__193 < 1); (i__conflict__0__193++)) {{
      (sum__192 = (sum__192 + ((*(____in+i__conflict__0__193)) * (*(____in+(i__conflict__0__193 + 31))))))/*float*/;
    }
  }
  for ((i__194 = 0)/*int*/; (i__194 < 32); (i__194++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__192);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__195_42__35__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__192 = 0.0f;/* float */
  int i__conflict__0__193 = 0;/* int */
  int i__194 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__192 = ((float)0.0))/*float*/;
  for ((i__conflict__0__193 = 0)/*int*/; (i__conflict__0__193 < 1); (i__conflict__0__193++)) {{
      (sum__192 = (sum__192 + ((*(____in+i__conflict__0__193)) * (*(____in+(i__conflict__0__193 + 31))))))/*float*/;
    }
  }
  for ((i__194 = 0)/*int*/; (i__194 < 32); (i__194++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__192);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__195_42__35(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__192 = 0.0f;/* float */
      int i__conflict__0__193 = 0;/* int */
      int i__194 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__192 = ((float)0.0))/*float*/;
      for ((i__conflict__0__193 = 0)/*int*/; (i__conflict__0__193 < 1); (i__conflict__0__193++)) {{
          (sum__192 = (sum__192 + (__peek__35(i__conflict__0__193) * __peek__35((i__conflict__0__193 + 31)))))/*float*/;
        }
      }
      for ((i__194 = 0)/*int*/; (i__194 < 32); (i__194++)) {{
          __pop__35();
        }
      }
      __push__35(sum__192);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_36;
int __counter_36 = 0;
int __steady_36 = 0;
int __tmp_36 = 0;
int __tmp2_36 = 0;
int *__state_flag_36 = NULL;
thread_info *__thread_36 = NULL;



void save_peek_buffer__36(object_write_buffer *buf);
void load_peek_buffer__36(object_write_buffer *buf);
void save_file_pointer__36(object_write_buffer *buf);
void load_file_pointer__36(object_write_buffer *buf);

 
void init_AnonFilter_a0__201_43__36();
inline void check_status__36();

void work_AnonFilter_a0__201_43__36(int);


inline float __pop__36() {
float res=BUFFER_1_36[TAIL_1_36];
TAIL_1_36++;
return res;
}

inline float __pop__36(int n) {
float res=BUFFER_1_36[TAIL_1_36];
TAIL_1_36+=n;

return res;
}

inline float __peek__36(int offs) {
return BUFFER_1_36[TAIL_1_36+offs];
}



inline void __push__36(float data) {
BUFFER_36_3[HEAD_36_3]=data;
HEAD_36_3++;
}



 
void init_AnonFilter_a0__201_43__36(){
}
void save_file_pointer__36(object_write_buffer *buf) {}
void load_file_pointer__36(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__201_43__36__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__198 = 0.0f;/* float */
  int i__conflict__0__199 = 0;/* int */
  int i__200 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__198 = ((float)0.0))/*float*/;
  for ((i__conflict__0__199 = 0)/*int*/; (i__conflict__0__199 < 0); (i__conflict__0__199++)) {{
      (sum__198 = (sum__198 + ((*(____in+i__conflict__0__199)) * (*(____in+(i__conflict__0__199 + 32))))))/*float*/;
    }
  }
  for ((i__200 = 0)/*int*/; (i__200 < 32); (i__200++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__198);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__201_43__36__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__198 = 0.0f;/* float */
  int i__conflict__0__199 = 0;/* int */
  int i__200 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__198 = ((float)0.0))/*float*/;
  for ((i__conflict__0__199 = 0)/*int*/; (i__conflict__0__199 < 0); (i__conflict__0__199++)) {{
      (sum__198 = (sum__198 + ((*(____in+i__conflict__0__199)) * (*(____in+(i__conflict__0__199 + 32))))))/*float*/;
    }
  }
  for ((i__200 = 0)/*int*/; (i__200 < 32); (i__200++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__198);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__201_43__36(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__198 = 0.0f;/* float */
      int i__conflict__0__199 = 0;/* int */
      int i__200 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__198 = ((float)0.0))/*float*/;
      for ((i__conflict__0__199 = 0)/*int*/; (i__conflict__0__199 < 0); (i__conflict__0__199++)) {{
          (sum__198 = (sum__198 + (__peek__36(i__conflict__0__199) * __peek__36((i__conflict__0__199 + 32)))))/*float*/;
        }
      }
      for ((i__200 = 0)/*int*/; (i__200 < 32); (i__200++)) {{
          __pop__36();
        }
      }
      __push__36(sum__198);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_37;
int __counter_37 = 0;
int __steady_37 = 0;
int __tmp_37 = 0;
int __tmp2_37 = 0;
int *__state_flag_37 = NULL;
thread_info *__thread_37 = NULL;



void save_peek_buffer__37(object_write_buffer *buf);
void load_peek_buffer__37(object_write_buffer *buf);
void save_file_pointer__37(object_write_buffer *buf);
void load_file_pointer__37(object_write_buffer *buf);

 
void init_AnonFilter_a0__207_44__37();
inline void check_status__37();

void work_AnonFilter_a0__207_44__37(int);


inline float __pop__37() {
float res=BUFFER_1_37[TAIL_1_37];
TAIL_1_37++;
return res;
}

inline float __pop__37(int n) {
float res=BUFFER_1_37[TAIL_1_37];
TAIL_1_37+=n;

return res;
}

inline float __peek__37(int offs) {
return BUFFER_1_37[TAIL_1_37+offs];
}



inline void __push__37(float data) {
BUFFER_37_3[HEAD_37_3]=data;
HEAD_37_3++;
}



 
void init_AnonFilter_a0__207_44__37(){
}
void save_file_pointer__37(object_write_buffer *buf) {}
void load_file_pointer__37(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__207_44__37__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__204 = 0.0f;/* float */
  int i__conflict__0__205 = 0;/* int */
  int i__206 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__204 = ((float)0.0))/*float*/;
  for ((i__conflict__0__205 = 0)/*int*/; (i__conflict__0__205 < -1); (i__conflict__0__205++)) {{
      (sum__204 = (sum__204 + ((*(____in+i__conflict__0__205)) * (*(____in+(i__conflict__0__205 + 33))))))/*float*/;
    }
  }
  for ((i__206 = 0)/*int*/; (i__206 < 32); (i__206++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__204);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__207_44__37__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__204 = 0.0f;/* float */
  int i__conflict__0__205 = 0;/* int */
  int i__206 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__204 = ((float)0.0))/*float*/;
  for ((i__conflict__0__205 = 0)/*int*/; (i__conflict__0__205 < -1); (i__conflict__0__205++)) {{
      (sum__204 = (sum__204 + ((*(____in+i__conflict__0__205)) * (*(____in+(i__conflict__0__205 + 33))))))/*float*/;
    }
  }
  for ((i__206 = 0)/*int*/; (i__206 < 32); (i__206++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__204);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__207_44__37(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__204 = 0.0f;/* float */
      int i__conflict__0__205 = 0;/* int */
      int i__206 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__204 = ((float)0.0))/*float*/;
      for ((i__conflict__0__205 = 0)/*int*/; (i__conflict__0__205 < -1); (i__conflict__0__205++)) {{
          (sum__204 = (sum__204 + (__peek__37(i__conflict__0__205) * __peek__37((i__conflict__0__205 + 33)))))/*float*/;
        }
      }
      for ((i__206 = 0)/*int*/; (i__206 < 32); (i__206++)) {{
          __pop__37();
        }
      }
      __push__37(sum__204);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_38;
int __counter_38 = 0;
int __steady_38 = 0;
int __tmp_38 = 0;
int __tmp2_38 = 0;
int *__state_flag_38 = NULL;
thread_info *__thread_38 = NULL;



void save_peek_buffer__38(object_write_buffer *buf);
void load_peek_buffer__38(object_write_buffer *buf);
void save_file_pointer__38(object_write_buffer *buf);
void load_file_pointer__38(object_write_buffer *buf);

 
void init_AnonFilter_a0__213_45__38();
inline void check_status__38();

void work_AnonFilter_a0__213_45__38(int);


inline float __pop__38() {
float res=BUFFER_1_38[TAIL_1_38];
TAIL_1_38++;
return res;
}

inline float __pop__38(int n) {
float res=BUFFER_1_38[TAIL_1_38];
TAIL_1_38+=n;

return res;
}

inline float __peek__38(int offs) {
return BUFFER_1_38[TAIL_1_38+offs];
}



inline void __push__38(float data) {
BUFFER_38_3[HEAD_38_3]=data;
HEAD_38_3++;
}



 
void init_AnonFilter_a0__213_45__38(){
}
void save_file_pointer__38(object_write_buffer *buf) {}
void load_file_pointer__38(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__213_45__38__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__210 = 0.0f;/* float */
  int i__conflict__0__211 = 0;/* int */
  int i__212 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__210 = ((float)0.0))/*float*/;
  for ((i__conflict__0__211 = 0)/*int*/; (i__conflict__0__211 < -2); (i__conflict__0__211++)) {{
      (sum__210 = (sum__210 + ((*(____in+i__conflict__0__211)) * (*(____in+(i__conflict__0__211 + 34))))))/*float*/;
    }
  }
  for ((i__212 = 0)/*int*/; (i__212 < 32); (i__212++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__210);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__213_45__38__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__210 = 0.0f;/* float */
  int i__conflict__0__211 = 0;/* int */
  int i__212 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__210 = ((float)0.0))/*float*/;
  for ((i__conflict__0__211 = 0)/*int*/; (i__conflict__0__211 < -2); (i__conflict__0__211++)) {{
      (sum__210 = (sum__210 + ((*(____in+i__conflict__0__211)) * (*(____in+(i__conflict__0__211 + 34))))))/*float*/;
    }
  }
  for ((i__212 = 0)/*int*/; (i__212 < 32); (i__212++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__210);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__213_45__38(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__210 = 0.0f;/* float */
      int i__conflict__0__211 = 0;/* int */
      int i__212 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__210 = ((float)0.0))/*float*/;
      for ((i__conflict__0__211 = 0)/*int*/; (i__conflict__0__211 < -2); (i__conflict__0__211++)) {{
          (sum__210 = (sum__210 + (__peek__38(i__conflict__0__211) * __peek__38((i__conflict__0__211 + 34)))))/*float*/;
        }
      }
      for ((i__212 = 0)/*int*/; (i__212 < 32); (i__212++)) {{
          __pop__38();
        }
      }
      __push__38(sum__210);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_39;
int __counter_39 = 0;
int __steady_39 = 0;
int __tmp_39 = 0;
int __tmp2_39 = 0;
int *__state_flag_39 = NULL;
thread_info *__thread_39 = NULL;



void save_peek_buffer__39(object_write_buffer *buf);
void load_peek_buffer__39(object_write_buffer *buf);
void save_file_pointer__39(object_write_buffer *buf);
void load_file_pointer__39(object_write_buffer *buf);

 
void init_AnonFilter_a0__219_46__39();
inline void check_status__39();

void work_AnonFilter_a0__219_46__39(int);


inline float __pop__39() {
float res=BUFFER_1_39[TAIL_1_39];
TAIL_1_39++;
return res;
}

inline float __pop__39(int n) {
float res=BUFFER_1_39[TAIL_1_39];
TAIL_1_39+=n;

return res;
}

inline float __peek__39(int offs) {
return BUFFER_1_39[TAIL_1_39+offs];
}



inline void __push__39(float data) {
BUFFER_39_3[HEAD_39_3]=data;
HEAD_39_3++;
}



 
void init_AnonFilter_a0__219_46__39(){
}
void save_file_pointer__39(object_write_buffer *buf) {}
void load_file_pointer__39(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__219_46__39__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__216 = 0.0f;/* float */
  int i__conflict__0__217 = 0;/* int */
  int i__218 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__216 = ((float)0.0))/*float*/;
  for ((i__conflict__0__217 = 0)/*int*/; (i__conflict__0__217 < -3); (i__conflict__0__217++)) {{
      (sum__216 = (sum__216 + ((*(____in+i__conflict__0__217)) * (*(____in+(i__conflict__0__217 + 35))))))/*float*/;
    }
  }
  for ((i__218 = 0)/*int*/; (i__218 < 32); (i__218++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__216);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__219_46__39__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__216 = 0.0f;/* float */
  int i__conflict__0__217 = 0;/* int */
  int i__218 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__216 = ((float)0.0))/*float*/;
  for ((i__conflict__0__217 = 0)/*int*/; (i__conflict__0__217 < -3); (i__conflict__0__217++)) {{
      (sum__216 = (sum__216 + ((*(____in+i__conflict__0__217)) * (*(____in+(i__conflict__0__217 + 35))))))/*float*/;
    }
  }
  for ((i__218 = 0)/*int*/; (i__218 < 32); (i__218++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__216);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__219_46__39(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__216 = 0.0f;/* float */
      int i__conflict__0__217 = 0;/* int */
      int i__218 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__216 = ((float)0.0))/*float*/;
      for ((i__conflict__0__217 = 0)/*int*/; (i__conflict__0__217 < -3); (i__conflict__0__217++)) {{
          (sum__216 = (sum__216 + (__peek__39(i__conflict__0__217) * __peek__39((i__conflict__0__217 + 35)))))/*float*/;
        }
      }
      for ((i__218 = 0)/*int*/; (i__218 < 32); (i__218++)) {{
          __pop__39();
        }
      }
      __push__39(sum__216);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_40;
int __counter_40 = 0;
int __steady_40 = 0;
int __tmp_40 = 0;
int __tmp2_40 = 0;
int *__state_flag_40 = NULL;
thread_info *__thread_40 = NULL;



void save_peek_buffer__40(object_write_buffer *buf);
void load_peek_buffer__40(object_write_buffer *buf);
void save_file_pointer__40(object_write_buffer *buf);
void load_file_pointer__40(object_write_buffer *buf);

 
void init_AnonFilter_a0__225_47__40();
inline void check_status__40();

void work_AnonFilter_a0__225_47__40(int);


inline float __pop__40() {
float res=BUFFER_1_40[TAIL_1_40];
TAIL_1_40++;
return res;
}

inline float __pop__40(int n) {
float res=BUFFER_1_40[TAIL_1_40];
TAIL_1_40+=n;

return res;
}

inline float __peek__40(int offs) {
return BUFFER_1_40[TAIL_1_40+offs];
}



inline void __push__40(float data) {
BUFFER_40_3[HEAD_40_3]=data;
HEAD_40_3++;
}



 
void init_AnonFilter_a0__225_47__40(){
}
void save_file_pointer__40(object_write_buffer *buf) {}
void load_file_pointer__40(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__225_47__40__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__222 = 0.0f;/* float */
  int i__conflict__0__223 = 0;/* int */
  int i__224 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__222 = ((float)0.0))/*float*/;
  for ((i__conflict__0__223 = 0)/*int*/; (i__conflict__0__223 < -4); (i__conflict__0__223++)) {{
      (sum__222 = (sum__222 + ((*(____in+i__conflict__0__223)) * (*(____in+(i__conflict__0__223 + 36))))))/*float*/;
    }
  }
  for ((i__224 = 0)/*int*/; (i__224 < 32); (i__224++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__222);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__225_47__40__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__222 = 0.0f;/* float */
  int i__conflict__0__223 = 0;/* int */
  int i__224 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__222 = ((float)0.0))/*float*/;
  for ((i__conflict__0__223 = 0)/*int*/; (i__conflict__0__223 < -4); (i__conflict__0__223++)) {{
      (sum__222 = (sum__222 + ((*(____in+i__conflict__0__223)) * (*(____in+(i__conflict__0__223 + 36))))))/*float*/;
    }
  }
  for ((i__224 = 0)/*int*/; (i__224 < 32); (i__224++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__222);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__225_47__40(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__222 = 0.0f;/* float */
      int i__conflict__0__223 = 0;/* int */
      int i__224 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__222 = ((float)0.0))/*float*/;
      for ((i__conflict__0__223 = 0)/*int*/; (i__conflict__0__223 < -4); (i__conflict__0__223++)) {{
          (sum__222 = (sum__222 + (__peek__40(i__conflict__0__223) * __peek__40((i__conflict__0__223 + 36)))))/*float*/;
        }
      }
      for ((i__224 = 0)/*int*/; (i__224 < 32); (i__224++)) {{
          __pop__40();
        }
      }
      __push__40(sum__222);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_41;
int __counter_41 = 0;
int __steady_41 = 0;
int __tmp_41 = 0;
int __tmp2_41 = 0;
int *__state_flag_41 = NULL;
thread_info *__thread_41 = NULL;



void save_peek_buffer__41(object_write_buffer *buf);
void load_peek_buffer__41(object_write_buffer *buf);
void save_file_pointer__41(object_write_buffer *buf);
void load_file_pointer__41(object_write_buffer *buf);

 
void init_AnonFilter_a0__231_48__41();
inline void check_status__41();

void work_AnonFilter_a0__231_48__41(int);


inline float __pop__41() {
float res=BUFFER_1_41[TAIL_1_41];
TAIL_1_41++;
return res;
}

inline float __pop__41(int n) {
float res=BUFFER_1_41[TAIL_1_41];
TAIL_1_41+=n;

return res;
}

inline float __peek__41(int offs) {
return BUFFER_1_41[TAIL_1_41+offs];
}



inline void __push__41(float data) {
BUFFER_41_3[HEAD_41_3]=data;
HEAD_41_3++;
}



 
void init_AnonFilter_a0__231_48__41(){
}
void save_file_pointer__41(object_write_buffer *buf) {}
void load_file_pointer__41(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__231_48__41__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__228 = 0.0f;/* float */
  int i__conflict__0__229 = 0;/* int */
  int i__230 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__228 = ((float)0.0))/*float*/;
  for ((i__conflict__0__229 = 0)/*int*/; (i__conflict__0__229 < -5); (i__conflict__0__229++)) {{
      (sum__228 = (sum__228 + ((*(____in+i__conflict__0__229)) * (*(____in+(i__conflict__0__229 + 37))))))/*float*/;
    }
  }
  for ((i__230 = 0)/*int*/; (i__230 < 32); (i__230++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__228);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__231_48__41__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__228 = 0.0f;/* float */
  int i__conflict__0__229 = 0;/* int */
  int i__230 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__228 = ((float)0.0))/*float*/;
  for ((i__conflict__0__229 = 0)/*int*/; (i__conflict__0__229 < -5); (i__conflict__0__229++)) {{
      (sum__228 = (sum__228 + ((*(____in+i__conflict__0__229)) * (*(____in+(i__conflict__0__229 + 37))))))/*float*/;
    }
  }
  for ((i__230 = 0)/*int*/; (i__230 < 32); (i__230++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__228);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__231_48__41(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__228 = 0.0f;/* float */
      int i__conflict__0__229 = 0;/* int */
      int i__230 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__228 = ((float)0.0))/*float*/;
      for ((i__conflict__0__229 = 0)/*int*/; (i__conflict__0__229 < -5); (i__conflict__0__229++)) {{
          (sum__228 = (sum__228 + (__peek__41(i__conflict__0__229) * __peek__41((i__conflict__0__229 + 37)))))/*float*/;
        }
      }
      for ((i__230 = 0)/*int*/; (i__230 < 32); (i__230++)) {{
          __pop__41();
        }
      }
      __push__41(sum__228);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_42;
int __counter_42 = 0;
int __steady_42 = 0;
int __tmp_42 = 0;
int __tmp2_42 = 0;
int *__state_flag_42 = NULL;
thread_info *__thread_42 = NULL;



void save_peek_buffer__42(object_write_buffer *buf);
void load_peek_buffer__42(object_write_buffer *buf);
void save_file_pointer__42(object_write_buffer *buf);
void load_file_pointer__42(object_write_buffer *buf);

 
void init_AnonFilter_a0__237_49__42();
inline void check_status__42();

void work_AnonFilter_a0__237_49__42(int);


inline float __pop__42() {
float res=BUFFER_1_42[TAIL_1_42];
TAIL_1_42++;
return res;
}

inline float __pop__42(int n) {
float res=BUFFER_1_42[TAIL_1_42];
TAIL_1_42+=n;

return res;
}

inline float __peek__42(int offs) {
return BUFFER_1_42[TAIL_1_42+offs];
}



inline void __push__42(float data) {
BUFFER_42_3[HEAD_42_3]=data;
HEAD_42_3++;
}



 
void init_AnonFilter_a0__237_49__42(){
}
void save_file_pointer__42(object_write_buffer *buf) {}
void load_file_pointer__42(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__237_49__42__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__234 = 0.0f;/* float */
  int i__conflict__0__235 = 0;/* int */
  int i__236 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__234 = ((float)0.0))/*float*/;
  for ((i__conflict__0__235 = 0)/*int*/; (i__conflict__0__235 < -6); (i__conflict__0__235++)) {{
      (sum__234 = (sum__234 + ((*(____in+i__conflict__0__235)) * (*(____in+(i__conflict__0__235 + 38))))))/*float*/;
    }
  }
  for ((i__236 = 0)/*int*/; (i__236 < 32); (i__236++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__234);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__237_49__42__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__234 = 0.0f;/* float */
  int i__conflict__0__235 = 0;/* int */
  int i__236 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__234 = ((float)0.0))/*float*/;
  for ((i__conflict__0__235 = 0)/*int*/; (i__conflict__0__235 < -6); (i__conflict__0__235++)) {{
      (sum__234 = (sum__234 + ((*(____in+i__conflict__0__235)) * (*(____in+(i__conflict__0__235 + 38))))))/*float*/;
    }
  }
  for ((i__236 = 0)/*int*/; (i__236 < 32); (i__236++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__234);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__237_49__42(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__234 = 0.0f;/* float */
      int i__conflict__0__235 = 0;/* int */
      int i__236 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__234 = ((float)0.0))/*float*/;
      for ((i__conflict__0__235 = 0)/*int*/; (i__conflict__0__235 < -6); (i__conflict__0__235++)) {{
          (sum__234 = (sum__234 + (__peek__42(i__conflict__0__235) * __peek__42((i__conflict__0__235 + 38)))))/*float*/;
        }
      }
      for ((i__236 = 0)/*int*/; (i__236 < 32); (i__236++)) {{
          __pop__42();
        }
      }
      __push__42(sum__234);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_43;
int __counter_43 = 0;
int __steady_43 = 0;
int __tmp_43 = 0;
int __tmp2_43 = 0;
int *__state_flag_43 = NULL;
thread_info *__thread_43 = NULL;



void save_peek_buffer__43(object_write_buffer *buf);
void load_peek_buffer__43(object_write_buffer *buf);
void save_file_pointer__43(object_write_buffer *buf);
void load_file_pointer__43(object_write_buffer *buf);

 
void init_AnonFilter_a0__243_50__43();
inline void check_status__43();

void work_AnonFilter_a0__243_50__43(int);


inline float __pop__43() {
float res=BUFFER_1_43[TAIL_1_43];
TAIL_1_43++;
return res;
}

inline float __pop__43(int n) {
float res=BUFFER_1_43[TAIL_1_43];
TAIL_1_43+=n;

return res;
}

inline float __peek__43(int offs) {
return BUFFER_1_43[TAIL_1_43+offs];
}



inline void __push__43(float data) {
BUFFER_43_3[HEAD_43_3]=data;
HEAD_43_3++;
}



 
void init_AnonFilter_a0__243_50__43(){
}
void save_file_pointer__43(object_write_buffer *buf) {}
void load_file_pointer__43(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__243_50__43__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__240 = 0.0f;/* float */
  int i__conflict__0__241 = 0;/* int */
  int i__242 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__240 = ((float)0.0))/*float*/;
  for ((i__conflict__0__241 = 0)/*int*/; (i__conflict__0__241 < -7); (i__conflict__0__241++)) {{
      (sum__240 = (sum__240 + ((*(____in+i__conflict__0__241)) * (*(____in+(i__conflict__0__241 + 39))))))/*float*/;
    }
  }
  for ((i__242 = 0)/*int*/; (i__242 < 32); (i__242++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__240);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__243_50__43__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__240 = 0.0f;/* float */
  int i__conflict__0__241 = 0;/* int */
  int i__242 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__240 = ((float)0.0))/*float*/;
  for ((i__conflict__0__241 = 0)/*int*/; (i__conflict__0__241 < -7); (i__conflict__0__241++)) {{
      (sum__240 = (sum__240 + ((*(____in+i__conflict__0__241)) * (*(____in+(i__conflict__0__241 + 39))))))/*float*/;
    }
  }
  for ((i__242 = 0)/*int*/; (i__242 < 32); (i__242++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__240);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__243_50__43(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__240 = 0.0f;/* float */
      int i__conflict__0__241 = 0;/* int */
      int i__242 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__240 = ((float)0.0))/*float*/;
      for ((i__conflict__0__241 = 0)/*int*/; (i__conflict__0__241 < -7); (i__conflict__0__241++)) {{
          (sum__240 = (sum__240 + (__peek__43(i__conflict__0__241) * __peek__43((i__conflict__0__241 + 39)))))/*float*/;
        }
      }
      for ((i__242 = 0)/*int*/; (i__242 < 32); (i__242++)) {{
          __pop__43();
        }
      }
      __push__43(sum__240);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_44;
int __counter_44 = 0;
int __steady_44 = 0;
int __tmp_44 = 0;
int __tmp2_44 = 0;
int *__state_flag_44 = NULL;
thread_info *__thread_44 = NULL;



void save_peek_buffer__44(object_write_buffer *buf);
void load_peek_buffer__44(object_write_buffer *buf);
void save_file_pointer__44(object_write_buffer *buf);
void load_file_pointer__44(object_write_buffer *buf);

 
void init_AnonFilter_a0__249_51__44();
inline void check_status__44();

void work_AnonFilter_a0__249_51__44(int);


inline float __pop__44() {
float res=BUFFER_1_44[TAIL_1_44];
TAIL_1_44++;
return res;
}

inline float __pop__44(int n) {
float res=BUFFER_1_44[TAIL_1_44];
TAIL_1_44+=n;

return res;
}

inline float __peek__44(int offs) {
return BUFFER_1_44[TAIL_1_44+offs];
}



inline void __push__44(float data) {
BUFFER_44_3[HEAD_44_3]=data;
HEAD_44_3++;
}



 
void init_AnonFilter_a0__249_51__44(){
}
void save_file_pointer__44(object_write_buffer *buf) {}
void load_file_pointer__44(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__249_51__44__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__246 = 0.0f;/* float */
  int i__conflict__0__247 = 0;/* int */
  int i__248 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__246 = ((float)0.0))/*float*/;
  for ((i__conflict__0__247 = 0)/*int*/; (i__conflict__0__247 < -8); (i__conflict__0__247++)) {{
      (sum__246 = (sum__246 + ((*(____in+i__conflict__0__247)) * (*(____in+(i__conflict__0__247 + 40))))))/*float*/;
    }
  }
  for ((i__248 = 0)/*int*/; (i__248 < 32); (i__248++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__246);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__249_51__44__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__246 = 0.0f;/* float */
  int i__conflict__0__247 = 0;/* int */
  int i__248 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__246 = ((float)0.0))/*float*/;
  for ((i__conflict__0__247 = 0)/*int*/; (i__conflict__0__247 < -8); (i__conflict__0__247++)) {{
      (sum__246 = (sum__246 + ((*(____in+i__conflict__0__247)) * (*(____in+(i__conflict__0__247 + 40))))))/*float*/;
    }
  }
  for ((i__248 = 0)/*int*/; (i__248 < 32); (i__248++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__246);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__249_51__44(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__246 = 0.0f;/* float */
      int i__conflict__0__247 = 0;/* int */
      int i__248 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__246 = ((float)0.0))/*float*/;
      for ((i__conflict__0__247 = 0)/*int*/; (i__conflict__0__247 < -8); (i__conflict__0__247++)) {{
          (sum__246 = (sum__246 + (__peek__44(i__conflict__0__247) * __peek__44((i__conflict__0__247 + 40)))))/*float*/;
        }
      }
      for ((i__248 = 0)/*int*/; (i__248 < 32); (i__248++)) {{
          __pop__44();
        }
      }
      __push__44(sum__246);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_45;
int __counter_45 = 0;
int __steady_45 = 0;
int __tmp_45 = 0;
int __tmp2_45 = 0;
int *__state_flag_45 = NULL;
thread_info *__thread_45 = NULL;



void save_peek_buffer__45(object_write_buffer *buf);
void load_peek_buffer__45(object_write_buffer *buf);
void save_file_pointer__45(object_write_buffer *buf);
void load_file_pointer__45(object_write_buffer *buf);

 
void init_AnonFilter_a0__255_52__45();
inline void check_status__45();

void work_AnonFilter_a0__255_52__45(int);


inline float __pop__45() {
float res=BUFFER_1_45[TAIL_1_45];
TAIL_1_45++;
return res;
}

inline float __pop__45(int n) {
float res=BUFFER_1_45[TAIL_1_45];
TAIL_1_45+=n;

return res;
}

inline float __peek__45(int offs) {
return BUFFER_1_45[TAIL_1_45+offs];
}



inline void __push__45(float data) {
BUFFER_45_3[HEAD_45_3]=data;
HEAD_45_3++;
}



 
void init_AnonFilter_a0__255_52__45(){
}
void save_file_pointer__45(object_write_buffer *buf) {}
void load_file_pointer__45(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__255_52__45__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__252 = 0.0f;/* float */
  int i__conflict__0__253 = 0;/* int */
  int i__254 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__252 = ((float)0.0))/*float*/;
  for ((i__conflict__0__253 = 0)/*int*/; (i__conflict__0__253 < -9); (i__conflict__0__253++)) {{
      (sum__252 = (sum__252 + ((*(____in+i__conflict__0__253)) * (*(____in+(i__conflict__0__253 + 41))))))/*float*/;
    }
  }
  for ((i__254 = 0)/*int*/; (i__254 < 32); (i__254++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__252);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__255_52__45__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__252 = 0.0f;/* float */
  int i__conflict__0__253 = 0;/* int */
  int i__254 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__252 = ((float)0.0))/*float*/;
  for ((i__conflict__0__253 = 0)/*int*/; (i__conflict__0__253 < -9); (i__conflict__0__253++)) {{
      (sum__252 = (sum__252 + ((*(____in+i__conflict__0__253)) * (*(____in+(i__conflict__0__253 + 41))))))/*float*/;
    }
  }
  for ((i__254 = 0)/*int*/; (i__254 < 32); (i__254++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__252);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__255_52__45(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__252 = 0.0f;/* float */
      int i__conflict__0__253 = 0;/* int */
      int i__254 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__252 = ((float)0.0))/*float*/;
      for ((i__conflict__0__253 = 0)/*int*/; (i__conflict__0__253 < -9); (i__conflict__0__253++)) {{
          (sum__252 = (sum__252 + (__peek__45(i__conflict__0__253) * __peek__45((i__conflict__0__253 + 41)))))/*float*/;
        }
      }
      for ((i__254 = 0)/*int*/; (i__254 < 32); (i__254++)) {{
          __pop__45();
        }
      }
      __push__45(sum__252);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_46;
int __counter_46 = 0;
int __steady_46 = 0;
int __tmp_46 = 0;
int __tmp2_46 = 0;
int *__state_flag_46 = NULL;
thread_info *__thread_46 = NULL;



void save_peek_buffer__46(object_write_buffer *buf);
void load_peek_buffer__46(object_write_buffer *buf);
void save_file_pointer__46(object_write_buffer *buf);
void load_file_pointer__46(object_write_buffer *buf);

 
void init_AnonFilter_a0__261_53__46();
inline void check_status__46();

void work_AnonFilter_a0__261_53__46(int);


inline float __pop__46() {
float res=BUFFER_1_46[TAIL_1_46];
TAIL_1_46++;
return res;
}

inline float __pop__46(int n) {
float res=BUFFER_1_46[TAIL_1_46];
TAIL_1_46+=n;

return res;
}

inline float __peek__46(int offs) {
return BUFFER_1_46[TAIL_1_46+offs];
}



inline void __push__46(float data) {
BUFFER_46_3[HEAD_46_3]=data;
HEAD_46_3++;
}



 
void init_AnonFilter_a0__261_53__46(){
}
void save_file_pointer__46(object_write_buffer *buf) {}
void load_file_pointer__46(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__261_53__46__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__258 = 0.0f;/* float */
  int i__conflict__0__259 = 0;/* int */
  int i__260 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__258 = ((float)0.0))/*float*/;
  for ((i__conflict__0__259 = 0)/*int*/; (i__conflict__0__259 < -10); (i__conflict__0__259++)) {{
      (sum__258 = (sum__258 + ((*(____in+i__conflict__0__259)) * (*(____in+(i__conflict__0__259 + 42))))))/*float*/;
    }
  }
  for ((i__260 = 0)/*int*/; (i__260 < 32); (i__260++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__258);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__261_53__46__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__258 = 0.0f;/* float */
  int i__conflict__0__259 = 0;/* int */
  int i__260 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__258 = ((float)0.0))/*float*/;
  for ((i__conflict__0__259 = 0)/*int*/; (i__conflict__0__259 < -10); (i__conflict__0__259++)) {{
      (sum__258 = (sum__258 + ((*(____in+i__conflict__0__259)) * (*(____in+(i__conflict__0__259 + 42))))))/*float*/;
    }
  }
  for ((i__260 = 0)/*int*/; (i__260 < 32); (i__260++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__258);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__261_53__46(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__258 = 0.0f;/* float */
      int i__conflict__0__259 = 0;/* int */
      int i__260 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__258 = ((float)0.0))/*float*/;
      for ((i__conflict__0__259 = 0)/*int*/; (i__conflict__0__259 < -10); (i__conflict__0__259++)) {{
          (sum__258 = (sum__258 + (__peek__46(i__conflict__0__259) * __peek__46((i__conflict__0__259 + 42)))))/*float*/;
        }
      }
      for ((i__260 = 0)/*int*/; (i__260 < 32); (i__260++)) {{
          __pop__46();
        }
      }
      __push__46(sum__258);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_47;
int __counter_47 = 0;
int __steady_47 = 0;
int __tmp_47 = 0;
int __tmp2_47 = 0;
int *__state_flag_47 = NULL;
thread_info *__thread_47 = NULL;



void save_peek_buffer__47(object_write_buffer *buf);
void load_peek_buffer__47(object_write_buffer *buf);
void save_file_pointer__47(object_write_buffer *buf);
void load_file_pointer__47(object_write_buffer *buf);

 
void init_AnonFilter_a0__267_54__47();
inline void check_status__47();

void work_AnonFilter_a0__267_54__47(int);


inline float __pop__47() {
float res=BUFFER_1_47[TAIL_1_47];
TAIL_1_47++;
return res;
}

inline float __pop__47(int n) {
float res=BUFFER_1_47[TAIL_1_47];
TAIL_1_47+=n;

return res;
}

inline float __peek__47(int offs) {
return BUFFER_1_47[TAIL_1_47+offs];
}



inline void __push__47(float data) {
BUFFER_47_3[HEAD_47_3]=data;
HEAD_47_3++;
}



 
void init_AnonFilter_a0__267_54__47(){
}
void save_file_pointer__47(object_write_buffer *buf) {}
void load_file_pointer__47(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__267_54__47__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__264 = 0.0f;/* float */
  int i__conflict__0__265 = 0;/* int */
  int i__266 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__264 = ((float)0.0))/*float*/;
  for ((i__conflict__0__265 = 0)/*int*/; (i__conflict__0__265 < -11); (i__conflict__0__265++)) {{
      (sum__264 = (sum__264 + ((*(____in+i__conflict__0__265)) * (*(____in+(i__conflict__0__265 + 43))))))/*float*/;
    }
  }
  for ((i__266 = 0)/*int*/; (i__266 < 32); (i__266++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__264);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__267_54__47__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__264 = 0.0f;/* float */
  int i__conflict__0__265 = 0;/* int */
  int i__266 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__264 = ((float)0.0))/*float*/;
  for ((i__conflict__0__265 = 0)/*int*/; (i__conflict__0__265 < -11); (i__conflict__0__265++)) {{
      (sum__264 = (sum__264 + ((*(____in+i__conflict__0__265)) * (*(____in+(i__conflict__0__265 + 43))))))/*float*/;
    }
  }
  for ((i__266 = 0)/*int*/; (i__266 < 32); (i__266++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__264);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__267_54__47(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__264 = 0.0f;/* float */
      int i__conflict__0__265 = 0;/* int */
      int i__266 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__264 = ((float)0.0))/*float*/;
      for ((i__conflict__0__265 = 0)/*int*/; (i__conflict__0__265 < -11); (i__conflict__0__265++)) {{
          (sum__264 = (sum__264 + (__peek__47(i__conflict__0__265) * __peek__47((i__conflict__0__265 + 43)))))/*float*/;
        }
      }
      for ((i__266 = 0)/*int*/; (i__266 < 32); (i__266++)) {{
          __pop__47();
        }
      }
      __push__47(sum__264);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_48;
int __counter_48 = 0;
int __steady_48 = 0;
int __tmp_48 = 0;
int __tmp2_48 = 0;
int *__state_flag_48 = NULL;
thread_info *__thread_48 = NULL;



void save_peek_buffer__48(object_write_buffer *buf);
void load_peek_buffer__48(object_write_buffer *buf);
void save_file_pointer__48(object_write_buffer *buf);
void load_file_pointer__48(object_write_buffer *buf);

 
void init_AnonFilter_a0__273_55__48();
inline void check_status__48();

void work_AnonFilter_a0__273_55__48(int);


inline float __pop__48() {
float res=BUFFER_1_48[TAIL_1_48];
TAIL_1_48++;
return res;
}

inline float __pop__48(int n) {
float res=BUFFER_1_48[TAIL_1_48];
TAIL_1_48+=n;

return res;
}

inline float __peek__48(int offs) {
return BUFFER_1_48[TAIL_1_48+offs];
}



inline void __push__48(float data) {
BUFFER_48_3[HEAD_48_3]=data;
HEAD_48_3++;
}



 
void init_AnonFilter_a0__273_55__48(){
}
void save_file_pointer__48(object_write_buffer *buf) {}
void load_file_pointer__48(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__273_55__48__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__270 = 0.0f;/* float */
  int i__conflict__0__271 = 0;/* int */
  int i__272 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__270 = ((float)0.0))/*float*/;
  for ((i__conflict__0__271 = 0)/*int*/; (i__conflict__0__271 < -12); (i__conflict__0__271++)) {{
      (sum__270 = (sum__270 + ((*(____in+i__conflict__0__271)) * (*(____in+(i__conflict__0__271 + 44))))))/*float*/;
    }
  }
  for ((i__272 = 0)/*int*/; (i__272 < 32); (i__272++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__270);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__273_55__48__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__270 = 0.0f;/* float */
  int i__conflict__0__271 = 0;/* int */
  int i__272 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__270 = ((float)0.0))/*float*/;
  for ((i__conflict__0__271 = 0)/*int*/; (i__conflict__0__271 < -12); (i__conflict__0__271++)) {{
      (sum__270 = (sum__270 + ((*(____in+i__conflict__0__271)) * (*(____in+(i__conflict__0__271 + 44))))))/*float*/;
    }
  }
  for ((i__272 = 0)/*int*/; (i__272 < 32); (i__272++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__270);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__273_55__48(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__270 = 0.0f;/* float */
      int i__conflict__0__271 = 0;/* int */
      int i__272 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__270 = ((float)0.0))/*float*/;
      for ((i__conflict__0__271 = 0)/*int*/; (i__conflict__0__271 < -12); (i__conflict__0__271++)) {{
          (sum__270 = (sum__270 + (__peek__48(i__conflict__0__271) * __peek__48((i__conflict__0__271 + 44)))))/*float*/;
        }
      }
      for ((i__272 = 0)/*int*/; (i__272 < 32); (i__272++)) {{
          __pop__48();
        }
      }
      __push__48(sum__270);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_49;
int __counter_49 = 0;
int __steady_49 = 0;
int __tmp_49 = 0;
int __tmp2_49 = 0;
int *__state_flag_49 = NULL;
thread_info *__thread_49 = NULL;



void save_peek_buffer__49(object_write_buffer *buf);
void load_peek_buffer__49(object_write_buffer *buf);
void save_file_pointer__49(object_write_buffer *buf);
void load_file_pointer__49(object_write_buffer *buf);

 
void init_AnonFilter_a0__279_56__49();
inline void check_status__49();

void work_AnonFilter_a0__279_56__49(int);


inline float __pop__49() {
float res=BUFFER_1_49[TAIL_1_49];
TAIL_1_49++;
return res;
}

inline float __pop__49(int n) {
float res=BUFFER_1_49[TAIL_1_49];
TAIL_1_49+=n;

return res;
}

inline float __peek__49(int offs) {
return BUFFER_1_49[TAIL_1_49+offs];
}



inline void __push__49(float data) {
BUFFER_49_3[HEAD_49_3]=data;
HEAD_49_3++;
}



 
void init_AnonFilter_a0__279_56__49(){
}
void save_file_pointer__49(object_write_buffer *buf) {}
void load_file_pointer__49(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__279_56__49__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__276 = 0.0f;/* float */
  int i__conflict__0__277 = 0;/* int */
  int i__278 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__276 = ((float)0.0))/*float*/;
  for ((i__conflict__0__277 = 0)/*int*/; (i__conflict__0__277 < -13); (i__conflict__0__277++)) {{
      (sum__276 = (sum__276 + ((*(____in+i__conflict__0__277)) * (*(____in+(i__conflict__0__277 + 45))))))/*float*/;
    }
  }
  for ((i__278 = 0)/*int*/; (i__278 < 32); (i__278++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__276);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__279_56__49__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__276 = 0.0f;/* float */
  int i__conflict__0__277 = 0;/* int */
  int i__278 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__276 = ((float)0.0))/*float*/;
  for ((i__conflict__0__277 = 0)/*int*/; (i__conflict__0__277 < -13); (i__conflict__0__277++)) {{
      (sum__276 = (sum__276 + ((*(____in+i__conflict__0__277)) * (*(____in+(i__conflict__0__277 + 45))))))/*float*/;
    }
  }
  for ((i__278 = 0)/*int*/; (i__278 < 32); (i__278++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__276);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__279_56__49(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__276 = 0.0f;/* float */
      int i__conflict__0__277 = 0;/* int */
      int i__278 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__276 = ((float)0.0))/*float*/;
      for ((i__conflict__0__277 = 0)/*int*/; (i__conflict__0__277 < -13); (i__conflict__0__277++)) {{
          (sum__276 = (sum__276 + (__peek__49(i__conflict__0__277) * __peek__49((i__conflict__0__277 + 45)))))/*float*/;
        }
      }
      for ((i__278 = 0)/*int*/; (i__278 < 32); (i__278++)) {{
          __pop__49();
        }
      }
      __push__49(sum__276);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_50;
int __counter_50 = 0;
int __steady_50 = 0;
int __tmp_50 = 0;
int __tmp2_50 = 0;
int *__state_flag_50 = NULL;
thread_info *__thread_50 = NULL;



void save_peek_buffer__50(object_write_buffer *buf);
void load_peek_buffer__50(object_write_buffer *buf);
void save_file_pointer__50(object_write_buffer *buf);
void load_file_pointer__50(object_write_buffer *buf);

 
void init_AnonFilter_a0__285_57__50();
inline void check_status__50();

void work_AnonFilter_a0__285_57__50(int);


inline float __pop__50() {
float res=BUFFER_1_50[TAIL_1_50];
TAIL_1_50++;
return res;
}

inline float __pop__50(int n) {
float res=BUFFER_1_50[TAIL_1_50];
TAIL_1_50+=n;

return res;
}

inline float __peek__50(int offs) {
return BUFFER_1_50[TAIL_1_50+offs];
}



inline void __push__50(float data) {
BUFFER_50_3[HEAD_50_3]=data;
HEAD_50_3++;
}



 
void init_AnonFilter_a0__285_57__50(){
}
void save_file_pointer__50(object_write_buffer *buf) {}
void load_file_pointer__50(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__285_57__50__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__282 = 0.0f;/* float */
  int i__conflict__0__283 = 0;/* int */
  int i__284 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__282 = ((float)0.0))/*float*/;
  for ((i__conflict__0__283 = 0)/*int*/; (i__conflict__0__283 < -14); (i__conflict__0__283++)) {{
      (sum__282 = (sum__282 + ((*(____in+i__conflict__0__283)) * (*(____in+(i__conflict__0__283 + 46))))))/*float*/;
    }
  }
  for ((i__284 = 0)/*int*/; (i__284 < 32); (i__284++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__282);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__285_57__50__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__282 = 0.0f;/* float */
  int i__conflict__0__283 = 0;/* int */
  int i__284 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__282 = ((float)0.0))/*float*/;
  for ((i__conflict__0__283 = 0)/*int*/; (i__conflict__0__283 < -14); (i__conflict__0__283++)) {{
      (sum__282 = (sum__282 + ((*(____in+i__conflict__0__283)) * (*(____in+(i__conflict__0__283 + 46))))))/*float*/;
    }
  }
  for ((i__284 = 0)/*int*/; (i__284 < 32); (i__284++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__282);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__285_57__50(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__282 = 0.0f;/* float */
      int i__conflict__0__283 = 0;/* int */
      int i__284 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__282 = ((float)0.0))/*float*/;
      for ((i__conflict__0__283 = 0)/*int*/; (i__conflict__0__283 < -14); (i__conflict__0__283++)) {{
          (sum__282 = (sum__282 + (__peek__50(i__conflict__0__283) * __peek__50((i__conflict__0__283 + 46)))))/*float*/;
        }
      }
      for ((i__284 = 0)/*int*/; (i__284 < 32); (i__284++)) {{
          __pop__50();
        }
      }
      __push__50(sum__282);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_51;
int __counter_51 = 0;
int __steady_51 = 0;
int __tmp_51 = 0;
int __tmp2_51 = 0;
int *__state_flag_51 = NULL;
thread_info *__thread_51 = NULL;



void save_peek_buffer__51(object_write_buffer *buf);
void load_peek_buffer__51(object_write_buffer *buf);
void save_file_pointer__51(object_write_buffer *buf);
void load_file_pointer__51(object_write_buffer *buf);

 
void init_AnonFilter_a0__291_58__51();
inline void check_status__51();

void work_AnonFilter_a0__291_58__51(int);


inline float __pop__51() {
float res=BUFFER_1_51[TAIL_1_51];
TAIL_1_51++;
return res;
}

inline float __pop__51(int n) {
float res=BUFFER_1_51[TAIL_1_51];
TAIL_1_51+=n;

return res;
}

inline float __peek__51(int offs) {
return BUFFER_1_51[TAIL_1_51+offs];
}



inline void __push__51(float data) {
BUFFER_51_3[HEAD_51_3]=data;
HEAD_51_3++;
}



 
void init_AnonFilter_a0__291_58__51(){
}
void save_file_pointer__51(object_write_buffer *buf) {}
void load_file_pointer__51(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__291_58__51__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__288 = 0.0f;/* float */
  int i__conflict__0__289 = 0;/* int */
  int i__290 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__288 = ((float)0.0))/*float*/;
  for ((i__conflict__0__289 = 0)/*int*/; (i__conflict__0__289 < -15); (i__conflict__0__289++)) {{
      (sum__288 = (sum__288 + ((*(____in+i__conflict__0__289)) * (*(____in+(i__conflict__0__289 + 47))))))/*float*/;
    }
  }
  for ((i__290 = 0)/*int*/; (i__290 < 32); (i__290++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__288);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__291_58__51__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__288 = 0.0f;/* float */
  int i__conflict__0__289 = 0;/* int */
  int i__290 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__288 = ((float)0.0))/*float*/;
  for ((i__conflict__0__289 = 0)/*int*/; (i__conflict__0__289 < -15); (i__conflict__0__289++)) {{
      (sum__288 = (sum__288 + ((*(____in+i__conflict__0__289)) * (*(____in+(i__conflict__0__289 + 47))))))/*float*/;
    }
  }
  for ((i__290 = 0)/*int*/; (i__290 < 32); (i__290++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__288);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__291_58__51(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__288 = 0.0f;/* float */
      int i__conflict__0__289 = 0;/* int */
      int i__290 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__288 = ((float)0.0))/*float*/;
      for ((i__conflict__0__289 = 0)/*int*/; (i__conflict__0__289 < -15); (i__conflict__0__289++)) {{
          (sum__288 = (sum__288 + (__peek__51(i__conflict__0__289) * __peek__51((i__conflict__0__289 + 47)))))/*float*/;
        }
      }
      for ((i__290 = 0)/*int*/; (i__290 < 32); (i__290++)) {{
          __pop__51();
        }
      }
      __push__51(sum__288);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_52;
int __counter_52 = 0;
int __steady_52 = 0;
int __tmp_52 = 0;
int __tmp2_52 = 0;
int *__state_flag_52 = NULL;
thread_info *__thread_52 = NULL;



void save_peek_buffer__52(object_write_buffer *buf);
void load_peek_buffer__52(object_write_buffer *buf);
void save_file_pointer__52(object_write_buffer *buf);
void load_file_pointer__52(object_write_buffer *buf);

 
void init_AnonFilter_a0__297_59__52();
inline void check_status__52();

void work_AnonFilter_a0__297_59__52(int);


inline float __pop__52() {
float res=BUFFER_1_52[TAIL_1_52];
TAIL_1_52++;
return res;
}

inline float __pop__52(int n) {
float res=BUFFER_1_52[TAIL_1_52];
TAIL_1_52+=n;

return res;
}

inline float __peek__52(int offs) {
return BUFFER_1_52[TAIL_1_52+offs];
}



inline void __push__52(float data) {
BUFFER_52_3[HEAD_52_3]=data;
HEAD_52_3++;
}



 
void init_AnonFilter_a0__297_59__52(){
}
void save_file_pointer__52(object_write_buffer *buf) {}
void load_file_pointer__52(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__297_59__52__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__294 = 0.0f;/* float */
  int i__conflict__0__295 = 0;/* int */
  int i__296 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__294 = ((float)0.0))/*float*/;
  for ((i__conflict__0__295 = 0)/*int*/; (i__conflict__0__295 < -16); (i__conflict__0__295++)) {{
      (sum__294 = (sum__294 + ((*(____in+i__conflict__0__295)) * (*(____in+(i__conflict__0__295 + 48))))))/*float*/;
    }
  }
  for ((i__296 = 0)/*int*/; (i__296 < 32); (i__296++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__294);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__297_59__52__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__294 = 0.0f;/* float */
  int i__conflict__0__295 = 0;/* int */
  int i__296 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__294 = ((float)0.0))/*float*/;
  for ((i__conflict__0__295 = 0)/*int*/; (i__conflict__0__295 < -16); (i__conflict__0__295++)) {{
      (sum__294 = (sum__294 + ((*(____in+i__conflict__0__295)) * (*(____in+(i__conflict__0__295 + 48))))))/*float*/;
    }
  }
  for ((i__296 = 0)/*int*/; (i__296 < 32); (i__296++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__294);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__297_59__52(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__294 = 0.0f;/* float */
      int i__conflict__0__295 = 0;/* int */
      int i__296 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__294 = ((float)0.0))/*float*/;
      for ((i__conflict__0__295 = 0)/*int*/; (i__conflict__0__295 < -16); (i__conflict__0__295++)) {{
          (sum__294 = (sum__294 + (__peek__52(i__conflict__0__295) * __peek__52((i__conflict__0__295 + 48)))))/*float*/;
        }
      }
      for ((i__296 = 0)/*int*/; (i__296 < 32); (i__296++)) {{
          __pop__52();
        }
      }
      __push__52(sum__294);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_53;
int __counter_53 = 0;
int __steady_53 = 0;
int __tmp_53 = 0;
int __tmp2_53 = 0;
int *__state_flag_53 = NULL;
thread_info *__thread_53 = NULL;



void save_peek_buffer__53(object_write_buffer *buf);
void load_peek_buffer__53(object_write_buffer *buf);
void save_file_pointer__53(object_write_buffer *buf);
void load_file_pointer__53(object_write_buffer *buf);

 
void init_AnonFilter_a0__303_60__53();
inline void check_status__53();

void work_AnonFilter_a0__303_60__53(int);


inline float __pop__53() {
float res=BUFFER_1_53[TAIL_1_53];
TAIL_1_53++;
return res;
}

inline float __pop__53(int n) {
float res=BUFFER_1_53[TAIL_1_53];
TAIL_1_53+=n;

return res;
}

inline float __peek__53(int offs) {
return BUFFER_1_53[TAIL_1_53+offs];
}



inline void __push__53(float data) {
BUFFER_53_3[HEAD_53_3]=data;
HEAD_53_3++;
}



 
void init_AnonFilter_a0__303_60__53(){
}
void save_file_pointer__53(object_write_buffer *buf) {}
void load_file_pointer__53(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__303_60__53__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__300 = 0.0f;/* float */
  int i__conflict__0__301 = 0;/* int */
  int i__302 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__300 = ((float)0.0))/*float*/;
  for ((i__conflict__0__301 = 0)/*int*/; (i__conflict__0__301 < -17); (i__conflict__0__301++)) {{
      (sum__300 = (sum__300 + ((*(____in+i__conflict__0__301)) * (*(____in+(i__conflict__0__301 + 49))))))/*float*/;
    }
  }
  for ((i__302 = 0)/*int*/; (i__302 < 32); (i__302++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__300);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__303_60__53__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__300 = 0.0f;/* float */
  int i__conflict__0__301 = 0;/* int */
  int i__302 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__300 = ((float)0.0))/*float*/;
  for ((i__conflict__0__301 = 0)/*int*/; (i__conflict__0__301 < -17); (i__conflict__0__301++)) {{
      (sum__300 = (sum__300 + ((*(____in+i__conflict__0__301)) * (*(____in+(i__conflict__0__301 + 49))))))/*float*/;
    }
  }
  for ((i__302 = 0)/*int*/; (i__302 < 32); (i__302++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__300);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__303_60__53(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__300 = 0.0f;/* float */
      int i__conflict__0__301 = 0;/* int */
      int i__302 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__300 = ((float)0.0))/*float*/;
      for ((i__conflict__0__301 = 0)/*int*/; (i__conflict__0__301 < -17); (i__conflict__0__301++)) {{
          (sum__300 = (sum__300 + (__peek__53(i__conflict__0__301) * __peek__53((i__conflict__0__301 + 49)))))/*float*/;
        }
      }
      for ((i__302 = 0)/*int*/; (i__302 < 32); (i__302++)) {{
          __pop__53();
        }
      }
      __push__53(sum__300);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_54;
int __counter_54 = 0;
int __steady_54 = 0;
int __tmp_54 = 0;
int __tmp2_54 = 0;
int *__state_flag_54 = NULL;
thread_info *__thread_54 = NULL;



void save_peek_buffer__54(object_write_buffer *buf);
void load_peek_buffer__54(object_write_buffer *buf);
void save_file_pointer__54(object_write_buffer *buf);
void load_file_pointer__54(object_write_buffer *buf);

 
void init_AnonFilter_a0__309_61__54();
inline void check_status__54();

void work_AnonFilter_a0__309_61__54(int);


inline float __pop__54() {
float res=BUFFER_1_54[TAIL_1_54];
TAIL_1_54++;
return res;
}

inline float __pop__54(int n) {
float res=BUFFER_1_54[TAIL_1_54];
TAIL_1_54+=n;

return res;
}

inline float __peek__54(int offs) {
return BUFFER_1_54[TAIL_1_54+offs];
}



inline void __push__54(float data) {
BUFFER_54_3[HEAD_54_3]=data;
HEAD_54_3++;
}



 
void init_AnonFilter_a0__309_61__54(){
}
void save_file_pointer__54(object_write_buffer *buf) {}
void load_file_pointer__54(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__309_61__54__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__306 = 0.0f;/* float */
  int i__conflict__0__307 = 0;/* int */
  int i__308 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__306 = ((float)0.0))/*float*/;
  for ((i__conflict__0__307 = 0)/*int*/; (i__conflict__0__307 < -18); (i__conflict__0__307++)) {{
      (sum__306 = (sum__306 + ((*(____in+i__conflict__0__307)) * (*(____in+(i__conflict__0__307 + 50))))))/*float*/;
    }
  }
  for ((i__308 = 0)/*int*/; (i__308 < 32); (i__308++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__306);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__309_61__54__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__306 = 0.0f;/* float */
  int i__conflict__0__307 = 0;/* int */
  int i__308 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__306 = ((float)0.0))/*float*/;
  for ((i__conflict__0__307 = 0)/*int*/; (i__conflict__0__307 < -18); (i__conflict__0__307++)) {{
      (sum__306 = (sum__306 + ((*(____in+i__conflict__0__307)) * (*(____in+(i__conflict__0__307 + 50))))))/*float*/;
    }
  }
  for ((i__308 = 0)/*int*/; (i__308 < 32); (i__308++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__306);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__309_61__54(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__306 = 0.0f;/* float */
      int i__conflict__0__307 = 0;/* int */
      int i__308 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__306 = ((float)0.0))/*float*/;
      for ((i__conflict__0__307 = 0)/*int*/; (i__conflict__0__307 < -18); (i__conflict__0__307++)) {{
          (sum__306 = (sum__306 + (__peek__54(i__conflict__0__307) * __peek__54((i__conflict__0__307 + 50)))))/*float*/;
        }
      }
      for ((i__308 = 0)/*int*/; (i__308 < 32); (i__308++)) {{
          __pop__54();
        }
      }
      __push__54(sum__306);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_55;
int __counter_55 = 0;
int __steady_55 = 0;
int __tmp_55 = 0;
int __tmp2_55 = 0;
int *__state_flag_55 = NULL;
thread_info *__thread_55 = NULL;



void save_peek_buffer__55(object_write_buffer *buf);
void load_peek_buffer__55(object_write_buffer *buf);
void save_file_pointer__55(object_write_buffer *buf);
void load_file_pointer__55(object_write_buffer *buf);

 
void init_AnonFilter_a0__315_62__55();
inline void check_status__55();

void work_AnonFilter_a0__315_62__55(int);


inline float __pop__55() {
float res=BUFFER_1_55[TAIL_1_55];
TAIL_1_55++;
return res;
}

inline float __pop__55(int n) {
float res=BUFFER_1_55[TAIL_1_55];
TAIL_1_55+=n;

return res;
}

inline float __peek__55(int offs) {
return BUFFER_1_55[TAIL_1_55+offs];
}



inline void __push__55(float data) {
BUFFER_55_3[HEAD_55_3]=data;
HEAD_55_3++;
}



 
void init_AnonFilter_a0__315_62__55(){
}
void save_file_pointer__55(object_write_buffer *buf) {}
void load_file_pointer__55(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__315_62__55__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__312 = 0.0f;/* float */
  int i__conflict__0__313 = 0;/* int */
  int i__314 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__312 = ((float)0.0))/*float*/;
  for ((i__conflict__0__313 = 0)/*int*/; (i__conflict__0__313 < -19); (i__conflict__0__313++)) {{
      (sum__312 = (sum__312 + ((*(____in+i__conflict__0__313)) * (*(____in+(i__conflict__0__313 + 51))))))/*float*/;
    }
  }
  for ((i__314 = 0)/*int*/; (i__314 < 32); (i__314++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__312);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__315_62__55__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__312 = 0.0f;/* float */
  int i__conflict__0__313 = 0;/* int */
  int i__314 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__312 = ((float)0.0))/*float*/;
  for ((i__conflict__0__313 = 0)/*int*/; (i__conflict__0__313 < -19); (i__conflict__0__313++)) {{
      (sum__312 = (sum__312 + ((*(____in+i__conflict__0__313)) * (*(____in+(i__conflict__0__313 + 51))))))/*float*/;
    }
  }
  for ((i__314 = 0)/*int*/; (i__314 < 32); (i__314++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__312);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__315_62__55(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__312 = 0.0f;/* float */
      int i__conflict__0__313 = 0;/* int */
      int i__314 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__312 = ((float)0.0))/*float*/;
      for ((i__conflict__0__313 = 0)/*int*/; (i__conflict__0__313 < -19); (i__conflict__0__313++)) {{
          (sum__312 = (sum__312 + (__peek__55(i__conflict__0__313) * __peek__55((i__conflict__0__313 + 51)))))/*float*/;
        }
      }
      for ((i__314 = 0)/*int*/; (i__314 < 32); (i__314++)) {{
          __pop__55();
        }
      }
      __push__55(sum__312);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_56;
int __counter_56 = 0;
int __steady_56 = 0;
int __tmp_56 = 0;
int __tmp2_56 = 0;
int *__state_flag_56 = NULL;
thread_info *__thread_56 = NULL;



void save_peek_buffer__56(object_write_buffer *buf);
void load_peek_buffer__56(object_write_buffer *buf);
void save_file_pointer__56(object_write_buffer *buf);
void load_file_pointer__56(object_write_buffer *buf);

 
void init_AnonFilter_a0__321_63__56();
inline void check_status__56();

void work_AnonFilter_a0__321_63__56(int);


inline float __pop__56() {
float res=BUFFER_1_56[TAIL_1_56];
TAIL_1_56++;
return res;
}

inline float __pop__56(int n) {
float res=BUFFER_1_56[TAIL_1_56];
TAIL_1_56+=n;

return res;
}

inline float __peek__56(int offs) {
return BUFFER_1_56[TAIL_1_56+offs];
}



inline void __push__56(float data) {
BUFFER_56_3[HEAD_56_3]=data;
HEAD_56_3++;
}



 
void init_AnonFilter_a0__321_63__56(){
}
void save_file_pointer__56(object_write_buffer *buf) {}
void load_file_pointer__56(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__321_63__56__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__318 = 0.0f;/* float */
  int i__conflict__0__319 = 0;/* int */
  int i__320 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__318 = ((float)0.0))/*float*/;
  for ((i__conflict__0__319 = 0)/*int*/; (i__conflict__0__319 < -20); (i__conflict__0__319++)) {{
      (sum__318 = (sum__318 + ((*(____in+i__conflict__0__319)) * (*(____in+(i__conflict__0__319 + 52))))))/*float*/;
    }
  }
  for ((i__320 = 0)/*int*/; (i__320 < 32); (i__320++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__318);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__321_63__56__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__318 = 0.0f;/* float */
  int i__conflict__0__319 = 0;/* int */
  int i__320 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__318 = ((float)0.0))/*float*/;
  for ((i__conflict__0__319 = 0)/*int*/; (i__conflict__0__319 < -20); (i__conflict__0__319++)) {{
      (sum__318 = (sum__318 + ((*(____in+i__conflict__0__319)) * (*(____in+(i__conflict__0__319 + 52))))))/*float*/;
    }
  }
  for ((i__320 = 0)/*int*/; (i__320 < 32); (i__320++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__318);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__321_63__56(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__318 = 0.0f;/* float */
      int i__conflict__0__319 = 0;/* int */
      int i__320 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__318 = ((float)0.0))/*float*/;
      for ((i__conflict__0__319 = 0)/*int*/; (i__conflict__0__319 < -20); (i__conflict__0__319++)) {{
          (sum__318 = (sum__318 + (__peek__56(i__conflict__0__319) * __peek__56((i__conflict__0__319 + 52)))))/*float*/;
        }
      }
      for ((i__320 = 0)/*int*/; (i__320 < 32); (i__320++)) {{
          __pop__56();
        }
      }
      __push__56(sum__318);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_57;
int __counter_57 = 0;
int __steady_57 = 0;
int __tmp_57 = 0;
int __tmp2_57 = 0;
int *__state_flag_57 = NULL;
thread_info *__thread_57 = NULL;



void save_peek_buffer__57(object_write_buffer *buf);
void load_peek_buffer__57(object_write_buffer *buf);
void save_file_pointer__57(object_write_buffer *buf);
void load_file_pointer__57(object_write_buffer *buf);

 
void init_AnonFilter_a0__327_64__57();
inline void check_status__57();

void work_AnonFilter_a0__327_64__57(int);


inline float __pop__57() {
float res=BUFFER_1_57[TAIL_1_57];
TAIL_1_57++;
return res;
}

inline float __pop__57(int n) {
float res=BUFFER_1_57[TAIL_1_57];
TAIL_1_57+=n;

return res;
}

inline float __peek__57(int offs) {
return BUFFER_1_57[TAIL_1_57+offs];
}



inline void __push__57(float data) {
BUFFER_57_3[HEAD_57_3]=data;
HEAD_57_3++;
}



 
void init_AnonFilter_a0__327_64__57(){
}
void save_file_pointer__57(object_write_buffer *buf) {}
void load_file_pointer__57(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__327_64__57__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__324 = 0.0f;/* float */
  int i__conflict__0__325 = 0;/* int */
  int i__326 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__324 = ((float)0.0))/*float*/;
  for ((i__conflict__0__325 = 0)/*int*/; (i__conflict__0__325 < -21); (i__conflict__0__325++)) {{
      (sum__324 = (sum__324 + ((*(____in+i__conflict__0__325)) * (*(____in+(i__conflict__0__325 + 53))))))/*float*/;
    }
  }
  for ((i__326 = 0)/*int*/; (i__326 < 32); (i__326++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__324);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__327_64__57__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__324 = 0.0f;/* float */
  int i__conflict__0__325 = 0;/* int */
  int i__326 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__324 = ((float)0.0))/*float*/;
  for ((i__conflict__0__325 = 0)/*int*/; (i__conflict__0__325 < -21); (i__conflict__0__325++)) {{
      (sum__324 = (sum__324 + ((*(____in+i__conflict__0__325)) * (*(____in+(i__conflict__0__325 + 53))))))/*float*/;
    }
  }
  for ((i__326 = 0)/*int*/; (i__326 < 32); (i__326++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__324);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__327_64__57(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__324 = 0.0f;/* float */
      int i__conflict__0__325 = 0;/* int */
      int i__326 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__324 = ((float)0.0))/*float*/;
      for ((i__conflict__0__325 = 0)/*int*/; (i__conflict__0__325 < -21); (i__conflict__0__325++)) {{
          (sum__324 = (sum__324 + (__peek__57(i__conflict__0__325) * __peek__57((i__conflict__0__325 + 53)))))/*float*/;
        }
      }
      for ((i__326 = 0)/*int*/; (i__326 < 32); (i__326++)) {{
          __pop__57();
        }
      }
      __push__57(sum__324);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_58;
int __counter_58 = 0;
int __steady_58 = 0;
int __tmp_58 = 0;
int __tmp2_58 = 0;
int *__state_flag_58 = NULL;
thread_info *__thread_58 = NULL;



void save_peek_buffer__58(object_write_buffer *buf);
void load_peek_buffer__58(object_write_buffer *buf);
void save_file_pointer__58(object_write_buffer *buf);
void load_file_pointer__58(object_write_buffer *buf);

 
void init_AnonFilter_a0__333_65__58();
inline void check_status__58();

void work_AnonFilter_a0__333_65__58(int);


inline float __pop__58() {
float res=BUFFER_1_58[TAIL_1_58];
TAIL_1_58++;
return res;
}

inline float __pop__58(int n) {
float res=BUFFER_1_58[TAIL_1_58];
TAIL_1_58+=n;

return res;
}

inline float __peek__58(int offs) {
return BUFFER_1_58[TAIL_1_58+offs];
}



inline void __push__58(float data) {
BUFFER_58_3[HEAD_58_3]=data;
HEAD_58_3++;
}



 
void init_AnonFilter_a0__333_65__58(){
}
void save_file_pointer__58(object_write_buffer *buf) {}
void load_file_pointer__58(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__333_65__58__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__330 = 0.0f;/* float */
  int i__conflict__0__331 = 0;/* int */
  int i__332 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__330 = ((float)0.0))/*float*/;
  for ((i__conflict__0__331 = 0)/*int*/; (i__conflict__0__331 < -22); (i__conflict__0__331++)) {{
      (sum__330 = (sum__330 + ((*(____in+i__conflict__0__331)) * (*(____in+(i__conflict__0__331 + 54))))))/*float*/;
    }
  }
  for ((i__332 = 0)/*int*/; (i__332 < 32); (i__332++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__330);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__333_65__58__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__330 = 0.0f;/* float */
  int i__conflict__0__331 = 0;/* int */
  int i__332 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__330 = ((float)0.0))/*float*/;
  for ((i__conflict__0__331 = 0)/*int*/; (i__conflict__0__331 < -22); (i__conflict__0__331++)) {{
      (sum__330 = (sum__330 + ((*(____in+i__conflict__0__331)) * (*(____in+(i__conflict__0__331 + 54))))))/*float*/;
    }
  }
  for ((i__332 = 0)/*int*/; (i__332 < 32); (i__332++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__330);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__333_65__58(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__330 = 0.0f;/* float */
      int i__conflict__0__331 = 0;/* int */
      int i__332 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__330 = ((float)0.0))/*float*/;
      for ((i__conflict__0__331 = 0)/*int*/; (i__conflict__0__331 < -22); (i__conflict__0__331++)) {{
          (sum__330 = (sum__330 + (__peek__58(i__conflict__0__331) * __peek__58((i__conflict__0__331 + 54)))))/*float*/;
        }
      }
      for ((i__332 = 0)/*int*/; (i__332 < 32); (i__332++)) {{
          __pop__58();
        }
      }
      __push__58(sum__330);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_59;
int __counter_59 = 0;
int __steady_59 = 0;
int __tmp_59 = 0;
int __tmp2_59 = 0;
int *__state_flag_59 = NULL;
thread_info *__thread_59 = NULL;



void save_peek_buffer__59(object_write_buffer *buf);
void load_peek_buffer__59(object_write_buffer *buf);
void save_file_pointer__59(object_write_buffer *buf);
void load_file_pointer__59(object_write_buffer *buf);

 
void init_AnonFilter_a0__339_66__59();
inline void check_status__59();

void work_AnonFilter_a0__339_66__59(int);


inline float __pop__59() {
float res=BUFFER_1_59[TAIL_1_59];
TAIL_1_59++;
return res;
}

inline float __pop__59(int n) {
float res=BUFFER_1_59[TAIL_1_59];
TAIL_1_59+=n;

return res;
}

inline float __peek__59(int offs) {
return BUFFER_1_59[TAIL_1_59+offs];
}



inline void __push__59(float data) {
BUFFER_59_3[HEAD_59_3]=data;
HEAD_59_3++;
}



 
void init_AnonFilter_a0__339_66__59(){
}
void save_file_pointer__59(object_write_buffer *buf) {}
void load_file_pointer__59(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__339_66__59__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__336 = 0.0f;/* float */
  int i__conflict__0__337 = 0;/* int */
  int i__338 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__336 = ((float)0.0))/*float*/;
  for ((i__conflict__0__337 = 0)/*int*/; (i__conflict__0__337 < -23); (i__conflict__0__337++)) {{
      (sum__336 = (sum__336 + ((*(____in+i__conflict__0__337)) * (*(____in+(i__conflict__0__337 + 55))))))/*float*/;
    }
  }
  for ((i__338 = 0)/*int*/; (i__338 < 32); (i__338++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__336);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__339_66__59__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__336 = 0.0f;/* float */
  int i__conflict__0__337 = 0;/* int */
  int i__338 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__336 = ((float)0.0))/*float*/;
  for ((i__conflict__0__337 = 0)/*int*/; (i__conflict__0__337 < -23); (i__conflict__0__337++)) {{
      (sum__336 = (sum__336 + ((*(____in+i__conflict__0__337)) * (*(____in+(i__conflict__0__337 + 55))))))/*float*/;
    }
  }
  for ((i__338 = 0)/*int*/; (i__338 < 32); (i__338++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__336);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__339_66__59(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__336 = 0.0f;/* float */
      int i__conflict__0__337 = 0;/* int */
      int i__338 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__336 = ((float)0.0))/*float*/;
      for ((i__conflict__0__337 = 0)/*int*/; (i__conflict__0__337 < -23); (i__conflict__0__337++)) {{
          (sum__336 = (sum__336 + (__peek__59(i__conflict__0__337) * __peek__59((i__conflict__0__337 + 55)))))/*float*/;
        }
      }
      for ((i__338 = 0)/*int*/; (i__338 < 32); (i__338++)) {{
          __pop__59();
        }
      }
      __push__59(sum__336);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_60;
int __counter_60 = 0;
int __steady_60 = 0;
int __tmp_60 = 0;
int __tmp2_60 = 0;
int *__state_flag_60 = NULL;
thread_info *__thread_60 = NULL;



void save_peek_buffer__60(object_write_buffer *buf);
void load_peek_buffer__60(object_write_buffer *buf);
void save_file_pointer__60(object_write_buffer *buf);
void load_file_pointer__60(object_write_buffer *buf);

 
void init_AnonFilter_a0__345_67__60();
inline void check_status__60();

void work_AnonFilter_a0__345_67__60(int);


inline float __pop__60() {
float res=BUFFER_1_60[TAIL_1_60];
TAIL_1_60++;
return res;
}

inline float __pop__60(int n) {
float res=BUFFER_1_60[TAIL_1_60];
TAIL_1_60+=n;

return res;
}

inline float __peek__60(int offs) {
return BUFFER_1_60[TAIL_1_60+offs];
}



inline void __push__60(float data) {
BUFFER_60_3[HEAD_60_3]=data;
HEAD_60_3++;
}



 
void init_AnonFilter_a0__345_67__60(){
}
void save_file_pointer__60(object_write_buffer *buf) {}
void load_file_pointer__60(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__345_67__60__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__342 = 0.0f;/* float */
  int i__conflict__0__343 = 0;/* int */
  int i__344 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__342 = ((float)0.0))/*float*/;
  for ((i__conflict__0__343 = 0)/*int*/; (i__conflict__0__343 < -24); (i__conflict__0__343++)) {{
      (sum__342 = (sum__342 + ((*(____in+i__conflict__0__343)) * (*(____in+(i__conflict__0__343 + 56))))))/*float*/;
    }
  }
  for ((i__344 = 0)/*int*/; (i__344 < 32); (i__344++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__342);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__345_67__60__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__342 = 0.0f;/* float */
  int i__conflict__0__343 = 0;/* int */
  int i__344 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__342 = ((float)0.0))/*float*/;
  for ((i__conflict__0__343 = 0)/*int*/; (i__conflict__0__343 < -24); (i__conflict__0__343++)) {{
      (sum__342 = (sum__342 + ((*(____in+i__conflict__0__343)) * (*(____in+(i__conflict__0__343 + 56))))))/*float*/;
    }
  }
  for ((i__344 = 0)/*int*/; (i__344 < 32); (i__344++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__342);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__345_67__60(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__342 = 0.0f;/* float */
      int i__conflict__0__343 = 0;/* int */
      int i__344 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__342 = ((float)0.0))/*float*/;
      for ((i__conflict__0__343 = 0)/*int*/; (i__conflict__0__343 < -24); (i__conflict__0__343++)) {{
          (sum__342 = (sum__342 + (__peek__60(i__conflict__0__343) * __peek__60((i__conflict__0__343 + 56)))))/*float*/;
        }
      }
      for ((i__344 = 0)/*int*/; (i__344 < 32); (i__344++)) {{
          __pop__60();
        }
      }
      __push__60(sum__342);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_61;
int __counter_61 = 0;
int __steady_61 = 0;
int __tmp_61 = 0;
int __tmp2_61 = 0;
int *__state_flag_61 = NULL;
thread_info *__thread_61 = NULL;



void save_peek_buffer__61(object_write_buffer *buf);
void load_peek_buffer__61(object_write_buffer *buf);
void save_file_pointer__61(object_write_buffer *buf);
void load_file_pointer__61(object_write_buffer *buf);

 
void init_AnonFilter_a0__351_68__61();
inline void check_status__61();

void work_AnonFilter_a0__351_68__61(int);


inline float __pop__61() {
float res=BUFFER_1_61[TAIL_1_61];
TAIL_1_61++;
return res;
}

inline float __pop__61(int n) {
float res=BUFFER_1_61[TAIL_1_61];
TAIL_1_61+=n;

return res;
}

inline float __peek__61(int offs) {
return BUFFER_1_61[TAIL_1_61+offs];
}



inline void __push__61(float data) {
BUFFER_61_3[HEAD_61_3]=data;
HEAD_61_3++;
}



 
void init_AnonFilter_a0__351_68__61(){
}
void save_file_pointer__61(object_write_buffer *buf) {}
void load_file_pointer__61(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__351_68__61__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__348 = 0.0f;/* float */
  int i__conflict__0__349 = 0;/* int */
  int i__350 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__348 = ((float)0.0))/*float*/;
  for ((i__conflict__0__349 = 0)/*int*/; (i__conflict__0__349 < -25); (i__conflict__0__349++)) {{
      (sum__348 = (sum__348 + ((*(____in+i__conflict__0__349)) * (*(____in+(i__conflict__0__349 + 57))))))/*float*/;
    }
  }
  for ((i__350 = 0)/*int*/; (i__350 < 32); (i__350++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__348);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__351_68__61__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__348 = 0.0f;/* float */
  int i__conflict__0__349 = 0;/* int */
  int i__350 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__348 = ((float)0.0))/*float*/;
  for ((i__conflict__0__349 = 0)/*int*/; (i__conflict__0__349 < -25); (i__conflict__0__349++)) {{
      (sum__348 = (sum__348 + ((*(____in+i__conflict__0__349)) * (*(____in+(i__conflict__0__349 + 57))))))/*float*/;
    }
  }
  for ((i__350 = 0)/*int*/; (i__350 < 32); (i__350++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__348);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__351_68__61(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__348 = 0.0f;/* float */
      int i__conflict__0__349 = 0;/* int */
      int i__350 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__348 = ((float)0.0))/*float*/;
      for ((i__conflict__0__349 = 0)/*int*/; (i__conflict__0__349 < -25); (i__conflict__0__349++)) {{
          (sum__348 = (sum__348 + (__peek__61(i__conflict__0__349) * __peek__61((i__conflict__0__349 + 57)))))/*float*/;
        }
      }
      for ((i__350 = 0)/*int*/; (i__350 < 32); (i__350++)) {{
          __pop__61();
        }
      }
      __push__61(sum__348);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_62;
int __counter_62 = 0;
int __steady_62 = 0;
int __tmp_62 = 0;
int __tmp2_62 = 0;
int *__state_flag_62 = NULL;
thread_info *__thread_62 = NULL;



void save_peek_buffer__62(object_write_buffer *buf);
void load_peek_buffer__62(object_write_buffer *buf);
void save_file_pointer__62(object_write_buffer *buf);
void load_file_pointer__62(object_write_buffer *buf);

 
void init_AnonFilter_a0__357_69__62();
inline void check_status__62();

void work_AnonFilter_a0__357_69__62(int);


inline float __pop__62() {
float res=BUFFER_1_62[TAIL_1_62];
TAIL_1_62++;
return res;
}

inline float __pop__62(int n) {
float res=BUFFER_1_62[TAIL_1_62];
TAIL_1_62+=n;

return res;
}

inline float __peek__62(int offs) {
return BUFFER_1_62[TAIL_1_62+offs];
}



inline void __push__62(float data) {
BUFFER_62_3[HEAD_62_3]=data;
HEAD_62_3++;
}



 
void init_AnonFilter_a0__357_69__62(){
}
void save_file_pointer__62(object_write_buffer *buf) {}
void load_file_pointer__62(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__357_69__62__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__354 = 0.0f;/* float */
  int i__conflict__0__355 = 0;/* int */
  int i__356 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__354 = ((float)0.0))/*float*/;
  for ((i__conflict__0__355 = 0)/*int*/; (i__conflict__0__355 < -26); (i__conflict__0__355++)) {{
      (sum__354 = (sum__354 + ((*(____in+i__conflict__0__355)) * (*(____in+(i__conflict__0__355 + 58))))))/*float*/;
    }
  }
  for ((i__356 = 0)/*int*/; (i__356 < 32); (i__356++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__354);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__357_69__62__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__354 = 0.0f;/* float */
  int i__conflict__0__355 = 0;/* int */
  int i__356 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__354 = ((float)0.0))/*float*/;
  for ((i__conflict__0__355 = 0)/*int*/; (i__conflict__0__355 < -26); (i__conflict__0__355++)) {{
      (sum__354 = (sum__354 + ((*(____in+i__conflict__0__355)) * (*(____in+(i__conflict__0__355 + 58))))))/*float*/;
    }
  }
  for ((i__356 = 0)/*int*/; (i__356 < 32); (i__356++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__354);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__357_69__62(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__354 = 0.0f;/* float */
      int i__conflict__0__355 = 0;/* int */
      int i__356 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__354 = ((float)0.0))/*float*/;
      for ((i__conflict__0__355 = 0)/*int*/; (i__conflict__0__355 < -26); (i__conflict__0__355++)) {{
          (sum__354 = (sum__354 + (__peek__62(i__conflict__0__355) * __peek__62((i__conflict__0__355 + 58)))))/*float*/;
        }
      }
      for ((i__356 = 0)/*int*/; (i__356 < 32); (i__356++)) {{
          __pop__62();
        }
      }
      __push__62(sum__354);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_63;
int __counter_63 = 0;
int __steady_63 = 0;
int __tmp_63 = 0;
int __tmp2_63 = 0;
int *__state_flag_63 = NULL;
thread_info *__thread_63 = NULL;



void save_peek_buffer__63(object_write_buffer *buf);
void load_peek_buffer__63(object_write_buffer *buf);
void save_file_pointer__63(object_write_buffer *buf);
void load_file_pointer__63(object_write_buffer *buf);

 
void init_AnonFilter_a0__363_70__63();
inline void check_status__63();

void work_AnonFilter_a0__363_70__63(int);


inline float __pop__63() {
float res=BUFFER_1_63[TAIL_1_63];
TAIL_1_63++;
return res;
}

inline float __pop__63(int n) {
float res=BUFFER_1_63[TAIL_1_63];
TAIL_1_63+=n;

return res;
}

inline float __peek__63(int offs) {
return BUFFER_1_63[TAIL_1_63+offs];
}



inline void __push__63(float data) {
BUFFER_63_3[HEAD_63_3]=data;
HEAD_63_3++;
}



 
void init_AnonFilter_a0__363_70__63(){
}
void save_file_pointer__63(object_write_buffer *buf) {}
void load_file_pointer__63(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__363_70__63__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__360 = 0.0f;/* float */
  int i__conflict__0__361 = 0;/* int */
  int i__362 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__360 = ((float)0.0))/*float*/;
  for ((i__conflict__0__361 = 0)/*int*/; (i__conflict__0__361 < -27); (i__conflict__0__361++)) {{
      (sum__360 = (sum__360 + ((*(____in+i__conflict__0__361)) * (*(____in+(i__conflict__0__361 + 59))))))/*float*/;
    }
  }
  for ((i__362 = 0)/*int*/; (i__362 < 32); (i__362++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__360);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__363_70__63__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__360 = 0.0f;/* float */
  int i__conflict__0__361 = 0;/* int */
  int i__362 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__360 = ((float)0.0))/*float*/;
  for ((i__conflict__0__361 = 0)/*int*/; (i__conflict__0__361 < -27); (i__conflict__0__361++)) {{
      (sum__360 = (sum__360 + ((*(____in+i__conflict__0__361)) * (*(____in+(i__conflict__0__361 + 59))))))/*float*/;
    }
  }
  for ((i__362 = 0)/*int*/; (i__362 < 32); (i__362++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__360);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__363_70__63(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__360 = 0.0f;/* float */
      int i__conflict__0__361 = 0;/* int */
      int i__362 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__360 = ((float)0.0))/*float*/;
      for ((i__conflict__0__361 = 0)/*int*/; (i__conflict__0__361 < -27); (i__conflict__0__361++)) {{
          (sum__360 = (sum__360 + (__peek__63(i__conflict__0__361) * __peek__63((i__conflict__0__361 + 59)))))/*float*/;
        }
      }
      for ((i__362 = 0)/*int*/; (i__362 < 32); (i__362++)) {{
          __pop__63();
        }
      }
      __push__63(sum__360);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_64;
int __counter_64 = 0;
int __steady_64 = 0;
int __tmp_64 = 0;
int __tmp2_64 = 0;
int *__state_flag_64 = NULL;
thread_info *__thread_64 = NULL;



void save_peek_buffer__64(object_write_buffer *buf);
void load_peek_buffer__64(object_write_buffer *buf);
void save_file_pointer__64(object_write_buffer *buf);
void load_file_pointer__64(object_write_buffer *buf);

 
void init_AnonFilter_a0__369_71__64();
inline void check_status__64();

void work_AnonFilter_a0__369_71__64(int);


inline float __pop__64() {
float res=BUFFER_1_64[TAIL_1_64];
TAIL_1_64++;
return res;
}

inline float __pop__64(int n) {
float res=BUFFER_1_64[TAIL_1_64];
TAIL_1_64+=n;

return res;
}

inline float __peek__64(int offs) {
return BUFFER_1_64[TAIL_1_64+offs];
}



inline void __push__64(float data) {
BUFFER_64_3[HEAD_64_3]=data;
HEAD_64_3++;
}



 
void init_AnonFilter_a0__369_71__64(){
}
void save_file_pointer__64(object_write_buffer *buf) {}
void load_file_pointer__64(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__369_71__64__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__366 = 0.0f;/* float */
  int i__conflict__0__367 = 0;/* int */
  int i__368 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__366 = ((float)0.0))/*float*/;
  for ((i__conflict__0__367 = 0)/*int*/; (i__conflict__0__367 < -28); (i__conflict__0__367++)) {{
      (sum__366 = (sum__366 + ((*(____in+i__conflict__0__367)) * (*(____in+(i__conflict__0__367 + 60))))))/*float*/;
    }
  }
  for ((i__368 = 0)/*int*/; (i__368 < 32); (i__368++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__366);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__369_71__64__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__366 = 0.0f;/* float */
  int i__conflict__0__367 = 0;/* int */
  int i__368 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__366 = ((float)0.0))/*float*/;
  for ((i__conflict__0__367 = 0)/*int*/; (i__conflict__0__367 < -28); (i__conflict__0__367++)) {{
      (sum__366 = (sum__366 + ((*(____in+i__conflict__0__367)) * (*(____in+(i__conflict__0__367 + 60))))))/*float*/;
    }
  }
  for ((i__368 = 0)/*int*/; (i__368 < 32); (i__368++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__366);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__369_71__64(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__366 = 0.0f;/* float */
      int i__conflict__0__367 = 0;/* int */
      int i__368 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__366 = ((float)0.0))/*float*/;
      for ((i__conflict__0__367 = 0)/*int*/; (i__conflict__0__367 < -28); (i__conflict__0__367++)) {{
          (sum__366 = (sum__366 + (__peek__64(i__conflict__0__367) * __peek__64((i__conflict__0__367 + 60)))))/*float*/;
        }
      }
      for ((i__368 = 0)/*int*/; (i__368 < 32); (i__368++)) {{
          __pop__64();
        }
      }
      __push__64(sum__366);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_65;
int __counter_65 = 0;
int __steady_65 = 0;
int __tmp_65 = 0;
int __tmp2_65 = 0;
int *__state_flag_65 = NULL;
thread_info *__thread_65 = NULL;



void save_peek_buffer__65(object_write_buffer *buf);
void load_peek_buffer__65(object_write_buffer *buf);
void save_file_pointer__65(object_write_buffer *buf);
void load_file_pointer__65(object_write_buffer *buf);

 
void init_AnonFilter_a0__375_72__65();
inline void check_status__65();

void work_AnonFilter_a0__375_72__65(int);


inline float __pop__65() {
float res=BUFFER_1_65[TAIL_1_65];
TAIL_1_65++;
return res;
}

inline float __pop__65(int n) {
float res=BUFFER_1_65[TAIL_1_65];
TAIL_1_65+=n;

return res;
}

inline float __peek__65(int offs) {
return BUFFER_1_65[TAIL_1_65+offs];
}



inline void __push__65(float data) {
BUFFER_65_3[HEAD_65_3]=data;
HEAD_65_3++;
}



 
void init_AnonFilter_a0__375_72__65(){
}
void save_file_pointer__65(object_write_buffer *buf) {}
void load_file_pointer__65(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__375_72__65__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__372 = 0.0f;/* float */
  int i__conflict__0__373 = 0;/* int */
  int i__374 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__372 = ((float)0.0))/*float*/;
  for ((i__conflict__0__373 = 0)/*int*/; (i__conflict__0__373 < -29); (i__conflict__0__373++)) {{
      (sum__372 = (sum__372 + ((*(____in+i__conflict__0__373)) * (*(____in+(i__conflict__0__373 + 61))))))/*float*/;
    }
  }
  for ((i__374 = 0)/*int*/; (i__374 < 32); (i__374++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__372);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__375_72__65__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__372 = 0.0f;/* float */
  int i__conflict__0__373 = 0;/* int */
  int i__374 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__372 = ((float)0.0))/*float*/;
  for ((i__conflict__0__373 = 0)/*int*/; (i__conflict__0__373 < -29); (i__conflict__0__373++)) {{
      (sum__372 = (sum__372 + ((*(____in+i__conflict__0__373)) * (*(____in+(i__conflict__0__373 + 61))))))/*float*/;
    }
  }
  for ((i__374 = 0)/*int*/; (i__374 < 32); (i__374++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__372);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__375_72__65(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__372 = 0.0f;/* float */
      int i__conflict__0__373 = 0;/* int */
      int i__374 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__372 = ((float)0.0))/*float*/;
      for ((i__conflict__0__373 = 0)/*int*/; (i__conflict__0__373 < -29); (i__conflict__0__373++)) {{
          (sum__372 = (sum__372 + (__peek__65(i__conflict__0__373) * __peek__65((i__conflict__0__373 + 61)))))/*float*/;
        }
      }
      for ((i__374 = 0)/*int*/; (i__374 < 32); (i__374++)) {{
          __pop__65();
        }
      }
      __push__65(sum__372);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_66;
int __counter_66 = 0;
int __steady_66 = 0;
int __tmp_66 = 0;
int __tmp2_66 = 0;
int *__state_flag_66 = NULL;
thread_info *__thread_66 = NULL;



void save_peek_buffer__66(object_write_buffer *buf);
void load_peek_buffer__66(object_write_buffer *buf);
void save_file_pointer__66(object_write_buffer *buf);
void load_file_pointer__66(object_write_buffer *buf);

 
void init_AnonFilter_a0__381_73__66();
inline void check_status__66();

void work_AnonFilter_a0__381_73__66(int);


inline float __pop__66() {
float res=BUFFER_1_66[TAIL_1_66];
TAIL_1_66++;
return res;
}

inline float __pop__66(int n) {
float res=BUFFER_1_66[TAIL_1_66];
TAIL_1_66+=n;

return res;
}

inline float __peek__66(int offs) {
return BUFFER_1_66[TAIL_1_66+offs];
}



inline void __push__66(float data) {
BUFFER_66_3[HEAD_66_3]=data;
HEAD_66_3++;
}



 
void init_AnonFilter_a0__381_73__66(){
}
void save_file_pointer__66(object_write_buffer *buf) {}
void load_file_pointer__66(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__381_73__66__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__378 = 0.0f;/* float */
  int i__conflict__0__379 = 0;/* int */
  int i__380 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__378 = ((float)0.0))/*float*/;
  for ((i__conflict__0__379 = 0)/*int*/; (i__conflict__0__379 < -30); (i__conflict__0__379++)) {{
      (sum__378 = (sum__378 + ((*(____in+i__conflict__0__379)) * (*(____in+(i__conflict__0__379 + 62))))))/*float*/;
    }
  }
  for ((i__380 = 0)/*int*/; (i__380 < 32); (i__380++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__378);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__381_73__66__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__378 = 0.0f;/* float */
  int i__conflict__0__379 = 0;/* int */
  int i__380 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__378 = ((float)0.0))/*float*/;
  for ((i__conflict__0__379 = 0)/*int*/; (i__conflict__0__379 < -30); (i__conflict__0__379++)) {{
      (sum__378 = (sum__378 + ((*(____in+i__conflict__0__379)) * (*(____in+(i__conflict__0__379 + 62))))))/*float*/;
    }
  }
  for ((i__380 = 0)/*int*/; (i__380 < 32); (i__380++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__378);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__381_73__66(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__378 = 0.0f;/* float */
      int i__conflict__0__379 = 0;/* int */
      int i__380 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__378 = ((float)0.0))/*float*/;
      for ((i__conflict__0__379 = 0)/*int*/; (i__conflict__0__379 < -30); (i__conflict__0__379++)) {{
          (sum__378 = (sum__378 + (__peek__66(i__conflict__0__379) * __peek__66((i__conflict__0__379 + 62)))))/*float*/;
        }
      }
      for ((i__380 = 0)/*int*/; (i__380 < 32); (i__380++)) {{
          __pop__66();
        }
      }
      __push__66(sum__378);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_67;
int __counter_67 = 0;
int __steady_67 = 0;
int __tmp_67 = 0;
int __tmp2_67 = 0;
int *__state_flag_67 = NULL;
thread_info *__thread_67 = NULL;



void save_peek_buffer__67(object_write_buffer *buf);
void load_peek_buffer__67(object_write_buffer *buf);
void save_file_pointer__67(object_write_buffer *buf);
void load_file_pointer__67(object_write_buffer *buf);

 
void init_AnonFilter_a0__387_74__67();
inline void check_status__67();

void work_AnonFilter_a0__387_74__67(int);


inline float __pop__67() {
float res=BUFFER_1_67[TAIL_1_67];
TAIL_1_67++;
return res;
}

inline float __pop__67(int n) {
float res=BUFFER_1_67[TAIL_1_67];
TAIL_1_67+=n;

return res;
}

inline float __peek__67(int offs) {
return BUFFER_1_67[TAIL_1_67+offs];
}



inline void __push__67(float data) {
BUFFER_67_3[HEAD_67_3]=data;
HEAD_67_3++;
}



 
void init_AnonFilter_a0__387_74__67(){
}
void save_file_pointer__67(object_write_buffer *buf) {}
void load_file_pointer__67(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__387_74__67__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__384 = 0.0f;/* float */
  int i__conflict__0__385 = 0;/* int */
  int i__386 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__384 = ((float)0.0))/*float*/;
  for ((i__conflict__0__385 = 0)/*int*/; (i__conflict__0__385 < -31); (i__conflict__0__385++)) {{
      (sum__384 = (sum__384 + ((*(____in+i__conflict__0__385)) * (*(____in+(i__conflict__0__385 + 63))))))/*float*/;
    }
  }
  for ((i__386 = 0)/*int*/; (i__386 < 32); (i__386++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__384);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__387_74__67__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__384 = 0.0f;/* float */
  int i__conflict__0__385 = 0;/* int */
  int i__386 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__384 = ((float)0.0))/*float*/;
  for ((i__conflict__0__385 = 0)/*int*/; (i__conflict__0__385 < -31); (i__conflict__0__385++)) {{
      (sum__384 = (sum__384 + ((*(____in+i__conflict__0__385)) * (*(____in+(i__conflict__0__385 + 63))))))/*float*/;
    }
  }
  for ((i__386 = 0)/*int*/; (i__386 < 32); (i__386++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__384);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__387_74__67(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__384 = 0.0f;/* float */
      int i__conflict__0__385 = 0;/* int */
      int i__386 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__384 = ((float)0.0))/*float*/;
      for ((i__conflict__0__385 = 0)/*int*/; (i__conflict__0__385 < -31); (i__conflict__0__385++)) {{
          (sum__384 = (sum__384 + (__peek__67(i__conflict__0__385) * __peek__67((i__conflict__0__385 + 63)))))/*float*/;
        }
      }
      for ((i__386 = 0)/*int*/; (i__386 < 32); (i__386++)) {{
          __pop__67();
        }
      }
      __push__67(sum__384);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_68;
int __counter_68 = 0;
int __steady_68 = 0;
int __tmp_68 = 0;
int __tmp2_68 = 0;
int *__state_flag_68 = NULL;
thread_info *__thread_68 = NULL;



void save_peek_buffer__68(object_write_buffer *buf);
void load_peek_buffer__68(object_write_buffer *buf);
void save_file_pointer__68(object_write_buffer *buf);
void load_file_pointer__68(object_write_buffer *buf);

 
void init_AnonFilter_a0__393_75__68();
inline void check_status__68();

void work_AnonFilter_a0__393_75__68(int);


inline float __pop__68() {
float res=BUFFER_1_68[TAIL_1_68];
TAIL_1_68++;
return res;
}

inline float __pop__68(int n) {
float res=BUFFER_1_68[TAIL_1_68];
TAIL_1_68+=n;

return res;
}

inline float __peek__68(int offs) {
return BUFFER_1_68[TAIL_1_68+offs];
}



inline void __push__68(float data) {
BUFFER_68_3[HEAD_68_3]=data;
HEAD_68_3++;
}



 
void init_AnonFilter_a0__393_75__68(){
}
void save_file_pointer__68(object_write_buffer *buf) {}
void load_file_pointer__68(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__393_75__68__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__390 = 0.0f;/* float */
  int i__conflict__0__391 = 0;/* int */
  int i__392 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__390 = ((float)0.0))/*float*/;
  for ((i__conflict__0__391 = 0)/*int*/; (i__conflict__0__391 < -32); (i__conflict__0__391++)) {{
      (sum__390 = (sum__390 + ((*(____in+i__conflict__0__391)) * (*(____in+(i__conflict__0__391 + 64))))))/*float*/;
    }
  }
  for ((i__392 = 0)/*int*/; (i__392 < 32); (i__392++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__390);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__393_75__68__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__390 = 0.0f;/* float */
  int i__conflict__0__391 = 0;/* int */
  int i__392 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__390 = ((float)0.0))/*float*/;
  for ((i__conflict__0__391 = 0)/*int*/; (i__conflict__0__391 < -32); (i__conflict__0__391++)) {{
      (sum__390 = (sum__390 + ((*(____in+i__conflict__0__391)) * (*(____in+(i__conflict__0__391 + 64))))))/*float*/;
    }
  }
  for ((i__392 = 0)/*int*/; (i__392 < 32); (i__392++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__390);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__393_75__68(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__390 = 0.0f;/* float */
      int i__conflict__0__391 = 0;/* int */
      int i__392 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__390 = ((float)0.0))/*float*/;
      for ((i__conflict__0__391 = 0)/*int*/; (i__conflict__0__391 < -32); (i__conflict__0__391++)) {{
          (sum__390 = (sum__390 + (__peek__68(i__conflict__0__391) * __peek__68((i__conflict__0__391 + 64)))))/*float*/;
        }
      }
      for ((i__392 = 0)/*int*/; (i__392 < 32); (i__392++)) {{
          __pop__68();
        }
      }
      __push__68(sum__390);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_69;
int __counter_69 = 0;
int __steady_69 = 0;
int __tmp_69 = 0;
int __tmp2_69 = 0;
int *__state_flag_69 = NULL;
thread_info *__thread_69 = NULL;



void save_peek_buffer__69(object_write_buffer *buf);
void load_peek_buffer__69(object_write_buffer *buf);
void save_file_pointer__69(object_write_buffer *buf);
void load_file_pointer__69(object_write_buffer *buf);

 
void init_AnonFilter_a0__399_76__69();
inline void check_status__69();

void work_AnonFilter_a0__399_76__69(int);


inline float __pop__69() {
float res=BUFFER_1_69[TAIL_1_69];
TAIL_1_69++;
return res;
}

inline float __pop__69(int n) {
float res=BUFFER_1_69[TAIL_1_69];
TAIL_1_69+=n;

return res;
}

inline float __peek__69(int offs) {
return BUFFER_1_69[TAIL_1_69+offs];
}



inline void __push__69(float data) {
BUFFER_69_3[HEAD_69_3]=data;
HEAD_69_3++;
}



 
void init_AnonFilter_a0__399_76__69(){
}
void save_file_pointer__69(object_write_buffer *buf) {}
void load_file_pointer__69(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__399_76__69__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__396 = 0.0f;/* float */
  int i__conflict__0__397 = 0;/* int */
  int i__398 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__396 = ((float)0.0))/*float*/;
  for ((i__conflict__0__397 = 0)/*int*/; (i__conflict__0__397 < -33); (i__conflict__0__397++)) {{
      (sum__396 = (sum__396 + ((*(____in+i__conflict__0__397)) * (*(____in+(i__conflict__0__397 + 65))))))/*float*/;
    }
  }
  for ((i__398 = 0)/*int*/; (i__398 < 32); (i__398++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__396);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__399_76__69__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__396 = 0.0f;/* float */
  int i__conflict__0__397 = 0;/* int */
  int i__398 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__396 = ((float)0.0))/*float*/;
  for ((i__conflict__0__397 = 0)/*int*/; (i__conflict__0__397 < -33); (i__conflict__0__397++)) {{
      (sum__396 = (sum__396 + ((*(____in+i__conflict__0__397)) * (*(____in+(i__conflict__0__397 + 65))))))/*float*/;
    }
  }
  for ((i__398 = 0)/*int*/; (i__398 < 32); (i__398++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__396);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__399_76__69(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__396 = 0.0f;/* float */
      int i__conflict__0__397 = 0;/* int */
      int i__398 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__396 = ((float)0.0))/*float*/;
      for ((i__conflict__0__397 = 0)/*int*/; (i__conflict__0__397 < -33); (i__conflict__0__397++)) {{
          (sum__396 = (sum__396 + (__peek__69(i__conflict__0__397) * __peek__69((i__conflict__0__397 + 65)))))/*float*/;
        }
      }
      for ((i__398 = 0)/*int*/; (i__398 < 32); (i__398++)) {{
          __pop__69();
        }
      }
      __push__69(sum__396);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_70;
int __counter_70 = 0;
int __steady_70 = 0;
int __tmp_70 = 0;
int __tmp2_70 = 0;
int *__state_flag_70 = NULL;
thread_info *__thread_70 = NULL;



void save_peek_buffer__70(object_write_buffer *buf);
void load_peek_buffer__70(object_write_buffer *buf);
void save_file_pointer__70(object_write_buffer *buf);
void load_file_pointer__70(object_write_buffer *buf);

 
void init_AnonFilter_a0__405_77__70();
inline void check_status__70();

void work_AnonFilter_a0__405_77__70(int);


inline float __pop__70() {
float res=BUFFER_1_70[TAIL_1_70];
TAIL_1_70++;
return res;
}

inline float __pop__70(int n) {
float res=BUFFER_1_70[TAIL_1_70];
TAIL_1_70+=n;

return res;
}

inline float __peek__70(int offs) {
return BUFFER_1_70[TAIL_1_70+offs];
}



inline void __push__70(float data) {
BUFFER_70_3[HEAD_70_3]=data;
HEAD_70_3++;
}



 
void init_AnonFilter_a0__405_77__70(){
}
void save_file_pointer__70(object_write_buffer *buf) {}
void load_file_pointer__70(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__405_77__70__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__402 = 0.0f;/* float */
  int i__conflict__0__403 = 0;/* int */
  int i__404 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__402 = ((float)0.0))/*float*/;
  for ((i__conflict__0__403 = 0)/*int*/; (i__conflict__0__403 < -34); (i__conflict__0__403++)) {{
      (sum__402 = (sum__402 + ((*(____in+i__conflict__0__403)) * (*(____in+(i__conflict__0__403 + 66))))))/*float*/;
    }
  }
  for ((i__404 = 0)/*int*/; (i__404 < 32); (i__404++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__402);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__405_77__70__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__402 = 0.0f;/* float */
  int i__conflict__0__403 = 0;/* int */
  int i__404 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__402 = ((float)0.0))/*float*/;
  for ((i__conflict__0__403 = 0)/*int*/; (i__conflict__0__403 < -34); (i__conflict__0__403++)) {{
      (sum__402 = (sum__402 + ((*(____in+i__conflict__0__403)) * (*(____in+(i__conflict__0__403 + 66))))))/*float*/;
    }
  }
  for ((i__404 = 0)/*int*/; (i__404 < 32); (i__404++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__402);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__405_77__70(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__402 = 0.0f;/* float */
      int i__conflict__0__403 = 0;/* int */
      int i__404 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__402 = ((float)0.0))/*float*/;
      for ((i__conflict__0__403 = 0)/*int*/; (i__conflict__0__403 < -34); (i__conflict__0__403++)) {{
          (sum__402 = (sum__402 + (__peek__70(i__conflict__0__403) * __peek__70((i__conflict__0__403 + 66)))))/*float*/;
        }
      }
      for ((i__404 = 0)/*int*/; (i__404 < 32); (i__404++)) {{
          __pop__70();
        }
      }
      __push__70(sum__402);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_71;
int __counter_71 = 0;
int __steady_71 = 0;
int __tmp_71 = 0;
int __tmp2_71 = 0;
int *__state_flag_71 = NULL;
thread_info *__thread_71 = NULL;



void save_peek_buffer__71(object_write_buffer *buf);
void load_peek_buffer__71(object_write_buffer *buf);
void save_file_pointer__71(object_write_buffer *buf);
void load_file_pointer__71(object_write_buffer *buf);

 
void init_AnonFilter_a0__411_78__71();
inline void check_status__71();

void work_AnonFilter_a0__411_78__71(int);


inline float __pop__71() {
float res=BUFFER_1_71[TAIL_1_71];
TAIL_1_71++;
return res;
}

inline float __pop__71(int n) {
float res=BUFFER_1_71[TAIL_1_71];
TAIL_1_71+=n;

return res;
}

inline float __peek__71(int offs) {
return BUFFER_1_71[TAIL_1_71+offs];
}



inline void __push__71(float data) {
BUFFER_71_3[HEAD_71_3]=data;
HEAD_71_3++;
}



 
void init_AnonFilter_a0__411_78__71(){
}
void save_file_pointer__71(object_write_buffer *buf) {}
void load_file_pointer__71(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__411_78__71__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__408 = 0.0f;/* float */
  int i__conflict__0__409 = 0;/* int */
  int i__410 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__408 = ((float)0.0))/*float*/;
  for ((i__conflict__0__409 = 0)/*int*/; (i__conflict__0__409 < -35); (i__conflict__0__409++)) {{
      (sum__408 = (sum__408 + ((*(____in+i__conflict__0__409)) * (*(____in+(i__conflict__0__409 + 67))))))/*float*/;
    }
  }
  for ((i__410 = 0)/*int*/; (i__410 < 32); (i__410++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__408);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__411_78__71__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__408 = 0.0f;/* float */
  int i__conflict__0__409 = 0;/* int */
  int i__410 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__408 = ((float)0.0))/*float*/;
  for ((i__conflict__0__409 = 0)/*int*/; (i__conflict__0__409 < -35); (i__conflict__0__409++)) {{
      (sum__408 = (sum__408 + ((*(____in+i__conflict__0__409)) * (*(____in+(i__conflict__0__409 + 67))))))/*float*/;
    }
  }
  for ((i__410 = 0)/*int*/; (i__410 < 32); (i__410++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__408);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__411_78__71(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__408 = 0.0f;/* float */
      int i__conflict__0__409 = 0;/* int */
      int i__410 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__408 = ((float)0.0))/*float*/;
      for ((i__conflict__0__409 = 0)/*int*/; (i__conflict__0__409 < -35); (i__conflict__0__409++)) {{
          (sum__408 = (sum__408 + (__peek__71(i__conflict__0__409) * __peek__71((i__conflict__0__409 + 67)))))/*float*/;
        }
      }
      for ((i__410 = 0)/*int*/; (i__410 < 32); (i__410++)) {{
          __pop__71();
        }
      }
      __push__71(sum__408);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_72;
int __counter_72 = 0;
int __steady_72 = 0;
int __tmp_72 = 0;
int __tmp2_72 = 0;
int *__state_flag_72 = NULL;
thread_info *__thread_72 = NULL;



void save_peek_buffer__72(object_write_buffer *buf);
void load_peek_buffer__72(object_write_buffer *buf);
void save_file_pointer__72(object_write_buffer *buf);
void load_file_pointer__72(object_write_buffer *buf);

 
void init_AnonFilter_a0__417_79__72();
inline void check_status__72();

void work_AnonFilter_a0__417_79__72(int);


inline float __pop__72() {
float res=BUFFER_1_72[TAIL_1_72];
TAIL_1_72++;
return res;
}

inline float __pop__72(int n) {
float res=BUFFER_1_72[TAIL_1_72];
TAIL_1_72+=n;

return res;
}

inline float __peek__72(int offs) {
return BUFFER_1_72[TAIL_1_72+offs];
}



inline void __push__72(float data) {
BUFFER_72_3[HEAD_72_3]=data;
HEAD_72_3++;
}



 
void init_AnonFilter_a0__417_79__72(){
}
void save_file_pointer__72(object_write_buffer *buf) {}
void load_file_pointer__72(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__417_79__72__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__414 = 0.0f;/* float */
  int i__conflict__0__415 = 0;/* int */
  int i__416 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__414 = ((float)0.0))/*float*/;
  for ((i__conflict__0__415 = 0)/*int*/; (i__conflict__0__415 < -36); (i__conflict__0__415++)) {{
      (sum__414 = (sum__414 + ((*(____in+i__conflict__0__415)) * (*(____in+(i__conflict__0__415 + 68))))))/*float*/;
    }
  }
  for ((i__416 = 0)/*int*/; (i__416 < 32); (i__416++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__414);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__417_79__72__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__414 = 0.0f;/* float */
  int i__conflict__0__415 = 0;/* int */
  int i__416 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__414 = ((float)0.0))/*float*/;
  for ((i__conflict__0__415 = 0)/*int*/; (i__conflict__0__415 < -36); (i__conflict__0__415++)) {{
      (sum__414 = (sum__414 + ((*(____in+i__conflict__0__415)) * (*(____in+(i__conflict__0__415 + 68))))))/*float*/;
    }
  }
  for ((i__416 = 0)/*int*/; (i__416 < 32); (i__416++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__414);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__417_79__72(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__414 = 0.0f;/* float */
      int i__conflict__0__415 = 0;/* int */
      int i__416 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__414 = ((float)0.0))/*float*/;
      for ((i__conflict__0__415 = 0)/*int*/; (i__conflict__0__415 < -36); (i__conflict__0__415++)) {{
          (sum__414 = (sum__414 + (__peek__72(i__conflict__0__415) * __peek__72((i__conflict__0__415 + 68)))))/*float*/;
        }
      }
      for ((i__416 = 0)/*int*/; (i__416 < 32); (i__416++)) {{
          __pop__72();
        }
      }
      __push__72(sum__414);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_73;
int __counter_73 = 0;
int __steady_73 = 0;
int __tmp_73 = 0;
int __tmp2_73 = 0;
int *__state_flag_73 = NULL;
thread_info *__thread_73 = NULL;



void save_peek_buffer__73(object_write_buffer *buf);
void load_peek_buffer__73(object_write_buffer *buf);
void save_file_pointer__73(object_write_buffer *buf);
void load_file_pointer__73(object_write_buffer *buf);

 
void init_AnonFilter_a0__423_80__73();
inline void check_status__73();

void work_AnonFilter_a0__423_80__73(int);


inline float __pop__73() {
float res=BUFFER_1_73[TAIL_1_73];
TAIL_1_73++;
return res;
}

inline float __pop__73(int n) {
float res=BUFFER_1_73[TAIL_1_73];
TAIL_1_73+=n;

return res;
}

inline float __peek__73(int offs) {
return BUFFER_1_73[TAIL_1_73+offs];
}



inline void __push__73(float data) {
BUFFER_73_3[HEAD_73_3]=data;
HEAD_73_3++;
}



 
void init_AnonFilter_a0__423_80__73(){
}
void save_file_pointer__73(object_write_buffer *buf) {}
void load_file_pointer__73(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__423_80__73__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__420 = 0.0f;/* float */
  int i__conflict__0__421 = 0;/* int */
  int i__422 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__420 = ((float)0.0))/*float*/;
  for ((i__conflict__0__421 = 0)/*int*/; (i__conflict__0__421 < -37); (i__conflict__0__421++)) {{
      (sum__420 = (sum__420 + ((*(____in+i__conflict__0__421)) * (*(____in+(i__conflict__0__421 + 69))))))/*float*/;
    }
  }
  for ((i__422 = 0)/*int*/; (i__422 < 32); (i__422++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__420);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__423_80__73__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__420 = 0.0f;/* float */
  int i__conflict__0__421 = 0;/* int */
  int i__422 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__420 = ((float)0.0))/*float*/;
  for ((i__conflict__0__421 = 0)/*int*/; (i__conflict__0__421 < -37); (i__conflict__0__421++)) {{
      (sum__420 = (sum__420 + ((*(____in+i__conflict__0__421)) * (*(____in+(i__conflict__0__421 + 69))))))/*float*/;
    }
  }
  for ((i__422 = 0)/*int*/; (i__422 < 32); (i__422++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__420);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__423_80__73(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__420 = 0.0f;/* float */
      int i__conflict__0__421 = 0;/* int */
      int i__422 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__420 = ((float)0.0))/*float*/;
      for ((i__conflict__0__421 = 0)/*int*/; (i__conflict__0__421 < -37); (i__conflict__0__421++)) {{
          (sum__420 = (sum__420 + (__peek__73(i__conflict__0__421) * __peek__73((i__conflict__0__421 + 69)))))/*float*/;
        }
      }
      for ((i__422 = 0)/*int*/; (i__422 < 32); (i__422++)) {{
          __pop__73();
        }
      }
      __push__73(sum__420);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_74;
int __counter_74 = 0;
int __steady_74 = 0;
int __tmp_74 = 0;
int __tmp2_74 = 0;
int *__state_flag_74 = NULL;
thread_info *__thread_74 = NULL;



void save_peek_buffer__74(object_write_buffer *buf);
void load_peek_buffer__74(object_write_buffer *buf);
void save_file_pointer__74(object_write_buffer *buf);
void load_file_pointer__74(object_write_buffer *buf);

 
void init_AnonFilter_a0__429_81__74();
inline void check_status__74();

void work_AnonFilter_a0__429_81__74(int);


inline float __pop__74() {
float res=BUFFER_1_74[TAIL_1_74];
TAIL_1_74++;
return res;
}

inline float __pop__74(int n) {
float res=BUFFER_1_74[TAIL_1_74];
TAIL_1_74+=n;

return res;
}

inline float __peek__74(int offs) {
return BUFFER_1_74[TAIL_1_74+offs];
}



inline void __push__74(float data) {
BUFFER_74_3[HEAD_74_3]=data;
HEAD_74_3++;
}



 
void init_AnonFilter_a0__429_81__74(){
}
void save_file_pointer__74(object_write_buffer *buf) {}
void load_file_pointer__74(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__429_81__74__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__426 = 0.0f;/* float */
  int i__conflict__0__427 = 0;/* int */
  int i__428 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__426 = ((float)0.0))/*float*/;
  for ((i__conflict__0__427 = 0)/*int*/; (i__conflict__0__427 < -38); (i__conflict__0__427++)) {{
      (sum__426 = (sum__426 + ((*(____in+i__conflict__0__427)) * (*(____in+(i__conflict__0__427 + 70))))))/*float*/;
    }
  }
  for ((i__428 = 0)/*int*/; (i__428 < 32); (i__428++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__426);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__429_81__74__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__426 = 0.0f;/* float */
  int i__conflict__0__427 = 0;/* int */
  int i__428 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__426 = ((float)0.0))/*float*/;
  for ((i__conflict__0__427 = 0)/*int*/; (i__conflict__0__427 < -38); (i__conflict__0__427++)) {{
      (sum__426 = (sum__426 + ((*(____in+i__conflict__0__427)) * (*(____in+(i__conflict__0__427 + 70))))))/*float*/;
    }
  }
  for ((i__428 = 0)/*int*/; (i__428 < 32); (i__428++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__426);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__429_81__74(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__426 = 0.0f;/* float */
      int i__conflict__0__427 = 0;/* int */
      int i__428 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__426 = ((float)0.0))/*float*/;
      for ((i__conflict__0__427 = 0)/*int*/; (i__conflict__0__427 < -38); (i__conflict__0__427++)) {{
          (sum__426 = (sum__426 + (__peek__74(i__conflict__0__427) * __peek__74((i__conflict__0__427 + 70)))))/*float*/;
        }
      }
      for ((i__428 = 0)/*int*/; (i__428 < 32); (i__428++)) {{
          __pop__74();
        }
      }
      __push__74(sum__426);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_75;
int __counter_75 = 0;
int __steady_75 = 0;
int __tmp_75 = 0;
int __tmp2_75 = 0;
int *__state_flag_75 = NULL;
thread_info *__thread_75 = NULL;



void save_peek_buffer__75(object_write_buffer *buf);
void load_peek_buffer__75(object_write_buffer *buf);
void save_file_pointer__75(object_write_buffer *buf);
void load_file_pointer__75(object_write_buffer *buf);

 
void init_AnonFilter_a0__435_82__75();
inline void check_status__75();

void work_AnonFilter_a0__435_82__75(int);


inline float __pop__75() {
float res=BUFFER_1_75[TAIL_1_75];
TAIL_1_75++;
return res;
}

inline float __pop__75(int n) {
float res=BUFFER_1_75[TAIL_1_75];
TAIL_1_75+=n;

return res;
}

inline float __peek__75(int offs) {
return BUFFER_1_75[TAIL_1_75+offs];
}



inline void __push__75(float data) {
BUFFER_75_3[HEAD_75_3]=data;
HEAD_75_3++;
}



 
void init_AnonFilter_a0__435_82__75(){
}
void save_file_pointer__75(object_write_buffer *buf) {}
void load_file_pointer__75(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__435_82__75__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__432 = 0.0f;/* float */
  int i__conflict__0__433 = 0;/* int */
  int i__434 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__432 = ((float)0.0))/*float*/;
  for ((i__conflict__0__433 = 0)/*int*/; (i__conflict__0__433 < -39); (i__conflict__0__433++)) {{
      (sum__432 = (sum__432 + ((*(____in+i__conflict__0__433)) * (*(____in+(i__conflict__0__433 + 71))))))/*float*/;
    }
  }
  for ((i__434 = 0)/*int*/; (i__434 < 32); (i__434++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__432);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__435_82__75__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__432 = 0.0f;/* float */
  int i__conflict__0__433 = 0;/* int */
  int i__434 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__432 = ((float)0.0))/*float*/;
  for ((i__conflict__0__433 = 0)/*int*/; (i__conflict__0__433 < -39); (i__conflict__0__433++)) {{
      (sum__432 = (sum__432 + ((*(____in+i__conflict__0__433)) * (*(____in+(i__conflict__0__433 + 71))))))/*float*/;
    }
  }
  for ((i__434 = 0)/*int*/; (i__434 < 32); (i__434++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__432);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__435_82__75(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__432 = 0.0f;/* float */
      int i__conflict__0__433 = 0;/* int */
      int i__434 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__432 = ((float)0.0))/*float*/;
      for ((i__conflict__0__433 = 0)/*int*/; (i__conflict__0__433 < -39); (i__conflict__0__433++)) {{
          (sum__432 = (sum__432 + (__peek__75(i__conflict__0__433) * __peek__75((i__conflict__0__433 + 71)))))/*float*/;
        }
      }
      for ((i__434 = 0)/*int*/; (i__434 < 32); (i__434++)) {{
          __pop__75();
        }
      }
      __push__75(sum__432);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_76;
int __counter_76 = 0;
int __steady_76 = 0;
int __tmp_76 = 0;
int __tmp2_76 = 0;
int *__state_flag_76 = NULL;
thread_info *__thread_76 = NULL;



void save_peek_buffer__76(object_write_buffer *buf);
void load_peek_buffer__76(object_write_buffer *buf);
void save_file_pointer__76(object_write_buffer *buf);
void load_file_pointer__76(object_write_buffer *buf);

 
void init_AnonFilter_a0__441_83__76();
inline void check_status__76();

void work_AnonFilter_a0__441_83__76(int);


inline float __pop__76() {
float res=BUFFER_1_76[TAIL_1_76];
TAIL_1_76++;
return res;
}

inline float __pop__76(int n) {
float res=BUFFER_1_76[TAIL_1_76];
TAIL_1_76+=n;

return res;
}

inline float __peek__76(int offs) {
return BUFFER_1_76[TAIL_1_76+offs];
}



inline void __push__76(float data) {
BUFFER_76_3[HEAD_76_3]=data;
HEAD_76_3++;
}



 
void init_AnonFilter_a0__441_83__76(){
}
void save_file_pointer__76(object_write_buffer *buf) {}
void load_file_pointer__76(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__441_83__76__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__438 = 0.0f;/* float */
  int i__conflict__0__439 = 0;/* int */
  int i__440 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__438 = ((float)0.0))/*float*/;
  for ((i__conflict__0__439 = 0)/*int*/; (i__conflict__0__439 < -40); (i__conflict__0__439++)) {{
      (sum__438 = (sum__438 + ((*(____in+i__conflict__0__439)) * (*(____in+(i__conflict__0__439 + 72))))))/*float*/;
    }
  }
  for ((i__440 = 0)/*int*/; (i__440 < 32); (i__440++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__438);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__441_83__76__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__438 = 0.0f;/* float */
  int i__conflict__0__439 = 0;/* int */
  int i__440 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__438 = ((float)0.0))/*float*/;
  for ((i__conflict__0__439 = 0)/*int*/; (i__conflict__0__439 < -40); (i__conflict__0__439++)) {{
      (sum__438 = (sum__438 + ((*(____in+i__conflict__0__439)) * (*(____in+(i__conflict__0__439 + 72))))))/*float*/;
    }
  }
  for ((i__440 = 0)/*int*/; (i__440 < 32); (i__440++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__438);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__441_83__76(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__438 = 0.0f;/* float */
      int i__conflict__0__439 = 0;/* int */
      int i__440 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__438 = ((float)0.0))/*float*/;
      for ((i__conflict__0__439 = 0)/*int*/; (i__conflict__0__439 < -40); (i__conflict__0__439++)) {{
          (sum__438 = (sum__438 + (__peek__76(i__conflict__0__439) * __peek__76((i__conflict__0__439 + 72)))))/*float*/;
        }
      }
      for ((i__440 = 0)/*int*/; (i__440 < 32); (i__440++)) {{
          __pop__76();
        }
      }
      __push__76(sum__438);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_77;
int __counter_77 = 0;
int __steady_77 = 0;
int __tmp_77 = 0;
int __tmp2_77 = 0;
int *__state_flag_77 = NULL;
thread_info *__thread_77 = NULL;



void save_peek_buffer__77(object_write_buffer *buf);
void load_peek_buffer__77(object_write_buffer *buf);
void save_file_pointer__77(object_write_buffer *buf);
void load_file_pointer__77(object_write_buffer *buf);

 
void init_AnonFilter_a0__447_84__77();
inline void check_status__77();

void work_AnonFilter_a0__447_84__77(int);


inline float __pop__77() {
float res=BUFFER_1_77[TAIL_1_77];
TAIL_1_77++;
return res;
}

inline float __pop__77(int n) {
float res=BUFFER_1_77[TAIL_1_77];
TAIL_1_77+=n;

return res;
}

inline float __peek__77(int offs) {
return BUFFER_1_77[TAIL_1_77+offs];
}



inline void __push__77(float data) {
BUFFER_77_3[HEAD_77_3]=data;
HEAD_77_3++;
}



 
void init_AnonFilter_a0__447_84__77(){
}
void save_file_pointer__77(object_write_buffer *buf) {}
void load_file_pointer__77(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__447_84__77__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__444 = 0.0f;/* float */
  int i__conflict__0__445 = 0;/* int */
  int i__446 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__444 = ((float)0.0))/*float*/;
  for ((i__conflict__0__445 = 0)/*int*/; (i__conflict__0__445 < -41); (i__conflict__0__445++)) {{
      (sum__444 = (sum__444 + ((*(____in+i__conflict__0__445)) * (*(____in+(i__conflict__0__445 + 73))))))/*float*/;
    }
  }
  for ((i__446 = 0)/*int*/; (i__446 < 32); (i__446++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__444);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__447_84__77__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__444 = 0.0f;/* float */
  int i__conflict__0__445 = 0;/* int */
  int i__446 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__444 = ((float)0.0))/*float*/;
  for ((i__conflict__0__445 = 0)/*int*/; (i__conflict__0__445 < -41); (i__conflict__0__445++)) {{
      (sum__444 = (sum__444 + ((*(____in+i__conflict__0__445)) * (*(____in+(i__conflict__0__445 + 73))))))/*float*/;
    }
  }
  for ((i__446 = 0)/*int*/; (i__446 < 32); (i__446++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__444);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__447_84__77(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__444 = 0.0f;/* float */
      int i__conflict__0__445 = 0;/* int */
      int i__446 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__444 = ((float)0.0))/*float*/;
      for ((i__conflict__0__445 = 0)/*int*/; (i__conflict__0__445 < -41); (i__conflict__0__445++)) {{
          (sum__444 = (sum__444 + (__peek__77(i__conflict__0__445) * __peek__77((i__conflict__0__445 + 73)))))/*float*/;
        }
      }
      for ((i__446 = 0)/*int*/; (i__446 < 32); (i__446++)) {{
          __pop__77();
        }
      }
      __push__77(sum__444);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_78;
int __counter_78 = 0;
int __steady_78 = 0;
int __tmp_78 = 0;
int __tmp2_78 = 0;
int *__state_flag_78 = NULL;
thread_info *__thread_78 = NULL;



void save_peek_buffer__78(object_write_buffer *buf);
void load_peek_buffer__78(object_write_buffer *buf);
void save_file_pointer__78(object_write_buffer *buf);
void load_file_pointer__78(object_write_buffer *buf);

 
void init_AnonFilter_a0__453_85__78();
inline void check_status__78();

void work_AnonFilter_a0__453_85__78(int);


inline float __pop__78() {
float res=BUFFER_1_78[TAIL_1_78];
TAIL_1_78++;
return res;
}

inline float __pop__78(int n) {
float res=BUFFER_1_78[TAIL_1_78];
TAIL_1_78+=n;

return res;
}

inline float __peek__78(int offs) {
return BUFFER_1_78[TAIL_1_78+offs];
}



inline void __push__78(float data) {
BUFFER_78_3[HEAD_78_3]=data;
HEAD_78_3++;
}



 
void init_AnonFilter_a0__453_85__78(){
}
void save_file_pointer__78(object_write_buffer *buf) {}
void load_file_pointer__78(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__453_85__78__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__450 = 0.0f;/* float */
  int i__conflict__0__451 = 0;/* int */
  int i__452 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__450 = ((float)0.0))/*float*/;
  for ((i__conflict__0__451 = 0)/*int*/; (i__conflict__0__451 < -42); (i__conflict__0__451++)) {{
      (sum__450 = (sum__450 + ((*(____in+i__conflict__0__451)) * (*(____in+(i__conflict__0__451 + 74))))))/*float*/;
    }
  }
  for ((i__452 = 0)/*int*/; (i__452 < 32); (i__452++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__450);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__453_85__78__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__450 = 0.0f;/* float */
  int i__conflict__0__451 = 0;/* int */
  int i__452 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__450 = ((float)0.0))/*float*/;
  for ((i__conflict__0__451 = 0)/*int*/; (i__conflict__0__451 < -42); (i__conflict__0__451++)) {{
      (sum__450 = (sum__450 + ((*(____in+i__conflict__0__451)) * (*(____in+(i__conflict__0__451 + 74))))))/*float*/;
    }
  }
  for ((i__452 = 0)/*int*/; (i__452 < 32); (i__452++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__450);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__453_85__78(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__450 = 0.0f;/* float */
      int i__conflict__0__451 = 0;/* int */
      int i__452 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__450 = ((float)0.0))/*float*/;
      for ((i__conflict__0__451 = 0)/*int*/; (i__conflict__0__451 < -42); (i__conflict__0__451++)) {{
          (sum__450 = (sum__450 + (__peek__78(i__conflict__0__451) * __peek__78((i__conflict__0__451 + 74)))))/*float*/;
        }
      }
      for ((i__452 = 0)/*int*/; (i__452 < 32); (i__452++)) {{
          __pop__78();
        }
      }
      __push__78(sum__450);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_79;
int __counter_79 = 0;
int __steady_79 = 0;
int __tmp_79 = 0;
int __tmp2_79 = 0;
int *__state_flag_79 = NULL;
thread_info *__thread_79 = NULL;



void save_peek_buffer__79(object_write_buffer *buf);
void load_peek_buffer__79(object_write_buffer *buf);
void save_file_pointer__79(object_write_buffer *buf);
void load_file_pointer__79(object_write_buffer *buf);

 
void init_AnonFilter_a0__459_86__79();
inline void check_status__79();

void work_AnonFilter_a0__459_86__79(int);


inline float __pop__79() {
float res=BUFFER_1_79[TAIL_1_79];
TAIL_1_79++;
return res;
}

inline float __pop__79(int n) {
float res=BUFFER_1_79[TAIL_1_79];
TAIL_1_79+=n;

return res;
}

inline float __peek__79(int offs) {
return BUFFER_1_79[TAIL_1_79+offs];
}



inline void __push__79(float data) {
BUFFER_79_3[HEAD_79_3]=data;
HEAD_79_3++;
}



 
void init_AnonFilter_a0__459_86__79(){
}
void save_file_pointer__79(object_write_buffer *buf) {}
void load_file_pointer__79(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__459_86__79__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__456 = 0.0f;/* float */
  int i__conflict__0__457 = 0;/* int */
  int i__458 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__456 = ((float)0.0))/*float*/;
  for ((i__conflict__0__457 = 0)/*int*/; (i__conflict__0__457 < -43); (i__conflict__0__457++)) {{
      (sum__456 = (sum__456 + ((*(____in+i__conflict__0__457)) * (*(____in+(i__conflict__0__457 + 75))))))/*float*/;
    }
  }
  for ((i__458 = 0)/*int*/; (i__458 < 32); (i__458++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__456);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__459_86__79__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__456 = 0.0f;/* float */
  int i__conflict__0__457 = 0;/* int */
  int i__458 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__456 = ((float)0.0))/*float*/;
  for ((i__conflict__0__457 = 0)/*int*/; (i__conflict__0__457 < -43); (i__conflict__0__457++)) {{
      (sum__456 = (sum__456 + ((*(____in+i__conflict__0__457)) * (*(____in+(i__conflict__0__457 + 75))))))/*float*/;
    }
  }
  for ((i__458 = 0)/*int*/; (i__458 < 32); (i__458++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__456);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__459_86__79(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__456 = 0.0f;/* float */
      int i__conflict__0__457 = 0;/* int */
      int i__458 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__456 = ((float)0.0))/*float*/;
      for ((i__conflict__0__457 = 0)/*int*/; (i__conflict__0__457 < -43); (i__conflict__0__457++)) {{
          (sum__456 = (sum__456 + (__peek__79(i__conflict__0__457) * __peek__79((i__conflict__0__457 + 75)))))/*float*/;
        }
      }
      for ((i__458 = 0)/*int*/; (i__458 < 32); (i__458++)) {{
          __pop__79();
        }
      }
      __push__79(sum__456);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_80;
int __counter_80 = 0;
int __steady_80 = 0;
int __tmp_80 = 0;
int __tmp2_80 = 0;
int *__state_flag_80 = NULL;
thread_info *__thread_80 = NULL;



void save_peek_buffer__80(object_write_buffer *buf);
void load_peek_buffer__80(object_write_buffer *buf);
void save_file_pointer__80(object_write_buffer *buf);
void load_file_pointer__80(object_write_buffer *buf);

 
void init_AnonFilter_a0__465_87__80();
inline void check_status__80();

void work_AnonFilter_a0__465_87__80(int);


inline float __pop__80() {
float res=BUFFER_1_80[TAIL_1_80];
TAIL_1_80++;
return res;
}

inline float __pop__80(int n) {
float res=BUFFER_1_80[TAIL_1_80];
TAIL_1_80+=n;

return res;
}

inline float __peek__80(int offs) {
return BUFFER_1_80[TAIL_1_80+offs];
}



inline void __push__80(float data) {
BUFFER_80_3[HEAD_80_3]=data;
HEAD_80_3++;
}



 
void init_AnonFilter_a0__465_87__80(){
}
void save_file_pointer__80(object_write_buffer *buf) {}
void load_file_pointer__80(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__465_87__80__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__462 = 0.0f;/* float */
  int i__conflict__0__463 = 0;/* int */
  int i__464 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__462 = ((float)0.0))/*float*/;
  for ((i__conflict__0__463 = 0)/*int*/; (i__conflict__0__463 < -44); (i__conflict__0__463++)) {{
      (sum__462 = (sum__462 + ((*(____in+i__conflict__0__463)) * (*(____in+(i__conflict__0__463 + 76))))))/*float*/;
    }
  }
  for ((i__464 = 0)/*int*/; (i__464 < 32); (i__464++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__462);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__465_87__80__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__462 = 0.0f;/* float */
  int i__conflict__0__463 = 0;/* int */
  int i__464 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__462 = ((float)0.0))/*float*/;
  for ((i__conflict__0__463 = 0)/*int*/; (i__conflict__0__463 < -44); (i__conflict__0__463++)) {{
      (sum__462 = (sum__462 + ((*(____in+i__conflict__0__463)) * (*(____in+(i__conflict__0__463 + 76))))))/*float*/;
    }
  }
  for ((i__464 = 0)/*int*/; (i__464 < 32); (i__464++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__462);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__465_87__80(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__462 = 0.0f;/* float */
      int i__conflict__0__463 = 0;/* int */
      int i__464 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__462 = ((float)0.0))/*float*/;
      for ((i__conflict__0__463 = 0)/*int*/; (i__conflict__0__463 < -44); (i__conflict__0__463++)) {{
          (sum__462 = (sum__462 + (__peek__80(i__conflict__0__463) * __peek__80((i__conflict__0__463 + 76)))))/*float*/;
        }
      }
      for ((i__464 = 0)/*int*/; (i__464 < 32); (i__464++)) {{
          __pop__80();
        }
      }
      __push__80(sum__462);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_81;
int __counter_81 = 0;
int __steady_81 = 0;
int __tmp_81 = 0;
int __tmp2_81 = 0;
int *__state_flag_81 = NULL;
thread_info *__thread_81 = NULL;



void save_peek_buffer__81(object_write_buffer *buf);
void load_peek_buffer__81(object_write_buffer *buf);
void save_file_pointer__81(object_write_buffer *buf);
void load_file_pointer__81(object_write_buffer *buf);

 
void init_AnonFilter_a0__471_88__81();
inline void check_status__81();

void work_AnonFilter_a0__471_88__81(int);


inline float __pop__81() {
float res=BUFFER_1_81[TAIL_1_81];
TAIL_1_81++;
return res;
}

inline float __pop__81(int n) {
float res=BUFFER_1_81[TAIL_1_81];
TAIL_1_81+=n;

return res;
}

inline float __peek__81(int offs) {
return BUFFER_1_81[TAIL_1_81+offs];
}



inline void __push__81(float data) {
BUFFER_81_3[HEAD_81_3]=data;
HEAD_81_3++;
}



 
void init_AnonFilter_a0__471_88__81(){
}
void save_file_pointer__81(object_write_buffer *buf) {}
void load_file_pointer__81(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__471_88__81__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__468 = 0.0f;/* float */
  int i__conflict__0__469 = 0;/* int */
  int i__470 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__468 = ((float)0.0))/*float*/;
  for ((i__conflict__0__469 = 0)/*int*/; (i__conflict__0__469 < -45); (i__conflict__0__469++)) {{
      (sum__468 = (sum__468 + ((*(____in+i__conflict__0__469)) * (*(____in+(i__conflict__0__469 + 77))))))/*float*/;
    }
  }
  for ((i__470 = 0)/*int*/; (i__470 < 32); (i__470++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__468);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__471_88__81__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__468 = 0.0f;/* float */
  int i__conflict__0__469 = 0;/* int */
  int i__470 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__468 = ((float)0.0))/*float*/;
  for ((i__conflict__0__469 = 0)/*int*/; (i__conflict__0__469 < -45); (i__conflict__0__469++)) {{
      (sum__468 = (sum__468 + ((*(____in+i__conflict__0__469)) * (*(____in+(i__conflict__0__469 + 77))))))/*float*/;
    }
  }
  for ((i__470 = 0)/*int*/; (i__470 < 32); (i__470++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__468);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__471_88__81(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__468 = 0.0f;/* float */
      int i__conflict__0__469 = 0;/* int */
      int i__470 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__468 = ((float)0.0))/*float*/;
      for ((i__conflict__0__469 = 0)/*int*/; (i__conflict__0__469 < -45); (i__conflict__0__469++)) {{
          (sum__468 = (sum__468 + (__peek__81(i__conflict__0__469) * __peek__81((i__conflict__0__469 + 77)))))/*float*/;
        }
      }
      for ((i__470 = 0)/*int*/; (i__470 < 32); (i__470++)) {{
          __pop__81();
        }
      }
      __push__81(sum__468);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_82;
int __counter_82 = 0;
int __steady_82 = 0;
int __tmp_82 = 0;
int __tmp2_82 = 0;
int *__state_flag_82 = NULL;
thread_info *__thread_82 = NULL;



void save_peek_buffer__82(object_write_buffer *buf);
void load_peek_buffer__82(object_write_buffer *buf);
void save_file_pointer__82(object_write_buffer *buf);
void load_file_pointer__82(object_write_buffer *buf);

 
void init_AnonFilter_a0__477_89__82();
inline void check_status__82();

void work_AnonFilter_a0__477_89__82(int);


inline float __pop__82() {
float res=BUFFER_1_82[TAIL_1_82];
TAIL_1_82++;
return res;
}

inline float __pop__82(int n) {
float res=BUFFER_1_82[TAIL_1_82];
TAIL_1_82+=n;

return res;
}

inline float __peek__82(int offs) {
return BUFFER_1_82[TAIL_1_82+offs];
}



inline void __push__82(float data) {
BUFFER_82_3[HEAD_82_3]=data;
HEAD_82_3++;
}



 
void init_AnonFilter_a0__477_89__82(){
}
void save_file_pointer__82(object_write_buffer *buf) {}
void load_file_pointer__82(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__477_89__82__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__474 = 0.0f;/* float */
  int i__conflict__0__475 = 0;/* int */
  int i__476 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__474 = ((float)0.0))/*float*/;
  for ((i__conflict__0__475 = 0)/*int*/; (i__conflict__0__475 < -46); (i__conflict__0__475++)) {{
      (sum__474 = (sum__474 + ((*(____in+i__conflict__0__475)) * (*(____in+(i__conflict__0__475 + 78))))))/*float*/;
    }
  }
  for ((i__476 = 0)/*int*/; (i__476 < 32); (i__476++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__474);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__477_89__82__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__474 = 0.0f;/* float */
  int i__conflict__0__475 = 0;/* int */
  int i__476 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__474 = ((float)0.0))/*float*/;
  for ((i__conflict__0__475 = 0)/*int*/; (i__conflict__0__475 < -46); (i__conflict__0__475++)) {{
      (sum__474 = (sum__474 + ((*(____in+i__conflict__0__475)) * (*(____in+(i__conflict__0__475 + 78))))))/*float*/;
    }
  }
  for ((i__476 = 0)/*int*/; (i__476 < 32); (i__476++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__474);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__477_89__82(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__474 = 0.0f;/* float */
      int i__conflict__0__475 = 0;/* int */
      int i__476 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__474 = ((float)0.0))/*float*/;
      for ((i__conflict__0__475 = 0)/*int*/; (i__conflict__0__475 < -46); (i__conflict__0__475++)) {{
          (sum__474 = (sum__474 + (__peek__82(i__conflict__0__475) * __peek__82((i__conflict__0__475 + 78)))))/*float*/;
        }
      }
      for ((i__476 = 0)/*int*/; (i__476 < 32); (i__476++)) {{
          __pop__82();
        }
      }
      __push__82(sum__474);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_83;
int __counter_83 = 0;
int __steady_83 = 0;
int __tmp_83 = 0;
int __tmp2_83 = 0;
int *__state_flag_83 = NULL;
thread_info *__thread_83 = NULL;



void save_peek_buffer__83(object_write_buffer *buf);
void load_peek_buffer__83(object_write_buffer *buf);
void save_file_pointer__83(object_write_buffer *buf);
void load_file_pointer__83(object_write_buffer *buf);

 
void init_AnonFilter_a0__483_90__83();
inline void check_status__83();

void work_AnonFilter_a0__483_90__83(int);


inline float __pop__83() {
float res=BUFFER_1_83[TAIL_1_83];
TAIL_1_83++;
return res;
}

inline float __pop__83(int n) {
float res=BUFFER_1_83[TAIL_1_83];
TAIL_1_83+=n;

return res;
}

inline float __peek__83(int offs) {
return BUFFER_1_83[TAIL_1_83+offs];
}



inline void __push__83(float data) {
BUFFER_83_3[HEAD_83_3]=data;
HEAD_83_3++;
}



 
void init_AnonFilter_a0__483_90__83(){
}
void save_file_pointer__83(object_write_buffer *buf) {}
void load_file_pointer__83(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__483_90__83__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__480 = 0.0f;/* float */
  int i__conflict__0__481 = 0;/* int */
  int i__482 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__480 = ((float)0.0))/*float*/;
  for ((i__conflict__0__481 = 0)/*int*/; (i__conflict__0__481 < -47); (i__conflict__0__481++)) {{
      (sum__480 = (sum__480 + ((*(____in+i__conflict__0__481)) * (*(____in+(i__conflict__0__481 + 79))))))/*float*/;
    }
  }
  for ((i__482 = 0)/*int*/; (i__482 < 32); (i__482++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__480);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__483_90__83__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__480 = 0.0f;/* float */
  int i__conflict__0__481 = 0;/* int */
  int i__482 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__480 = ((float)0.0))/*float*/;
  for ((i__conflict__0__481 = 0)/*int*/; (i__conflict__0__481 < -47); (i__conflict__0__481++)) {{
      (sum__480 = (sum__480 + ((*(____in+i__conflict__0__481)) * (*(____in+(i__conflict__0__481 + 79))))))/*float*/;
    }
  }
  for ((i__482 = 0)/*int*/; (i__482 < 32); (i__482++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__480);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__483_90__83(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__480 = 0.0f;/* float */
      int i__conflict__0__481 = 0;/* int */
      int i__482 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__480 = ((float)0.0))/*float*/;
      for ((i__conflict__0__481 = 0)/*int*/; (i__conflict__0__481 < -47); (i__conflict__0__481++)) {{
          (sum__480 = (sum__480 + (__peek__83(i__conflict__0__481) * __peek__83((i__conflict__0__481 + 79)))))/*float*/;
        }
      }
      for ((i__482 = 0)/*int*/; (i__482 < 32); (i__482++)) {{
          __pop__83();
        }
      }
      __push__83(sum__480);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_84;
int __counter_84 = 0;
int __steady_84 = 0;
int __tmp_84 = 0;
int __tmp2_84 = 0;
int *__state_flag_84 = NULL;
thread_info *__thread_84 = NULL;



void save_peek_buffer__84(object_write_buffer *buf);
void load_peek_buffer__84(object_write_buffer *buf);
void save_file_pointer__84(object_write_buffer *buf);
void load_file_pointer__84(object_write_buffer *buf);

 
void init_AnonFilter_a0__489_91__84();
inline void check_status__84();

void work_AnonFilter_a0__489_91__84(int);


inline float __pop__84() {
float res=BUFFER_1_84[TAIL_1_84];
TAIL_1_84++;
return res;
}

inline float __pop__84(int n) {
float res=BUFFER_1_84[TAIL_1_84];
TAIL_1_84+=n;

return res;
}

inline float __peek__84(int offs) {
return BUFFER_1_84[TAIL_1_84+offs];
}



inline void __push__84(float data) {
BUFFER_84_3[HEAD_84_3]=data;
HEAD_84_3++;
}



 
void init_AnonFilter_a0__489_91__84(){
}
void save_file_pointer__84(object_write_buffer *buf) {}
void load_file_pointer__84(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__489_91__84__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__486 = 0.0f;/* float */
  int i__conflict__0__487 = 0;/* int */
  int i__488 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__486 = ((float)0.0))/*float*/;
  for ((i__conflict__0__487 = 0)/*int*/; (i__conflict__0__487 < -48); (i__conflict__0__487++)) {{
      (sum__486 = (sum__486 + ((*(____in+i__conflict__0__487)) * (*(____in+(i__conflict__0__487 + 80))))))/*float*/;
    }
  }
  for ((i__488 = 0)/*int*/; (i__488 < 32); (i__488++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__486);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__489_91__84__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__486 = 0.0f;/* float */
  int i__conflict__0__487 = 0;/* int */
  int i__488 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__486 = ((float)0.0))/*float*/;
  for ((i__conflict__0__487 = 0)/*int*/; (i__conflict__0__487 < -48); (i__conflict__0__487++)) {{
      (sum__486 = (sum__486 + ((*(____in+i__conflict__0__487)) * (*(____in+(i__conflict__0__487 + 80))))))/*float*/;
    }
  }
  for ((i__488 = 0)/*int*/; (i__488 < 32); (i__488++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__486);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__489_91__84(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__486 = 0.0f;/* float */
      int i__conflict__0__487 = 0;/* int */
      int i__488 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__486 = ((float)0.0))/*float*/;
      for ((i__conflict__0__487 = 0)/*int*/; (i__conflict__0__487 < -48); (i__conflict__0__487++)) {{
          (sum__486 = (sum__486 + (__peek__84(i__conflict__0__487) * __peek__84((i__conflict__0__487 + 80)))))/*float*/;
        }
      }
      for ((i__488 = 0)/*int*/; (i__488 < 32); (i__488++)) {{
          __pop__84();
        }
      }
      __push__84(sum__486);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_85;
int __counter_85 = 0;
int __steady_85 = 0;
int __tmp_85 = 0;
int __tmp2_85 = 0;
int *__state_flag_85 = NULL;
thread_info *__thread_85 = NULL;



void save_peek_buffer__85(object_write_buffer *buf);
void load_peek_buffer__85(object_write_buffer *buf);
void save_file_pointer__85(object_write_buffer *buf);
void load_file_pointer__85(object_write_buffer *buf);

 
void init_AnonFilter_a0__495_92__85();
inline void check_status__85();

void work_AnonFilter_a0__495_92__85(int);


inline float __pop__85() {
float res=BUFFER_1_85[TAIL_1_85];
TAIL_1_85++;
return res;
}

inline float __pop__85(int n) {
float res=BUFFER_1_85[TAIL_1_85];
TAIL_1_85+=n;

return res;
}

inline float __peek__85(int offs) {
return BUFFER_1_85[TAIL_1_85+offs];
}



inline void __push__85(float data) {
BUFFER_85_3[HEAD_85_3]=data;
HEAD_85_3++;
}



 
void init_AnonFilter_a0__495_92__85(){
}
void save_file_pointer__85(object_write_buffer *buf) {}
void load_file_pointer__85(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__495_92__85__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__492 = 0.0f;/* float */
  int i__conflict__0__493 = 0;/* int */
  int i__494 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__492 = ((float)0.0))/*float*/;
  for ((i__conflict__0__493 = 0)/*int*/; (i__conflict__0__493 < -49); (i__conflict__0__493++)) {{
      (sum__492 = (sum__492 + ((*(____in+i__conflict__0__493)) * (*(____in+(i__conflict__0__493 + 81))))))/*float*/;
    }
  }
  for ((i__494 = 0)/*int*/; (i__494 < 32); (i__494++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__492);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__495_92__85__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__492 = 0.0f;/* float */
  int i__conflict__0__493 = 0;/* int */
  int i__494 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__492 = ((float)0.0))/*float*/;
  for ((i__conflict__0__493 = 0)/*int*/; (i__conflict__0__493 < -49); (i__conflict__0__493++)) {{
      (sum__492 = (sum__492 + ((*(____in+i__conflict__0__493)) * (*(____in+(i__conflict__0__493 + 81))))))/*float*/;
    }
  }
  for ((i__494 = 0)/*int*/; (i__494 < 32); (i__494++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__492);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__495_92__85(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__492 = 0.0f;/* float */
      int i__conflict__0__493 = 0;/* int */
      int i__494 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__492 = ((float)0.0))/*float*/;
      for ((i__conflict__0__493 = 0)/*int*/; (i__conflict__0__493 < -49); (i__conflict__0__493++)) {{
          (sum__492 = (sum__492 + (__peek__85(i__conflict__0__493) * __peek__85((i__conflict__0__493 + 81)))))/*float*/;
        }
      }
      for ((i__494 = 0)/*int*/; (i__494 < 32); (i__494++)) {{
          __pop__85();
        }
      }
      __push__85(sum__492);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_86;
int __counter_86 = 0;
int __steady_86 = 0;
int __tmp_86 = 0;
int __tmp2_86 = 0;
int *__state_flag_86 = NULL;
thread_info *__thread_86 = NULL;



void save_peek_buffer__86(object_write_buffer *buf);
void load_peek_buffer__86(object_write_buffer *buf);
void save_file_pointer__86(object_write_buffer *buf);
void load_file_pointer__86(object_write_buffer *buf);

 
void init_AnonFilter_a0__501_93__86();
inline void check_status__86();

void work_AnonFilter_a0__501_93__86(int);


inline float __pop__86() {
float res=BUFFER_1_86[TAIL_1_86];
TAIL_1_86++;
return res;
}

inline float __pop__86(int n) {
float res=BUFFER_1_86[TAIL_1_86];
TAIL_1_86+=n;

return res;
}

inline float __peek__86(int offs) {
return BUFFER_1_86[TAIL_1_86+offs];
}



inline void __push__86(float data) {
BUFFER_86_3[HEAD_86_3]=data;
HEAD_86_3++;
}



 
void init_AnonFilter_a0__501_93__86(){
}
void save_file_pointer__86(object_write_buffer *buf) {}
void load_file_pointer__86(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__501_93__86__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__498 = 0.0f;/* float */
  int i__conflict__0__499 = 0;/* int */
  int i__500 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__498 = ((float)0.0))/*float*/;
  for ((i__conflict__0__499 = 0)/*int*/; (i__conflict__0__499 < -50); (i__conflict__0__499++)) {{
      (sum__498 = (sum__498 + ((*(____in+i__conflict__0__499)) * (*(____in+(i__conflict__0__499 + 82))))))/*float*/;
    }
  }
  for ((i__500 = 0)/*int*/; (i__500 < 32); (i__500++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__498);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__501_93__86__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__498 = 0.0f;/* float */
  int i__conflict__0__499 = 0;/* int */
  int i__500 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__498 = ((float)0.0))/*float*/;
  for ((i__conflict__0__499 = 0)/*int*/; (i__conflict__0__499 < -50); (i__conflict__0__499++)) {{
      (sum__498 = (sum__498 + ((*(____in+i__conflict__0__499)) * (*(____in+(i__conflict__0__499 + 82))))))/*float*/;
    }
  }
  for ((i__500 = 0)/*int*/; (i__500 < 32); (i__500++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__498);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__501_93__86(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__498 = 0.0f;/* float */
      int i__conflict__0__499 = 0;/* int */
      int i__500 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__498 = ((float)0.0))/*float*/;
      for ((i__conflict__0__499 = 0)/*int*/; (i__conflict__0__499 < -50); (i__conflict__0__499++)) {{
          (sum__498 = (sum__498 + (__peek__86(i__conflict__0__499) * __peek__86((i__conflict__0__499 + 82)))))/*float*/;
        }
      }
      for ((i__500 = 0)/*int*/; (i__500 < 32); (i__500++)) {{
          __pop__86();
        }
      }
      __push__86(sum__498);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_87;
int __counter_87 = 0;
int __steady_87 = 0;
int __tmp_87 = 0;
int __tmp2_87 = 0;
int *__state_flag_87 = NULL;
thread_info *__thread_87 = NULL;



void save_peek_buffer__87(object_write_buffer *buf);
void load_peek_buffer__87(object_write_buffer *buf);
void save_file_pointer__87(object_write_buffer *buf);
void load_file_pointer__87(object_write_buffer *buf);

 
void init_AnonFilter_a0__507_94__87();
inline void check_status__87();

void work_AnonFilter_a0__507_94__87(int);


inline float __pop__87() {
float res=BUFFER_1_87[TAIL_1_87];
TAIL_1_87++;
return res;
}

inline float __pop__87(int n) {
float res=BUFFER_1_87[TAIL_1_87];
TAIL_1_87+=n;

return res;
}

inline float __peek__87(int offs) {
return BUFFER_1_87[TAIL_1_87+offs];
}



inline void __push__87(float data) {
BUFFER_87_3[HEAD_87_3]=data;
HEAD_87_3++;
}



 
void init_AnonFilter_a0__507_94__87(){
}
void save_file_pointer__87(object_write_buffer *buf) {}
void load_file_pointer__87(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__507_94__87__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__504 = 0.0f;/* float */
  int i__conflict__0__505 = 0;/* int */
  int i__506 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__504 = ((float)0.0))/*float*/;
  for ((i__conflict__0__505 = 0)/*int*/; (i__conflict__0__505 < -51); (i__conflict__0__505++)) {{
      (sum__504 = (sum__504 + ((*(____in+i__conflict__0__505)) * (*(____in+(i__conflict__0__505 + 83))))))/*float*/;
    }
  }
  for ((i__506 = 0)/*int*/; (i__506 < 32); (i__506++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__504);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__507_94__87__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__504 = 0.0f;/* float */
  int i__conflict__0__505 = 0;/* int */
  int i__506 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__504 = ((float)0.0))/*float*/;
  for ((i__conflict__0__505 = 0)/*int*/; (i__conflict__0__505 < -51); (i__conflict__0__505++)) {{
      (sum__504 = (sum__504 + ((*(____in+i__conflict__0__505)) * (*(____in+(i__conflict__0__505 + 83))))))/*float*/;
    }
  }
  for ((i__506 = 0)/*int*/; (i__506 < 32); (i__506++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__504);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__507_94__87(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__504 = 0.0f;/* float */
      int i__conflict__0__505 = 0;/* int */
      int i__506 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__504 = ((float)0.0))/*float*/;
      for ((i__conflict__0__505 = 0)/*int*/; (i__conflict__0__505 < -51); (i__conflict__0__505++)) {{
          (sum__504 = (sum__504 + (__peek__87(i__conflict__0__505) * __peek__87((i__conflict__0__505 + 83)))))/*float*/;
        }
      }
      for ((i__506 = 0)/*int*/; (i__506 < 32); (i__506++)) {{
          __pop__87();
        }
      }
      __push__87(sum__504);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_88;
int __counter_88 = 0;
int __steady_88 = 0;
int __tmp_88 = 0;
int __tmp2_88 = 0;
int *__state_flag_88 = NULL;
thread_info *__thread_88 = NULL;



void save_peek_buffer__88(object_write_buffer *buf);
void load_peek_buffer__88(object_write_buffer *buf);
void save_file_pointer__88(object_write_buffer *buf);
void load_file_pointer__88(object_write_buffer *buf);

 
void init_AnonFilter_a0__513_95__88();
inline void check_status__88();

void work_AnonFilter_a0__513_95__88(int);


inline float __pop__88() {
float res=BUFFER_1_88[TAIL_1_88];
TAIL_1_88++;
return res;
}

inline float __pop__88(int n) {
float res=BUFFER_1_88[TAIL_1_88];
TAIL_1_88+=n;

return res;
}

inline float __peek__88(int offs) {
return BUFFER_1_88[TAIL_1_88+offs];
}



inline void __push__88(float data) {
BUFFER_88_3[HEAD_88_3]=data;
HEAD_88_3++;
}



 
void init_AnonFilter_a0__513_95__88(){
}
void save_file_pointer__88(object_write_buffer *buf) {}
void load_file_pointer__88(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__513_95__88__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__510 = 0.0f;/* float */
  int i__conflict__0__511 = 0;/* int */
  int i__512 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__510 = ((float)0.0))/*float*/;
  for ((i__conflict__0__511 = 0)/*int*/; (i__conflict__0__511 < -52); (i__conflict__0__511++)) {{
      (sum__510 = (sum__510 + ((*(____in+i__conflict__0__511)) * (*(____in+(i__conflict__0__511 + 84))))))/*float*/;
    }
  }
  for ((i__512 = 0)/*int*/; (i__512 < 32); (i__512++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__510);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__513_95__88__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__510 = 0.0f;/* float */
  int i__conflict__0__511 = 0;/* int */
  int i__512 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__510 = ((float)0.0))/*float*/;
  for ((i__conflict__0__511 = 0)/*int*/; (i__conflict__0__511 < -52); (i__conflict__0__511++)) {{
      (sum__510 = (sum__510 + ((*(____in+i__conflict__0__511)) * (*(____in+(i__conflict__0__511 + 84))))))/*float*/;
    }
  }
  for ((i__512 = 0)/*int*/; (i__512 < 32); (i__512++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__510);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__513_95__88(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__510 = 0.0f;/* float */
      int i__conflict__0__511 = 0;/* int */
      int i__512 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__510 = ((float)0.0))/*float*/;
      for ((i__conflict__0__511 = 0)/*int*/; (i__conflict__0__511 < -52); (i__conflict__0__511++)) {{
          (sum__510 = (sum__510 + (__peek__88(i__conflict__0__511) * __peek__88((i__conflict__0__511 + 84)))))/*float*/;
        }
      }
      for ((i__512 = 0)/*int*/; (i__512 < 32); (i__512++)) {{
          __pop__88();
        }
      }
      __push__88(sum__510);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_89;
int __counter_89 = 0;
int __steady_89 = 0;
int __tmp_89 = 0;
int __tmp2_89 = 0;
int *__state_flag_89 = NULL;
thread_info *__thread_89 = NULL;



void save_peek_buffer__89(object_write_buffer *buf);
void load_peek_buffer__89(object_write_buffer *buf);
void save_file_pointer__89(object_write_buffer *buf);
void load_file_pointer__89(object_write_buffer *buf);

 
void init_AnonFilter_a0__519_96__89();
inline void check_status__89();

void work_AnonFilter_a0__519_96__89(int);


inline float __pop__89() {
float res=BUFFER_1_89[TAIL_1_89];
TAIL_1_89++;
return res;
}

inline float __pop__89(int n) {
float res=BUFFER_1_89[TAIL_1_89];
TAIL_1_89+=n;

return res;
}

inline float __peek__89(int offs) {
return BUFFER_1_89[TAIL_1_89+offs];
}



inline void __push__89(float data) {
BUFFER_89_3[HEAD_89_3]=data;
HEAD_89_3++;
}



 
void init_AnonFilter_a0__519_96__89(){
}
void save_file_pointer__89(object_write_buffer *buf) {}
void load_file_pointer__89(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__519_96__89__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__516 = 0.0f;/* float */
  int i__conflict__0__517 = 0;/* int */
  int i__518 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__516 = ((float)0.0))/*float*/;
  for ((i__conflict__0__517 = 0)/*int*/; (i__conflict__0__517 < -53); (i__conflict__0__517++)) {{
      (sum__516 = (sum__516 + ((*(____in+i__conflict__0__517)) * (*(____in+(i__conflict__0__517 + 85))))))/*float*/;
    }
  }
  for ((i__518 = 0)/*int*/; (i__518 < 32); (i__518++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__516);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__519_96__89__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__516 = 0.0f;/* float */
  int i__conflict__0__517 = 0;/* int */
  int i__518 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__516 = ((float)0.0))/*float*/;
  for ((i__conflict__0__517 = 0)/*int*/; (i__conflict__0__517 < -53); (i__conflict__0__517++)) {{
      (sum__516 = (sum__516 + ((*(____in+i__conflict__0__517)) * (*(____in+(i__conflict__0__517 + 85))))))/*float*/;
    }
  }
  for ((i__518 = 0)/*int*/; (i__518 < 32); (i__518++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__516);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__519_96__89(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__516 = 0.0f;/* float */
      int i__conflict__0__517 = 0;/* int */
      int i__518 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__516 = ((float)0.0))/*float*/;
      for ((i__conflict__0__517 = 0)/*int*/; (i__conflict__0__517 < -53); (i__conflict__0__517++)) {{
          (sum__516 = (sum__516 + (__peek__89(i__conflict__0__517) * __peek__89((i__conflict__0__517 + 85)))))/*float*/;
        }
      }
      for ((i__518 = 0)/*int*/; (i__518 < 32); (i__518++)) {{
          __pop__89();
        }
      }
      __push__89(sum__516);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_90;
int __counter_90 = 0;
int __steady_90 = 0;
int __tmp_90 = 0;
int __tmp2_90 = 0;
int *__state_flag_90 = NULL;
thread_info *__thread_90 = NULL;



void save_peek_buffer__90(object_write_buffer *buf);
void load_peek_buffer__90(object_write_buffer *buf);
void save_file_pointer__90(object_write_buffer *buf);
void load_file_pointer__90(object_write_buffer *buf);

 
void init_AnonFilter_a0__525_97__90();
inline void check_status__90();

void work_AnonFilter_a0__525_97__90(int);


inline float __pop__90() {
float res=BUFFER_1_90[TAIL_1_90];
TAIL_1_90++;
return res;
}

inline float __pop__90(int n) {
float res=BUFFER_1_90[TAIL_1_90];
TAIL_1_90+=n;

return res;
}

inline float __peek__90(int offs) {
return BUFFER_1_90[TAIL_1_90+offs];
}



inline void __push__90(float data) {
BUFFER_90_3[HEAD_90_3]=data;
HEAD_90_3++;
}



 
void init_AnonFilter_a0__525_97__90(){
}
void save_file_pointer__90(object_write_buffer *buf) {}
void load_file_pointer__90(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__525_97__90__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__522 = 0.0f;/* float */
  int i__conflict__0__523 = 0;/* int */
  int i__524 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__522 = ((float)0.0))/*float*/;
  for ((i__conflict__0__523 = 0)/*int*/; (i__conflict__0__523 < -54); (i__conflict__0__523++)) {{
      (sum__522 = (sum__522 + ((*(____in+i__conflict__0__523)) * (*(____in+(i__conflict__0__523 + 86))))))/*float*/;
    }
  }
  for ((i__524 = 0)/*int*/; (i__524 < 32); (i__524++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__522);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__525_97__90__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__522 = 0.0f;/* float */
  int i__conflict__0__523 = 0;/* int */
  int i__524 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__522 = ((float)0.0))/*float*/;
  for ((i__conflict__0__523 = 0)/*int*/; (i__conflict__0__523 < -54); (i__conflict__0__523++)) {{
      (sum__522 = (sum__522 + ((*(____in+i__conflict__0__523)) * (*(____in+(i__conflict__0__523 + 86))))))/*float*/;
    }
  }
  for ((i__524 = 0)/*int*/; (i__524 < 32); (i__524++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__522);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__525_97__90(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__522 = 0.0f;/* float */
      int i__conflict__0__523 = 0;/* int */
      int i__524 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__522 = ((float)0.0))/*float*/;
      for ((i__conflict__0__523 = 0)/*int*/; (i__conflict__0__523 < -54); (i__conflict__0__523++)) {{
          (sum__522 = (sum__522 + (__peek__90(i__conflict__0__523) * __peek__90((i__conflict__0__523 + 86)))))/*float*/;
        }
      }
      for ((i__524 = 0)/*int*/; (i__524 < 32); (i__524++)) {{
          __pop__90();
        }
      }
      __push__90(sum__522);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_91;
int __counter_91 = 0;
int __steady_91 = 0;
int __tmp_91 = 0;
int __tmp2_91 = 0;
int *__state_flag_91 = NULL;
thread_info *__thread_91 = NULL;



void save_peek_buffer__91(object_write_buffer *buf);
void load_peek_buffer__91(object_write_buffer *buf);
void save_file_pointer__91(object_write_buffer *buf);
void load_file_pointer__91(object_write_buffer *buf);

 
void init_AnonFilter_a0__531_98__91();
inline void check_status__91();

void work_AnonFilter_a0__531_98__91(int);


inline float __pop__91() {
float res=BUFFER_1_91[TAIL_1_91];
TAIL_1_91++;
return res;
}

inline float __pop__91(int n) {
float res=BUFFER_1_91[TAIL_1_91];
TAIL_1_91+=n;

return res;
}

inline float __peek__91(int offs) {
return BUFFER_1_91[TAIL_1_91+offs];
}



inline void __push__91(float data) {
BUFFER_91_3[HEAD_91_3]=data;
HEAD_91_3++;
}



 
void init_AnonFilter_a0__531_98__91(){
}
void save_file_pointer__91(object_write_buffer *buf) {}
void load_file_pointer__91(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__531_98__91__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__528 = 0.0f;/* float */
  int i__conflict__0__529 = 0;/* int */
  int i__530 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__528 = ((float)0.0))/*float*/;
  for ((i__conflict__0__529 = 0)/*int*/; (i__conflict__0__529 < -55); (i__conflict__0__529++)) {{
      (sum__528 = (sum__528 + ((*(____in+i__conflict__0__529)) * (*(____in+(i__conflict__0__529 + 87))))))/*float*/;
    }
  }
  for ((i__530 = 0)/*int*/; (i__530 < 32); (i__530++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__528);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__531_98__91__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__528 = 0.0f;/* float */
  int i__conflict__0__529 = 0;/* int */
  int i__530 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__528 = ((float)0.0))/*float*/;
  for ((i__conflict__0__529 = 0)/*int*/; (i__conflict__0__529 < -55); (i__conflict__0__529++)) {{
      (sum__528 = (sum__528 + ((*(____in+i__conflict__0__529)) * (*(____in+(i__conflict__0__529 + 87))))))/*float*/;
    }
  }
  for ((i__530 = 0)/*int*/; (i__530 < 32); (i__530++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__528);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__531_98__91(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__528 = 0.0f;/* float */
      int i__conflict__0__529 = 0;/* int */
      int i__530 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__528 = ((float)0.0))/*float*/;
      for ((i__conflict__0__529 = 0)/*int*/; (i__conflict__0__529 < -55); (i__conflict__0__529++)) {{
          (sum__528 = (sum__528 + (__peek__91(i__conflict__0__529) * __peek__91((i__conflict__0__529 + 87)))))/*float*/;
        }
      }
      for ((i__530 = 0)/*int*/; (i__530 < 32); (i__530++)) {{
          __pop__91();
        }
      }
      __push__91(sum__528);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_92;
int __counter_92 = 0;
int __steady_92 = 0;
int __tmp_92 = 0;
int __tmp2_92 = 0;
int *__state_flag_92 = NULL;
thread_info *__thread_92 = NULL;



void save_peek_buffer__92(object_write_buffer *buf);
void load_peek_buffer__92(object_write_buffer *buf);
void save_file_pointer__92(object_write_buffer *buf);
void load_file_pointer__92(object_write_buffer *buf);

 
void init_AnonFilter_a0__537_99__92();
inline void check_status__92();

void work_AnonFilter_a0__537_99__92(int);


inline float __pop__92() {
float res=BUFFER_1_92[TAIL_1_92];
TAIL_1_92++;
return res;
}

inline float __pop__92(int n) {
float res=BUFFER_1_92[TAIL_1_92];
TAIL_1_92+=n;

return res;
}

inline float __peek__92(int offs) {
return BUFFER_1_92[TAIL_1_92+offs];
}



inline void __push__92(float data) {
BUFFER_92_3[HEAD_92_3]=data;
HEAD_92_3++;
}



 
void init_AnonFilter_a0__537_99__92(){
}
void save_file_pointer__92(object_write_buffer *buf) {}
void load_file_pointer__92(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__537_99__92__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__534 = 0.0f;/* float */
  int i__conflict__0__535 = 0;/* int */
  int i__536 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__534 = ((float)0.0))/*float*/;
  for ((i__conflict__0__535 = 0)/*int*/; (i__conflict__0__535 < -56); (i__conflict__0__535++)) {{
      (sum__534 = (sum__534 + ((*(____in+i__conflict__0__535)) * (*(____in+(i__conflict__0__535 + 88))))))/*float*/;
    }
  }
  for ((i__536 = 0)/*int*/; (i__536 < 32); (i__536++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__534);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__537_99__92__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__534 = 0.0f;/* float */
  int i__conflict__0__535 = 0;/* int */
  int i__536 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__534 = ((float)0.0))/*float*/;
  for ((i__conflict__0__535 = 0)/*int*/; (i__conflict__0__535 < -56); (i__conflict__0__535++)) {{
      (sum__534 = (sum__534 + ((*(____in+i__conflict__0__535)) * (*(____in+(i__conflict__0__535 + 88))))))/*float*/;
    }
  }
  for ((i__536 = 0)/*int*/; (i__536 < 32); (i__536++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__534);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__537_99__92(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__534 = 0.0f;/* float */
      int i__conflict__0__535 = 0;/* int */
      int i__536 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__534 = ((float)0.0))/*float*/;
      for ((i__conflict__0__535 = 0)/*int*/; (i__conflict__0__535 < -56); (i__conflict__0__535++)) {{
          (sum__534 = (sum__534 + (__peek__92(i__conflict__0__535) * __peek__92((i__conflict__0__535 + 88)))))/*float*/;
        }
      }
      for ((i__536 = 0)/*int*/; (i__536 < 32); (i__536++)) {{
          __pop__92();
        }
      }
      __push__92(sum__534);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_93;
int __counter_93 = 0;
int __steady_93 = 0;
int __tmp_93 = 0;
int __tmp2_93 = 0;
int *__state_flag_93 = NULL;
thread_info *__thread_93 = NULL;



void save_peek_buffer__93(object_write_buffer *buf);
void load_peek_buffer__93(object_write_buffer *buf);
void save_file_pointer__93(object_write_buffer *buf);
void load_file_pointer__93(object_write_buffer *buf);

 
void init_AnonFilter_a0__543_100__93();
inline void check_status__93();

void work_AnonFilter_a0__543_100__93(int);


inline float __pop__93() {
float res=BUFFER_1_93[TAIL_1_93];
TAIL_1_93++;
return res;
}

inline float __pop__93(int n) {
float res=BUFFER_1_93[TAIL_1_93];
TAIL_1_93+=n;

return res;
}

inline float __peek__93(int offs) {
return BUFFER_1_93[TAIL_1_93+offs];
}



inline void __push__93(float data) {
BUFFER_93_3[HEAD_93_3]=data;
HEAD_93_3++;
}



 
void init_AnonFilter_a0__543_100__93(){
}
void save_file_pointer__93(object_write_buffer *buf) {}
void load_file_pointer__93(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__543_100__93__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__540 = 0.0f;/* float */
  int i__conflict__0__541 = 0;/* int */
  int i__542 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__540 = ((float)0.0))/*float*/;
  for ((i__conflict__0__541 = 0)/*int*/; (i__conflict__0__541 < -57); (i__conflict__0__541++)) {{
      (sum__540 = (sum__540 + ((*(____in+i__conflict__0__541)) * (*(____in+(i__conflict__0__541 + 89))))))/*float*/;
    }
  }
  for ((i__542 = 0)/*int*/; (i__542 < 32); (i__542++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__540);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__543_100__93__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__540 = 0.0f;/* float */
  int i__conflict__0__541 = 0;/* int */
  int i__542 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__540 = ((float)0.0))/*float*/;
  for ((i__conflict__0__541 = 0)/*int*/; (i__conflict__0__541 < -57); (i__conflict__0__541++)) {{
      (sum__540 = (sum__540 + ((*(____in+i__conflict__0__541)) * (*(____in+(i__conflict__0__541 + 89))))))/*float*/;
    }
  }
  for ((i__542 = 0)/*int*/; (i__542 < 32); (i__542++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__540);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__543_100__93(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__540 = 0.0f;/* float */
      int i__conflict__0__541 = 0;/* int */
      int i__542 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__540 = ((float)0.0))/*float*/;
      for ((i__conflict__0__541 = 0)/*int*/; (i__conflict__0__541 < -57); (i__conflict__0__541++)) {{
          (sum__540 = (sum__540 + (__peek__93(i__conflict__0__541) * __peek__93((i__conflict__0__541 + 89)))))/*float*/;
        }
      }
      for ((i__542 = 0)/*int*/; (i__542 < 32); (i__542++)) {{
          __pop__93();
        }
      }
      __push__93(sum__540);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_94;
int __counter_94 = 0;
int __steady_94 = 0;
int __tmp_94 = 0;
int __tmp2_94 = 0;
int *__state_flag_94 = NULL;
thread_info *__thread_94 = NULL;



void save_peek_buffer__94(object_write_buffer *buf);
void load_peek_buffer__94(object_write_buffer *buf);
void save_file_pointer__94(object_write_buffer *buf);
void load_file_pointer__94(object_write_buffer *buf);

 
void init_AnonFilter_a0__549_101__94();
inline void check_status__94();

void work_AnonFilter_a0__549_101__94(int);


inline float __pop__94() {
float res=BUFFER_1_94[TAIL_1_94];
TAIL_1_94++;
return res;
}

inline float __pop__94(int n) {
float res=BUFFER_1_94[TAIL_1_94];
TAIL_1_94+=n;

return res;
}

inline float __peek__94(int offs) {
return BUFFER_1_94[TAIL_1_94+offs];
}



inline void __push__94(float data) {
BUFFER_94_3[HEAD_94_3]=data;
HEAD_94_3++;
}



 
void init_AnonFilter_a0__549_101__94(){
}
void save_file_pointer__94(object_write_buffer *buf) {}
void load_file_pointer__94(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__549_101__94__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__546 = 0.0f;/* float */
  int i__conflict__0__547 = 0;/* int */
  int i__548 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__546 = ((float)0.0))/*float*/;
  for ((i__conflict__0__547 = 0)/*int*/; (i__conflict__0__547 < -58); (i__conflict__0__547++)) {{
      (sum__546 = (sum__546 + ((*(____in+i__conflict__0__547)) * (*(____in+(i__conflict__0__547 + 90))))))/*float*/;
    }
  }
  for ((i__548 = 0)/*int*/; (i__548 < 32); (i__548++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__546);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__549_101__94__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__546 = 0.0f;/* float */
  int i__conflict__0__547 = 0;/* int */
  int i__548 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__546 = ((float)0.0))/*float*/;
  for ((i__conflict__0__547 = 0)/*int*/; (i__conflict__0__547 < -58); (i__conflict__0__547++)) {{
      (sum__546 = (sum__546 + ((*(____in+i__conflict__0__547)) * (*(____in+(i__conflict__0__547 + 90))))))/*float*/;
    }
  }
  for ((i__548 = 0)/*int*/; (i__548 < 32); (i__548++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__546);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__549_101__94(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__546 = 0.0f;/* float */
      int i__conflict__0__547 = 0;/* int */
      int i__548 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__546 = ((float)0.0))/*float*/;
      for ((i__conflict__0__547 = 0)/*int*/; (i__conflict__0__547 < -58); (i__conflict__0__547++)) {{
          (sum__546 = (sum__546 + (__peek__94(i__conflict__0__547) * __peek__94((i__conflict__0__547 + 90)))))/*float*/;
        }
      }
      for ((i__548 = 0)/*int*/; (i__548 < 32); (i__548++)) {{
          __pop__94();
        }
      }
      __push__94(sum__546);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_95;
int __counter_95 = 0;
int __steady_95 = 0;
int __tmp_95 = 0;
int __tmp2_95 = 0;
int *__state_flag_95 = NULL;
thread_info *__thread_95 = NULL;



void save_peek_buffer__95(object_write_buffer *buf);
void load_peek_buffer__95(object_write_buffer *buf);
void save_file_pointer__95(object_write_buffer *buf);
void load_file_pointer__95(object_write_buffer *buf);

 
void init_AnonFilter_a0__555_102__95();
inline void check_status__95();

void work_AnonFilter_a0__555_102__95(int);


inline float __pop__95() {
float res=BUFFER_1_95[TAIL_1_95];
TAIL_1_95++;
return res;
}

inline float __pop__95(int n) {
float res=BUFFER_1_95[TAIL_1_95];
TAIL_1_95+=n;

return res;
}

inline float __peek__95(int offs) {
return BUFFER_1_95[TAIL_1_95+offs];
}



inline void __push__95(float data) {
BUFFER_95_3[HEAD_95_3]=data;
HEAD_95_3++;
}



 
void init_AnonFilter_a0__555_102__95(){
}
void save_file_pointer__95(object_write_buffer *buf) {}
void load_file_pointer__95(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__555_102__95__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__552 = 0.0f;/* float */
  int i__conflict__0__553 = 0;/* int */
  int i__554 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__552 = ((float)0.0))/*float*/;
  for ((i__conflict__0__553 = 0)/*int*/; (i__conflict__0__553 < -59); (i__conflict__0__553++)) {{
      (sum__552 = (sum__552 + ((*(____in+i__conflict__0__553)) * (*(____in+(i__conflict__0__553 + 91))))))/*float*/;
    }
  }
  for ((i__554 = 0)/*int*/; (i__554 < 32); (i__554++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__552);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__555_102__95__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__552 = 0.0f;/* float */
  int i__conflict__0__553 = 0;/* int */
  int i__554 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__552 = ((float)0.0))/*float*/;
  for ((i__conflict__0__553 = 0)/*int*/; (i__conflict__0__553 < -59); (i__conflict__0__553++)) {{
      (sum__552 = (sum__552 + ((*(____in+i__conflict__0__553)) * (*(____in+(i__conflict__0__553 + 91))))))/*float*/;
    }
  }
  for ((i__554 = 0)/*int*/; (i__554 < 32); (i__554++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__552);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__555_102__95(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__552 = 0.0f;/* float */
      int i__conflict__0__553 = 0;/* int */
      int i__554 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__552 = ((float)0.0))/*float*/;
      for ((i__conflict__0__553 = 0)/*int*/; (i__conflict__0__553 < -59); (i__conflict__0__553++)) {{
          (sum__552 = (sum__552 + (__peek__95(i__conflict__0__553) * __peek__95((i__conflict__0__553 + 91)))))/*float*/;
        }
      }
      for ((i__554 = 0)/*int*/; (i__554 < 32); (i__554++)) {{
          __pop__95();
        }
      }
      __push__95(sum__552);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_96;
int __counter_96 = 0;
int __steady_96 = 0;
int __tmp_96 = 0;
int __tmp2_96 = 0;
int *__state_flag_96 = NULL;
thread_info *__thread_96 = NULL;



void save_peek_buffer__96(object_write_buffer *buf);
void load_peek_buffer__96(object_write_buffer *buf);
void save_file_pointer__96(object_write_buffer *buf);
void load_file_pointer__96(object_write_buffer *buf);

 
void init_AnonFilter_a0__561_103__96();
inline void check_status__96();

void work_AnonFilter_a0__561_103__96(int);


inline float __pop__96() {
float res=BUFFER_1_96[TAIL_1_96];
TAIL_1_96++;
return res;
}

inline float __pop__96(int n) {
float res=BUFFER_1_96[TAIL_1_96];
TAIL_1_96+=n;

return res;
}

inline float __peek__96(int offs) {
return BUFFER_1_96[TAIL_1_96+offs];
}



inline void __push__96(float data) {
BUFFER_96_3[HEAD_96_3]=data;
HEAD_96_3++;
}



 
void init_AnonFilter_a0__561_103__96(){
}
void save_file_pointer__96(object_write_buffer *buf) {}
void load_file_pointer__96(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__561_103__96__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__558 = 0.0f;/* float */
  int i__conflict__0__559 = 0;/* int */
  int i__560 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__558 = ((float)0.0))/*float*/;
  for ((i__conflict__0__559 = 0)/*int*/; (i__conflict__0__559 < -60); (i__conflict__0__559++)) {{
      (sum__558 = (sum__558 + ((*(____in+i__conflict__0__559)) * (*(____in+(i__conflict__0__559 + 92))))))/*float*/;
    }
  }
  for ((i__560 = 0)/*int*/; (i__560 < 32); (i__560++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__558);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__561_103__96__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__558 = 0.0f;/* float */
  int i__conflict__0__559 = 0;/* int */
  int i__560 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__558 = ((float)0.0))/*float*/;
  for ((i__conflict__0__559 = 0)/*int*/; (i__conflict__0__559 < -60); (i__conflict__0__559++)) {{
      (sum__558 = (sum__558 + ((*(____in+i__conflict__0__559)) * (*(____in+(i__conflict__0__559 + 92))))))/*float*/;
    }
  }
  for ((i__560 = 0)/*int*/; (i__560 < 32); (i__560++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__558);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__561_103__96(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__558 = 0.0f;/* float */
      int i__conflict__0__559 = 0;/* int */
      int i__560 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__558 = ((float)0.0))/*float*/;
      for ((i__conflict__0__559 = 0)/*int*/; (i__conflict__0__559 < -60); (i__conflict__0__559++)) {{
          (sum__558 = (sum__558 + (__peek__96(i__conflict__0__559) * __peek__96((i__conflict__0__559 + 92)))))/*float*/;
        }
      }
      for ((i__560 = 0)/*int*/; (i__560 < 32); (i__560++)) {{
          __pop__96();
        }
      }
      __push__96(sum__558);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_97;
int __counter_97 = 0;
int __steady_97 = 0;
int __tmp_97 = 0;
int __tmp2_97 = 0;
int *__state_flag_97 = NULL;
thread_info *__thread_97 = NULL;



void save_peek_buffer__97(object_write_buffer *buf);
void load_peek_buffer__97(object_write_buffer *buf);
void save_file_pointer__97(object_write_buffer *buf);
void load_file_pointer__97(object_write_buffer *buf);

 
void init_AnonFilter_a0__567_104__97();
inline void check_status__97();

void work_AnonFilter_a0__567_104__97(int);


inline float __pop__97() {
float res=BUFFER_1_97[TAIL_1_97];
TAIL_1_97++;
return res;
}

inline float __pop__97(int n) {
float res=BUFFER_1_97[TAIL_1_97];
TAIL_1_97+=n;

return res;
}

inline float __peek__97(int offs) {
return BUFFER_1_97[TAIL_1_97+offs];
}



inline void __push__97(float data) {
BUFFER_97_3[HEAD_97_3]=data;
HEAD_97_3++;
}



 
void init_AnonFilter_a0__567_104__97(){
}
void save_file_pointer__97(object_write_buffer *buf) {}
void load_file_pointer__97(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__567_104__97__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__564 = 0.0f;/* float */
  int i__conflict__0__565 = 0;/* int */
  int i__566 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__564 = ((float)0.0))/*float*/;
  for ((i__conflict__0__565 = 0)/*int*/; (i__conflict__0__565 < -61); (i__conflict__0__565++)) {{
      (sum__564 = (sum__564 + ((*(____in+i__conflict__0__565)) * (*(____in+(i__conflict__0__565 + 93))))))/*float*/;
    }
  }
  for ((i__566 = 0)/*int*/; (i__566 < 32); (i__566++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__564);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__567_104__97__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__564 = 0.0f;/* float */
  int i__conflict__0__565 = 0;/* int */
  int i__566 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__564 = ((float)0.0))/*float*/;
  for ((i__conflict__0__565 = 0)/*int*/; (i__conflict__0__565 < -61); (i__conflict__0__565++)) {{
      (sum__564 = (sum__564 + ((*(____in+i__conflict__0__565)) * (*(____in+(i__conflict__0__565 + 93))))))/*float*/;
    }
  }
  for ((i__566 = 0)/*int*/; (i__566 < 32); (i__566++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__564);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__567_104__97(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__564 = 0.0f;/* float */
      int i__conflict__0__565 = 0;/* int */
      int i__566 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__564 = ((float)0.0))/*float*/;
      for ((i__conflict__0__565 = 0)/*int*/; (i__conflict__0__565 < -61); (i__conflict__0__565++)) {{
          (sum__564 = (sum__564 + (__peek__97(i__conflict__0__565) * __peek__97((i__conflict__0__565 + 93)))))/*float*/;
        }
      }
      for ((i__566 = 0)/*int*/; (i__566 < 32); (i__566++)) {{
          __pop__97();
        }
      }
      __push__97(sum__564);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_98;
int __counter_98 = 0;
int __steady_98 = 0;
int __tmp_98 = 0;
int __tmp2_98 = 0;
int *__state_flag_98 = NULL;
thread_info *__thread_98 = NULL;



void save_peek_buffer__98(object_write_buffer *buf);
void load_peek_buffer__98(object_write_buffer *buf);
void save_file_pointer__98(object_write_buffer *buf);
void load_file_pointer__98(object_write_buffer *buf);

 
void init_AnonFilter_a0__573_105__98();
inline void check_status__98();

void work_AnonFilter_a0__573_105__98(int);


inline float __pop__98() {
float res=BUFFER_1_98[TAIL_1_98];
TAIL_1_98++;
return res;
}

inline float __pop__98(int n) {
float res=BUFFER_1_98[TAIL_1_98];
TAIL_1_98+=n;

return res;
}

inline float __peek__98(int offs) {
return BUFFER_1_98[TAIL_1_98+offs];
}



inline void __push__98(float data) {
BUFFER_98_3[HEAD_98_3]=data;
HEAD_98_3++;
}



 
void init_AnonFilter_a0__573_105__98(){
}
void save_file_pointer__98(object_write_buffer *buf) {}
void load_file_pointer__98(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__573_105__98__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__570 = 0.0f;/* float */
  int i__conflict__0__571 = 0;/* int */
  int i__572 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__570 = ((float)0.0))/*float*/;
  for ((i__conflict__0__571 = 0)/*int*/; (i__conflict__0__571 < -62); (i__conflict__0__571++)) {{
      (sum__570 = (sum__570 + ((*(____in+i__conflict__0__571)) * (*(____in+(i__conflict__0__571 + 94))))))/*float*/;
    }
  }
  for ((i__572 = 0)/*int*/; (i__572 < 32); (i__572++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__570);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__573_105__98__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__570 = 0.0f;/* float */
  int i__conflict__0__571 = 0;/* int */
  int i__572 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__570 = ((float)0.0))/*float*/;
  for ((i__conflict__0__571 = 0)/*int*/; (i__conflict__0__571 < -62); (i__conflict__0__571++)) {{
      (sum__570 = (sum__570 + ((*(____in+i__conflict__0__571)) * (*(____in+(i__conflict__0__571 + 94))))))/*float*/;
    }
  }
  for ((i__572 = 0)/*int*/; (i__572 < 32); (i__572++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__570);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__573_105__98(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__570 = 0.0f;/* float */
      int i__conflict__0__571 = 0;/* int */
      int i__572 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__570 = ((float)0.0))/*float*/;
      for ((i__conflict__0__571 = 0)/*int*/; (i__conflict__0__571 < -62); (i__conflict__0__571++)) {{
          (sum__570 = (sum__570 + (__peek__98(i__conflict__0__571) * __peek__98((i__conflict__0__571 + 94)))))/*float*/;
        }
      }
      for ((i__572 = 0)/*int*/; (i__572 < 32); (i__572++)) {{
          __pop__98();
        }
      }
      __push__98(sum__570);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_99;
int __counter_99 = 0;
int __steady_99 = 0;
int __tmp_99 = 0;
int __tmp2_99 = 0;
int *__state_flag_99 = NULL;
thread_info *__thread_99 = NULL;



void save_peek_buffer__99(object_write_buffer *buf);
void load_peek_buffer__99(object_write_buffer *buf);
void save_file_pointer__99(object_write_buffer *buf);
void load_file_pointer__99(object_write_buffer *buf);

 
void init_AnonFilter_a0__579_106__99();
inline void check_status__99();

void work_AnonFilter_a0__579_106__99(int);


inline float __pop__99() {
float res=BUFFER_1_99[TAIL_1_99];
TAIL_1_99++;
return res;
}

inline float __pop__99(int n) {
float res=BUFFER_1_99[TAIL_1_99];
TAIL_1_99+=n;

return res;
}

inline float __peek__99(int offs) {
return BUFFER_1_99[TAIL_1_99+offs];
}



inline void __push__99(float data) {
BUFFER_99_3[HEAD_99_3]=data;
HEAD_99_3++;
}



 
void init_AnonFilter_a0__579_106__99(){
}
void save_file_pointer__99(object_write_buffer *buf) {}
void load_file_pointer__99(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__579_106__99__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__576 = 0.0f;/* float */
  int i__conflict__0__577 = 0;/* int */
  int i__578 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__576 = ((float)0.0))/*float*/;
  for ((i__conflict__0__577 = 0)/*int*/; (i__conflict__0__577 < -63); (i__conflict__0__577++)) {{
      (sum__576 = (sum__576 + ((*(____in+i__conflict__0__577)) * (*(____in+(i__conflict__0__577 + 95))))))/*float*/;
    }
  }
  for ((i__578 = 0)/*int*/; (i__578 < 32); (i__578++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__576);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__579_106__99__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__576 = 0.0f;/* float */
  int i__conflict__0__577 = 0;/* int */
  int i__578 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__576 = ((float)0.0))/*float*/;
  for ((i__conflict__0__577 = 0)/*int*/; (i__conflict__0__577 < -63); (i__conflict__0__577++)) {{
      (sum__576 = (sum__576 + ((*(____in+i__conflict__0__577)) * (*(____in+(i__conflict__0__577 + 95))))))/*float*/;
    }
  }
  for ((i__578 = 0)/*int*/; (i__578 < 32); (i__578++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__576);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__579_106__99(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__576 = 0.0f;/* float */
      int i__conflict__0__577 = 0;/* int */
      int i__578 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__576 = ((float)0.0))/*float*/;
      for ((i__conflict__0__577 = 0)/*int*/; (i__conflict__0__577 < -63); (i__conflict__0__577++)) {{
          (sum__576 = (sum__576 + (__peek__99(i__conflict__0__577) * __peek__99((i__conflict__0__577 + 95)))))/*float*/;
        }
      }
      for ((i__578 = 0)/*int*/; (i__578 < 32); (i__578++)) {{
          __pop__99();
        }
      }
      __push__99(sum__576);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_100;
int __counter_100 = 0;
int __steady_100 = 0;
int __tmp_100 = 0;
int __tmp2_100 = 0;
int *__state_flag_100 = NULL;
thread_info *__thread_100 = NULL;



void save_peek_buffer__100(object_write_buffer *buf);
void load_peek_buffer__100(object_write_buffer *buf);
void save_file_pointer__100(object_write_buffer *buf);
void load_file_pointer__100(object_write_buffer *buf);

 
void init_AnonFilter_a0__585_107__100();
inline void check_status__100();

void work_AnonFilter_a0__585_107__100(int);


inline float __pop__100() {
float res=BUFFER_1_100[TAIL_1_100];
TAIL_1_100++;
return res;
}

inline float __pop__100(int n) {
float res=BUFFER_1_100[TAIL_1_100];
TAIL_1_100+=n;

return res;
}

inline float __peek__100(int offs) {
return BUFFER_1_100[TAIL_1_100+offs];
}



inline void __push__100(float data) {
BUFFER_100_3[HEAD_100_3]=data;
HEAD_100_3++;
}



 
void init_AnonFilter_a0__585_107__100(){
}
void save_file_pointer__100(object_write_buffer *buf) {}
void load_file_pointer__100(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__585_107__100__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__582 = 0.0f;/* float */
  int i__conflict__0__583 = 0;/* int */
  int i__584 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__582 = ((float)0.0))/*float*/;
  for ((i__conflict__0__583 = 0)/*int*/; (i__conflict__0__583 < -64); (i__conflict__0__583++)) {{
      (sum__582 = (sum__582 + ((*(____in+i__conflict__0__583)) * (*(____in+(i__conflict__0__583 + 96))))))/*float*/;
    }
  }
  for ((i__584 = 0)/*int*/; (i__584 < 32); (i__584++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__582);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__585_107__100__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__582 = 0.0f;/* float */
  int i__conflict__0__583 = 0;/* int */
  int i__584 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__582 = ((float)0.0))/*float*/;
  for ((i__conflict__0__583 = 0)/*int*/; (i__conflict__0__583 < -64); (i__conflict__0__583++)) {{
      (sum__582 = (sum__582 + ((*(____in+i__conflict__0__583)) * (*(____in+(i__conflict__0__583 + 96))))))/*float*/;
    }
  }
  for ((i__584 = 0)/*int*/; (i__584 < 32); (i__584++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__582);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__585_107__100(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__582 = 0.0f;/* float */
      int i__conflict__0__583 = 0;/* int */
      int i__584 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__582 = ((float)0.0))/*float*/;
      for ((i__conflict__0__583 = 0)/*int*/; (i__conflict__0__583 < -64); (i__conflict__0__583++)) {{
          (sum__582 = (sum__582 + (__peek__100(i__conflict__0__583) * __peek__100((i__conflict__0__583 + 96)))))/*float*/;
        }
      }
      for ((i__584 = 0)/*int*/; (i__584 < 32); (i__584++)) {{
          __pop__100();
        }
      }
      __push__100(sum__582);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_101;
int __counter_101 = 0;
int __steady_101 = 0;
int __tmp_101 = 0;
int __tmp2_101 = 0;
int *__state_flag_101 = NULL;
thread_info *__thread_101 = NULL;



void save_peek_buffer__101(object_write_buffer *buf);
void load_peek_buffer__101(object_write_buffer *buf);
void save_file_pointer__101(object_write_buffer *buf);
void load_file_pointer__101(object_write_buffer *buf);

 
void init_AnonFilter_a0__591_108__101();
inline void check_status__101();

void work_AnonFilter_a0__591_108__101(int);


inline float __pop__101() {
float res=BUFFER_1_101[TAIL_1_101];
TAIL_1_101++;
return res;
}

inline float __pop__101(int n) {
float res=BUFFER_1_101[TAIL_1_101];
TAIL_1_101+=n;

return res;
}

inline float __peek__101(int offs) {
return BUFFER_1_101[TAIL_1_101+offs];
}



inline void __push__101(float data) {
BUFFER_101_3[HEAD_101_3]=data;
HEAD_101_3++;
}



 
void init_AnonFilter_a0__591_108__101(){
}
void save_file_pointer__101(object_write_buffer *buf) {}
void load_file_pointer__101(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__591_108__101__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__588 = 0.0f;/* float */
  int i__conflict__0__589 = 0;/* int */
  int i__590 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__588 = ((float)0.0))/*float*/;
  for ((i__conflict__0__589 = 0)/*int*/; (i__conflict__0__589 < -65); (i__conflict__0__589++)) {{
      (sum__588 = (sum__588 + ((*(____in+i__conflict__0__589)) * (*(____in+(i__conflict__0__589 + 97))))))/*float*/;
    }
  }
  for ((i__590 = 0)/*int*/; (i__590 < 32); (i__590++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__588);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__591_108__101__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__588 = 0.0f;/* float */
  int i__conflict__0__589 = 0;/* int */
  int i__590 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__588 = ((float)0.0))/*float*/;
  for ((i__conflict__0__589 = 0)/*int*/; (i__conflict__0__589 < -65); (i__conflict__0__589++)) {{
      (sum__588 = (sum__588 + ((*(____in+i__conflict__0__589)) * (*(____in+(i__conflict__0__589 + 97))))))/*float*/;
    }
  }
  for ((i__590 = 0)/*int*/; (i__590 < 32); (i__590++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__588);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__591_108__101(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__588 = 0.0f;/* float */
      int i__conflict__0__589 = 0;/* int */
      int i__590 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__588 = ((float)0.0))/*float*/;
      for ((i__conflict__0__589 = 0)/*int*/; (i__conflict__0__589 < -65); (i__conflict__0__589++)) {{
          (sum__588 = (sum__588 + (__peek__101(i__conflict__0__589) * __peek__101((i__conflict__0__589 + 97)))))/*float*/;
        }
      }
      for ((i__590 = 0)/*int*/; (i__590 < 32); (i__590++)) {{
          __pop__101();
        }
      }
      __push__101(sum__588);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_102;
int __counter_102 = 0;
int __steady_102 = 0;
int __tmp_102 = 0;
int __tmp2_102 = 0;
int *__state_flag_102 = NULL;
thread_info *__thread_102 = NULL;



void save_peek_buffer__102(object_write_buffer *buf);
void load_peek_buffer__102(object_write_buffer *buf);
void save_file_pointer__102(object_write_buffer *buf);
void load_file_pointer__102(object_write_buffer *buf);

 
void init_AnonFilter_a0__597_109__102();
inline void check_status__102();

void work_AnonFilter_a0__597_109__102(int);


inline float __pop__102() {
float res=BUFFER_1_102[TAIL_1_102];
TAIL_1_102++;
return res;
}

inline float __pop__102(int n) {
float res=BUFFER_1_102[TAIL_1_102];
TAIL_1_102+=n;

return res;
}

inline float __peek__102(int offs) {
return BUFFER_1_102[TAIL_1_102+offs];
}



inline void __push__102(float data) {
BUFFER_102_3[HEAD_102_3]=data;
HEAD_102_3++;
}



 
void init_AnonFilter_a0__597_109__102(){
}
void save_file_pointer__102(object_write_buffer *buf) {}
void load_file_pointer__102(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__597_109__102__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__594 = 0.0f;/* float */
  int i__conflict__0__595 = 0;/* int */
  int i__596 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__594 = ((float)0.0))/*float*/;
  for ((i__conflict__0__595 = 0)/*int*/; (i__conflict__0__595 < -66); (i__conflict__0__595++)) {{
      (sum__594 = (sum__594 + ((*(____in+i__conflict__0__595)) * (*(____in+(i__conflict__0__595 + 98))))))/*float*/;
    }
  }
  for ((i__596 = 0)/*int*/; (i__596 < 32); (i__596++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__594);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__597_109__102__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__594 = 0.0f;/* float */
  int i__conflict__0__595 = 0;/* int */
  int i__596 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__594 = ((float)0.0))/*float*/;
  for ((i__conflict__0__595 = 0)/*int*/; (i__conflict__0__595 < -66); (i__conflict__0__595++)) {{
      (sum__594 = (sum__594 + ((*(____in+i__conflict__0__595)) * (*(____in+(i__conflict__0__595 + 98))))))/*float*/;
    }
  }
  for ((i__596 = 0)/*int*/; (i__596 < 32); (i__596++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__594);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__597_109__102(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__594 = 0.0f;/* float */
      int i__conflict__0__595 = 0;/* int */
      int i__596 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__594 = ((float)0.0))/*float*/;
      for ((i__conflict__0__595 = 0)/*int*/; (i__conflict__0__595 < -66); (i__conflict__0__595++)) {{
          (sum__594 = (sum__594 + (__peek__102(i__conflict__0__595) * __peek__102((i__conflict__0__595 + 98)))))/*float*/;
        }
      }
      for ((i__596 = 0)/*int*/; (i__596 < 32); (i__596++)) {{
          __pop__102();
        }
      }
      __push__102(sum__594);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_103;
int __counter_103 = 0;
int __steady_103 = 0;
int __tmp_103 = 0;
int __tmp2_103 = 0;
int *__state_flag_103 = NULL;
thread_info *__thread_103 = NULL;



void save_peek_buffer__103(object_write_buffer *buf);
void load_peek_buffer__103(object_write_buffer *buf);
void save_file_pointer__103(object_write_buffer *buf);
void load_file_pointer__103(object_write_buffer *buf);

 
void init_AnonFilter_a0__603_110__103();
inline void check_status__103();

void work_AnonFilter_a0__603_110__103(int);


inline float __pop__103() {
float res=BUFFER_1_103[TAIL_1_103];
TAIL_1_103++;
return res;
}

inline float __pop__103(int n) {
float res=BUFFER_1_103[TAIL_1_103];
TAIL_1_103+=n;

return res;
}

inline float __peek__103(int offs) {
return BUFFER_1_103[TAIL_1_103+offs];
}



inline void __push__103(float data) {
BUFFER_103_3[HEAD_103_3]=data;
HEAD_103_3++;
}



 
void init_AnonFilter_a0__603_110__103(){
}
void save_file_pointer__103(object_write_buffer *buf) {}
void load_file_pointer__103(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__603_110__103__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__600 = 0.0f;/* float */
  int i__conflict__0__601 = 0;/* int */
  int i__602 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__600 = ((float)0.0))/*float*/;
  for ((i__conflict__0__601 = 0)/*int*/; (i__conflict__0__601 < -67); (i__conflict__0__601++)) {{
      (sum__600 = (sum__600 + ((*(____in+i__conflict__0__601)) * (*(____in+(i__conflict__0__601 + 99))))))/*float*/;
    }
  }
  for ((i__602 = 0)/*int*/; (i__602 < 32); (i__602++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__600);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__603_110__103__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__600 = 0.0f;/* float */
  int i__conflict__0__601 = 0;/* int */
  int i__602 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__600 = ((float)0.0))/*float*/;
  for ((i__conflict__0__601 = 0)/*int*/; (i__conflict__0__601 < -67); (i__conflict__0__601++)) {{
      (sum__600 = (sum__600 + ((*(____in+i__conflict__0__601)) * (*(____in+(i__conflict__0__601 + 99))))))/*float*/;
    }
  }
  for ((i__602 = 0)/*int*/; (i__602 < 32); (i__602++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__600);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__603_110__103(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__600 = 0.0f;/* float */
      int i__conflict__0__601 = 0;/* int */
      int i__602 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__600 = ((float)0.0))/*float*/;
      for ((i__conflict__0__601 = 0)/*int*/; (i__conflict__0__601 < -67); (i__conflict__0__601++)) {{
          (sum__600 = (sum__600 + (__peek__103(i__conflict__0__601) * __peek__103((i__conflict__0__601 + 99)))))/*float*/;
        }
      }
      for ((i__602 = 0)/*int*/; (i__602 < 32); (i__602++)) {{
          __pop__103();
        }
      }
      __push__103(sum__600);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_104;
int __counter_104 = 0;
int __steady_104 = 0;
int __tmp_104 = 0;
int __tmp2_104 = 0;
int *__state_flag_104 = NULL;
thread_info *__thread_104 = NULL;



void save_peek_buffer__104(object_write_buffer *buf);
void load_peek_buffer__104(object_write_buffer *buf);
void save_file_pointer__104(object_write_buffer *buf);
void load_file_pointer__104(object_write_buffer *buf);

 
void init_AnonFilter_a0__609_111__104();
inline void check_status__104();

void work_AnonFilter_a0__609_111__104(int);


inline float __pop__104() {
float res=BUFFER_1_104[TAIL_1_104];
TAIL_1_104++;
return res;
}

inline float __pop__104(int n) {
float res=BUFFER_1_104[TAIL_1_104];
TAIL_1_104+=n;

return res;
}

inline float __peek__104(int offs) {
return BUFFER_1_104[TAIL_1_104+offs];
}



inline void __push__104(float data) {
BUFFER_104_3[HEAD_104_3]=data;
HEAD_104_3++;
}



 
void init_AnonFilter_a0__609_111__104(){
}
void save_file_pointer__104(object_write_buffer *buf) {}
void load_file_pointer__104(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__609_111__104__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__606 = 0.0f;/* float */
  int i__conflict__0__607 = 0;/* int */
  int i__608 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__606 = ((float)0.0))/*float*/;
  for ((i__conflict__0__607 = 0)/*int*/; (i__conflict__0__607 < -68); (i__conflict__0__607++)) {{
      (sum__606 = (sum__606 + ((*(____in+i__conflict__0__607)) * (*(____in+(i__conflict__0__607 + 100))))))/*float*/;
    }
  }
  for ((i__608 = 0)/*int*/; (i__608 < 32); (i__608++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__606);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__609_111__104__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__606 = 0.0f;/* float */
  int i__conflict__0__607 = 0;/* int */
  int i__608 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__606 = ((float)0.0))/*float*/;
  for ((i__conflict__0__607 = 0)/*int*/; (i__conflict__0__607 < -68); (i__conflict__0__607++)) {{
      (sum__606 = (sum__606 + ((*(____in+i__conflict__0__607)) * (*(____in+(i__conflict__0__607 + 100))))))/*float*/;
    }
  }
  for ((i__608 = 0)/*int*/; (i__608 < 32); (i__608++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__606);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__609_111__104(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__606 = 0.0f;/* float */
      int i__conflict__0__607 = 0;/* int */
      int i__608 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__606 = ((float)0.0))/*float*/;
      for ((i__conflict__0__607 = 0)/*int*/; (i__conflict__0__607 < -68); (i__conflict__0__607++)) {{
          (sum__606 = (sum__606 + (__peek__104(i__conflict__0__607) * __peek__104((i__conflict__0__607 + 100)))))/*float*/;
        }
      }
      for ((i__608 = 0)/*int*/; (i__608 < 32); (i__608++)) {{
          __pop__104();
        }
      }
      __push__104(sum__606);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_105;
int __counter_105 = 0;
int __steady_105 = 0;
int __tmp_105 = 0;
int __tmp2_105 = 0;
int *__state_flag_105 = NULL;
thread_info *__thread_105 = NULL;



void save_peek_buffer__105(object_write_buffer *buf);
void load_peek_buffer__105(object_write_buffer *buf);
void save_file_pointer__105(object_write_buffer *buf);
void load_file_pointer__105(object_write_buffer *buf);

 
void init_AnonFilter_a0__615_112__105();
inline void check_status__105();

void work_AnonFilter_a0__615_112__105(int);


inline float __pop__105() {
float res=BUFFER_1_105[TAIL_1_105];
TAIL_1_105++;
return res;
}

inline float __pop__105(int n) {
float res=BUFFER_1_105[TAIL_1_105];
TAIL_1_105+=n;

return res;
}

inline float __peek__105(int offs) {
return BUFFER_1_105[TAIL_1_105+offs];
}



inline void __push__105(float data) {
BUFFER_105_3[HEAD_105_3]=data;
HEAD_105_3++;
}



 
void init_AnonFilter_a0__615_112__105(){
}
void save_file_pointer__105(object_write_buffer *buf) {}
void load_file_pointer__105(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__615_112__105__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__612 = 0.0f;/* float */
  int i__conflict__0__613 = 0;/* int */
  int i__614 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__612 = ((float)0.0))/*float*/;
  for ((i__conflict__0__613 = 0)/*int*/; (i__conflict__0__613 < -69); (i__conflict__0__613++)) {{
      (sum__612 = (sum__612 + ((*(____in+i__conflict__0__613)) * (*(____in+(i__conflict__0__613 + 101))))))/*float*/;
    }
  }
  for ((i__614 = 0)/*int*/; (i__614 < 32); (i__614++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__612);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__615_112__105__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__612 = 0.0f;/* float */
  int i__conflict__0__613 = 0;/* int */
  int i__614 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__612 = ((float)0.0))/*float*/;
  for ((i__conflict__0__613 = 0)/*int*/; (i__conflict__0__613 < -69); (i__conflict__0__613++)) {{
      (sum__612 = (sum__612 + ((*(____in+i__conflict__0__613)) * (*(____in+(i__conflict__0__613 + 101))))))/*float*/;
    }
  }
  for ((i__614 = 0)/*int*/; (i__614 < 32); (i__614++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__612);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__615_112__105(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__612 = 0.0f;/* float */
      int i__conflict__0__613 = 0;/* int */
      int i__614 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__612 = ((float)0.0))/*float*/;
      for ((i__conflict__0__613 = 0)/*int*/; (i__conflict__0__613 < -69); (i__conflict__0__613++)) {{
          (sum__612 = (sum__612 + (__peek__105(i__conflict__0__613) * __peek__105((i__conflict__0__613 + 101)))))/*float*/;
        }
      }
      for ((i__614 = 0)/*int*/; (i__614 < 32); (i__614++)) {{
          __pop__105();
        }
      }
      __push__105(sum__612);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_106;
int __counter_106 = 0;
int __steady_106 = 0;
int __tmp_106 = 0;
int __tmp2_106 = 0;
int *__state_flag_106 = NULL;
thread_info *__thread_106 = NULL;



void save_peek_buffer__106(object_write_buffer *buf);
void load_peek_buffer__106(object_write_buffer *buf);
void save_file_pointer__106(object_write_buffer *buf);
void load_file_pointer__106(object_write_buffer *buf);

 
void init_AnonFilter_a0__621_113__106();
inline void check_status__106();

void work_AnonFilter_a0__621_113__106(int);


inline float __pop__106() {
float res=BUFFER_1_106[TAIL_1_106];
TAIL_1_106++;
return res;
}

inline float __pop__106(int n) {
float res=BUFFER_1_106[TAIL_1_106];
TAIL_1_106+=n;

return res;
}

inline float __peek__106(int offs) {
return BUFFER_1_106[TAIL_1_106+offs];
}



inline void __push__106(float data) {
BUFFER_106_3[HEAD_106_3]=data;
HEAD_106_3++;
}



 
void init_AnonFilter_a0__621_113__106(){
}
void save_file_pointer__106(object_write_buffer *buf) {}
void load_file_pointer__106(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__621_113__106__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__618 = 0.0f;/* float */
  int i__conflict__0__619 = 0;/* int */
  int i__620 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__618 = ((float)0.0))/*float*/;
  for ((i__conflict__0__619 = 0)/*int*/; (i__conflict__0__619 < -70); (i__conflict__0__619++)) {{
      (sum__618 = (sum__618 + ((*(____in+i__conflict__0__619)) * (*(____in+(i__conflict__0__619 + 102))))))/*float*/;
    }
  }
  for ((i__620 = 0)/*int*/; (i__620 < 32); (i__620++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__618);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__621_113__106__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__618 = 0.0f;/* float */
  int i__conflict__0__619 = 0;/* int */
  int i__620 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__618 = ((float)0.0))/*float*/;
  for ((i__conflict__0__619 = 0)/*int*/; (i__conflict__0__619 < -70); (i__conflict__0__619++)) {{
      (sum__618 = (sum__618 + ((*(____in+i__conflict__0__619)) * (*(____in+(i__conflict__0__619 + 102))))))/*float*/;
    }
  }
  for ((i__620 = 0)/*int*/; (i__620 < 32); (i__620++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__618);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__621_113__106(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__618 = 0.0f;/* float */
      int i__conflict__0__619 = 0;/* int */
      int i__620 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__618 = ((float)0.0))/*float*/;
      for ((i__conflict__0__619 = 0)/*int*/; (i__conflict__0__619 < -70); (i__conflict__0__619++)) {{
          (sum__618 = (sum__618 + (__peek__106(i__conflict__0__619) * __peek__106((i__conflict__0__619 + 102)))))/*float*/;
        }
      }
      for ((i__620 = 0)/*int*/; (i__620 < 32); (i__620++)) {{
          __pop__106();
        }
      }
      __push__106(sum__618);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_107;
int __counter_107 = 0;
int __steady_107 = 0;
int __tmp_107 = 0;
int __tmp2_107 = 0;
int *__state_flag_107 = NULL;
thread_info *__thread_107 = NULL;



void save_peek_buffer__107(object_write_buffer *buf);
void load_peek_buffer__107(object_write_buffer *buf);
void save_file_pointer__107(object_write_buffer *buf);
void load_file_pointer__107(object_write_buffer *buf);

 
void init_AnonFilter_a0__627_114__107();
inline void check_status__107();

void work_AnonFilter_a0__627_114__107(int);


inline float __pop__107() {
float res=BUFFER_1_107[TAIL_1_107];
TAIL_1_107++;
return res;
}

inline float __pop__107(int n) {
float res=BUFFER_1_107[TAIL_1_107];
TAIL_1_107+=n;

return res;
}

inline float __peek__107(int offs) {
return BUFFER_1_107[TAIL_1_107+offs];
}



inline void __push__107(float data) {
BUFFER_107_3[HEAD_107_3]=data;
HEAD_107_3++;
}



 
void init_AnonFilter_a0__627_114__107(){
}
void save_file_pointer__107(object_write_buffer *buf) {}
void load_file_pointer__107(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__627_114__107__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__624 = 0.0f;/* float */
  int i__conflict__0__625 = 0;/* int */
  int i__626 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__624 = ((float)0.0))/*float*/;
  for ((i__conflict__0__625 = 0)/*int*/; (i__conflict__0__625 < -71); (i__conflict__0__625++)) {{
      (sum__624 = (sum__624 + ((*(____in+i__conflict__0__625)) * (*(____in+(i__conflict__0__625 + 103))))))/*float*/;
    }
  }
  for ((i__626 = 0)/*int*/; (i__626 < 32); (i__626++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__624);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__627_114__107__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__624 = 0.0f;/* float */
  int i__conflict__0__625 = 0;/* int */
  int i__626 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__624 = ((float)0.0))/*float*/;
  for ((i__conflict__0__625 = 0)/*int*/; (i__conflict__0__625 < -71); (i__conflict__0__625++)) {{
      (sum__624 = (sum__624 + ((*(____in+i__conflict__0__625)) * (*(____in+(i__conflict__0__625 + 103))))))/*float*/;
    }
  }
  for ((i__626 = 0)/*int*/; (i__626 < 32); (i__626++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__624);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__627_114__107(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__624 = 0.0f;/* float */
      int i__conflict__0__625 = 0;/* int */
      int i__626 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__624 = ((float)0.0))/*float*/;
      for ((i__conflict__0__625 = 0)/*int*/; (i__conflict__0__625 < -71); (i__conflict__0__625++)) {{
          (sum__624 = (sum__624 + (__peek__107(i__conflict__0__625) * __peek__107((i__conflict__0__625 + 103)))))/*float*/;
        }
      }
      for ((i__626 = 0)/*int*/; (i__626 < 32); (i__626++)) {{
          __pop__107();
        }
      }
      __push__107(sum__624);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_108;
int __counter_108 = 0;
int __steady_108 = 0;
int __tmp_108 = 0;
int __tmp2_108 = 0;
int *__state_flag_108 = NULL;
thread_info *__thread_108 = NULL;



void save_peek_buffer__108(object_write_buffer *buf);
void load_peek_buffer__108(object_write_buffer *buf);
void save_file_pointer__108(object_write_buffer *buf);
void load_file_pointer__108(object_write_buffer *buf);

 
void init_AnonFilter_a0__633_115__108();
inline void check_status__108();

void work_AnonFilter_a0__633_115__108(int);


inline float __pop__108() {
float res=BUFFER_1_108[TAIL_1_108];
TAIL_1_108++;
return res;
}

inline float __pop__108(int n) {
float res=BUFFER_1_108[TAIL_1_108];
TAIL_1_108+=n;

return res;
}

inline float __peek__108(int offs) {
return BUFFER_1_108[TAIL_1_108+offs];
}



inline void __push__108(float data) {
BUFFER_108_3[HEAD_108_3]=data;
HEAD_108_3++;
}



 
void init_AnonFilter_a0__633_115__108(){
}
void save_file_pointer__108(object_write_buffer *buf) {}
void load_file_pointer__108(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__633_115__108__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__630 = 0.0f;/* float */
  int i__conflict__0__631 = 0;/* int */
  int i__632 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__630 = ((float)0.0))/*float*/;
  for ((i__conflict__0__631 = 0)/*int*/; (i__conflict__0__631 < -72); (i__conflict__0__631++)) {{
      (sum__630 = (sum__630 + ((*(____in+i__conflict__0__631)) * (*(____in+(i__conflict__0__631 + 104))))))/*float*/;
    }
  }
  for ((i__632 = 0)/*int*/; (i__632 < 32); (i__632++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__630);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__633_115__108__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__630 = 0.0f;/* float */
  int i__conflict__0__631 = 0;/* int */
  int i__632 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__630 = ((float)0.0))/*float*/;
  for ((i__conflict__0__631 = 0)/*int*/; (i__conflict__0__631 < -72); (i__conflict__0__631++)) {{
      (sum__630 = (sum__630 + ((*(____in+i__conflict__0__631)) * (*(____in+(i__conflict__0__631 + 104))))))/*float*/;
    }
  }
  for ((i__632 = 0)/*int*/; (i__632 < 32); (i__632++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__630);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__633_115__108(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__630 = 0.0f;/* float */
      int i__conflict__0__631 = 0;/* int */
      int i__632 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__630 = ((float)0.0))/*float*/;
      for ((i__conflict__0__631 = 0)/*int*/; (i__conflict__0__631 < -72); (i__conflict__0__631++)) {{
          (sum__630 = (sum__630 + (__peek__108(i__conflict__0__631) * __peek__108((i__conflict__0__631 + 104)))))/*float*/;
        }
      }
      for ((i__632 = 0)/*int*/; (i__632 < 32); (i__632++)) {{
          __pop__108();
        }
      }
      __push__108(sum__630);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_109;
int __counter_109 = 0;
int __steady_109 = 0;
int __tmp_109 = 0;
int __tmp2_109 = 0;
int *__state_flag_109 = NULL;
thread_info *__thread_109 = NULL;



void save_peek_buffer__109(object_write_buffer *buf);
void load_peek_buffer__109(object_write_buffer *buf);
void save_file_pointer__109(object_write_buffer *buf);
void load_file_pointer__109(object_write_buffer *buf);

 
void init_AnonFilter_a0__639_116__109();
inline void check_status__109();

void work_AnonFilter_a0__639_116__109(int);


inline float __pop__109() {
float res=BUFFER_1_109[TAIL_1_109];
TAIL_1_109++;
return res;
}

inline float __pop__109(int n) {
float res=BUFFER_1_109[TAIL_1_109];
TAIL_1_109+=n;

return res;
}

inline float __peek__109(int offs) {
return BUFFER_1_109[TAIL_1_109+offs];
}



inline void __push__109(float data) {
BUFFER_109_3[HEAD_109_3]=data;
HEAD_109_3++;
}



 
void init_AnonFilter_a0__639_116__109(){
}
void save_file_pointer__109(object_write_buffer *buf) {}
void load_file_pointer__109(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__639_116__109__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__636 = 0.0f;/* float */
  int i__conflict__0__637 = 0;/* int */
  int i__638 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__636 = ((float)0.0))/*float*/;
  for ((i__conflict__0__637 = 0)/*int*/; (i__conflict__0__637 < -73); (i__conflict__0__637++)) {{
      (sum__636 = (sum__636 + ((*(____in+i__conflict__0__637)) * (*(____in+(i__conflict__0__637 + 105))))))/*float*/;
    }
  }
  for ((i__638 = 0)/*int*/; (i__638 < 32); (i__638++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__636);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__639_116__109__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__636 = 0.0f;/* float */
  int i__conflict__0__637 = 0;/* int */
  int i__638 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__636 = ((float)0.0))/*float*/;
  for ((i__conflict__0__637 = 0)/*int*/; (i__conflict__0__637 < -73); (i__conflict__0__637++)) {{
      (sum__636 = (sum__636 + ((*(____in+i__conflict__0__637)) * (*(____in+(i__conflict__0__637 + 105))))))/*float*/;
    }
  }
  for ((i__638 = 0)/*int*/; (i__638 < 32); (i__638++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__636);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__639_116__109(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__636 = 0.0f;/* float */
      int i__conflict__0__637 = 0;/* int */
      int i__638 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__636 = ((float)0.0))/*float*/;
      for ((i__conflict__0__637 = 0)/*int*/; (i__conflict__0__637 < -73); (i__conflict__0__637++)) {{
          (sum__636 = (sum__636 + (__peek__109(i__conflict__0__637) * __peek__109((i__conflict__0__637 + 105)))))/*float*/;
        }
      }
      for ((i__638 = 0)/*int*/; (i__638 < 32); (i__638++)) {{
          __pop__109();
        }
      }
      __push__109(sum__636);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_110;
int __counter_110 = 0;
int __steady_110 = 0;
int __tmp_110 = 0;
int __tmp2_110 = 0;
int *__state_flag_110 = NULL;
thread_info *__thread_110 = NULL;



void save_peek_buffer__110(object_write_buffer *buf);
void load_peek_buffer__110(object_write_buffer *buf);
void save_file_pointer__110(object_write_buffer *buf);
void load_file_pointer__110(object_write_buffer *buf);

 
void init_AnonFilter_a0__645_117__110();
inline void check_status__110();

void work_AnonFilter_a0__645_117__110(int);


inline float __pop__110() {
float res=BUFFER_1_110[TAIL_1_110];
TAIL_1_110++;
return res;
}

inline float __pop__110(int n) {
float res=BUFFER_1_110[TAIL_1_110];
TAIL_1_110+=n;

return res;
}

inline float __peek__110(int offs) {
return BUFFER_1_110[TAIL_1_110+offs];
}



inline void __push__110(float data) {
BUFFER_110_3[HEAD_110_3]=data;
HEAD_110_3++;
}



 
void init_AnonFilter_a0__645_117__110(){
}
void save_file_pointer__110(object_write_buffer *buf) {}
void load_file_pointer__110(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__645_117__110__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__642 = 0.0f;/* float */
  int i__conflict__0__643 = 0;/* int */
  int i__644 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__642 = ((float)0.0))/*float*/;
  for ((i__conflict__0__643 = 0)/*int*/; (i__conflict__0__643 < -74); (i__conflict__0__643++)) {{
      (sum__642 = (sum__642 + ((*(____in+i__conflict__0__643)) * (*(____in+(i__conflict__0__643 + 106))))))/*float*/;
    }
  }
  for ((i__644 = 0)/*int*/; (i__644 < 32); (i__644++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__642);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__645_117__110__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__642 = 0.0f;/* float */
  int i__conflict__0__643 = 0;/* int */
  int i__644 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__642 = ((float)0.0))/*float*/;
  for ((i__conflict__0__643 = 0)/*int*/; (i__conflict__0__643 < -74); (i__conflict__0__643++)) {{
      (sum__642 = (sum__642 + ((*(____in+i__conflict__0__643)) * (*(____in+(i__conflict__0__643 + 106))))))/*float*/;
    }
  }
  for ((i__644 = 0)/*int*/; (i__644 < 32); (i__644++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__642);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__645_117__110(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__642 = 0.0f;/* float */
      int i__conflict__0__643 = 0;/* int */
      int i__644 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__642 = ((float)0.0))/*float*/;
      for ((i__conflict__0__643 = 0)/*int*/; (i__conflict__0__643 < -74); (i__conflict__0__643++)) {{
          (sum__642 = (sum__642 + (__peek__110(i__conflict__0__643) * __peek__110((i__conflict__0__643 + 106)))))/*float*/;
        }
      }
      for ((i__644 = 0)/*int*/; (i__644 < 32); (i__644++)) {{
          __pop__110();
        }
      }
      __push__110(sum__642);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_111;
int __counter_111 = 0;
int __steady_111 = 0;
int __tmp_111 = 0;
int __tmp2_111 = 0;
int *__state_flag_111 = NULL;
thread_info *__thread_111 = NULL;



void save_peek_buffer__111(object_write_buffer *buf);
void load_peek_buffer__111(object_write_buffer *buf);
void save_file_pointer__111(object_write_buffer *buf);
void load_file_pointer__111(object_write_buffer *buf);

 
void init_AnonFilter_a0__651_118__111();
inline void check_status__111();

void work_AnonFilter_a0__651_118__111(int);


inline float __pop__111() {
float res=BUFFER_1_111[TAIL_1_111];
TAIL_1_111++;
return res;
}

inline float __pop__111(int n) {
float res=BUFFER_1_111[TAIL_1_111];
TAIL_1_111+=n;

return res;
}

inline float __peek__111(int offs) {
return BUFFER_1_111[TAIL_1_111+offs];
}



inline void __push__111(float data) {
BUFFER_111_3[HEAD_111_3]=data;
HEAD_111_3++;
}



 
void init_AnonFilter_a0__651_118__111(){
}
void save_file_pointer__111(object_write_buffer *buf) {}
void load_file_pointer__111(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__651_118__111__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__648 = 0.0f;/* float */
  int i__conflict__0__649 = 0;/* int */
  int i__650 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__648 = ((float)0.0))/*float*/;
  for ((i__conflict__0__649 = 0)/*int*/; (i__conflict__0__649 < -75); (i__conflict__0__649++)) {{
      (sum__648 = (sum__648 + ((*(____in+i__conflict__0__649)) * (*(____in+(i__conflict__0__649 + 107))))))/*float*/;
    }
  }
  for ((i__650 = 0)/*int*/; (i__650 < 32); (i__650++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__648);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__651_118__111__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__648 = 0.0f;/* float */
  int i__conflict__0__649 = 0;/* int */
  int i__650 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__648 = ((float)0.0))/*float*/;
  for ((i__conflict__0__649 = 0)/*int*/; (i__conflict__0__649 < -75); (i__conflict__0__649++)) {{
      (sum__648 = (sum__648 + ((*(____in+i__conflict__0__649)) * (*(____in+(i__conflict__0__649 + 107))))))/*float*/;
    }
  }
  for ((i__650 = 0)/*int*/; (i__650 < 32); (i__650++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__648);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__651_118__111(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__648 = 0.0f;/* float */
      int i__conflict__0__649 = 0;/* int */
      int i__650 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__648 = ((float)0.0))/*float*/;
      for ((i__conflict__0__649 = 0)/*int*/; (i__conflict__0__649 < -75); (i__conflict__0__649++)) {{
          (sum__648 = (sum__648 + (__peek__111(i__conflict__0__649) * __peek__111((i__conflict__0__649 + 107)))))/*float*/;
        }
      }
      for ((i__650 = 0)/*int*/; (i__650 < 32); (i__650++)) {{
          __pop__111();
        }
      }
      __push__111(sum__648);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_112;
int __counter_112 = 0;
int __steady_112 = 0;
int __tmp_112 = 0;
int __tmp2_112 = 0;
int *__state_flag_112 = NULL;
thread_info *__thread_112 = NULL;



void save_peek_buffer__112(object_write_buffer *buf);
void load_peek_buffer__112(object_write_buffer *buf);
void save_file_pointer__112(object_write_buffer *buf);
void load_file_pointer__112(object_write_buffer *buf);

 
void init_AnonFilter_a0__657_119__112();
inline void check_status__112();

void work_AnonFilter_a0__657_119__112(int);


inline float __pop__112() {
float res=BUFFER_1_112[TAIL_1_112];
TAIL_1_112++;
return res;
}

inline float __pop__112(int n) {
float res=BUFFER_1_112[TAIL_1_112];
TAIL_1_112+=n;

return res;
}

inline float __peek__112(int offs) {
return BUFFER_1_112[TAIL_1_112+offs];
}



inline void __push__112(float data) {
BUFFER_112_3[HEAD_112_3]=data;
HEAD_112_3++;
}



 
void init_AnonFilter_a0__657_119__112(){
}
void save_file_pointer__112(object_write_buffer *buf) {}
void load_file_pointer__112(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__657_119__112__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__654 = 0.0f;/* float */
  int i__conflict__0__655 = 0;/* int */
  int i__656 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__654 = ((float)0.0))/*float*/;
  for ((i__conflict__0__655 = 0)/*int*/; (i__conflict__0__655 < -76); (i__conflict__0__655++)) {{
      (sum__654 = (sum__654 + ((*(____in+i__conflict__0__655)) * (*(____in+(i__conflict__0__655 + 108))))))/*float*/;
    }
  }
  for ((i__656 = 0)/*int*/; (i__656 < 32); (i__656++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__654);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__657_119__112__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__654 = 0.0f;/* float */
  int i__conflict__0__655 = 0;/* int */
  int i__656 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__654 = ((float)0.0))/*float*/;
  for ((i__conflict__0__655 = 0)/*int*/; (i__conflict__0__655 < -76); (i__conflict__0__655++)) {{
      (sum__654 = (sum__654 + ((*(____in+i__conflict__0__655)) * (*(____in+(i__conflict__0__655 + 108))))))/*float*/;
    }
  }
  for ((i__656 = 0)/*int*/; (i__656 < 32); (i__656++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__654);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__657_119__112(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__654 = 0.0f;/* float */
      int i__conflict__0__655 = 0;/* int */
      int i__656 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__654 = ((float)0.0))/*float*/;
      for ((i__conflict__0__655 = 0)/*int*/; (i__conflict__0__655 < -76); (i__conflict__0__655++)) {{
          (sum__654 = (sum__654 + (__peek__112(i__conflict__0__655) * __peek__112((i__conflict__0__655 + 108)))))/*float*/;
        }
      }
      for ((i__656 = 0)/*int*/; (i__656 < 32); (i__656++)) {{
          __pop__112();
        }
      }
      __push__112(sum__654);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_113;
int __counter_113 = 0;
int __steady_113 = 0;
int __tmp_113 = 0;
int __tmp2_113 = 0;
int *__state_flag_113 = NULL;
thread_info *__thread_113 = NULL;



void save_peek_buffer__113(object_write_buffer *buf);
void load_peek_buffer__113(object_write_buffer *buf);
void save_file_pointer__113(object_write_buffer *buf);
void load_file_pointer__113(object_write_buffer *buf);

 
void init_AnonFilter_a0__663_120__113();
inline void check_status__113();

void work_AnonFilter_a0__663_120__113(int);


inline float __pop__113() {
float res=BUFFER_1_113[TAIL_1_113];
TAIL_1_113++;
return res;
}

inline float __pop__113(int n) {
float res=BUFFER_1_113[TAIL_1_113];
TAIL_1_113+=n;

return res;
}

inline float __peek__113(int offs) {
return BUFFER_1_113[TAIL_1_113+offs];
}



inline void __push__113(float data) {
BUFFER_113_3[HEAD_113_3]=data;
HEAD_113_3++;
}



 
void init_AnonFilter_a0__663_120__113(){
}
void save_file_pointer__113(object_write_buffer *buf) {}
void load_file_pointer__113(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__663_120__113__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__660 = 0.0f;/* float */
  int i__conflict__0__661 = 0;/* int */
  int i__662 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__660 = ((float)0.0))/*float*/;
  for ((i__conflict__0__661 = 0)/*int*/; (i__conflict__0__661 < -77); (i__conflict__0__661++)) {{
      (sum__660 = (sum__660 + ((*(____in+i__conflict__0__661)) * (*(____in+(i__conflict__0__661 + 109))))))/*float*/;
    }
  }
  for ((i__662 = 0)/*int*/; (i__662 < 32); (i__662++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__660);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__663_120__113__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__660 = 0.0f;/* float */
  int i__conflict__0__661 = 0;/* int */
  int i__662 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__660 = ((float)0.0))/*float*/;
  for ((i__conflict__0__661 = 0)/*int*/; (i__conflict__0__661 < -77); (i__conflict__0__661++)) {{
      (sum__660 = (sum__660 + ((*(____in+i__conflict__0__661)) * (*(____in+(i__conflict__0__661 + 109))))))/*float*/;
    }
  }
  for ((i__662 = 0)/*int*/; (i__662 < 32); (i__662++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__660);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__663_120__113(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__660 = 0.0f;/* float */
      int i__conflict__0__661 = 0;/* int */
      int i__662 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__660 = ((float)0.0))/*float*/;
      for ((i__conflict__0__661 = 0)/*int*/; (i__conflict__0__661 < -77); (i__conflict__0__661++)) {{
          (sum__660 = (sum__660 + (__peek__113(i__conflict__0__661) * __peek__113((i__conflict__0__661 + 109)))))/*float*/;
        }
      }
      for ((i__662 = 0)/*int*/; (i__662 < 32); (i__662++)) {{
          __pop__113();
        }
      }
      __push__113(sum__660);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_114;
int __counter_114 = 0;
int __steady_114 = 0;
int __tmp_114 = 0;
int __tmp2_114 = 0;
int *__state_flag_114 = NULL;
thread_info *__thread_114 = NULL;



void save_peek_buffer__114(object_write_buffer *buf);
void load_peek_buffer__114(object_write_buffer *buf);
void save_file_pointer__114(object_write_buffer *buf);
void load_file_pointer__114(object_write_buffer *buf);

 
void init_AnonFilter_a0__669_121__114();
inline void check_status__114();

void work_AnonFilter_a0__669_121__114(int);


inline float __pop__114() {
float res=BUFFER_1_114[TAIL_1_114];
TAIL_1_114++;
return res;
}

inline float __pop__114(int n) {
float res=BUFFER_1_114[TAIL_1_114];
TAIL_1_114+=n;

return res;
}

inline float __peek__114(int offs) {
return BUFFER_1_114[TAIL_1_114+offs];
}



inline void __push__114(float data) {
BUFFER_114_3[HEAD_114_3]=data;
HEAD_114_3++;
}



 
void init_AnonFilter_a0__669_121__114(){
}
void save_file_pointer__114(object_write_buffer *buf) {}
void load_file_pointer__114(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__669_121__114__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__666 = 0.0f;/* float */
  int i__conflict__0__667 = 0;/* int */
  int i__668 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__666 = ((float)0.0))/*float*/;
  for ((i__conflict__0__667 = 0)/*int*/; (i__conflict__0__667 < -78); (i__conflict__0__667++)) {{
      (sum__666 = (sum__666 + ((*(____in+i__conflict__0__667)) * (*(____in+(i__conflict__0__667 + 110))))))/*float*/;
    }
  }
  for ((i__668 = 0)/*int*/; (i__668 < 32); (i__668++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__666);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__669_121__114__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__666 = 0.0f;/* float */
  int i__conflict__0__667 = 0;/* int */
  int i__668 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__666 = ((float)0.0))/*float*/;
  for ((i__conflict__0__667 = 0)/*int*/; (i__conflict__0__667 < -78); (i__conflict__0__667++)) {{
      (sum__666 = (sum__666 + ((*(____in+i__conflict__0__667)) * (*(____in+(i__conflict__0__667 + 110))))))/*float*/;
    }
  }
  for ((i__668 = 0)/*int*/; (i__668 < 32); (i__668++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__666);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__669_121__114(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__666 = 0.0f;/* float */
      int i__conflict__0__667 = 0;/* int */
      int i__668 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__666 = ((float)0.0))/*float*/;
      for ((i__conflict__0__667 = 0)/*int*/; (i__conflict__0__667 < -78); (i__conflict__0__667++)) {{
          (sum__666 = (sum__666 + (__peek__114(i__conflict__0__667) * __peek__114((i__conflict__0__667 + 110)))))/*float*/;
        }
      }
      for ((i__668 = 0)/*int*/; (i__668 < 32); (i__668++)) {{
          __pop__114();
        }
      }
      __push__114(sum__666);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_115;
int __counter_115 = 0;
int __steady_115 = 0;
int __tmp_115 = 0;
int __tmp2_115 = 0;
int *__state_flag_115 = NULL;
thread_info *__thread_115 = NULL;



void save_peek_buffer__115(object_write_buffer *buf);
void load_peek_buffer__115(object_write_buffer *buf);
void save_file_pointer__115(object_write_buffer *buf);
void load_file_pointer__115(object_write_buffer *buf);

 
void init_AnonFilter_a0__675_122__115();
inline void check_status__115();

void work_AnonFilter_a0__675_122__115(int);


inline float __pop__115() {
float res=BUFFER_1_115[TAIL_1_115];
TAIL_1_115++;
return res;
}

inline float __pop__115(int n) {
float res=BUFFER_1_115[TAIL_1_115];
TAIL_1_115+=n;

return res;
}

inline float __peek__115(int offs) {
return BUFFER_1_115[TAIL_1_115+offs];
}



inline void __push__115(float data) {
BUFFER_115_3[HEAD_115_3]=data;
HEAD_115_3++;
}



 
void init_AnonFilter_a0__675_122__115(){
}
void save_file_pointer__115(object_write_buffer *buf) {}
void load_file_pointer__115(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__675_122__115__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__672 = 0.0f;/* float */
  int i__conflict__0__673 = 0;/* int */
  int i__674 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__672 = ((float)0.0))/*float*/;
  for ((i__conflict__0__673 = 0)/*int*/; (i__conflict__0__673 < -79); (i__conflict__0__673++)) {{
      (sum__672 = (sum__672 + ((*(____in+i__conflict__0__673)) * (*(____in+(i__conflict__0__673 + 111))))))/*float*/;
    }
  }
  for ((i__674 = 0)/*int*/; (i__674 < 32); (i__674++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__672);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__675_122__115__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__672 = 0.0f;/* float */
  int i__conflict__0__673 = 0;/* int */
  int i__674 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__672 = ((float)0.0))/*float*/;
  for ((i__conflict__0__673 = 0)/*int*/; (i__conflict__0__673 < -79); (i__conflict__0__673++)) {{
      (sum__672 = (sum__672 + ((*(____in+i__conflict__0__673)) * (*(____in+(i__conflict__0__673 + 111))))))/*float*/;
    }
  }
  for ((i__674 = 0)/*int*/; (i__674 < 32); (i__674++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__672);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__675_122__115(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__672 = 0.0f;/* float */
      int i__conflict__0__673 = 0;/* int */
      int i__674 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__672 = ((float)0.0))/*float*/;
      for ((i__conflict__0__673 = 0)/*int*/; (i__conflict__0__673 < -79); (i__conflict__0__673++)) {{
          (sum__672 = (sum__672 + (__peek__115(i__conflict__0__673) * __peek__115((i__conflict__0__673 + 111)))))/*float*/;
        }
      }
      for ((i__674 = 0)/*int*/; (i__674 < 32); (i__674++)) {{
          __pop__115();
        }
      }
      __push__115(sum__672);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_116;
int __counter_116 = 0;
int __steady_116 = 0;
int __tmp_116 = 0;
int __tmp2_116 = 0;
int *__state_flag_116 = NULL;
thread_info *__thread_116 = NULL;



void save_peek_buffer__116(object_write_buffer *buf);
void load_peek_buffer__116(object_write_buffer *buf);
void save_file_pointer__116(object_write_buffer *buf);
void load_file_pointer__116(object_write_buffer *buf);

 
void init_AnonFilter_a0__681_123__116();
inline void check_status__116();

void work_AnonFilter_a0__681_123__116(int);


inline float __pop__116() {
float res=BUFFER_1_116[TAIL_1_116];
TAIL_1_116++;
return res;
}

inline float __pop__116(int n) {
float res=BUFFER_1_116[TAIL_1_116];
TAIL_1_116+=n;

return res;
}

inline float __peek__116(int offs) {
return BUFFER_1_116[TAIL_1_116+offs];
}



inline void __push__116(float data) {
BUFFER_116_3[HEAD_116_3]=data;
HEAD_116_3++;
}



 
void init_AnonFilter_a0__681_123__116(){
}
void save_file_pointer__116(object_write_buffer *buf) {}
void load_file_pointer__116(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__681_123__116__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__678 = 0.0f;/* float */
  int i__conflict__0__679 = 0;/* int */
  int i__680 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__678 = ((float)0.0))/*float*/;
  for ((i__conflict__0__679 = 0)/*int*/; (i__conflict__0__679 < -80); (i__conflict__0__679++)) {{
      (sum__678 = (sum__678 + ((*(____in+i__conflict__0__679)) * (*(____in+(i__conflict__0__679 + 112))))))/*float*/;
    }
  }
  for ((i__680 = 0)/*int*/; (i__680 < 32); (i__680++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__678);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__681_123__116__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__678 = 0.0f;/* float */
  int i__conflict__0__679 = 0;/* int */
  int i__680 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__678 = ((float)0.0))/*float*/;
  for ((i__conflict__0__679 = 0)/*int*/; (i__conflict__0__679 < -80); (i__conflict__0__679++)) {{
      (sum__678 = (sum__678 + ((*(____in+i__conflict__0__679)) * (*(____in+(i__conflict__0__679 + 112))))))/*float*/;
    }
  }
  for ((i__680 = 0)/*int*/; (i__680 < 32); (i__680++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__678);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__681_123__116(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__678 = 0.0f;/* float */
      int i__conflict__0__679 = 0;/* int */
      int i__680 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__678 = ((float)0.0))/*float*/;
      for ((i__conflict__0__679 = 0)/*int*/; (i__conflict__0__679 < -80); (i__conflict__0__679++)) {{
          (sum__678 = (sum__678 + (__peek__116(i__conflict__0__679) * __peek__116((i__conflict__0__679 + 112)))))/*float*/;
        }
      }
      for ((i__680 = 0)/*int*/; (i__680 < 32); (i__680++)) {{
          __pop__116();
        }
      }
      __push__116(sum__678);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_117;
int __counter_117 = 0;
int __steady_117 = 0;
int __tmp_117 = 0;
int __tmp2_117 = 0;
int *__state_flag_117 = NULL;
thread_info *__thread_117 = NULL;



void save_peek_buffer__117(object_write_buffer *buf);
void load_peek_buffer__117(object_write_buffer *buf);
void save_file_pointer__117(object_write_buffer *buf);
void load_file_pointer__117(object_write_buffer *buf);

 
void init_AnonFilter_a0__687_124__117();
inline void check_status__117();

void work_AnonFilter_a0__687_124__117(int);


inline float __pop__117() {
float res=BUFFER_1_117[TAIL_1_117];
TAIL_1_117++;
return res;
}

inline float __pop__117(int n) {
float res=BUFFER_1_117[TAIL_1_117];
TAIL_1_117+=n;

return res;
}

inline float __peek__117(int offs) {
return BUFFER_1_117[TAIL_1_117+offs];
}



inline void __push__117(float data) {
BUFFER_117_3[HEAD_117_3]=data;
HEAD_117_3++;
}



 
void init_AnonFilter_a0__687_124__117(){
}
void save_file_pointer__117(object_write_buffer *buf) {}
void load_file_pointer__117(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__687_124__117__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__684 = 0.0f;/* float */
  int i__conflict__0__685 = 0;/* int */
  int i__686 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__684 = ((float)0.0))/*float*/;
  for ((i__conflict__0__685 = 0)/*int*/; (i__conflict__0__685 < -81); (i__conflict__0__685++)) {{
      (sum__684 = (sum__684 + ((*(____in+i__conflict__0__685)) * (*(____in+(i__conflict__0__685 + 113))))))/*float*/;
    }
  }
  for ((i__686 = 0)/*int*/; (i__686 < 32); (i__686++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__684);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__687_124__117__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__684 = 0.0f;/* float */
  int i__conflict__0__685 = 0;/* int */
  int i__686 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__684 = ((float)0.0))/*float*/;
  for ((i__conflict__0__685 = 0)/*int*/; (i__conflict__0__685 < -81); (i__conflict__0__685++)) {{
      (sum__684 = (sum__684 + ((*(____in+i__conflict__0__685)) * (*(____in+(i__conflict__0__685 + 113))))))/*float*/;
    }
  }
  for ((i__686 = 0)/*int*/; (i__686 < 32); (i__686++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__684);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__687_124__117(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__684 = 0.0f;/* float */
      int i__conflict__0__685 = 0;/* int */
      int i__686 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__684 = ((float)0.0))/*float*/;
      for ((i__conflict__0__685 = 0)/*int*/; (i__conflict__0__685 < -81); (i__conflict__0__685++)) {{
          (sum__684 = (sum__684 + (__peek__117(i__conflict__0__685) * __peek__117((i__conflict__0__685 + 113)))))/*float*/;
        }
      }
      for ((i__686 = 0)/*int*/; (i__686 < 32); (i__686++)) {{
          __pop__117();
        }
      }
      __push__117(sum__684);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_118;
int __counter_118 = 0;
int __steady_118 = 0;
int __tmp_118 = 0;
int __tmp2_118 = 0;
int *__state_flag_118 = NULL;
thread_info *__thread_118 = NULL;



void save_peek_buffer__118(object_write_buffer *buf);
void load_peek_buffer__118(object_write_buffer *buf);
void save_file_pointer__118(object_write_buffer *buf);
void load_file_pointer__118(object_write_buffer *buf);

 
void init_AnonFilter_a0__693_125__118();
inline void check_status__118();

void work_AnonFilter_a0__693_125__118(int);


inline float __pop__118() {
float res=BUFFER_1_118[TAIL_1_118];
TAIL_1_118++;
return res;
}

inline float __pop__118(int n) {
float res=BUFFER_1_118[TAIL_1_118];
TAIL_1_118+=n;

return res;
}

inline float __peek__118(int offs) {
return BUFFER_1_118[TAIL_1_118+offs];
}



inline void __push__118(float data) {
BUFFER_118_3[HEAD_118_3]=data;
HEAD_118_3++;
}



 
void init_AnonFilter_a0__693_125__118(){
}
void save_file_pointer__118(object_write_buffer *buf) {}
void load_file_pointer__118(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__693_125__118__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__690 = 0.0f;/* float */
  int i__conflict__0__691 = 0;/* int */
  int i__692 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__690 = ((float)0.0))/*float*/;
  for ((i__conflict__0__691 = 0)/*int*/; (i__conflict__0__691 < -82); (i__conflict__0__691++)) {{
      (sum__690 = (sum__690 + ((*(____in+i__conflict__0__691)) * (*(____in+(i__conflict__0__691 + 114))))))/*float*/;
    }
  }
  for ((i__692 = 0)/*int*/; (i__692 < 32); (i__692++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__690);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__693_125__118__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__690 = 0.0f;/* float */
  int i__conflict__0__691 = 0;/* int */
  int i__692 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__690 = ((float)0.0))/*float*/;
  for ((i__conflict__0__691 = 0)/*int*/; (i__conflict__0__691 < -82); (i__conflict__0__691++)) {{
      (sum__690 = (sum__690 + ((*(____in+i__conflict__0__691)) * (*(____in+(i__conflict__0__691 + 114))))))/*float*/;
    }
  }
  for ((i__692 = 0)/*int*/; (i__692 < 32); (i__692++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__690);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__693_125__118(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__690 = 0.0f;/* float */
      int i__conflict__0__691 = 0;/* int */
      int i__692 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__690 = ((float)0.0))/*float*/;
      for ((i__conflict__0__691 = 0)/*int*/; (i__conflict__0__691 < -82); (i__conflict__0__691++)) {{
          (sum__690 = (sum__690 + (__peek__118(i__conflict__0__691) * __peek__118((i__conflict__0__691 + 114)))))/*float*/;
        }
      }
      for ((i__692 = 0)/*int*/; (i__692 < 32); (i__692++)) {{
          __pop__118();
        }
      }
      __push__118(sum__690);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_119;
int __counter_119 = 0;
int __steady_119 = 0;
int __tmp_119 = 0;
int __tmp2_119 = 0;
int *__state_flag_119 = NULL;
thread_info *__thread_119 = NULL;



void save_peek_buffer__119(object_write_buffer *buf);
void load_peek_buffer__119(object_write_buffer *buf);
void save_file_pointer__119(object_write_buffer *buf);
void load_file_pointer__119(object_write_buffer *buf);

 
void init_AnonFilter_a0__699_126__119();
inline void check_status__119();

void work_AnonFilter_a0__699_126__119(int);


inline float __pop__119() {
float res=BUFFER_1_119[TAIL_1_119];
TAIL_1_119++;
return res;
}

inline float __pop__119(int n) {
float res=BUFFER_1_119[TAIL_1_119];
TAIL_1_119+=n;

return res;
}

inline float __peek__119(int offs) {
return BUFFER_1_119[TAIL_1_119+offs];
}



inline void __push__119(float data) {
BUFFER_119_3[HEAD_119_3]=data;
HEAD_119_3++;
}



 
void init_AnonFilter_a0__699_126__119(){
}
void save_file_pointer__119(object_write_buffer *buf) {}
void load_file_pointer__119(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__699_126__119__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__696 = 0.0f;/* float */
  int i__conflict__0__697 = 0;/* int */
  int i__698 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__696 = ((float)0.0))/*float*/;
  for ((i__conflict__0__697 = 0)/*int*/; (i__conflict__0__697 < -83); (i__conflict__0__697++)) {{
      (sum__696 = (sum__696 + ((*(____in+i__conflict__0__697)) * (*(____in+(i__conflict__0__697 + 115))))))/*float*/;
    }
  }
  for ((i__698 = 0)/*int*/; (i__698 < 32); (i__698++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__696);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__699_126__119__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__696 = 0.0f;/* float */
  int i__conflict__0__697 = 0;/* int */
  int i__698 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__696 = ((float)0.0))/*float*/;
  for ((i__conflict__0__697 = 0)/*int*/; (i__conflict__0__697 < -83); (i__conflict__0__697++)) {{
      (sum__696 = (sum__696 + ((*(____in+i__conflict__0__697)) * (*(____in+(i__conflict__0__697 + 115))))))/*float*/;
    }
  }
  for ((i__698 = 0)/*int*/; (i__698 < 32); (i__698++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__696);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__699_126__119(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__696 = 0.0f;/* float */
      int i__conflict__0__697 = 0;/* int */
      int i__698 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__696 = ((float)0.0))/*float*/;
      for ((i__conflict__0__697 = 0)/*int*/; (i__conflict__0__697 < -83); (i__conflict__0__697++)) {{
          (sum__696 = (sum__696 + (__peek__119(i__conflict__0__697) * __peek__119((i__conflict__0__697 + 115)))))/*float*/;
        }
      }
      for ((i__698 = 0)/*int*/; (i__698 < 32); (i__698++)) {{
          __pop__119();
        }
      }
      __push__119(sum__696);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_120;
int __counter_120 = 0;
int __steady_120 = 0;
int __tmp_120 = 0;
int __tmp2_120 = 0;
int *__state_flag_120 = NULL;
thread_info *__thread_120 = NULL;



void save_peek_buffer__120(object_write_buffer *buf);
void load_peek_buffer__120(object_write_buffer *buf);
void save_file_pointer__120(object_write_buffer *buf);
void load_file_pointer__120(object_write_buffer *buf);

 
void init_AnonFilter_a0__705_127__120();
inline void check_status__120();

void work_AnonFilter_a0__705_127__120(int);


inline float __pop__120() {
float res=BUFFER_1_120[TAIL_1_120];
TAIL_1_120++;
return res;
}

inline float __pop__120(int n) {
float res=BUFFER_1_120[TAIL_1_120];
TAIL_1_120+=n;

return res;
}

inline float __peek__120(int offs) {
return BUFFER_1_120[TAIL_1_120+offs];
}



inline void __push__120(float data) {
BUFFER_120_3[HEAD_120_3]=data;
HEAD_120_3++;
}



 
void init_AnonFilter_a0__705_127__120(){
}
void save_file_pointer__120(object_write_buffer *buf) {}
void load_file_pointer__120(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__705_127__120__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__702 = 0.0f;/* float */
  int i__conflict__0__703 = 0;/* int */
  int i__704 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__702 = ((float)0.0))/*float*/;
  for ((i__conflict__0__703 = 0)/*int*/; (i__conflict__0__703 < -84); (i__conflict__0__703++)) {{
      (sum__702 = (sum__702 + ((*(____in+i__conflict__0__703)) * (*(____in+(i__conflict__0__703 + 116))))))/*float*/;
    }
  }
  for ((i__704 = 0)/*int*/; (i__704 < 32); (i__704++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__702);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__705_127__120__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__702 = 0.0f;/* float */
  int i__conflict__0__703 = 0;/* int */
  int i__704 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__702 = ((float)0.0))/*float*/;
  for ((i__conflict__0__703 = 0)/*int*/; (i__conflict__0__703 < -84); (i__conflict__0__703++)) {{
      (sum__702 = (sum__702 + ((*(____in+i__conflict__0__703)) * (*(____in+(i__conflict__0__703 + 116))))))/*float*/;
    }
  }
  for ((i__704 = 0)/*int*/; (i__704 < 32); (i__704++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__702);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__705_127__120(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__702 = 0.0f;/* float */
      int i__conflict__0__703 = 0;/* int */
      int i__704 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__702 = ((float)0.0))/*float*/;
      for ((i__conflict__0__703 = 0)/*int*/; (i__conflict__0__703 < -84); (i__conflict__0__703++)) {{
          (sum__702 = (sum__702 + (__peek__120(i__conflict__0__703) * __peek__120((i__conflict__0__703 + 116)))))/*float*/;
        }
      }
      for ((i__704 = 0)/*int*/; (i__704 < 32); (i__704++)) {{
          __pop__120();
        }
      }
      __push__120(sum__702);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_121;
int __counter_121 = 0;
int __steady_121 = 0;
int __tmp_121 = 0;
int __tmp2_121 = 0;
int *__state_flag_121 = NULL;
thread_info *__thread_121 = NULL;



void save_peek_buffer__121(object_write_buffer *buf);
void load_peek_buffer__121(object_write_buffer *buf);
void save_file_pointer__121(object_write_buffer *buf);
void load_file_pointer__121(object_write_buffer *buf);

 
void init_AnonFilter_a0__711_128__121();
inline void check_status__121();

void work_AnonFilter_a0__711_128__121(int);


inline float __pop__121() {
float res=BUFFER_1_121[TAIL_1_121];
TAIL_1_121++;
return res;
}

inline float __pop__121(int n) {
float res=BUFFER_1_121[TAIL_1_121];
TAIL_1_121+=n;

return res;
}

inline float __peek__121(int offs) {
return BUFFER_1_121[TAIL_1_121+offs];
}



inline void __push__121(float data) {
BUFFER_121_3[HEAD_121_3]=data;
HEAD_121_3++;
}



 
void init_AnonFilter_a0__711_128__121(){
}
void save_file_pointer__121(object_write_buffer *buf) {}
void load_file_pointer__121(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__711_128__121__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__708 = 0.0f;/* float */
  int i__conflict__0__709 = 0;/* int */
  int i__710 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__708 = ((float)0.0))/*float*/;
  for ((i__conflict__0__709 = 0)/*int*/; (i__conflict__0__709 < -85); (i__conflict__0__709++)) {{
      (sum__708 = (sum__708 + ((*(____in+i__conflict__0__709)) * (*(____in+(i__conflict__0__709 + 117))))))/*float*/;
    }
  }
  for ((i__710 = 0)/*int*/; (i__710 < 32); (i__710++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__708);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__711_128__121__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__708 = 0.0f;/* float */
  int i__conflict__0__709 = 0;/* int */
  int i__710 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__708 = ((float)0.0))/*float*/;
  for ((i__conflict__0__709 = 0)/*int*/; (i__conflict__0__709 < -85); (i__conflict__0__709++)) {{
      (sum__708 = (sum__708 + ((*(____in+i__conflict__0__709)) * (*(____in+(i__conflict__0__709 + 117))))))/*float*/;
    }
  }
  for ((i__710 = 0)/*int*/; (i__710 < 32); (i__710++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__708);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__711_128__121(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__708 = 0.0f;/* float */
      int i__conflict__0__709 = 0;/* int */
      int i__710 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__708 = ((float)0.0))/*float*/;
      for ((i__conflict__0__709 = 0)/*int*/; (i__conflict__0__709 < -85); (i__conflict__0__709++)) {{
          (sum__708 = (sum__708 + (__peek__121(i__conflict__0__709) * __peek__121((i__conflict__0__709 + 117)))))/*float*/;
        }
      }
      for ((i__710 = 0)/*int*/; (i__710 < 32); (i__710++)) {{
          __pop__121();
        }
      }
      __push__121(sum__708);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_122;
int __counter_122 = 0;
int __steady_122 = 0;
int __tmp_122 = 0;
int __tmp2_122 = 0;
int *__state_flag_122 = NULL;
thread_info *__thread_122 = NULL;



void save_peek_buffer__122(object_write_buffer *buf);
void load_peek_buffer__122(object_write_buffer *buf);
void save_file_pointer__122(object_write_buffer *buf);
void load_file_pointer__122(object_write_buffer *buf);

 
void init_AnonFilter_a0__717_129__122();
inline void check_status__122();

void work_AnonFilter_a0__717_129__122(int);


inline float __pop__122() {
float res=BUFFER_1_122[TAIL_1_122];
TAIL_1_122++;
return res;
}

inline float __pop__122(int n) {
float res=BUFFER_1_122[TAIL_1_122];
TAIL_1_122+=n;

return res;
}

inline float __peek__122(int offs) {
return BUFFER_1_122[TAIL_1_122+offs];
}



inline void __push__122(float data) {
BUFFER_122_3[HEAD_122_3]=data;
HEAD_122_3++;
}



 
void init_AnonFilter_a0__717_129__122(){
}
void save_file_pointer__122(object_write_buffer *buf) {}
void load_file_pointer__122(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__717_129__122__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__714 = 0.0f;/* float */
  int i__conflict__0__715 = 0;/* int */
  int i__716 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__714 = ((float)0.0))/*float*/;
  for ((i__conflict__0__715 = 0)/*int*/; (i__conflict__0__715 < -86); (i__conflict__0__715++)) {{
      (sum__714 = (sum__714 + ((*(____in+i__conflict__0__715)) * (*(____in+(i__conflict__0__715 + 118))))))/*float*/;
    }
  }
  for ((i__716 = 0)/*int*/; (i__716 < 32); (i__716++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__714);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__717_129__122__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__714 = 0.0f;/* float */
  int i__conflict__0__715 = 0;/* int */
  int i__716 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__714 = ((float)0.0))/*float*/;
  for ((i__conflict__0__715 = 0)/*int*/; (i__conflict__0__715 < -86); (i__conflict__0__715++)) {{
      (sum__714 = (sum__714 + ((*(____in+i__conflict__0__715)) * (*(____in+(i__conflict__0__715 + 118))))))/*float*/;
    }
  }
  for ((i__716 = 0)/*int*/; (i__716 < 32); (i__716++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__714);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__717_129__122(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__714 = 0.0f;/* float */
      int i__conflict__0__715 = 0;/* int */
      int i__716 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__714 = ((float)0.0))/*float*/;
      for ((i__conflict__0__715 = 0)/*int*/; (i__conflict__0__715 < -86); (i__conflict__0__715++)) {{
          (sum__714 = (sum__714 + (__peek__122(i__conflict__0__715) * __peek__122((i__conflict__0__715 + 118)))))/*float*/;
        }
      }
      for ((i__716 = 0)/*int*/; (i__716 < 32); (i__716++)) {{
          __pop__122();
        }
      }
      __push__122(sum__714);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_123;
int __counter_123 = 0;
int __steady_123 = 0;
int __tmp_123 = 0;
int __tmp2_123 = 0;
int *__state_flag_123 = NULL;
thread_info *__thread_123 = NULL;



void save_peek_buffer__123(object_write_buffer *buf);
void load_peek_buffer__123(object_write_buffer *buf);
void save_file_pointer__123(object_write_buffer *buf);
void load_file_pointer__123(object_write_buffer *buf);

 
void init_AnonFilter_a0__723_130__123();
inline void check_status__123();

void work_AnonFilter_a0__723_130__123(int);


inline float __pop__123() {
float res=BUFFER_1_123[TAIL_1_123];
TAIL_1_123++;
return res;
}

inline float __pop__123(int n) {
float res=BUFFER_1_123[TAIL_1_123];
TAIL_1_123+=n;

return res;
}

inline float __peek__123(int offs) {
return BUFFER_1_123[TAIL_1_123+offs];
}



inline void __push__123(float data) {
BUFFER_123_3[HEAD_123_3]=data;
HEAD_123_3++;
}



 
void init_AnonFilter_a0__723_130__123(){
}
void save_file_pointer__123(object_write_buffer *buf) {}
void load_file_pointer__123(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__723_130__123__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__720 = 0.0f;/* float */
  int i__conflict__0__721 = 0;/* int */
  int i__722 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__720 = ((float)0.0))/*float*/;
  for ((i__conflict__0__721 = 0)/*int*/; (i__conflict__0__721 < -87); (i__conflict__0__721++)) {{
      (sum__720 = (sum__720 + ((*(____in+i__conflict__0__721)) * (*(____in+(i__conflict__0__721 + 119))))))/*float*/;
    }
  }
  for ((i__722 = 0)/*int*/; (i__722 < 32); (i__722++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__720);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__723_130__123__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__720 = 0.0f;/* float */
  int i__conflict__0__721 = 0;/* int */
  int i__722 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__720 = ((float)0.0))/*float*/;
  for ((i__conflict__0__721 = 0)/*int*/; (i__conflict__0__721 < -87); (i__conflict__0__721++)) {{
      (sum__720 = (sum__720 + ((*(____in+i__conflict__0__721)) * (*(____in+(i__conflict__0__721 + 119))))))/*float*/;
    }
  }
  for ((i__722 = 0)/*int*/; (i__722 < 32); (i__722++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__720);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__723_130__123(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__720 = 0.0f;/* float */
      int i__conflict__0__721 = 0;/* int */
      int i__722 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__720 = ((float)0.0))/*float*/;
      for ((i__conflict__0__721 = 0)/*int*/; (i__conflict__0__721 < -87); (i__conflict__0__721++)) {{
          (sum__720 = (sum__720 + (__peek__123(i__conflict__0__721) * __peek__123((i__conflict__0__721 + 119)))))/*float*/;
        }
      }
      for ((i__722 = 0)/*int*/; (i__722 < 32); (i__722++)) {{
          __pop__123();
        }
      }
      __push__123(sum__720);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_124;
int __counter_124 = 0;
int __steady_124 = 0;
int __tmp_124 = 0;
int __tmp2_124 = 0;
int *__state_flag_124 = NULL;
thread_info *__thread_124 = NULL;



void save_peek_buffer__124(object_write_buffer *buf);
void load_peek_buffer__124(object_write_buffer *buf);
void save_file_pointer__124(object_write_buffer *buf);
void load_file_pointer__124(object_write_buffer *buf);

 
void init_AnonFilter_a0__729_131__124();
inline void check_status__124();

void work_AnonFilter_a0__729_131__124(int);


inline float __pop__124() {
float res=BUFFER_1_124[TAIL_1_124];
TAIL_1_124++;
return res;
}

inline float __pop__124(int n) {
float res=BUFFER_1_124[TAIL_1_124];
TAIL_1_124+=n;

return res;
}

inline float __peek__124(int offs) {
return BUFFER_1_124[TAIL_1_124+offs];
}



inline void __push__124(float data) {
BUFFER_124_3[HEAD_124_3]=data;
HEAD_124_3++;
}



 
void init_AnonFilter_a0__729_131__124(){
}
void save_file_pointer__124(object_write_buffer *buf) {}
void load_file_pointer__124(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__729_131__124__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__726 = 0.0f;/* float */
  int i__conflict__0__727 = 0;/* int */
  int i__728 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__726 = ((float)0.0))/*float*/;
  for ((i__conflict__0__727 = 0)/*int*/; (i__conflict__0__727 < -88); (i__conflict__0__727++)) {{
      (sum__726 = (sum__726 + ((*(____in+i__conflict__0__727)) * (*(____in+(i__conflict__0__727 + 120))))))/*float*/;
    }
  }
  for ((i__728 = 0)/*int*/; (i__728 < 32); (i__728++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__726);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__729_131__124__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__726 = 0.0f;/* float */
  int i__conflict__0__727 = 0;/* int */
  int i__728 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__726 = ((float)0.0))/*float*/;
  for ((i__conflict__0__727 = 0)/*int*/; (i__conflict__0__727 < -88); (i__conflict__0__727++)) {{
      (sum__726 = (sum__726 + ((*(____in+i__conflict__0__727)) * (*(____in+(i__conflict__0__727 + 120))))))/*float*/;
    }
  }
  for ((i__728 = 0)/*int*/; (i__728 < 32); (i__728++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__726);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__729_131__124(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__726 = 0.0f;/* float */
      int i__conflict__0__727 = 0;/* int */
      int i__728 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__726 = ((float)0.0))/*float*/;
      for ((i__conflict__0__727 = 0)/*int*/; (i__conflict__0__727 < -88); (i__conflict__0__727++)) {{
          (sum__726 = (sum__726 + (__peek__124(i__conflict__0__727) * __peek__124((i__conflict__0__727 + 120)))))/*float*/;
        }
      }
      for ((i__728 = 0)/*int*/; (i__728 < 32); (i__728++)) {{
          __pop__124();
        }
      }
      __push__124(sum__726);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_125;
int __counter_125 = 0;
int __steady_125 = 0;
int __tmp_125 = 0;
int __tmp2_125 = 0;
int *__state_flag_125 = NULL;
thread_info *__thread_125 = NULL;



void save_peek_buffer__125(object_write_buffer *buf);
void load_peek_buffer__125(object_write_buffer *buf);
void save_file_pointer__125(object_write_buffer *buf);
void load_file_pointer__125(object_write_buffer *buf);

 
void init_AnonFilter_a0__735_132__125();
inline void check_status__125();

void work_AnonFilter_a0__735_132__125(int);


inline float __pop__125() {
float res=BUFFER_1_125[TAIL_1_125];
TAIL_1_125++;
return res;
}

inline float __pop__125(int n) {
float res=BUFFER_1_125[TAIL_1_125];
TAIL_1_125+=n;

return res;
}

inline float __peek__125(int offs) {
return BUFFER_1_125[TAIL_1_125+offs];
}



inline void __push__125(float data) {
BUFFER_125_3[HEAD_125_3]=data;
HEAD_125_3++;
}



 
void init_AnonFilter_a0__735_132__125(){
}
void save_file_pointer__125(object_write_buffer *buf) {}
void load_file_pointer__125(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__735_132__125__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__732 = 0.0f;/* float */
  int i__conflict__0__733 = 0;/* int */
  int i__734 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__732 = ((float)0.0))/*float*/;
  for ((i__conflict__0__733 = 0)/*int*/; (i__conflict__0__733 < -89); (i__conflict__0__733++)) {{
      (sum__732 = (sum__732 + ((*(____in+i__conflict__0__733)) * (*(____in+(i__conflict__0__733 + 121))))))/*float*/;
    }
  }
  for ((i__734 = 0)/*int*/; (i__734 < 32); (i__734++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__732);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__735_132__125__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__732 = 0.0f;/* float */
  int i__conflict__0__733 = 0;/* int */
  int i__734 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__732 = ((float)0.0))/*float*/;
  for ((i__conflict__0__733 = 0)/*int*/; (i__conflict__0__733 < -89); (i__conflict__0__733++)) {{
      (sum__732 = (sum__732 + ((*(____in+i__conflict__0__733)) * (*(____in+(i__conflict__0__733 + 121))))))/*float*/;
    }
  }
  for ((i__734 = 0)/*int*/; (i__734 < 32); (i__734++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__732);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__735_132__125(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__732 = 0.0f;/* float */
      int i__conflict__0__733 = 0;/* int */
      int i__734 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__732 = ((float)0.0))/*float*/;
      for ((i__conflict__0__733 = 0)/*int*/; (i__conflict__0__733 < -89); (i__conflict__0__733++)) {{
          (sum__732 = (sum__732 + (__peek__125(i__conflict__0__733) * __peek__125((i__conflict__0__733 + 121)))))/*float*/;
        }
      }
      for ((i__734 = 0)/*int*/; (i__734 < 32); (i__734++)) {{
          __pop__125();
        }
      }
      __push__125(sum__732);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_126;
int __counter_126 = 0;
int __steady_126 = 0;
int __tmp_126 = 0;
int __tmp2_126 = 0;
int *__state_flag_126 = NULL;
thread_info *__thread_126 = NULL;



void save_peek_buffer__126(object_write_buffer *buf);
void load_peek_buffer__126(object_write_buffer *buf);
void save_file_pointer__126(object_write_buffer *buf);
void load_file_pointer__126(object_write_buffer *buf);

 
void init_AnonFilter_a0__741_133__126();
inline void check_status__126();

void work_AnonFilter_a0__741_133__126(int);


inline float __pop__126() {
float res=BUFFER_1_126[TAIL_1_126];
TAIL_1_126++;
return res;
}

inline float __pop__126(int n) {
float res=BUFFER_1_126[TAIL_1_126];
TAIL_1_126+=n;

return res;
}

inline float __peek__126(int offs) {
return BUFFER_1_126[TAIL_1_126+offs];
}



inline void __push__126(float data) {
BUFFER_126_3[HEAD_126_3]=data;
HEAD_126_3++;
}



 
void init_AnonFilter_a0__741_133__126(){
}
void save_file_pointer__126(object_write_buffer *buf) {}
void load_file_pointer__126(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__741_133__126__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__738 = 0.0f;/* float */
  int i__conflict__0__739 = 0;/* int */
  int i__740 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__738 = ((float)0.0))/*float*/;
  for ((i__conflict__0__739 = 0)/*int*/; (i__conflict__0__739 < -90); (i__conflict__0__739++)) {{
      (sum__738 = (sum__738 + ((*(____in+i__conflict__0__739)) * (*(____in+(i__conflict__0__739 + 122))))))/*float*/;
    }
  }
  for ((i__740 = 0)/*int*/; (i__740 < 32); (i__740++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__738);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__741_133__126__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__738 = 0.0f;/* float */
  int i__conflict__0__739 = 0;/* int */
  int i__740 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__738 = ((float)0.0))/*float*/;
  for ((i__conflict__0__739 = 0)/*int*/; (i__conflict__0__739 < -90); (i__conflict__0__739++)) {{
      (sum__738 = (sum__738 + ((*(____in+i__conflict__0__739)) * (*(____in+(i__conflict__0__739 + 122))))))/*float*/;
    }
  }
  for ((i__740 = 0)/*int*/; (i__740 < 32); (i__740++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__738);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__741_133__126(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__738 = 0.0f;/* float */
      int i__conflict__0__739 = 0;/* int */
      int i__740 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__738 = ((float)0.0))/*float*/;
      for ((i__conflict__0__739 = 0)/*int*/; (i__conflict__0__739 < -90); (i__conflict__0__739++)) {{
          (sum__738 = (sum__738 + (__peek__126(i__conflict__0__739) * __peek__126((i__conflict__0__739 + 122)))))/*float*/;
        }
      }
      for ((i__740 = 0)/*int*/; (i__740 < 32); (i__740++)) {{
          __pop__126();
        }
      }
      __push__126(sum__738);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_127;
int __counter_127 = 0;
int __steady_127 = 0;
int __tmp_127 = 0;
int __tmp2_127 = 0;
int *__state_flag_127 = NULL;
thread_info *__thread_127 = NULL;



void save_peek_buffer__127(object_write_buffer *buf);
void load_peek_buffer__127(object_write_buffer *buf);
void save_file_pointer__127(object_write_buffer *buf);
void load_file_pointer__127(object_write_buffer *buf);

 
void init_AnonFilter_a0__747_134__127();
inline void check_status__127();

void work_AnonFilter_a0__747_134__127(int);


inline float __pop__127() {
float res=BUFFER_1_127[TAIL_1_127];
TAIL_1_127++;
return res;
}

inline float __pop__127(int n) {
float res=BUFFER_1_127[TAIL_1_127];
TAIL_1_127+=n;

return res;
}

inline float __peek__127(int offs) {
return BUFFER_1_127[TAIL_1_127+offs];
}



inline void __push__127(float data) {
BUFFER_127_3[HEAD_127_3]=data;
HEAD_127_3++;
}



 
void init_AnonFilter_a0__747_134__127(){
}
void save_file_pointer__127(object_write_buffer *buf) {}
void load_file_pointer__127(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__747_134__127__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__744 = 0.0f;/* float */
  int i__conflict__0__745 = 0;/* int */
  int i__746 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__744 = ((float)0.0))/*float*/;
  for ((i__conflict__0__745 = 0)/*int*/; (i__conflict__0__745 < -91); (i__conflict__0__745++)) {{
      (sum__744 = (sum__744 + ((*(____in+i__conflict__0__745)) * (*(____in+(i__conflict__0__745 + 123))))))/*float*/;
    }
  }
  for ((i__746 = 0)/*int*/; (i__746 < 32); (i__746++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__744);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__747_134__127__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__744 = 0.0f;/* float */
  int i__conflict__0__745 = 0;/* int */
  int i__746 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__744 = ((float)0.0))/*float*/;
  for ((i__conflict__0__745 = 0)/*int*/; (i__conflict__0__745 < -91); (i__conflict__0__745++)) {{
      (sum__744 = (sum__744 + ((*(____in+i__conflict__0__745)) * (*(____in+(i__conflict__0__745 + 123))))))/*float*/;
    }
  }
  for ((i__746 = 0)/*int*/; (i__746 < 32); (i__746++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__744);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__747_134__127(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__744 = 0.0f;/* float */
      int i__conflict__0__745 = 0;/* int */
      int i__746 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__744 = ((float)0.0))/*float*/;
      for ((i__conflict__0__745 = 0)/*int*/; (i__conflict__0__745 < -91); (i__conflict__0__745++)) {{
          (sum__744 = (sum__744 + (__peek__127(i__conflict__0__745) * __peek__127((i__conflict__0__745 + 123)))))/*float*/;
        }
      }
      for ((i__746 = 0)/*int*/; (i__746 < 32); (i__746++)) {{
          __pop__127();
        }
      }
      __push__127(sum__744);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_128;
int __counter_128 = 0;
int __steady_128 = 0;
int __tmp_128 = 0;
int __tmp2_128 = 0;
int *__state_flag_128 = NULL;
thread_info *__thread_128 = NULL;



void save_peek_buffer__128(object_write_buffer *buf);
void load_peek_buffer__128(object_write_buffer *buf);
void save_file_pointer__128(object_write_buffer *buf);
void load_file_pointer__128(object_write_buffer *buf);

 
void init_AnonFilter_a0__753_135__128();
inline void check_status__128();

void work_AnonFilter_a0__753_135__128(int);


inline float __pop__128() {
float res=BUFFER_1_128[TAIL_1_128];
TAIL_1_128++;
return res;
}

inline float __pop__128(int n) {
float res=BUFFER_1_128[TAIL_1_128];
TAIL_1_128+=n;

return res;
}

inline float __peek__128(int offs) {
return BUFFER_1_128[TAIL_1_128+offs];
}



inline void __push__128(float data) {
BUFFER_128_3[HEAD_128_3]=data;
HEAD_128_3++;
}



 
void init_AnonFilter_a0__753_135__128(){
}
void save_file_pointer__128(object_write_buffer *buf) {}
void load_file_pointer__128(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__753_135__128__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__750 = 0.0f;/* float */
  int i__conflict__0__751 = 0;/* int */
  int i__752 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__750 = ((float)0.0))/*float*/;
  for ((i__conflict__0__751 = 0)/*int*/; (i__conflict__0__751 < -92); (i__conflict__0__751++)) {{
      (sum__750 = (sum__750 + ((*(____in+i__conflict__0__751)) * (*(____in+(i__conflict__0__751 + 124))))))/*float*/;
    }
  }
  for ((i__752 = 0)/*int*/; (i__752 < 32); (i__752++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__750);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__753_135__128__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__750 = 0.0f;/* float */
  int i__conflict__0__751 = 0;/* int */
  int i__752 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__750 = ((float)0.0))/*float*/;
  for ((i__conflict__0__751 = 0)/*int*/; (i__conflict__0__751 < -92); (i__conflict__0__751++)) {{
      (sum__750 = (sum__750 + ((*(____in+i__conflict__0__751)) * (*(____in+(i__conflict__0__751 + 124))))))/*float*/;
    }
  }
  for ((i__752 = 0)/*int*/; (i__752 < 32); (i__752++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__750);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__753_135__128(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__750 = 0.0f;/* float */
      int i__conflict__0__751 = 0;/* int */
      int i__752 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__750 = ((float)0.0))/*float*/;
      for ((i__conflict__0__751 = 0)/*int*/; (i__conflict__0__751 < -92); (i__conflict__0__751++)) {{
          (sum__750 = (sum__750 + (__peek__128(i__conflict__0__751) * __peek__128((i__conflict__0__751 + 124)))))/*float*/;
        }
      }
      for ((i__752 = 0)/*int*/; (i__752 < 32); (i__752++)) {{
          __pop__128();
        }
      }
      __push__128(sum__750);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_129;
int __counter_129 = 0;
int __steady_129 = 0;
int __tmp_129 = 0;
int __tmp2_129 = 0;
int *__state_flag_129 = NULL;
thread_info *__thread_129 = NULL;



void save_peek_buffer__129(object_write_buffer *buf);
void load_peek_buffer__129(object_write_buffer *buf);
void save_file_pointer__129(object_write_buffer *buf);
void load_file_pointer__129(object_write_buffer *buf);

 
void init_AnonFilter_a0__759_136__129();
inline void check_status__129();

void work_AnonFilter_a0__759_136__129(int);


inline float __pop__129() {
float res=BUFFER_1_129[TAIL_1_129];
TAIL_1_129++;
return res;
}

inline float __pop__129(int n) {
float res=BUFFER_1_129[TAIL_1_129];
TAIL_1_129+=n;

return res;
}

inline float __peek__129(int offs) {
return BUFFER_1_129[TAIL_1_129+offs];
}



inline void __push__129(float data) {
BUFFER_129_3[HEAD_129_3]=data;
HEAD_129_3++;
}



 
void init_AnonFilter_a0__759_136__129(){
}
void save_file_pointer__129(object_write_buffer *buf) {}
void load_file_pointer__129(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__759_136__129__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__756 = 0.0f;/* float */
  int i__conflict__0__757 = 0;/* int */
  int i__758 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__756 = ((float)0.0))/*float*/;
  for ((i__conflict__0__757 = 0)/*int*/; (i__conflict__0__757 < -93); (i__conflict__0__757++)) {{
      (sum__756 = (sum__756 + ((*(____in+i__conflict__0__757)) * (*(____in+(i__conflict__0__757 + 125))))))/*float*/;
    }
  }
  for ((i__758 = 0)/*int*/; (i__758 < 32); (i__758++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__756);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__759_136__129__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__756 = 0.0f;/* float */
  int i__conflict__0__757 = 0;/* int */
  int i__758 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__756 = ((float)0.0))/*float*/;
  for ((i__conflict__0__757 = 0)/*int*/; (i__conflict__0__757 < -93); (i__conflict__0__757++)) {{
      (sum__756 = (sum__756 + ((*(____in+i__conflict__0__757)) * (*(____in+(i__conflict__0__757 + 125))))))/*float*/;
    }
  }
  for ((i__758 = 0)/*int*/; (i__758 < 32); (i__758++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__756);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__759_136__129(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__756 = 0.0f;/* float */
      int i__conflict__0__757 = 0;/* int */
      int i__758 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__756 = ((float)0.0))/*float*/;
      for ((i__conflict__0__757 = 0)/*int*/; (i__conflict__0__757 < -93); (i__conflict__0__757++)) {{
          (sum__756 = (sum__756 + (__peek__129(i__conflict__0__757) * __peek__129((i__conflict__0__757 + 125)))))/*float*/;
        }
      }
      for ((i__758 = 0)/*int*/; (i__758 < 32); (i__758++)) {{
          __pop__129();
        }
      }
      __push__129(sum__756);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_130;
int __counter_130 = 0;
int __steady_130 = 0;
int __tmp_130 = 0;
int __tmp2_130 = 0;
int *__state_flag_130 = NULL;
thread_info *__thread_130 = NULL;



void save_peek_buffer__130(object_write_buffer *buf);
void load_peek_buffer__130(object_write_buffer *buf);
void save_file_pointer__130(object_write_buffer *buf);
void load_file_pointer__130(object_write_buffer *buf);

 
void init_AnonFilter_a0__765_137__130();
inline void check_status__130();

void work_AnonFilter_a0__765_137__130(int);


inline float __pop__130() {
float res=BUFFER_1_130[TAIL_1_130];
TAIL_1_130++;
return res;
}

inline float __pop__130(int n) {
float res=BUFFER_1_130[TAIL_1_130];
TAIL_1_130+=n;

return res;
}

inline float __peek__130(int offs) {
return BUFFER_1_130[TAIL_1_130+offs];
}



inline void __push__130(float data) {
BUFFER_130_3[HEAD_130_3]=data;
HEAD_130_3++;
}



 
void init_AnonFilter_a0__765_137__130(){
}
void save_file_pointer__130(object_write_buffer *buf) {}
void load_file_pointer__130(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__765_137__130__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__762 = 0.0f;/* float */
  int i__conflict__0__763 = 0;/* int */
  int i__764 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__762 = ((float)0.0))/*float*/;
  for ((i__conflict__0__763 = 0)/*int*/; (i__conflict__0__763 < -94); (i__conflict__0__763++)) {{
      (sum__762 = (sum__762 + ((*(____in+i__conflict__0__763)) * (*(____in+(i__conflict__0__763 + 126))))))/*float*/;
    }
  }
  for ((i__764 = 0)/*int*/; (i__764 < 32); (i__764++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__762);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__765_137__130__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__762 = 0.0f;/* float */
  int i__conflict__0__763 = 0;/* int */
  int i__764 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__762 = ((float)0.0))/*float*/;
  for ((i__conflict__0__763 = 0)/*int*/; (i__conflict__0__763 < -94); (i__conflict__0__763++)) {{
      (sum__762 = (sum__762 + ((*(____in+i__conflict__0__763)) * (*(____in+(i__conflict__0__763 + 126))))))/*float*/;
    }
  }
  for ((i__764 = 0)/*int*/; (i__764 < 32); (i__764++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__762);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__765_137__130(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__762 = 0.0f;/* float */
      int i__conflict__0__763 = 0;/* int */
      int i__764 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__762 = ((float)0.0))/*float*/;
      for ((i__conflict__0__763 = 0)/*int*/; (i__conflict__0__763 < -94); (i__conflict__0__763++)) {{
          (sum__762 = (sum__762 + (__peek__130(i__conflict__0__763) * __peek__130((i__conflict__0__763 + 126)))))/*float*/;
        }
      }
      for ((i__764 = 0)/*int*/; (i__764 < 32); (i__764++)) {{
          __pop__130();
        }
      }
      __push__130(sum__762);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

// peek: 32 pop: 32 push 1
// init counts: 0 steady counts: 1

// ClusterFusion isEliminated: false



int __number_of_iterations_131;
int __counter_131 = 0;
int __steady_131 = 0;
int __tmp_131 = 0;
int __tmp2_131 = 0;
int *__state_flag_131 = NULL;
thread_info *__thread_131 = NULL;



void save_peek_buffer__131(object_write_buffer *buf);
void load_peek_buffer__131(object_write_buffer *buf);
void save_file_pointer__131(object_write_buffer *buf);
void load_file_pointer__131(object_write_buffer *buf);

 
void init_AnonFilter_a0__771_138__131();
inline void check_status__131();

void work_AnonFilter_a0__771_138__131(int);


inline float __pop__131() {
float res=BUFFER_1_131[TAIL_1_131];
TAIL_1_131++;
return res;
}

inline float __pop__131(int n) {
float res=BUFFER_1_131[TAIL_1_131];
TAIL_1_131+=n;

return res;
}

inline float __peek__131(int offs) {
return BUFFER_1_131[TAIL_1_131+offs];
}



inline void __push__131(float data) {
BUFFER_131_3[HEAD_131_3]=data;
HEAD_131_3++;
}



 
void init_AnonFilter_a0__771_138__131(){
}
void save_file_pointer__131(object_write_buffer *buf) {}
void load_file_pointer__131(object_write_buffer *buf) {}


#ifdef BUFFER_MERGE


void work_AnonFilter_a0__771_138__131__mod(int ____n, float *____in, float *____out) {
  for (; (0 < ____n); ____n--)
{
  float sum__768 = 0.0f;/* float */
  int i__conflict__0__769 = 0;/* int */
  int i__770 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__768 = ((float)0.0))/*float*/;
  for ((i__conflict__0__769 = 0)/*int*/; (i__conflict__0__769 < -95); (i__conflict__0__769++)) {{
      (sum__768 = (sum__768 + ((*(____in+i__conflict__0__769)) * (*(____in+(i__conflict__0__769 + 127))))))/*float*/;
    }
  }
  for ((i__770 = 0)/*int*/; (i__770 < 32); (i__770++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__768);
  // mark end: SIRFilter AnonFilter_a0

}}


void work_AnonFilter_a0__771_138__131__mod2(int ____n, float *____in, float *____out, int s1, int s2) {
  for (; (0 < ____n); (____n--, ____in+=s1, ____out+=s2))
{
  float sum__768 = 0.0f;/* float */
  int i__conflict__0__769 = 0;/* int */
  int i__770 = 0;/* int */

  // mark begin: SIRFilter AnonFilter_a0

  (sum__768 = ((float)0.0))/*float*/;
  for ((i__conflict__0__769 = 0)/*int*/; (i__conflict__0__769 < -95); (i__conflict__0__769++)) {{
      (sum__768 = (sum__768 + ((*(____in+i__conflict__0__769)) * (*(____in+(i__conflict__0__769 + 127))))))/*float*/;
    }
  }
  for ((i__770 = 0)/*int*/; (i__770 < 32); (i__770++)) {{
      (*____in++);
    }
  }
  ((*____out++)=sum__768);
  // mark end: SIRFilter AnonFilter_a0

}}


#endif // BUFFER_MERGE


 
void work_AnonFilter_a0__771_138__131(int ____n){
  for (
  ; (0 < ____n); (____n--)) {{
      float sum__768 = 0.0f;/* float */
      int i__conflict__0__769 = 0;/* int */
      int i__770 = 0;/* int */

      // mark begin: SIRFilter AnonFilter_a0

      (sum__768 = ((float)0.0))/*float*/;
      for ((i__conflict__0__769 = 0)/*int*/; (i__conflict__0__769 < -95); (i__conflict__0__769++)) {{
          (sum__768 = (sum__768 + (__peek__131(i__conflict__0__769) * __peek__131((i__conflict__0__769 + 127)))))/*float*/;
        }
      }
      for ((i__770 = 0)/*int*/; (i__770 < 32); (i__770++)) {{
          __pop__131();
        }
      }
      __push__131(sum__768);
      // mark end: SIRFilter AnonFilter_a0

    }
  }
}

