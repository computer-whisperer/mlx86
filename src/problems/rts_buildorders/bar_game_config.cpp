//
// Created by christian on 11/12/23.
//
#include "bar_game_config.hpp"

uint32_t bar_game_get_num_of_unit_type(Game<BAR_game_config>::Player *player, BAR_UnitType unit_type)
{
  uint32_t count = 0;
  for (uint32_t i = 0; i < player->furthest_allocated_unit[unit_type]; i++)
  {
    auto unit_data = (BARUnitData*)player->units[i]->data;
    if (player->units[i]->is_allocated && unit_data->is_alive)
    {
      count++;
    }
  }
  return count;
}

uint32_t bar_game_get_full_upkeep(Game<BAR_game_config>::Player *player, BAR_ResourceTypes resource_type)
{
  uint32_t total_upkeep = 0;

  for (uint32_t i = 0; i < BAR_UnitType_MAX; i++)
  {
    for (uint32_t j = 0; j < player->furthest_allocated_unit[i]; j++)
    {
      auto unit_data = (BARUnitData*)player->units[j]->data;
      if (player->units[j]->is_allocated && unit_data->is_alive)
      {
        if (resource_type == BAR_ResourceType_Energy)
        {
          total_upkeep += BAR_UnitType_Metadata[i].energy_upkeep;
        }
        if (resource_type == BAR_ResourceType_Metal)
        {
          total_upkeep += BAR_UnitType_Metadata[i].metal_upkeep;
        }
      }
    }
  }
  return total_upkeep;
}
