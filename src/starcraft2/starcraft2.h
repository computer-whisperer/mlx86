#include "starcraft2_buildspec.h"
#include "types.h"
void starcraft2_init_sc2(void);
void starcraft2_deinit_sc2(void);
void starcraft2_run_build(struct StarCraft2BuildStep * build, U32 num_build_steps);
double starcraft2_check_supplymax_time();
