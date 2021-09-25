#ifndef REPORTER_H
#define REPORTER_H
#include <mutex>
#include <thread>
#include "problems/problem.h"
#include "types.h"
#include "main.h"

struct REPORTER_MEM_T
{
    std::mutex mtx;

	U8 abort_reporter;

	U8 abort_solve;
	U64 trials_completed;
	U8 current_data[MAX_DATA_LEN];

	U8 enable_best_data;
	U8 best_data[MAX_DATA_LEN];

	U8 enable_cycles;
	U64 cycle_num;
	U64 trials_in_cycle;
	U64 trials_completed_in_cycle;

	std::thread t;
};

struct REPORTER_MEM_T * reporter_mem_init();
void init_reporter_process(struct REPORTER_MEM_T * reporter_mem, Problem * problem);
void deinit_reporter_process(struct REPORTER_MEM_T * reporter_mem);

#endif
