//
// Created by christian on 11/10/23.
//

#ifndef MLX86_BAR_GAME_CONFIG_HPP
#define MLX86_BAR_GAME_CONFIG_HPP

#include "game.hpp"

enum BAR_UnitType {
  BAR_UnitType_None,
  BAR_UnitType_ArmadaCommander,
  BAR_UnitType_ArmadaMetalExtractor,
  BAR_UnitType_ArmadaEnergyConverter,
  BAR_UnitType_ArmadaMetalStorage,
  BAR_UnitType_ArmadaWindTurbine,
  BAR_UnitType_ArmadaSolarCollector,
  BAR_UnitType_ArmadaEnergyStorage,
  BAR_UnitType_ArmadaBotLab,
  BAR_UnitType_ArmadaVehiclePlant,
  BAR_UnitType_ArmadaHovercraftPlant,
  BAR_UnitType_ArmadaAircraftPlant,
  BAR_UnitType_ArmadaConstructionBot,
  BAR_UnitType_ArmadaAdvancedSolarCollector,
  BAR_UnitType_ArmadaGeothermalPowerplant,
  BAR_UnitType_ArmadaConstructionTurret,
  BAR_UnitType_ArmadaAdvancedBotLab,
  BAR_UnitType_ArmadaAdvancedConstructionBot,
  BAR_UnitType_ArmadaAdvancedMetalExtractor,
  BAR_UnitType_ArmadaAdvancedEnergyConverter,
  BAR_UnitType_ArmadaHardenedMetalStorage,
  BAR_UnitType_ArmadaPrude,
  BAR_UnitType_ArmadaAdvancedGeothermalPowerplant,
  BAR_UnitType_ArmadaFusionReactor,
  BAR_UnitType_ArmadaCloakableFusionReactor,
  BAR_UnitType_ArmadaAdvancedFusionReactor,
  BAR_UnitType_ArmadaHardenedEnergyStorage,
  BAR_UnitType_ArmadaArmageddon,
  BAR_UnitType_ArmadaTick,
  BAR_UnitType_ArmadaConstructionAircraft,
  BAR_UnitType_ArmadaStormbringer,
  BAR_UnitType_ArmadaAdvancedAircraftPlant,
  BAR_UnitType_ArmadaBlizzard,
  BAR_UnitType_ArmadaAdvancedConstructionAircraft,
  BAR_UnitType_ArmadaLiche,
  BAR_UnitType_MAX
};

enum BAR_ResourceTypes {
  BAR_ResourceType_Metal,
  BAR_ResourceType_Energy,
  BAR_ResourceType_MAX
};

constexpr uint32_t bar_game_max_num_required_units = 4;
constexpr uint32_t bar_game_max_num_units_of_type = 256;

class BARUnitTypeMetadata_T {
public:
  const char* name = "Invalid";
  uint32_t metal_cost = 0;
  uint32_t energy_cost = 0;
  uint32_t build_cost  = 0;
  uint32_t build_power = 0;
  uint32_t direct_build_power = 0;
  bool can_share_build_power = true;
  uint32_t metal_production = 0;
  uint32_t energy_production = 0;
  uint32_t energy_capacity = 0;
  uint32_t metal_capacity = 0;
  BAR_UnitType required_unit[bar_game_max_num_required_units] = {};
  uint32_t max_count = bar_game_max_num_units_of_type;
  uint32_t avg_travel_time = 0;
  uint32_t energy_upkeep = 0;
  uint32_t metal_upkeep = 0;
};

constexpr uint32_t mine_limit = 3;
constexpr uint32_t geothermal_limit = 0;
constexpr uint32_t turbine_limit = 500;

constexpr uint32_t bar_game_tps = 20;

constexpr uint32_t bar_game_resource_denominator = 1000;

#define general_armada_tier1_constructor {BAR_UnitType_ArmadaConstructionBot, BAR_UnitType_ArmadaConstructionAircraft}
#define general_armada_tier2_constructor {BAR_UnitType_ArmadaAdvancedConstructionBot, BAR_UnitType_ArmadaAdvancedConstructionAircraft}

