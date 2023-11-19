//
// Created by christian on 11/18/23.
//

#include "BAR_game/bar_game_config.hpp"
#include "christian_utils.h"

int main(int argc, char **argv) {
  double start_unix_time = getUnixTime();
  for (uint32_t i = 0; i < 1000; i++)
  {
    Game<BAR_game_config> game;
    auto player = game.add_player();
    BAR_game_config::free_unit_build(player, BAR_UnitType_armestor);
    BAR_game_config::free_unit_build(player, BAR_UnitType_armmstor);
    BAR_game_config::free_unit_build(player, BAR_UnitType_armadvsol);
    player->append_instruction(BAR_Instruction_Build, BAR_UnitType_armmex, 5);
    player->append_instruction(BAR_Instruction_Build, BAR_UnitType_armwin, 3);
    player->append_instruction(BAR_Instruction_Build, BAR_UnitType_armmakr, 2);
    player->append_instruction(BAR_Instruction_Build, BAR_UnitType_armlab);
    player->append_instruction(BAR_Instruction_Build, BAR_UnitType_armck, 10);
    player->append_instruction(BAR_Instruction_Build, BAR_UnitType_armadvsol, 15);
    player->append_instruction(BAR_Instruction_Build, BAR_UnitType_armalab);
    player->append_instruction(BAR_Instruction_Build, BAR_UnitType_armack);
    player->append_instruction(BAR_Instruction_Build, BAR_UnitType_armck, 10);
    player->append_instruction(BAR_Instruction_Build, BAR_UnitType_armamex, 5);
    player->append_instruction(BAR_Instruction_Build, BAR_UnitType_armfus, 10);
    for (uint32_t j = 0; j < 30*60*20; j++)
    {
      game.do_tick();
    }
  }
  double end_unix_time = getUnixTime();
  //game.print_summary(false);
  printf("Time: %f\n", end_unix_time - start_unix_time);
}