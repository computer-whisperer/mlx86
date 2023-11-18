//
// Created by christian on 11/12/23.
//
#include "bar_game_config.hpp"

BAR_UnitMetadata_Table_T bar_game_unit_type_metadata_table;

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

      if (first_tick)
      {
        player->data.currently_building_unit = nullptr;

        // Find unit to reclaim
        for (uint32_t i = 0; i < player->furthest_allocated_unit[unit_type]; i++)
        {
          if (player->units[unit_type][i].is_allocated && player->units[unit_type][i].data.is_alive)
          {
            player->data.currently_building_unit = &player->units[unit_type][i];
            break;
          }
        }

        if (!player->data.currently_building_unit)
        {
          player->invalid_command_count++;
          advance_instruction = true;
          break;
        }
        command_all_units_to_assist(
                player,
                static_cast<Game<BAR_game_config>::Unit *>(player->data.currently_building_unit),
                unit_type
        );
      }

      if (!player->data.currently_building_unit)
      {
        player->invalid_command_count++;
        advance_instruction = true;
        break;
      }

      if (!static_cast<Game<BAR_game_config>::Unit *>(player->data.currently_building_unit)->data.is_alive) {
        // Reclaim order was finished, continue!
        advance_instruction = true;
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
      BARUnitTypeMetadata_T& unit_metadata = bar_game_unit_type_metadata_table.values[unit_type];

      if (first_tick) {
        if (player->data.builder_for_unit_type_cache_dirty)
        {
          update_builder_for_unit_type_cache(player);
        }
        player->data.currently_building_unit = nullptr;

        // Check if we have a builder that can build this
        if (player->data.builder_for_unit_type_cache[unit_type] == BAR_UnitType_None)
        {
          advance_instruction = true;
          player->invalid_command_count++;
          break;
        }

        BAR_UnitType builder_type = player->data.builder_for_unit_type_cache[unit_type];

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

        // Find builder
        Game<BAR_game_config>::Unit* builder = nullptr;
        for (uint32_t i = 0; i < player->furthest_allocated_unit[builder_type]; i++)
        {
          if (player->units[builder_type][i].is_allocated && player->units[builder_type][i].data.is_alive)
          {
            builder = &player->units[builder_type][i];
            break;
          }
        }

        if (!builder)
        {
          advance_instruction = true;
          player->invalid_command_count++;
          break;
        }

        // Do it
        player->data.currently_building_unit = player->allocate_unit(unit_type);
        if (!player->data.currently_building_unit)
        {
          // Allocation failure
          advance_instruction = true;
          player->invalid_command_count++;
          break;
        }
        command_unit_to_build_inner(
                builder,
                builder_type,
                static_cast<Game<BAR_game_config>::Unit *>(player->data.currently_building_unit),
                unit_type);
        command_all_units_to_assist(
                player,
                static_cast<Game<BAR_game_config>::Unit *>(player->data.currently_building_unit),
                unit_type
                );
      }

      if (!player->data.currently_building_unit)
      {
        player->invalid_command_count++;
        advance_instruction = true;
        break;
      }

      if (static_cast<Game<BAR_game_config>::Unit *>(player->data.currently_building_unit)->data.is_alive) {
        // Build order was finished, continue!
        advance_instruction = true;
      }
      break;
    }
  }
  return advance_instruction;
}

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

template<uint32_t unit_type=0> static inline uint32_t upkeep_iterator(Game<BAR_game_config>::Player *player, BAR_ResourceType resource_type)
{
  uint32_t total_upkeep = 0;
  for (uint32_t j = 0; j < player->furthest_allocated_unit[unit_type]; j++)
  {
    if (player->units[unit_type][j].is_allocated && player->units[unit_type][j].data.is_alive)
    {
      if (resource_type == BAR_ResourceType_Energy)
      {
        total_upkeep += bar_game_get_unit_type_metadata<static_cast<BAR_UnitType>(unit_type)>().energy_upkeep;
      }
      if (resource_type == BAR_ResourceType_Metal)
      {
        total_upkeep += bar_game_get_unit_type_metadata<static_cast<BAR_UnitType>(unit_type)>().metal_upkeep;
      }
    }
  }
  if constexpr (unit_type == BAR_UnitType_MAX)
  {
    return total_upkeep;
  }
  else
  {
    return total_upkeep + upkeep_iterator<unit_type+1>(player, resource_type);
  }
}

uint32_t bar_game_get_full_upkeep(Game<BAR_game_config>::Player *player, BAR_ResourceType resource_type)
{
  return upkeep_iterator(player, resource_type);
}
