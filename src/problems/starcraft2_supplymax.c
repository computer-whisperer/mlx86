#include <stdio.h>
#include "problems/problems.h"
#include "starcraft2/starcraft2.h"
#include "starcraft2/starcraft2_buildspec.h"
#include "problems/common.h"
#include "types.h"
#include "utils.h"

float scalar_trial_starcraft2_supplymax(struct Problem_T * problem, U8 * data)
{
	starcraft2_run_build((struct StarCraft2BuildStep *)data, problem->data_len/sizeof(struct StarCraft2BuildStep));
	double time = starcraft2_check_supplymax_time();
	return 1-(0/600);
}

void starcraft2_scramble_init(struct Problem_T * problem, U8 * data)
{
	U32 build_len = problem->data_len/sizeof(struct StarCraft2BuildStep);
	struct StarCraft2BuildStep * build = (struct StarCraft2BuildStep *)data;
	for (U32 i = 0; i < build_len; i++)
	{
		build[i].wait = 20;
		build[i].command = StarCraft2BuildCommand_BuildProbe;
	}
}

void starcraft2_scramble(struct Problem_T * problem, U8 * data)
{
	U32 build_len = problem->data_len/sizeof(struct StarCraft2BuildStep);
	struct StarCraft2BuildStep * build = (struct StarCraft2BuildStep *)data;
	U32 step_idx = fast_rand()%build_len;
	if ((fast_rand()%2) == 0)
	{
		build[step_idx].wait = fast_rand()%100;
	}
	if ((fast_rand()%2) == 0)
	{
		build[step_idx].command = fast_rand()%StarCraft2BuildCommand_NumCommands;
	}
}


void starcraft2_printbuild(struct Problem_T * problem, U8 * data)
{
	U32 build_len = problem->data_len/sizeof(struct StarCraft2BuildStep);
	if (build_len > 10)
		build_len = 10;
	struct StarCraft2BuildStep * build = (struct StarCraft2BuildStep *)data;
	for (U32 i = 0; i < build_len; i++)
	{
		U8 * command = "";
		switch (build[i].command)
		{
		case StarCraft2BuildCommand_BuildProbe:
			command = "Build Probe";
			break;
		case StarCraft2BuildCommand_BuildNexus:
			command = "Build Nexus";
			break;
		case StarCraft2BuildCommand_BuildPylon:
			command = "Build Pylon";
			break;
		case StarCraft2BuildCommand_BuildGateway:
			command = "Build Gateway";
			break;
		case StarCraft2BuildCommand_BuildZealot:
			command = "Build Zealot";
			break;
		}
		printf("Wait %d steps and %s\n", build[i].wait, command);
	}
}

struct Problem_T problem_starcraft2_supplymax =
{
	scalar_trial_starcraft2_supplymax,
	starcraft2_scramble,
	starcraft2_printbuild,
	starcraft2_scramble_init,
	0x1000,
	0
};
