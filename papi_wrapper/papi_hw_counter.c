#include "papi_hw_counter.h"


//-------------------------------------------------------------------------------
// Helper functions 
//-------------------------------------------------------------------------------

/*
 * Record error message when papi operation fails.
 */
void papi_fail(char *file, int ln, char * msg, int retval) {
   fprintf(stderr, "%s: %d: %s %d\n", file, ln, msg, retval);
   exit(EXIT_FAILURE);
}

void papi_write_header(int tid, char* header){
  fprintf(thr_vars[tid].f, "%s\n",header);
  fprintf(thr_vars[tid].f, "--------------------------------------------------------------------\n");
}

void papi_write_time(char* id, int tid, int iter, long long ts){
  fprintf(thr_vars[tid].f, "%40s%4d%4d%18lld\n", id, tid, iter, ts);
}

void papi_write_summary_header(char* header){
  fprintf(summary_f, "%s\n",header);
  fprintf(summary_f, "--------------------------------------------------------------------\n");
}

void papi_write_summary_time(char* id, int tid, int iter, long long ts){
  fprintf(summary_f, "%40s%4d%4d%18lld\n", id, tid, iter, ts);
}

int get_median_idx(long long* vals, int size){
  int i,j;
  int tmp;
  int* idxs;
  int result;

  if (posix_memalign((void**)&idxs, CACHELINE_SIZE, sizeof(int)*size)){
    printf("ERROR: failed to allocate memory to tmp idxs\n"); exit(1);
  }
  for(i=0; i<numiters; i++){
    idxs[i]=i;
  }
  for(i=size-1; i>0; i--){
    for(j=i-1; j>=0; j--){
      if(vals[idxs[j]]>vals[idxs[j+1]]){
        tmp=idxs[j];
        idxs[j]=idxs[j+1];
        idxs[j+1]=tmp;
      }
    }
  }
  result = idxs[size/2];
  free(idxs);
  return result;
}

#ifdef __ARM_ARCH_7A__  // or __arm__
int pin_cpu(pid_t pid, unsigned int cpu) {
   size_t size;
   cpu_set_t * setPtr = CPU_ALLOC(1);
   assert (NULL != setPtr && "cpu_set allocation failed!");

   size = CPU_ALLOC_SIZE(1);
   CPU_ZERO_S(size, setPtr);     // clear set
   CPU_SET_S(cpu, size, setPtr); // enable requested cpu in set
   assert(1 == CPU_COUNT_S(size, setPtr));
   assert (CPU_ISSET_S(cpu, size, setPtr));

   int ret = sched_setaffinity(pid, size, setPtr);
   assert (ret == 0 && "sched_setaffinity failed");
   assert (cpu == sched_getcpu() && "Pinning failed");

   CPU_FREE(setPtr);
   return ret;
}
#endif


//-------------------------------------------------------------------------------
// wrapper functions 
//-------------------------------------------------------------------------------

#ifdef NO_MEASURE
unsigned long int PAPI_HW_COUNTER_thread_pid(void){ return -1; }
void PAPI_HW_COUNTER_init(int _numcpus, int _numiters){}
void PAPI_HW_COUNTER_thread_init(int tid, int _numtasks){}
void PAPI_HW_COUNTER_open(int tid){}
void PAPI_HW_COUNTER_on(int tid, int aid){}
void PAPI_HW_COUNTER_off(int tid, int aid){}
void PAPI_HW_COUNTER_record(char* id, int tid, int aid, int iter){}
void PAPI_HW_COUNTER_close(int tid){}
void PAPI_HW_COUNTER_write(int tid){}
void PAPI_HW_COUNTER_write_summary(void){}
#else
unsigned long int PAPI_HW_COUNTER_thread_pid(void){
  unsigned long int pid;
  if ((pid=PAPI_thread_id()) == (unsigned long int) -1){
    printf("ERROR: PAPI failed to obtain thread id.\n");
    exit(1);
  }
  return pid;
}

void PAPI_HW_COUNTER_init(int _numcpus, int _numiters){
  int retval;
  int i,j;

  numcpus = _numcpus;
  numiters= _numiters;

  retval = PAPI_library_init(PAPI_VER_CURRENT);
  if (retval <0){
    printf("ERROR: PAPI failed to initialize.(%d)\n",retval);
    exit(1);
  }else if (retval != PAPI_VER_CURRENT) {
    printf("ERROR: PAPI library version mismatch.(%d!=%d)\n", PAPI_VER_CURRENT,retval);
    exit(1);
  }
  printf("PAPI is initialized.\n");

  // malloc thr_vars
  if (posix_memalign((void**)&thr_vars, CACHELINE_SIZE, sizeof(struct PAPI_VARS)*numcpus)){
    printf("ERROR: failed to allocate memory to thr_vars\n");
    exit(1);
  }

  // Open file to overall result
  summary_f=fopen("summary.txt", "w");
  if (summary_f == NULL){
    printf("ERROR: failed to open file summary.txt.\n");
    exit(1);
  }
}

