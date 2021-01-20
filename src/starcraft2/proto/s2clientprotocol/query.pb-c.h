/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: s2clientprotocol/query.proto */

#ifndef PROTOBUF_C_s2clientprotocol_2fquery_2eproto__INCLUDED
#define PROTOBUF_C_s2clientprotocol_2fquery_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003003 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif

#include "s2clientprotocol/common.pb-c.h"
#include "s2clientprotocol/error.pb-c.h"

typedef struct _SC2APIProtocol__RequestQuery SC2APIProtocol__RequestQuery;
typedef struct _SC2APIProtocol__ResponseQuery SC2APIProtocol__ResponseQuery;
typedef struct _SC2APIProtocol__RequestQueryPathing SC2APIProtocol__RequestQueryPathing;
typedef struct _SC2APIProtocol__ResponseQueryPathing SC2APIProtocol__ResponseQueryPathing;
typedef struct _SC2APIProtocol__RequestQueryAvailableAbilities SC2APIProtocol__RequestQueryAvailableAbilities;
typedef struct _SC2APIProtocol__ResponseQueryAvailableAbilities SC2APIProtocol__ResponseQueryAvailableAbilities;
typedef struct _SC2APIProtocol__RequestQueryBuildingPlacement SC2APIProtocol__RequestQueryBuildingPlacement;
typedef struct _SC2APIProtocol__ResponseQueryBuildingPlacement SC2APIProtocol__ResponseQueryBuildingPlacement;


/* --- enums --- */


/* --- messages --- */

struct  _SC2APIProtocol__RequestQuery
{
  ProtobufCMessage base;
  size_t n_pathing;
  SC2APIProtocol__RequestQueryPathing **pathing;
  size_t n_abilities;
  SC2APIProtocol__RequestQueryAvailableAbilities **abilities;
  size_t n_placements;
  SC2APIProtocol__RequestQueryBuildingPlacement **placements;
  /*
   * Ignores requirements like food, minerals and so on.
   */
  protobuf_c_boolean has_ignore_resource_requirements;
  protobuf_c_boolean ignore_resource_requirements;
};
#define SC2_APIPROTOCOL__REQUEST_QUERY__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__request_query__descriptor) \
    , 0,NULL, 0,NULL, 0,NULL, 0, 0 }


struct  _SC2APIProtocol__ResponseQuery
{
  ProtobufCMessage base;
  size_t n_pathing;
  SC2APIProtocol__ResponseQueryPathing **pathing;
  size_t n_abilities;
  SC2APIProtocol__ResponseQueryAvailableAbilities **abilities;
  size_t n_placements;
  SC2APIProtocol__ResponseQueryBuildingPlacement **placements;
};
#define SC2_APIPROTOCOL__RESPONSE_QUERY__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__response_query__descriptor) \
    , 0,NULL, 0,NULL, 0,NULL }


typedef enum {
  SC2_APIPROTOCOL__REQUEST_QUERY_PATHING__START__NOT_SET = 0,
  SC2_APIPROTOCOL__REQUEST_QUERY_PATHING__START_START_POS = 1,
  SC2_APIPROTOCOL__REQUEST_QUERY_PATHING__START_UNIT_TAG = 2
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(SC2_APIPROTOCOL__REQUEST_QUERY_PATHING__START)
} SC2APIProtocol__RequestQueryPathing__StartCase;

/*
 *--------------------------------------------------------------------------------------------------
 */
struct  _SC2APIProtocol__RequestQueryPathing
{
  ProtobufCMessage base;
  SC2APIProtocol__Point2D *end_pos;
  SC2APIProtocol__RequestQueryPathing__StartCase start_case;
  union {
    SC2APIProtocol__Point2D *start_pos;
    uint64_t unit_tag;
  };
};
#define SC2_APIPROTOCOL__REQUEST_QUERY_PATHING__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__request_query_pathing__descriptor) \
    , NULL, SC2_APIPROTOCOL__REQUEST_QUERY_PATHING__START__NOT_SET, {0} }


struct  _SC2APIProtocol__ResponseQueryPathing
{
  ProtobufCMessage base;
  /*
   * 0 if no path exists
   */
  protobuf_c_boolean has_distance;
  float distance;
};
#define SC2_APIPROTOCOL__RESPONSE_QUERY_PATHING__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__response_query_pathing__descriptor) \
    , 0, 0 }


/*
 *--------------------------------------------------------------------------------------------------
 */
