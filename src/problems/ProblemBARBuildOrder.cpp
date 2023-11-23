//
// Created by christian on 11/11/23.
//

#include "ProblemBARBuildOrder.hpp"
#include "christian_utils.h"
#include <opencv2/opencv.hpp>

double ProblemBARBuildOrder::scalarTrial(uint8_t *data) {
  return scalarTrialHeavyTankRush(data);
}

static double scalarTrialPawnDefence(Game<BAR_game_config>::Player* player, uint32_t tick)
{
  float num_turrets = 0;
  num_turrets += (float)bar_game_get_num_of_unit_type(player, BAR_UnitType_armpw);
  num_turrets += (float)bar_game_get_num_of_unit_type(player, BAR_UnitType_corak);
  float expected_num_turrets = floor((((float)tick/(float)BAR_game_config::game_tps)/60)*5);
  float score = (num_turrets - expected_num_turrets)/3;
  if (score > 0)
  {
    score = 0;
  }
  return score;
}

static double scalarTrialTurretDefence(Game<BAR_game_config>::Player* player, uint32_t tick)
{
  float num_turrets = 0;
  num_turrets += (float)bar_game_get_num_of_unit_type(player, BAR_UnitType_armllt);
  num_turrets += (float)bar_game_get_num_of_unit_type(player, BAR_UnitType_corllt);
  float num_mines = 0;
  num_mines += (float)bar_game_get_num_of_unit_type(player, BAR_UnitType_armmex);
  num_mines += (float)bar_game_get_num_of_unit_type(player, BAR_UnitType_cormex);
  num_mines += (float)bar_game_get_num_of_unit_type(player, BAR_UnitType_armmoho);
  num_mines += (float)bar_game_get_num_of_unit_type(player, BAR_UnitType_cormoho);
  num_mines += (float)bar_game_get_num_of_unit_type(player, BAR_UnitType_armamex);
  num_mines += (float)bar_game_get_num_of_unit_type(player, BAR_UnitType_corexp);
  float expected_num_turrets = floor((((float)tick/(float)BAR_game_config::game_tps)/60)*num_mines*0.2);
  float score = (num_turrets - expected_num_turrets)/3;
  if (score > 0)
  {
    score = 0;
  }
  return score;
}

double ProblemBARBuildOrder::scalarTrialHeavyTankRush(uint8_t *data) const {
  auto instructions = (Game<BAR_game_config>::Instruction*)data;
  Game<BAR_game_config> game;
  auto player = game.add_player(faction);
  player->instructions = instructions;
  player->num_instructions = num_instructions;

  double score = 0;
  for (uint32_t i = 0; i < sim_time_ticks; i++)
  {
    game.do_tick();
    score += scalarTrialTurretDefence(player, i);
  }
  score /= sim_time_ticks;

  // T1 Lab Incentive
  uint32_t num_t1_factories = 0;
  num_t1_factories += bar_game_get_num_of_unit_type(player, BAR_UnitType_armvp);
  num_t1_factories += bar_game_get_num_of_unit_type(player, BAR_UnitType_corvp);
  if (num_t1_factories > 0)
  {
    score += 0.3;
  }

  // T2 Factory Incentive
  uint32_t num_t2_factories = 0;
  num_t2_factories += bar_game_get_num_of_unit_type(player, BAR_UnitType_armavp);
  num_t2_factories += bar_game_get_num_of_unit_type(player, BAR_UnitType_coravp);
  if (num_t2_factories > 0)
  {
    score += 0.2;
  }



  score += (float)bar_game_get_num_of_unit_type(player, BAR_UnitType_armbull)*2;
  score += (float)bar_game_get_num_of_unit_type(player, BAR_UnitType_correap)*2;

  score += scalarTrialGeneralRules(&game, player, instructions);
  return score;
}