void PAPI_HW_COUNTER_thread_init(int tid, int _numtasks){
  int retval;
  int i,j;

  retval = PAPI_thread_init(pthread_self);
  if(retval != PAPI_OK){
    //handle_error(retval);
    printf("ERROR: PAPI failed to initialized threads.(%d)\n", retval);
    exit(1);
  }

  unsigned long int pid = PAPI_HW_COUNTER_thread_pid();
  printf("Thread %6d: PAPI initialized.\n", tid);

  // init PAPI_VARS
  thr_vars[tid].pid = pid;
  thr_vars[tid].EventSet = PAPI_NULL;
  thr_vars[tid].num_events = 0;
 
#ifdef MEASURE_ENERGY
  thr_vars[tid].num_energy_events = 0;
  thr_vars[tid].EnergyEventSet = PAPI_NULL;
  thr_vars[tid].rapl_cid = -1;
  thr_vars[tid].cmpinfo = NULL;
#endif

  // task level memory allocation
  thr_vars[tid].numtasks = _numtasks;

  if(posix_memalign((void**)&(thr_vars[tid].ids), CACHELINE_SIZE, sizeof(char*)*_numtasks)){
    printf("ERROR: failed to allocate memory to thr_vars[%d].ids\n",tid); exit(1);
  }

  for(i=0; i<_numtasks; i++){
    if(posix_memalign((void**)&(thr_vars[tid].ids[i]), CACHELINE_SIZE, sizeof(char)*(MAX_TASK_ID_LENGTH+1))){
      printf("ERROR: failed to allocate memory to thr_vars[%d].ids[%d]\n",tid,i); exit(1);
    }
  }
   
  #ifdef MEASURE_TIME
  if(posix_memalign((void**)&(thr_vars[tid]._tmp_time), CACHELINE_SIZE, sizeof(struct TS)*_numtasks)){
    printf("ERROR: failed to allocate memory to thr_vars[%d]._tmp_time\n",tid); exit(1);
  }
  if(posix_memalign((void**)&(thr_vars[tid]._time), CACHELINE_SIZE, sizeof(long long*)*_numtasks)){
    printf("ERROR: failed to allocate memory to thr_vars[%d]._time\n",tid); exit(1);
  }
  #endif

  #ifdef MEASURE_CPI
  if(posix_memalign((void**)&(thr_vars[tid]._tmp_inst), CACHELINE_SIZE, sizeof(struct TS)*_numtasks)){
    printf("ERROR: failed to allocate memory to thr_vars[%d]._tmp_inst\n",tid); exit(1);
  }
  if(posix_memalign((void**)&(thr_vars[tid]._tmp_cyc), CACHELINE_SIZE, sizeof(struct TS)*_numtasks)){
    printf("ERROR: failed to allocate memory to thr_vars[%d]._tmp_cyc\n",tid); exit(1);
  }
  if(posix_memalign((void**)&(thr_vars[tid]._inst), CACHELINE_SIZE, sizeof(long long*)*_numtasks)){
    printf("ERROR: failed to allocate memory to thr_vars[%d]._inst\n",tid); exit(1);
  }
  if(posix_memalign((void**)&(thr_vars[tid]._cyc), CACHELINE_SIZE, sizeof(long long*)*_numtasks)){
    printf("ERROR: failed to allocate memory to thr_vars[%d]_cyc\n",tid); exit(1);
  }
  if(posix_memalign((void**)&(thr_vars[tid]._cpi), CACHELINE_SIZE, sizeof(long long*)*_numtasks)){
    printf("ERROR: failed to allocate memory to thr_vars[%d]_cpi\n",tid); exit(1);
  }
  #endif

  #ifdef MEASURE_MEMACC
  if(posix_memalign((void**)&(thr_vars[tid]._tmp_load), CACHELINE_SIZE, sizeof(struct TS)*_numtasks)){
    printf("ERROR: failed to allocate memory to thr_vars[%d]._tmp_load\n",tid); exit(1);
  }
  if(posix_memalign((void**)&(thr_vars[tid]._tmp_store), CACHELINE_SIZE, sizeof(struct TS)*_numtasks)){
    printf("ERROR: failed to allocate memory to thr_vars[%d]._tmp_store\n",tid); exit(1);
  }
  if(posix_memalign((void**)&(thr_vars[tid]._load), CACHELINE_SIZE, sizeof(long long*)*_numtasks)){
    printf("ERROR: failed to allocate memory to thr_vars[%d]._load\n",tid); exit(1);
  }
  if(posix_memalign((void**)&(thr_vars[tid]._store), CACHELINE_SIZE, sizeof(long long*)*_numtasks)){
    printf("ERROR: failed to allocate memory to thr_vars[%d]._store\n",tid); exit(1);
  }
  #endif

  #ifdef MEASURE_LLCMISS
  if(posix_memalign((void**)&(thr_vars[tid]._tmp_llcmiss), CACHELINE_SIZE, sizeof(struct TS)*_numtasks)){
    printf("ERROR: failed to allocate memory to thr_vars[%d]._tmp_llcmiss\n",tid); exit(1);
  }
  if(posix_memalign((void**)&(thr_vars[tid]._llcmiss), CACHELINE_SIZE,sizeof(long long*)*_numtasks)){
    printf("ERROR: failed to allocate memory to thr_vars[%d]._llcmiss\n",tid); exit(1);
  }
  #endif

  #ifdef MEASURE_ICACHEMISS
  if(posix_memalign((void**)&(thr_vars[tid]._tmp_icachemiss), CACHELINE_SIZE, sizeof(struct TS)*_numtasks)){
    printf("ERROR: failed to allocate memory to thr_vars[%d]._tmp_icachemiss\n",tid); exit(1);
  }
  if(posix_memalign((void**)&(thr_vars[tid]._icachemiss),CACHELINE_SIZE,sizeof(long long*)*_numtasks)){
    printf("ERROR: failed to allocate memory to thr_vars[%d]._icachemiss\n",tid); exit(1);
  }
  #endif

  #ifdef MEASURE_DCACHEMISS
  if(posix_memalign((void**)&(thr_vars[tid]._tmp_l1dcm), CACHELINE_SIZE, sizeof(struct TS)*_numtasks)){
    printf("ERROR: failed to allocate memory to thr_vars[%d]._tmp_l1dcm\n",tid); exit(1);
  }
  if(posix_memalign((void**)&(thr_vars[tid]._tmp_l1dca), CACHELINE_SIZE, sizeof(struct TS)*_numtasks)){
    printf("ERROR: failed to allocate memory to thr_vars[%d]._tmp_l1dca\n",tid); exit(1);
  }
  if (posix_memalign((void**)&(thr_vars[tid]._l1dcm), CACHELINE_SIZE, sizeof(long long*)*_numtasks)){
    printf("ERROR: failed to allocate memory to thr_vars[%d]._l1dcm\n",tid); exit(1);
  }
  if (posix_memalign((void**)&(thr_vars[tid]._l1dca), CACHELINE_SIZE, sizeof(long long*)*_numtasks)){
    printf("ERROR: failed to allocate memory to thr_vars[%d]._l1dca\n",tid); exit(1);
  }
  #endif

  #ifdef MEASURE_ENERGY
  for( j = 0; j < MAX_RAPL_EVENTS; j++){
    if(posix_memalign((void**)&(thr_vars[tid]._tmp_energies[j]),CACHELINE_SIZE, sizeof(struct TS)*_numtasks)){
      printf("ERROR: failed to allocate memory to thr_vars[%d]._tmp_energies[%d]\n",tid,j); exit(1);
    }
    if (posix_memalign((void**)&(thr_vars[tid]._energies[j]), CACHELINE_SIZE, 
          sizeof(long long*)*_numtasks)){
      printf("ERROR: failed to allocate memory to thr_vars[%d]._energies[%d]\n",tid,j); exit(1);
    }
  }
  #endif

  // iteration level memory allocation
  for (i = 0; i < _numtasks; i++){
  #ifdef MEASURE_TIME
    if (posix_memalign((void**)&thr_vars[tid]._time[i], CACHELINE_SIZE, sizeof(long long)*numiters)){
      printf("ERROR: failed to allocate memory to thr_vars[%d]._time[]\n",tid); exit(1);
    }
  #endif

  #ifdef MEASURE_CPI
    if (posix_memalign((void**)&thr_vars[tid]._inst[i], CACHELINE_SIZE, sizeof(long long)*numiters)){
      printf("ERROR: failed to allocate memory to thr_vars[%d]._inst[]\n",tid); exit(1);
    }
    if (posix_memalign((void**)&thr_vars[tid]._cyc[i], CACHELINE_SIZE, sizeof(long long)*numiters)){
      printf("ERROR: failed to allocate memory to thr_vars[%d]_cyc[]\n",tid); exit(1);
    }
    if (posix_memalign((void**)&thr_vars[tid]._cpi[i], CACHELINE_SIZE, sizeof(long long)*numiters)){
      printf("ERROR: failed to allocate memory to thr_vars[%d]_cpi[]\n",tid); exit(1);
    }
  #endif

  #ifdef MEASURE_MEMACC
    if (posix_memalign((void**)&thr_vars[tid]._load[i], CACHELINE_SIZE, sizeof(long long)*numiters)){
      printf("ERROR: failed to allocate memory to thr_vars[%d]._load[]\n",tid); exit(1);
    }
    if (posix_memalign((void**)&thr_vars[tid]._store[i], CACHELINE_SIZE, sizeof(long long)*numiters)){
      printf("ERROR: failed to allocate memory to thr_vars[%d]._store[]\n",tid); exit(1);
    }
  #endif

  #ifdef MEASURE_LLCMISS
    if (posix_memalign((void**)&thr_vars[tid]._llcmiss[i], CACHELINE_SIZE, sizeof(long long)*numiters)){
      printf("ERROR: failed to allocate memory to thr_vars[%d]._llcmiss[]\n",tid); exit(1);
    }
  #endif

  #ifdef MEASURE_ICACHEMISS
    if (posix_memalign((void**)&thr_vars[tid]._icachemiss[i], CACHELINE_SIZE, sizeof(long long)*numiters)){
      printf("ERROR: failed to allocate memory to thr_vars[%d]._icachemiss[]\n",tid); exit(1);
    }
  #endif

  #ifdef MEASURE_DCACHEMISS
    if (posix_memalign((void**)&thr_vars[tid]._l1dcm[i], CACHELINE_SIZE, sizeof(long long)*numiters)){
      printf("ERROR: failed to allocate memory to thr_vars[%d]._l1dcm[]\n",tid); exit(1);
    }
    if (posix_memalign((void**)&thr_vars[tid]._l1dca[i], CACHELINE_SIZE, sizeof(long long)*numiters)){
      printf("ERROR: failed to allocate memory to thr_vars[%d]._l1dca[]\n",tid); exit(1);
    }
  #endif

  #ifdef MEASURE_ENERGY
    for( j = 0; j < MAX_RAPL_EVENTS; j++){
      if (posix_memalign((void**)&(thr_vars[tid]._energies[j][i]), CACHELINE_SIZE, 
            sizeof(long long)*numiters)){
        printf("ERROR: failed to allocate memory to thr_vars[%d]._energies[%d][]\n",tid,j); exit(1);
      }
    }
  #endif
  }
}

