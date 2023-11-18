//
// Created by christian on 11/10/23.
//

#ifndef MLX86_BAR_GAME_CONFIG_HPP
#define MLX86_BAR_GAME_CONFIG_HPP

#include "game.hpp"
#include "bar_game_config_metadata.hpp"
#include "bar_game_map_data.hpp"

enum BAR_ResourceType {
  BAR_ResourceType_Metal,
  BAR_ResourceType_Energy,
  BAR_ResourceType_MAX
};

constexpr float metal_richness = bar_game_map_metal_positions[0].value;
constexpr float wind_speed = (bar_game_map_max_wind_speed + bar_game_map_min_wind_speed)/2;

constexpr inline static struct BARUnitTypeMetadata_T bar_game_get_unit_type_metadata_shim(BAR_UnitType unit_type) {
  BARUnitTypeMetadata_T metadata = bar_game_get_unit_type_metadata_generated(unit_type);

  // Patch generated metadata
  if (metadata.metal_extractor)
  {
    metadata.metal_production += (uint32_t) ((uint32_t)(metal_richness * (float)metadata.metal_extractor) / bar_game_tps);
    metadata.avg_travel_time = 280;
  }

  if (metadata.is_builder)
  {
    if (metadata.build_distance == 0)
    {
      metadata.can_assist = 0;
    }
    if (!metadata.can_assist)
    {
      metadata.build_power = 0;
    }
  }

  switch (unit_type) {
    default:
      break;
    case BAR_UnitType_corwin:
    case BAR_UnitType_armwin:
      metadata.energy_production = (uint32_t)(wind_speed * bar_game_resource_denominator / bar_game_tps);
      break;
  }

  return metadata;
}

template <BAR_UnitType unit_type> inline static struct BARUnitTypeMetadata_T bar_game_get_unit_type_metadata()
{
  return bar_game_get_unit_type_metadata_shim(unit_type);
}

struct BAR_UnitMetadata_Table_T {
  constexpr BAR_UnitMetadata_Table_T() : values()
  {
    for (uint32_t unit_type = BAR_UnitType_None; unit_type < BAR_UnitType_MAX; unit_type++)
    {
      values[unit_type] = bar_game_get_unit_type_metadata_shim(static_cast<BAR_UnitType>(unit_type));
    }
  }
  struct BARUnitTypeMetadata_T values[BAR_UnitType_MAX];
};

extern BAR_UnitMetadata_Table_T bar_game_unit_type_metadata_table;

enum BAR_Instruction {
  BAR_Instruction_None,
  BAR_Instruction_Build,
  BAR_Instruction_Reclaim
};

class BAR_game_config {
public:

  static constexpr const char* name()
  {
    return "Beyond All Reason";
  }

  static constexpr uint32_t game_tps = bar_game_tps;
  static constexpr uint32_t num_players = 1;
  static constexpr uint32_t num_unit_types = BAR_UnitType_MAX;
  static constexpr uint32_t max_num_units_of_type = bar_game_max_num_units_of_type;
  static constexpr uint32_t num_resource_types = BAR_ResourceType_MAX;
  static constexpr uint32_t resource_denominator = bar_game_resource_denominator;

  struct PlayerData {
    uint32_t geothermal_slots_total = 0;
    uint32_t geothermal_slots_used = 0;
    uint32_t metal_slots_total = 0;
    uint32_t metal_slots_used = 0;

    BAR_UnitType builder_for_unit_type_cache[BAR_game_config::num_unit_types] = {};
    bool builder_for_unit_type_cache_dirty = true;

    void* currently_building_unit = nullptr;
  };

  struct UnitData {
    bool is_alive = false;
    bool is_starved = false;
    int32_t allocated_resources[BAR_ResourceType_MAX] = {0};
    int32_t allocated_build_points = 0;
    int32_t energy_stored = 0;
    void* currently_building_unit = nullptr;
    uint32_t currently_building_unit_type = BAR_UnitType_None;
    int32_t current_build_resources_per_tick[BAR_ResourceType_MAX] = {0};
    uint32_t currently_reclaiming_unit_type = BAR_UnitType_None;
  };

  static constexpr const char * get_unit_type_name(uint32_t unit)
  {
    if (unit >= BAR_UnitType_MAX)
      return "Invalid";
    return bar_game_unit_type_metadata_table.values[unit].name;
  }