double ProblemBARBuildOrder::scalarTrialFastTankRush(uint8_t *data) const {
  auto instructions = (Game<BAR_game_config>::Instruction*)data;
  Game<BAR_game_config> game;
  auto player = game.add_player(faction);
  player->instructions = instructions;
  player->num_instructions = num_instructions;

  double score = 0;
  for (uint32_t i = 0; i < sim_time_ticks; i++)
  {
    game.do_tick();
  }
  score /= sim_time_ticks;

  score += (float)bar_game_get_num_of_unit_type(player, BAR_UnitType_armflash) / 5;
  score += (float)bar_game_get_num_of_unit_type(player, BAR_UnitType_corgator) / 5;

  score += scalarTrialGeneralRules(&game, player, instructions);
  return score;
}


double ProblemBARBuildOrder::scalarTrialMediumTankRush(uint8_t *data) const {
  auto instructions = (Game<BAR_game_config>::Instruction*)data;
  Game<BAR_game_config> game;
  auto player = game.add_player(faction);
  player->instructions = instructions;
  player->num_instructions = num_instructions;

  double score = 0;
  for (uint32_t i = 0; i < sim_time_ticks; i++)
  {
    game.do_tick();
  }
  score /= sim_time_ticks;

  score += (float)bar_game_get_num_of_unit_type(player, BAR_UnitType_armstump) / 5;
  score += (float)bar_game_get_num_of_unit_type(player, BAR_UnitType_corraid) / 5;

  score += scalarTrialGeneralRules(&game, player, instructions);
  return score;
}

double ProblemBARBuildOrder::scalarTrialTickSpam(uint8_t *data) const {
  auto instructions = (Game<BAR_game_config>::Instruction*)data;
  Game<BAR_game_config> game;
  auto player = game.add_player(faction);
  player->instructions = instructions;
  player->num_instructions = num_instructions;

  double score = 0;
  for (uint32_t i = 0; i < sim_time_ticks; i++)
  {
    game.do_tick();
  }
  score /= sim_time_ticks;

  score += (float)bar_game_get_num_of_unit_type(player, BAR_UnitType_armflea) / 100;
  score += (float)bar_game_get_num_of_unit_type(player, BAR_UnitType_corak) / 100;

  score += scalarTrialGeneralRules(&game, player, instructions);
  return score;
}


double ProblemBARBuildOrder::scalarTrialGauntletRush(uint8_t *data) const {
  auto instructions = (Game<BAR_game_config>::Instruction*)data;
  Game<BAR_game_config> game;
  auto player = game.add_player(faction);
  player->instructions = instructions;
  player->num_instructions = num_instructions;

  double score = 0;
  for (uint32_t i = 0; i < sim_time_ticks; i++)
  {
    game.do_tick();
    if (bar_game_get_num_of_unit_type(player, BAR_UnitType_armguard))
    {
      score += 1;
    }
    if (bar_game_get_num_of_unit_type(player, BAR_UnitType_armatlas))
    {
      score += 1;
    }
  }
  score /= sim_time_ticks;


  score += scalarTrialGeneralRules(&game, player, instructions);
  return score;
}


double ProblemBARBuildOrder::scalarTrialBomberRush(uint8_t *data) const {
  auto instructions = (Game<BAR_game_config>::Instruction*)data;
  Game<BAR_game_config> game;
  auto player = game.add_player(faction);
  player->instructions = instructions;
  player->num_instructions = num_instructions;

  double score = 0;
  for (uint32_t i = 0; i < sim_time_ticks; i++)
  {
    game.do_tick();

    if (bar_game_get_num_of_unit_type(player, BAR_UnitType_armliche))
    {
      score += 5;
    }
    if (bar_game_get_num_of_unit_type(player, BAR_UnitType_armliche) >= 2)
    {
      score += 20;
    }
  }
  score /= sim_time_ticks;

  score += player->resource_production_rate_cache[BAR_ResourceType_Metal] / 1000;
  score += player->resource_production_rate_cache[BAR_ResourceType_Energy] / 100000;

  if (bar_game_get_num_of_unit_type(player, BAR_UnitType_armaap))
  {
    score += 1;
  }

  if (bar_game_get_num_of_unit_type(player, BAR_UnitType_armap))
  {
    score += 1;
  }

  score += scalarTrialGeneralRules(&game, player, instructions);
  return score;
}

