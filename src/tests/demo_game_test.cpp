//
// Created by christian on 11/10/23.
//

#include "problems/rts_buildorders/game.hpp"
#include "problems/rts_buildorders/BAR_game/bar_game_config.hpp"
#include "unistd.h"
#include "ProblemBARBuildOrder.hpp"

int main(int argc, char **argv) {

  /*
  ProblemBARBuildOrder problem;
  uint8_t data[problem.data_len];
  problem.dataInit(data);
  for (uint32_t i = 0;  i < 80; i++)
  {
    problem.prettyPrintData(data);
    problem.scrambler(data);
  }
  return 0;*/

  Game<BAR_game_config> game;
  auto player = game.add_player();
  BAR_game_config::free_unit_build(player, BAR_UnitType_armestor);
  BAR_game_config::free_unit_build(player, BAR_UnitType_armmstor);
  BAR_game_config::free_unit_build(player, BAR_UnitType_armadvsol);
  player->append_instruction(BAR_Instruction_Build, BAR_UnitType_armmex, 0);
  player->append_instruction(BAR_Instruction_Build, BAR_UnitType_armwin, 3);
  player->append_instruction(BAR_Instruction_Build, BAR_UnitType_armmakr, 2);
  /*
  player->append_instruction(BAR_Instruction_Build, BAR_UnitType_armlab);
  player->append_instruction(BAR_Instruction_Build, BAR_UnitType_armck);
  player->append_instruction(BAR_Instruction_Build, BAR_UnitType_armadvsol, 15);
  player->append_instruction(BAR_Instruction_Build, BAR_UnitType_armalab);*/
  player->print_instructions("");
  for (uint32_t i = 0; i < 4*60; i++)
  {
    for (uint32_t j = 0; j < 20; j++)
    {
      game.do_tick();
    }
    game.print_summary(false);
    sleep(1);
  }
  game.print_summary(false);
}