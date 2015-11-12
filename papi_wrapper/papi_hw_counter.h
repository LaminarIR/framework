#ifndef __PAPI_WRAPPER_HEADER
#define __PAPI_WRAPPER_HEADER

#ifdef __ARM_ARCH_7A__
#define _GNU_SOURCE
#endif // arm

#include <papi.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>

#define OUTFILEID "times"
#define OUTFILEEXT ".out"
#define CACHELINE_SIZE 64
#define MAX_TASK_ID_LENGTH 32

#ifdef __AMD_ABU_DHABI__
 #define MAX_HW_COUNTER 6
 #define PAPI_INST "PAPI_TOT_INS"
 #define PAPI_CYC "PAPI_TOT_CYC"
 #define PAPI_MEM_LOAD "NODE-LOADS"
 #define PAPI_MEM_STORE "NODE-STORES"
 #define PAPI_LLC_MISS "LLC-LOAD-MISSES"
 #define PAPI_IC_MISS  "PAPI_L1_ICM"
 #define PAPI_L1_DC_MISS "PAPI_L1_DCM"
 #define PAPI_L1_DC_ACCESS "PAPI_L1_DCA"
#endif // elc2

#ifdef __INTEL_HASWELL__
 #define MAX_HW_COUNTER 11
 #define PAPI_INST "PAPI_TOT_INS"
 #define PAPI_CYC "PAPI_TOT_CYC"
 #define PAPI_MEM_LOAD "PAPI_LD_INS"
 #define PAPI_MEM_STORE "PAPI_SR_INS"
 #define PAPI_LLC_MISS "LLC_MISSES"
 #define PAPI_IC_MISS "PAPI_L1_ICM"
 #define PAPI_L1_DC_MISS "PAPI_L1_DCM"
 #define PAPI_L1_DC_ACCESS "PAPI_L1_DCA"
#endif // desktop

#ifdef __INTEL_SANDYBRIDGE__
 #define MAX_HW_COUNTER 
 #define PAPI_INST "PAPI_TOT_INS"
 #define PAPI_CYC "PAPI_TOT_CYC"
 #define PAPI_MEM_LOAD "MEM_UOP_RETIRED:ALL_LOADS"
 #define PAPI_MEM_STORE "MEM_UOP_RETIRED:ALL_STORES"
 #define PAPI_LLC_MISS "MEM_LOAD_MISC_RETIRED:LLC_MISS"
 #define PAPI_IC_MISS "ICACHE:MISSES"
 #define PAPI_L1_DC_MISS "PAPI_L1_DCM"
 #define PAPI_L1_DC_ACCESS "PAPI_L1_DCA"
#endif // silentmaxx

#ifdef __INTEL_IVYBRIDGE__
 #define MAX_HW_COUNTER 11
 #define PAPI_INST "PAPI_TOT_INS"
 #define PAPI_CYC "PAPI_TOT_CYC"
 #define PAPI_MEM_LOAD "PAPI_LD_INS"
 #define PAPI_MEM_STORE "PAPI_SR_INS"
 #define PAPI_LLC_MISS 
 #define PAPI_IC_MISS "PAPI_L1_ICM"
 #define PAPI_L1_DC_MISS "PAPI_L1_DCM"
 #define PAPI_L1_DC_ACCESS 
#endif // gpgpu3

#ifdef __ARMv7__
 #define MAX_HW_COUNTER 6
 #define PAPI_INST "PAPI_TOT_INS"
 #define PAPI_CYC "PAPI_TOT_CYC"
 #define PAPI_MEM_LOAD "PAPI_LD_INS"
 #define PAPI_MEM_STORE "PAPI_SR_INS"
 #define PAPI_LLC_MISS "LLC-LOAD-MISSES"
 #define PAPI_IC_MISS "PAPI_L1_ICM"
 #define PAPI_L1_DC_MISS "PAPI_L1_DCM"
 #define PAPI_L1_DC_ACCESS "PAPI_L1_DCA"
