//
// Created by christian on 11/10/23.
//

#ifndef MLX86_BAR_GAME_CONFIG_HPP
#define MLX86_BAR_GAME_CONFIG_HPP

#include "game.hpp"

enum BAR_Unit {
  BAR_Unit_None,
  BAR_Unit_ArmadaCommander,
  BAR_Unit_ArmadaMetalExtractor,
  BAR_Unit_ArmadaEnergyConverter,
  BAR_Unit_ArmadaMetalStorage,
  BAR_Unit_ArmadaWindTurbine,
  BAR_Unit_ArmadaSolarCollector,
  BAR_Unit_ArmadaEnergyStorage,
  BAR_Unit_ArmadaBotLab,
  BAR_Unit_ArmadaVehiclePlant,
  BAR_Unit_ArmadaHovercraftPlant,
  BAR_Unit_ArmadaAircraftPlant,
  BAR_Unit_ArmadaConstructionBot,
  BAR_Unit_ArmadaAdvancedSolarCollector,
  BAR_Unit_ArmadaGeothermalPowerplant,
  BAR_Unit_ArmadaConstructionTurret,
  BAR_Unit_ArmadaAdvancedBotLab,
  BAR_Unit_ArmadaAdvancedConstructionBot,
  BAR_Unit_ArmadaAdvancedMetalExtractor,
  BAR_Unit_ArmadaAdvancedEnergyConverter,
  BAR_Unit_ArmadaHardenedMetalStorage,
  BAR_Unit_ArmadaPrude,
  BAR_Unit_ArmadaAdvancedGeothermalPowerplant,
  BAR_Unit_FusionReactor,
  BAR_Unit_CloakableFusionReactor,
  BAR_Unit_AdvancedFusionReactor,
  BAR_Unit_HardenedEnergyStorage,
  BAR_Unit_Armageddon,
  BAR_Unit_MAX
};

enum BAR_ResourceTypes {
  BAR_ResourceType_Metal,
  BAR_ResourceType_Energy,
  BAR_ResourceType_MAX
};

class BARUnitData {
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
  BAR_Unit required_unit = BAR_Unit_None;
  uint32_t max_count = 0;
  uint32_t avg_travel_time = 0;
};

constexpr uint32_t mine_limit = 5;
constexpr uint32_t geothermal_limit = 1;
constexpr uint32_t turbine_limit = 25;

constexpr uint32_t bar_game_tps = 20;

constexpr uint32_t bar_game_resource_denominator = 1000;

