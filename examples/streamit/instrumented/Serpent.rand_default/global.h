#ifndef __GLOBAL_H
#define __GLOBAL_H

#include <math.h>
#include "structs.h"
#include <StreamItVectorLib.h>

#define max(A,B) (((A)>(B))?(A):(B))
#define min(A,B) (((A)<(B))?(A):(B))

extern void __global__init();

extern int __global__BITS_PER_WORD;
extern int __global__NBITS;
extern int __global__PHI;
extern int __global__MAXROUNDS;
extern bool __global__PRINTINFO;
extern int __global__PLAINTEXT;
extern int __global__USERKEY;
extern int __global__CIPHERTEXT;
extern int __global__USERKEYS[5][8];
extern int __global__USERKEY_LENGTH;
extern int __global__IP[128];
extern int __global__FP[128];
extern int __global__SBOXES[8][16];
#endif // __GLOBAL_H