  static constexpr inline const char * get_resource_type_name(uint32_t resource_type)
  {
    if (resource_type == BAR_ResourceType_Metal)
      return "Metal";
    else if (resource_type == BAR_ResourceType_Energy)
      return "Energy";
    return "Invalid";
  }

  static inline void pretty_print_instruction(std::string indent, void* instruction_in)
  {
    auto instruction = (Game<BAR_game_config>::Instruction*)(instruction_in);
    auto instruction_type = (BAR_Instruction)instruction->type;
    switch (instruction_type) {
      case BAR_Instruction_None:
        break;
      case BAR_Instruction_Build:
        if ((instruction->data >= BAR_UnitType_MAX) || (instruction->data == BAR_UnitType_None) || (instruction->iterations == 0))
        {
          printf("%sInvalid Instruction\n", indent.c_str());
          break;
        }
        if (instruction->iterations != 1)
        {
          printf("%sBuild %s (x%d)\n", indent.c_str(), get_unit_type_name(instruction->data), instruction->iterations);
        }
        else
        {
          printf("%sBuild %s\n", indent.c_str(), get_unit_type_name(instruction->data));
        }
        break;
      case BAR_Instruction_Reclaim:
        if ((instruction->data >= BAR_UnitType_MAX) || (instruction->data == BAR_UnitType_None))
        {
          printf("%sInvalid Instruction\n", indent.c_str());
          break;
        }
        if (instruction->iterations != 1)
        {
          printf("%sReclaim %s (x%d)\n", indent.c_str(), get_unit_type_name(instruction->data), instruction->iterations);
        }
        else
        {
          printf("%sReclaim %s\n", indent.c_str(), get_unit_type_name(instruction->data));
        }
        break;
      default:
        printf("%sInvalid Instruction\n", indent.c_str());
        break;
    }
  }

  template <uint32_t unit_type, uint32_t resource_type> static inline uint32_t get_unit_resource_production_rate(Game<BAR_game_config>::Player * player, Game<BAR_game_config>::Unit* unit)
  {
    if (!unit->data.is_alive || unit->data.is_starved)
    {
      return 0;
    }
    if (resource_type == BAR_ResourceType_Metal)
      return bar_game_get_unit_type_metadata<static_cast<BAR_UnitType>(unit_type)>().metal_production;
    else if (resource_type == BAR_ResourceType_Energy)
      return bar_game_get_unit_type_metadata<static_cast<BAR_UnitType>(unit_type)>().energy_production;
    return 0;
  }

  template <uint32_t unit_type, uint32_t resource_type> static inline uint32_t get_unit_resource_capacity(Game<BAR_game_config>::Player* player, Game<BAR_game_config>::Unit* unit)
  {
    if (!unit->data.is_alive)
    {
      return 0;
    }
    if (resource_type == BAR_ResourceType_Metal)
      return bar_game_get_unit_type_metadata<static_cast<BAR_UnitType>(unit_type)>().metal_capacity;
    else if (resource_type == BAR_ResourceType_Energy)
      return bar_game_get_unit_type_metadata<static_cast<BAR_UnitType>(unit_type)>().energy_capacity;
    return 0;
  }

  static Game<BAR_game_config>::Unit* free_unit_build(Game<BAR_game_config>::Player* player, BAR_UnitType unit_type)
  {
    auto unit = player->allocate_unit(unit_type);
    if (unit)
    {
      on_unit_alive(player, unit, unit_type);
    }
    return unit;
  }

  static inline void on_player_init(Game<BAR_game_config>::Player* player, BAR_Faction faction = BAR_Faction_Armada)
  {
    Game<BAR_game_config>::Unit* commander;
    switch (faction)
    {
      case BAR_Faction_Cortex:
        commander = free_unit_build(player, BAR_UnitType_corcom);
        break;
      default:
      case BAR_Faction_Armada:
        commander = free_unit_build(player, BAR_UnitType_armcom);
        break;
    }
    commander->x = bar_game_map_start_positions[0].x;
    commander->z = bar_game_map_start_positions[0].z;
    player->resources[BAR_ResourceType_Metal] = 1000*bar_game_resource_denominator;
    player->resources[BAR_ResourceType_Energy] = 1000*bar_game_resource_denominator;
    player->base_resource_capacity[BAR_ResourceType_Metal] = 500 * bar_game_resource_denominator;
    player->base_resource_capacity[BAR_ResourceType_Energy] = 500 * bar_game_resource_denominator;

    player->data.geothermal_slots_total = 0;
    player->data.metal_slots_total = 5;

    for (auto & i : player->data.builder_for_unit_type_cache) {
      i = BAR_UnitType_None;
    }
  }

