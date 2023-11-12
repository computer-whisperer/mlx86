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
  uint32_t (*get_unit_resource_cost)(uint32_t unit_type, uint32_t resource_type);
  const char* (*get_resource_type_name)(uint32_t resource_type);
  uint32_t (*get_start_resource_count)(uint32_t resource_type);
  void (*pretty_print_instruction)(std::string indent, void* instruction);
  void (*game_tick)(void* game);
} ;



template <struct GameConfig config> class Game {
public:

  Game()= default;

  uint32_t tick = 0;

  class Instruction {
  public:
    uint32_t type = 0;
    uint32_t data = 0;
  };

  class Unit {
  public:
    uint32_t build_points = 0;
    bool is_alive = false;
    uint32_t resources[config.num_resource_types] = {0};
  };

  class Player {
  public:
    uint32_t num_instructions = 0;
    Instruction* instructions = nullptr;
    uint32_t current_instruction = 0;
    bool is_new_instruction = true;
    uint64_t total_build_power = 0;
    uint32_t travel_time_spent = 0;
    uint32_t converter_bank = 0;
    uint32_t advanced_converter_bank = 0;

    Unit units[config.num_unit_types][config.max_num_units_of_type];
    uint32_t num_units[config.num_unit_types] = {0};

    uint32_t resources[config.num_resource_types];
    uint32_t total_resource_rates[config.num_resource_types];
    uint32_t total_resource_upkeep[config.num_resource_types];
    uint32_t total_resource_capacity[config.num_resource_types];

    void reset()
    {
      total_build_power = 0;
      is_new_instruction = true;
      current_instruction = 0;
      for (uint32_t i = 0; i < config.num_unit_types; i++) {
        for (uint32_t j = 0; j < config.max_num_units_of_type; j++) {
          units[i][j].is_alive = false;
          units[i][j].build_points = 0;
          for (uint32_t k = 0; k < config.num_resource_types; k++) {
            units[i][j].resources[k] = 0;
          }
        }
      }
      for (uint32_t i = 0; i < config.num_resource_types; i++) {
        resources[i] = config.get_start_resource_count(i);
        total_resource_rates[i] = 0;
        total_resource_capacity[i] = 0;
        total_resource_upkeep[i] = 0;
      }
    }

    Player()
    {
      reset();
    }

    void spawn_unit(uint32_t unit_type){
      for (uint32_t i = 0; i < config.max_num_units_of_type; i++) {
        if (!units[unit_type][i].is_alive) {
          units[unit_type][i].is_alive = true;
          return;
        }
      }
    }

    void print_summary(std::string indent) {
      printf("%sCurrent Instruction: ", indent.c_str());
      if (current_instruction >= num_instructions) {
        printf("End Of Instructions\n");
      }
      else
      {
        config.pretty_print_instruction(indent, &instructions[current_instruction]);
      }
      for (uint32_t i = 0; i < config.num_resource_types; i++)
      {
        printf("%s%s: %f (max: %f, prod: %f)\n", indent.c_str(), config.get_resource_type_name(i), ((float)resources[i])/((float)config.resource_denominator), ((float)total_resource_capacity[i])/((float)config.resource_denominator), ((float)total_resource_rates[i])/((float)config.resource_denominator));
      }
      printf("%sUnits:\n", indent.c_str());
      indent = indent + " ";
      for (uint32_t i = 0; i < config.num_unit_types; i++) {
        if (false)
        {
          for (uint32_t j = 0; j < config.max_num_units_of_type; j++) {
            if (units[i][j].is_alive || units[i][j].build_points > 0) {
              printf("%s%s:", indent.c_str(), config.get_unit_type_name(i));
              printf(" %d build points", units[i][j].build_points);
              for (uint32_t k = 0; k < config.num_resource_types; k++)
              {
                printf(", %f %s", ((float)units[i][j].resources[k])/((float)config.resource_denominator), config.get_resource_type_name(k));
              }
              printf("\n");
            }
          }
        }
        else
        {
          if (num_units[i])
          {
            printf("%s%s: %d\n", indent.c_str(), config.get_unit_type_name(i), num_units[i]);
          }
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
          is_new_instruction = true;
          return;
        }
      }
    }
  };

  Player players[config.num_players];

  void reset()
  {
    tick = 0;
    for (uint32_t i = 0; i < config.num_players; i++) {
      players[i].reset();
    }
  }

  void spawn_unit(uint32_t player_id, uint32_t unit_type){
    players[player_id].spawn_unit(unit_type);
  }

  void print_summary() {
    printf("Tick %d\n", tick);
    for (uint32_t i = 0; i < config.num_players; i++) {
      printf("Player %d:\n", i);
      players[i].print_summary(" ");
    }
  }


  void do_tick()
  {
    if (!players[0].instructions)
    {
      return;
    }
    config.game_tick(this);
    tick++;
  }

};

#endif //MLX86_GAME_HPP
