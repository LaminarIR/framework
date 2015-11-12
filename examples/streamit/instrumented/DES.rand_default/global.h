#ifndef __GLOBAL_H
#define __GLOBAL_H

#include <math.h>
#include "structs.h"
#include <StreamItVectorLib.h>

#define max(A,B) (((A)>(B))?(A):(B))
#define min(A,B) (((A)<(B))?(A):(B))

extern void __global__init();

extern bool __global__PRINTINFO;
extern int __global__PLAINTEXT;
extern int __global__USERKEY;
extern int __global__CIPHERTEXT;
extern int __global__MAXROUNDS;
extern int __global__USERKEYS[34][2];
extern int __global__PC1[56];
extern int __global__PC2[48];
extern int __global__RT[16];
extern int __global__IP[64];
extern int __global__E[48];
extern int __global__P[32];
extern int __global__IPm1[64];
extern int __global__S1[4][16];
extern int __global__S2[4][16];
extern int __global__S3[4][16];
extern int __global__S4[4][16];
extern int __global__S5[4][16];
extern int __global__S6[4][16];
extern int __global__S7[4][16];
extern int __global__S8[4][16];
#endif // __GLOBAL_H
