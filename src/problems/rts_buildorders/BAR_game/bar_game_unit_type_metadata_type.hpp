//
// Created by christian on 11/14/23.
//

#ifndef MLX86_BAR_GAME_UNIT_TYPE_METADATA_TYPE_HPP
#define MLX86_BAR_GAME_UNIT_TYPE_METADATA_TYPE_HPP
#include <cstdint>

constexpr uint32_t bar_game_tps = 20;
constexpr uint32_t bar_game_resource_denominator = 1000;

constexpr uint32_t bar_game_max_num_build_options = 32;
constexpr uint32_t bar_game_max_num_units_of_type = 256;


struct BARUnitTypeMetadata_T {
  const char* name = "Invalid";
  uint32_t metal_cost = 0;
  uint32_t energy_cost = 0;
  uint32_t build_cost  = 0;
  uint32_t build_power = 0;
  uint32_t direct_build_power = 0;
  uint32_t metal_production = 0;
  uint32_t energy_production = 0;
  uint32_t energy_capacity = 0;
  uint32_t metal_capacity = 0;
  BAR_UnitType build_options[bar_game_max_num_build_options];
  uint32_t max_count = bar_game_max_num_units_of_type;
  uint32_t avg_travel_time = 0;
  uint32_t energy_upkeep = 0;
  uint32_t metal_upkeep = 0;
  uint32_t energy_convert_capacity = 0;
  uint32_t energy_convert_return = 0;
  uint8_t geothermal = 0;
  uint8_t metal_extractor = 0;
};

#endif //MLX86_BAR_GAME_UNIT_TYPE_METADATA_TYPE_HPP