  template <uint32_t unit_type=BAR_game_config::num_unit_types-1> static inline void update_builder_for_unit_type_cache(Game<BAR_game_config>::Player* player)
  {
    if constexpr (unit_type == BAR_game_config::num_unit_types-1)
    {
      for (auto& unit_type_cache : player->data.builder_for_unit_type_cache) {
        unit_type_cache = BAR_UnitType_None;
      }
    }

    // See if this unit has any alive
    bool has_one_alive = false;
    for (uint32_t i = 0; i < player->furthest_allocated_unit[unit_type]; i++) {
      if (player->units[unit_type][i].is_allocated && player->units[unit_type][i].data.is_alive) {
        has_one_alive = true;
        break;
      }
    }

    if (has_one_alive)
    {
      auto metadata = bar_game_get_unit_type_metadata<static_cast<BAR_UnitType>(unit_type)>();

      for (auto unit_type_to_build : metadata.build_options) {
        if (unit_type_to_build == BAR_UnitType_None) {
          break;
        }
        uint32_t competing_build_power = 0;
        if (player->data.builder_for_unit_type_cache[unit_type_to_build])
        {
          competing_build_power = bar_game_unit_type_metadata_table.values[player->data.builder_for_unit_type_cache[unit_type_to_build]].direct_build_power;
        }
        if (competing_build_power <= metadata.direct_build_power) {
          player->data.builder_for_unit_type_cache[unit_type_to_build] = static_cast<BAR_UnitType>(unit_type);
        }
      }
    }

    if constexpr (unit_type == 0)
    {
      player->data.builder_for_unit_type_cache_dirty = false;
      return;
    }
    else
    {
      update_builder_for_unit_type_cache<unit_type-1>(player);
    }
  }

  template <uint32_t unit_type> static inline void unit_pre_tick(Game<BAR_game_config>::Player* player, Game<BAR_game_config>::Unit* unit)
  {
    auto metadata = bar_game_get_unit_type_metadata<static_cast<BAR_UnitType>(unit_type)>();
    if (!unit->data.is_alive)
    {
      // Figure out if we can build ourselves this tick
      bool can_finish = true;
      if (unit->data.allocated_build_points < metadata.build_cost)
        can_finish = false;
      if (unit->data.allocated_resources[BAR_ResourceType_Energy] < metadata.energy_cost)
        can_finish = false;
      if (unit->data.allocated_resources[BAR_ResourceType_Metal] < metadata.metal_cost)
        can_finish = false;
      if (can_finish) {
        // Refund extra resources
        player->resources[BAR_ResourceType_Metal] += unit->data.allocated_resources[BAR_ResourceType_Metal] - metadata.metal_cost;
        player->resources[BAR_ResourceType_Energy] += unit->data.allocated_resources[BAR_ResourceType_Energy] - metadata.energy_cost;

        on_unit_alive(player, unit, unit_type);
      }
      return;
    }

    if (unit->data.allocated_build_points < 0)
    {
      // Unit has been reclaimed!
      player->resources[BAR_ResourceType_Metal] += metadata.metal_cost;
      player->deallocate_unit(unit, unit_type);
      return;
    }

    if ((metadata.metal_upkeep > 0) || (metadata.energy_upkeep > 0))
    {
      // Unit upkeep cost
      unit->data.is_starved = false;
      unit->data.is_starved |= player->resources[BAR_ResourceType_Metal] < metadata.metal_upkeep;
      unit->data.is_starved |= player->resources[BAR_ResourceType_Energy] < metadata.energy_upkeep;
      if (!unit->data.is_starved) {
        player->resources[BAR_ResourceType_Metal] -= metadata.metal_upkeep;
        player->resources[BAR_ResourceType_Energy] -= metadata.energy_upkeep;
      }
    }

    // Cash out converters
    if ((metadata.energy_convert_capacity != 0) && (player->game->tick % bar_game_tps == 0))
    {
      if ((unit->data.energy_stored >= metadata.energy_convert_capacity) && (player->resources[BAR_ResourceType_Metal] < player->resource_max_capacity_cache[BAR_ResourceType_Metal]))
      {
        unit->data.energy_stored -= metadata.energy_convert_capacity;
        player->resources[BAR_ResourceType_Metal] += metadata.energy_convert_return;
      }
    }

    // Stop building if a unit is now alive
    if ((metadata.build_power > 0) && (unit->data.currently_building_unit_type != BAR_UnitType_None))
    {
      if (unit->data.currently_building_unit == nullptr ||
          static_cast<Game<BAR_game_config>::Unit*>(unit->data.currently_building_unit)->data.is_alive)
      {
        unit->data.currently_building_unit_type = BAR_UnitType_None;
        unit->data.currently_building_unit = nullptr;
      }
    }

    // Build tick
    if ((metadata.build_power > 0) && (unit->data.currently_building_unit_type != BAR_UnitType_None))
    {
      bool can_build_this_tick = true;
      for (uint32_t i = 0; i < BAR_ResourceType_MAX; i++)
      {
        can_build_this_tick &= (player->resources[i] >= unit->data.current_build_resources_per_tick[i]);
      }

      auto buildee = static_cast<Game<BAR_game_config>::Unit*>(unit->data.currently_building_unit);

      if (can_build_this_tick) {
        buildee->data.allocated_build_points += metadata.build_power;
        for (uint32_t i = 0; i < BAR_ResourceType_MAX; i++)
        {
          buildee->data.allocated_resources[i] += unit->data.current_build_resources_per_tick[i];
          player->resources[i] -= unit->data.current_build_resources_per_tick[i];
        }
      }
    }

    // Stop reclaiming if a unit is now dead
    if ((metadata.build_power > 0) && (unit->data.currently_reclaiming_unit_type != BAR_UnitType_None))
    {
      if (unit->data.currently_building_unit == nullptr ||
          !static_cast<Game<BAR_game_config>::Unit*>(unit->data.currently_building_unit)->data.is_alive)
      {
        unit->data.currently_reclaiming_unit_type = BAR_UnitType_None;
        unit->data.currently_building_unit = nullptr;
      }
    }

    // Reclaim tick
    if ((metadata.build_power > 0) && (unit->data.currently_reclaiming_unit_type != BAR_UnitType_None))
    {
      static_cast<Game<BAR_game_config>::Unit*>(unit->data.currently_building_unit)->data.allocated_build_points -= metadata.build_power;
    }
  }

