#ifndef __FUSION_H
#define __FUSION_H

#define max(A,B) (((A)>(B))?(A):(B))
#define pow2ceil(A) ((A<=256)?(256):(((A<=1024)?(1024):(((A<=4096)?(4096):(((A<=16384)?(16384):(((A<=65536)?(65536):(((A<=131072)?(131072):(((A<=262144)?(262144):(((A<=524288)?(524288):(((A<=1048576)?(1048576):(((A<=2097152)?(2097152):(((A<=4194304)?(4194304):(((A<=8388608)?(8388608):(((A<=16777216)?(16777216):(((A<=33554432)?(33554432):(((A<=67108864)?(67108864):(((A<=134217728)?(134217728):(((A<=268435456)?(268435456):(((A<=536870912)?(536870912):(1073741824))))))))))))))))))))))))))))))))))))
#define __BUF_SIZE_MASK_0_1 (pow2ceil(16+0)-1)

#define __BUF_SIZE_MASK_1_2 (pow2ceil(16+0)-1)

#define __BUF_SIZE_MASK_1_6 (pow2ceil(16+0)-1)

#define __BUF_SIZE_MASK_1_7 (pow2ceil(16+0)-1)

#define __BUF_SIZE_MASK_1_8 (pow2ceil(16+0)-1)

#define __BUF_SIZE_MASK_1_9 (pow2ceil(16+0)-1)

#define __BUF_SIZE_MASK_1_10 (pow2ceil(16+0)-1)

#define __BUF_SIZE_MASK_1_11 (pow2ceil(16+0)-1)

#define __BUF_SIZE_MASK_1_12 (pow2ceil(16+0)-1)

#define __BUF_SIZE_MASK_1_13 (pow2ceil(16+0)-1)

#define __BUF_SIZE_MASK_1_14 (pow2ceil(16+0)-1)

#define __BUF_SIZE_MASK_1_15 (pow2ceil(16+0)-1)

#define __BUF_SIZE_MASK_1_16 (pow2ceil(16+0)-1)

#define __BUF_SIZE_MASK_1_17 (pow2ceil(16+0)-1)

#define __BUF_SIZE_MASK_1_18 (pow2ceil(16+0)-1)

#define __BUF_SIZE_MASK_1_19 (pow2ceil(16+0)-1)

#define __BUF_SIZE_MASK_1_20 (pow2ceil(16+0)-1)

#define __BUF_SIZE_MASK_1_21 (pow2ceil(16+0)-1)

#define __BUF_SIZE_MASK_2_3 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_3_4 (pow2ceil(32+0)-1)

#define __BUF_SIZE_MASK_4_5 (pow2ceil(16+0)-1)

#define __BUF_SIZE_MASK_6_3 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_7_3 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_8_3 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_9_3 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_10_3 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_11_3 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_12_3 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_13_3 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_14_3 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_15_3 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_16_3 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_17_3 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_18_3 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_19_3 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_20_3 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_21_3 (pow2ceil(16+0)-1)

#endif
