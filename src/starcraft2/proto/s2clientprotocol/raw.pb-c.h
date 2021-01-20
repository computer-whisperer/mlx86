/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: s2clientprotocol/raw.proto */

#ifndef PROTOBUF_C_s2clientprotocol_2fraw_2eproto__INCLUDED
#define PROTOBUF_C_s2clientprotocol_2fraw_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003003 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif

#include "s2clientprotocol/common.pb-c.h"

typedef struct _SC2APIProtocol__StartRaw SC2APIProtocol__StartRaw;
typedef struct _SC2APIProtocol__ObservationRaw SC2APIProtocol__ObservationRaw;
typedef struct _SC2APIProtocol__RadarRing SC2APIProtocol__RadarRing;
typedef struct _SC2APIProtocol__PowerSource SC2APIProtocol__PowerSource;
typedef struct _SC2APIProtocol__PlayerRaw SC2APIProtocol__PlayerRaw;
typedef struct _SC2APIProtocol__UnitOrder SC2APIProtocol__UnitOrder;
typedef struct _SC2APIProtocol__PassengerUnit SC2APIProtocol__PassengerUnit;
typedef struct _SC2APIProtocol__RallyTarget SC2APIProtocol__RallyTarget;
typedef struct _SC2APIProtocol__Unit SC2APIProtocol__Unit;
typedef struct _SC2APIProtocol__MapState SC2APIProtocol__MapState;
typedef struct _SC2APIProtocol__Event SC2APIProtocol__Event;
typedef struct _SC2APIProtocol__Effect SC2APIProtocol__Effect;
typedef struct _SC2APIProtocol__ActionRaw SC2APIProtocol__ActionRaw;
typedef struct _SC2APIProtocol__ActionRawUnitCommand SC2APIProtocol__ActionRawUnitCommand;
typedef struct _SC2APIProtocol__ActionRawCameraMove SC2APIProtocol__ActionRawCameraMove;
typedef struct _SC2APIProtocol__ActionRawToggleAutocast SC2APIProtocol__ActionRawToggleAutocast;


/* --- enums --- */

typedef enum _SC2APIProtocol__DisplayType {
  /*
   * Fully visible
   */
  SC2_APIPROTOCOL__DISPLAY_TYPE__Visible = 1,
  /*
   * Dimmed version of unit left behind after entering fog of war
   */
  SC2_APIPROTOCOL__DISPLAY_TYPE__Snapshot = 2,
  /*
   * Fully hidden
   */
  SC2_APIPROTOCOL__DISPLAY_TYPE__Hidden = 3,
  /*
   * Building that hasn't started construction.
   */
  SC2_APIPROTOCOL__DISPLAY_TYPE__Placeholder = 4
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(SC2_APIPROTOCOL__DISPLAY_TYPE)
} SC2APIProtocol__DisplayType;
typedef enum _SC2APIProtocol__Alliance {
  SC2_APIPROTOCOL__ALLIANCE__Self = 1,
  SC2_APIPROTOCOL__ALLIANCE__Ally = 2,
  SC2_APIPROTOCOL__ALLIANCE__Neutral = 3,
  SC2_APIPROTOCOL__ALLIANCE__Enemy = 4
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(SC2_APIPROTOCOL__ALLIANCE)
} SC2APIProtocol__Alliance;
typedef enum _SC2APIProtocol__CloakState {
  /*
   * Under the fog, so unknown whether it's cloaked or not.
   */
  SC2_APIPROTOCOL__CLOAK_STATE__CloakedUnknown = 0,
  SC2_APIPROTOCOL__CLOAK_STATE__Cloaked = 1,
  SC2_APIPROTOCOL__CLOAK_STATE__CloakedDetected = 2,
  SC2_APIPROTOCOL__CLOAK_STATE__NotCloaked = 3,
  SC2_APIPROTOCOL__CLOAK_STATE__CloakedAllied = 4
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(SC2_APIPROTOCOL__CLOAK_STATE)
} SC2APIProtocol__CloakState;

