//
// Created by christian on 11/10/23.
//

#include "problems/rts_buildorders/game.hpp"
#include "problems/rts_buildorders/bar_game_config.hpp"
#include "unistd.h"

int main(int argc, char **argv) {
  Game<BAR_game_config> game;
  bar_game_setup(&game);
  game.players[0].append_instruction(BAR_Instruction_Build, BAR_UnitType_ArmadaMetalExtractor);
  game.players[0].append_instruction(BAR_Instruction_Build, BAR_UnitType_ArmadaWindTurbine);
  game.players[0].append_instruction(BAR_Instruction_Build, BAR_UnitType_ArmadaWindTurbine);
  game.players[0].append_instruction(BAR_Instruction_Build, BAR_UnitType_ArmadaWindTurbine);
  game.players[0].append_instruction(BAR_Instruction_Build, BAR_UnitType_ArmadaAdvancedGeothermalPowerplant);
  game.players[0].append_instruction(BAR_Instruction_Build, BAR_UnitType_ArmadaEnergyConverter);
  game.players[0].print_instructions("");
  for (uint32_t i = 0; i < 2*60; i++)
  {
    for (uint32_t j = 0; j < 20; j++)
    {
      game.do_tick();
    }
    game.print_summary();
    sleep(1);
  }
}