double ProblemBARBuildOrder::scalarTrialNukeRush(uint8_t *data) const {
  auto instructions = (Game<BAR_game_config>::Instruction*)data;
  Game<BAR_game_config> game;
  auto player = game.add_player(faction);
  player->instructions = instructions;
  player->num_instructions = num_instructions;

  double score = 0;
  float armageddon_count = 0;
  for (uint32_t i = 0; i < sim_time_ticks; i++)
  {
    game.do_tick();


    float nuke_energy_count = (float)(player->resource_production_rate_cache[BAR_ResourceType_Energy] - bar_game_get_full_upkeep(player, BAR_ResourceType_Energy)) / (float)((1111 * bar_game_resource_denominator) / bar_game_tps);
    armageddon_count = (float)(bar_game_get_num_of_unit_type(player, BAR_UnitType_armsilo) + bar_game_get_num_of_unit_type(player, BAR_UnitType_corsilo));
    if (nuke_energy_count < armageddon_count)
    {
      armageddon_count = nuke_energy_count;
    }
    score += armageddon_count/2;
  }
  score /= sim_time_ticks;

  score += armageddon_count * 3;

  player->update_cached_resource_rates();

  score += scalarTrialGeneralRules(&game, player, instructions);
  return score;
}

double ProblemBARBuildOrder::scalarTrialDragonRush(uint8_t *data) const {
  auto instructions = (Game<BAR_game_config>::Instruction*)data;
  Game<BAR_game_config> game;
  auto player = game.add_player(BAR_Faction_Cortex);
  player->instructions = instructions;
  player->num_instructions = num_instructions;

  double score = 0;
  for (uint32_t i = 0; i < sim_time_ticks; i++)
  {
    game.do_tick();

    if (bar_game_get_num_of_unit_type(player, BAR_UnitType_corap))
    {
      score += 0.1;
    }
    if (bar_game_get_num_of_unit_type(player, BAR_UnitType_corca))
    {
      score += 0.1;
    }
    if (bar_game_get_num_of_unit_type(player, BAR_UnitType_coraap))
    {
      score += 0.1;
    }
    score += bar_game_get_num_of_unit_type(player, BAR_UnitType_corcrw);
  }
  score /= sim_time_ticks;

  score += scalarTrialGeneralRules(&game, player, instructions);
  return score;
}

double ProblemBARBuildOrder::scalarTrialEconomyRush(uint8_t *data) const {
  auto instructions = (Game<BAR_game_config>::Instruction*)data;
  Game<BAR_game_config> game;
  auto player = game.add_player(faction);
  player->instructions = instructions;
  player->num_instructions = num_instructions;

  double score = 0;
  for (uint32_t i = 0; i < sim_time_ticks; i++)
  {
    game.do_tick();
  }
  score /= sim_time_ticks;

  score += scalarTrialGeneralRules(&game, player, instructions);
  return score;
}

double ProblemBARBuildOrder::scalarTrialCorsairFlood(uint8_t *data) const {
  auto instructions = (Game<BAR_game_config>::Instruction*)data;
  Game<BAR_game_config> game;
  auto player = game.add_player(faction);
  player->instructions = instructions;
  player->num_instructions = num_instructions;

  double score = 0;
  for (uint32_t i = 0; i < sim_time_ticks; i++)
  {
    game.do_tick();
  }
  score /= sim_time_ticks;

  // Ship yard bonus
  if (bar_game_get_num_of_unit_type(player, BAR_UnitType_armsy)||
          bar_game_get_num_of_unit_type(player, BAR_UnitType_corsy))
  {
    score += 0.2;
  }

  score += (float)bar_game_get_num_of_unit_type(player, BAR_UnitType_armroy) / 4;
  score += (float)bar_game_get_num_of_unit_type(player, BAR_UnitType_corroy) / 4;

  score += scalarTrialGeneralRules(&game, player, instructions);
  return score;
}