constexpr BARUnitTypeMetadata_T BAR_UnitType_Metadata[BAR_UnitType_MAX] =
{
    {
      .name = "Invalid Unit"
    },
    {
      .name = "Armada Commander",
      .metal_cost = 2700*bar_game_resource_denominator,
      .energy_cost = 26000*bar_game_resource_denominator,
      .build_cost = 75000,
      .build_power = 300/bar_game_tps,
      .metal_production = 2*bar_game_resource_denominator/bar_game_tps,
      .energy_production = 25*bar_game_resource_denominator/bar_game_tps,
      .energy_capacity = 1000*bar_game_resource_denominator,
      .metal_capacity = 1000*bar_game_resource_denominator,
      .max_count = 1
    },
    {
      .name = "Armada Metal Extractor",
      .metal_cost = 50*bar_game_resource_denominator,
      .energy_cost = 500*bar_game_resource_denominator,
      .build_cost = 1800,
      .metal_production = (uint32_t)(1.8*bar_game_resource_denominator/bar_game_tps),
      .metal_capacity = 50*bar_game_resource_denominator,
      .required_unit = {BAR_UnitType_ArmadaCommander},
      .max_count = mine_limit,
      .avg_travel_time = 280,
      .energy_upkeep = 3*bar_game_resource_denominator/bar_game_tps,
    },
    {
      .name = "Armada Energy Converter",
      .metal_cost = 1*bar_game_resource_denominator,
      .energy_cost = 1150*bar_game_resource_denominator,
      .build_cost = 2600,
      .required_unit = {BAR_UnitType_ArmadaCommander},
    },
    {
      .name = "Armada Metal Storage",
      .metal_cost = 330*bar_game_resource_denominator,
      .energy_cost = 570*bar_game_resource_denominator,
      .build_cost = 2920,
      .metal_capacity = 3000*bar_game_resource_denominator,
      .required_unit = {BAR_UnitType_ArmadaCommander},
    },
    {
      .name = "Armada Wind Turbine",
      .metal_cost = 37*bar_game_resource_denominator,
      .energy_cost = 175*bar_game_resource_denominator,
      .build_cost = 1600,
      .energy_production = 14*bar_game_resource_denominator/bar_game_tps,
      .energy_capacity = (uint32_t)(0.5*bar_game_resource_denominator),
      .required_unit = {BAR_UnitType_ArmadaCommander},
      .max_count = turbine_limit
    },
    {
      .name = "Armada Solar Collector",
      .metal_cost = 155*bar_game_resource_denominator,
      .energy_cost = 0*bar_game_resource_denominator,
      .build_cost = 2800,
      .energy_production = 20*bar_game_resource_denominator/bar_game_tps,
      .energy_capacity = 50*bar_game_resource_denominator,
      .required_unit = {BAR_UnitType_ArmadaCommander},
    },
    {
      .name = "Armada Energy Storage",
      .metal_cost = 170*bar_game_resource_denominator,
      .energy_cost = 1700*bar_game_resource_denominator,
      .build_cost = 4110,
      .energy_capacity = 6000*bar_game_resource_denominator,
      .required_unit = {BAR_UnitType_ArmadaCommander},
    },
    {
      .name = "Armada Bot Lab",
      .metal_cost = 650*bar_game_resource_denominator,
      .energy_cost = 1200*bar_game_resource_denominator,
      .build_cost = 6500,
      .direct_build_power = 100/bar_game_tps,
      .can_share_build_power = false,
      .energy_capacity = 100*bar_game_resource_denominator,
      .metal_capacity = 100*bar_game_resource_denominator,
      .required_unit = {BAR_UnitType_ArmadaCommander},
    },
    {
      .name = "Armada Vehicle Plant",
      .metal_cost = 740*bar_game_resource_denominator,
      .energy_cost = 1800*bar_game_resource_denominator,
      .build_cost = 7200,
      .direct_build_power = 100/bar_game_tps,
      .energy_capacity = 100*bar_game_resource_denominator,
      .metal_capacity = 100*bar_game_resource_denominator,
      .required_unit = {BAR_UnitType_ArmadaCommander},
    },
    {
      .name = "Armada Hovercraft Plant",
      .metal_cost =  900*bar_game_resource_denominator,
      .energy_cost = 3000*bar_game_resource_denominator,
      .build_cost = 11000,
      .direct_build_power = 100/bar_game_tps,
      .energy_capacity = 200*bar_game_resource_denominator,
      .metal_capacity = 200*bar_game_resource_denominator,
      .required_unit = {BAR_UnitType_ArmadaCommander},
    },
    {
      .name = "Armada Aircraft Plant",
      .metal_cost =  860*bar_game_resource_denominator,
      .energy_cost = 1350*bar_game_resource_denominator,
      .build_cost = 7250,
      .direct_build_power = 100/bar_game_tps,
      .energy_capacity = 100*bar_game_resource_denominator,
      .metal_capacity = 100*bar_game_resource_denominator,
      .required_unit = {BAR_UnitType_ArmadaCommander, BAR_UnitType_ArmadaConstructionAircraft, BAR_UnitType_ArmadaAdvancedConstructionAircraft},
    },
    {
      .name = "Armada Construction Bot",
      .metal_cost = 110*bar_game_resource_denominator,
      .energy_cost = 1600*bar_game_resource_denominator,
      .build_cost = 3450,
      .build_power = 80/bar_game_tps,
      .energy_production = 7*bar_game_resource_denominator/bar_game_tps,
      .required_unit = {BAR_UnitType_ArmadaBotLab}
    },
    {
      .name = "Armada Advanced Solar Collector",
      .metal_cost = 370*bar_game_resource_denominator,
      .energy_cost = 5000*bar_game_resource_denominator,
      .build_cost = 7950,
      .energy_production = 75*bar_game_resource_denominator/bar_game_tps,
      .energy_capacity = 100*bar_game_resource_denominator,
      .required_unit = general_armada_tier1_constructor,
    },
    {
      .name = "Armada Geothermal Power plant",
      .metal_cost = 360*bar_game_resource_denominator,
      .energy_cost = 13000*bar_game_resource_denominator,
      .build_cost = 13100,
      .energy_production = 300*bar_game_resource_denominator/bar_game_tps,
      .energy_capacity = 1000*bar_game_resource_denominator,
      .required_unit = general_armada_tier1_constructor,
      .max_count = geothermal_limit,
      .avg_travel_time = 28*bar_game_tps
    },
    {
      .name = "Armada Construction Turret",
      .metal_cost = 210*bar_game_resource_denominator,
      .energy_cost = 3200*bar_game_resource_denominator,
      .build_cost = 5300,
      .build_power = 200/bar_game_tps,
      .required_unit = general_armada_tier1_constructor,
    },
    {
      .name = "Armada Advanced Bot Lab",
      .metal_cost = 2900*bar_game_resource_denominator,
      .energy_cost = 15000*bar_game_resource_denominator,
      .build_cost = 16200,
      .direct_build_power = 300/bar_game_tps,
      .energy_capacity = 200*bar_game_resource_denominator,
      .metal_capacity = 200*bar_game_resource_denominator,
      .required_unit = {BAR_UnitType_ArmadaConstructionBot},
    },
    {
      .name = "Armada Advanced Construction Bot",
      .metal_cost = 430*bar_game_resource_denominator,
      .energy_cost = 6900*bar_game_resource_denominator,
      .build_cost = 9500,
      .build_power = 180/bar_game_tps,
      .energy_production = 14*bar_game_resource_denominator/bar_game_tps,
      .energy_capacity = 100*bar_game_resource_denominator,
      .required_unit = {BAR_UnitType_ArmadaAdvancedBotLab},
    },
    {
      .name = "Armada Advanced Metal Extractor",
      .metal_cost = 620*bar_game_resource_denominator,
      .energy_cost = 7700*bar_game_resource_denominator,
      .build_cost = 14900,
      .metal_production = (uint32_t)(7.23*bar_game_resource_denominator/bar_game_tps),
      .metal_capacity = 600*bar_game_resource_denominator,
      .required_unit = general_armada_tier2_constructor,
      .max_count = mine_limit,
      .avg_travel_time = 280,
      .energy_upkeep = 20*bar_game_resource_denominator/bar_game_tps,
    },
    {
      .name = "Armada Advanced Energy Converter",
      .metal_cost = 380*bar_game_resource_denominator,
      .energy_cost = 21000*bar_game_resource_denominator,
      .build_cost = 35000,
      .required_unit = general_armada_tier2_constructor,
    },
    {
      .name = "Armada Hardened Metal Storage",
      .metal_cost = 750*bar_game_resource_denominator,
      .energy_cost = 11500*bar_game_resource_denominator,
      .build_cost = 20400,
      .metal_capacity = 10000*bar_game_resource_denominator,
      .required_unit = general_armada_tier2_constructor,
    },
    {
      .name = "Armada Prude",
      .metal_cost = 1150*bar_game_resource_denominator,
      .energy_cost = 25000*bar_game_resource_denominator,
      .build_cost = 41500,
      //.energy_production = 750*bar_game_resource_denominator/bar_game_tps,
      .energy_capacity = 1500*bar_game_resource_denominator,
      .required_unit = general_armada_tier2_constructor,
      .max_count = geothermal_limit,
      .avg_travel_time = 28*bar_game_tps
    },
    {
      .name = "Armada Advanced Geothermal Power Plant",
      .metal_cost = 1150*bar_game_resource_denominator,
      .energy_cost = 25000*bar_game_resource_denominator,
      .build_cost = 33300,
      .energy_production = 1250*bar_game_resource_denominator/bar_game_tps,
      .energy_capacity = 12000*bar_game_resource_denominator,
      .required_unit = general_armada_tier2_constructor,
      .max_count = geothermal_limit,
      .avg_travel_time = 28*bar_game_tps,
    },
    {
      .name = "Armada Fusion Reactor",
      .metal_cost = 4300*bar_game_resource_denominator,
      .energy_cost = 21000*bar_game_resource_denominator,
      .build_cost = 70000,
      .energy_production = 1000*bar_game_resource_denominator/bar_game_tps,
      .energy_capacity = 2500*bar_game_resource_denominator,
      .required_unit = general_armada_tier2_constructor,
    },
    {
      .name = "Armada Cloakable Fusion Reactor",
      .metal_cost = 4700*bar_game_resource_denominator,
      .energy_cost = 26000*bar_game_resource_denominator,
      .build_cost = 84400,
      //.energy_production = 1050*bar_game_resource_denominator/bar_game_tps,
      .energy_capacity = 2500*bar_game_resource_denominator,
      .required_unit = general_armada_tier2_constructor,
    },
    {
      .name = "Armada Advanced Fusion Reactor",
      .metal_cost = 9700*bar_game_resource_denominator,
      .energy_cost = 69000*bar_game_resource_denominator,
      .build_cost = 312500,
      .energy_production = 3000*bar_game_resource_denominator/bar_game_tps,
      .energy_capacity = 9000*bar_game_resource_denominator,
      .required_unit = general_armada_tier2_constructor,
    },
    {
      .name = "Armada Hardened Energy Storage",
      .metal_cost = 830*bar_game_resource_denominator,
      .energy_cost = 10000*bar_game_resource_denominator,
      .build_cost = 20300,
      .energy_capacity = 40000*bar_game_resource_denominator,
      .required_unit = general_armada_tier2_constructor,
    },
    {
      .name = "Armada Armageddon",
      .metal_cost = 8100*bar_game_resource_denominator,
      .energy_cost = 90000*bar_game_resource_denominator,
      .build_cost = 178500,
      .required_unit = general_armada_tier2_constructor,
    },
    {
      .name = "Armada Tick",
      .metal_cost = 17*bar_game_resource_denominator,
      .energy_cost = 340*bar_game_resource_denominator,
      .build_cost = 800,
      .required_unit = {BAR_UnitType_ArmadaBotLab},
    },
    {
      .name = "Armada Construction Aircraft",
      .metal_cost = 110*bar_game_resource_denominator,
      .energy_cost = 3000*bar_game_resource_denominator,
      .build_cost = 7960,
      .build_power = 60/bar_game_tps,
      .energy_production = 5*bar_game_resource_denominator/bar_game_tps,
      .energy_capacity = 25*bar_game_resource_denominator,
      .required_unit = {BAR_UnitType_ArmadaAircraftPlant},
    },
    {
      .name = "Armada Stormbringer",
      .metal_cost = 145*bar_game_resource_denominator,
      .energy_cost = 4200*bar_game_resource_denominator,
      .build_cost = 7250,
      .required_unit = {BAR_UnitType_ArmadaAircraftPlant},
    },
    {
      .name = "Armada Advanced Aircraft Plant",
      .metal_cost = 3200*bar_game_resource_denominator,
      .energy_cost = 29000*bar_game_resource_denominator,
      .build_cost = 20900,
      .required_unit = {BAR_UnitType_ArmadaConstructionAircraft},
    },
    {
      .name = "Armada Blizzard",
      .metal_cost = 230*bar_game_resource_denominator,
      .energy_cost = 12500*bar_game_resource_denominator,
      .build_cost = 21000,
      .required_unit = {BAR_UnitType_ArmadaAdvancedAircraftPlant},
    },
    {
      .name = "Armada Advanced Construction Aircraft",
      .metal_cost = 340*bar_game_resource_denominator,
      .energy_cost = 12000*bar_game_resource_denominator,
      .build_cost = 17750,
      .build_power = 100/bar_game_tps,
      .energy_production = 10*bar_game_resource_denominator/bar_game_tps,
      .energy_capacity = 50*bar_game_resource_denominator,
      .required_unit = {BAR_UnitType_ArmadaAdvancedAircraftPlant},
    },
    {
      .name = "Armada Liche",
      .metal_cost = 2200*bar_game_resource_denominator,
      .energy_cost = 46000*bar_game_resource_denominator,
      .build_cost = 57400,
      .required_unit = {BAR_UnitType_ArmadaAdvancedAircraftPlant},
    },
};

