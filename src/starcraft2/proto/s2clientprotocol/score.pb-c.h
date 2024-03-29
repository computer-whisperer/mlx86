/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: s2clientprotocol/score.proto */

#ifndef PROTOBUF_C_s2clientprotocol_2fscore_2eproto__INCLUDED
#define PROTOBUF_C_s2clientprotocol_2fscore_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003003 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _SC2APIProtocol__Score SC2APIProtocol__Score;
typedef struct _SC2APIProtocol__CategoryScoreDetails SC2APIProtocol__CategoryScoreDetails;
typedef struct _SC2APIProtocol__VitalScoreDetails SC2APIProtocol__VitalScoreDetails;
typedef struct _SC2APIProtocol__ScoreDetails SC2APIProtocol__ScoreDetails;


/* --- enums --- */

typedef enum _SC2APIProtocol__Score__ScoreType {
  /*
   * map generated score (from curriculum maps with special scoring)
   */
  SC2_APIPROTOCOL__SCORE__SCORE_TYPE__Curriculum = 1,
  /*
   * summation of in-progress and current units/buildings value + minerals + vespene
   */
  SC2_APIPROTOCOL__SCORE__SCORE_TYPE__Melee = 2
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(SC2_APIPROTOCOL__SCORE__SCORE_TYPE)
} SC2APIProtocol__Score__ScoreType;

/* --- messages --- */

struct  _SC2APIProtocol__Score
{
  ProtobufCMessage base;
  protobuf_c_boolean has_score_type;
  SC2APIProtocol__Score__ScoreType score_type;
  /*
   * Note: check score_type to know whether this is a melee score or curriculum score
   */
  protobuf_c_boolean has_score;
  int32_t score;
  SC2APIProtocol__ScoreDetails *score_details;
};
#define SC2_APIPROTOCOL__SCORE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__score__descriptor) \
    , 0, SC2_APIPROTOCOL__SCORE__SCORE_TYPE__Curriculum, 0, 0, NULL }


struct  _SC2APIProtocol__CategoryScoreDetails
{
  ProtobufCMessage base;
  /*
   * Used when no other category is configured in game data
   */
  protobuf_c_boolean has_none;
  float none;
  protobuf_c_boolean has_army;
  float army;
  protobuf_c_boolean has_economy;
  float economy;
  protobuf_c_boolean has_technology;
  float technology;
  protobuf_c_boolean has_upgrade;
  float upgrade;
};
#define SC2_APIPROTOCOL__CATEGORY_SCORE_DETAILS__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__category_score_details__descriptor) \
    , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }


struct  _SC2APIProtocol__VitalScoreDetails
{
  ProtobufCMessage base;
  protobuf_c_boolean has_life;
  float life;
  protobuf_c_boolean has_shields;
  float shields;
  protobuf_c_boolean has_energy;
  float energy;
};
#define SC2_APIPROTOCOL__VITAL_SCORE_DETAILS__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__vital_score_details__descriptor) \
    , 0, 0, 0, 0, 0, 0 }


