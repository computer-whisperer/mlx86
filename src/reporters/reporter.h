#ifndef REPORTER_H
#define REPORTER_H
#include <semaphore.h>
#include "problems/problems.h"
#include "types.h"
#include "main.h"

struct REPORTER_MEM_T
{
	sem_t data_sem;

	U8 abort_reporter;
	pid_t reporter_pid;

	U8 abort_solve;
	U64 trials_completed;
	U8 current_data[MAX_DATA_LEN];

	U8 enable_best_data;
	U8 best_data[MAX_DATA_LEN];

	U8 enable_cycles;
	U64 cycle_num;
	U64 trials_in_cycle;
	U64 trials_completed_in_cycle;
};

struct REPORTER_MEM_T * reporter_mem_init(void);
void init_reporter_process(struct REPORTER_MEM_T * reporter_mem, struct Problem_T * problem);
void deinit_reporter_process(struct REPORTER_MEM_T * reporter_mem);

#endif