enum BAR_Instruction {
  BAR_Instruction_None,
  BAR_Instruction_Build
};


inline const char * bar_game_name() {return "Beyond All Reason";}

inline const char * bar_game_get_unit_type_name(uint32_t unit)
{
  if (unit >= BAR_UnitType_MAX)
    return "Invalid";
  return BAR_UnitType_Metadata[unit].name;
}

inline const char * bar_game_get_resource_type_name(uint32_t resource_type)
{
  if (resource_type == BAR_ResourceType_Metal)
    return "Metal";
  else if (resource_type == BAR_ResourceType_Energy)
    return "Energy";
  return "Invalid";
}

void bar_game_pretty_print_instruction(std::string indent, void* instruction_in);
uint32_t bar_game_get_unit_resource_production_rate(void* player_in, void* unit_in, uint32_t unit_type, uint32_t resource_type);
uint32_t bar_game_get_unit_resource_capacity(void* player_in, void* unit_in, uint32_t unit_type, uint32_t resource_type);
void bar_game_on_unit_allocated(void* player_in, void* unit_in, uint32_t unit_type);
void bar_game_print_detailed_unit_info(void* player_in, void* unit_in, uint32_t unit_type, std::string indent);
void bar_game_unit_pre_tick(void* player, void* unit, uint32_t unit_type);
void bar_game_unit_post_tick(void* player_in, void* unit_in, uint32_t unit_type);
bool bar_game_follow_instruction(void *player_in, void *instruction_in, bool first_tick);
void bar_game_on_player_init(void *player_in);

