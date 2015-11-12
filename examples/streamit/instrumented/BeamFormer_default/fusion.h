#ifndef __FUSION_H
#define __FUSION_H

#define max(A,B) (((A)>(B))?(A):(B))
#define pow2ceil(A) ((A<=256)?(256):(((A<=1024)?(1024):(((A<=4096)?(4096):(((A<=16384)?(16384):(((A<=65536)?(65536):(((A<=131072)?(131072):(((A<=262144)?(262144):(((A<=524288)?(524288):(((A<=1048576)?(1048576):(((A<=2097152)?(2097152):(((A<=4194304)?(4194304):(((A<=8388608)?(8388608):(((A<=16777216)?(16777216):(((A<=33554432)?(33554432):(((A<=67108864)?(67108864):(((A<=134217728)?(134217728):(((A<=268435456)?(268435456):(((A<=536870912)?(536870912):(1073741824))))))))))))))))))))))))))))))))))))
#define __BUF_SIZE_MASK_0_1 (pow2ceil(72+0)-1)

#define __BUF_SIZE_MASK_1_2 (pow2ceil(6+0)-1)

#define __BUF_SIZE_MASK_1_25 (pow2ceil(6+0)-1)

#define __BUF_SIZE_MASK_1_28 (pow2ceil(6+0)-1)

#define __BUF_SIZE_MASK_1_31 (pow2ceil(6+0)-1)

#define __BUF_SIZE_MASK_1_34 (pow2ceil(6+0)-1)

#define __BUF_SIZE_MASK_1_37 (pow2ceil(6+0)-1)

#define __BUF_SIZE_MASK_1_40 (pow2ceil(6+0)-1)

#define __BUF_SIZE_MASK_1_43 (pow2ceil(6+0)-1)

#define __BUF_SIZE_MASK_1_46 (pow2ceil(6+0)-1)

#define __BUF_SIZE_MASK_1_49 (pow2ceil(6+0)-1)

#define __BUF_SIZE_MASK_1_52 (pow2ceil(6+0)-1)

#define __BUF_SIZE_MASK_1_55 (pow2ceil(6+0)-1)

#define __BUF_SIZE_MASK_2_3 (pow2ceil(12+0)-1)

#define __BUF_SIZE_MASK_3_4 (pow2ceil(6+0)-1)

#define __BUF_SIZE_MASK_4_5 (pow2ceil(2+0)-1)

#define __BUF_SIZE_MASK_5_6 (pow2ceil(24+0)-1)

#define __BUF_SIZE_MASK_6_7 (pow2ceil(24+0)-1)

#define __BUF_SIZE_MASK_6_13 (pow2ceil(24+0)-1)

#define __BUF_SIZE_MASK_6_17 (pow2ceil(24+0)-1)

#define __BUF_SIZE_MASK_6_21 (pow2ceil(24+0)-1)

#define __BUF_SIZE_MASK_7_8 (pow2ceil(2+0)-1)

#define __BUF_SIZE_MASK_8_9 (pow2ceil(2+0)-1)

#define __BUF_SIZE_MASK_9_10 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_10_11 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_11_12 (pow2ceil(4+0)-1)

#define __BUF_SIZE_MASK_13_14 (pow2ceil(2+0)-1)

#define __BUF_SIZE_MASK_14_15 (pow2ceil(2+0)-1)

#define __BUF_SIZE_MASK_15_16 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_16_11 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_17_18 (pow2ceil(2+0)-1)

#define __BUF_SIZE_MASK_18_19 (pow2ceil(2+0)-1)

#define __BUF_SIZE_MASK_19_20 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_20_11 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_21_22 (pow2ceil(2+0)-1)

#define __BUF_SIZE_MASK_22_23 (pow2ceil(2+0)-1)

#define __BUF_SIZE_MASK_23_24 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_24_11 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_25_26 (pow2ceil(12+0)-1)

#define __BUF_SIZE_MASK_26_27 (pow2ceil(6+0)-1)

#define __BUF_SIZE_MASK_27_5 (pow2ceil(2+0)-1)

#define __BUF_SIZE_MASK_28_29 (pow2ceil(12+0)-1)

#define __BUF_SIZE_MASK_29_30 (pow2ceil(6+0)-1)

#define __BUF_SIZE_MASK_30_5 (pow2ceil(2+0)-1)

#define __BUF_SIZE_MASK_31_32 (pow2ceil(12+0)-1)

#define __BUF_SIZE_MASK_32_33 (pow2ceil(6+0)-1)

#define __BUF_SIZE_MASK_33_5 (pow2ceil(2+0)-1)

#define __BUF_SIZE_MASK_34_35 (pow2ceil(12+0)-1)

#define __BUF_SIZE_MASK_35_36 (pow2ceil(6+0)-1)

#define __BUF_SIZE_MASK_36_5 (pow2ceil(2+0)-1)

#define __BUF_SIZE_MASK_37_38 (pow2ceil(12+0)-1)

#define __BUF_SIZE_MASK_38_39 (pow2ceil(6+0)-1)

#define __BUF_SIZE_MASK_39_5 (pow2ceil(2+0)-1)

#define __BUF_SIZE_MASK_40_41 (pow2ceil(12+0)-1)

#define __BUF_SIZE_MASK_41_42 (pow2ceil(6+0)-1)

#define __BUF_SIZE_MASK_42_5 (pow2ceil(2+0)-1)

#define __BUF_SIZE_MASK_43_44 (pow2ceil(12+0)-1)

#define __BUF_SIZE_MASK_44_45 (pow2ceil(6+0)-1)

#define __BUF_SIZE_MASK_45_5 (pow2ceil(2+0)-1)

#define __BUF_SIZE_MASK_46_47 (pow2ceil(12+0)-1)

#define __BUF_SIZE_MASK_47_48 (pow2ceil(6+0)-1)

#define __BUF_SIZE_MASK_48_5 (pow2ceil(2+0)-1)

#define __BUF_SIZE_MASK_49_50 (pow2ceil(12+0)-1)

#define __BUF_SIZE_MASK_50_51 (pow2ceil(6+0)-1)

#define __BUF_SIZE_MASK_51_5 (pow2ceil(2+0)-1)

#define __BUF_SIZE_MASK_52_53 (pow2ceil(12+0)-1)

#define __BUF_SIZE_MASK_53_54 (pow2ceil(6+0)-1)

#define __BUF_SIZE_MASK_54_5 (pow2ceil(2+0)-1)

#define __BUF_SIZE_MASK_55_56 (pow2ceil(12+0)-1)

#define __BUF_SIZE_MASK_56_57 (pow2ceil(6+0)-1)

#define __BUF_SIZE_MASK_57_5 (pow2ceil(2+0)-1)

#endif
