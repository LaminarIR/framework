#ifndef __FUSION_H
#define __FUSION_H

#define max(A,B) (((A)>(B))?(A):(B))
#define pow2ceil(A) ((A<=256)?(256):(((A<=1024)?(1024):(((A<=4096)?(4096):(((A<=16384)?(16384):(((A<=65536)?(65536):(((A<=131072)?(131072):(((A<=262144)?(262144):(((A<=524288)?(524288):(((A<=1048576)?(1048576):(((A<=2097152)?(2097152):(((A<=4194304)?(4194304):(((A<=8388608)?(8388608):(((A<=16777216)?(16777216):(((A<=33554432)?(33554432):(((A<=67108864)?(67108864):(((A<=134217728)?(134217728):(((A<=268435456)?(268435456):(((A<=536870912)?(536870912):(1073741824))))))))))))))))))))))))))))))))))))
#define __BUF_SIZE_MASK_0_1 (pow2ceil(64+0)-1)

#define __BUF_SIZE_MASK_1_2 (pow2ceil(64+0)-1)

#define __BUF_SIZE_MASK_2_3 (pow2ceil(64+0)-1)

#define __BUF_SIZE_MASK_3_4 (pow2ceil(64+0)-1)

#define __BUF_SIZE_MASK_4_5 (pow2ceil(64+0)-1)

#define __BUF_SIZE_MASK_5_6 (pow2ceil(64+0)-1)

#define __BUF_SIZE_MASK_6_7 (pow2ceil(64+0)-1)

#define __BUF_SIZE_MASK_7_8 (pow2ceil(64+0)-1)

#define __BUF_SIZE_MASK_8_9 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_8_35 (pow2ceil(63+0)-1)

#define __BUF_SIZE_MASK_9_10 (pow2ceil(2+0)-1)

#define __BUF_SIZE_MASK_10_11 (pow2ceil(2+0)-1)

#define __BUF_SIZE_MASK_11_12 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_11_34 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_12_13 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_13_14 (pow2ceil(64+0)-1)

#define __BUF_SIZE_MASK_14_15 (pow2ceil(64+0)-1)

#define __BUF_SIZE_MASK_15_16 (pow2ceil(64+0)-1)

#define __BUF_SIZE_MASK_16_17 (pow2ceil(64+0)-1)

#define __BUF_SIZE_MASK_17_18 (pow2ceil(64+0)-1)

#define __BUF_SIZE_MASK_18_19 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_18_33 (pow2ceil(63+0)-1)

#define __BUF_SIZE_MASK_19_20 (pow2ceil(2+0)-1)

#define __BUF_SIZE_MASK_20_21 (pow2ceil(2+0)-1)

#define __BUF_SIZE_MASK_21_22 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_21_32 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_22_23 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_23_24 (pow2ceil(64+0)-1)

#define __BUF_SIZE_MASK_24_25 (pow2ceil(64+0)-1)

#define __BUF_SIZE_MASK_25_26 (pow2ceil(64+0)-1)

#define __BUF_SIZE_MASK_26_27 (pow2ceil(64+0)-1)

#define __BUF_SIZE_MASK_27_28 (pow2ceil(64+0)-1)

#define __BUF_SIZE_MASK_28_29 (pow2ceil(64+0)-1)

#define __BUF_SIZE_MASK_29_30 (pow2ceil(64+0)-1)

#define __BUF_SIZE_MASK_30_31 (pow2ceil(64+0)-1)

//destination peeks: 1 extra items
#define __PEEK_BUF_SIZE_32_19 1
#define __BUF_SIZE_MASK_32_19 (pow2ceil(1+1)-1)

#define __BUF_SIZE_MASK_33_23 (pow2ceil(63+0)-1)

//destination peeks: 1 extra items
#define __PEEK_BUF_SIZE_34_9 1
#define __BUF_SIZE_MASK_34_9 (pow2ceil(1+1)-1)

#define __BUF_SIZE_MASK_35_13 (pow2ceil(63+0)-1)

#endif
