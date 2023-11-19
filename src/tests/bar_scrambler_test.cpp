//
// Created by christian on 11/18/23.
//

#include "problems/rts_buildorders/game.hpp"
#include "problems/rts_buildorders/BAR_game/bar_game_config.hpp"
#include "unistd.h"
#include "ProblemBARBuildOrder.hpp"

int main(int argc, char **argv) {

  ProblemBARBuildOrder problem;
  uint8_t data[problem.data_len];
  problem.dataInit(data);
  for (uint32_t i = 0;  i < 80; i++)
  {
    problem.prettyPrintData(data);
    problem.scrambler(data);
  }
  return 0;
}