  template <BAR_UnitType builder_unit_type=BAR_UnitType_None> static inline void command_all_units_to_assist(
    Game<BAR_game_config>::Player* player,
    Game<BAR_game_config>::Unit* buildee,
    BAR_UnitType buildee_unit_type)
  {
    if constexpr (builder_unit_type >= BAR_UnitType_MAX)
    {
      return;
    }
    else
    {
      for (uint32_t i = 0; i < player->furthest_allocated_unit[builder_unit_type]; i++)
      {
        if (player->units[builder_unit_type][i].is_allocated && player->units[builder_unit_type][i].data.is_alive)
        {
          auto builder_metadata = bar_game_get_unit_type_metadata<builder_unit_type>();

          if (builder_metadata.build_power == 0)
          {
            // Cannot assist!
            continue;
          }

          if (builder_metadata.build_distance == 0)
          {
            // Cannot assist!
            continue;
          }

          if (!builder_metadata.can_assist)
          {
            // Cannot assist!
            continue;
          }

          command_unit_to_build_inner(
                  &player->units[builder_unit_type][i],
                  builder_unit_type,
                  buildee,
                  buildee_unit_type);
        }
      }

      command_all_units_to_assist<static_cast<BAR_UnitType>(static_cast<uint32_t>(builder_unit_type) + 1)>(
              player,
              buildee,
              buildee_unit_type
      );
    }
  }

  static inline void command_unit_to_build_inner(
          Game<BAR_game_config>::Unit* builder,
          BAR_UnitType builder_unit_type,
          Game<BAR_game_config>::Unit* buildee,
          BAR_UnitType buildee_unit_type)
  {
    if (!builder->data.is_alive)
    {
      return;
    }

    auto builder_metadata = bar_game_unit_type_metadata_table.values[builder_unit_type];
    auto& buildee_metadata = bar_game_unit_type_metadata_table.values[buildee_unit_type];

    builder->data.currently_building_unit = buildee;
    builder->data.currently_building_unit_type = buildee_unit_type;

    builder->data.current_build_resources_per_tick[BAR_ResourceType_Energy] = (int32_t)(((int64_t)builder_metadata.build_power * (int64_t)buildee_metadata.energy_cost) / (uint64_t)buildee_metadata.build_cost);
    builder->data.current_build_resources_per_tick[BAR_ResourceType_Metal] = (int32_t)(((int64_t)builder_metadata.build_power * (int64_t)buildee_metadata.metal_cost) / (uint64_t)buildee_metadata.build_cost);
  }