struct BARUnitData {
  bool is_alive = false;
  bool is_starved = false;
  uint32_t allocated_resources[BAR_ResourceType_MAX] = {0};
  uint32_t allocated_build_points = 0;
  uint32_t energy_stored = 0;
};

struct BARPlayerData {
  void* currently_building_unit;
  BAR_UnitType currently_building_unit_builder_type;
  uint32_t travel_time_spent = 0;
  uint32_t cached_total_general_build_power = 0;
};

constexpr struct GameConfig BAR_game_config = {
        .name = bar_game_name,
        .num_players = 1,
        .num_unit_types = BAR_UnitType_MAX,
        .max_num_units_of_type = bar_game_max_num_units_of_type,
        .num_resource_types = BAR_ResourceType_MAX,
        .resource_denominator = bar_game_resource_denominator,
        .get_unit_type_name = bar_game_get_unit_type_name,
        .get_resource_type_name = bar_game_get_resource_type_name,
        .pretty_print_instruction = bar_game_pretty_print_instruction,
        .get_unit_resource_production_rate = bar_game_get_unit_resource_production_rate,
        .get_unit_resource_capacity = bar_game_get_unit_resource_capacity,
        .on_unit_allocated = bar_game_on_unit_allocated,
        .game_tps = bar_game_tps,
        .unit_data_size = sizeof(BARUnitData),
        .player_data_size = sizeof(BARPlayerData),
        .print_detailed_unit_info = bar_game_print_detailed_unit_info,
        .unit_pre_tick = bar_game_unit_pre_tick,
        .unit_post_tick = bar_game_unit_post_tick,
        .follow_instruction = bar_game_follow_instruction,
        .on_player_init = bar_game_on_player_init,
};