/* --- messages --- */

struct  _SC2APIProtocol__StartRaw
{
  ProtobufCMessage base;
  /*
   * Width and height of the map.
   */
  SC2APIProtocol__Size2DI *map_size;
  /*
   * 1 bit bitmap of the pathing grid.
   */
  SC2APIProtocol__ImageData *pathing_grid;
  /*
   * 1 byte bitmap of the terrain height.
   */
  SC2APIProtocol__ImageData *terrain_height;
  /*
   * 1 bit bitmap of the building placement grid.
   */
  SC2APIProtocol__ImageData *placement_grid;
  /*
   * The playable cells.
   */
  SC2APIProtocol__RectangleI *playable_area;
  /*
   * Possible start locations for players.
   */
  size_t n_start_locations;
  SC2APIProtocol__Point2D **start_locations;
};
#define SC2_APIPROTOCOL__START_RAW__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__start_raw__descriptor) \
    , NULL, NULL, NULL, NULL, NULL, 0,NULL }


struct  _SC2APIProtocol__ObservationRaw
{
  ProtobufCMessage base;
  SC2APIProtocol__PlayerRaw *player;
  size_t n_units;
  SC2APIProtocol__Unit **units;
  /*
   * Fog of war, creep and so on. Board stuff that changes per frame.
   */
  SC2APIProtocol__MapState *map_state;
  SC2APIProtocol__Event *event;
  size_t n_effects;
  SC2APIProtocol__Effect **effects;
  size_t n_radar;
  SC2APIProtocol__RadarRing **radar;
};
#define SC2_APIPROTOCOL__OBSERVATION_RAW__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__observation_raw__descriptor) \
    , NULL, 0,NULL, NULL, NULL, 0,NULL, 0,NULL }


struct  _SC2APIProtocol__RadarRing
{
  ProtobufCMessage base;
  SC2APIProtocol__Point *pos;
  protobuf_c_boolean has_radius;
  float radius;
};
#define SC2_APIPROTOCOL__RADAR_RING__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__radar_ring__descriptor) \
    , NULL, 0, 0 }


struct  _SC2APIProtocol__PowerSource
{
  ProtobufCMessage base;
  SC2APIProtocol__Point *pos;
  protobuf_c_boolean has_radius;
  float radius;
  protobuf_c_boolean has_tag;
  uint64_t tag;
};
#define SC2_APIPROTOCOL__POWER_SOURCE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__power_source__descriptor) \
    , NULL, 0, 0, 0, 0 }


struct  _SC2APIProtocol__PlayerRaw
{
  ProtobufCMessage base;
  size_t n_power_sources;
  SC2APIProtocol__PowerSource **power_sources;
  SC2APIProtocol__Point *camera;
  /*
   * TODO: Add to UI observation?
   */
  size_t n_upgrade_ids;
  uint32_t *upgrade_ids;
};
#define SC2_APIPROTOCOL__PLAYER_RAW__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__player_raw__descriptor) \
    , 0,NULL, NULL, 0,NULL }


typedef enum {
  SC2_APIPROTOCOL__UNIT_ORDER__TARGET__NOT_SET = 0,
  SC2_APIPROTOCOL__UNIT_ORDER__TARGET_TARGET_WORLD_SPACE_POS = 2,
  SC2_APIPROTOCOL__UNIT_ORDER__TARGET_TARGET_UNIT_TAG = 3
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(SC2_APIPROTOCOL__UNIT_ORDER__TARGET)
} SC2APIProtocol__UnitOrder__TargetCase;