struct  _SC2APIProtocol__RequestQueryAvailableAbilities
{
  ProtobufCMessage base;
  protobuf_c_boolean has_unit_tag;
  uint64_t unit_tag;
};
#define SC2_APIPROTOCOL__REQUEST_QUERY_AVAILABLE_ABILITIES__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__request_query_available_abilities__descriptor) \
    , 0, 0 }


struct  _SC2APIProtocol__ResponseQueryAvailableAbilities
{
  ProtobufCMessage base;
  size_t n_abilities;
  SC2APIProtocol__AvailableAbility **abilities;
  protobuf_c_boolean has_unit_tag;
  uint64_t unit_tag;
  protobuf_c_boolean has_unit_type_id;
  uint32_t unit_type_id;
};
#define SC2_APIPROTOCOL__RESPONSE_QUERY_AVAILABLE_ABILITIES__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__response_query_available_abilities__descriptor) \
    , 0,NULL, 0, 0, 0, 0 }


/*
 *--------------------------------------------------------------------------------------------------
 */
struct  _SC2APIProtocol__RequestQueryBuildingPlacement
{
  ProtobufCMessage base;
  protobuf_c_boolean has_ability_id;
  int32_t ability_id;
  SC2APIProtocol__Point2D *target_pos;
  /*
   * Not required
   */
  protobuf_c_boolean has_placing_unit_tag;
  uint64_t placing_unit_tag;
};
#define SC2_APIPROTOCOL__REQUEST_QUERY_BUILDING_PLACEMENT__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__request_query_building_placement__descriptor) \
    , 0, 0, NULL, 0, 0 }


struct  _SC2APIProtocol__ResponseQueryBuildingPlacement
{
  ProtobufCMessage base;
  protobuf_c_boolean has_result;
  SC2APIProtocol__ActionResult result;
};
#define SC2_APIPROTOCOL__RESPONSE_QUERY_BUILDING_PLACEMENT__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__response_query_building_placement__descriptor) \
    , 0, SC2_APIPROTOCOL__ACTION_RESULT__Success }


/* SC2APIProtocol__RequestQuery methods */
void   sc2_apiprotocol__request_query__init
                     (SC2APIProtocol__RequestQuery         *message);
size_t sc2_apiprotocol__request_query__get_packed_size
                     (const SC2APIProtocol__RequestQuery   *message);