inline void update_cached_total_general_build_power(Game<BAR_game_config>::Player* player)
{
  auto player_data = (BARPlayerData*)player->data;
  player_data->cached_total_general_build_power = 0;
  for (uint32_t unit_type = 0; unit_type < BAR_UnitType_MAX; unit_type++) {
    for (uint32_t i = 0; i < player->furthest_allocated_unit[unit_type]; i++) {
      if (player->units[unit_type][i].is_allocated) {
        player_data->cached_total_general_build_power += BAR_UnitType_Metadata[unit_type].build_power;
      }
    }
  }
}

inline Game<BAR_game_config>::Unit* bar_free_unit_build(Game<BAR_game_config>::Player* player, BAR_UnitType unit_type)
{
  auto unit = player->allocate_unit(unit_type);
  auto unit_data = (BARUnitData*)unit->data;
  unit_data->allocated_resources[BAR_ResourceType_Energy] = BAR_UnitType_Metadata[unit_type].energy_cost;
  unit_data->allocated_resources[BAR_ResourceType_Metal] = BAR_UnitType_Metadata[unit_type].metal_cost;
  unit_data->allocated_build_points = BAR_UnitType_Metadata[unit_type].build_cost;
  unit_data->is_alive = true;
  unit_data->is_starved = false;
  return unit;
}

inline void bar_game_setup(Game<BAR_game_config>* game)
{
  for (auto& player : game->players) {
    bar_free_unit_build(&player, BAR_UnitType_ArmadaCommander);
    player.resources[BAR_ResourceType_Metal] = 1000*bar_game_resource_denominator;
    player.resources[BAR_ResourceType_Energy] = 1000*bar_game_resource_denominator;
  }
}

inline void bar_game_on_player_init(void* player_in)
{
  auto player = (Game<BAR_game_config>::Player*)player_in;
  auto player_data = (BARPlayerData*)player->data;
  player_data->currently_building_unit = nullptr;
  player_data->currently_building_unit_builder_type = BAR_UnitType_None;
}

