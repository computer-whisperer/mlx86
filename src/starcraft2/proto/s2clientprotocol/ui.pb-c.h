/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: s2clientprotocol/ui.proto */

#ifndef PROTOBUF_C_s2clientprotocol_2fui_2eproto__INCLUDED
#define PROTOBUF_C_s2clientprotocol_2fui_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003003 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _SC2APIProtocol__ObservationUI SC2APIProtocol__ObservationUI;
typedef struct _SC2APIProtocol__ControlGroup SC2APIProtocol__ControlGroup;
typedef struct _SC2APIProtocol__UnitInfo SC2APIProtocol__UnitInfo;
typedef struct _SC2APIProtocol__SinglePanel SC2APIProtocol__SinglePanel;
typedef struct _SC2APIProtocol__MultiPanel SC2APIProtocol__MultiPanel;
typedef struct _SC2APIProtocol__CargoPanel SC2APIProtocol__CargoPanel;
typedef struct _SC2APIProtocol__BuildItem SC2APIProtocol__BuildItem;
typedef struct _SC2APIProtocol__ProductionPanel SC2APIProtocol__ProductionPanel;
typedef struct _SC2APIProtocol__ActionUI SC2APIProtocol__ActionUI;
typedef struct _SC2APIProtocol__ActionControlGroup SC2APIProtocol__ActionControlGroup;
typedef struct _SC2APIProtocol__ActionSelectArmy SC2APIProtocol__ActionSelectArmy;
typedef struct _SC2APIProtocol__ActionSelectWarpGates SC2APIProtocol__ActionSelectWarpGates;
typedef struct _SC2APIProtocol__ActionSelectLarva SC2APIProtocol__ActionSelectLarva;
typedef struct _SC2APIProtocol__ActionSelectIdleWorker SC2APIProtocol__ActionSelectIdleWorker;
typedef struct _SC2APIProtocol__ActionMultiPanel SC2APIProtocol__ActionMultiPanel;
typedef struct _SC2APIProtocol__ActionCargoPanelUnload SC2APIProtocol__ActionCargoPanelUnload;
typedef struct _SC2APIProtocol__ActionProductionPanelRemoveFromQueue SC2APIProtocol__ActionProductionPanelRemoveFromQueue;
typedef struct _SC2APIProtocol__ActionToggleAutocast SC2APIProtocol__ActionToggleAutocast;


/* --- enums --- */