struct  _SC2APIProtocol__UnitOrder
{
  ProtobufCMessage base;
  protobuf_c_boolean has_ability_id;
  uint32_t ability_id;
  /*
   * Progress of train abilities. Range: [0.0, 1.0]
   */
  protobuf_c_boolean has_progress;
  float progress;
  SC2APIProtocol__UnitOrder__TargetCase target_case;
  union {
    SC2APIProtocol__Point *target_world_space_pos;
    uint64_t target_unit_tag;
  };
};
#define SC2_APIPROTOCOL__UNIT_ORDER__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__unit_order__descriptor) \
    , 0, 0, 0, 0, SC2_APIPROTOCOL__UNIT_ORDER__TARGET__NOT_SET, {0} }


struct  _SC2APIProtocol__PassengerUnit
{
  ProtobufCMessage base;
  protobuf_c_boolean has_tag;
  uint64_t tag;
  protobuf_c_boolean has_health;
  float health;
  protobuf_c_boolean has_health_max;
  float health_max;
  protobuf_c_boolean has_shield;
  float shield;
  protobuf_c_boolean has_shield_max;
  float shield_max;
  protobuf_c_boolean has_energy;
  float energy;
  protobuf_c_boolean has_energy_max;
  float energy_max;
  protobuf_c_boolean has_unit_type;
  uint32_t unit_type;
};
#define SC2_APIPROTOCOL__PASSENGER_UNIT__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__passenger_unit__descriptor) \
    , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }


struct  _SC2APIProtocol__RallyTarget
{
  ProtobufCMessage base;
  /*
   * Will always be filled.
   */
  SC2APIProtocol__Point *point;
  /*
   * Only if it's targeting a unit.
   */
  protobuf_c_boolean has_tag;
  uint64_t tag;
};
#define SC2_APIPROTOCOL__RALLY_TARGET__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__rally_target__descriptor) \
    , NULL, 0, 0 }