void PAPI_HW_COUNTER_open(int tid){
    // set events to measure
    int *Events;
    int EventCode;
    int event_ctr = 0;
    int retval;
  #ifdef MEASURE_TIME
  #endif

  #ifdef MEASURE_CPI
    thr_vars[tid].papi_idx_inst = thr_vars[tid].num_events++;
    thr_vars[tid].papi_idx_cyc = thr_vars[tid].num_events++;
  #endif

  #ifdef MEASURE_MEMACC
    thr_vars[tid].papi_idx_load = thr_vars[tid].num_events++;
    thr_vars[tid].papi_idx_store = thr_vars[tid].num_events++;
  #endif
    
  #ifdef MEASURE_LLCMISS
    thr_vars[tid].papi_idx_llcmiss = thr_vars[tid].num_events++;
  #endif

  #ifdef MEASURE_ICACHEMISS
    thr_vars[tid].papi_idx_icachemiss = thr_vars[tid].num_events++;
  #endif

  #ifdef MEASURE_DCACHEMISS
    thr_vars[tid].papi_idx_l1dcm = thr_vars[tid].num_events++;
    thr_vars[tid].papi_idx_l1dca = thr_vars[tid].num_events++;
  #endif 

  #ifdef MEASURE_ENERGY
  #endif
    
    event_ctr = 0;  // reset event counter

    if((Events=(int*)malloc(sizeof(int)*thr_vars[tid].num_events)) == NULL){
        printf("ERROR: Failed to allocate memory for Events.");
    }
    if((thr_vars[tid].values=(long long int*)malloc(sizeof(long long)*thr_vars[tid].num_events)) == NULL){
        printf("ERROR: Failed to allocate memory for Events.");
    }

  #ifdef __ARM_ARCH_7A__
    // pin processor only on arm arch.
    pid_t pid = getpid();
    int core = 0;
    printf("Pinning thread %d to cores %d..%d\n", pid, 0, 0);
    printf("Observe in terminal via \"ps -p <PID> -L -o pid,tid,psr\"\n");
    pin_cpu(pid, core);
    printf("Pinned to core %d\n", core);
  #endif

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
        printf("failed to open file %s.\n", filename_w_id);
        exit(1);
    }

    // Measure clock frequency
    long long elapsed_cyc;
    elapsed_cyc = PAPI_get_real_cyc();
    sleep(1);
    elapsed_cyc = PAPI_get_real_cyc()-elapsed_cyc;
    thr_vars[tid].PAPI_CLOCK_RATE = elapsed_cyc;
    printf("Measured clock frequency: %.0lld Hz\n",thr_vars[tid].PAPI_CLOCK_RATE);

    // Set EventSet
    thr_vars[tid].EventSet = PAPI_NULL;/*EventSet*/
    retval=PAPI_create_eventset(&(thr_vars[tid].EventSet));
    if (retval != PAPI_OK){
        papi_fail(__FILE__, __LINE__, "PAPI_create_eventset()", retval);
    }


  #ifdef MEASURE_TIME
  #endif

  #ifdef MEASURE_CPI
    retval = PAPI_event_name_to_code( PAPI_INST , &EventCode );
    if (retval != PAPI_OK ) {
        papi_fail(__FILE__, __LINE__,
                "PAPI_event_name_to_code, inst", retval);
    }
    Events[event_ctr++] = EventCode;
   
    retval = PAPI_event_name_to_code( PAPI_CYC , &EventCode );
    if (retval != PAPI_OK ) {
        papi_fail(__FILE__, __LINE__,
                "PAPI_event_name_to_code, cyc", retval);
    }
    Events[event_ctr++] = EventCode;
  #endif

  #ifdef MEASURE_MEMACC
    retval = PAPI_event_name_to_code( PAPI_MEM_LOAD , &EventCode );
    if (retval != PAPI_OK ) {
        papi_fail(__FILE__, __LINE__,
                "PAPI_event_name_to_code, loads", retval);
    }
    Events[event_ctr++] = EventCode;
    
    retval=PAPI_event_name_to_code( PAPI_MEM_STORE , &EventCode );
    if (retval != PAPI_OK ) {
        papi_fail(__FILE__, __LINE__,
                "PAPI_event_name_to_code, stores", retval);
    }
    Events[event_ctr++] = EventCode;
  #endif
  
  #ifdef MEASURE_LLCMISS
    retval = PAPI_event_name_to_code( PAPI_LLC_MISS , &EventCode );
    if (retval != PAPI_OK ) {
        papi_fail(__FILE__, __LINE__,
                "PAPI_event_name_to_code, llc miss", retval);
    }
    Events[event_ctr++] = EventCode;
  #endif

  #ifdef MEASURE_ICACHEMISS
    retval = PAPI_event_name_to_code( PAPI_IC_MISS , &EventCode );
    if (retval != PAPI_OK ) {
        papi_fail(__FILE__, __LINE__,
                "PAPI_event_name_to_code, llc miss", retval);
    }
    Events[event_ctr++] = EventCode;
  #endif 

  #ifdef MEASURE_DCACHEMISS
    retval = PAPI_event_name_to_code( PAPI_L1_DC_MISS , &EventCode );
    if (retval != PAPI_OK ) {
        papi_fail(__FILE__, __LINE__,
                "PAPI_event_name_to_code, Level 1 data cache misses", retval);
    }
    Events[event_ctr++] = EventCode;

    retval = PAPI_event_name_to_code( PAPI_L1_DC_ACCESS, &EventCode );
    if (retval != PAPI_OK ) {
        papi_fail(__FILE__, __LINE__,
                "PAPI_event_name_to_code, Level 1 data cache accesses", retval);
    }
    Events[event_ctr++] = EventCode;
  #endif

  #ifdef MEASURE_ENERGY
    printf("Probing all RAPL events\n");

    thr_vars[tid].numcmp = PAPI_num_components();

    for(thr_vars[tid].cid=0; thr_vars[tid].cid<thr_vars[tid].numcmp; thr_vars[tid].cid++) {
        if ( (thr_vars[tid].cmpinfo = PAPI_get_component_info(thr_vars[tid].cid)) == NULL) {
            papi_fail(__FILE__, __LINE__,"PAPI_get_component_info failed\n", 0);
        }
        if (strstr(thr_vars[tid].cmpinfo->name,"rapl")) {
            thr_vars[tid].rapl_cid=thr_vars[tid].cid;
            printf("Found rapl component at cid %d.\n",thr_vars[tid].rapl_cid);
            if (thr_vars[tid].cmpinfo->disabled) {
                printf("RAPL component disabled: %s\n",
                        thr_vars[tid].cmpinfo->disabled_reason);
                exit(EXIT_FAILURE);
            }
            break;
        }
    }

    if (thr_vars[tid].cid==thr_vars[tid].numcmp) {
        // Component not found:
        papi_fail(__FILE__,__LINE__,"No rapl component found\n",0);
    }

    retval = PAPI_create_eventset( &(thr_vars[tid].EnergyEventSet) );
    if (retval != PAPI_OK){
        papi_fail(__FILE__,__LINE__, "PAPI_create_eventset()", retval);
    }

  // Add all events:
  int r;
  thr_vars[tid].code = PAPI_NATIVE_MASK;
  r = PAPI_enum_cmp_event( &(thr_vars[tid].code), PAPI_ENUM_FIRST, thr_vars[tid].rapl_cid );
  while ( r == PAPI_OK ) {
     retval = PAPI_event_code_to_name( thr_vars[tid].code, thr_vars[tid].event_names[thr_vars[tid].num_energy_events] );
     if ( retval != PAPI_OK ) {
        printf("Error translating %#x\n",thr_vars[tid].code);
        papi_fail(__FILE__, __LINE__, 
                  "PAPI_event_code_to_name", retval );
     }

     printf("Found event: %s\n", thr_vars[tid].event_names[thr_vars[tid].num_energy_events]);

     retval = PAPI_get_event_info(thr_vars[tid].code,&(thr_vars[tid].evinfo));
     if (retval != PAPI_OK) {
        papi_fail(__FILE__, __LINE__,
                  "Error getting event info\n",retval);
     }
	
     strncpy(thr_vars[tid].units[thr_vars[tid].num_energy_events],thr_vars[tid].evinfo.units,PAPI_MIN_STR_LEN);
     thr_vars[tid].data_type[thr_vars[tid].num_energy_events] = thr_vars[tid].evinfo.data_type;

     retval = PAPI_add_event(thr_vars[tid].EnergyEventSet, thr_vars[tid].code);
     if (retval != PAPI_OK ) {
         papi_fail( __FILE__, __LINE__, "PAPI_add_event()", retval);
     }
  	      
     r = PAPI_enum_cmp_event( &(thr_vars[tid].code), PAPI_ENUM_EVENTS, thr_vars[tid].rapl_cid );
     thr_vars[tid].num_energy_events++;
  }

    if((thr_vars[tid].energy_values=(long long int*)malloc(sizeof(long long)*thr_vars[tid].num_energy_events)) == NULL){
        printf("ERROR: Failed to allocate memory for Events.");
    }

  #endif

  #ifdef MEASURE_HW_COUNTER
    int k;
    for(k = 0; k < thr_vars[tid].num_events; k++){
        retval = PAPI_add_event(thr_vars[tid].EventSet, Events[k]);
        if (retval != PAPI_OK ) {
           printf("At event %d:\n",k); 
           papi_fail( __FILE__, __LINE__, "PAPI_add_event()", retval);
        }
    }

    retval=PAPI_start(thr_vars[tid].EventSet);
    if (retval != PAPI_OK){
        papi_fail(__FILE__, __LINE__, "PAPI_start()", retval);
    }
  #endif

  #ifdef MEASURE_ENERGY
    retval=PAPI_start(thr_vars[tid].EnergyEventSet);
    if (retval != PAPI_OK){
        papi_fail(__FILE__, __LINE__, "PAPI_start() on energy", retval);
    }
  #endif
}

