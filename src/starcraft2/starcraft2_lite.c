#include <stdlib.h>
#include "starcraft2_buildspec.h"
#include "types.h"

#define STEPS_PER_SEC 20

enum EventType_T
{
	EventType_FinishNexus,
	EventType_FinishProbe,
	EventType_FinishPylon,
	EventType_FinishGateway,
	EventType_FinishZealot,
	EventType_BuildStep,
	EventType_EndGame
};

struct Event_T
{
	U64 time;
	enum EventType_T type;
	struct StarCraft2BuildStep * step;
	struct Event_T * next_event;
	struct Structure_T * structure;
};

enum StructureType_T
{
	StructureType_Nexus,
	StructureType_Gateway,
	StructureType_Pylon,
	StructureType_Num_Structures
};

struct Structure_T
{
	enum StructureType_T type;
	struct Event_T * current_task;
	U64 crono_timeout;

	double energy;
	U64 last_energy_update_time;
};

#define MAX_NUM_STRUCTURES

struct Gamestate_T
{
	U64 last_resource_update_time;
	U64 time;
	U32 probes;
	double minerals;
	U32 zealots;

	U32 structure_count[StructureType_Num_Structures];
	struct Structure_T structures[100];
	U32 num_structures;
	struct Event_T * next_event;
};

void insert_event(struct Gamestate_T * game, struct Event_T * event)
{
	struct Event_T * prev_e = 0;
	struct Event_T * curr_e = game->next_event;
	while (curr_e && (curr_e->time < event->time))
	{
		prev_e = curr_e;
		curr_e = curr_e->next_event;
	}
	if (prev_e)
	{
		prev_e->next_event = event;
	}
	else
	{
		game->next_event = event;
	}
	event->next_event = curr_e;
}

void new_structure(struct Gamestate_T * game, enum StructureType_T type)
{
	game->structures[game->num_structures].crono_timeout = 0;
	game->structures[game->num_structures].current_task = 0;
	game->structures[game->num_structures].type = type;
	game->structures[game->num_structures].energy = 0;
	game->structures[game->num_structures].last_energy_update_time = 0;
	game->structure_count[type]++;
	game->num_structures++;
}

static void init_game(struct Gamestate_T * game)
{
	game->last_resource_update_time = 0;
	game->time = 0;
	game->probes = 12;
	game->minerals = 50;
	game->zealots = 0;
	game->next_event = 0;

	game->num_structures = 0;

	for (U32 i = 0; i < StructureType_Num_Structures; i++)
	{
		game->structure_count[i] = 0;
	}

	new_structure(game, StructureType_Nexus);
}

static struct Gamestate_T game;

void starcraft2_lite_print_game_stats()
{
	U64 game_s = game.time/STEPS_PER_SEC;
	printf("Game clock: %d:%02d\n", game_s/60, game_s%60);
	printf("Probes: %d\n", game.probes);
	printf("Minerals: %f\n", game.minerals);
	printf("Zealots: %d\n", game.zealots);
	printf("Pylons: %d\n", game.structure_count[StructureType_Pylon]);
	printf("Gateways: %d\n", game.structure_count[StructureType_Gateway]);
	printf("Nexi: %d\n", game.structure_count[StructureType_Nexus]);
}

void updateResources(struct Gamestate_T * game)
{
	double diff = ((double)(game->time - game->last_resource_update_time))/STEPS_PER_SEC;
	U32 working_probes = game->probes;
	if (working_probes > (game->nexus*16))
	{
		working_probes = game->nexus*16;
	}
	game->minerals += (diff*game->probes)*(50.0/60.0);
	game->last_resource_update_time = game->time;
}


#define CRONO_MULTIPLIER (2/3)

