#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <setjmp.h>
#include "vx32.h"
#include "args.h"
#include <time.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/types.h>
#include <stdint.h>

#include "vx32impl.h"

#include "academy.h"
#include "utils.h"

#define NUM_PROCESS_PROCESSES 8

#define IO_DATA_LEN VXPAGESIZE
#define PROG_DATA_LEN VXPAGESIZE

struct ml86_process_memory
{
	unsigned char io_data[IO_DATA_LEN];
	unsigned char program_data[PROG_DATA_LEN];
};

#define VXC_MAXNAMLEN 255

#define	VXC_FD_CLOEXEC	1

#define V (void*)(uintptr_t)

const char *argv0;

int trace;

#define RET proc->cpu->reg[EAX]
#define NUM proc->cpu->reg[EAX]
#define ARG1 proc->cpu->reg[EDX]
#define ARG2 proc->cpu->reg[ECX]
#define ARG3 proc->cpu->reg[EBX]
#define ARG4 proc->cpu->reg[EDI]
#define ARG5 proc->cpu->reg[ESI]

int run_proc(vxproc *volatile p)
{
	// Set up the process's initial register state
	memset(p->cpu->reg, 0, sizeof(p->cpu->reg));
	p->cpu->eflags = 0;
	p->cpu->eip = IO_DATA_LEN;
	vxproc_flush(p);
	int rc = vxproc_run(p);
	return rc;
}

char * goal = "I am mlx86!!!";
//char * goal = "Hi";
float task_judge_hello_world(unsigned char * data)
{
  float score = 0;

  //char * goal = "I am mlx86!!!";

  for (int i = 0; i < strlen(goal); i++) {
    int error = goal[i] - data[i];
    if (error < 0)
      error = -error;
    if (error > 5)
      error = 5;
    score += 1.0-(((float)error)/5.0);
  }

  return score/strlen(goal);
}

void print_as_hex(unsigned char * data, unsigned int len)
{
	unsigned int i;
	for(i = 0; i < len; i++)
	{
		printf("%02x ", data[i]);
	}
}

struct SharedVars_T
{
	sem_t academy_write_sems[NUM_PROCESS_PROCESSES];
	unsigned long maintenance_process_state;
	unsigned long maintenance_process_counter;
	unsigned long execution_process_states[NUM_PROCESS_PROCESSES];
	unsigned long execution_process_counters[NUM_PROCESS_PROCESSES];
};

struct SharedVars_T * sharedvars;

struct Academy_T * academy;

struct Add_Agent_Request_T
{
	ACADEMY_AGENT_ID parent_id;
	unsigned char data[PROG_DATA_LEN];
};

struct Add_Game_Request_T
{
	ACADEMY_AGENT_ID winner_id;
	ACADEMY_AGENT_ID looser_id;
	float winner_score;
	float looser_score;
};

