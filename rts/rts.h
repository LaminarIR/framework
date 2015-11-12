#ifndef __RTS_H__
#define __RTS_H__

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#define OUTFILEID "times"
#define OUTFILEEXT ".out"
#define CACHELINE_SIZE 64
#define MAX_TASK_ID_LENGTH 32


/*
 * array of function pointers 
 */
typedef void (**t_rts_processes)(void); 
typedef void (*t_rts_procdecl[])(void); 

/*
 * type for time stamp
 */

#if defined (__i386__) || defined (__x86_64__)
  #define IS_x86 1
#else
  #define IS_x86 0
  #warning "WARNING: no clock counter support for the machine."
#endif
typedef struct {
    unsigned start_cyc_hi;
    unsigned start_cyc_lo;
    unsigned stop_cyc_hi;
    unsigned stop_cyc_lo;
} rts_time_stamp;

struct RTS_VARS{
  int pid;
  FILE *f;
  double RTS_CLOCK_RATE;

  int numtasks;
  char** ids;

  rts_time_stamp* _tmp_time; 
  double** _time;
}__attribute__((aligned(CACHELINE_SIZE)));

#ifdef ATOMIC_BARRIER
struct BVAR {
    volatile unsigned int b;
}__attribute__((aligned(CACHELINE_SIZE)));

static struct BVAR* thread_ctrs;
#endif

extern struct RTS_VARS* thr_vars __attribute__((aligned(CACHELINE_SIZE)));

extern int numcpus __attribute__((aligned(CACHELINE_SIZE)));
extern int numiters __attribute__((aligned(CACHELINE_SIZE)));

/*
 * init function
 */
void RTS_init(int _numcpus, int _numiters);
void RTS_thread_init(int tid, int _numtasks);


/*
 * error function
 */
void RTS_error(char *txt, ...);

/*
 * synchronize processes 
 */ 
void RTS_sync(int tid);
void RTS_wait(int mytid, int tid);
void RTS_sign(int tid);

/*
 * functions for time measurements
 */
void RTS_start_timer(int tid, int aid);
void RTS_stop_timer(int tid, int aid);
void RTS_record(char* id, int tid, int aid, int iter);
void RTS_write_time(int tid);

/* 
 * initialize pthreads and invoke work functions of processes 
 */ 
void RTS_run(int num_processes, t_rts_processes processes);

#endif