  template <uint32_t unit_type> static inline void unit_post_tick(Game<BAR_game_config>::Player* player, Game<BAR_game_config>::Unit* unit) {
    if (!unit->data.is_alive) {
      return;
    }

    auto metadata = bar_game_get_unit_type_metadata<static_cast<BAR_UnitType>(unit_type)>();

    if (metadata.energy_convert_capacity == 0)
    {
      return;
    }

    // Add to power converter banks
    int32_t power_for_converters = (int32_t) player->resources[BAR_ResourceType_Energy] - (int32_t) (player->resource_max_capacity_cache[BAR_ResourceType_Energy] / 2);
    if (power_for_converters > 0) {
      int32_t max_allowed_contribution = metadata.energy_convert_capacity - (int32_t) unit->data.energy_stored;
      if (max_allowed_contribution < power_for_converters) {
        power_for_converters = max_allowed_contribution;
      }
      unit->data.energy_stored += power_for_converters;
      player->resources[BAR_ResourceType_Energy] -= power_for_converters;
    }
  }

  static inline void on_unit_allocated(Game<BAR_game_config>::Player* player, Game<BAR_game_config>::Unit* unit, uint32_t unit_type) {
    unit->data.is_alive = false;
    unit->data.allocated_resources[BAR_ResourceType_Metal] = 0;
    unit->data.allocated_resources[BAR_ResourceType_Energy] = 0;
    unit->data.allocated_build_points = 0;
    unit->data.energy_stored = 0;
  }

  static inline void on_unit_death(Game<BAR_game_config>::Player* player, Game<BAR_game_config>::Unit* unit, uint32_t unit_type)
  {
    // Reclaim slots used
    if (bar_game_unit_type_metadata_table.values[unit_type].geothermal)
    {
      if (player->data.geothermal_slots_used)
      {
        player->data.geothermal_slots_used--;
      }
    }

    if (bar_game_unit_type_metadata_table.values[unit_type].metal_extractor)
    {
      if (player->data.metal_slots_used)
      {
        player->data.metal_slots_used--;
      }
    }
  }

  static inline void on_unit_alive(Game<BAR_game_config>::Player* player, Game<BAR_game_config>::Unit* unit, uint32_t unit_type)
  {
    unit->data.allocated_resources[BAR_ResourceType_Energy] = bar_game_unit_type_metadata_table.values[unit_type].energy_cost;
    unit->data.allocated_resources[BAR_ResourceType_Metal] = bar_game_unit_type_metadata_table.values[unit_type].metal_cost;
    unit->data.allocated_build_points = bar_game_unit_type_metadata_table.values[unit_type].build_cost;
    unit->data.is_alive = true;
    unit->data.is_starved = false;
    unit->data.currently_building_unit = nullptr;

    // Delete without recovery any replaced structures
    if (bar_game_unit_type_metadata_table.values[unit_type].geothermal)
    {
      if (player->data.geothermal_slots_total == player->data.geothermal_slots_used)
      {
        bool did_reclaim = false;
        if (!did_reclaim)
        {
          did_reclaim = reclaim_unit_instant(player, BAR_UnitType_armgeo, false, false);
        }
        if (!did_reclaim)
        {
          did_reclaim = reclaim_unit_instant(player, BAR_UnitType_corgeo, false, false);
        }
        if (!did_reclaim)
        {
          did_reclaim = reclaim_unit_instant(player, BAR_UnitType_armageo, false, false);
        }
        if (!did_reclaim)
        {
          did_reclaim = reclaim_unit_instant(player, BAR_UnitType_corageo, false, false);
        }
        if (!did_reclaim)
        {
          did_reclaim = reclaim_unit_instant(player, BAR_UnitType_armgmm, false, false);
        }
        if (!did_reclaim)
        {
          did_reclaim = reclaim_unit_instant(player, BAR_UnitType_corbhmth, false, false);
        }
      }
    }
    if (bar_game_unit_type_metadata_table.values[unit_type].metal_extractor)
    {
      if (player->data.metal_slots_total == player->data.metal_slots_used)
      {
        bool did_reclaim = false;
        if (!did_reclaim)
        {
          did_reclaim = reclaim_unit_instant(player, BAR_UnitType_armmex, false, false);
        }
        if (!did_reclaim)
        {
          did_reclaim = reclaim_unit_instant(player, BAR_UnitType_armamex, false, false);
        }
        if (!did_reclaim)
        {
          did_reclaim = reclaim_unit_instant(player, BAR_UnitType_armmoho, false, false);
        }
        if (!did_reclaim)
        {
          did_reclaim = reclaim_unit_instant(player, BAR_UnitType_armuwmme, false, false);
        }
        if (!did_reclaim)
        {
          did_reclaim = reclaim_unit_instant(player, BAR_UnitType_cormex, false, false);
        }
        if (!did_reclaim)
        {
          did_reclaim = reclaim_unit_instant(player, BAR_UnitType_corexp, false, false);
        }
        if (!did_reclaim)
        {
          did_reclaim = reclaim_unit_instant(player, BAR_UnitType_cormoho, false, false);
        }
      }
    }

    // Claim slots used
    if (bar_game_unit_type_metadata_table.values[unit_type].geothermal)
    {
      player->data.geothermal_slots_used++;
    }
    if (bar_game_unit_type_metadata_table.values[unit_type].metal_extractor)
    {
      player->data.metal_slots_used++;
    }

    if (bar_game_unit_type_metadata_table.values[unit_type].build_options[0])
    {
      player->data.builder_for_unit_type_cache_dirty = true;
    }

    if (bar_game_unit_type_metadata_table.values[unit_type].energy_capacity || bar_game_unit_type_metadata_table.values[unit_type].metal_capacity)
    {
      player->resource_max_capacity_cache_dirty = true;
    }

    if (bar_game_unit_type_metadata_table.values[unit_type].energy_production || bar_game_unit_type_metadata_table.values[unit_type].metal_production)
    {
      player->resource_production_rate_cache_dirty = true;
    }
  }