typedef enum _SC2APIProtocol__ActionControlGroup__ControlGroupAction {
  /*
   * Equivalent to number hotkey. Replaces current selection with control group.
   */
  SC2_APIPROTOCOL__ACTION_CONTROL_GROUP__CONTROL_GROUP_ACTION__Recall = 1,
  /*
   * Equivalent to Control + number hotkey. Sets control group to current selection.
   */
  SC2_APIPROTOCOL__ACTION_CONTROL_GROUP__CONTROL_GROUP_ACTION__Set = 2,
  /*
   * Equivalent to Shift + number hotkey. Adds current selection into control group.
   */
  SC2_APIPROTOCOL__ACTION_CONTROL_GROUP__CONTROL_GROUP_ACTION__Append = 3,
  /*
   * Equivalent to Control + Alt + number hotkey. Sets control group to current selection. Units are removed from other control groups.
   */
  SC2_APIPROTOCOL__ACTION_CONTROL_GROUP__CONTROL_GROUP_ACTION__SetAndSteal = 4,
  /*
   * Equivalent to Shift + Alt + number hotkey. Adds current selection into control group. Units are removed from other control groups.
   */
  SC2_APIPROTOCOL__ACTION_CONTROL_GROUP__CONTROL_GROUP_ACTION__AppendAndSteal = 5
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(SC2_APIPROTOCOL__ACTION_CONTROL_GROUP__CONTROL_GROUP_ACTION)
} SC2APIProtocol__ActionControlGroup__ControlGroupAction;
typedef enum _SC2APIProtocol__ActionSelectIdleWorker__Type {
  /*
   * Equivalent to click with no modifiers. Replaces selection with single idle worker.
   */
  SC2_APIPROTOCOL__ACTION_SELECT_IDLE_WORKER__TYPE__Set = 1,
  /*
   * Equivalent to shift+click. Adds single idle worker to current selection.
   */
  SC2_APIPROTOCOL__ACTION_SELECT_IDLE_WORKER__TYPE__Add = 2,
  /*
   * Equivalent to control+click. Selects all idle workers.
   */
  SC2_APIPROTOCOL__ACTION_SELECT_IDLE_WORKER__TYPE__All = 3,
  /*
   * Equivalent to shift+control+click. Adds all idle workers to current selection.
   */
  SC2_APIPROTOCOL__ACTION_SELECT_IDLE_WORKER__TYPE__AddAll = 4
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(SC2_APIPROTOCOL__ACTION_SELECT_IDLE_WORKER__TYPE)
} SC2APIProtocol__ActionSelectIdleWorker__Type;
typedef enum _SC2APIProtocol__ActionMultiPanel__Type {
  /*
   * Click on icon
   */
  SC2_APIPROTOCOL__ACTION_MULTI_PANEL__TYPE__SingleSelect = 1,
  /*
   * Shift Click on icon
   */
  SC2_APIPROTOCOL__ACTION_MULTI_PANEL__TYPE__DeselectUnit = 2,
  /*
   * Control Click on icon.
   */
  SC2_APIPROTOCOL__ACTION_MULTI_PANEL__TYPE__SelectAllOfType = 3,
  /*
   * Control+Shift Click on icon.
   */
  SC2_APIPROTOCOL__ACTION_MULTI_PANEL__TYPE__DeselectAllOfType = 4
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(SC2_APIPROTOCOL__ACTION_MULTI_PANEL__TYPE)
} SC2APIProtocol__ActionMultiPanel__Type;

/* --- messages --- */

typedef enum {
  SC2_APIPROTOCOL__OBSERVATION_UI__PANEL__NOT_SET = 0,
  SC2_APIPROTOCOL__OBSERVATION_UI__PANEL_SINGLE = 2,
  SC2_APIPROTOCOL__OBSERVATION_UI__PANEL_MULTI = 3,
  SC2_APIPROTOCOL__OBSERVATION_UI__PANEL_CARGO = 4,
  SC2_APIPROTOCOL__OBSERVATION_UI__PANEL_PRODUCTION = 5
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(SC2_APIPROTOCOL__OBSERVATION_UI__PANEL)
} SC2APIProtocol__ObservationUI__PanelCase;

struct  _SC2APIProtocol__ObservationUI
{
  ProtobufCMessage base;
  size_t n_groups;
  SC2APIProtocol__ControlGroup **groups;
  SC2APIProtocol__ObservationUI__PanelCase panel_case;
  union {
    SC2APIProtocol__SinglePanel *single;
    SC2APIProtocol__MultiPanel *multi;
    SC2APIProtocol__CargoPanel *cargo;
    SC2APIProtocol__ProductionPanel *production;
  };
};
#define SC2_APIPROTOCOL__OBSERVATION_UI__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__observation_ui__descriptor) \
    , 0,NULL, SC2_APIPROTOCOL__OBSERVATION_UI__PANEL__NOT_SET, {0} }


struct  _SC2APIProtocol__ControlGroup
{
  ProtobufCMessage base;
  protobuf_c_boolean has_control_group_index;
  uint32_t control_group_index;
  protobuf_c_boolean has_leader_unit_type;
  uint32_t leader_unit_type;
  protobuf_c_boolean has_count;
  uint32_t count;
};
#define SC2_APIPROTOCOL__CONTROL_GROUP__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__control_group__descriptor) \
    , 0, 0, 0, 0, 0, 0 }