double ProblemBARBuildOrder::scalarTrialGeneralRules(Game<BAR_game_config> *game, Game<BAR_game_config>::Player *player, Game<BAR_game_config>::Instruction* instructions) const {

  double score = 0;

  uint32_t total_income = player->resource_production_rate_cache[BAR_ResourceType_Metal]*70 + player->resource_production_rate_cache[BAR_ResourceType_Energy];

  score += (float)((total_income*bar_game_tps)/bar_game_resource_denominator)/10000;

  // T2 Factory Incentive
  uint32_t num_t2_factories = 0;
  num_t2_factories += bar_game_get_num_of_unit_type(player, BAR_UnitType_armalab);
  num_t2_factories += bar_game_get_num_of_unit_type(player, BAR_UnitType_coralab);
  num_t2_factories += bar_game_get_num_of_unit_type(player, BAR_UnitType_armavp);
  num_t2_factories += bar_game_get_num_of_unit_type(player, BAR_UnitType_coravp);
  num_t2_factories += bar_game_get_num_of_unit_type(player, BAR_UnitType_armaap);
  num_t2_factories += bar_game_get_num_of_unit_type(player, BAR_UnitType_coraap);
  num_t2_factories += bar_game_get_num_of_unit_type(player, BAR_UnitType_armasy);
  num_t2_factories += bar_game_get_num_of_unit_type(player, BAR_UnitType_corasy);
  if (num_t2_factories > 0)
  {
    score += 0.2;
  }

  // T2 Constructor Incentive
  uint32_t num_t2_builders = 0;
  num_t2_builders += bar_game_get_num_of_unit_type(player, BAR_UnitType_armack);
  num_t2_builders += bar_game_get_num_of_unit_type(player, BAR_UnitType_corack);
  num_t2_builders += bar_game_get_num_of_unit_type(player, BAR_UnitType_armacv);
  num_t2_builders += bar_game_get_num_of_unit_type(player, BAR_UnitType_coracv);
  num_t2_builders += bar_game_get_num_of_unit_type(player, BAR_UnitType_armaca);
  num_t2_builders += bar_game_get_num_of_unit_type(player, BAR_UnitType_coraca);
  num_t2_builders += bar_game_get_num_of_unit_type(player, BAR_UnitType_armacsub);
  num_t2_builders += bar_game_get_num_of_unit_type(player, BAR_UnitType_coracsub);
  if (num_t2_builders > 0)
  {
    score += 0.3;
  }

  // Prefer build orders that end by the end
  if (player->current_instruction == num_instructions)
  {
    score += 0.001;
  }

  // Prefer short build orders
  uint32_t num_valid_instructions = 0;
  uint32_t num_total_iterations = 0;
  for (uint32_t i = 0; i < num_instructions; i++)
  {
    if (instructions[i].type)
    {
      num_valid_instructions++;
      num_total_iterations += instructions[i].iterations;
    }
  }
  score -= ((float)num_valid_instructions)/5000;
  score -= ((float)num_total_iterations)/10000;

  score -= (float)player->invalid_command_count/100;
  return score;
}


void ProblemBARBuildOrder::prettyPrintData(uint8_t *data) {
  uint32_t time_mins = (sim_time_ticks) / (60 * BAR_game_config::game_tps);
  uint32_t time_secs = (sim_time_ticks - (time_mins * 60 * BAR_game_config::game_tps)) / BAR_game_config::game_tps;
  printf("(%d:%02d %s)\n", time_mins, time_secs, bar_game_map_name);
  auto instructions = (Game<BAR_game_config>::Instruction*)data;
  Game<BAR_game_config> game;
  auto player = game.add_player(faction);
  player->instructions = instructions;
  player->num_instructions = num_instructions;
  player->play_by_play = true;
  for (uint32_t i = 0; i < sim_time_ticks; i++)
  {
    game.do_tick();
  }
  game.print_summary(false);
}

void ProblemBARBuildOrder::dataInit(uint8_t *data) {
  auto instructions = (Game<BAR_game_config>::Instruction*)data;
  for (uint32_t i = 0; i < num_instructions; i++)
  {
    instructions[i].type = 0;
    instructions[i].data = 0;
  }
}