inline void bar_game_unit_pre_tick(void* player_in, void* unit_in, uint32_t unit_type)
{
  auto player = (Game<BAR_game_config>::Player*)player_in;
  auto unit = (Game<BAR_game_config>::Unit*)unit_in;
  auto unit_data = (BARUnitData*)unit->data;

  if (!unit_data->is_alive)
  {
    return;
  }

  if ((BAR_UnitType_Metadata[unit_type].metal_upkeep > 0) || (BAR_UnitType_Metadata[unit_type].energy_upkeep > 0))
  {
    // Unit upkeep cost
    unit_data->is_starved = false;
    unit_data->is_starved |= player->resources[BAR_ResourceType_Metal] < BAR_UnitType_Metadata[unit_type].metal_upkeep;
    unit_data->is_starved |= player->resources[BAR_ResourceType_Energy] < BAR_UnitType_Metadata[unit_type].energy_upkeep;
    if (!unit_data->is_starved) {
      player->resources[BAR_ResourceType_Metal] -= BAR_UnitType_Metadata[unit_type].metal_upkeep;
      player->resources[BAR_ResourceType_Energy] -= BAR_UnitType_Metadata[unit_type].energy_upkeep;
    }
  }

  // Cash out converters
  if (player->game->tick % bar_game_tps == 0)
  {
    if (unit_type == BAR_UnitType_ArmadaEnergyConverter)
    {
      if ((unit_data->energy_stored >= 70 * bar_game_resource_denominator) && (player->resources[BAR_ResourceType_Metal] < player->cached_resource_capacity[BAR_ResourceType_Metal]))
      {
        unit_data->energy_stored -= 70 * bar_game_resource_denominator;
        player->resources[BAR_ResourceType_Metal] += 1*bar_game_resource_denominator;
      }
    }
    if (unit_type == BAR_UnitType_ArmadaAdvancedEnergyConverter)
    {
      if ((unit_data->energy_stored >= 600 * bar_game_resource_denominator) && (player->resources[BAR_ResourceType_Metal] < player->cached_resource_capacity[BAR_ResourceType_Metal]))
      {
        unit_data->energy_stored -= 600 * bar_game_resource_denominator;
        player->resources[BAR_ResourceType_Metal] += 10*bar_game_resource_denominator;
      }
    }
  }
}

inline void bar_game_unit_post_tick(void* player_in, void* unit_in, uint32_t unit_type) {
  auto player = (Game<BAR_game_config>::Player *) player_in;
  auto unit = (Game<BAR_game_config>::Unit *) unit_in;
  auto unit_data = (BARUnitData *) unit->data;

  if (!unit_data->is_alive) {
    return;
  }

  // Add to power converter banks
  int32_t power_for_converters = (int32_t) player->resources[BAR_ResourceType_Energy] - (int32_t) (player->cached_resource_capacity[BAR_ResourceType_Energy] / 2);
  if (power_for_converters > 0) {
    int32_t max_allowed_contribution = 0;
    if (unit_type == BAR_UnitType_ArmadaEnergyConverter) {
      max_allowed_contribution = (int32_t) (70 * bar_game_resource_denominator) - (int32_t) unit_data->energy_stored;
    }
    if (unit_type == BAR_UnitType_ArmadaAdvancedEnergyConverter) {
      max_allowed_contribution = (int32_t) (600 * bar_game_resource_denominator) - (int32_t) unit_data->energy_stored;
    }
    if (max_allowed_contribution < power_for_converters) {
      power_for_converters = max_allowed_contribution;
    }
    unit_data->energy_stored += power_for_converters;
    player->resources[BAR_ResourceType_Energy] -= power_for_converters;
  }
}

inline void bar_game_pretty_print_instruction(std::string indent, void* instruction_in)
{
  auto instruction = (Game<BAR_game_config>::Instruction*)(instruction_in);
  auto instruction_type = (BAR_Instruction)instruction->type;
  switch (instruction_type) {
    default:
      break;
    case BAR_Instruction_Build:
      if ((instruction->data >= BAR_UnitType_MAX) || (instruction->data == BAR_UnitType_None))
      {
        printf("%sInvalid Instruction\n", indent.c_str());
        break;
      }
      printf("%sBuild %s\n", indent.c_str(), BAR_UnitType_Metadata[instruction->data].name);
      break;
  }
}

inline uint32_t bar_game_get_unit_resource_production_rate(void* player_in, void* unit_in, uint32_t unit_type, uint32_t resource_type)
{
  auto player = (Game<BAR_game_config>::Player*)player_in;
  auto unit = (Game<BAR_game_config>::Unit*)unit_in;
  auto unit_data = (BARUnitData*)unit->data;
  if (!unit_data->is_alive || unit_data->is_starved)
  {
    return 0;
  }
  if (resource_type == BAR_ResourceType_Metal)
    return BAR_UnitType_Metadata[unit_type].metal_production;
  else if (resource_type == BAR_ResourceType_Energy)
    return BAR_UnitType_Metadata[unit_type].energy_production;
  return 0;
}

