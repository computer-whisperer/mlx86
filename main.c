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

#include <sys/types.h>
#include <stdint.h>

#include "vx32impl.h"

#include "academy.h"
#include "utils.h"

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

int main(int argc, const char *const *argv)
{

	double start_time = getUnixTime();
	double last_update = getUnixTime();

	vx32_siginit();

	struct Academy_T * academy = malloc(sizeof(struct Academy_T));
	build_new_academy(academy);

	// Init with two null programs
	unsigned char data[PROG_DATA_LEN];
	memset(data, 0x90, PROG_DATA_LEN);
	academy_add_new_agent(academy, NULL, data, PROG_DATA_LEN);
	struct Academy_Agent_T * tree_root = academy_get_agent_from_id(academy, academy->root_agent_id);
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
	academy_add_new_agent(academy, tree_root, data, PROG_DATA_LEN);

	vxproc *volatile p = vxproc_alloc();
	p->allowfp = 1;

	vxmem * mem = vxmem_chunk_new(sizeof(struct ml86_process_memory));
	vxmmap * mem_map = vxmem_map(mem, 0);
	struct ml86_process_memory * proc_mem = mem_map->base;
	vxmem_setperm(mem, 0, sizeof(struct ml86_process_memory), VXPERM_READ | VXPERM_WRITE | VXPERM_EXEC);
	p->mem = mem;

	unsigned long games_played = 0;

	float best_score = 0;
	unsigned char best_output[50];
	struct Academy_Agent_T * best_node = NULL;

	while (1)
	{
		struct Academy_Agent_T * node_0 = NULL;
		struct Academy_Agent_T * node_1 = NULL;
		float score_0 = 0;
		float score_1 = 0;

		int fuck_ctr = 0;
		while (node_0 == node_1) {
			if (games_played %2)
				academy_select_matchup(academy, &node_0, &node_1);
			else
				academy_select_matchup(academy, &node_1, &node_0);

			fuck_ctr++;

			if (fuck_ctr > 1000)
			{
				printf("\n\nFuck... \n");
				exit(1);
			}
		}

		memset(proc_mem->io_data, 0, IO_DATA_LEN);
		memcpy(proc_mem->program_data, node_0->data, node_0->data_len);

		run_proc(p);
		mem_map = vxmem_map(mem, 0);
		proc_mem = mem_map->base;
		score_0 = task_judge_hello_world(proc_mem->io_data);

		if (score_0 > best_score)
		{
			best_score = score_0;
			memcpy(best_output, proc_mem->io_data, sizeof(best_output));
			best_node = node_0;
		}

		memset(proc_mem->io_data, 0, IO_DATA_LEN);
		memcpy(proc_mem->program_data, node_1->data, node_1->data_len);

		run_proc(p);
		mem_map = vxmem_map(mem, 0);
		proc_mem = mem_map->base;
		score_1 = task_judge_hello_world(proc_mem->io_data);

		if (score_1 > best_score)
		{
			best_score = score_1;
			memcpy(best_output, proc_mem->io_data, sizeof(best_output));
			best_node = node_1;
		}

		if (score_0 >= score_1)
		{
			academy_report_agent_win(node_0, score_0, node_1, score_1);
		}
		else
		{
			academy_report_agent_win(node_1, score_1, node_0, score_0);
			node_0 = node_1;
			score_0 = score_1;
		}
		/* Winner has been set to node 0 */

		// Occasionally make a new node

		if (!(games_played % 3))
		{
			memcpy(data, node_0->data, PROG_DATA_LEN);

			/* Make a random change and introduce a new agent */
			unsigned char * to_change = data + fast_rand()%(40);
			*to_change = fast_rand()&0xFF;

			academy_add_new_agent(academy, node_0, data, PROG_DATA_LEN);
		}

	    if (!((games_played % 100)&&0) && (getUnixTime()-last_update > 0.5)) {
	        last_update += 0.5;
			printf("\n\nGeneration: %ld \n", node_0->generation);
			printf("Played %ld games.\n", games_played);
			printf("Made %ld agents.\n", academy->last_agent_id);
			printf("Remembering %ld agents.\n", academy->agent_count);
			printf("Root value: %f \n", tree_root->own_metadata.value);
			printf("Agent ID table max offset: %ld \n", academy->agent_id_hashtable_max_offset);
			printf("Best score: %f\n", best_score);
			printf("Best output: ");
			print_as_hex(best_output, strlen(goal));
			printf("\nGoal output: ");
			print_as_hex((unsigned char *)goal, strlen(goal));
			putchar('\n');
		}

		if (games_played > 10000000)
		{
			/* Export last candidate. */
			if (best_node)
			{
				FILE * fp = fopen("result", "w");
				fwrite(best_node->data, best_node->data_len, 1, fp);
				fclose(fp);
			}
			//export_academy(academy, "academy.gexf");
			break;
		}

		games_played++;
	}
	 printf("Finished after %f seconds.\n", (getUnixTime() - start_time));
}