size_t sc2_apiprotocol__request_query__pack
                     (const SC2APIProtocol__RequestQuery   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__request_query__pack_to_buffer
                     (const SC2APIProtocol__RequestQuery   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__RequestQuery *
       sc2_apiprotocol__request_query__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__request_query__free_unpacked
                     (SC2APIProtocol__RequestQuery *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__ResponseQuery methods */
void   sc2_apiprotocol__response_query__init
                     (SC2APIProtocol__ResponseQuery         *message);
size_t sc2_apiprotocol__response_query__get_packed_size
                     (const SC2APIProtocol__ResponseQuery   *message);
size_t sc2_apiprotocol__response_query__pack
                     (const SC2APIProtocol__ResponseQuery   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__response_query__pack_to_buffer
                     (const SC2APIProtocol__ResponseQuery   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__ResponseQuery *
       sc2_apiprotocol__response_query__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__response_query__free_unpacked
                     (SC2APIProtocol__ResponseQuery *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__RequestQueryPathing methods */
void   sc2_apiprotocol__request_query_pathing__init
                     (SC2APIProtocol__RequestQueryPathing         *message);
size_t sc2_apiprotocol__request_query_pathing__get_packed_size
                     (const SC2APIProtocol__RequestQueryPathing   *message);
size_t sc2_apiprotocol__request_query_pathing__pack
                     (const SC2APIProtocol__RequestQueryPathing   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__request_query_pathing__pack_to_buffer
                     (const SC2APIProtocol__RequestQueryPathing   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__RequestQueryPathing *
       sc2_apiprotocol__request_query_pathing__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__request_query_pathing__free_unpacked
                     (SC2APIProtocol__RequestQueryPathing *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__ResponseQueryPathing methods */
void   sc2_apiprotocol__response_query_pathing__init
                     (SC2APIProtocol__ResponseQueryPathing         *message);
size_t sc2_apiprotocol__response_query_pathing__get_packed_size
                     (const SC2APIProtocol__ResponseQueryPathing   *message);
size_t sc2_apiprotocol__response_query_pathing__pack
                     (const SC2APIProtocol__ResponseQueryPathing   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__response_query_pathing__pack_to_buffer
                     (const SC2APIProtocol__ResponseQueryPathing   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__ResponseQueryPathing *
       sc2_apiprotocol__response_query_pathing__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__response_query_pathing__free_unpacked
                     (SC2APIProtocol__ResponseQueryPathing *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__RequestQueryAvailableAbilities methods */
void   sc2_apiprotocol__request_query_available_abilities__init
                     (SC2APIProtocol__RequestQueryAvailableAbilities         *message);
size_t sc2_apiprotocol__request_query_available_abilities__get_packed_size
                     (const SC2APIProtocol__RequestQueryAvailableAbilities   *message);
size_t sc2_apiprotocol__request_query_available_abilities__pack
                     (const SC2APIProtocol__RequestQueryAvailableAbilities   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__request_query_available_abilities__pack_to_buffer
                     (const SC2APIProtocol__RequestQueryAvailableAbilities   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__RequestQueryAvailableAbilities *
       sc2_apiprotocol__request_query_available_abilities__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__request_query_available_abilities__free_unpacked
                     (SC2APIProtocol__RequestQueryAvailableAbilities *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__ResponseQueryAvailableAbilities methods */
void   sc2_apiprotocol__response_query_available_abilities__init
                     (SC2APIProtocol__ResponseQueryAvailableAbilities         *message);
size_t sc2_apiprotocol__response_query_available_abilities__get_packed_size
                     (const SC2APIProtocol__ResponseQueryAvailableAbilities   *message);
size_t sc2_apiprotocol__response_query_available_abilities__pack
                     (const SC2APIProtocol__ResponseQueryAvailableAbilities   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__response_query_available_abilities__pack_to_buffer
                     (const SC2APIProtocol__ResponseQueryAvailableAbilities   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__ResponseQueryAvailableAbilities *
       sc2_apiprotocol__response_query_available_abilities__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__response_query_available_abilities__free_unpacked
                     (SC2APIProtocol__ResponseQueryAvailableAbilities *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__RequestQueryBuildingPlacement methods */
void   sc2_apiprotocol__request_query_building_placement__init
                     (SC2APIProtocol__RequestQueryBuildingPlacement         *message);
size_t sc2_apiprotocol__request_query_building_placement__get_packed_size
                     (const SC2APIProtocol__RequestQueryBuildingPlacement   *message);
size_t sc2_apiprotocol__request_query_building_placement__pack
                     (const SC2APIProtocol__RequestQueryBuildingPlacement   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__request_query_building_placement__pack_to_buffer
                     (const SC2APIProtocol__RequestQueryBuildingPlacement   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__RequestQueryBuildingPlacement *
       sc2_apiprotocol__request_query_building_placement__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__request_query_building_placement__free_unpacked
                     (SC2APIProtocol__RequestQueryBuildingPlacement *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__ResponseQueryBuildingPlacement methods */
void   sc2_apiprotocol__response_query_building_placement__init
                     (SC2APIProtocol__ResponseQueryBuildingPlacement         *message);
size_t sc2_apiprotocol__response_query_building_placement__get_packed_size
                     (const SC2APIProtocol__ResponseQueryBuildingPlacement   *message);
size_t sc2_apiprotocol__response_query_building_placement__pack
                     (const SC2APIProtocol__ResponseQueryBuildingPlacement   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__response_query_building_placement__pack_to_buffer
                     (const SC2APIProtocol__ResponseQueryBuildingPlacement   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__ResponseQueryBuildingPlacement *
       sc2_apiprotocol__response_query_building_placement__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__response_query_building_placement__free_unpacked
                     (SC2APIProtocol__ResponseQueryBuildingPlacement *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*SC2APIProtocol__RequestQuery_Closure)
                 (const SC2APIProtocol__RequestQuery *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__ResponseQuery_Closure)
                 (const SC2APIProtocol__ResponseQuery *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__RequestQueryPathing_Closure)
                 (const SC2APIProtocol__RequestQueryPathing *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__ResponseQueryPathing_Closure)
                 (const SC2APIProtocol__ResponseQueryPathing *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__RequestQueryAvailableAbilities_Closure)
                 (const SC2APIProtocol__RequestQueryAvailableAbilities *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__ResponseQueryAvailableAbilities_Closure)
                 (const SC2APIProtocol__ResponseQueryAvailableAbilities *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__RequestQueryBuildingPlacement_Closure)
                 (const SC2APIProtocol__RequestQueryBuildingPlacement *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__ResponseQueryBuildingPlacement_Closure)
                 (const SC2APIProtocol__ResponseQueryBuildingPlacement *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor sc2_apiprotocol__request_query__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__response_query__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__request_query_pathing__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__response_query_pathing__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__request_query_available_abilities__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__response_query_available_abilities__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__request_query_building_placement__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__response_query_building_placement__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_s2clientprotocol_2fquery_2eproto__INCLUDED */