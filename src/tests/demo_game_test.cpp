//
// Created by christian on 11/10/23.
//

#include "problems/rts_buildorders/game.hpp"
#include "problems/rts_buildorders/bar_game_config.hpp"
#include "unistd.h"

int main(int argc, char **argv) {
  Game<BAR_game_config> game;
  game.players[0].append_instruction(BAR_Instruction_Build, BAR_Unit_ArmadaCommander);
  game.players[0].append_instruction(BAR_Instruction_Build, BAR_Unit_ArmadaMetalExtractor);
  game.players[0].append_instruction(BAR_Instruction_Build, BAR_Unit_ArmadaWindTurbine);
  game.players[0].append_instruction(BAR_Instruction_Build, BAR_Unit_ArmadaMetalExtractor);
  game.players[0].append_instruction(BAR_Instruction_Build, BAR_Unit_ArmadaMetalExtractor);
  game.players[0].append_instruction(BAR_Instruction_Build, BAR_Unit_ArmadaWindTurbine);
  game.players[0].append_instruction(BAR_Instruction_Build, BAR_Unit_ArmadaWindTurbine);
  game.players[0].append_instruction(BAR_Instruction_Build, BAR_Unit_ArmadaWindTurbine);
  game.players[0].append_instruction(BAR_Instruction_Build, BAR_Unit_ArmadaBotLab);
  game.players[0].append_instruction(BAR_Instruction_Build, BAR_Unit_ArmadaConstructionBot);
  game.players[0].print_instructions("");
  for (uint32_t i = 0; i < 120; i++)
  {
    for (uint32_t j = 0; j < 20; j++)
    {
      game.do_tick();
    }
    game.print_summary();
    sleep(1);
  }
}