  static inline void on_unit_deallocated(Game<BAR_game_config>::Player* player, Game<BAR_game_config>::Unit* unit, uint32_t unit_type) {
    if (unit->data.is_alive)
    {
      on_unit_death(player, unit, unit_type);
      unit->data.is_alive = false;
    }
  }



  static inline void print_detailed_unit_info(Game<BAR_game_config>::Player* player, Game<BAR_game_config>::Unit* unit, uint32_t unit_type, std::string indent)
  {
    printf("%s%s:", indent.c_str(), get_unit_type_name(unit_type));
    printf(" %d build points", unit->data.allocated_build_points);
    for (uint32_t k = 0; k < BAR_ResourceType_MAX; k++)
    {
      printf(", %f %s", ((float)unit->data.allocated_resources[k])/((float)bar_game_resource_denominator), get_resource_type_name(k));
    }
    printf("\n");
  }

  static inline bool reclaim_unit_instant(Game<BAR_game_config>::Player *player, uint32_t unit_type, bool reclaim_metal, bool reclaim_energy)
  {
    if (unit_type >= BAR_UnitType_MAX)
    {
      return false;
    }
    Game<BAR_game_config>::Unit *unit = nullptr;
    for (uint32_t i = 0; i < player->furthest_allocated_unit[unit_type]; i++)
    {
      if (player->units[unit_type][i].is_allocated)
      {
        if (player->units[unit_type][i].data.is_alive)
        {
          unit = &player->units[unit_type][i];
          break;
        }
      }
    }

    if (unit)
    {
      if (reclaim_metal)
      {
        player->resources[BAR_ResourceType_Metal] += bar_game_unit_type_metadata_table.values[unit_type].metal_cost;
      }
      if (reclaim_energy)
      {
        player->resources[BAR_ResourceType_Energy] += bar_game_unit_type_metadata_table.values[unit_type].energy_cost;
      }
      player->deallocate_unit(unit, unit_type);
    }
    return unit != nullptr;
  }

  static bool follow_instruction(Game<BAR_game_config>::Player* player, Game<BAR_game_config>::Instruction* instruction, bool first_tick);
  template <BAR_UnitType unit_type> static bool follow_instruction_build_unit();
};


uint32_t bar_game_get_num_of_unit_type(Game<BAR_game_config>::Player *player, BAR_UnitType unit_type);

uint32_t bar_game_get_full_upkeep(Game<BAR_game_config>::Player *player, BAR_ResourceType resource_type);


#endif //MLX86_BAR_GAME_CONFIG_HPP
