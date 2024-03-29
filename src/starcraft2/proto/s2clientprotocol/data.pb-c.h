/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: s2clientprotocol/data.proto */

#ifndef PROTOBUF_C_s2clientprotocol_2fdata_2eproto__INCLUDED
#define PROTOBUF_C_s2clientprotocol_2fdata_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003003 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif

#include "s2clientprotocol/common.pb-c.h"

typedef struct _SC2APIProtocol__AbilityData SC2APIProtocol__AbilityData;
typedef struct _SC2APIProtocol__DamageBonus SC2APIProtocol__DamageBonus;
typedef struct _SC2APIProtocol__Weapon SC2APIProtocol__Weapon;
typedef struct _SC2APIProtocol__UnitTypeData SC2APIProtocol__UnitTypeData;
typedef struct _SC2APIProtocol__UpgradeData SC2APIProtocol__UpgradeData;
typedef struct _SC2APIProtocol__BuffData SC2APIProtocol__BuffData;
typedef struct _SC2APIProtocol__EffectData SC2APIProtocol__EffectData;


/* --- enums --- */

typedef enum _SC2APIProtocol__AbilityData__Target {
  /*
   * Does not require a target.
   */
  SC2_APIPROTOCOL__ABILITY_DATA__TARGET__None = 1,
  /*
   * Requires a target position.
   */
  SC2_APIPROTOCOL__ABILITY_DATA__TARGET__Point = 2,
  /*
   * Requires a unit to target. Given by position using feature layers.
   */
  SC2_APIPROTOCOL__ABILITY_DATA__TARGET__Unit = 3,
  /*
   * Requires either a target point or target unit.
   */
  SC2_APIPROTOCOL__ABILITY_DATA__TARGET__PointOrUnit = 4,
  /*
   * Requires either a target point or no target. (eg. building add-ons)
   */
  SC2_APIPROTOCOL__ABILITY_DATA__TARGET__PointOrNone = 5
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(SC2_APIPROTOCOL__ABILITY_DATA__TARGET)
} SC2APIProtocol__AbilityData__Target;
typedef enum _SC2APIProtocol__Weapon__TargetType {
  SC2_APIPROTOCOL__WEAPON__TARGET_TYPE__Ground = 1,
  SC2_APIPROTOCOL__WEAPON__TARGET_TYPE__Air = 2,
  SC2_APIPROTOCOL__WEAPON__TARGET_TYPE__Any = 3
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(SC2_APIPROTOCOL__WEAPON__TARGET_TYPE)
} SC2APIProtocol__Weapon__TargetType;
typedef enum _SC2APIProtocol__Attribute {
  SC2_APIPROTOCOL__ATTRIBUTE__Light = 1,
  SC2_APIPROTOCOL__ATTRIBUTE__Armored = 2,
  SC2_APIPROTOCOL__ATTRIBUTE__Biological = 3,
  SC2_APIPROTOCOL__ATTRIBUTE__Mechanical = 4,
  SC2_APIPROTOCOL__ATTRIBUTE__Robotic = 5,
  SC2_APIPROTOCOL__ATTRIBUTE__Psionic = 6,
  SC2_APIPROTOCOL__ATTRIBUTE__Massive = 7,
  SC2_APIPROTOCOL__ATTRIBUTE__Structure = 8,
  SC2_APIPROTOCOL__ATTRIBUTE__Hover = 9,
  SC2_APIPROTOCOL__ATTRIBUTE__Heroic = 10,
  SC2_APIPROTOCOL__ATTRIBUTE__Summoned = 11
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(SC2_APIPROTOCOL__ATTRIBUTE)
} SC2APIProtocol__Attribute;

/* --- messages --- */

/*
 * May not relevant: queueable (everything is queueable).
 * May not be important: AbilSetId - marine stim, marauder stim.
 * Stuff omitted: transient.
 * Stuff that may be important: cost, range, Alignment, targetfilters.
 */
