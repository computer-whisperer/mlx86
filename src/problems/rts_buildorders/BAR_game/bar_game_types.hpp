//
// Created by christian on 11/14/23.
//

#ifndef MLX86_BAR_GAME_TYPES_HPP
#define MLX86_BAR_GAME_TYPES_HPP
#include <cstdint>

constexpr uint32_t bar_game_tps = 20;
constexpr uint32_t bar_game_resource_denominator = 1000;

constexpr uint32_t bar_game_max_num_build_options = 44;
constexpr uint32_t bar_game_max_num_units_of_type = 256;

struct BARMetalMineMetadata {
  int32_t x = 0;
  int32_t z = 0;
  uint32_t value = 0;
};

struct BARCoordinate {
  int32_t x = 0;
  int32_t z = 0;
};

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
  uint32_t energy_upkeep = 0;
  uint32_t metal_upkeep = 0;
  BAR_UnitType build_options[bar_game_max_num_build_options];
  uint32_t max_count = bar_game_max_num_units_of_type;
  uint32_t avg_travel_time = 0;
  uint32_t energy_convert_capacity = 0;
  uint32_t energy_convert_return = 0;
  uint8_t geothermal = 0;
  uint8_t metal_extractor = 0;
  uint8_t wind_generator = 0;
  uint8_t tidal_generator = 0;
  uint8_t is_builder = 0;
  uint8_t can_assist = 0;
  uint32_t build_distance = 0;
  float max_velocity = 0.0;
  uint32_t min_water_depth = 0;
  uint32_t max_water_depth = 1000;
  uint32_t build_shuffle_delay = 0;
  bool show_in_inline_summary = false;
  const char* summary_name = "";
  float footprint_x = 0;
  float footprint_z = 0;
  uint8_t color_r = 0;
  uint8_t color_g = 255;
  uint8_t color_b = 0;
};

#endif //MLX86_BAR_GAME_TYPES_HPP