inline uint32_t bar_game_get_unit_resource_capacity(void* player_in, void* unit_in, uint32_t unit_type, uint32_t resource_type)
{
  auto player = (Game<BAR_game_config>::Player*)player_in;
  auto unit = (Game<BAR_game_config>::Unit*)unit_in;
  auto unit_data = (BARUnitData*)unit->data;
  if (!unit_data->is_alive)
  {
    return 0;
  }
  if (resource_type == BAR_ResourceType_Metal)
    return BAR_UnitType_Metadata[unit_type].metal_capacity;
  else if (resource_type == BAR_ResourceType_Energy)
    return BAR_UnitType_Metadata[unit_type].energy_capacity;
  return 0;
}

inline void bar_game_on_unit_allocated(void *player_in, void *unit_in, uint32_t unit_type) {
  auto player = (Game<BAR_game_config>::Player*)player_in;
  auto unit = (Game<BAR_game_config>::Unit*)unit_in;
  auto unit_data = (BARUnitData*)unit->data;
  unit_data->is_alive = false;
  unit_data->allocated_resources[BAR_ResourceType_Metal] = 0;
  unit_data->allocated_resources[BAR_ResourceType_Energy] = 0;
  unit_data->allocated_build_points = 0;
  unit_data->energy_stored = 0;
}

inline void bar_game_print_detailed_unit_info(void *player_in, void *unit_in, uint32_t unit_type, std::string indent)
{
  auto player = (Game<BAR_game_config>::Player*)player_in;
  auto unit = (Game<BAR_game_config>::Unit*)unit_in;
  auto unit_data = (BARUnitData*)unit->data;
  printf("%s%s:", indent.c_str(), bar_game_get_unit_type_name(unit_type));
  printf(" %d build points", unit_data->allocated_build_points);
  for (uint32_t k = 0; k < BAR_ResourceType_MAX; k++)
  {
    printf(", %f %s", ((float)unit_data->allocated_resources[k])/((float)bar_game_resource_denominator), bar_game_get_resource_type_name(k));
  }
  printf("\n");
}

uint32_t bar_game_get_num_of_unit_type(Game<BAR_game_config>::Player *player, BAR_UnitType unit_type);

uint32_t bar_game_get_full_upkeep(Game<BAR_game_config>::Player *player, BAR_ResourceTypes resource_type);
// Find and reclaim a unit
inline bool bar_game_reclaim_unit_instant(Game<BAR_game_config>::Player *player, uint32_t unit_type, bool reclaim_metal, bool reclaim_energy)
{
  if (unit_type >= BAR_UnitType_MAX)
  {
    return false;
  }
  Game<BAR_game_config>::Unit *unit = nullptr;
  for (uint32_t i = 0; i < player->furthest_allocated_unit[unit_type]; i++)
  {
    if (player->units[i]->is_allocated)
    {
      auto unit_data = (BARUnitData*)player->units[i]->data;
      if (unit_data->is_alive)
      {
        unit = player->units[i];
        break;
      }
    }
  }

  if (unit)
  {
    if (reclaim_metal)
    {
      player->resources[BAR_ResourceType_Metal] += BAR_UnitType_Metadata[unit_type].metal_cost;
    }
    if (reclaim_energy)
    {
      player->resources[BAR_ResourceType_Energy] += BAR_UnitType_Metadata[unit_type].energy_cost;
    }
    player->deallocate_unit(unit, unit_type);
  }
  return unit != nullptr;
}