#endif // arm

#if defined (MEASURE_CPI) || (MEASURE_MEMACC) || (MEASURE_LLCMISS) || (MEASURE_ICACHEMISS) || (MEASURE_DCACHEMISS)
  #define MEASURE_HW_COUNTER
#endif



//----------------------//
// Variable definitions //
//----------------------//

struct TS{
    long long begin;
    long long end;
};

struct PAPI_VARS{
  unsigned long int pid;
  FILE *f;					// file handler for record
  int EventSet;				// set of events
  int num_events;			// number of events
  long long PAPI_CLOCK_RATE;// measured clock rate
  long long* values;		// container to get counter values

  int numtasks;	// number of tasks assigned to the thread
  char** ids;	// ids of tasks 

#ifdef MEASURE_TIME
  struct TS* _tmp_time;
  long long** _time;
#endif

#ifdef MEASURE_CPI
  struct TS* _tmp_inst;
  struct TS* _tmp_cyc;
  long long** _inst;
  long long** _cyc;
  long long** _cpi;
  int papi_idx_inst;
  int papi_idx_cyc;
#endif

#ifdef MEASURE_MEMACC
  struct TS* _tmp_load;
  struct TS* _tmp_store;
  long long** _load;
  long long** _store;
  int papi_idx_load;
  int papi_idx_store;
#endif

#ifdef MEASURE_LLCMISS
  struct TS* _tmp_llcmiss;
  long long** _llcmiss;
  int papi_idx_llcmiss;
#endif

#ifdef MEASURE_ICACHEMISS
  struct TS* _tmp_icachemiss;
  long long** _icachemiss;
  int papi_idx_icachemiss;
#endif

#ifdef MEASURE_DCACHEMISS
  struct TS* _tmp_l1dcm;
  struct TS* _tmp_l1dca;
  long long** _l1dcm;
  long long** _l1dca;
  int papi_idx_l1dcm; 
  int papi_idx_l1dca;
#endif

#ifdef MEASURE_ENERGY
#define MAX_RAPL_EVENTS 64
  int EnergyEventSet;		// set of energy events
  int num_energy_events;	// number of enery events
  long long* energy_values;	// container to get counter values

  int cid,rapl_cid,numcmp;
  int code;
  int EnergyEvents[MAX_RAPL_EVENTS];
  char event_names[MAX_RAPL_EVENTS][PAPI_MAX_STR_LEN];
  char units[MAX_RAPL_EVENTS][PAPI_MIN_STR_LEN];
  int data_type[MAX_RAPL_EVENTS];
  int r;
  const PAPI_component_info_t *cmpinfo;
  PAPI_event_info_t evinfo;
 
  struct TS* _tmp_energies[MAX_RAPL_EVENTS];
  long long** _energies[MAX_RAPL_EVENTS]; 
#endif
}__attribute__((aligned(CACHELINE_SIZE)));

extern struct PAPI_VARS* thr_vars __attribute__((aligned(CACHELINE_SIZE)));

extern FILE *summary_f;		// a file to write overall result

extern int numcpus __attribute__((aligned(CACHELINE_SIZE)));
extern int numiters __attribute__((aligned(CACHELINE_SIZE)));

unsigned long int PAPI_HW_COUNTER_thread_pid(void);
void PAPI_HW_COUNTER_init(int _numcpus, int _numiters);
void PAPI_HW_COUNTER_thread_init(int tid, int _numtasks);
void PAPI_HW_COUNTER_open(int tid);
void PAPI_HW_COUNTER_on(int tid, int aid);
void PAPI_HW_COUNTER_off(int tid, int aid);
void PAPI_HW_COUNTER_record(char* id, int tid, int aid, int iter);
void PAPI_HW_COUNTER_close(int tid);
void PAPI_HW_COUNTER_write(int tid);
void PAPI_HW_COUNTER_write_summary(void);
#endif
