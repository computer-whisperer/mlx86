//
// Created by christian on 11/10/23.
//

#ifndef MLX86_GAME_HPP
#define MLX86_GAME_HPP
#include <string>
#include <cstdint>
#include <cassert>

template<std::size_t N, class F, std::size_t START = 0>
inline void repeat(const F &f) {
  if constexpr (N == 0) {
    return;
  } else {
    f(START);
    repeat<N - 1, F, START + 1>(f);
  }
}

template <typename GameConfig> class Game {
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
    GameConfig::UnitData data = {0};
  };

  class Player {
  public:
    uint32_t num_instructions = 0;
    Instruction* instructions = nullptr;
    uint32_t current_instruction = 0;
    bool is_instruction_first_tick = true;

    GameConfig::PlayerData data;

    Unit units[GameConfig::num_unit_types][GameConfig::max_num_units_of_type];
    uint32_t furthest_allocated_unit[GameConfig::num_unit_types] = {};

    uint32_t resources[GameConfig::num_resource_types] = {};
    bool resource_rate_cache_dirty = true;
    uint32_t cached_resource_rates[GameConfig::num_resource_types] = {};
    bool resource_capacity_cache_dirty = true;
    uint32_t cached_resource_capacity[GameConfig::num_resource_types] = {};

    uint32_t base_resource_capactiy[GameConfig::num_resource_types] = {};

    template <uint32_t unit_type=GameConfig::num_unit_types-1, uint32_t resource_type=GameConfig::num_resource_types-1> inline void resource_rate_iterator()
    {
      for (uint32_t i = 0; i < furthest_allocated_unit[unit_type]; i++) {
        if (units[unit_type][i].is_allocated) {
          cached_resource_rates[resource_type] += GameConfig::template get_unit_resource_production_rate<unit_type, resource_type>(this, &units[unit_type][i]);
        }
      }
      if constexpr (resource_type == 0) {
        if constexpr (unit_type == 0) {
          return;
        }
        else
        {
          resource_rate_iterator<unit_type - 1, GameConfig::num_resource_types-1>();
        }
      }
      else {
        resource_rate_iterator<unit_type, resource_type - 1>();
      }
    }

    void update_cached_resource_rates()
    {
      for (uint32_t resource_type = 0; resource_type < GameConfig::num_resource_types; resource_type++) {
        cached_resource_rates[resource_type] = 0;
      }
      resource_rate_iterator();
      resource_rate_cache_dirty = false;
    }

    template <uint32_t unit_type=GameConfig::num_unit_types-1, uint32_t resource_type=GameConfig::num_resource_types-1> inline void resource_capacity_iterator()
    {
      for (uint32_t i = 0; i < furthest_allocated_unit[unit_type]; i++) {
        if (units[unit_type][i].is_allocated) {
          cached_resource_capacity[resource_type] += GameConfig::template get_unit_resource_capacity<unit_type, resource_type>(this, &units[unit_type][i]);
        }
      }
      if constexpr (resource_type == 0) {
        if constexpr (unit_type == 0) {
          return;
        }
        else
        {
          resource_capacity_iterator<unit_type - 1, GameConfig::num_resource_types-1>();
        }
      }
      else {
        resource_capacity_iterator<unit_type, resource_type - 1>();
      }
    }

    void update_cached_resource_capacity()
    {
      for (uint32_t resource_type = 0; resource_type < GameConfig::num_resource_types; resource_type++) {
        cached_resource_capacity[resource_type] = base_resource_capactiy[resource_type];
      }
      resource_capacity_iterator();
      resource_capacity_cache_dirty = false;
    }

    Game* game;

    explicit Player(Game* game_in): game(game_in)
    {
      GameConfig::on_player_init(this);
    }

    Unit* allocate_unit(uint32_t unit_type)
    {
      Unit* ret = nullptr;
      for (uint32_t i = 0; i < GameConfig::max_num_units_of_type; i++) {
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
        GameConfig::on_unit_allocated(this, ret, unit_type);
      }
      resource_capacity_cache_dirty = true;
      resource_rate_cache_dirty = true;
      return ret;
    }

    void deallocate_unit(Unit* unit, uint32_t unit_type)
    {
      unit->is_allocated = false;
      resource_capacity_cache_dirty = true;
      resource_rate_cache_dirty = true;
      GameConfig::on_unit_deallocated(this, unit, unit_type);
    }

    void print_summary(std::string indent, bool detailed_mode) {
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
        GameConfig::pretty_print_instruction(indent, &instructions[current_instruction]);
      }
      for (uint32_t i = 0; i < GameConfig::num_resource_types; i++)
      {
        printf("%s%s: %f (max: %f, prod: %f)\n", indent.c_str(), GameConfig::get_resource_type_name(i), ((float)resources[i]) / ((float)GameConfig::resource_denominator), ((float)cached_resource_capacity[i]) / ((float)GameConfig::resource_denominator), (((float)cached_resource_rates[i]) / ((float)GameConfig::resource_denominator)) * (float)GameConfig::game_tps);
      }
      printf("%sUnits:\n", indent.c_str());
      indent = indent + " ";

      repeat<GameConfig::num_unit_types>([&](std::size_t x){
          if (detailed_mode)
          {
            for (uint32_t j = 0; j < furthest_allocated_unit[x]; j++) {
              if (units[x][j].is_allocated) {
                GameConfig::print_detailed_unit_info(this, &units[x][j], x, indent);
              }
            }
          }
          else
          {
            uint32_t num_units = 0;
            for (uint32_t j = 0; j < furthest_allocated_unit[x]; j++) {
              if (units[x][j].is_allocated) {
                num_units++;
              }
            }
            if (num_units)
            {
              printf("%s%s: %d\n", indent.c_str(), GameConfig::get_unit_type_name(x), num_units);
            }
          }
        }
      );
    }

    void print_instructions(std::string indent)
    {
      printf("%sInstructions:\n", indent.c_str());
      indent = indent + " ";
      for (uint32_t i = 0; i < num_instructions; i++)
      {
        if (instructions[i].type != 0) {
          GameConfig::pretty_print_instruction(indent, &instructions[i]);
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

    template <uint32_t unit_type=GameConfig::num_unit_types-1> inline void pre_tick_iterator()
    {
      for (uint32_t i = 0; i < furthest_allocated_unit[unit_type]; i++) {
        if (units[unit_type][i].is_allocated) {
          GameConfig::template unit_pre_tick<unit_type>(this, &units[unit_type][i]);
        }
      }
      if constexpr (unit_type == 0) {
        return;
      }
      else {
        pre_tick_iterator<unit_type - 1>();
      }
    }

    template <uint32_t unit_type=GameConfig::num_unit_types-1> inline void post_tick_iterator()
    {
      for (uint32_t i = 0; i < furthest_allocated_unit[unit_type]; i++) {
        if (units[unit_type][i].is_allocated) {
          GameConfig::template unit_post_tick<unit_type>(this, &units[unit_type][i]);
        }
      }
      if constexpr (unit_type == 0) {
        return;
      }
      else {
        post_tick_iterator<unit_type - 1>();
      }
    }

    void tick()
    {
      if (resource_rate_cache_dirty)
      {
        update_cached_resource_rates();
      }
      for (uint32_t resource_type = 0; resource_type < GameConfig::num_resource_types; resource_type++)
      {
        resources[resource_type] += cached_resource_rates[resource_type];
      }

      pre_tick_iterator();

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
          advance_instruction = GameConfig::follow_instruction(this, &instruction, is_instruction_first_tick);
        }

        is_instruction_first_tick = false;
        if (advance_instruction)
        {
          is_instruction_first_tick = true;
          current_instruction++;
        }
      }

      post_tick_iterator();

      if (resource_capacity_cache_dirty)
      {
        update_cached_resource_capacity();
      }
      for (uint32_t resource_type = 0; resource_type < GameConfig::num_resource_types; resource_type++)
      {
        if (resources[resource_type] > cached_resource_capacity[resource_type])
        {
          resources[resource_type] = cached_resource_capacity[resource_type];
        }
      }
    }
  };

  Player players[GameConfig::num_players] = {Player(this)};

  void print_summary(bool detailed_mode) {
    printf("Tick %d\n", tick);
    for (uint32_t i = 0; i < GameConfig::num_players; i++) {
      printf("Player %d:\n", i);
      players[i].print_summary(" ", detailed_mode);
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