struct  _SC2APIProtocol__UnitInfo
{
  ProtobufCMessage base;
  protobuf_c_boolean has_unit_type;
  uint32_t unit_type;
  protobuf_c_boolean has_player_relative;
  uint32_t player_relative;
  protobuf_c_boolean has_health;
  int32_t health;
  protobuf_c_boolean has_shields;
  int32_t shields;
  protobuf_c_boolean has_energy;
  int32_t energy;
  protobuf_c_boolean has_transport_slots_taken;
  int32_t transport_slots_taken;
  /*
   * Range: [0.0, 1.0]
   */
  protobuf_c_boolean has_build_progress;
  float build_progress;
  SC2APIProtocol__UnitInfo *add_on;
  protobuf_c_boolean has_max_health;
  int32_t max_health;
  protobuf_c_boolean has_max_shields;
  int32_t max_shields;
  protobuf_c_boolean has_max_energy;
  int32_t max_energy;
};
#define SC2_APIPROTOCOL__UNIT_INFO__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__unit_info__descriptor) \
    , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0 }


struct  _SC2APIProtocol__SinglePanel
{
  ProtobufCMessage base;
  SC2APIProtocol__UnitInfo *unit;
  protobuf_c_boolean has_attack_upgrade_level;
  int32_t attack_upgrade_level;
  protobuf_c_boolean has_armor_upgrade_level;
  int32_t armor_upgrade_level;
  protobuf_c_boolean has_shield_upgrade_level;
  int32_t shield_upgrade_level;
  size_t n_buffs;
  int32_t *buffs;
};
#define SC2_APIPROTOCOL__SINGLE_PANEL__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__single_panel__descriptor) \
    , NULL, 0, 0, 0, 0, 0, 0, 0,NULL }


struct  _SC2APIProtocol__MultiPanel
{
  ProtobufCMessage base;
  size_t n_units;
  SC2APIProtocol__UnitInfo **units;
};
#define SC2_APIPROTOCOL__MULTI_PANEL__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__multi_panel__descriptor) \
    , 0,NULL }


struct  _SC2APIProtocol__CargoPanel
{
  ProtobufCMessage base;
  SC2APIProtocol__UnitInfo *unit;
  size_t n_passengers;
  SC2APIProtocol__UnitInfo **passengers;
  /*
   * TODO: Change to cargo size
   */
  protobuf_c_boolean has_slots_available;
  int32_t slots_available;
};
#define SC2_APIPROTOCOL__CARGO_PANEL__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__cargo_panel__descriptor) \
    , NULL, 0,NULL, 0, 0 }


struct  _SC2APIProtocol__BuildItem
{
  ProtobufCMessage base;
  protobuf_c_boolean has_ability_id;
  uint32_t ability_id;
  /*
   * Range: [0.0, 1.0]
   */
  protobuf_c_boolean has_build_progress;
  float build_progress;
};
#define SC2_APIPROTOCOL__BUILD_ITEM__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__build_item__descriptor) \
    , 0, 0, 0, 0 }


struct  _SC2APIProtocol__ProductionPanel
{
  ProtobufCMessage base;
  SC2APIProtocol__UnitInfo *unit;
  /*
   * build_queue ONLY gives information about units that are being produced.
   * Use production_queue instead to see both units being trained as well as research in the queue.
   */
  size_t n_build_queue;
  SC2APIProtocol__UnitInfo **build_queue;
  size_t n_production_queue;
  SC2APIProtocol__BuildItem **production_queue;
};
#define SC2_APIPROTOCOL__PRODUCTION_PANEL__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__production_panel__descriptor) \
    , NULL, 0,NULL, 0,NULL }


typedef enum {
  SC2_APIPROTOCOL__ACTION_UI__ACTION__NOT_SET = 0,
  SC2_APIPROTOCOL__ACTION_UI__ACTION_CONTROL_GROUP = 1,
  SC2_APIPROTOCOL__ACTION_UI__ACTION_SELECT_ARMY = 2,
  SC2_APIPROTOCOL__ACTION_UI__ACTION_SELECT_WARP_GATES = 3,
  SC2_APIPROTOCOL__ACTION_UI__ACTION_SELECT_LARVA = 4,
  SC2_APIPROTOCOL__ACTION_UI__ACTION_SELECT_IDLE_WORKER = 5,
  SC2_APIPROTOCOL__ACTION_UI__ACTION_MULTI_PANEL = 6,
  SC2_APIPROTOCOL__ACTION_UI__ACTION_CARGO_PANEL = 7,
  SC2_APIPROTOCOL__ACTION_UI__ACTION_PRODUCTION_PANEL = 8,
  SC2_APIPROTOCOL__ACTION_UI__ACTION_TOGGLE_AUTOCAST = 9
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(SC2_APIPROTOCOL__ACTION_UI__ACTION)
} SC2APIProtocol__ActionUI__ActionCase;