void PAPI_HW_COUNTER_on(int tid, int aid)
{
  int retval;
  #ifdef MEASURE_TIME
    thr_vars[tid]._tmp_time[aid].begin=PAPI_get_real_cyc();
  #endif

  #ifdef MEASURE_HW_COUNTER
    retval=PAPI_read(thr_vars[tid].EventSet, thr_vars[tid].values);
    if (retval != PAPI_OK) {
       papi_fail(__FILE__, __LINE__, "PAPI_read()", retval);
    }
  #endif
 
  #ifdef MEASURE_CPI
    thr_vars[tid]._tmp_inst[aid].begin=thr_vars[tid].values[thr_vars[tid].papi_idx_inst];
    thr_vars[tid]._tmp_cyc[aid].begin=thr_vars[tid].values[thr_vars[tid].papi_idx_cyc];
  #endif
 
  #ifdef MEASURE_MEMACC
    thr_vars[tid]._tmp_load[aid].begin=thr_vars[tid].values[thr_vars[tid].papi_idx_load];
    thr_vars[tid]._tmp_store[aid].begin=thr_vars[tid].values[thr_vars[tid].papi_idx_store];
  #endif

  #ifdef MEASURE_LLCMISS
    thr_vars[tid]._tmp_llcmiss[aid].begin=thr_vars[tid].values[thr_vars[tid].papi_idx_llcmiss];
  #endif

  #ifdef MEASURE_ICACHEMISS
    thr_vars[tid]._tmp_icachemiss[aid].begin=thr_vars[tid].values[thr_vars[tid].papi_idx_icachemiss];
  #endif

  #ifdef MEASURE_DCACHEMISS
    thr_vars[tid]._tmp_l1dcm[aid].begin=thr_vars[tid].values[thr_vars[tid].papi_idx_l1dcm];
    thr_vars[tid]._tmp_l1dca[aid].begin=thr_vars[tid].values[thr_vars[tid].papi_idx_l1dca];
  #endif

  #ifdef MEASURE_ENERGY
    retval=PAPI_read(thr_vars[tid].EnergyEventSet, thr_vars[tid].energy_values);
    if (retval != PAPI_OK) {
       papi_fail(__FILE__, __LINE__, "PAPI_read()", retval);
    }
    int i;
    for(i=0;i<thr_vars[tid].num_energy_events;i++){
      thr_vars[tid]._tmp_energies[i][aid].begin=thr_vars[tid].energy_values[i];
      
    }
  #endif
}


