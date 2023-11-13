//
// Created by christian on 11/10/23.
//

#ifndef MLX86_GAME_HPP
#define MLX86_GAME_HPP
#include <string>
#include <cstdint>
#include <cassert>

struct GameConfig{
  const char* (*name)();
  uint32_t num_players;
  uint32_t num_unit_types;
  uint32_t max_num_units_of_type;
  uint32_t num_resource_types;
  uint32_t resource_denominator;
  const char* (*get_unit_type_name)(uint32_t unit_type);
  const char* (*get_resource_type_name)(uint32_t resource_type);
  void (*pretty_print_instruction)(std::string indent, void* instruction);
  uint32_t (*get_unit_resource_production_rate)(void* player, void* unit, uint32_t unit_type, uint32_t resource_type);
  uint32_t (*get_unit_resource_capacity)(void* player, void* unit, uint32_t unit_type, uint32_t resource_type);
  void (*on_unit_allocated)(void *player_in, void *unit_in, uint32_t unit_type);
  uint32_t game_tps;
  uint32_t unit_data_size;
  uint32_t player_data_size;
  void (*print_detailed_unit_info)(void* player, void* unit, uint32_t unit_type, std::string indent);
  void (*unit_pre_tick)(void* player, void* unit, uint32_t unit_type);
  void (*unit_post_tick)(void* player, void* unit, uint32_t unit_type);
  bool (*follow_instruction)(void *player, void *instruction, bool first_tick);
  void (*on_player_init)(void* player);
} ;