struct  _SC2APIProtocol__Unit
{
  ProtobufCMessage base;
  /*
   * Fields are populated based on type/alliance
   */
  protobuf_c_boolean has_display_type;
  SC2APIProtocol__DisplayType display_type;
  protobuf_c_boolean has_alliance;
  SC2APIProtocol__Alliance alliance;
  /*
   * Unique identifier for a unit
   */
  protobuf_c_boolean has_tag;
  uint64_t tag;
  protobuf_c_boolean has_unit_type;
  uint32_t unit_type;
  protobuf_c_boolean has_owner;
  int32_t owner;
  SC2APIProtocol__Point *pos;
  protobuf_c_boolean has_facing;
  float facing;
  protobuf_c_boolean has_radius;
  float radius;
  /*
   * Range: [0.0, 1.0]
   */
  protobuf_c_boolean has_build_progress;
  float build_progress;
  protobuf_c_boolean has_cloak;
  SC2APIProtocol__CloakState cloak;
  size_t n_buff_ids;
  uint32_t *buff_ids;
  protobuf_c_boolean has_detect_range;
  float detect_range;
  protobuf_c_boolean has_radar_range;
  float radar_range;
  protobuf_c_boolean has_is_selected;
  protobuf_c_boolean is_selected;
  /*
   * Visible and within the camera frustrum.
   */
  protobuf_c_boolean has_is_on_screen;
  protobuf_c_boolean is_on_screen;
  /*
   * Detected by sensor tower
   */
  protobuf_c_boolean has_is_blip;
  protobuf_c_boolean is_blip;
  protobuf_c_boolean has_is_powered;
  protobuf_c_boolean is_powered;
  /*
   * Building is training/researching (ie animated).
   */
  protobuf_c_boolean has_is_active;
  protobuf_c_boolean is_active;
  protobuf_c_boolean has_attack_upgrade_level;
  int32_t attack_upgrade_level;
  protobuf_c_boolean has_armor_upgrade_level;
  int32_t armor_upgrade_level;
  protobuf_c_boolean has_shield_upgrade_level;
  int32_t shield_upgrade_level;
  /*
   * Not populated for snapshots
   */
  protobuf_c_boolean has_health;
  float health;
  protobuf_c_boolean has_health_max;
  float health_max;
  protobuf_c_boolean has_shield;
  float shield;
  protobuf_c_boolean has_shield_max;
  float shield_max;
  protobuf_c_boolean has_energy;
  float energy;
  protobuf_c_boolean has_energy_max;
  float energy_max;
  protobuf_c_boolean has_mineral_contents;
  int32_t mineral_contents;
  protobuf_c_boolean has_vespene_contents;
  int32_t vespene_contents;
  protobuf_c_boolean has_is_flying;
  protobuf_c_boolean is_flying;
  protobuf_c_boolean has_is_burrowed;
  protobuf_c_boolean is_burrowed;
  /*
   * Unit is your own or detected as a hallucination.
   */
  protobuf_c_boolean has_is_hallucination;
  protobuf_c_boolean is_hallucination;
  /*
   * Not populated for enemies
   */
  size_t n_orders;
  SC2APIProtocol__UnitOrder **orders;
  protobuf_c_boolean has_add_on_tag;
  uint64_t add_on_tag;
  size_t n_passengers;
  SC2APIProtocol__PassengerUnit **passengers;
  protobuf_c_boolean has_cargo_space_taken;
  int32_t cargo_space_taken;
  protobuf_c_boolean has_cargo_space_max;
  int32_t cargo_space_max;
  protobuf_c_boolean has_assigned_harvesters;
  int32_t assigned_harvesters;
  protobuf_c_boolean has_ideal_harvesters;
  int32_t ideal_harvesters;
  protobuf_c_boolean has_weapon_cooldown;
  float weapon_cooldown;
  protobuf_c_boolean has_engaged_target_tag;
  uint64_t engaged_target_tag;
  /*
   * How long a buff or unit is still around (eg mule, broodling, chronoboost).
   */
  protobuf_c_boolean has_buff_duration_remain;
  int32_t buff_duration_remain;
  /*
   * How long the buff or unit is still around (eg mule, broodling, chronoboost).
   */
  protobuf_c_boolean has_buff_duration_max;
  int32_t buff_duration_max;
  size_t n_rally_targets;
  SC2APIProtocol__RallyTarget **rally_targets;
};
#define SC2_APIPROTOCOL__UNIT__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__unit__descriptor) \
    , 0, SC2_APIPROTOCOL__DISPLAY_TYPE__Visible, 0, SC2_APIPROTOCOL__ALLIANCE__Self, 0, 0, 0, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 0, SC2_APIPROTOCOL__CLOAK_STATE__CloakedUnknown, 0,NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,NULL, 0, 0, 0,NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,NULL }


struct  _SC2APIProtocol__MapState
{
  ProtobufCMessage base;
  /*
   * 1 byte visibility layer.
   */
  SC2APIProtocol__ImageData *visibility;
  /*
   * 1 bit creep layer.
   */
  SC2APIProtocol__ImageData *creep;
};
#define SC2_APIPROTOCOL__MAP_STATE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__map_state__descriptor) \
    , NULL, NULL }


struct  _SC2APIProtocol__Event
{
  ProtobufCMessage base;
  size_t n_dead_units;
  uint64_t *dead_units;
};
#define SC2_APIPROTOCOL__EVENT__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__event__descriptor) \
    , 0,NULL }


struct  _SC2APIProtocol__Effect
{
  ProtobufCMessage base;
  protobuf_c_boolean has_effect_id;
  uint32_t effect_id;
  /*
   * Effect may impact multiple locations. (eg. Lurker attack)
   */
  size_t n_pos;
  SC2APIProtocol__Point2D **pos;
  protobuf_c_boolean has_alliance;
  SC2APIProtocol__Alliance alliance;
  protobuf_c_boolean has_owner;
  int32_t owner;
  protobuf_c_boolean has_radius;
  float radius;
};
#define SC2_APIPROTOCOL__EFFECT__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__effect__descriptor) \
    , 0, 0, 0,NULL, 0, SC2_APIPROTOCOL__ALLIANCE__Self, 0, 0, 0, 0 }