struct  _SC2APIProtocol__ScoreDetails
{
  ProtobufCMessage base;
  /*
   * Sum of time any available structure able to produce a unit is not. The time stacks, as in, three idle barracks will increase idle_production_time three times quicker than just one.
   */
  protobuf_c_boolean has_idle_production_time;
  float idle_production_time;
  /*
   * Sum of time any worker is not mining. Note a worker building is not idle and three idle workers will increase this value three times quicker than just one.
   */
  protobuf_c_boolean has_idle_worker_time;
  float idle_worker_time;
  /*
   * Sum of minerals and vespene spent on completed units.
   */
  protobuf_c_boolean has_total_value_units;
  float total_value_units;
  /*
   * Sum of minerals and vespene spent on completed structures.
   */
  protobuf_c_boolean has_total_value_structures;
  float total_value_structures;
  /*
   * Sum of minerals and vespene of units, belonging to the opponent, that the player has destroyed.
   */
  protobuf_c_boolean has_killed_value_units;
  float killed_value_units;
  /*
   * Sum of minerals and vespene of structures, belonging to the opponent, that the player has destroyed.
   */
  protobuf_c_boolean has_killed_value_structures;
  float killed_value_structures;
  /*
   * Sum of minerals collected by the player.
   */
  protobuf_c_boolean has_collected_minerals;
  float collected_minerals;
  /*
   * Sum of vespene collected by the player.
   */
  protobuf_c_boolean has_collected_vespene;
  float collected_vespene;
  /*
   * Estimated income of minerals over the next minute based on the players current income. The unit is minerals per minute.
   */
  protobuf_c_boolean has_collection_rate_minerals;
  float collection_rate_minerals;
  /*
   * Estimated income of vespene over the next minute based on the players current income. The unit is vespene per minute.
   */
  protobuf_c_boolean has_collection_rate_vespene;
  float collection_rate_vespene;
  /*
   * Sum of spent minerals at the moment it is spent. For example, this number is incremented by 50 the moment an scv is queued in a command center.  It is decremented by 50 if that unit is canceled.
   */
  protobuf_c_boolean has_spent_minerals;
  float spent_minerals;
  /*
   * Sum of spent vespene at the moment it is spent. For example, this number is incremented by 50 when a reaper is queued but decremented by 50 if it is canceled.
   */
  protobuf_c_boolean has_spent_vespene;
  float spent_vespene;
  /*
   * Sum of food, or supply, utilized in the categories above.
   */
  SC2APIProtocol__CategoryScoreDetails *food_used;
  /*
   * Sum of enemies catagories destroyed in minerals.
   */
  SC2APIProtocol__CategoryScoreDetails *killed_minerals;
  /*
   * Sum of enemies catagories destroyed in vespene.
   */
  SC2APIProtocol__CategoryScoreDetails *killed_vespene;
  /*
   *  Sum of lost minerals for the player in each category.
   */
  SC2APIProtocol__CategoryScoreDetails *lost_minerals;
  /*
   * Sum of lost vespene for the player in each category.
   */
  SC2APIProtocol__CategoryScoreDetails *lost_vespene;
  /*
   * Sum of the lost minerals via destroying the players own units/buildings.
   */
  SC2APIProtocol__CategoryScoreDetails *friendly_fire_minerals;
  /*
   * Sum of the lost vespene via destroying the players own units/buildings.
   */
  SC2APIProtocol__CategoryScoreDetails *friendly_fire_vespene;
  /*
   * Sum of used minerals for the player in each category for each existing unit or upgrade. Therefore if a unit died worth 50 mierals this number will be decremented by 50.
   */
  SC2APIProtocol__CategoryScoreDetails *used_minerals;
  /*
   * Sum of used vespene for the player in each category. Therefore if a unit died worth 50 vespene this number will be decremented by 50.
   */
  SC2APIProtocol__CategoryScoreDetails *used_vespene;
  /*
   * Sum of used minerals throughout the entire game for each category. Unliked used_minerals, this value is never decremented.
   */
  SC2APIProtocol__CategoryScoreDetails *total_used_minerals;
  /*
   * Sum of used vespene throughout the entire game for each category. Unliked used_vespene, this value is never decremented.
   */
  SC2APIProtocol__CategoryScoreDetails *total_used_vespene;
  /*
   * Sum of damage dealt to the player's opponent for each category.
   */
  SC2APIProtocol__VitalScoreDetails *total_damage_dealt;
  /*
   * Sum of damage taken by the player for each category.
   */
  SC2APIProtocol__VitalScoreDetails *total_damage_taken;
  /*
   * Sum of health healed by the player. Note that technology can be healed (by queens) or repaired (by scvs).
   */
  SC2APIProtocol__VitalScoreDetails *total_healed;
  /*
   * Recent raw APM.
   */
  protobuf_c_boolean has_current_apm;
  float current_apm;
  /*
   * Recent effective APM.
   */
  protobuf_c_boolean has_current_effective_apm;
  float current_effective_apm;
};
#define SC2_APIPROTOCOL__SCORE_DETAILS__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sc2_apiprotocol__score_details__descriptor) \
    , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0 }