struct  _SC2APIProtocol__AbilityData
{
  ProtobufCMessage base;
  /*
   * Stable ID.
   */
  protobuf_c_boolean has_ability_id;
  uint32_t ability_id;
  /*
   * Catalog name of the ability.
   */
  char *link_name;
  /*
   * Catalog index of the ability.
   */
  protobuf_c_boolean has_link_index;
  uint32_t link_index;
  /*
   * Name used for the command card. May not always be set.
   */
  char *button_name;
  /*
   * A human friendly name when the button name or link name isn't descriptive.
   */
  char *friendly_name;
  /*
   * Hotkey. May not always be set.
   */
  char *hotkey;
  /*
   * This ability id may be represented by the given more generic id.
   */
  protobuf_c_boolean has_remaps_to_ability_id;
  uint32_t remaps_to_ability_id;
  /*
   * If true, the ability may be used by this set of mods/map.
   */
  protobuf_c_boolean has_available;
  protobuf_c_boolean available;
  /*
   * Determines if a point is optional or required.
   */
  protobuf_c_boolean has_target;
  SC2APIProtocol__AbilityData__Target target;
  /*
   * Can be cast in the minimap.
   */
  protobuf_c_boolean has_allow_minimap;
  protobuf_c_boolean allow_minimap;
  /*
   * Autocast can be set.
   */
  protobuf_c_boolean has_allow_autocast;
  protobuf_c_boolean allow_autocast;
  /*
   * Requires placement to construct a building.
   */
  protobuf_c_boolean has_is_building;
  protobuf_c_boolean is_building;
  /*
   * Estimation of the footprint size. Need a better footprint.
   */
  protobuf_c_boolean has_footprint_radius;
  float footprint_radius;
  /*
   * Placement next to an existing structure, e.g., an add-on like a Tech Lab.
   */
  protobuf_c_boolean has_is_instant_placement;
  protobuf_c_boolean is_instant_placement;
  /*
   * Range unit can cast ability without needing to approach target.
   */
  protobuf_c_boolean has_cast_range;
  float cast_range;
};
#define SC2_APIPROTOCOL__ABILITY_DATA__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__ability_data__descriptor) \
    , 0, 0, NULL, 0, 0, NULL, NULL, NULL, 0, 0, 0, 0, 0, SC2_APIPROTOCOL__ABILITY_DATA__TARGET__None, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }


struct  _SC2APIProtocol__DamageBonus
{
  ProtobufCMessage base;
  protobuf_c_boolean has_attribute;
  SC2APIProtocol__Attribute attribute;
  protobuf_c_boolean has_bonus;
  float bonus;
};
#define SC2_APIPROTOCOL__DAMAGE_BONUS__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__damage_bonus__descriptor) \
    , 0, SC2_APIPROTOCOL__ATTRIBUTE__Light, 0, 0 }


struct  _SC2APIProtocol__Weapon
{
  ProtobufCMessage base;
  protobuf_c_boolean has_type;
  SC2APIProtocol__Weapon__TargetType type;
  protobuf_c_boolean has_damage;
  float damage;
  size_t n_damage_bonus;
  SC2APIProtocol__DamageBonus **damage_bonus;
  /*
   * Number of hits per attack. (eg. Colossus has 2 beams)
   */
  protobuf_c_boolean has_attacks;
  uint32_t attacks;
  protobuf_c_boolean has_range;
  float range;
  /*
   * Time between attacks.
   */
  protobuf_c_boolean has_speed;
  float speed;
};
#define SC2_APIPROTOCOL__WEAPON__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__weapon__descriptor) \
    , 0, SC2_APIPROTOCOL__WEAPON__TARGET_TYPE__Ground, 0, 0, 0,NULL, 0, 0, 0, 0, 0, 0 }


