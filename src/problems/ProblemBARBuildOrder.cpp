//
// Created by christian on 11/11/23.
//

#include "ProblemBARBuildOrder.hpp"
#include "christian_utils.h"


double ProblemBARBuildOrder::scalarTrial(uint8_t *data) {
  return scalarTrialNukeRush(data);
}

double ProblemBARBuildOrder::scalarTrialTickSpam(uint8_t *data) const {
  auto instructions = (Game<BAR_game_config>::Instruction*)data;
  Game<BAR_game_config> game;
  bar_game_setup(&game);
  game.players[0].instructions = instructions;
  game.players[0].num_instructions = num_instructions;

  double score = 0;
  for (uint32_t i = 0; i < sim_time_ticks; i++)
  {
    game.do_tick();
  }
  score /= sim_time_ticks;

  score += (float)bar_game_get_num_of_unit_type(&game.players[0], BAR_UnitType_ArmadaTick) / 100;

  uint32_t num_valid_instructions = 0;
  for (uint32_t i = 0; i < num_instructions; i++)
  {
    if (instructions[i].type)
    {
      num_valid_instructions++;
    }
  }
  score -= ((float)num_valid_instructions)/100000;
  return score;
}


double ProblemBARBuildOrder::scalarTrialBomberRush(uint8_t *data) const {
  auto instructions = (Game<BAR_game_config>::Instruction*)data;
  Game<BAR_game_config> game;
  bar_game_setup(&game);
  game.players[0].instructions = instructions;
  game.players[0].num_instructions = num_instructions;

  double score = 0;
  for (uint32_t i = 0; i < sim_time_ticks; i++)
  {
    game.do_tick();

    if (bar_game_get_num_of_unit_type(&game.players[0], BAR_UnitType_ArmadaLiche))
    {
      score += 5;
    }
    if (bar_game_get_num_of_unit_type(&game.players[0], BAR_UnitType_ArmadaLiche) >= 2)
    {
      score += 20;
    }
  }
  score /= sim_time_ticks;

  if (bar_game_get_num_of_unit_type(&game.players[0], BAR_UnitType_ArmadaAdvancedAircraftPlant))
  {
    score += 0.1;
  }

  uint32_t num_valid_instructions = 0;
  for (uint32_t i = 0; i < num_instructions; i++)
  {
    if (instructions[i].type)
    {
      num_valid_instructions++;
    }
  }
  score -= ((float)num_valid_instructions)/100000;
  return score;
}

double ProblemBARBuildOrder::scalarTrialNukeRush(uint8_t *data) const {
  auto instructions = (Game<BAR_game_config>::Instruction*)data;
  Game<BAR_game_config> game;
  bar_game_setup(&game);
  game.players[0].instructions = instructions;
  game.players[0].num_instructions = num_instructions;

  double score = 0;
  for (uint32_t i = 0; i < sim_time_ticks; i++)
  {
    game.do_tick();
    if (bar_game_get_num_of_unit_type(&game.players[0], BAR_UnitType_ArmadaConstructionBot))
    {
      score += 0.1;
    }
    if (bar_game_get_num_of_unit_type(&game.players[0], BAR_UnitType_ArmadaAdvancedBotLab))
    {
      score += 0.1;
    }

    if (bar_game_get_num_of_unit_type(&game.players[0], BAR_UnitType_ArmadaAdvancedConstructionBot))
    {
      score += 0.1;
    }
    uint32_t nuke_energy_count = (game.players[0].cached_resource_rates[BAR_ResourceType_Energy] - bar_game_get_full_upkeep(&game.players[0], BAR_ResourceType_Energy)) / ((1111 * bar_game_resource_denominator) / bar_game_tps);
    uint32_t armageddon_count = bar_game_get_num_of_unit_type(&game.players[0], BAR_UnitType_ArmadaArmageddon);
    if (nuke_energy_count < armageddon_count)
    {
      armageddon_count = nuke_energy_count;
    }
    score += armageddon_count * 10;
  }
  score /= sim_time_ticks;

  score += (float)bar_game_get_num_of_unit_type(&game.players[0], BAR_UnitType_ArmadaArmageddon)/3;
  score += game.players[0].cached_resource_rates[BAR_ResourceType_Metal] / 100000;
  score += game.players[0].cached_resource_rates[BAR_ResourceType_Energy] / 1000000;

  uint32_t num_valid_instructions = 0;
  for (uint32_t i = 0; i < num_instructions; i++)
  {
    if (instructions[i].type)
    {
      num_valid_instructions++;
    }
  }
  score -= ((float)num_valid_instructions)/20000;
  return score;
}


double ProblemBARBuildOrder::scalarTrialEconomyRush(uint8_t *data) const {
  auto instructions = (Game<BAR_game_config>::Instruction*)data;
  Game<BAR_game_config> game;
  bar_game_setup(&game);
  game.players[0].instructions = instructions;
  game.players[0].num_instructions = num_instructions;

  double score = 0;
  for (uint32_t i = 0; i < sim_time_ticks; i++)
  {
    game.do_tick();
    score += (float)game.players[0].cached_resource_rates[BAR_ResourceType_Metal] / 1000000;
    score += (float)game.players[0].cached_resource_rates[BAR_ResourceType_Energy] / 10000000;
  }
  score /= sim_time_ticks;

  score += (float)game.players[0].cached_resource_rates[BAR_ResourceType_Metal] / 1000000;
  score += (float)game.players[0].cached_resource_rates[BAR_ResourceType_Energy] / 10000000;


  uint32_t num_valid_instructions = 0;
  for (uint32_t i = 0; i < num_instructions; i++)
  {
    if (instructions[i].type)
    {
      num_valid_instructions++;
    }
  }
  score -= ((float)num_valid_instructions)/20000;
  return score;
}

void ProblemBARBuildOrder::prettyPrintData(uint8_t *data) {
  auto instructions = (Game<BAR_game_config>::Instruction*)data;
  Game<BAR_game_config> game;
  bar_game_setup(&game);
  game.players[0].instructions = instructions;
  game.players[0].num_instructions = num_instructions;
  game.players[0].print_instructions("");
  for (uint32_t i = 0; i < sim_time_ticks; i++)
  {
    game.do_tick();
  }
  game.print_summary();
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
  // Find instruction to erase
  if (fast_rand()%2 == 0)
  {
    uint32_t i = fast_rand()%(num_instructions);
    i += 1;
    for (; i < num_instructions; i++)
    {
      auto instruction = &instructions[i];
      if (instruction->type)
      {
        instruction->type = 0;
        instruction->data = 0;
        break;
      }
    }

  }
  // Set build instruction
  else
  {
    auto instruction = &instructions[fast_rand()%(num_instructions)];
    instruction->type = BAR_Instruction_Build;
    instruction->data = (fast_rand()%(BAR_UnitType_MAX));
  }
}