constexpr BARUnitData BAR_UnitData[BAR_Unit_MAX] =
{
    {
      .name = "Invalid Unit"
    },
    {
      .name = "Armada Commander",
      // Free so it can be instantly built first
     // .metal_cost = 2700*bar_game_resource_denominator,
     // .energy_cost = 26000*bar_game_resource_denominator,
     // .build_cost = 75000,
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
      .required_unit = BAR_Unit_ArmadaCommander,
      .max_count = mine_limit,
      .avg_travel_time = 280,
    },
    {
      .name = "Armada Energy Converter",
      .metal_cost = 1*bar_game_resource_denominator,
      .energy_cost = 1150*bar_game_resource_denominator,
      .build_cost = 2600,
      .required_unit = BAR_Unit_ArmadaCommander,
    },
    {
      .name = "Armada Metal Storage",
      .metal_cost = 330*bar_game_resource_denominator,
      .energy_cost = 570*bar_game_resource_denominator,
      .build_cost = 2920,
      .metal_capacity = 3000*bar_game_resource_denominator,
      .required_unit = BAR_Unit_ArmadaCommander,
    },
    {
      .name = "Armada Wind Turbine",
      .metal_cost = 37*bar_game_resource_denominator,
      .energy_cost = 175*bar_game_resource_denominator,
      .build_cost = 1600,
      .energy_production = 14*bar_game_resource_denominator/bar_game_tps,
      .energy_capacity = (uint32_t)(0.5*bar_game_resource_denominator),
      .required_unit = BAR_Unit_ArmadaCommander,
      .max_count = turbine_limit
    },
    {
      .name = "Armada Solar Collector",
      .metal_cost = 155*bar_game_resource_denominator,
      .energy_cost = 0*bar_game_resource_denominator,
      .build_cost = 2800,
      .energy_production = 20*bar_game_resource_denominator/bar_game_tps,
      .energy_capacity = 50*bar_game_resource_denominator,
      .required_unit = BAR_Unit_ArmadaCommander,
    },
    {
      .name = "Armada Energy Storage",
      .metal_cost = 170*bar_game_resource_denominator,
      .energy_cost = 1700*bar_game_resource_denominator,
      .build_cost = 4110,
      .energy_capacity = 6000*bar_game_resource_denominator,
      .required_unit = BAR_Unit_ArmadaCommander,
    },
    {
      .name = "Armada Bot Lab",
      .metal_cost = 650*bar_game_resource_denominator,
      .energy_cost = 1200*bar_game_resource_denominator,
      .build_cost = 6500,
      .build_power = 100,
      .can_share_build_power = false,
      .energy_capacity = 100*bar_game_resource_denominator,
      .metal_capacity = 100*bar_game_resource_denominator,
      .required_unit = BAR_Unit_ArmadaCommander,
    },
    {
      .name = "Armada Vehicle Plant",
      .metal_cost = 740*bar_game_resource_denominator,
      .energy_cost = 1800*bar_game_resource_denominator,
      .build_cost = 7200,
      .direct_build_power = 100,
      .energy_capacity = 100*bar_game_resource_denominator,
      .metal_capacity = 100*bar_game_resource_denominator,
      .required_unit = BAR_Unit_ArmadaCommander,
    },
    {
      .name = "Armada Hovercraft Plant",
      .metal_cost =  900*bar_game_resource_denominator,
      .energy_cost = 3000*bar_game_resource_denominator,
      .build_cost = 11000,
      .direct_build_power = 100,
      .energy_capacity = 200*bar_game_resource_denominator,
      .metal_capacity = 200*bar_game_resource_denominator,
      .required_unit = BAR_Unit_ArmadaCommander,
    },
    {
      .name = "Armada Aircraft Plant",
      .metal_cost =  860*bar_game_resource_denominator,
      .energy_cost = 1350*bar_game_resource_denominator,
      .build_cost = 7250,
      .direct_build_power = 100,
      .energy_capacity = 100*bar_game_resource_denominator,
      .metal_capacity = 100*bar_game_resource_denominator,
      .required_unit = BAR_Unit_ArmadaCommander,
    },
    {
            .name = "Armada Construction Bot",
            .metal_cost = 110*bar_game_resource_denominator,
            .energy_cost = 1600*bar_game_resource_denominator,
            .build_cost = 3450,
            .build_power = 80/bar_game_tps,
            .energy_production = 7*bar_game_resource_denominator/bar_game_tps,
            .required_unit = BAR_Unit_ArmadaBotLab
    },
    {
      .name = "Armada Advanced Solar Collector",
      .metal_cost = 370*bar_game_resource_denominator,
      .energy_cost = 5000*bar_game_resource_denominator,
      .build_cost = 7950,
      .energy_production = 75*bar_game_resource_denominator/bar_game_tps,
      .energy_capacity = 100*bar_game_resource_denominator,
      .required_unit = BAR_Unit_ArmadaConstructionBot,
    },
    {
      .name = "Armada Geothermal Power plant",
      .metal_cost = 360*bar_game_resource_denominator,
      .energy_cost = 13000*bar_game_resource_denominator,
      .build_cost = 13100,
      .energy_production = 300*bar_game_resource_denominator/bar_game_tps,
      .energy_capacity = 1000*bar_game_resource_denominator,
      .required_unit = BAR_Unit_ArmadaConstructionBot,
      .max_count = geothermal_limit,
    },
    {
      .name = "Armada Construction Turret",
      .metal_cost = 210*bar_game_resource_denominator,
      .energy_cost = 3200*bar_game_resource_denominator,
      .build_cost = 5300,
      .build_power = 200/bar_game_tps,
      .required_unit = BAR_Unit_ArmadaConstructionBot,
    },
    {
      .name = "Armada Advanced Bot Lab",
      .metal_cost = 2900*bar_game_resource_denominator,
      .energy_cost = 15000*bar_game_resource_denominator,
      .build_cost = 16200,
      .direct_build_power = 300/bar_game_tps,
      .energy_capacity = 200*bar_game_resource_denominator,
      .metal_capacity = 200*bar_game_resource_denominator,
      .required_unit = BAR_Unit_ArmadaConstructionBot,
    },
    {
      .name = "Armada Advanced Construction Bot",
      .metal_cost = 430*bar_game_resource_denominator,
      .energy_cost = 6900*bar_game_resource_denominator,
      .build_cost = 9500,
      .build_power = 180/bar_game_tps,
      .energy_production = 14*bar_game_resource_denominator/bar_game_tps,
      .energy_capacity = 100*bar_game_resource_denominator,
      .required_unit = BAR_Unit_ArmadaAdvancedBotLab,
    },
    {
      .name = "Armada Advanced Metal Extractor",
      .metal_cost = 620*bar_game_resource_denominator,
      .energy_cost = 7700*bar_game_resource_denominator,
      .build_cost = 14900,
      .metal_production = (uint32_t)(7.23*bar_game_resource_denominator/bar_game_tps),
      .metal_capacity = 600*bar_game_resource_denominator,
      .required_unit = BAR_Unit_ArmadaAdvancedConstructionBot,
      .max_count = mine_limit,
    },
    {
      .name = "Armada Advanced Energy Converter",
      .metal_cost = 380*bar_game_resource_denominator,
      .energy_cost = 21000*bar_game_resource_denominator,
      .build_cost = 35000,
      .required_unit = BAR_Unit_ArmadaAdvancedConstructionBot,
    },
    {
      .name = "Armada Hardened Metal Storage",
      .metal_cost = 750*bar_game_resource_denominator,
      .energy_cost = 11500*bar_game_resource_denominator,
      .build_cost = 20400,
      .metal_capacity = 10000*bar_game_resource_denominator,
      .required_unit = BAR_Unit_ArmadaAdvancedConstructionBot,
    },
    {
      .name = "Armada Prude",
      .metal_cost = 1150*bar_game_resource_denominator,
      .energy_cost = 25000*bar_game_resource_denominator,
      .build_cost = 41500,
      .energy_production = 750*bar_game_resource_denominator/bar_game_tps,
      .energy_capacity = 1500*bar_game_resource_denominator,
      .required_unit = BAR_Unit_ArmadaAdvancedConstructionBot,
    },
    {
      .name = "Armada Advanced Geothermal Power Plant",
      .metal_cost = 1150*bar_game_resource_denominator,
      .energy_cost = 25000*bar_game_resource_denominator,
      .build_cost = 33300,
      .energy_production = 1250*bar_game_resource_denominator/bar_game_tps,
      .energy_capacity = 12000*bar_game_resource_denominator,
      .required_unit = BAR_Unit_ArmadaAdvancedConstructionBot,
    },
    {
      .name = "Armada Fusion Reactor",
      .metal_cost = 4300*bar_game_resource_denominator,
      .energy_cost = 21000*bar_game_resource_denominator,
      .build_cost = 70000,
      .energy_production = 1000*bar_game_resource_denominator/bar_game_tps,
      .energy_capacity = 2500*bar_game_resource_denominator,
      .required_unit = BAR_Unit_ArmadaAdvancedConstructionBot,
    },
    {
      .name = "Armada Cloakable Fusion Reactor",
      .metal_cost = 4700*bar_game_resource_denominator,
      .energy_cost = 26000*bar_game_resource_denominator,
      .build_cost = 84400,
      .energy_production = 1050*bar_game_resource_denominator/bar_game_tps,
      .energy_capacity = 2500*bar_game_resource_denominator,
      .required_unit = BAR_Unit_ArmadaAdvancedConstructionBot,
    },
    {
      .name = "Armada Advanced Fusion Reactor",
      .metal_cost = 9700*bar_game_resource_denominator,
      .energy_cost = 69000*bar_game_resource_denominator,
      .build_cost = 312500,
      .energy_production = 3000*bar_game_resource_denominator/bar_game_tps,
      .energy_capacity = 9000*bar_game_resource_denominator,
      .required_unit = BAR_Unit_ArmadaAdvancedConstructionBot,
    },
    {
      .name = "Hardened Energy Storage",
      .metal_cost = 830*bar_game_resource_denominator,
      .energy_cost = 10000*bar_game_resource_denominator,
      .build_cost = 20300,
      .energy_capacity = 40000*bar_game_resource_denominator,
      .required_unit = BAR_Unit_ArmadaAdvancedConstructionBot,
    },
    {
      .name = "Armageddon",
      .metal_cost = 8100*bar_game_resource_denominator,
      .energy_cost = 90000*bar_game_resource_denominator,
      .build_cost = 178500,
      .required_unit = BAR_Unit_ArmadaAdvancedConstructionBot,
    },
};