struct  _SC2APIProtocol__ActionUI
{
  ProtobufCMessage base;
  SC2APIProtocol__ActionUI__ActionCase action_case;
  union {
    SC2APIProtocol__ActionControlGroup *control_group;
    SC2APIProtocol__ActionSelectArmy *select_army;
    SC2APIProtocol__ActionSelectWarpGates *select_warp_gates;
    SC2APIProtocol__ActionSelectLarva *select_larva;
    SC2APIProtocol__ActionSelectIdleWorker *select_idle_worker;
    SC2APIProtocol__ActionMultiPanel *multi_panel;
    SC2APIProtocol__ActionCargoPanelUnload *cargo_panel;
    SC2APIProtocol__ActionProductionPanelRemoveFromQueue *production_panel;
    SC2APIProtocol__ActionToggleAutocast *toggle_autocast;
  };
};
#define SC2_APIPROTOCOL__ACTION_UI__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__action_ui__descriptor) \
    , SC2_APIPROTOCOL__ACTION_UI__ACTION__NOT_SET, {0} }


struct  _SC2APIProtocol__ActionControlGroup
{
  ProtobufCMessage base;
  protobuf_c_boolean has_action;
  SC2APIProtocol__ActionControlGroup__ControlGroupAction action;
  protobuf_c_boolean has_control_group_index;
  uint32_t control_group_index;
};
#define SC2_APIPROTOCOL__ACTION_CONTROL_GROUP__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__action_control_group__descriptor) \
    , 0, SC2_APIPROTOCOL__ACTION_CONTROL_GROUP__CONTROL_GROUP_ACTION__Recall, 0, 0 }


struct  _SC2APIProtocol__ActionSelectArmy
{
  ProtobufCMessage base;
  protobuf_c_boolean has_selection_add;
  protobuf_c_boolean selection_add;
};
#define SC2_APIPROTOCOL__ACTION_SELECT_ARMY__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__action_select_army__descriptor) \
    , 0, 0 }


struct  _SC2APIProtocol__ActionSelectWarpGates
{
  ProtobufCMessage base;
  protobuf_c_boolean has_selection_add;
  protobuf_c_boolean selection_add;
};
#define SC2_APIPROTOCOL__ACTION_SELECT_WARP_GATES__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__action_select_warp_gates__descriptor) \
    , 0, 0 }


struct  _SC2APIProtocol__ActionSelectLarva
{
  ProtobufCMessage base;
};
#define SC2_APIPROTOCOL__ACTION_SELECT_LARVA__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__action_select_larva__descriptor) \
     }


struct  _SC2APIProtocol__ActionSelectIdleWorker
{
  ProtobufCMessage base;
  protobuf_c_boolean has_type;
  SC2APIProtocol__ActionSelectIdleWorker__Type type;
};
#define SC2_APIPROTOCOL__ACTION_SELECT_IDLE_WORKER__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__action_select_idle_worker__descriptor) \
    , 0, SC2_APIPROTOCOL__ACTION_SELECT_IDLE_WORKER__TYPE__Set }


struct  _SC2APIProtocol__ActionMultiPanel
{
  ProtobufCMessage base;
  protobuf_c_boolean has_type;
  SC2APIProtocol__ActionMultiPanel__Type type;
  protobuf_c_boolean has_unit_index;
  int32_t unit_index;
};
#define SC2_APIPROTOCOL__ACTION_MULTI_PANEL__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__action_multi_panel__descriptor) \
    , 0, SC2_APIPROTOCOL__ACTION_MULTI_PANEL__TYPE__SingleSelect, 0, 0 }


