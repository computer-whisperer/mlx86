//
// Created by christian on 11/12/23.
//
#include "bar_game_config.hpp"

uint32_t bar_game_get_num_of_unit_type(Game<BAR_game_config>::Player *player, BAR_UnitType unit_type)
{
  uint32_t count = 0;
  for (uint32_t i = 0; i < player->furthest_allocated_unit[unit_type]; i++)
  {
    if (player->units[unit_type][i].is_allocated && player->units[unit_type][i].data.is_alive)
    {
      count++;
    }
  }
  return count;
}

template<uint32_t unit_type=BAR_UnitType_MAX-1> static inline uint32_t upkeep_iterator(Game<BAR_game_config>::Player *player, BAR_ResourceTypes resource_type)
{
  uint32_t total_upkeep = 0;
  for (uint32_t j = 0; j < player->furthest_allocated_unit[unit_type]; j++)
  {
    if (player->units[unit_type][j].is_allocated && player->units[unit_type][j].data.is_alive)
    {
      if (resource_type == BAR_ResourceType_Energy)
      {
        total_upkeep += bar_game_get_unit_type_metadata(unit_type).energy_upkeep;
      }
      if (resource_type == BAR_ResourceType_Metal)
      {
        total_upkeep += bar_game_get_unit_type_metadata(unit_type).metal_upkeep;
      }
    }
  }
  if constexpr (unit_type == 0)
  {
    return total_upkeep;
  }
  else
  {
    return total_upkeep + upkeep_iterator<unit_type-1>(player, resource_type);
  }
}

uint32_t bar_game_get_full_upkeep(Game<BAR_game_config>::Player *player, BAR_ResourceTypes resource_type)
{
  return upkeep_iterator(player, resource_type);
}