void PAPI_HW_COUNTER_off(int tid, int aid)
{
  int retval;

  #ifdef MEASURE_TIME
    thr_vars[tid]._tmp_time[aid].end=PAPI_get_real_cyc();
  #endif

  #ifdef MEASURE_HW_COUNTER
    retval=PAPI_read(thr_vars[tid].EventSet, thr_vars[tid].values);
    if (retval != PAPI_OK) {
       papi_fail(__FILE__, __LINE__, "PAPI_read()", retval);
    }
  #endif
 
  #ifdef MEASURE_CPI
    thr_vars[tid]._tmp_inst[aid].end=thr_vars[tid].values[thr_vars[tid].papi_idx_inst];
    thr_vars[tid]._tmp_cyc[aid].end=thr_vars[tid].values[thr_vars[tid].papi_idx_cyc];
  #endif
 
  #ifdef MEASURE_MEMACC
    thr_vars[tid]._tmp_load[aid].end=thr_vars[tid].values[thr_vars[tid].papi_idx_load];
    thr_vars[tid]._tmp_store[aid].end=thr_vars[tid].values[thr_vars[tid].papi_idx_store];
  #endif

  #ifdef MEASURE_LLCMISS
    thr_vars[tid]._tmp_llcmiss[aid].end=thr_vars[tid].values[thr_vars[tid].papi_idx_llcmiss];
  #endif

  #ifdef MEASURE_ICACHEMISS
    thr_vars[tid]._tmp_icachemiss[aid].end=thr_vars[tid].values[thr_vars[tid].papi_idx_icachemiss];
  #endif

  #ifdef MEASURE_DCACHEMISS
    thr_vars[tid]._tmp_l1dcm[aid].end=thr_vars[tid].values[thr_vars[tid].papi_idx_l1dcm];
    thr_vars[tid]._tmp_l1dca[aid].end=thr_vars[tid].values[thr_vars[tid].papi_idx_l1dca];
  #endif

  #ifdef MEASURE_ENERGY
    retval=PAPI_read(thr_vars[tid].EnergyEventSet, thr_vars[tid].energy_values);
    if (retval != PAPI_OK) {
       papi_fail(__FILE__, __LINE__, "PAPI_read()", retval);
    }
    int i;
    for(i=0;i<thr_vars[tid].num_energy_events;i++){
      thr_vars[tid]._tmp_energies[i][aid].end=thr_vars[tid].energy_values[i];
    }
  #endif
}