/* SC2APIProtocol__Score methods */
void   sc2_apiprotocol__score__init
                     (SC2APIProtocol__Score         *message);
size_t sc2_apiprotocol__score__get_packed_size
                     (const SC2APIProtocol__Score   *message);
size_t sc2_apiprotocol__score__pack
                     (const SC2APIProtocol__Score   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__score__pack_to_buffer
                     (const SC2APIProtocol__Score   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__Score *
       sc2_apiprotocol__score__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__score__free_unpacked
                     (SC2APIProtocol__Score *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__CategoryScoreDetails methods */
void   sc2_apiprotocol__category_score_details__init
                     (SC2APIProtocol__CategoryScoreDetails         *message);
size_t sc2_apiprotocol__category_score_details__get_packed_size
                     (const SC2APIProtocol__CategoryScoreDetails   *message);
size_t sc2_apiprotocol__category_score_details__pack
                     (const SC2APIProtocol__CategoryScoreDetails   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__category_score_details__pack_to_buffer
                     (const SC2APIProtocol__CategoryScoreDetails   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__CategoryScoreDetails *
       sc2_apiprotocol__category_score_details__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__category_score_details__free_unpacked
                     (SC2APIProtocol__CategoryScoreDetails *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__VitalScoreDetails methods */
void   sc2_apiprotocol__vital_score_details__init
                     (SC2APIProtocol__VitalScoreDetails         *message);
size_t sc2_apiprotocol__vital_score_details__get_packed_size
                     (const SC2APIProtocol__VitalScoreDetails   *message);
size_t sc2_apiprotocol__vital_score_details__pack
                     (const SC2APIProtocol__VitalScoreDetails   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__vital_score_details__pack_to_buffer
                     (const SC2APIProtocol__VitalScoreDetails   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__VitalScoreDetails *
       sc2_apiprotocol__vital_score_details__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__vital_score_details__free_unpacked
                     (SC2APIProtocol__VitalScoreDetails *message,
                      ProtobufCAllocator *allocator);
/* SC2APIProtocol__ScoreDetails methods */
void   sc2_apiprotocol__score_details__init
                     (SC2APIProtocol__ScoreDetails         *message);
size_t sc2_apiprotocol__score_details__get_packed_size
                     (const SC2APIProtocol__ScoreDetails   *message);
size_t sc2_apiprotocol__score_details__pack
                     (const SC2APIProtocol__ScoreDetails   *message,
                      uint8_t             *out);
size_t sc2_apiprotocol__score_details__pack_to_buffer
                     (const SC2APIProtocol__ScoreDetails   *message,
                      ProtobufCBuffer     *buffer);
SC2APIProtocol__ScoreDetails *
       sc2_apiprotocol__score_details__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sc2_apiprotocol__score_details__free_unpacked
                     (SC2APIProtocol__ScoreDetails *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*SC2APIProtocol__Score_Closure)
                 (const SC2APIProtocol__Score *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__CategoryScoreDetails_Closure)
                 (const SC2APIProtocol__CategoryScoreDetails *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__VitalScoreDetails_Closure)
                 (const SC2APIProtocol__VitalScoreDetails *message,
                  void *closure_data);
typedef void (*SC2APIProtocol__ScoreDetails_Closure)
                 (const SC2APIProtocol__ScoreDetails *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor sc2_apiprotocol__score__descriptor;
extern const ProtobufCEnumDescriptor    sc2_apiprotocol__score__score_type__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__category_score_details__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__vital_score_details__descriptor;
extern const ProtobufCMessageDescriptor sc2_apiprotocol__score_details__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_s2clientprotocol_2fscore_2eproto__INCLUDED */