void ProblemBARBuildOrder::scrambler(U8 *data) {

  auto instructions = (Game<BAR_game_config>::Instruction*)data;
  // Get total instruction count
  uint32_t populated_instruction_count = 0;
  for (uint32_t i = 0; i < num_instructions; i++)
  {
    if (instructions[i].type)
    {
      populated_instruction_count++;
    }
  }

  // Find random instruction to act on
  uint32_t random_instruction_idx = 0;
  if (populated_instruction_count)
  {
    uint32_t instruction_to_modify = fast_rand() % populated_instruction_count;
    for (random_instruction_idx = 0; random_instruction_idx < num_instructions; random_instruction_idx++)
    {
      if (instructions[random_instruction_idx].type)
      {
        if (instruction_to_modify == 0)
        {
          break;
        }
        instruction_to_modify--;
      }
    }
  }


  Game<BAR_game_config> game;
  auto player = game.add_player(faction);
  // Run simplified up to this point to see what we can build/reclaim
  for (uint32_t i = 0; i <= random_instruction_idx; i++)
  {
    if (instructions[i].type == BAR_Instruction_Build)
    {
      BAR_game_config::free_unit_build(player, (BAR_UnitType)instructions[i].data);
    }
  }

  // Figure out what unit types could be built at this point
  BAR_game_config::update_builder_for_unit_type_cache(player);
  uint32_t unit_build_num_options = 0;
  for (auto & builder_type : player->data.builder_for_unit_type_cache)
  {
    if (builder_type)
    {
      unit_build_num_options++;
    }
  }

  uint32_t random_unit_type_for_build = 0;
  if (unit_build_num_options)
  {
    uint32_t i = fast_rand() % unit_build_num_options;
    for (random_unit_type_for_build = 0; random_unit_type_for_build < BAR_UnitType_MAX; random_unit_type_for_build++)
    {
      if (player->data.builder_for_unit_type_cache[random_unit_type_for_build])
      {
        if (i == 0)
        {
          break;
        }
        i--;
      }
    }
  }

  // Figure out what unit types could be reclaimed at this point
  uint32_t unit_reclaim_num_options = 0;
  for (auto & i : player->furthest_allocated_unit)
  {
    if (i)
    {
      unit_reclaim_num_options++;
    }
  }

  uint32_t random_unit_type_for_reclaim = 0;
  if (unit_reclaim_num_options)
  {
    uint32_t i = fast_rand() % unit_reclaim_num_options;
    for (random_unit_type_for_reclaim = 0; random_unit_type_for_reclaim < BAR_UnitType_MAX; random_unit_type_for_reclaim++)
    {
      if (player->furthest_allocated_unit[random_unit_type_for_reclaim])
      {
        if (i == 0)
        {
          break;
        }
        i--;
      }
    }
  }


  // Choose operation
  uint32_t op_chooser = fast_rand()%8;
  if (op_chooser < 2 && (populated_instruction_count > 0))
  {
    // Delete random instruction
    if (random_instruction_idx < num_instructions)
    {
      instructions[random_instruction_idx].type = 0;
      instructions[random_instruction_idx].data = 0;
      instructions[random_instruction_idx].iterations = 0;
    }
  }
  else if (op_chooser < 3 && (populated_instruction_count > 2))
  {
    // Swap instruction with successor
    uint32_t j;
    for (j = random_instruction_idx; j < num_instructions; j++)
    {
      if (instructions[j].type)
      {
        break;
      }
    }
    if (random_instruction_idx < num_instructions && j < num_instructions)
    {
      Game<BAR_game_config>::Instruction tmp = instructions[random_instruction_idx];
      instructions[random_instruction_idx] = instructions[j];
      instructions[j] = tmp;
    }
  }
  else if (op_chooser < 4 && (populated_instruction_count > 2) && (instructions[random_instruction_idx].iterations > 1))
  {
    // Take one of this instruction and insert it up or down by 2

    // Try to find a predecessor
    uint32_t predecessor_id = num_instructions;
    for (uint32_t j = 0; j < random_instruction_idx; j++)
    {
      if (instructions[j].type)
      {
        predecessor_id = j;
      }
    }

    uint32_t location_to_insert = num_instructions;
    if ((fast_rand()%2 == 0) && (predecessor_id < num_instructions))
    {
      location_to_insert = predecessor_id;
    }
    else
    {
      location_to_insert = random_instruction_idx + 2;
    }

    if (location_to_insert < num_instructions)
    {
      Game<BAR_game_config>::Instruction to_insert = instructions[random_instruction_idx];
      to_insert.iterations = 1;
      instructions[random_instruction_idx].iterations--;

      // Insert and shift down
      for (uint32_t j = location_to_insert; j < num_instructions; j++)
      {
        if (to_insert.type == 0)
        {
          break;
        }
        Game<BAR_game_config>::Instruction tmp = instructions[j];
        instructions[j] = to_insert;
        to_insert = tmp;
      }
    }
  }
  else if (op_chooser < 5 &&  (populated_instruction_count > 0) && (random_instruction_idx+2 < num_instructions) && (instructions[random_instruction_idx].iterations > 1))
  {
    // Split an instruction apart with a new one inside

    Game<BAR_game_config>::Instruction to_insert;
    if (fast_rand()%25 == 0)
    {
      // Set reclaim unit instruction
      to_insert.type = BAR_Instruction_Reclaim;
      to_insert.data = random_unit_type_for_reclaim;
      to_insert.iterations = 1;
    }
    else
    {
      // Set build instruction
      to_insert.type = BAR_Instruction_Build;
      to_insert.data = random_unit_type_for_build;
      to_insert.iterations = 1;
    }

    // Insert new instruction at random_instruction_idx+1
    for (uint32_t j = random_instruction_idx+1; j < num_instructions; j++)
    {
      if (to_insert.type == 0)
      {
        break;
      }
      Game<BAR_game_config>::Instruction tmp = instructions[j];
      instructions[j] = to_insert;
      to_insert = tmp;
    }

    // Split existing instruction into two instructions
    to_insert = instructions[random_instruction_idx];
    instructions[random_instruction_idx].iterations = fast_rand()%instructions[random_instruction_idx].iterations;
    to_insert.iterations -= instructions[random_instruction_idx].iterations;

    // Insert new instruction at random_instruction_idx+2
    for (uint32_t j = random_instruction_idx+2; j < num_instructions; j++)
    {
      if (to_insert.type == 0)
      {
        break;
      }
      Game<BAR_game_config>::Instruction tmp = instructions[j];
      instructions[j] = to_insert;
      to_insert = tmp;
    }
  }
  else if (op_chooser < 6 && (populated_instruction_count > 0))
  {
    // Modify
    if (random_instruction_idx < num_instructions)
    {
      if ((fast_rand()%2 == 0) || (instructions[random_instruction_idx].iterations <= 1))
      {
        // Increment quantity
        instructions[random_instruction_idx].iterations++;
      }
      else
      {
        // Decrement quantity
        instructions[random_instruction_idx].iterations--;
      }
    }
  }
  else {
    // Insert new instruction

    // Make room
    Game<BAR_game_config>::Instruction to_insert;
    if (fast_rand()%25 == 0)
    {
      // Set reclaim unit instruction
      to_insert.type = BAR_Instruction_Reclaim;
      to_insert.data = random_unit_type_for_reclaim;
      to_insert.iterations = 1;
    }
    else
    {
      // Set build instruction
      to_insert.type = BAR_Instruction_Build;
      to_insert.data = random_unit_type_for_build;
      to_insert.iterations = 1;
    }

    // Sometimes insert after
    if ((random_instruction_idx+1 < num_instructions) && (fast_rand()%2))
    {
      random_instruction_idx += 1;
    }

    // Insert and shift down
    for (uint32_t j = random_instruction_idx; j < num_instructions; j++)
    {
      if (to_insert.type == 0)
      {
        break;
      }
      Game<BAR_game_config>::Instruction tmp = instructions[j];
      instructions[j] = to_insert;
      to_insert = tmp;
    }
  }

  // Collapse and deduplicate
  int32_t last_output_idx = -1;
  for (uint32_t j = 0; j < num_instructions; j++)
  {
    if (instructions[j].type)
    {
      if (last_output_idx < 0)
      {
        last_output_idx = 0;
        instructions[last_output_idx] = instructions[j];
      }
      else if (
              (instructions[j].type == instructions[last_output_idx].type) &&
              (instructions[j].data == instructions[last_output_idx].data) )
      {
        instructions[last_output_idx].iterations += instructions[j].iterations;
      }
      else
      {
        last_output_idx++;
        instructions[last_output_idx] = instructions[j];
      }
    }
  }
  last_output_idx++;
  for (; last_output_idx < num_instructions; last_output_idx++)
  {
    instructions[last_output_idx].type = 0;
    instructions[last_output_idx].data = 0;
    instructions[last_output_idx].iterations = 0;
  }
}