void PAPI_HW_COUNTER_record(char* _id, int tid, int aid, int iter)
{
  int i,j;

  // copy the task name 
  for (i = 0; i < MAX_TASK_ID_LENGTH; i++){
    thr_vars[tid].ids[aid][i]=_id[i];
    if(_id[i]=='\0'){
      break;
    }
  }
  thr_vars[tid].ids[aid][MAX_TASK_ID_LENGTH] = '\0'; 

  #ifdef MEASURE_TIME
    thr_vars[tid]._time[aid][iter]=(long long)(((float)(thr_vars[tid]._tmp_time[aid].end
         -thr_vars[tid]._tmp_time[aid].begin))/(thr_vars[tid].PAPI_CLOCK_RATE)*1000);
  #endif

  #ifdef MEASURE_CPI
    thr_vars[tid]._inst[aid][iter]=thr_vars[tid]._tmp_inst[aid].end-thr_vars[tid]._tmp_inst[aid].begin;
    thr_vars[tid]._cyc[aid][iter]=thr_vars[tid]._tmp_cyc[aid].end-thr_vars[tid]._tmp_cyc[aid].begin;
    thr_vars[tid]._cpi[aid][iter]=(long long)(((float)(thr_vars[tid]._cyc[aid][iter]))
        /(thr_vars[tid]._inst[aid][iter])*1000);
  #endif
  
  #ifdef MEASURE_MEMACC
    thr_vars[tid]._load[aid][iter]=thr_vars[tid]._tmp_load[aid].end-thr_vars[tid]._tmp_load[aid].begin;
    thr_vars[tid]._store[aid][iter]=thr_vars[tid]._tmp_store[aid].end-thr_vars[tid]._tmp_store[aid].begin;
  #endif

  #ifdef MEASURE_LLCMISS
    thr_vars[tid]._llcmiss[aid][iter]=thr_vars[tid]._tmp_llcmiss[aid].end-thr_vars[tid]._tmp_llcmiss[aid].begin;
  #endif

  #ifdef MEASURE_ICACHEMISS
    thr_vars[tid]._icachemiss[aid][iter]=
		thr_vars[tid]._tmp_icachemiss[aid].end-thr_vars[tid]._tmp_icachemiss[aid].begin;
  #endif

  #ifdef MEASURE_ENERGY
    for(i=0;i<thr_vars[tid].num_energy_events;i++){
      thr_vars[tid]._energies[i][aid][iter]=
         thr_vars[tid]._tmp_energies[i][aid].end-thr_vars[tid]._tmp_energies[i][aid].begin;
    }
  #endif
}

void PAPI_HW_COUNTER_close(int tid){
    int retval;
  #ifdef MEASURE_TIME
  #endif

  #ifdef MEASURE_HW_COUNTER
    retval = PAPI_stop( thr_vars[tid].EventSet, thr_vars[tid].values);
    if (retval != PAPI_OK) {
       papi_fail(__FILE__, __LINE__, "PAPI_stop()", retval);
    }

    retval = PAPI_cleanup_eventset( thr_vars[tid].EventSet );
    if (retval != PAPI_OK) {
	   papi_fail(__FILE__, __LINE__, 
                             "PAPI_cleanup_eventset()",retval);
    }

    retval = PAPI_destroy_eventset( &(thr_vars[tid].EventSet) );
    if (retval != PAPI_OK) {
	   papi_fail(__FILE__, __LINE__, 
                              "PAPI_destroy_eventset()",retval);
    }
  #endif

  #ifdef MEASURE_ENERGY
    retval = PAPI_stop( thr_vars[tid].EnergyEventSet, thr_vars[tid].energy_values);
    if (retval != PAPI_OK) {
       papi_fail(__FILE__, __LINE__, "PAPI_stop()", retval);
    }

    retval = PAPI_cleanup_eventset( thr_vars[tid].EnergyEventSet );
    if (retval != PAPI_OK) {
	   papi_fail(__FILE__, __LINE__, 
                             "PAPI_cleanup_eventset()",retval);
    }

    retval = PAPI_destroy_eventset( &(thr_vars[tid].EnergyEventSet) );
    if (retval != PAPI_OK) {
	   papi_fail(__FILE__, __LINE__, 
                              "PAPI_destroy_eventset()",retval);
    }
  #endif

}