void maintenance_process(int add_game_request_pipes[NUM_PROCESS_PROCESSES], int add_agent_request_pipes[NUM_PROCESS_PROCESSES], sem_t academy_write_sems[NUM_PROCESS_PROCESSES])
{
	printf("Maintenance process started.\n");
	sharedvars->maintenance_process_counter = 0;
	sharedvars->maintenance_process_state = 0;

	fd_set set;
	struct timeval timeout;

	int highest_fd = 0;

	/* Initialize the file descriptor set. */
	for (int i = 0; i < NUM_PROCESS_PROCESSES; i++)
	{
		if (add_game_request_pipes[i] > highest_fd)
		{
			highest_fd = add_game_request_pipes[i];
		}
		if (add_agent_request_pipes[i] > highest_fd)
		{
			highest_fd = add_agent_request_pipes[i];
		}
	}

	/* Initialize the timeout data structure. */
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	int retval;
	while (1)
	{
		FD_ZERO(&set);
		for (int i = 0; i < NUM_PROCESS_PROCESSES; i++)
		{
			FD_SET(add_game_request_pipes[i], &set);
			FD_SET(add_agent_request_pipes[i], &set);
		}
		retval = select(highest_fd+1, &set, NULL, NULL, &timeout);
		if (retval > 0)
		{
			for (int i = 0; i < NUM_PROCESS_PROCESSES; i++)
			{
				sem_wait(&(academy_write_sems[i]));
			}
			while (1)
			{
				FD_ZERO(&set);
				for (int i = 0; i < NUM_PROCESS_PROCESSES; i++)
				{
					FD_SET(add_game_request_pipes[i], &set);
					FD_SET(add_agent_request_pipes[i], &set);
				}
				retval = select(highest_fd+1, &set, NULL, NULL, &timeout);
				if (retval <= 0)
				{
					break;
				}
				for (int i = 0; i < NUM_PROCESS_PROCESSES; i++)
				{
					if (FD_ISSET(add_game_request_pipes[i], &set))
					{
						struct Add_Game_Request_T req;
						int len = read(add_game_request_pipes[i], &req, sizeof(req));
						if (len == sizeof(req))
						{
							academy_report_agent_win(academy, req.winner_id, req.winner_score, req.looser_id, req.looser_score);
						}
						else
						{
							printf("Received bad data in add_game_request_pipe\n");
						}
					}

					if (FD_ISSET(add_agent_request_pipes[i], &set))
					{
						struct Add_Agent_Request_T req;
						int len = read(add_agent_request_pipes[i], &req, sizeof(req));
						if (len == sizeof(req))
						{
							academy_add_new_agent(academy, req.parent_id, req.data, sizeof(req.data));
						}
						else
						{
							printf("Received bad data in add_agent_request_pipe\n");
						}
					}
				}
			}
			for (int i = 0; i < NUM_PROCESS_PROCESSES; i++)
			{
				sem_post(&(academy_write_sems[i]));
			}
			//printf("Finished a batch!\n");
		}
		usleep(100);
		sharedvars->maintenance_process_counter++;
		if (academy->should_exit)
		{
			break;
		}
	}
	printf("Maintenance process ended.\n");
}