struct  _SC2APIProtocol__ActionCargoPanelUnload
{
  ProtobufCMessage base;
  protobuf_c_boolean has_unit_index;
  int32_t unit_index;
};
#define SC2_APIPROTOCOL__ACTION_CARGO_PANEL_UNLOAD__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__action_cargo_panel_unload__descriptor) \
    , 0, 0 }


struct  _SC2APIProtocol__ActionProductionPanelRemoveFromQueue
{
  ProtobufCMessage base;
  protobuf_c_boolean has_unit_index;
  int32_t unit_index;
};
#define SC2_APIPROTOCOL__ACTION_PRODUCTION_PANEL_REMOVE_FROM_QUEUE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__action_production_panel_remove_from_queue__descriptor) \
    , 0, 0 }


struct  _SC2APIProtocol__ActionToggleAutocast
{
  ProtobufCMessage base;
  protobuf_c_boolean has_ability_id;
  int32_t ability_id;
};
#define SC2_APIPROTOCOL__ACTION_TOGGLE_AUTOCAST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__action_toggle_autocast__descriptor) \
    , 0, 0 }


/* SC2APIProtocol__ObservationUI methods */
void   sc2_apiprotocol__observation_ui__init
                     (SC2APIProtocol__ObservationUI         *message);
size_t sc2_apiprotocol__observation_ui__get_packed_size
                     (const SC2APIProtocol__ObservationUI   *message);