struct  _SC2APIProtocol__UnitTypeData
{
  ProtobufCMessage base;
  /*
   * Stable ID.
   */
  protobuf_c_boolean has_unit_id;
  uint32_t unit_id;
  /*
   * Catalog name of the unit.
   */
  char *name;
  /*
   * If true, the ability may be used by this set of mods/map.
   */
  protobuf_c_boolean has_available;
  protobuf_c_boolean available;
  /*
   * Number of cargo slots it occupies in transports.
   */
  protobuf_c_boolean has_cargo_size;
  uint32_t cargo_size;
  protobuf_c_boolean has_mineral_cost;
  uint32_t mineral_cost;
  protobuf_c_boolean has_vespene_cost;
  uint32_t vespene_cost;
  protobuf_c_boolean has_food_required;
  float food_required;
  protobuf_c_boolean has_food_provided;
  float food_provided;
  /*
   * The ability that builds this unit.
   */
  protobuf_c_boolean has_ability_id;
  uint32_t ability_id;
  protobuf_c_boolean has_race;
  SC2APIProtocol__Race race;
  protobuf_c_boolean has_build_time;
  float build_time;
  protobuf_c_boolean has_has_vespene;
  protobuf_c_boolean has_vespene;
  protobuf_c_boolean has_has_minerals;
  protobuf_c_boolean has_minerals;
  /*
   * Range unit reveals vision.
   */
  protobuf_c_boolean has_sight_range;
  float sight_range;
  /*
   * Other units that satisfy the same tech requirement.
   */
  size_t n_tech_alias;
  uint32_t *tech_alias;
  /*
   * The morphed variant of this unit.
   */
  protobuf_c_boolean has_unit_alias;
  uint32_t unit_alias;
  /*
   * Structure required to build this unit. (Or any with the same tech_alias)
   */
  protobuf_c_boolean has_tech_requirement;
  uint32_t tech_requirement;
  /*
   * Whether tech_requirement is an add-on.
   */
  protobuf_c_boolean has_require_attached;
  protobuf_c_boolean require_attached;
  /*
   * Values include changes from upgrades
   */
  size_t n_attributes;
  SC2APIProtocol__Attribute *attributes;
  protobuf_c_boolean has_movement_speed;
  float movement_speed;
  protobuf_c_boolean has_armor;
  float armor;
  size_t n_weapons;
  SC2APIProtocol__Weapon **weapons;
};
#define SC2_APIPROTOCOL__UNIT_TYPE_DATA__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__unit_type_data__descriptor) \
    , 0, 0, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SC2_APIPROTOCOL__RACE__NoRace, 0, 0, 0, 0, 0, 0, 0, 0, 0,NULL, 0, 0, 0, 0, 0, 0, 0,NULL, 0, 0, 0, 0, 0,NULL }


struct  _SC2APIProtocol__UpgradeData
{
  ProtobufCMessage base;
  /*
   * Stable ID.
   */
  protobuf_c_boolean has_upgrade_id;
  uint32_t upgrade_id;
  char *name;
  protobuf_c_boolean has_mineral_cost;
  uint32_t mineral_cost;
  protobuf_c_boolean has_vespene_cost;
  uint32_t vespene_cost;
  protobuf_c_boolean has_research_time;
  float research_time;
  protobuf_c_boolean has_ability_id;
  uint32_t ability_id;
};
#define SC2_APIPROTOCOL__UPGRADE_DATA__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__upgrade_data__descriptor) \
    , 0, 0, NULL, 0, 0, 0, 0, 0, 0, 0, 0 }


struct  _SC2APIProtocol__BuffData
{
  ProtobufCMessage base;
  /*
   * Stable ID.
   */
  protobuf_c_boolean has_buff_id;
  uint32_t buff_id;
  char *name;
};
#define SC2_APIPROTOCOL__BUFF_DATA__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__buff_data__descriptor) \
    , 0, 0, NULL }


struct  _SC2APIProtocol__EffectData
{
  ProtobufCMessage base;
  /*
   * Stable ID.
   */
  protobuf_c_boolean has_effect_id;
  uint32_t effect_id;
  char *name;
  char *friendly_name;
  protobuf_c_boolean has_radius;
  float radius;
};
#define SC2_APIPROTOCOL__EFFECT_DATA__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__effect_data__descriptor) \
    , 0, 0, NULL, NULL, 0, 0 }


/* SC2APIProtocol__AbilityData methods */
void   sc2_apiprotocol__ability_data__init
                     (SC2APIProtocol__AbilityData         *message);
size_t sc2_apiprotocol__ability_data__get_packed_size
                     (const SC2APIProtocol__AbilityData   *message);