void execution_process(int exec_proc_id ,int add_game_request_pipe, int add_agent_request_pipe, sem_t * academy_write_sem)
{
	printf("Execution process %d started.\n", exec_proc_id);
	sharedvars->execution_process_counters[exec_proc_id] = 0;
	unsigned long process_games_played = 0;

	vx32_siginit();

	vxproc *volatile p = vxproc_alloc();
	p->allowfp = 1;

	vxmem * mem = vxmem_chunk_new(sizeof(struct ml86_process_memory));
	vxmmap * mem_map = vxmem_map(mem, 0);
	struct ml86_process_memory * proc_mem = mem_map->base;
	vxmem_setperm(mem, 0, sizeof(struct ml86_process_memory), VXPERM_READ | VXPERM_WRITE | VXPERM_EXEC);
	p->mem = mem;

	while (1)
	{
		ACADEMY_AGENT_ID node_0_id = ACADEMY_INVALID_AGENT_ID;
		ACADEMY_AGENT_ID node_1_id = ACADEMY_INVALID_AGENT_ID;
		struct Academy_Agent_T * node_0 = NULL;
		struct Academy_Agent_T * node_1 = NULL;
		float score_0 = 0;
		float score_1 = 0;

		sharedvars->execution_process_states[exec_proc_id] = 0;

		int fuck_ctr = 0;
		sem_wait(academy_write_sem);

		sharedvars->execution_process_states[exec_proc_id] = 1;

		while (node_0_id == node_1_id)
		{
			if (process_games_played %2)
				academy_select_matchup(academy, &node_0_id, &node_1_id);
			else
				academy_select_matchup(academy, &node_1_id, &node_0_id);

			fuck_ctr++;

			if (fuck_ctr > 1000)
			{
				printf("\n\nFuck... \n");
				exit(1);
			}
			sharedvars->execution_process_states[exec_proc_id] = 2;
		}

		node_0 = academy_get_agent_from_id(academy, node_0_id);
		node_1 = academy_get_agent_from_id(academy, node_1_id);

		sharedvars->execution_process_states[exec_proc_id] = 3;

		memset(proc_mem->io_data, 0, IO_DATA_LEN);
		memcpy(proc_mem->program_data, node_0->data, node_0->data_len);
		sem_post(academy_write_sem);

		sharedvars->execution_process_states[exec_proc_id] = 4;

		run_proc(p);
		sharedvars->execution_process_states[exec_proc_id] = 40;
		mem_map = vxmem_map(mem, 0);
		sharedvars->execution_process_states[exec_proc_id] = 41;
		proc_mem = mem_map->base;
		sharedvars->execution_process_states[exec_proc_id] = 42;
		score_0 = task_judge_hello_world(proc_mem->io_data);

		sharedvars->execution_process_states[exec_proc_id] = 5;

		memset(proc_mem->io_data, 0, IO_DATA_LEN);
		memcpy(proc_mem->program_data, node_1->data, node_1->data_len);

		sharedvars->execution_process_states[exec_proc_id] = 6;

		run_proc(p);
		mem_map = vxmem_map(mem, 0);
		proc_mem = mem_map->base;
		score_1 = task_judge_hello_world(proc_mem->io_data);

		sharedvars->execution_process_states[exec_proc_id] = 7;

		if (score_1 > score_0)
		{
			float third_score;
			ACADEMY_AGENT_ID third_id;
			third_score = score_0;
			third_id = node_0_id;
			score_0 = score_1;
			node_0_id = node_1_id;
			score_1 = third_score;
			node_1_id = third_id;
		}

		sharedvars->execution_process_states[exec_proc_id] = 8;

		/* Winner has been set to node 0 */
		struct Add_Game_Request_T new_g_request;
		new_g_request.winner_id = node_0_id;
		new_g_request.winner_score = score_0;
		new_g_request.looser_id = node_1_id;
		new_g_request.looser_score = score_1;
		write(add_game_request_pipe, &new_g_request, sizeof(new_g_request));

		sharedvars->execution_process_states[exec_proc_id] = 9;

		// Occasionally make a new node
		if (!(process_games_played % 3))
		{
			sharedvars->execution_process_states[exec_proc_id] = 10;
			sem_wait(academy_write_sem);
			node_0 = academy_get_agent_from_id(academy, node_0_id);
			if (node_0)
			{
				sharedvars->execution_process_states[exec_proc_id] = 11;

				struct Add_Agent_Request_T new_a_request;
				new_a_request.parent_id = node_0_id;
				memcpy(new_a_request.data, node_0->data, PROG_DATA_LEN);
				/* Make a random change and introduce a new agent */
				unsigned char * to_change = new_a_request.data + fast_rand()%(40);
				*to_change = fast_rand()&0xFF;

				write(add_agent_request_pipe, &new_a_request, sizeof(new_a_request));
			}
			else
			{
				printf("This sucks!\n");
			}
			sem_post(academy_write_sem);

			sharedvars->execution_process_states[exec_proc_id] = 12;
		}

		process_games_played++;
		sharedvars->execution_process_counters[exec_proc_id]++;

		if (academy->should_exit)
		{
			break;
		}
	}
	printf("Execution process ended.\n");
}

void status_reporting_process()
{
	double start_time = getUnixTime();
	while(1)
	{
		sleep(1);
		struct Academy_Agent_T * tree_root = academy_get_agent_from_id(academy, academy->root_agent_id);
		printf("Played %ld games.\n", academy->games_played);
		printf("Made %ld agents.\n", academy->last_agent_id);
		printf("Remembering %ld agents.\n", academy->agent_count);
		printf("Root value: %f \n", tree_root->own_metadata.value);
		printf("Agent ID table max offset: %ld \n", academy->agent_id_hashtable_max_offset);
		printf("Best score: %f\n", academy->max_value);
		printf("Maintenance process counter: %ld\n", sharedvars->maintenance_process_counter);
		printf("Maintenance process state: %ld\n", sharedvars->maintenance_process_state);
		for (int i = 0; i < NUM_PROCESS_PROCESSES; i++)
		{
			printf("Execution process %d counter: %ld\n", i, sharedvars->execution_process_counters[i]);
			printf("Execution process %d state: %ld\n", i, sharedvars->execution_process_states[i]);
		}
		putchar('\n');
		if (academy->games_played > 1000000 || academy->should_exit)
		{
			//export_academy(academy, "academy.gexf");
			break;
		}
	}
	printf("Finished after %f seconds.\n", (getUnixTime() - start_time));
}