enum BAR_Instruction {
  BAR_Instruction_None,
  BAR_Instruction_Build
};


inline const char * bar_game_name() {return "Beyond All Reason";}

inline const char * bar_game_get_unit_name(uint32_t unit)
{
  if (unit >= BAR_Unit_MAX)
    return "Invalid";
  return BAR_UnitData[unit].name;
}

inline uint32_t bar_game_get_unit_resource_cost(uint32_t unit_type, uint32_t resource_type)
{
  if (unit_type >= BAR_Unit_MAX)
    return 0;
  if (resource_type >= BAR_ResourceType_MAX)
    return 0;
  if (resource_type == BAR_ResourceType_Metal)
    return BAR_UnitData[unit_type].metal_cost;
  else if (resource_type == BAR_ResourceType_Energy)
    return BAR_UnitData[unit_type].energy_cost;
  return 0;
}

inline const char * bar_game_get_resource_type_name(uint32_t resource_type)
{
  if (resource_type == BAR_ResourceType_Metal)
    return "Metal";
  else if (resource_type == BAR_ResourceType_Energy)
    return "Energy";
  return "Invalid";
}

inline uint32_t bar_game_get_start_resource_count(uint32_t resource_type)
{
  if (resource_type == BAR_ResourceType_Metal)
    return 1000*bar_game_resource_denominator;
  else if (resource_type == BAR_ResourceType_Energy)
    return 1000*bar_game_resource_denominator;
  return 0;
}

