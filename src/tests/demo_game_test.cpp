//
// Created by christian on 11/10/23.
//

#include "problems/rts_buildorders/game.hpp"
#include "problems/rts_buildorders/bar_game_config.hpp"
#include "unistd.h"

int main(int argc, char **argv) {
  Game<BAR_game_config> game;
  BAR_game_config::free_unit_build(&game.players[0], BAR_UnitType_ArmadaMetalStorage);
  BAR_game_config::free_unit_build(&game.players[0], BAR_UnitType_ArmadaEnergyStorage);
  game.players[0].append_instruction(BAR_Instruction_Build, BAR_UnitType_ArmadaMetalExtractor);
  game.players[0].append_instruction(BAR_Instruction_Reclaim, BAR_UnitType_ArmadaMetalExtractor);
  game.players[0].print_instructions("");
  for (uint32_t i = 0; i < 2*60; i++)
  {
    for (uint32_t j = 0; j < 20; j++)
    {
      game.do_tick();
    }
    game.print_summary(true);
    sleep(1);
  }
}