int main(int argc, const char *const *argv)
{
	academy = mmap(NULL, sizeof(struct Academy_T), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	sharedvars = mmap(NULL, sizeof(struct SharedVars_T), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	build_new_academy(academy);

	// Init with two null programs
	unsigned char data[PROG_DATA_LEN];
	memset(data, 0x90, PROG_DATA_LEN);
	academy_add_new_agent(academy, ACADEMY_INVALID_AGENT_ID, data, PROG_DATA_LEN);
	memset(data, 0x90, PROG_DATA_LEN);
	//data[0] = 0xC6;
	//data[1] = 0x05;
	//data[6] = 0x49;
	//data[0] = 0xEB;
	//data[1] = 0xFE;
	//data[99] = 0x5F;
	//data[173] = 0x7F;
	//data[335] = 0x8B;
	data[100] = 0xFF;
	academy_add_new_agent(academy, academy->root_agent_id, data, PROG_DATA_LEN);

	// Create pipes
	int add_agent_request_pipes[NUM_PROCESS_PROCESSES][2];
	int add_game_request_pipes[NUM_PROCESS_PROCESSES][2];
	for (int i = 0; i < NUM_PROCESS_PROCESSES; i++)
	{
		pipe(add_agent_request_pipes[i]);
		fcntl(add_agent_request_pipes[i][0], F_SETPIPE_SZ, sizeof(struct Add_Agent_Request_T)*100);
		pipe(add_game_request_pipes[i]);
		fcntl(add_game_request_pipes[i][0], F_SETPIPE_SZ, sizeof(struct Add_Game_Request_T)*100);
	}
	// Create semaphores
	for (int i = 0; i < NUM_PROCESS_PROCESSES; i++)
	{
		sem_init(&(sharedvars->academy_write_sems[i]), 1, 1);
	}
	// Fork execution processes
	pid_t execution_pids[NUM_PROCESS_PROCESSES];
	for (int i = 0; i < NUM_PROCESS_PROCESSES; i++)
	{
		execution_pids[i] = fork();
		if (execution_pids[i] == 0)
		{
			// Nudge rng
			seed_fast_rand(i);
			execution_process(i, add_game_request_pipes[i][1], add_agent_request_pipes[i][1], &(sharedvars->academy_write_sems[i]));
			return 0;
		}
	}
	// Fork management process
	pid_t maintenance_pid = fork();
	if (maintenance_pid == 0)
	{
		int add_agent_request_read_pipes[NUM_PROCESS_PROCESSES];
		int add_game_request_read_pipes[NUM_PROCESS_PROCESSES];
		for (int i = 0; i < NUM_PROCESS_PROCESSES; i++)
		{
			add_agent_request_read_pipes[i] = add_agent_request_pipes[i][0];
			add_game_request_read_pipes[i] = add_game_request_pipes[i][0];
		}
		maintenance_process(add_game_request_read_pipes, add_agent_request_read_pipes, sharedvars->academy_write_sems);
		return 0;
	}
	// Reporting process
	status_reporting_process();
	// Close all threads
	academy->should_exit = 1;
	// Wait for subprocesses
	waitpid(maintenance_pid, NULL, 0);
	for (int i = 0; i < NUM_PROCESS_PROCESSES; i++)
	{
		waitpid(execution_pids[i], NULL, 0);
	}
}