inline bool bar_game_follow_instruction(void *player_in, void *instruction_in, bool first_tick)
{
  auto player = (Game<BAR_game_config>::Player*)player_in;
  auto player_data = (BARPlayerData*)player->data;
  auto instruction = (Game<BAR_game_config>::Instruction*)(instruction_in);
  bool advance_instruction = false;
  switch (instruction->type) {
    default:
      // Invalid
      advance_instruction = true;
      break;
    case BAR_Instruction_Build:
    {
      auto unit_type = (BAR_UnitType) instruction->data;
      if ((unit_type >= BAR_UnitType_MAX) || (unit_type == BAR_UnitType_None)) {
        // Invalid
        advance_instruction = true;
        break;
      }

      if (first_tick) {
        update_cached_total_general_build_power(player);
        player_data->travel_time_spent = 0;
        player_data->currently_building_unit = nullptr;

        bool found_required_unit = BAR_UnitType_Metadata[unit_type].required_unit[0] == BAR_UnitType_None;
        for (auto required_unit_type : BAR_UnitType_Metadata[unit_type].required_unit)
        {
          if (required_unit_type == BAR_UnitType_None)
          {
            break;
          }
          for (uint32_t j = 0; j < player->furthest_allocated_unit[required_unit_type]; j++)
          {
            if (player->units[required_unit_type][j].is_allocated)
            {
              if (((BARUnitData*)player->units[required_unit_type][j].data)->is_alive)
              {
                player_data->currently_building_unit_builder_type = required_unit_type;
                found_required_unit = true;
                break;
              }
            }
          }
          if (found_required_unit)
          {
            break;
          }
        }
        if (!found_required_unit)
        {
          // Invalid
          advance_instruction = true;
          break;
        }
        // Check if we already built too many units (approximated here by furthest_allocated_unit)
        if (player->furthest_allocated_unit[unit_type] >= BAR_UnitType_Metadata[unit_type].max_count)
        {
          advance_instruction = true;
          break;
        }
        // Do it
        player_data->currently_building_unit = player->allocate_unit(unit_type);
      }
      if (!player_data->currently_building_unit)
      {
        // Allocation failure
        advance_instruction = true;
        break;
      }
      auto unit = (Game<BAR_game_config>::Unit*) player_data->currently_building_unit;
      auto unit_data = (BARUnitData*)unit->data;
      if (player_data->travel_time_spent < BAR_UnitType_Metadata[unit_type].avg_travel_time) {
        player_data->travel_time_spent++;
      } else {
        // Add resources
        uint32_t build_points_to_give = player_data->cached_total_general_build_power + BAR_UnitType_Metadata[player_data->currently_building_unit_builder_type].direct_build_power;
        if (BAR_UnitType_Metadata[unit_type].energy_cost > 0) {
          uint32_t build_points_allowed_by_energy =
                  ((uint64_t) player->resources[BAR_ResourceType_Energy] * (uint64_t) BAR_UnitType_Metadata[unit_type].build_cost) / (uint64_t) BAR_UnitType_Metadata[unit_type].energy_cost;
          if (build_points_to_give > build_points_allowed_by_energy) {
            build_points_to_give = build_points_allowed_by_energy;
          }
        }
        if (BAR_UnitType_Metadata[unit_type].metal_cost > 0) {
          uint32_t build_points_allowed_by_metal =
                  ((uint64_t) player->resources[BAR_ResourceType_Metal] * (uint64_t) BAR_UnitType_Metadata[unit_type].build_cost) / (uint64_t) BAR_UnitType_Metadata[unit_type].metal_cost;
          if (build_points_to_give > build_points_allowed_by_metal) {
            build_points_to_give = build_points_allowed_by_metal;
          }
        }

        unit_data->allocated_build_points += build_points_to_give;
        if (BAR_UnitType_Metadata[unit_type].build_cost > 0) {
          uint32_t energy_to_give = (BAR_UnitType_Metadata[unit_type].energy_cost * build_points_to_give) / BAR_UnitType_Metadata[unit_type].build_cost;
          player->resources[BAR_ResourceType_Energy] -= energy_to_give;
          unit_data->allocated_resources[BAR_ResourceType_Energy] += energy_to_give;
          uint32_t metal_to_give = (BAR_UnitType_Metadata[unit_type].metal_cost * build_points_to_give) / BAR_UnitType_Metadata[unit_type].build_cost;
          player->resources[BAR_ResourceType_Metal] -= metal_to_give;
          unit_data->allocated_resources[BAR_ResourceType_Metal] += metal_to_give;
        }
      }

      // Figure out if we can build this tick
      bool can_finish = true;
      if (unit_data->allocated_build_points < BAR_UnitType_Metadata[unit_type].build_cost)
        can_finish = false;
      if (unit_data->allocated_resources[BAR_ResourceType_Energy] < BAR_UnitType_Metadata[unit_type].energy_cost)
        can_finish = false;
      if (unit_data->allocated_resources[BAR_ResourceType_Metal] < BAR_UnitType_Metadata[unit_type].metal_cost)
        can_finish = false;
      if (can_finish) {
        unit_data->is_alive = true;
        // Refund extra resources
        player->resources[BAR_ResourceType_Metal] += unit_data->allocated_resources[BAR_ResourceType_Metal] - BAR_UnitType_Metadata[unit_type].metal_cost;
        unit_data->allocated_resources[BAR_ResourceType_Metal] = BAR_UnitType_Metadata[unit_type].metal_cost;
        player->resources[BAR_ResourceType_Energy] += unit_data->allocated_resources[BAR_ResourceType_Energy] - BAR_UnitType_Metadata[unit_type].energy_cost;
        unit_data->allocated_resources[BAR_ResourceType_Energy] = BAR_UnitType_Metadata[unit_type].energy_cost;
        unit_data->allocated_build_points = BAR_UnitType_Metadata[unit_type].build_cost;

        if (unit_type == BAR_UnitType_ArmadaAdvancedMetalExtractor) {
          bar_game_reclaim_unit_instant(player, BAR_UnitType_ArmadaMetalExtractor, true, false);
        }

        if (unit_type == BAR_UnitType_ArmadaAdvancedGeothermalPowerplant) {
          bar_game_reclaim_unit_instant(player, BAR_UnitType_ArmadaGeothermalPowerplant, true, false);
        }

        advance_instruction = true;
      }
      break;
    }
  }
  return advance_instruction;
}


#endif //MLX86_BAR_GAME_CONFIG_HPP
