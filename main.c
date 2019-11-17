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
static const char *progname;
static pid_t progpid;


static void dumpregs(struct vxproc *p)
{
	struct vxcpu *c = p->cpu;

	fprintf(stderr, "eax %08x  ecx %08x  edx %08x  ebx %08x\n",
		c->reg[EAX], c->reg[ECX], c->reg[EDX], c->reg[EBX]);
	fprintf(stderr, "esp %08x  ebp %08x  esi %08x  edi %08x\n",
		c->reg[ESP], c->reg[EBP], c->reg[ESI], c->reg[EDI]);
	fprintf(stderr, "eip %08x  eflags %08x\n",
		c->eip, c->eflags);

//	for (int i = 0; i < 8; i++) {
//		int32_t *val = r.xmm[i].i32;
//		fprintf(stderr, "xmm%d %08x%08x%08x%08x\n",
//			i, val[3], val[2], val[1], val[0]);
//	}
}
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
	p->cpu->reg[ESP] = 0;
	p->cpu->eflags = 0;
	p->cpu->eip = IO_DATA_LEN;
	vxproc_flush(p);
	return vxproc_run(p);
}

float task_judge_hello_world(unsigned char * data)
{
  float score = 0;

  char * goal = "I am mlx86!!!";

  for (int i = 0; i < strlen(goal); i++) {
    int error = goal[i] - data[i];
    if (error < 0)
      error = -error;
    if (error > 50)
      error = 50;
    score += 1-(((float)error)/50.0);
  }

  return score/strlen(goal);
}

int main(int argc, const char *const *argv)
{
	progname = argv[0];
	progpid = getpid();

	vx32_siginit();

	struct Academy_T * academy = build_new_academy();

	// Init with two null programs
	char * data = malloc(PROG_DATA_LEN);
	struct Academy_Agent_T * tree_root = academy_add_new_agent(academy, NULL, data, PROG_DATA_LEN);
	data = malloc(PROG_DATA_LEN);
	data[0] = 0xC6;
	data[1] = 0x05;
	data[6] = 0x49;
	academy_add_new_agent(academy, tree_root, data, PROG_DATA_LEN);

	vxproc *volatile p = vxproc_alloc();
	p->allowfp = 1;

	vxmem * mem = vxmem_chunk_new(sizeof(struct ml86_process_memory));
	vxmmap * mem_map = vxmem_map(mem, 0);
	struct ml86_process_memory * proc_mem = mem_map->base;
	vxmem_setperm(mem, 0, sizeof(struct ml86_process_memory), VXPERM_READ | VXPERM_WRITE | VXPERM_EXEC);
	p->mem = mem;

	int games_played = 0;

	float best_score = 0;

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

			if (fuck_ctr > 100)
			{
				printf("\n\nFuck... \n");
				exit(1);
			}
		}

		memset(proc_mem->io_data, 0, IO_DATA_LEN);
		memcpy(proc_mem->program_data, node_0->data, node_0->data_len);

		run_proc(p);
		score_0 = task_judge_hello_world(proc_mem->io_data);

		memset(proc_mem->io_data, 0, IO_DATA_LEN);
		memcpy(proc_mem->program_data, node_1->data, node_1->data_len);

		run_proc(p);
		score_1 = task_judge_hello_world(proc_mem->io_data);

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

		if (score_0 > best_score)
			best_score = score_0;

		unsigned char * new_data = malloc(PROG_DATA_LEN);
		memcpy(new_data, node_0->data, PROG_DATA_LEN);

		/* Make a random change and introduce a new agent */
		unsigned char * to_change = new_data + fast_rand()%PROG_DATA_LEN;
		*to_change = fast_rand()&0xFF;

		academy_add_new_agent(academy, node_0, new_data, PROG_DATA_LEN);

		if (!(games_played % 10000)) {
			printf("\n\nGeneration: %d \n", node_0->generation);
			printf("Played %d games.\n", games_played);
			printf("Made %d agents.\n", academy->agent_count);
			printf("Remembering %d agents.\n", academy->loaded_agent_count);
			printf("Rejected %d duplicate agents.\n", academy->duplicates_rejected);
			printf("Root value: %f \n", tree_root->own_value);
			printf("Hashtable len: %d \n", academy->hashtable_len);
		}

		if (games_played > 100000)
		{
			/* Export last candidate. */
			FILE * fp = fopen("result", "w");
			fwrite(node_0->data, node_0->data_len, 1, fp);
			fclose(fp);
			export_academy(academy, "academy.gexf");
			break;
		}

		games_played++;
	}
	printf("Best score: %f\n", best_score);
}