double ProblemBARBuildOrder::scalarTrialGunshipRush(uint8_t *data) const {
  auto instructions = (Game<BAR_game_config>::Instruction*)data;
  Game<BAR_game_config> game;
  auto player = game.add_player(faction);
  player->instructions = instructions;
  player->num_instructions = num_instructions;

  double score = 0;
  for (uint32_t i = 0; i < sim_time_ticks; i++)
  {
    game.do_tick();
  }
  score /= sim_time_ticks;

  score += (float)bar_game_get_num_of_unit_type(player, BAR_UnitType_armkam) / 5;
  score += (float)bar_game_get_num_of_unit_type(player, BAR_UnitType_corbw) / 5;

  score += scalarTrialGeneralRules(&game, player, instructions);
  return score;
}

double ProblemBARBuildOrder::scalarTrialPawnRush(uint8_t *data) const {
  auto instructions = (Game<BAR_game_config>::Instruction*)data;
  Game<BAR_game_config> game;
  auto player = game.add_player(faction);
  player->instructions = instructions;
  player->num_instructions = num_instructions;

  double score = 0;
  for (uint32_t i = 0; i < sim_time_ticks; i++)
  {
    game.do_tick();
  }
  score /= sim_time_ticks;

  score += (float)bar_game_get_num_of_unit_type(player, BAR_UnitType_armpw) / 5;
  score += (float)bar_game_get_num_of_unit_type(player, BAR_UnitType_corak) / 5;

  score += scalarTrialGeneralRules(&game, player, instructions);
  return score;
}

