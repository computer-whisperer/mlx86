//
// Created by christian on 11/10/23.
//

#ifndef MLX86_GAME_HPP
#define MLX86_GAME_HPP
#include <string>
#include <cstdint>
#include <cassert>


enum BAR_Faction{
  BAR_Faction_Cortex,
  BAR_Faction_Armada
};

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
  GameConfig::GameData data;

  uint32_t tick = 0;

  class Instruction {
  public:
    uint32_t type = 0;
    uint32_t data = 0;
    uint32_t iterations = 0;
    int32_t resource_index = 0;
    int32_t operating_unit_index = 0;
  };

  class Unit {
  public:
    int32_t position_x = 0;
    int32_t position_z = 0;
    bool is_allocated = false; // If this unit memory has been allocated
    GameConfig::UnitData data = {0};
  };

  class Player {
  public:
    bool play_by_play = false;
    bool is_allocated = false; // If this player is used
    uint32_t num_instructions = 0;
    Instruction* instructions = nullptr;
    uint32_t current_instruction = 0;
    uint32_t current_iteration = 0;
    uint32_t invalid_command_count = 0;
    bool is_instruction_first_tick = true;

    GameConfig::PlayerData data;

    Unit units_by_type[GameConfig::num_unit_types][GameConfig::max_num_units_of_type];
    uint32_t furthest_allocated_unit[GameConfig::num_unit_types] = {};
    static constexpr uint32_t num_enable_categories = 8;
    bool is_unit_set_enabled[num_enable_categories] = {false};

    Unit* allocated_units[GameConfig::max_num_units] = {};
    uint32_t allocated_unit_types[GameConfig::max_num_units] = {};
    uint32_t num_allocated_units = 0;

    uint32_t resources[GameConfig::num_resource_types] = {};

    uint32_t resource_production_rate_cache[GameConfig::num_resource_types] = {};
    bool resource_production_rate_cache_dirty = true;

    uint32_t resource_max_capacity_cache[GameConfig::num_resource_types] = {};
    bool resource_max_capacity_cache_dirty = true;

    uint32_t base_resource_capacity[GameConfig::num_resource_types] = {};


    template <typename inner_wrapper, uint32_t unit_type=0> inline void unit_iterator_by_type()
    {
      if constexpr (unit_type < GameConfig::num_unit_types) {
        uint8_t category = unit_type*num_enable_categories/GameConfig::num_unit_types;
        if (is_unit_set_enabled[category]){
          for (uint32_t i = 0; i < furthest_allocated_unit[unit_type]; i++) {
            if (units_by_type[unit_type][i].is_allocated) {
              inner_wrapper::template inner_iterator<unit_type>(this, &units_by_type[unit_type][i], unit_type);
            }
          }
        }

        unit_iterator_by_type<inner_wrapper, unit_type+1>();;
      }
    }

    template <typename inner_wrapper> class IteratorLookupGenerator
    {
      typedef void(*InnerIterator)(Game<GameConfig>::Player* player, Game<GameConfig>::Unit* unit, uint32_t unit_type);
      template<uint32_t unit_type=0> constexpr void inner_iterator_populator()
      {
        if constexpr (unit_type < GameConfig::num_unit_types) {
          inner_iterators[unit_type] = inner_wrapper::template inner_iterator<unit_type>;
          inner_iterator_populator<unit_type+1>();
        }
      }
      public:
      InnerIterator inner_iterators[GameConfig::num_unit_types] = {nullptr};
      constexpr IteratorLookupGenerator() {
        inner_iterator_populator();
      }
    };

    template <typename inner_wrapper> inline void unit_iterator_by_unit_lookup()
    {
      static IteratorLookupGenerator<inner_wrapper> iterator_lookup_generator;
      for (uint32_t i = 0; i < num_allocated_units; i++)
      {
        iterator_lookup_generator.inner_iterators[allocated_unit_types[i]](this, allocated_units[i], allocated_unit_types[i]);
      }
    }

    template <typename inner_wrapper, uint32_t unit_type=0> inline void inner_unit_iterator_by_unit(uint32_t unit_type_selected, Unit* unit)
    {
      if constexpr (unit_type < GameConfig::num_unit_types) {
        if (unit_type == unit_type_selected) {
          inner_wrapper::template inner_iterator<unit_type>(this, unit, unit_type);
        }
        inner_unit_iterator_by_unit<inner_wrapper, unit_type+1>(unit_type_selected, unit);
      }
    }

    template <typename inner_wrapper> inline void unit_iterator_by_unit()
    {
      for (uint32_t i = 0; i < num_allocated_units; i++)
      {
        inner_unit_iterator_by_unit<inner_wrapper>(allocated_unit_types[i], allocated_units[i]);
      }
    }

    class ResourceRateIterator {
      public:
      template <uint32_t unit_type> inline static void inner_iterator(Game<GameConfig>::Player* player, Game<GameConfig>::Unit* unit, uint32_t unit_type_runtime)
      {
        for (uint32_t i = 0; i < GameConfig::num_resource_types; i++) {
          player->resource_production_rate_cache[i] += GameConfig::template get_unit_resource_production_rate<unit_type>(player, unit, i, unit_type_runtime);
        }
      };
    };

    void update_cached_resource_rates()
    {
      for (uint32_t resource_type = 0; resource_type < GameConfig::num_resource_types; resource_type++) {
        resource_production_rate_cache[resource_type] = 0;
      }
      unit_iterator_by_type<ResourceRateIterator>();
      resource_production_rate_cache_dirty = false;
    }

    class ResourceCapacityIterator {
    public:
      template <uint32_t unit_type> inline static void inner_iterator(Game<GameConfig>::Player* player, Game<GameConfig>::Unit* unit, uint32_t unit_type_runtime)
      {
        for (uint32_t i = 0; i < GameConfig::num_resource_types; i++) {
          player->resource_max_capacity_cache[i] += GameConfig::template get_unit_resource_capacity<unit_type>(player, unit, i, unit_type_runtime);
        }
      };
    };

    void update_cached_resource_capacity()
    {
      for (uint32_t resource_type = 0; resource_type < GameConfig::num_resource_types; resource_type++) {
        resource_max_capacity_cache[resource_type] = base_resource_capacity[resource_type];
      }
      unit_iterator_by_type<ResourceCapacityIterator>();
      resource_max_capacity_cache_dirty = false;
    }

    Game* game;

    explicit Player(Game* game_in): game(game_in){}

    Unit* allocate_unit(uint32_t unit_type)
    {
      if (unit_type >= GameConfig::num_unit_types) {
        return nullptr;
      }
      if (num_allocated_units >= GameConfig::max_num_units) {
        return nullptr;
      }
      uint8_t category = unit_type*num_enable_categories/GameConfig::num_unit_types;
      is_unit_set_enabled[category] = true;
      Unit* ret = nullptr;
      for (uint32_t i = 0; i < GameConfig::max_num_units_of_type; i++) {
        if (!units_by_type[unit_type][i].is_allocated) {
          if ((i+1) > furthest_allocated_unit[unit_type]) {
            furthest_allocated_unit[unit_type] = i+1;
          }

          units_by_type[unit_type][i].is_allocated = true;
          ret = &units_by_type[unit_type][i];
          break;
        }
      }
      if(ret)
      {
        allocated_units[num_allocated_units] = ret;
        allocated_unit_types[num_allocated_units] = unit_type;
        num_allocated_units++;
        GameConfig::on_unit_allocated(this, ret, unit_type);
      }
      resource_max_capacity_cache_dirty = true;
      resource_production_rate_cache_dirty = true;
      return ret;
    }

    void deallocate_unit(Unit* unit, uint32_t unit_type)
    {
      unit->is_allocated = false;
      resource_max_capacity_cache_dirty = true;
      resource_production_rate_cache_dirty = true;
      GameConfig::on_unit_deallocated(this, unit, unit_type);

      // Drop unit from allocated table
      uint32_t i = 0;
      for (; i < num_allocated_units; i++)
      {
        if (allocated_units[i] == unit)
        {
          allocated_units[i] = nullptr;
          allocated_unit_types[i] = 0;
          break;
        }
      }
      for (; i < num_allocated_units-1; i++)
      {
        allocated_units[i] = allocated_units[i+1];
        allocated_unit_types[i] = allocated_unit_types[i+1];
      }
      num_allocated_units--;
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
        printf("%s%s: %f (max: %f, prod: %f)\n", indent.c_str(), GameConfig::get_resource_type_name(i), ((float)resources[i]) / ((float)GameConfig::resource_denominator), ((float)resource_max_capacity_cache[i]) / ((float)GameConfig::resource_denominator), (((float)resource_production_rate_cache[i]) / ((float)GameConfig::resource_denominator)) * (float)GameConfig::game_tps);
      }
      printf("%sUnits:\n", indent.c_str());
      indent = indent + " ";

      repeat<GameConfig::num_unit_types>([&](std::size_t x){
          if (detailed_mode)
          {
            for (uint32_t j = 0; j < furthest_allocated_unit[x]; j++) {
              if (units_by_type[x][j].is_allocated) {
                GameConfig::print_detailed_unit_info(this, &units_by_type[x][j], x, indent);
              }
            }
          }
          else
          {
            uint32_t num_units = 0;
            for (uint32_t j = 0; j < furthest_allocated_unit[x]; j++) {
              if (units_by_type[x][j].is_allocated) {
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

    void append_instruction(uint32_t instruction_type, uint32_t instruction_data, uint32_t iterations=1, int32_t resource_index=-1, int32_t operating_unit_index=-1)
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
          instructions[i].iterations = iterations;
          instructions[i].resource_index = resource_index;
          instructions[i].operating_unit_index = operating_unit_index;
          return;
        }
      }
    }

    class UnitTickIterator {
    public:
      template <uint32_t unit_type> inline static void inner_iterator(Game<GameConfig>::Player* player, Game<GameConfig>::Unit* unit, uint32_t unit_type_runtime)
      {
        GameConfig::template unit_tick<unit_type>(player, unit, unit_type_runtime);
      };
    };

    class UnitPostTickIterator {
    public:
      template <uint32_t unit_type> inline static void inner_iterator(Game<GameConfig>::Player* player, Game<GameConfig>::Unit* unit, uint32_t unit_type_runtime)
      {
        GameConfig::template unit_post_tick<unit_type>(player, unit, unit_type_runtime);
      };
    };

    void tick();
  };

  Player players[GameConfig::num_players] = {Player(this)};

  Player* add_player(BAR_Faction faction = BAR_Faction_Armada)
  {
    for (uint32_t i = 0; i < GameConfig::num_players; i++)
    {
      if (!players[i].is_allocated)
      {
        players[i].is_allocated = true;
        GameConfig::on_player_init(&players[i], faction);
        return &players[i];
      }
    }
    return nullptr;
  }

  void print_summary(bool detailed_mode) {
    uint32_t time_mins = (tick) / (60 * GameConfig::game_tps);
    uint32_t time_secs = (tick - (time_mins * 60 * GameConfig::game_tps)) / GameConfig::game_tps;
    printf("Game Time: %d:%02d\n", time_mins, time_secs);
    for (uint32_t i = 0; i < GameConfig::num_players; i++) {
      printf("Player %d:\n", i);
      players[i].print_summary(" ", detailed_mode);
    }
  }

  void do_tick();
};

template<typename GameConfig>
void Game<GameConfig>::do_tick() {
  {
    for (auto& player : players) {
      player.tick();
    }
    tick++;
  }
}

template<typename GameConfig>
void Game<GameConfig>::Player::tick() {
  if (play_by_play)
  {
    if ((game->tick % (GameConfig::game_tps*60) == 0) && (game->tick > 0))
    {
      uint32_t time_mins = (game->tick) / (60 * GameConfig::game_tps);
      uint32_t time_secs = (game->tick - (time_mins * 60 * GameConfig::game_tps)) / GameConfig::game_tps;
      printf("(%d:%02d) ", time_mins, time_secs);
      GameConfig::print_inline_summary(this);
      printf("\n");
    }
  }

  if (resource_production_rate_cache_dirty)
  {
    update_cached_resource_rates();
  }
  for (uint32_t resource_type = 0; resource_type < GameConfig::num_resource_types; resource_type++)
  {
    resources[resource_type] += resource_production_rate_cache[resource_type];
  }

  unit_iterator_by_unit_lookup<UnitTickIterator>();

  if (instructions && current_instruction < num_instructions)
  {
    auto instruction = instructions[current_instruction];
    bool advance_instruction = false;
    if ((instruction.type == 0) || (instruction.iterations == 0))
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
      current_iteration++;
      is_instruction_first_tick = true;
      if (current_iteration >= instruction.iterations) {
        current_instruction++;
        current_iteration = 0;
        if (play_by_play)
        {
          GameConfig::pretty_print_instruction("   ", (void*)(&instructions[current_instruction]));
        }
      }
    }
  }

  unit_iterator_by_type<UnitPostTickIterator>();

  if (resource_max_capacity_cache_dirty)
  {
    update_cached_resource_capacity();
  }
  for (uint32_t resource_type = 0; resource_type < GameConfig::num_resource_types; resource_type++)
  {
    if (resources[resource_type] > resource_max_capacity_cache[resource_type])
    {
      resources[resource_type] = resource_max_capacity_cache[resource_type];
    }
  }
}

#endif //MLX86_GAME_HPP
