//
// Created by christian on 11/12/23.
//
#include "bar_game_config.hpp"

bool BAR_game_config::follow_instruction(Game<BAR_game_config>::Player* player, Game<BAR_game_config>::Instruction* instruction, bool first_tick)
{
  bool advance_instruction = false;
  switch (instruction->type) {
    default:
      // Invalid
      advance_instruction = true;
      player->invalid_command_count++;
      break;
    case BAR_Instruction_Reclaim:
    {
      auto unit_type = (BAR_UnitType) instruction->data;
      if ((unit_type >= BAR_UnitType_MAX) || (unit_type == BAR_UnitType_None)) {
        // Invalid
        advance_instruction = true;
        player->invalid_command_count++;
        break;
      }
      BARUnitTypeMetadata_T unit_metadata = bar_game_get_unit_type_metadata(unit_type);

      if (first_tick)
      {
        update_cached_total_general_build_power(player);
        player->data.currently_building_unit = nullptr;

        // Find unit to reclaim
        for (uint32_t i = 0; i < player->furthest_allocated_unit[unit_type]; i++)
        {
          if (player->units[unit_type][i].is_allocated)
          {
            if (player->units[unit_type][i].data.is_alive)
            {
              player->data.currently_building_unit = &player->units[unit_type][i];
              break;
            }
          }
        }

        if (!player->data.currently_building_unit)
        {
          player->invalid_command_count++;
          advance_instruction = true;
          break;
        }
      }

      auto unit = (Game<BAR_game_config>::Unit *) player->data.currently_building_unit;

      uint32_t build_power = player->data.cached_total_general_build_power - unit_metadata.build_power;

      if (build_power >= unit->data.allocated_build_points)
      {
        player->resources[BAR_ResourceType_Metal] += bar_game_get_unit_type_metadata(unit_type).metal_cost;
        player->deallocate_unit(unit, unit_type);
        advance_instruction = true;
      } else{
        unit->data.allocated_build_points -= build_power;
      }
      break;
    }
    case BAR_Instruction_Build:
    {
      auto unit_type = (BAR_UnitType) instruction->data;
      if ((unit_type >= BAR_UnitType_MAX) || (unit_type == BAR_UnitType_None)) {
        // Invalid
        advance_instruction = true;
        player->invalid_command_count++;
        break;
      }
      BARUnitTypeMetadata_T unit_metadata = bar_game_get_unit_type_metadata(unit_type);

      if (first_tick) {
        update_cached_total_general_build_power(player);

        if (player->data.builder_for_unit_type_cache_dirty)
        {
          update_builder_for_unit_type_cache(player);
        }

        player->data.travel_time_remaining = 0;
        player->data.currently_building_unit = nullptr;

        // Check if we have a builder that can build this
        if (player->data.builder_for_unit_type_cache[unit_type] == BAR_UnitType_None)
        {
          advance_instruction = true;
          player->invalid_command_count++;
          break;
        }

        // Check if we already built too many units (approximated here by furthest_allocated_unit)
        if (player->furthest_allocated_unit[unit_type] >= unit_metadata.max_count)
        {
          advance_instruction = true;
          player->invalid_command_count++;
          break;
        }
        // Check for an available spot
        if (unit_metadata.geothermal)
        {
          if (!player->data.geothermal_slots_total)
          {
            advance_instruction = true;
            player->invalid_command_count++;
            break;
          }
        }
        if (unit_metadata.metal_extractor)
        {
          if (!player->data.metal_slots_total)
          {
            advance_instruction = true;
            player->invalid_command_count++;
            break;
          }
        }
        // Do it
        player->data.currently_building_unit = player->allocate_unit(unit_type);
        if (player->data.currently_building_unit)
        {
          player->data.travel_time_remaining = unit_metadata.avg_travel_time;
        }
      }
      if (!player->data.currently_building_unit)
      {
        // Allocation failure
        advance_instruction = true;
        player->invalid_command_count++;
        break;
      }
      auto unit = (Game<BAR_game_config>::Unit*) player->data.currently_building_unit;
      if (player->data.travel_time_remaining) {
        player->data.travel_time_remaining--;
      } else {
        // Add resources
        uint32_t build_points_to_give = player->data.cached_total_general_build_power + bar_game_get_unit_type_metadata(player->data.builder_for_unit_type_cache[unit_type]).direct_build_power;
        if (unit_metadata.energy_cost > 0) {
          uint32_t build_points_allowed_by_energy =
                  ((uint64_t) player->resources[BAR_ResourceType_Energy] * (uint64_t) unit_metadata.build_cost) / (uint64_t) unit_metadata.energy_cost;
          if (build_points_to_give > build_points_allowed_by_energy) {
            build_points_to_give = build_points_allowed_by_energy;
          }
        }
        if (unit_metadata.metal_cost > 0) {
          uint32_t build_points_allowed_by_metal =
                  ((uint64_t) player->resources[BAR_ResourceType_Metal] * (uint64_t) unit_metadata.build_cost) / (uint64_t) unit_metadata.metal_cost;
          if (build_points_to_give > build_points_allowed_by_metal) {
            build_points_to_give = build_points_allowed_by_metal;
          }
        }

        unit->data.allocated_build_points += build_points_to_give;
        if (unit_metadata.build_cost > 0) {
          uint32_t energy_to_give = (unit_metadata.energy_cost * build_points_to_give) / unit_metadata.build_cost;
          player->resources[BAR_ResourceType_Energy] -= energy_to_give;
          unit->data.allocated_resources[BAR_ResourceType_Energy] += energy_to_give;
          uint32_t metal_to_give = (unit_metadata.metal_cost * build_points_to_give) / unit_metadata.build_cost;
          player->resources[BAR_ResourceType_Metal] -= metal_to_give;
          unit->data.allocated_resources[BAR_ResourceType_Metal] += metal_to_give;
        }
      }

      // Figure out if we can build this tick
      bool can_finish = true;
      if (unit->data.allocated_build_points < unit_metadata.build_cost)
        can_finish = false;
      if (unit->data.allocated_resources[BAR_ResourceType_Energy] < unit_metadata.energy_cost)
        can_finish = false;
      if (unit->data.allocated_resources[BAR_ResourceType_Metal] < unit_metadata.metal_cost)
        can_finish = false;
      if (can_finish) {
        // Refund extra resources
        player->resources[BAR_ResourceType_Metal] += unit->data.allocated_resources[BAR_ResourceType_Metal] - unit_metadata.metal_cost;
        player->resources[BAR_ResourceType_Energy] += unit->data.allocated_resources[BAR_ResourceType_Energy] - unit_metadata.energy_cost;

        on_unit_alive(player, unit, unit_type);

        advance_instruction = true;
      }
      break;
    }
  }
  return advance_instruction;
};

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
