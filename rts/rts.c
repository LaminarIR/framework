#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include "rts.h"

#ifdef ATOMIC_BARRIER
#ifdef __STD_NO_ATOMICS__
#error Atomic operations are not supported.
#endif
static int num_threads;
#endif

#ifdef PTHREAD_BARRIER
/*
 * Barrier variable 
 */
static pthread_barrier_t barrier;
#endif


/* 
 * arguments for thread function
 */
typedef struct rts_args {
    int num_processes;     /* number of processes */ 
    t_rts_processes processes; /* function array pointing to processes */
    int current;           /* current process id */ 
} rts_args;

/*
 * error function
 */

void RTS_error(char *txt, ...)
{
   va_list args;
   va_start (args, txt);
   fprintf(stderr,"rts error:");
   vfprintf(stderr,txt,args);
   fprintf(stderr,"\n");
   exit(1);
}


/*
 * Synchronize processes 
 */ 
#ifdef ATOMIC_BARRIER
void RTS_sync(int tid) 
{  
    int i;
    __atomic_add_fetch(&thread_ctrs[tid].b,1,5);
    for(i = 0; i < num_threads; i++){
        while(__atomic_load_n(&thread_ctrs[i].b,5)<thread_ctrs[tid].b);
    }
}

void RTS_wait(int mytid, int tid){
    while(__atomic_load_n(&thread_ctrs[tid].b,5)<thread_ctrs[mytid].b);
}
void RTS_sign(int tid){
    __atomic_add_fetch(&thread_ctrs[tid].b,1,5);
}

#endif

#ifdef PTHREAD_BARRIER
void RTS_sync(int tid) 
{   /* execute barrier function */
    int err = pthread_barrier_wait(&barrier);
    if (err != 0 && err != PTHREAD_BARRIER_SERIAL_THREAD) {
		RTS_error((char*)"barrier synchronization failed");
    }
}
#endif


#ifdef NO_BARRIER
void RTS_sync(int tid){}
void RTS_wait(int mytid, int tid){}
void RTS_sign(int tid){}
#endif

/*
 * functions for time measurement
 */
#if IS_x86
/* x86 cycle counter */
/* Set *hi and *low to the high and low order bits of the cycle counter.
   Implementation requires assembly code to use the rdtsc instruction. */

void access_counter (unsigned *hi, unsigned *lo)
{
    /* legacy code
    asm("rdtsc; movl %%edx,%0; movl %%eax,%1"   // Read cycle counter
            : "=r" (*hi), "=r" (*lo)            // and move results to
            :   // no input // the two outputs
            : "%edx", "%eax");
    */
    __asm__ __volatile__ (      // serialize
        "xorl %%eax,%%eax \n        cpuid"
        ::: "%rax", "%rbx", "%rcx", "%rdx");
        // We cannot use "=A", since this would use %rax on x86_64 and 
        // return only the lower 32bits of the TSC 
    __asm__ __volatile__ ("rdtsc" : "=a" (*lo), "=d" (*hi));
}

void RTS_start_timer(int tid, int aid){
    access_counter(&(thr_vars[tid]._tmp_time[aid].start_cyc_hi), 
					&(thr_vars[tid]._tmp_time[aid].start_cyc_lo));
}

void RTS_stop_timer(int tid, int aid){
    access_counter(&(thr_vars[tid]._tmp_time[aid].stop_cyc_hi), 
					&(thr_vars[tid]._tmp_time[aid].stop_cyc_lo));
}


double mhz(int tid, int sleeptime){
    double rate;
    rts_time_stamp mhz_ts;
    RTS_start_timer(tid,0);
    sleep(sleeptime);
    RTS_stop_timer(tid,0);
	RTS_record((char*)"Mhz(dummy)", tid, 0, 0);
    rate = thr_vars[tid]._time[0][0] / (sleeptime);
    return rate;
}

void RTS_init(int _numcpus, int _numiters){
	numcpus = _numcpus;
	numiters= _numiters;
    
	// malloc thr_vars
	if (posix_memalign((void**)&thr_vars, CACHELINE_SIZE, sizeof(struct RTS_VARS)*numcpus)){
		RTS_error((char*)"failed to allocate memory to thr_vars\n");
	}
}