void PAPI_HW_COUNTER_write(int tid)
{
  int iter,aid;
  for(aid = 0; aid < thr_vars[tid].numtasks; aid++){
    for(iter = 0; iter<numiters; iter++){
    #ifdef MEASURE_TIME
      papi_write_header(tid,"EXECUTION TIME");
      papi_write_time("In_milli_second",tid,iter,thr_vars[tid]._time[aid][iter]);
      fprintf(thr_vars[tid].f, "\n");
    #endif
  
    #ifdef MEASURE_CPI
      papi_write_header(tid,"CPI");
      papi_write_time("NUMBER_OF_INSTRUCTIONS",tid,iter,thr_vars[tid]._inst[aid][iter]);
      papi_write_time("NUMBER_OF_CYCLES",tid,iter,thr_vars[tid]._cyc[aid][iter]);
      papi_write_time("CPI",tid,iter,thr_vars[tid]._cpi[aid][iter]);
      fprintf(thr_vars[tid].f,"\n");
    #endif
  
    #ifdef MEASURE_MEMACC
      papi_write_header(tid,"MEMORY ACCESS");
      papi_write_time("LOADS",tid,iter,thr_vars[tid]._load[aid][iter]);
      papi_write_time("STORES",tid,iter,thr_vars[tid]._store[aid][iter]);
      fprintf(thr_vars[tid].f, "\n");
    #endif
  
    #ifdef MEASURE_LLCMISS
      papi_write_header(tid, "LAST LEVEL CACHE");
      papi_write_time("MISSES",tid,iter,thr_vars[tid]._llcmiss[aid][iter]);
      fprintf(thr_vars[tid].f, "\n");
    #endif
  
    #ifdef MEASURE_ICACHEMISS
      papi_write_header(tid, "L1 INSTRUCTION CACHE");
      papi_write_time("MISSES",tid,iter,thr_vars[tid]._icachemiss[aid][iter]);
      fprintf(thr_vars[tid].f, "\n");
    #endif
  
    #ifdef MEASURE_DCACHEMISS
      papi_write_header(tid, "L1 DATA CACHE");
      papi_write_time("MISSES",tid,iter,thr_vars[tid]._l1dcm[aid][iter]);
      papi_write_time("ACCESSES",tid,iter,thr_vars[tid]._l1dca[aid][iter]);
      fprintf(thr_vars[tid].f, "\n");
    #endif
  
    #ifdef MEASURE_ENERGY
      int i;
      papi_write_header(tid, "ENERGY MEASUREMENTS");
      fprintf(thr_vars[tid].f, "Scaled energy measurements:\n");
  
      for(i=0;i<thr_vars[tid].num_energy_events;i++) {
        if (strstr(thr_vars[tid].units[i],"nJ")) {
          fprintf(thr_vars[tid].f, "%-40s%12.6f J\n", 
              thr_vars[tid].event_names[i],
              (double)(thr_vars[tid]._energies[i][aid][iter])/1.0e9);
        }
      }
      fprintf(thr_vars[tid].f,"\n");
  
      fprintf(thr_vars[tid].f,"Energy measurement counts:\n");
  
      for(i=0;i<thr_vars[tid].num_energy_events;i++) {
        if (strstr(thr_vars[tid].event_names[i],"ENERGY_CNT")) {
          fprintf(thr_vars[tid].f,"%-40s%12lld\t0x%08llx\n", 
              thr_vars[tid].event_names[i], 
              thr_vars[tid]._energies[i][aid][iter], thr_vars[tid]._energies[i][aid][iter]);
        }
      }
      fprintf(thr_vars[tid].f,"\n");
  
      fprintf(thr_vars[tid].f,"Scaled Fixed energy_values:\n");
      for(i=0;i<thr_vars[tid].num_energy_events;i++) {
        if (!strstr(thr_vars[tid].event_names[i],"ENERGY")) {
          if (thr_vars[tid].data_type[i] == PAPI_DATATYPE_FP64) {
            union {
                long long ll;
                double fp;
            } result;
            result.ll=thr_vars[tid]._energies[i][aid][iter];
            fprintf(thr_vars[tid].f,"%-40s%12.3f %s\n", 
                thr_vars[tid].event_names[i], 
                result.fp, 
                thr_vars[tid].units[i]);
          }
        }
      }
      fprintf(thr_vars[tid].f,"\n");
  
      fprintf(thr_vars[tid].f,"Fixed value counts:\n");
      for(i=0;i<thr_vars[tid].num_energy_events;i++) {
        if (!strstr(thr_vars[tid].event_names[i],"ENERGY")) {
          if (thr_vars[tid].data_type[i] == PAPI_DATATYPE_UINT64) {
            fprintf(thr_vars[tid].f,"%-40s%12lld\t0x%08llx\n", 
                thr_vars[tid].event_names[i], 
                thr_vars[tid]._energies[i][aid][iter], thr_vars[tid]._energies[i][aid][iter]);
          }
        }
      }
  #endif
    }
  }
  fclose(thr_vars[tid].f);
}


