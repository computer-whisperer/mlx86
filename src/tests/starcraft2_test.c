#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "starcraft2_lite.h"

#include "types.h"


int main(int argc, char **argv)
{
    struct StarCraft2BuildStep build[] = {
    		{20*0, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildPylon},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildNexus},
			{20*13, StarCraft2BuildCommand_BuildPylon},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildPylon},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildPylon},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildPylon},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
			{20*13, StarCraft2BuildCommand_BuildProbe},
    };
    starcraft2_run_build(build, sizeof(build)/sizeof(struct StarCraft2BuildStep));
    //printf("Game finished, time: %f\n", t);
    //starcraft2_lite_print_game_stats();
}