typedef enum {
  SC2_APIPROTOCOL__ACTION_RAW__ACTION__NOT_SET = 0,
  SC2_APIPROTOCOL__ACTION_RAW__ACTION_UNIT_COMMAND = 1,
  SC2_APIPROTOCOL__ACTION_RAW__ACTION_CAMERA_MOVE = 2,
  SC2_APIPROTOCOL__ACTION_RAW__ACTION_TOGGLE_AUTOCAST = 3
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(SC2_APIPROTOCOL__ACTION_RAW__ACTION)
} SC2APIProtocol__ActionRaw__ActionCase;

struct  _SC2APIProtocol__ActionRaw
{
  ProtobufCMessage base;
  SC2APIProtocol__ActionRaw__ActionCase action_case;
  union {
    SC2APIProtocol__ActionRawUnitCommand *unit_command;
    SC2APIProtocol__ActionRawCameraMove *camera_move;
    SC2APIProtocol__ActionRawToggleAutocast *toggle_autocast;
  };
};
#define SC2_APIPROTOCOL__ACTION_RAW__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__action_raw__descriptor) \
    , SC2_APIPROTOCOL__ACTION_RAW__ACTION__NOT_SET, {0} }


typedef enum {
  SC2_APIPROTOCOL__ACTION_RAW_UNIT_COMMAND__TARGET__NOT_SET = 0,
  SC2_APIPROTOCOL__ACTION_RAW_UNIT_COMMAND__TARGET_TARGET_WORLD_SPACE_POS = 2,
  SC2_APIPROTOCOL__ACTION_RAW_UNIT_COMMAND__TARGET_TARGET_UNIT_TAG = 3
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(SC2_APIPROTOCOL__ACTION_RAW_UNIT_COMMAND__TARGET)
} SC2APIProtocol__ActionRawUnitCommand__TargetCase;

struct  _SC2APIProtocol__ActionRawUnitCommand
{
  ProtobufCMessage base;
  protobuf_c_boolean has_ability_id;
  int32_t ability_id;
  size_t n_unit_tags;
  uint64_t *unit_tags;
  protobuf_c_boolean has_queue_command;
  protobuf_c_boolean queue_command;
  SC2APIProtocol__ActionRawUnitCommand__TargetCase target_case;
  union {
    SC2APIProtocol__Point2D *target_world_space_pos;
    uint64_t target_unit_tag;
  };
};
#define SC2_APIPROTOCOL__ACTION_RAW_UNIT_COMMAND__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__action_raw_unit_command__descriptor) \
    , 0, 0, 0,NULL, 0, 0, SC2_APIPROTOCOL__ACTION_RAW_UNIT_COMMAND__TARGET__NOT_SET, {0} }


struct  _SC2APIProtocol__ActionRawCameraMove
{
  ProtobufCMessage base;
  SC2APIProtocol__Point *center_world_space;
};
#define SC2_APIPROTOCOL__ACTION_RAW_CAMERA_MOVE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__action_raw_camera_move__descriptor) \
    , NULL }


struct  _SC2APIProtocol__ActionRawToggleAutocast
{
  ProtobufCMessage base;
  protobuf_c_boolean has_ability_id;
  int32_t ability_id;
  size_t n_unit_tags;
  uint64_t *unit_tags;
};
#define SC2_APIPROTOCOL__ACTION_RAW_TOGGLE_AUTOCAST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__action_raw_toggle_autocast__descriptor) \
    , 0, 0, 0,NULL }


/* SC2APIProtocol__StartRaw methods */
void   sc2_apiprotocol__start_raw__init
                     (SC2APIProtocol__StartRaw         *message);
size_t sc2_apiprotocol__start_raw__get_packed_size
                     (const SC2APIProtocol__StartRaw   *message);