size_t sc2_apiprotocol__observation_ui__pack
                     (const SC2APIProtocol__ObservationUI   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__observation_ui__pack_to_buffer
                     (const SC2APIProtocol__ObservationUI   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__ObservationUI *
       sc2_apiprotocol__observation_ui__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__observation_ui__free_unpacked
                     (SC2APIProtocol__ObservationUI *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__ControlGroup methods */
void   sc2_apiprotocol__control_group__init
                     (SC2APIProtocol__ControlGroup         *message);
size_t sc2_apiprotocol__control_group__get_packed_size
                     (const SC2APIProtocol__ControlGroup   *message);
size_t sc2_apiprotocol__control_group__pack
                     (const SC2APIProtocol__ControlGroup   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__control_group__pack_to_buffer
                     (const SC2APIProtocol__ControlGroup   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__ControlGroup *
       sc2_apiprotocol__control_group__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__control_group__free_unpacked
                     (SC2APIProtocol__ControlGroup *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__UnitInfo methods */
void   sc2_apiprotocol__unit_info__init
                     (SC2APIProtocol__UnitInfo         *message);
size_t sc2_apiprotocol__unit_info__get_packed_size
                     (const SC2APIProtocol__UnitInfo   *message);
size_t sc2_apiprotocol__unit_info__pack
                     (const SC2APIProtocol__UnitInfo   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__unit_info__pack_to_buffer
                     (const SC2APIProtocol__UnitInfo   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__UnitInfo *
       sc2_apiprotocol__unit_info__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__unit_info__free_unpacked
                     (SC2APIProtocol__UnitInfo *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__SinglePanel methods */
void   sc2_apiprotocol__single_panel__init
                     (SC2APIProtocol__SinglePanel         *message);
size_t sc2_apiprotocol__single_panel__get_packed_size
                     (const SC2APIProtocol__SinglePanel   *message);
size_t sc2_apiprotocol__single_panel__pack
                     (const SC2APIProtocol__SinglePanel   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__single_panel__pack_to_buffer
                     (const SC2APIProtocol__SinglePanel   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__SinglePanel *
       sc2_apiprotocol__single_panel__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__single_panel__free_unpacked
                     (SC2APIProtocol__SinglePanel *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__MultiPanel methods */
void   sc2_apiprotocol__multi_panel__init
                     (SC2APIProtocol__MultiPanel         *message);
size_t sc2_apiprotocol__multi_panel__get_packed_size
                     (const SC2APIProtocol__MultiPanel   *message);
size_t sc2_apiprotocol__multi_panel__pack
                     (const SC2APIProtocol__MultiPanel   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__multi_panel__pack_to_buffer
                     (const SC2APIProtocol__MultiPanel   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__MultiPanel *
       sc2_apiprotocol__multi_panel__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__multi_panel__free_unpacked
                     (SC2APIProtocol__MultiPanel *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__CargoPanel methods */
void   sc2_apiprotocol__cargo_panel__init
                     (SC2APIProtocol__CargoPanel         *message);
size_t sc2_apiprotocol__cargo_panel__get_packed_size
                     (const SC2APIProtocol__CargoPanel   *message);
size_t sc2_apiprotocol__cargo_panel__pack
                     (const SC2APIProtocol__CargoPanel   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__cargo_panel__pack_to_buffer
                     (const SC2APIProtocol__CargoPanel   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__CargoPanel *
       sc2_apiprotocol__cargo_panel__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__cargo_panel__free_unpacked
                     (SC2APIProtocol__CargoPanel *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__BuildItem methods */
void   sc2_apiprotocol__build_item__init
                     (SC2APIProtocol__BuildItem         *message);
size_t sc2_apiprotocol__build_item__get_packed_size
                     (const SC2APIProtocol__BuildItem   *message);
size_t sc2_apiprotocol__build_item__pack
                     (const SC2APIProtocol__BuildItem   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__build_item__pack_to_buffer
                     (const SC2APIProtocol__BuildItem   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__BuildItem *
       sc2_apiprotocol__build_item__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__build_item__free_unpacked
                     (SC2APIProtocol__BuildItem *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__ProductionPanel methods */
void   sc2_apiprotocol__production_panel__init
                     (SC2APIProtocol__ProductionPanel         *message);
size_t sc2_apiprotocol__production_panel__get_packed_size
                     (const SC2APIProtocol__ProductionPanel   *message);
size_t sc2_apiprotocol__production_panel__pack
                     (const SC2APIProtocol__ProductionPanel   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__production_panel__pack_to_buffer
                     (const SC2APIProtocol__ProductionPanel   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__ProductionPanel *
       sc2_apiprotocol__production_panel__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__production_panel__free_unpacked
                     (SC2APIProtocol__ProductionPanel *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__ActionUI methods */
void   sc2_apiprotocol__action_ui__init
                     (SC2APIProtocol__ActionUI         *message);
size_t sc2_apiprotocol__action_ui__get_packed_size
                     (const SC2APIProtocol__ActionUI   *message);
size_t sc2_apiprotocol__action_ui__pack
                     (const SC2APIProtocol__ActionUI   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__action_ui__pack_to_buffer
                     (const SC2APIProtocol__ActionUI   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__ActionUI *
       sc2_apiprotocol__action_ui__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__action_ui__free_unpacked
                     (SC2APIProtocol__ActionUI *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__ActionControlGroup methods */
void   sc2_apiprotocol__action_control_group__init
                     (SC2APIProtocol__ActionControlGroup         *message);
size_t sc2_apiprotocol__action_control_group__get_packed_size
                     (const SC2APIProtocol__ActionControlGroup   *message);
size_t sc2_apiprotocol__action_control_group__pack
                     (const SC2APIProtocol__ActionControlGroup   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__action_control_group__pack_to_buffer
                     (const SC2APIProtocol__ActionControlGroup   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__ActionControlGroup *
       sc2_apiprotocol__action_control_group__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__action_control_group__free_unpacked
                     (SC2APIProtocol__ActionControlGroup *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__ActionSelectArmy methods */
void   sc2_apiprotocol__action_select_army__init
                     (SC2APIProtocol__ActionSelectArmy         *message);
size_t sc2_apiprotocol__action_select_army__get_packed_size
                     (const SC2APIProtocol__ActionSelectArmy   *message);
size_t sc2_apiprotocol__action_select_army__pack
                     (const SC2APIProtocol__ActionSelectArmy   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__action_select_army__pack_to_buffer
                     (const SC2APIProtocol__ActionSelectArmy   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__ActionSelectArmy *
       sc2_apiprotocol__action_select_army__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__action_select_army__free_unpacked
                     (SC2APIProtocol__ActionSelectArmy *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__ActionSelectWarpGates methods */
void   sc2_apiprotocol__action_select_warp_gates__init
                     (SC2APIProtocol__ActionSelectWarpGates         *message);
size_t sc2_apiprotocol__action_select_warp_gates__get_packed_size
                     (const SC2APIProtocol__ActionSelectWarpGates   *message);
size_t sc2_apiprotocol__action_select_warp_gates__pack
                     (const SC2APIProtocol__ActionSelectWarpGates   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__action_select_warp_gates__pack_to_buffer
                     (const SC2APIProtocol__ActionSelectWarpGates   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__ActionSelectWarpGates *
       sc2_apiprotocol__action_select_warp_gates__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__action_select_warp_gates__free_unpacked
                     (SC2APIProtocol__ActionSelectWarpGates *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__ActionSelectLarva methods */
void   sc2_apiprotocol__action_select_larva__init
                     (SC2APIProtocol__ActionSelectLarva         *message);
size_t sc2_apiprotocol__action_select_larva__get_packed_size
                     (const SC2APIProtocol__ActionSelectLarva   *message);
size_t sc2_apiprotocol__action_select_larva__pack
                     (const SC2APIProtocol__ActionSelectLarva   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__action_select_larva__pack_to_buffer
                     (const SC2APIProtocol__ActionSelectLarva   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__ActionSelectLarva *
       sc2_apiprotocol__action_select_larva__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__action_select_larva__free_unpacked
                     (SC2APIProtocol__ActionSelectLarva *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__ActionSelectIdleWorker methods */
void   sc2_apiprotocol__action_select_idle_worker__init
                     (SC2APIProtocol__ActionSelectIdleWorker         *message);
size_t sc2_apiprotocol__action_select_idle_worker__get_packed_size
                     (const SC2APIProtocol__ActionSelectIdleWorker   *message);
size_t sc2_apiprotocol__action_select_idle_worker__pack
                     (const SC2APIProtocol__ActionSelectIdleWorker   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__action_select_idle_worker__pack_to_buffer
                     (const SC2APIProtocol__ActionSelectIdleWorker   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__ActionSelectIdleWorker *
       sc2_apiprotocol__action_select_idle_worker__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__action_select_idle_worker__free_unpacked
                     (SC2APIProtocol__ActionSelectIdleWorker *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__ActionMultiPanel methods */
void   sc2_apiprotocol__action_multi_panel__init
                     (SC2APIProtocol__ActionMultiPanel         *message);
size_t sc2_apiprotocol__action_multi_panel__get_packed_size
                     (const SC2APIProtocol__ActionMultiPanel   *message);
size_t sc2_apiprotocol__action_multi_panel__pack
                     (const SC2APIProtocol__ActionMultiPanel   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__action_multi_panel__pack_to_buffer
                     (const SC2APIProtocol__ActionMultiPanel   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__ActionMultiPanel *
       sc2_apiprotocol__action_multi_panel__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__action_multi_panel__free_unpacked
                     (SC2APIProtocol__ActionMultiPanel *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__ActionCargoPanelUnload methods */
void   sc2_apiprotocol__action_cargo_panel_unload__init
                     (SC2APIProtocol__ActionCargoPanelUnload         *message);
size_t sc2_apiprotocol__action_cargo_panel_unload__get_packed_size
                     (const SC2APIProtocol__ActionCargoPanelUnload   *message);
size_t sc2_apiprotocol__action_cargo_panel_unload__pack
                     (const SC2APIProtocol__ActionCargoPanelUnload   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__action_cargo_panel_unload__pack_to_buffer
                     (const SC2APIProtocol__ActionCargoPanelUnload   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__ActionCargoPanelUnload *
       sc2_apiprotocol__action_cargo_panel_unload__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__action_cargo_panel_unload__free_unpacked
                     (SC2APIProtocol__ActionCargoPanelUnload *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__ActionProductionPanelRemoveFromQueue methods */
void   sc2_apiprotocol__action_production_panel_remove_from_queue__init
                     (SC2APIProtocol__ActionProductionPanelRemoveFromQueue         *message);
size_t sc2_apiprotocol__action_production_panel_remove_from_queue__get_packed_size
                     (const SC2APIProtocol__ActionProductionPanelRemoveFromQueue   *message);
size_t sc2_apiprotocol__action_production_panel_remove_from_queue__pack
                     (const SC2APIProtocol__ActionProductionPanelRemoveFromQueue   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__action_production_panel_remove_from_queue__pack_to_buffer
                     (const SC2APIProtocol__ActionProductionPanelRemoveFromQueue   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__ActionProductionPanelRemoveFromQueue *
       sc2_apiprotocol__action_production_panel_remove_from_queue__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__action_production_panel_remove_from_queue__free_unpacked
                     (SC2APIProtocol__ActionProductionPanelRemoveFromQueue *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__ActionToggleAutocast methods */
void   sc2_apiprotocol__action_toggle_autocast__init
                     (SC2APIProtocol__ActionToggleAutocast         *message);
size_t sc2_apiprotocol__action_toggle_autocast__get_packed_size
                     (const SC2APIProtocol__ActionToggleAutocast   *message);
size_t sc2_apiprotocol__action_toggle_autocast__pack
                     (const SC2APIProtocol__ActionToggleAutocast   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__action_toggle_autocast__pack_to_buffer
                     (const SC2APIProtocol__ActionToggleAutocast   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__ActionToggleAutocast *
       sc2_apiprotocol__action_toggle_autocast__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__action_toggle_autocast__free_unpacked
                     (SC2APIProtocol__ActionToggleAutocast *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*SC2APIProtocol__ObservationUI_Closure)
                 (const SC2APIProtocol__ObservationUI *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__ControlGroup_Closure)
                 (const SC2APIProtocol__ControlGroup *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__UnitInfo_Closure)
                 (const SC2APIProtocol__UnitInfo *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__SinglePanel_Closure)
                 (const SC2APIProtocol__SinglePanel *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__MultiPanel_Closure)
                 (const SC2APIProtocol__MultiPanel *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__CargoPanel_Closure)
                 (const SC2APIProtocol__CargoPanel *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__BuildItem_Closure)
                 (const SC2APIProtocol__BuildItem *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__ProductionPanel_Closure)
                 (const SC2APIProtocol__ProductionPanel *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__ActionUI_Closure)
                 (const SC2APIProtocol__ActionUI *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__ActionControlGroup_Closure)
                 (const SC2APIProtocol__ActionControlGroup *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__ActionSelectArmy_Closure)
                 (const SC2APIProtocol__ActionSelectArmy *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__ActionSelectWarpGates_Closure)
                 (const SC2APIProtocol__ActionSelectWarpGates *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__ActionSelectLarva_Closure)
                 (const SC2APIProtocol__ActionSelectLarva *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__ActionSelectIdleWorker_Closure)
                 (const SC2APIProtocol__ActionSelectIdleWorker *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__ActionMultiPanel_Closure)
                 (const SC2APIProtocol__ActionMultiPanel *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__ActionCargoPanelUnload_Closure)
                 (const SC2APIProtocol__ActionCargoPanelUnload *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__ActionProductionPanelRemoveFromQueue_Closure)
                 (const SC2APIProtocol__ActionProductionPanelRemoveFromQueue *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__ActionToggleAutocast_Closure)
                 (const SC2APIProtocol__ActionToggleAutocast *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor sc2_apiprotocol__observation_ui__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__control_group__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__unit_info__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__single_panel__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__multi_panel__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__cargo_panel__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__build_item__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__production_panel__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__action_ui__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__action_control_group__descriptor;
extern const ProtobufCEnumDescriptor    sc2_apiprotocol__action_control_group__control_group_action__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__action_select_army__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__action_select_warp_gates__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__action_select_larva__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__action_select_idle_worker__descriptor;
extern const ProtobufCEnumDescriptor    sc2_apiprotocol__action_select_idle_worker__type__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__action_multi_panel__descriptor;
extern const ProtobufCEnumDescriptor    sc2_apiprotocol__action_multi_panel__type__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__action_cargo_panel_unload__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__action_production_panel_remove_from_queue__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__action_toggle_autocast__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_s2clientprotocol_2fui_2eproto__INCLUDED */