using namespace cv;
void ProblemBARBuildOrder::presentSolution(uint8_t *data) {
  auto instructions = (Game<BAR_game_config>::Instruction*)data;
  Game<BAR_game_config> game;
  auto player = game.add_player(faction);
  player->instructions = instructions;
  player->num_instructions = num_instructions;

  Mat img = imread("../src/problems/rts_buildorders/BAR_game/map_texture.png");
  Mat img_resized;
  uint32_t scaling_factor = 4;
  resize(img, img_resized, Size(img.cols/scaling_factor, img.rows/scaling_factor), INTER_LINEAR);
  namedWindow("mlx86", WINDOW_NORMAL);
  resizeWindow("mlx86", 1024, 1024);
  imshow("mlx86", img_resized);
  for (uint32_t i = 0; i < sim_time_ticks; i++)
  {
    Mat img_frame = img_resized.clone();
    game.do_tick();
    for (uint32_t unit_type = 0; unit_type < BAR_game_config::num_unit_types; unit_type++)
    {
      for (uint32_t j = 0; j < player->furthest_allocated_unit[unit_type]; j++)
      {
        uint32_t x_pos = player->units_by_type[unit_type][j].position_x/scaling_factor;
        uint32_t y_pos = img_resized.rows - player->units_by_type[unit_type][j].position_z/scaling_factor;
        uint32_t half_width = bar_game_unit_type_metadata_table.values[unit_type].footprint_x/2;
        uint32_t half_height = bar_game_unit_type_metadata_table.values[unit_type].footprint_z/2;
        rectangle(img_frame,
               Point(x_pos-half_width, y_pos-half_height),
               Point(x_pos+half_width, y_pos+half_height),
               Scalar(bar_game_unit_type_metadata_table.values[unit_type].color_r,
                      bar_game_unit_type_metadata_table.values[unit_type].color_g,
                      bar_game_unit_type_metadata_table.values[unit_type].color_b),
               2);
      }
    }
    imshow("mlx86", img_frame);
    waitKey(5);
  }
  waitKey(0);
}




