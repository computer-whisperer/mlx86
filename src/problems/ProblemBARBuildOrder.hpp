//
// Created by christian on 11/11/23.
//

#ifndef MLX86_PROBLEM_BAR_BUILD_ORDER_HPP
#define MLX86_PROBLEM_BAR_BUILD_ORDER_HPP

#include <cstdint>
#include "problem.h"
#include "rts_buildorders/bar_game_config.hpp"
#include "rts_buildorders/game.hpp"

class ProblemBARBuildOrder: public Problem {
public:
  uint32_t num_instructions;

  uint32_t sim_time_ticks = bar_game_tps*60*30;

  explicit ProblemBARBuildOrder(uint32_t n_instructions=100): num_instructions(n_instructions), Problem(sizeof(Game<BAR_game_config>::Instruction) * n_instructions){};

  ProblemBARBuildOrder(const ProblemBARBuildOrder& problem) = default;

  ProblemBARBuildOrder * Clone() override
  {
    return new ProblemBARBuildOrder(*this);
  }

  double scalarTrial(uint8_t *data) override;
  double scalarTrialNukeRush(uint8_t *data) const;
  double scalarTrialTickSpam(uint8_t *data) const;
  double scalarTrialEconomyRush(uint8_t *data) const;
  double scalarTrialBomberRush(uint8_t *data) const;

  void prettyPrintData(uint8_t *data) override;

  void dataInit(uint8_t *data) override;

  void scrambler(U8* data) override;
};

#endif //MLX86_PROBLEM_BAR_BUILD_ORDER_HPP