void bar_game_tick(void* game_in);
void bar_game_pretty_print_instruction(std::string indent, void* instruction_in);

constexpr struct GameConfig BAR_game_config = {
        bar_game_name,
        1,
        BAR_Unit_MAX,
        32,
        BAR_ResourceType_MAX,
        bar_game_resource_denominator,
        bar_game_get_unit_name,
        bar_game_get_unit_resource_cost,
        bar_game_get_resource_type_name,
        bar_game_get_start_resource_count,
        bar_game_pretty_print_instruction,
        bar_game_tick
};

inline void bar_game_pretty_print_instruction(std::string indent, void* instruction_in)
{
  auto instruction = (Game<BAR_game_config>::Instruction*)(instruction_in);
  auto instruction_type = (BAR_Instruction)instruction->type;
  switch (instruction_type) {
    default:
      break;
    case BAR_Instruction_Build:
      if ((instruction->data >= BAR_Unit_MAX) || (instruction->data == BAR_Unit_None))
      {
        printf("%sInvalid Instruction\n", indent.c_str());
        break;
      }
      printf("%sBuild %s\n", indent.c_str(), BAR_UnitData[instruction->data].name);
      break;
  }
}

inline void bar_game_tick(void* game_in)
{
  auto game = (Game<BAR_game_config>*)game_in;
  for (auto & player : game->players)
  {
    // General game tick stuff
    for (uint32_t i = 0; i < BAR_game_config.num_resource_types; i++)
    {
      player.resources[i] += player.total_resource_rates[i];
    }

    // Cash out converters
    if ((game->tick % bar_game_tps == 0) && (player.resources[BAR_ResourceType_Metal] < player.total_resource_capacity[BAR_ResourceType_Metal]))
    {
      uint32_t basic_converter_operations = player.converter_bank/(70*bar_game_resource_denominator);
      player.converter_bank -= basic_converter_operations*(70*bar_game_resource_denominator);
      player.resources[BAR_ResourceType_Metal] += basic_converter_operations*(1*bar_game_resource_denominator);
      uint32_t advanced_converter_operations = player.advanced_converter_bank/(600*bar_game_resource_denominator);
      player.advanced_converter_bank -= advanced_converter_operations*(600*bar_game_resource_denominator);
      player.resources[BAR_ResourceType_Metal] += advanced_converter_operations*(10*bar_game_resource_denominator);
    }

    if (player.current_instruction < player.num_instructions)
    {
      auto current_instruction = player.instructions[player.current_instruction];
      bool advance_instruction = false;

      switch (current_instruction.type) {
        default:
          // Invalid
          advance_instruction = true;
          break;
        case BAR_Instruction_Build:
          if ((current_instruction.data >= BAR_Unit_MAX) || (current_instruction.data == BAR_Unit_None))
          {
            // Invalid
            advance_instruction = true;
          }
          else
          {
            auto unit_type = (BAR_Unit)current_instruction.data;
            if (BAR_UnitData[unit_type].required_unit != BAR_Unit_None &&
                player.num_units[BAR_UnitData[unit_type].required_unit] == 0)
            {
              // Invalid (do not have prerequisite unit)
              advance_instruction = true;
            }
            else if (BAR_UnitData[unit_type].max_count && BAR_UnitData[unit_type].max_count <= player.num_units[unit_type])
            {
              // Too many units
              advance_instruction = true;
            }
            else if (player.num_units[unit_type] >= BAR_game_config.max_num_units_of_type)
            {
              // Too many units
              advance_instruction = true;
            }
            else
            {
              auto unit = &player.units[unit_type][0];
              if (player.num_units[unit_type] < BAR_game_config.max_num_units_of_type)
              {
                unit = &player.units[unit_type][player.num_units[unit_type]];
              }

              if (player.travel_time_spent < BAR_UnitData[unit_type].avg_travel_time)
              {
                player.travel_time_spent++;
              } else
              {
                // Add resources
                uint32_t build_points_to_give = player.total_build_power + BAR_UnitData[BAR_UnitData[unit_type].required_unit].direct_build_power;
                if (BAR_UnitData[unit_type].energy_cost > 0)
                {
                  uint32_t build_points_allowed_by_energy = ((uint64_t)player.resources[BAR_ResourceType_Energy]*(uint64_t)BAR_UnitData[unit_type].build_cost) / (uint64_t)BAR_UnitData[unit_type].energy_cost;
                  if (build_points_to_give > build_points_allowed_by_energy)
                  {
                    build_points_to_give = build_points_allowed_by_energy;
                  }
                }
                if (BAR_UnitData[unit_type].metal_cost > 0)
                {
                  uint32_t build_points_allowed_by_metal = ((uint64_t)player.resources[BAR_ResourceType_Metal]*(uint64_t)BAR_UnitData[unit_type].build_cost) / (uint64_t)BAR_UnitData[unit_type].metal_cost;
                  if (build_points_to_give > build_points_allowed_by_metal)
                  {
                    build_points_to_give = build_points_allowed_by_metal;
                  }
                }

                unit->build_points += build_points_to_give;
                if (BAR_UnitData[unit_type].build_cost > 0)
                {
                  uint32_t energy_to_give = (BAR_UnitData[unit_type].energy_cost*build_points_to_give)/BAR_UnitData[unit_type].build_cost;
                  player.resources[BAR_ResourceType_Energy] -= energy_to_give;
                  unit->resources[BAR_ResourceType_Energy] += energy_to_give;
                  uint32_t metal_to_give = (BAR_UnitData[unit_type].metal_cost*build_points_to_give)/BAR_UnitData[unit_type].build_cost;
                  player.resources[BAR_ResourceType_Metal] -= metal_to_give;
                  unit->resources[BAR_ResourceType_Metal] += metal_to_give;
                }
              }

              // Figure out if we can build this tick
              bool can_finish = true;
              if (unit->build_points < BAR_UnitData[unit_type].build_cost)
                can_finish = false;
              if (unit->resources[BAR_ResourceType_Energy] < BAR_UnitData[unit_type].energy_cost)
                can_finish = false;
              if (unit->resources[BAR_ResourceType_Metal] < BAR_UnitData[unit_type].metal_cost)
                can_finish = false;
              if (can_finish)
              {
                player.travel_time_spent = 0;
                unit->is_alive = true;
                // Refund extra resources
                player.resources[BAR_ResourceType_Metal] += unit->resources[BAR_ResourceType_Metal] - BAR_UnitData[unit_type].metal_cost;
                unit->resources[BAR_ResourceType_Metal] = BAR_UnitData[unit_type].metal_cost;
                player.resources[BAR_ResourceType_Energy] += unit->resources[BAR_ResourceType_Energy] - BAR_UnitData[unit_type].energy_cost;
                unit->resources[BAR_ResourceType_Energy] = BAR_UnitData[unit_type].energy_cost;
                // Add income and capacity
                player.total_build_power += BAR_UnitData[unit_type].build_power;
                player.total_resource_rates[BAR_ResourceType_Metal] += BAR_UnitData[unit_type].metal_production;
                player.total_resource_rates[BAR_ResourceType_Energy] += BAR_UnitData[unit_type].energy_production;
                player.total_resource_capacity[BAR_ResourceType_Energy] += BAR_UnitData[unit_type].energy_capacity;
                player.total_resource_capacity[BAR_ResourceType_Metal] += BAR_UnitData[unit_type].metal_capacity;

                if (unit_type == BAR_Unit_ArmadaAdvancedMetalExtractor)
                {
                  if (player.num_units[BAR_Unit_ArmadaAdvancedMetalExtractor] < player.num_units[BAR_Unit_ArmadaMetalExtractor])
                  {
                    // Deduct old metal extractor power
                    player.total_resource_rates[BAR_ResourceType_Metal] -= BAR_UnitData[BAR_Unit_ArmadaMetalExtractor].metal_production;
                    player.total_resource_rates[BAR_ResourceType_Energy] -= BAR_UnitData[BAR_Unit_ArmadaMetalExtractor].energy_production;
                    player.total_resource_capacity[BAR_ResourceType_Energy] -= BAR_UnitData[BAR_Unit_ArmadaMetalExtractor].energy_capacity;
                    player.total_resource_capacity[BAR_ResourceType_Metal] -= BAR_UnitData[BAR_Unit_ArmadaMetalExtractor].metal_capacity;
                  }
                }

                player.num_units[unit_type]++;
                advance_instruction = true;
              }
            }
          }
          break;
      }

      player.is_new_instruction = false;
      if (advance_instruction)
      {
        player.is_new_instruction = true;
        player.current_instruction++;
      }
    }

    // Add to power converter banks
    int32_t power_for_converters = player.resources[BAR_ResourceType_Energy] - (player.total_resource_capacity[BAR_ResourceType_Energy]/2);
    if (power_for_converters > 0)
    {
      uint32_t max_advanced_converter_bank = player.num_units[BAR_Unit_ArmadaAdvancedEnergyConverter]*600*bar_game_resource_denominator;
      uint32_t max_to_add = max_advanced_converter_bank - player.advanced_converter_bank;
      if (power_for_converters < max_to_add)
      {
        max_to_add = power_for_converters;
      }
      player.advanced_converter_bank += max_to_add;
      power_for_converters = (int32_t)(power_for_converters - max_to_add);
      player.resources[BAR_ResourceType_Energy] -= max_to_add;
    }

    if (power_for_converters > 0)
    {
      uint32_t max_basic_converter_bank = player.num_units[BAR_Unit_ArmadaEnergyConverter]*70*bar_game_resource_denominator;
      uint32_t max_to_add = max_basic_converter_bank - player.converter_bank;
      if (power_for_converters < max_to_add)
      {
        max_to_add = power_for_converters;
      }
      player.converter_bank += max_to_add;
      power_for_converters = (int32_t)(power_for_converters - max_to_add);
      player.resources[BAR_ResourceType_Energy] -= max_to_add;
    }

    for (uint32_t i = 0; i < BAR_game_config.num_resource_types; i++)
    {
      if (player.resources[i] > player.total_resource_capacity[i])
      {
        player.resources[i] = player.total_resource_capacity[i];
      }
    }
  }

}

#endif //MLX86_BAR_GAME_CONFIG_HPP
