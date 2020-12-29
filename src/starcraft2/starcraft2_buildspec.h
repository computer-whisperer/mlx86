#ifndef STARCRAFT2_BUILDSPEC_H
#define STARCRAFT2_BUILDSPEC_H
#include "types.h"


enum StarCraft2BuildCommand
{
	StarCraft2BuildCommand_BuildProbe,
	StarCraft2BuildCommand_BuildNexus,
	StarCraft2BuildCommand_BuildPylon,
	StarCraft2BuildCommand_BuildGateway,
	StarCraft2BuildCommand_BuildZealot,
	StarCraft2BuildCommand_NumCommands
};

struct StarCraft2BuildStep
{
	U32 wait;
	U32 selector; // Which building, which unit, etc
	enum StarCraft2BuildCommand command;
};

#endif