size_t sc2_apiprotocol__ability_data__pack
                     (const SC2APIProtocol__AbilityData   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__ability_data__pack_to_buffer
                     (const SC2APIProtocol__AbilityData   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__AbilityData *
       sc2_apiprotocol__ability_data__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__ability_data__free_unpacked
                     (SC2APIProtocol__AbilityData *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__DamageBonus methods */
void   sc2_apiprotocol__damage_bonus__init
                     (SC2APIProtocol__DamageBonus         *message);
size_t sc2_apiprotocol__damage_bonus__get_packed_size
                     (const SC2APIProtocol__DamageBonus   *message);
size_t sc2_apiprotocol__damage_bonus__pack
                     (const SC2APIProtocol__DamageBonus   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__damage_bonus__pack_to_buffer
                     (const SC2APIProtocol__DamageBonus   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__DamageBonus *
       sc2_apiprotocol__damage_bonus__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__damage_bonus__free_unpacked
                     (SC2APIProtocol__DamageBonus *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__Weapon methods */
void   sc2_apiprotocol__weapon__init
                     (SC2APIProtocol__Weapon         *message);
size_t sc2_apiprotocol__weapon__get_packed_size
                     (const SC2APIProtocol__Weapon   *message);
size_t sc2_apiprotocol__weapon__pack
                     (const SC2APIProtocol__Weapon   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__weapon__pack_to_buffer
                     (const SC2APIProtocol__Weapon   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__Weapon *
       sc2_apiprotocol__weapon__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__weapon__free_unpacked
                     (SC2APIProtocol__Weapon *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__UnitTypeData methods */
void   sc2_apiprotocol__unit_type_data__init
                     (SC2APIProtocol__UnitTypeData         *message);
size_t sc2_apiprotocol__unit_type_data__get_packed_size
                     (const SC2APIProtocol__UnitTypeData   *message);
size_t sc2_apiprotocol__unit_type_data__pack
                     (const SC2APIProtocol__UnitTypeData   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__unit_type_data__pack_to_buffer
                     (const SC2APIProtocol__UnitTypeData   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__UnitTypeData *
       sc2_apiprotocol__unit_type_data__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__unit_type_data__free_unpacked
                     (SC2APIProtocol__UnitTypeData *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__UpgradeData methods */
void   sc2_apiprotocol__upgrade_data__init
                     (SC2APIProtocol__UpgradeData         *message);
size_t sc2_apiprotocol__upgrade_data__get_packed_size
                     (const SC2APIProtocol__UpgradeData   *message);
size_t sc2_apiprotocol__upgrade_data__pack
                     (const SC2APIProtocol__UpgradeData   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__upgrade_data__pack_to_buffer
                     (const SC2APIProtocol__UpgradeData   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__UpgradeData *
       sc2_apiprotocol__upgrade_data__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__upgrade_data__free_unpacked
                     (SC2APIProtocol__UpgradeData *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__BuffData methods */
void   sc2_apiprotocol__buff_data__init
                     (SC2APIProtocol__BuffData         *message);
size_t sc2_apiprotocol__buff_data__get_packed_size
                     (const SC2APIProtocol__BuffData   *message);
size_t sc2_apiprotocol__buff_data__pack
                     (const SC2APIProtocol__BuffData   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__buff_data__pack_to_buffer
                     (const SC2APIProtocol__BuffData   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__BuffData *
       sc2_apiprotocol__buff_data__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__buff_data__free_unpacked
                     (SC2APIProtocol__BuffData *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__EffectData methods */
void   sc2_apiprotocol__effect_data__init
                     (SC2APIProtocol__EffectData         *message);
size_t sc2_apiprotocol__effect_data__get_packed_size
                     (const SC2APIProtocol__EffectData   *message);
size_t sc2_apiprotocol__effect_data__pack
                     (const SC2APIProtocol__EffectData   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__effect_data__pack_to_buffer
                     (const SC2APIProtocol__EffectData   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__EffectData *
       sc2_apiprotocol__effect_data__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__effect_data__free_unpacked
                     (SC2APIProtocol__EffectData *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*SC2APIProtocol__AbilityData_Closure)
                 (const SC2APIProtocol__AbilityData *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__DamageBonus_Closure)
                 (const SC2APIProtocol__DamageBonus *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__Weapon_Closure)
                 (const SC2APIProtocol__Weapon *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__UnitTypeData_Closure)
                 (const SC2APIProtocol__UnitTypeData *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__UpgradeData_Closure)
                 (const SC2APIProtocol__UpgradeData *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__BuffData_Closure)
                 (const SC2APIProtocol__BuffData *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__EffectData_Closure)
                 (const SC2APIProtocol__EffectData *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCEnumDescriptor    sc2_apiprotocol__attribute__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__ability_data__descriptor;
extern const ProtobufCEnumDescriptor    sc2_apiprotocol__ability_data__target__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__damage_bonus__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__weapon__descriptor;
extern const ProtobufCEnumDescriptor    sc2_apiprotocol__weapon__target_type__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__unit_type_data__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__upgrade_data__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__buff_data__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__effect_data__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_s2clientprotocol_2fdata_2eproto__INCLUDED */