size_t sc2_apiprotocol__start_raw__pack
                     (const SC2APIProtocol__StartRaw   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__start_raw__pack_to_buffer
                     (const SC2APIProtocol__StartRaw   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__StartRaw *
       sc2_apiprotocol__start_raw__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__start_raw__free_unpacked
                     (SC2APIProtocol__StartRaw *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__ObservationRaw methods */
void   sc2_apiprotocol__observation_raw__init
                     (SC2APIProtocol__ObservationRaw         *message);
size_t sc2_apiprotocol__observation_raw__get_packed_size
                     (const SC2APIProtocol__ObservationRaw   *message);
size_t sc2_apiprotocol__observation_raw__pack
                     (const SC2APIProtocol__ObservationRaw   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__observation_raw__pack_to_buffer
                     (const SC2APIProtocol__ObservationRaw   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__ObservationRaw *
       sc2_apiprotocol__observation_raw__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__observation_raw__free_unpacked
                     (SC2APIProtocol__ObservationRaw *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__RadarRing methods */
void   sc2_apiprotocol__radar_ring__init
                     (SC2APIProtocol__RadarRing         *message);
size_t sc2_apiprotocol__radar_ring__get_packed_size
                     (const SC2APIProtocol__RadarRing   *message);
size_t sc2_apiprotocol__radar_ring__pack
                     (const SC2APIProtocol__RadarRing   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__radar_ring__pack_to_buffer
                     (const SC2APIProtocol__RadarRing   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__RadarRing *
       sc2_apiprotocol__radar_ring__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__radar_ring__free_unpacked
                     (SC2APIProtocol__RadarRing *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__PowerSource methods */
void   sc2_apiprotocol__power_source__init
                     (SC2APIProtocol__PowerSource         *message);
size_t sc2_apiprotocol__power_source__get_packed_size
                     (const SC2APIProtocol__PowerSource   *message);
size_t sc2_apiprotocol__power_source__pack
                     (const SC2APIProtocol__PowerSource   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__power_source__pack_to_buffer
                     (const SC2APIProtocol__PowerSource   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__PowerSource *
       sc2_apiprotocol__power_source__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__power_source__free_unpacked
                     (SC2APIProtocol__PowerSource *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__PlayerRaw methods */
void   sc2_apiprotocol__player_raw__init
                     (SC2APIProtocol__PlayerRaw         *message);
size_t sc2_apiprotocol__player_raw__get_packed_size
                     (const SC2APIProtocol__PlayerRaw   *message);
size_t sc2_apiprotocol__player_raw__pack
                     (const SC2APIProtocol__PlayerRaw   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__player_raw__pack_to_buffer
                     (const SC2APIProtocol__PlayerRaw   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__PlayerRaw *
       sc2_apiprotocol__player_raw__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__player_raw__free_unpacked
                     (SC2APIProtocol__PlayerRaw *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__UnitOrder methods */
void   sc2_apiprotocol__unit_order__init
                     (SC2APIProtocol__UnitOrder         *message);
size_t sc2_apiprotocol__unit_order__get_packed_size
                     (const SC2APIProtocol__UnitOrder   *message);
size_t sc2_apiprotocol__unit_order__pack
                     (const SC2APIProtocol__UnitOrder   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__unit_order__pack_to_buffer
                     (const SC2APIProtocol__UnitOrder   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__UnitOrder *
       sc2_apiprotocol__unit_order__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__unit_order__free_unpacked
                     (SC2APIProtocol__UnitOrder *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__PassengerUnit methods */
void   sc2_apiprotocol__passenger_unit__init
                     (SC2APIProtocol__PassengerUnit         *message);
size_t sc2_apiprotocol__passenger_unit__get_packed_size
                     (const SC2APIProtocol__PassengerUnit   *message);
size_t sc2_apiprotocol__passenger_unit__pack
                     (const SC2APIProtocol__PassengerUnit   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__passenger_unit__pack_to_buffer
                     (const SC2APIProtocol__PassengerUnit   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__PassengerUnit *
       sc2_apiprotocol__passenger_unit__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__passenger_unit__free_unpacked
                     (SC2APIProtocol__PassengerUnit *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__RallyTarget methods */
void   sc2_apiprotocol__rally_target__init
                     (SC2APIProtocol__RallyTarget         *message);
size_t sc2_apiprotocol__rally_target__get_packed_size
                     (const SC2APIProtocol__RallyTarget   *message);
size_t sc2_apiprotocol__rally_target__pack
                     (const SC2APIProtocol__RallyTarget   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__rally_target__pack_to_buffer
                     (const SC2APIProtocol__RallyTarget   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__RallyTarget *
       sc2_apiprotocol__rally_target__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__rally_target__free_unpacked
                     (SC2APIProtocol__RallyTarget *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__Unit methods */
void   sc2_apiprotocol__unit__init
                     (SC2APIProtocol__Unit         *message);
size_t sc2_apiprotocol__unit__get_packed_size
                     (const SC2APIProtocol__Unit   *message);
size_t sc2_apiprotocol__unit__pack
                     (const SC2APIProtocol__Unit   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__unit__pack_to_buffer
                     (const SC2APIProtocol__Unit   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__Unit *
       sc2_apiprotocol__unit__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__unit__free_unpacked
                     (SC2APIProtocol__Unit *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__MapState methods */
void   sc2_apiprotocol__map_state__init
                     (SC2APIProtocol__MapState         *message);
size_t sc2_apiprotocol__map_state__get_packed_size
                     (const SC2APIProtocol__MapState   *message);
size_t sc2_apiprotocol__map_state__pack
                     (const SC2APIProtocol__MapState   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__map_state__pack_to_buffer
                     (const SC2APIProtocol__MapState   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__MapState *
       sc2_apiprotocol__map_state__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__map_state__free_unpacked
                     (SC2APIProtocol__MapState *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__Event methods */
void   sc2_apiprotocol__event__init
                     (SC2APIProtocol__Event         *message);
size_t sc2_apiprotocol__event__get_packed_size
                     (const SC2APIProtocol__Event   *message);
size_t sc2_apiprotocol__event__pack
                     (const SC2APIProtocol__Event   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__event__pack_to_buffer
                     (const SC2APIProtocol__Event   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__Event *
       sc2_apiprotocol__event__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__event__free_unpacked
                     (SC2APIProtocol__Event *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__Effect methods */
void   sc2_apiprotocol__effect__init
                     (SC2APIProtocol__Effect         *message);
size_t sc2_apiprotocol__effect__get_packed_size
                     (const SC2APIProtocol__Effect   *message);
size_t sc2_apiprotocol__effect__pack
                     (const SC2APIProtocol__Effect   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__effect__pack_to_buffer
                     (const SC2APIProtocol__Effect   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__Effect *
       sc2_apiprotocol__effect__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__effect__free_unpacked
                     (SC2APIProtocol__Effect *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__ActionRaw methods */
void   sc2_apiprotocol__action_raw__init
                     (SC2APIProtocol__ActionRaw         *message);
size_t sc2_apiprotocol__action_raw__get_packed_size
                     (const SC2APIProtocol__ActionRaw   *message);
size_t sc2_apiprotocol__action_raw__pack
                     (const SC2APIProtocol__ActionRaw   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__action_raw__pack_to_buffer
                     (const SC2APIProtocol__ActionRaw   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__ActionRaw *
       sc2_apiprotocol__action_raw__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__action_raw__free_unpacked
                     (SC2APIProtocol__ActionRaw *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__ActionRawUnitCommand methods */
void   sc2_apiprotocol__action_raw_unit_command__init
                     (SC2APIProtocol__ActionRawUnitCommand         *message);
size_t sc2_apiprotocol__action_raw_unit_command__get_packed_size
                     (const SC2APIProtocol__ActionRawUnitCommand   *message);
size_t sc2_apiprotocol__action_raw_unit_command__pack
                     (const SC2APIProtocol__ActionRawUnitCommand   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__action_raw_unit_command__pack_to_buffer
                     (const SC2APIProtocol__ActionRawUnitCommand   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__ActionRawUnitCommand *
       sc2_apiprotocol__action_raw_unit_command__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__action_raw_unit_command__free_unpacked
                     (SC2APIProtocol__ActionRawUnitCommand *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__ActionRawCameraMove methods */
void   sc2_apiprotocol__action_raw_camera_move__init
                     (SC2APIProtocol__ActionRawCameraMove         *message);
size_t sc2_apiprotocol__action_raw_camera_move__get_packed_size
                     (const SC2APIProtocol__ActionRawCameraMove   *message);
size_t sc2_apiprotocol__action_raw_camera_move__pack
                     (const SC2APIProtocol__ActionRawCameraMove   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__action_raw_camera_move__pack_to_buffer
                     (const SC2APIProtocol__ActionRawCameraMove   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__ActionRawCameraMove *
       sc2_apiprotocol__action_raw_camera_move__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__action_raw_camera_move__free_unpacked
                     (SC2APIProtocol__ActionRawCameraMove *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__ActionRawToggleAutocast methods */
void   sc2_apiprotocol__action_raw_toggle_autocast__init
                     (SC2APIProtocol__ActionRawToggleAutocast         *message);
size_t sc2_apiprotocol__action_raw_toggle_autocast__get_packed_size
                     (const SC2APIProtocol__ActionRawToggleAutocast   *message);
size_t sc2_apiprotocol__action_raw_toggle_autocast__pack
                     (const SC2APIProtocol__ActionRawToggleAutocast   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__action_raw_toggle_autocast__pack_to_buffer
                     (const SC2APIProtocol__ActionRawToggleAutocast   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__ActionRawToggleAutocast *
       sc2_apiprotocol__action_raw_toggle_autocast__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__action_raw_toggle_autocast__free_unpacked
                     (SC2APIProtocol__ActionRawToggleAutocast *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*SC2APIProtocol__StartRaw_Closure)
                 (const SC2APIProtocol__StartRaw *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__ObservationRaw_Closure)
                 (const SC2APIProtocol__ObservationRaw *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__RadarRing_Closure)
                 (const SC2APIProtocol__RadarRing *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__PowerSource_Closure)
                 (const SC2APIProtocol__PowerSource *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__PlayerRaw_Closure)
                 (const SC2APIProtocol__PlayerRaw *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__UnitOrder_Closure)
                 (const SC2APIProtocol__UnitOrder *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__PassengerUnit_Closure)
                 (const SC2APIProtocol__PassengerUnit *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__RallyTarget_Closure)
                 (const SC2APIProtocol__RallyTarget *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__Unit_Closure)
                 (const SC2APIProtocol__Unit *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__MapState_Closure)
                 (const SC2APIProtocol__MapState *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__Event_Closure)
                 (const SC2APIProtocol__Event *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__Effect_Closure)
                 (const SC2APIProtocol__Effect *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__ActionRaw_Closure)
                 (const SC2APIProtocol__ActionRaw *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__ActionRawUnitCommand_Closure)
                 (const SC2APIProtocol__ActionRawUnitCommand *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__ActionRawCameraMove_Closure)
                 (const SC2APIProtocol__ActionRawCameraMove *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__ActionRawToggleAutocast_Closure)
                 (const SC2APIProtocol__ActionRawToggleAutocast *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCEnumDescriptor    sc2_apiprotocol__display_type__descriptor;
extern const ProtobufCEnumDescriptor    sc2_apiprotocol__alliance__descriptor;
extern const ProtobufCEnumDescriptor    sc2_apiprotocol__cloak_state__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__start_raw__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__observation_raw__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__radar_ring__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__power_source__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__player_raw__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__unit_order__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__passenger_unit__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__rally_target__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__unit__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__map_state__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__event__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__effect__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__action_raw__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__action_raw_unit_command__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__action_raw_camera_move__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__action_raw_toggle_autocast__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_s2clientprotocol_2fraw_2eproto__INCLUDED */