double starcraft2_lite_run_supplymax_build(struct StarCraft2BuildStep * build, U32 num_build_steps)
{
	init_game(&game);
	U64 game_time = 0;
	for (U32 i = 0; i < num_build_steps; i++)
	{
		struct Event_T * new_event = malloc(sizeof(struct Event_T));
		game_time += build[i].wait;
		new_event->type = EventType_BuildStep;
		new_event->time = game_time;
		new_event->step = &(build[i]);
		insert_event(&game, new_event);
	}


	struct Event_T * new_event;

	new_event = malloc(sizeof(struct Event_T));
	new_event->type = EventType_EndGame;
	new_event->time = STEPS_PER_SEC*60*20;
	new_event->step = 0;
	insert_event(&game, new_event);

	struct Event_T * current_event = game.next_event;
	U8 sim_done = 0;
	while (!sim_done && (current_event != 0))
	{
		game.time = current_event->time;
		switch (current_event->type)
		{
		case EventType_EndGame:
			updateResources(&game);
			sim_done = 1;
			break;
		case EventType_FinishNexus:
			updateResources(&game);
			new_structure(game, StructureType_Nexus);
			break;
		case EventType_FinishProbe:
			updateResources(&game);
			game.probes++;
			game.structures[current_event->step->selector].current_task = 0;
			break;
		case EventType_FinishZealot:
			game.zealots++;
			game.structures[current_event->step->selector].current_task = 0;
			break;
		case EventType_FinishPylon:
			new_structure(game, StructureType_Pylon);
			break;
		case EventType_FinishGateway:
			new_structure(game, StructureType_Gateway);
			break;
		case EventType_BuildStep:
			updateResources(&game);
			U32 supply_avail = game.structure_count[StructureType_Nexus]*15 + game.structure_count[StructureType_Pylon]*8;
			if (supply_avail > 200)
				supply_avail = 200;
			U32 supply_used = (game.probes + game.probes_building)*1 + (game.zealots + game.zealots_building)*2;
			U32 supply_left = supply_avail - supply_used;
			switch (current_event->step->command)
			{
			case StarCraft2BuildCommand_BuildProbe:
				if (current_event->step->selector > game.structure_count)
				{
					break;
				}
				if (game.structures[current_event->step->selector].type != StructureType_Nexus)
				{
					break;
				}
				if (game.structures[current_event->step->selector].current_task != 0)
				{
					break;
				}
				if (game.minerals < 50)
				{
					break;
				}
				if (supply_left < 1)
				{
					break;
				}
				game.minerals -= 50;
				new_event = malloc(sizeof(struct Event_T));
				new_event->type = EventType_FinishProbe;
				if (game.structures[current_event->step->selector].crono_timeout < game.time)
				{
					new_event->time = game.time + 12*STEPS_PER_SEC*CRONO_MULTIPLIER;
				}
				else
				{
					new_event->time = game.time + 12*STEPS_PER_SEC;
				}
				new_event->structure = &(game.structures[current_event->step->selector]);
				game.structures[current_event->step->selector].current_task = new_event;
				new_event->step = 0;
				insert_event(&game, new_event);
				break;
			case StarCraft2BuildCommand_BuildNexus:
				if (game.minerals < 400)
				{
					break;
				}
				game.minerals -= 400;
				new_event = malloc(sizeof(struct Event_T));
				new_event->type = EventType_FinishNexus;
				new_event->time = game.time + 71*STEPS_PER_SEC;
				new_event->step = 0;
				insert_event(&game, new_event);
				break;
			case StarCraft2BuildCommand_BuildPylon:
				if (game.minerals < 100)
				{
					break;
				}
				game.minerals -= 100;
				new_event = malloc(sizeof(struct Event_T));
				new_event->type = EventType_FinishPylon;
				new_event->time = game.time + 71*STEPS_PER_SEC;
				new_event->step = 0;
				insert_event(&game, new_event);
				break;
			case StarCraft2BuildCommand_BuildGateway:
				if (game.minerals < 150)
				{
					break;
				}
				game.minerals -= 150;
				new_event = malloc(sizeof(struct Event_T));
				new_event->type = EventType_FinishGateway;
				new_event->time = game.time + 46*STEPS_PER_SEC;
				new_event->step = 0;
				insert_event(&game, new_event);
				break;
			case StarCraft2BuildCommand_BuildZealot:
				if (current_event->step->selector > game.structure_count)
				{
					break;
				}
				if (game.structures[current_event->step->selector].type != StructureType_Gateway)
				{
					break;
				}
				if (game.structures[current_event->step->selector].current_task != 0)
				{
					break;
				}
				if (game.minerals < 100)
				{
					break;
				}
				if (supply_left < 2)
				{
					break;
				}
				game.minerals -= 100;
				new_event = malloc(sizeof(struct Event_T));
				new_event->type = EventType_FinishZealot;
				if (game.structures[current_event->step->selector].crono_timeout < game.time)
				{
					new_event->time = game.time + 27*STEPS_PER_SEC*CRONO_MULTIPLIER;
				}
				else
				{
					new_event->time = game.time + 27*STEPS_PER_SEC;
				}
				game.structures[current_event->step->selector].current_task = new_event;
				new_event->structure = &(game.structures[current_event->step->selector]);
				new_event->step = 0;
				insert_event(&game, new_event);
				break;
			}
			break;
		}
		U32 standing_supply_used = game.probes + game.zealots*2;
		if (standing_supply_used >= 200)
		{
			sim_done = 1;
			break;
		}

		game.next_event = current_event->next_event;
		free(current_event);
		current_event = game.next_event;
	}
	// Free all upcoming events
	while (game.next_event)
	{
		struct Event_T * next_event = game.next_event;
		game.next_event = next_event->next_event;
		free(next_event);
	}

	return game.time/STEPS_PER_SEC;
}

U32 starcraft2_lite_get_supply()
{
	return game.probes + game.zealots*2;
}