template <struct GameConfig config> class Game {
public:

  uint32_t tick = 0;

  class Instruction {
  public:
    uint32_t type = 0;
    uint32_t data = 0;
  };

  class Unit {
  public:
    bool is_allocated = false; // If this unit memory has been allocated
    uint8_t data[config.unit_data_size] = {0};
  };

  class Player {
  public:
    uint32_t num_instructions = 0;
    Instruction* instructions = nullptr;
    uint32_t current_instruction = 0;
    bool is_instruction_first_tick = true;

    uint8_t data[config.player_data_size] = {};

    Unit units[config.num_unit_types][config.max_num_units_of_type];
    uint32_t furthest_allocated_unit[config.num_unit_types] = {};

    uint32_t resources[config.num_resource_types] = {};
    bool resource_rate_cache_dirty = true;
    uint32_t cached_resource_rates[config.num_resource_types] = {};
    bool resource_capacity_cache_dirty = true;
    uint32_t cached_resource_capacity[config.num_resource_types] = {};

    inline void update_cached_resource_rates()
    {
      for (uint32_t resource_type = 0; resource_type < config.num_resource_types; resource_type++) {
        cached_resource_rates[resource_type] = 0;
      }
      if (!config.get_unit_resource_production_rate)
        return;
      for (uint32_t unit_type = 0; unit_type < config.num_unit_types; unit_type++) {
        for (uint32_t i = 0; i < furthest_allocated_unit[unit_type]; i++) {
          if (units[unit_type][i].is_allocated) {
            for (uint32_t resource_type = 0; resource_type < config.num_resource_types; resource_type++) {
              cached_resource_rates[resource_type] += config.get_unit_resource_production_rate(this, &units[unit_type][i], unit_type, resource_type);
            }
          }
        }
      }
      resource_rate_cache_dirty = false;
    }
    inline void update_cached_resource_capacity()
    {
      for (uint32_t resource_type = 0; resource_type < config.num_resource_types; resource_type++) {
        cached_resource_capacity[resource_type] = 0;
      }
      if (!config.get_unit_resource_capacity)
        return;
      for (uint32_t unit_type = 0; unit_type < config.num_unit_types; unit_type++) {
        for (uint32_t i = 0; i < furthest_allocated_unit[unit_type]; i++) {
          if (units[unit_type][i].is_allocated) {
            for (uint32_t resource_type = 0; resource_type < config.num_resource_types; resource_type++) {
              cached_resource_capacity[resource_type] += config.get_unit_resource_capacity(this, &units[unit_type][i], unit_type, resource_type);
            }
          }
        }
      }
      resource_capacity_cache_dirty = false;
    }

    Game* game;

    explicit Player(Game* game_in): game(game_in)
    {
      if (config.on_player_init) {
        config.on_player_init(this);
      }
    }

    Unit* allocate_unit(uint32_t unit_type)
    {
      Unit* ret = nullptr;
      for (uint32_t i = 0; i < config.max_num_units_of_type; i++) {
        if (!units[unit_type][i].is_allocated) {
          if ((i+1) > furthest_allocated_unit[unit_type]) {
            furthest_allocated_unit[unit_type] = i+1;
          }

          units[unit_type][i].is_allocated = true;
          ret = &units[unit_type][i];
          break;
        }
      }
      if(ret)
      {
        if (config.on_unit_allocated)
        {
          config.on_unit_allocated(this, ret, unit_type);
        }
      }
      return ret;
    }

    void deallocate_unit(Unit* unit, uint32_t unit_type)
    {
      unit->is_allocated = false;
    }

    void print_summary(std::string indent) {
      printf("%sCurrent Instruction: ", indent.c_str());
      if (current_instruction >= num_instructions) {
        printf("End Of Instructions\n");
      }
      else if (!instructions[current_instruction].type)
      {
        printf("Invalid Instruction\n");
      }
      else
      {
        config.pretty_print_instruction(indent, &instructions[current_instruction]);
      }
      for (uint32_t i = 0; i < config.num_resource_types; i++)
      {
        printf("%s%s: %f (max: %f, prod: %f)\n", indent.c_str(), config.get_resource_type_name(i), ((float)resources[i])/((float)config.resource_denominator), ((float)cached_resource_capacity[i])/((float)config.resource_denominator), (((float)cached_resource_rates[i])/((float)config.resource_denominator))*(float)config.game_tps);
      }
      printf("%sUnits:\n", indent.c_str());
      indent = indent + " ";
      for (uint32_t i = 0; i < config.num_unit_types; i++) {
        if (true)
        {
          for (uint32_t j = 0; j < furthest_allocated_unit[i]; j++) {
            if (units[i][j].is_allocated && config.print_detailed_unit_info) {
              config.print_detailed_unit_info(this, &units[i][j], i, indent);
            }
          }
        }
        else
        {/*
          if (num_units[i])
          {
            printf("%s%s: %d\n", indent.c_str(), config.get_unit_type_name(i), num_units[i]);
          }*/
        }
      }
    }

    void print_instructions(std::string indent)
    {
      printf("%sInstructions:\n", indent.c_str());
      indent = indent + " ";
      for (uint32_t i = 0; i < num_instructions; i++)
      {
        if (instructions[i].type != 0) {
          config.pretty_print_instruction(indent, &instructions[i]);
        }
      }
    }

    void append_instruction(uint32_t instruction_type, uint32_t instruction_data)
    {
      if (instructions == nullptr)
      {
        num_instructions = 1000;
        instructions = new Instruction[num_instructions];
      }
      for (uint32_t i = 0; i < num_instructions; i++)
      {
        if (instructions[i].type == 0) {
          instructions[i].type = instruction_type;
          instructions[i].data = instruction_data;
          return;
        }
      }
    }

    void tick()
    {
      if (resource_rate_cache_dirty)
      {
        update_cached_resource_rates();
      }
      for (uint32_t resource_type = 0; resource_type < config.num_resource_types; resource_type++)
      {
        resources[resource_type] += cached_resource_rates[resource_type];
      }

      for (uint32_t unit_type = 0; unit_type < config.num_unit_types; unit_type++) {
        for (uint32_t i = 0; i < furthest_allocated_unit[unit_type]; i++) {
          if (units[unit_type][i].is_allocated) {
            if (config.unit_pre_tick) {
              config.unit_pre_tick(this, &units[unit_type][i], unit_type);
            }
          }
        }
      }

      if (instructions && current_instruction < num_instructions)
      {
        auto instruction = instructions[current_instruction];
        bool advance_instruction = false;
        if (instruction.type == 0)
        {
          advance_instruction = true;
        }
        else
        {
          if (config.follow_instruction) {
            advance_instruction = config.follow_instruction(this, &instruction, is_instruction_first_tick);
          }
        }

        is_instruction_first_tick = false;
        if (advance_instruction)
        {
          is_instruction_first_tick = true;
          current_instruction++;
        }
      }

      for (uint32_t unit_type = 0; unit_type < config.num_unit_types; unit_type++) {
        for (uint32_t i = 0; i < furthest_allocated_unit[unit_type]; i++) {
          if (units[unit_type][i].is_allocated) {
            if (config.unit_post_tick) {
              config.unit_post_tick(this, &units[unit_type][i], unit_type);
            }
          }
        }
      }

      if (resource_capacity_cache_dirty)
      {
        update_cached_resource_capacity();
      }
      for (uint32_t resource_type = 0; resource_type < config.num_resource_types; resource_type++)
      {
        if (resources[resource_type] > cached_resource_capacity[resource_type])
        {
          resources[resource_type] = cached_resource_capacity[resource_type];
        }
      }
    }
  };

  Player players[config.num_players] = {Player(this)};

  void print_summary() {
    printf("Tick %d\n", tick);
    for (uint32_t i = 0; i < config.num_players; i++) {
      printf("Player %d:\n", i);
      players[i].print_summary(" ");
    }
  }

  void do_tick()
  {
    for (auto& player : players) {
      player.tick();
    }
    tick++;
  }

};

#endif //MLX86_GAME_HPP
