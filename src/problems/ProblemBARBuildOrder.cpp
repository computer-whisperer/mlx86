//
// Created by christian on 11/11/23.
//

#include "ProblemBARBuildOrder.hpp"
#include "christian_utils.h"


double ProblemBARBuildOrder::scalarTrial(uint8_t *data) {
  return scalarTrialEconomyRush(data);
}

double ProblemBARBuildOrder::scalarTrialTickSpam(uint8_t *data) const {
  auto instructions = (Game<BAR_game_config>::Instruction*)data;
  Game<BAR_game_config> game;
  game.players[0].instructions = instructions;
  game.players[0].num_instructions = num_instructions;

  double score = 0;
  for (uint32_t i = 0; i < sim_time_ticks; i++)
  {
    game.do_tick();
  }
  score /= sim_time_ticks;

  score += (float)game.players[0].num_units[BAR_Unit_ArmadaTick] / 100;

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
  game.players[0].instructions = instructions;
  game.players[0].num_instructions = num_instructions;

  double score = 0;
  for (uint32_t i = 0; i < sim_time_ticks; i++)
  {
    game.do_tick();

    if (game.players[0].num_units[BAR_Unit_ArmadaLiche])
    {
      score += 5;
    }
    if (game.players[0].num_units[BAR_Unit_ArmadaLiche] >= 2)
    {
      score += 20;
    }
  }
  score /= sim_time_ticks;

  if (game.players[0].num_units[BAR_Unit_ArmadaAdvancedAircraftPlant])
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
  game.players[0].instructions = instructions;
  game.players[0].num_instructions = num_instructions;

  double score = 0;
  for (uint32_t i = 0; i < sim_time_ticks; i++)
  {
    game.do_tick();
    if (game.players[0].num_units[BAR_Unit_ArmadaConstructionBot])
    {
      score += 0.1;
    }
    if (game.players[0].num_units[BAR_Unit_ArmadaAdvancedBotLab])
    {
      score += 0.1;
    }

    if (game.players[0].num_units[BAR_Unit_ArmadaAdvancedConstructionBot])
    {
      score += 0.1;
    }
    if (game.players[0].num_units[BAR_Unit_ArmadaArmageddon])
    {
      score += 3;
      uint32_t nuke_energy_rate = (1111*bar_game_resource_denominator)/bar_game_tps;
      if ((game.players[0].total_resource_rates[BAR_ResourceType_Energy] - game.players[0].total_resource_upkeep[BAR_ResourceType_Energy]) < nuke_energy_rate)
      {
        nuke_energy_rate = game.players[0].total_resource_rates[BAR_ResourceType_Energy] - game.players[0].total_resource_upkeep[BAR_ResourceType_Energy];
      }
      score += 2*((float)nuke_energy_rate/((1111*bar_game_resource_denominator)/bar_game_tps));
    }
  }
  score /= sim_time_ticks;

  score += game.players[0].total_resource_rates[BAR_ResourceType_Metal] / 100000;
  score += game.players[0].total_resource_rates[BAR_ResourceType_Energy] / 1000000;
  score += game.players[0].total_build_power / 100000;

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
  game.players[0].instructions = instructions;
  game.players[0].num_instructions = num_instructions;

  double score = 0;
  for (uint32_t i = 0; i < sim_time_ticks; i++)
  {
    game.do_tick();
    score += (float)game.players[0].total_resource_rates[BAR_ResourceType_Metal] / 1000000;
    score += (float)game.players[0].total_resource_rates[BAR_ResourceType_Energy] / 10000000;
  }
  score /= sim_time_ticks;

  score += (float)game.players[0].total_resource_rates[BAR_ResourceType_Metal] / 1000000;
  score += (float)game.players[0].total_resource_rates[BAR_ResourceType_Energy] / 10000000;


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
  instructions[0].type = BAR_Instruction_Build;
  instructions[0].data = BAR_Unit_ArmadaCommander;
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
    // Don't overwrite commander
    if (instruction == &instructions[0])
    {
      instruction = &instructions[1];
    }
    instruction->type = BAR_Instruction_Build;
    instruction->data = (fast_rand()%(BAR_Unit_MAX));
    if (instruction->data == BAR_Unit_None)
    {
      instruction->data = BAR_Unit_ArmadaCommander;
    }
  }
}