void RTS_thread_init(int tid, int _numtasks){
	int i;

	thr_vars[tid].pid=tid;
	printf("Thread %6d: RTS initialized.\n", tid);
	
	// init RTS_VARS
	thr_vars[tid].numtasks = _numtasks;
	

	// task level memory allocation
	if(posix_memalign((void**)&(thr_vars[tid].ids), CACHELINE_SIZE, sizeof(char*)*_numtasks)){
		RTS_error((char*)"failed to allocate memory to thr_vars[%d].ids\n", tid);
	}	

	for(i = 0; i <_numtasks; i++){
		if(posix_memalign((void**)&(thr_vars[tid].ids[i]), CACHELINE_SIZE, sizeof(char)*(MAX_TASK_ID_LENGTH+1))){
			RTS_error((char*)"failed to allocate memory to thr_vars[%d].ids[%d]\n",tid,i);
		}
	}

    if(posix_memalign((void**)&(thr_vars[tid]._tmp_time), CACHELINE_SIZE, sizeof(rts_time_stamp)*_numtasks)){
		RTS_error((char*)"failed to allocate memory to thr_vars[%d]._tmp_time\n",tid); 
	}
    if(posix_memalign((void**)&(thr_vars[tid]._time), CACHELINE_SIZE, sizeof(double*)*_numtasks)){
		RTS_error((char*)"failed to allocate memory to thr_vars[%d]._tmp_time\n",tid); 
	}
   
	// iteration level memory allocation
	for(i = 0; i < _numtasks; i++){
		if(posix_memalign((void**)&thr_vars[tid]._time[i], CACHELINE_SIZE, sizeof(double)*numiters)){
			RTS_error((char*)"failed to allocate memory to thr_vars[%d]._time[%d]\n",tid,i);
		}
	} 

    thr_vars[tid].RTS_CLOCK_RATE = mhz(tid,2);
    printf("Measured clock frequency: %.0f Hz\n", thr_vars[tid].RTS_CLOCK_RATE);

    // Open file to output
    char filename_id[2*sizeof(int)];
    snprintf(filename_id, sizeof(filename_id),"%d",tid);
    char* filename_w_id;
    filename_w_id=(char*)malloc(strlen(OUTFILEID)+strlen(OUTFILEEXT)+strlen(filename_id)+1);
    strcpy(filename_w_id, OUTFILEID);
    strcat(filename_w_id, filename_id);
    strcat(filename_w_id, OUTFILEEXT);

    thr_vars[tid].f=fopen(filename_w_id, "w");
    if (thr_vars[tid].f == NULL){
        RTS_error((char*)"failed to open file %s.\n", filename_w_id);
    }
}
#endif  /* x86 cycle counter */

/*
 * Write recorded execution time
 */
void RTS_record(char* id, int tid, int aid, int iter){
    int i;
    unsigned hi, lo, borrow;
    double result;

	// copt the task name
	for (i = 0 ; i < MAX_TASK_ID_LENGTH; i++){
		thr_vars[tid].ids[aid][i]=id[i];
		if(id[i]=='\0'){
			break;
		}
	}
	thr_vars[tid].ids[aid][MAX_TASK_ID_LENGTH]='\0';

    // Do double precision subtraction
    lo = thr_vars[tid]._tmp_time[aid].stop_cyc_lo - thr_vars[tid]._tmp_time[aid].start_cyc_lo;
    borrow = lo > thr_vars[tid]._tmp_time[aid].stop_cyc_lo;
    hi = thr_vars[tid]._tmp_time[aid].stop_cyc_hi - thr_vars[tid]._tmp_time[aid].start_cyc_hi - borrow;
    result = (double) hi * (1<<30) * 4 + lo;
    if (result < 0){
        RTS_error((char*)"counter returns neg value: %.0f\n", result);
    }
    thr_vars[tid]._time[aid][iter] = result;
}

void RTS_write_time(int tid){
	int iter,aid;
    for(aid = 0; aid< thr_vars[tid].numtasks; aid++){
		for(iter = 0; iter<numiters; iter++){
			fprintf(thr_vars[tid].f, "EXECUTION TIME\n");
			fprintf(thr_vars[tid].f, "--------------------------------------------------------------------\n");
			fprintf(thr_vars[tid].f, "In_milli_second\t%4d%4d%18.4f\n", 
					tid, iter, (thr_vars[tid]._time[aid][iter]/thr_vars[tid].RTS_CLOCK_RATE)*1000);
			fprintf(thr_vars[tid].f,"\n");
		}
	}

    fclose(thr_vars[tid].f);
}
/*
 * Executes a work function 
 */
static void *rts_threadfunction(void *arg_ptr)
{
    rts_args *args = (rts_args *) arg_ptr;

    /* check range of current process id */
    assert(args -> current >= 0 && args -> current < args-> num_processes && "wrong proccess id");

    /* invoke work function of process with the number of iterations */ 
    args->processes[args->current]();

    return NULL;
}

/* 
 * Initialize pthreads and invoke work functions of processes 
 */ 
void RTS_run(int num_processes, t_rts_processes processes)
{
    int i;
    pthread_t *threads;
    rts_args *args;

    assert(num_processes > 0 && "number of threads is not greater than 0");


    /* allocate memory for threads and thread arguments */
    if((threads=(pthread_t*)malloc(sizeof(pthread_t) * num_processes)) == NULL){
       RTS_error((char*)"failed to allocate memory for threads");
    }
    if((args=(rts_args*)malloc(sizeof(rts_args) * num_processes)) == NULL){
       RTS_error((char*)"failed to allocate memory for thread args");
    }
    
#ifdef ATOMIC_BARRIER
    num_threads=num_processes;
    if(posix_memalign((void**)&thread_ctrs, CACHELINE_SIZE, sizeof(struct BVAR)*num_processes)){
        RTS_error((char*)"BVAR posix_memalign failed"); 
    }

    for (i = 0; i < num_processes; i++){
        thread_ctrs[i].b=0;
    }
#endif
    
#ifdef PTHREAD_BARRIER
    printf("pthread barrier enabled...\n");
    /* create barrier for synchronization */
    if(pthread_barrier_init(&barrier, NULL, num_processes)) {
       RTS_error((char*)"barrier init failed");
    }
#endif

    /* create arguments and pass on arguments */
    for(i = 0; i < num_processes; i++) {
        args[i].num_processes = num_processes; 
        args[i].current = i; 
        args[i].processes = processes;
        if(pthread_create(&threads[i], NULL, rts_threadfunction, &args[i])) {
           RTS_error((char*)"failed to create process %d",i);
        }
    }

    
    for(i = 0; i < num_processes; ++i) {
        if(pthread_join(threads[i], NULL)) {
           RTS_error((char*)"failed to join process %d",i);
        }
    }
}

