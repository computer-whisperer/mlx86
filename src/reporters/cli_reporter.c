#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <locale.h>
#include "reporters/reporter.h"
#include "problems/problems.h"
#include "utils.h"
#include "testing.h"

#define RETRY_COUNT 5

struct REPORTER_MEM_T * reporter_mem_init(void)
{
	struct REPORTER_MEM_T * reporter_mem = mmap(NULL, sizeof(struct REPORTER_MEM_T), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	sem_init(&(reporter_mem->data_sem), 1, 1);
	reporter_mem->abort_solve = 0;
	reporter_mem->abort_reporter = 0;
	reporter_mem->trials_completed = 0;
	memset(reporter_mem->current_data, 0, sizeof(reporter_mem->current_data));
	reporter_mem->enable_best_data = 0;
	memset(reporter_mem->best_data, 0, sizeof(reporter_mem->best_data));
	reporter_mem->enable_cycles = 0;
	reporter_mem->cycle_num = 0;
	reporter_mem->trials_in_cycle = 0;
	reporter_mem->trials_completed_in_cycle = 0;

	return reporter_mem;
}

void init_reporter_process(struct REPORTER_MEM_T * reporter_mem, struct Problem_T * problem)
{
	reporter_mem->abort_reporter = 0;
	pid_t pid = fork();
	if (pid != 0)
	{
		reporter_mem->reporter_pid = pid;
		return;
	}

	if (problem->trial_initializer)
	{
		problem->trial_initializer(problem);
	}

	setlocale(LC_ALL,"");

	F64 start_unix_time = getUnixTime();
	F64 prev_unix_time = start_unix_time;
	U64 prev_trials_completed = 0;
	while (!reporter_mem->abort_reporter)
	{
		U32 r = sleep(1);
		testing_assert("cli_reporter_sleep", r == 0);
		//usleep(1000000);
		struct REPORTER_MEM_T reporter_mem_buff;
		sem_wait(&(reporter_mem->data_sem));

		reporter_mem_buff.trials_completed = reporter_mem->trials_completed;
		memcpy(reporter_mem_buff.current_data, reporter_mem->current_data, problem->data_len);

		reporter_mem_buff.enable_best_data = reporter_mem->enable_best_data;
		if (reporter_mem_buff.enable_best_data)
		{
			memcpy(reporter_mem_buff.best_data, reporter_mem->best_data, problem->data_len);
		}

		reporter_mem_buff.enable_cycles = reporter_mem->enable_cycles;
		reporter_mem_buff.cycle_num = reporter_mem->cycle_num;
		reporter_mem_buff.trials_completed_in_cycle = reporter_mem->trials_completed_in_cycle;
		reporter_mem_buff.trials_in_cycle = reporter_mem->trials_in_cycle;
		sem_post(&(reporter_mem->data_sem));

		F64 current_score = 0;
		for (U8 i = 0; i < RETRY_COUNT; i++)
		{
			current_score += problem->scalar_trial(problem, reporter_mem_buff.current_data);
		}
		current_score = current_score/(F64)RETRY_COUNT;

		F64 best_score = 0;
		if (reporter_mem_buff.enable_best_data)
		{
			for (U8 i = 0; i < RETRY_COUNT; i++)
			{
				best_score += problem->scalar_trial(problem, reporter_mem_buff.best_data);
			}
			best_score = best_score/(F64)RETRY_COUNT;
		}

		F64 curr_unix_time = getUnixTime();
		U64 secs_elapsed = (U64)(curr_unix_time - start_unix_time);
		U64 mins_elapsed = secs_elapsed/60;
		secs_elapsed -= mins_elapsed*60;
		U64 hours_elapsed = mins_elapsed/60;
		mins_elapsed -= hours_elapsed;
		printf("\nTime: %02d:%02d:%02d\n", hours_elapsed, mins_elapsed, secs_elapsed);
		printf("Trial rate: %'d/s\n", (U64)((F64)(reporter_mem_buff.trials_completed - prev_trials_completed)/(curr_unix_time - prev_unix_time)));
		printf("Trials completed: %'d\n", reporter_mem_buff.trials_completed);

		if (reporter_mem->enable_cycles)
		{
			printf("Cycle: %d\n", reporter_mem_buff.cycle_num);
			printf("Cycle progress: %d/%d\n", reporter_mem_buff.trials_completed_in_cycle, reporter_mem_buff.trials_in_cycle);
		}

		printf("Current Score: %f\n", current_score);
	    printf("Current Data:\n");
	    problem->data_pretty_printer(problem, reporter_mem_buff.current_data);


		if (reporter_mem_buff.enable_best_data)
		{
			printf("Best Score: %f\n", current_score);
		    printf("Best Data:\n");
		    problem->data_pretty_printer(problem, reporter_mem_buff.best_data);
		}

		fflush(0);

	    prev_unix_time = curr_unix_time;
	    prev_trials_completed = reporter_mem_buff.trials_completed;
	}

	if (problem->trial_deinitializer)
	{
	    problem->trial_deinitializer(problem);
	}

	reporter_mem->abort_solve = 1;
}
void deinit_reporter_process(struct REPORTER_MEM_T * reporter_mem)
{
	reporter_mem->abort_reporter = 1;
	waitpid(reporter_mem->reporter_pid, NULL, 0);
}