void PAPI_HW_COUNTER_write_summary(void){
  int tid = 0;
  int aid = 0;
  int median_idx = 0;

#ifdef MEASURE_TIME
  // pick the thread with the max execution times
  int max_tid_idx=0;
  long long max_tid_sum=0.0;
  papi_write_summary_header("EXECUTION TIMES");
  for(tid = 0; tid < numcpus; tid++){
    long long tid_sum=0.0;
    for(aid = 0; aid < thr_vars[tid].numtasks; aid++){
      // get the result of median
      median_idx=get_median_idx(thr_vars[tid]._time[aid], numiters);
      tid_sum=tid_sum+thr_vars[tid]._time[aid][median_idx];
    }
    papi_write_summary_time("THRAD", tid, 0, tid_sum);
    if (tid_sum > max_tid_sum){
      max_tid_idx=tid;
      max_tid_sum=tid_sum;
    }
  } 
  papi_write_summary_time("MAX", max_tid_idx, 0, max_tid_sum);
  fprintf(summary_f, "\n");

  // write the information of the max thread to a file
  papi_write_summary_header("DETAIL OF THE MAX");

  // case 1: write only exection times of each actors 
  if (thr_vars[max_tid_idx].numtasks > 1){ 
    papi_write_summary_header("EXECUTION TIMES");
    for (aid = 0; aid < thr_vars[max_tid_idx].numtasks; aid++){
      median_idx = get_median_idx(thr_vars[max_tid_idx]._time[aid], numiters);
      papi_write_summary_time(thr_vars[max_tid_idx].ids[aid], max_tid_idx, median_idx, thr_vars[max_tid_idx]._time[aid][median_idx]);
    }
  // case 2: write all other available records when there is only a single actor per thread
  }else{
    tid = max_tid_idx;
    aid = 0;
    int iter = get_median_idx(thr_vars[tid]._time[0], numiters);

    papi_write_summary_header("EXECUTION TIME");
    papi_write_summary_time("In_milli_second",tid,iter,thr_vars[tid]._time[aid][iter]);

  #ifdef MEASURE_CPI
    papi_write_summary_header("CPI");
    papi_write_summary_time("NUMBER_OF_INSTRUCTIONS",tid,iter,thr_vars[tid]._inst[aid][iter]);
    papi_write_summary_time("NUMBER_OF_CYCLES",tid,iter,thr_vars[tid]._cyc[aid][iter]);
    papi_write_summary_time("CPI",tid,iter,thr_vars[tid]._cpi[aid][iter]);
    fprintf(summary_f,"\n");
  #endif

  #ifdef MEASURE_MEMACC
    papi_write_summary_header("MEMORY ACCESS");
    papi_write_summary_time("LOADS",tid,iter,thr_vars[tid]._load[aid][iter]);
    papi_write_summary_time("STORES",tid,iter,thr_vars[tid]._store[aid][iter]);
    fprintf(summary_f, "\n");
  #endif

  #ifdef MEASURE_LLCMISS
    papi_write_summary_header("LAST LEVEL CACHE");
    papi_write_summary_time("MISSES",tid,iter,thr_vars[tid]._llcmiss[aid][iter]);
    fprintf(summary_f, "\n");
  #endif

  #ifdef MEASURE_ICACHEMISS
    papi_write_summary_header("L1 INSTRUCTION CACHE");
    papi_write_summary_time("MISSES",tid,iter,thr_vars[tid]._icachemiss[aid][iter]);
    fprintf(summary_f, "\n");
  #endif

  #ifdef MEASURE_DCACHEMISS
    papi_write_summary_header("L1 DATA CACHE");
    papi_write_summary_time("MISSES",tid,iter,thr_vars[tid]._l1dcm[aid][iter]);
    papi_write_summary_time("ACCESSES",tid,iter,thr_vars[tid]._l1dca[aid][iter]);
    fprintf(summary_f, "\n");
  #endif

  #ifdef MEASURE_ENERGY
    int i;
    papi_write_summary_header("ENERGY MEASUREMENTS");
    fprintf(summary_f, "Scaled energy measurements:\n");

    for(i=0;i<thr_vars[tid].num_energy_events;i++) {
      if (strstr(thr_vars[tid].units[i],"nJ")) {
        fprintf(summary_f, "%-40s%12.6f J\n", 
            thr_vars[tid].event_names[i],
            (double)(thr_vars[tid]._energies[i][aid][iter])/1.0e9);
      }
    }
    fprintf(summary_f,"\n");

    fprintf(summary_f,"Energy measurement counts:\n");

    for(i=0;i<thr_vars[tid].num_energy_events;i++) {
      if (strstr(thr_vars[tid].event_names[i],"ENERGY_CNT")) {
        fprintf(summary_f,"%-40s%12lld\t0x%08llx\n", 
            thr_vars[tid].event_names[i], 
            thr_vars[tid]._energies[i][aid][iter], thr_vars[tid]._energies[i][aid][iter]);
      }
    }
    fprintf(summary_f,"\n");

    fprintf(summary_f,"Scaled Fixed energy_values:\n");
    for(i=0;i<thr_vars[tid].num_energy_events;i++) {
      if (!strstr(thr_vars[tid].event_names[i],"ENERGY")) {
        if (thr_vars[tid].data_type[i] == PAPI_DATATYPE_FP64) {
          union {
              long long ll;
              double fp;
          } result;
          result.ll=thr_vars[tid]._energies[i][aid][iter];
          fprintf(summary_f,"%-40s%12.3f %s\n", 
              thr_vars[tid].event_names[i], 
              result.fp, 
              thr_vars[tid].units[i]);
        }
      }
    }
    fprintf(summary_f,"\n");

    fprintf(summary_f,"Fixed value counts:\n");
    for(i=0;i<thr_vars[tid].num_energy_events;i++) {
      if (!strstr(thr_vars[tid].event_names[i],"ENERGY")) {
        if (thr_vars[tid].data_type[i] == PAPI_DATATYPE_UINT64) {
          fprintf(summary_f,"%-40s%12lld\t0x%08llx\n", 
              thr_vars[tid].event_names[i], 
              thr_vars[tid]._energies[i][aid][iter], thr_vars[tid]._energies[i][aid][iter]);
        }
      }
    }
  #endif  
  }
#endif
  fclose(summary_f);
}
#endif
