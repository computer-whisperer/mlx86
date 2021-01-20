/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: s2clientprotocol/error.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "s2clientprotocol/error.pb-c.h"
static const ProtobufCEnumValue sc2_apiprotocol__action_result__enum_values_by_number[214] =
{
  { "Success", "SC2_APIPROTOCOL__ACTION_RESULT__Success", 1 },
  { "NotSupported", "SC2_APIPROTOCOL__ACTION_RESULT__NotSupported", 2 },
  { "Error", "SC2_APIPROTOCOL__ACTION_RESULT__Error", 3 },
  { "CantQueueThatOrder", "SC2_APIPROTOCOL__ACTION_RESULT__CantQueueThatOrder", 4 },
  { "Retry", "SC2_APIPROTOCOL__ACTION_RESULT__Retry", 5 },
  { "Cooldown", "SC2_APIPROTOCOL__ACTION_RESULT__Cooldown", 6 },
  { "QueueIsFull", "SC2_APIPROTOCOL__ACTION_RESULT__QueueIsFull", 7 },
  { "RallyQueueIsFull", "SC2_APIPROTOCOL__ACTION_RESULT__RallyQueueIsFull", 8 },
  { "NotEnoughMinerals", "SC2_APIPROTOCOL__ACTION_RESULT__NotEnoughMinerals", 9 },
  { "NotEnoughVespene", "SC2_APIPROTOCOL__ACTION_RESULT__NotEnoughVespene", 10 },
  { "NotEnoughTerrazine", "SC2_APIPROTOCOL__ACTION_RESULT__NotEnoughTerrazine", 11 },
  { "NotEnoughCustom", "SC2_APIPROTOCOL__ACTION_RESULT__NotEnoughCustom", 12 },
  { "NotEnoughFood", "SC2_APIPROTOCOL__ACTION_RESULT__NotEnoughFood", 13 },
  { "FoodUsageImpossible", "SC2_APIPROTOCOL__ACTION_RESULT__FoodUsageImpossible", 14 },
  { "NotEnoughLife", "SC2_APIPROTOCOL__ACTION_RESULT__NotEnoughLife", 15 },
  { "NotEnoughShields", "SC2_APIPROTOCOL__ACTION_RESULT__NotEnoughShields", 16 },
  { "NotEnoughEnergy", "SC2_APIPROTOCOL__ACTION_RESULT__NotEnoughEnergy", 17 },
  { "LifeSuppressed", "SC2_APIPROTOCOL__ACTION_RESULT__LifeSuppressed", 18 },
  { "ShieldsSuppressed", "SC2_APIPROTOCOL__ACTION_RESULT__ShieldsSuppressed", 19 },
  { "EnergySuppressed", "SC2_APIPROTOCOL__ACTION_RESULT__EnergySuppressed", 20 },
  { "NotEnoughCharges", "SC2_APIPROTOCOL__ACTION_RESULT__NotEnoughCharges", 21 },
  { "CantAddMoreCharges", "SC2_APIPROTOCOL__ACTION_RESULT__CantAddMoreCharges", 22 },
  { "TooMuchMinerals", "SC2_APIPROTOCOL__ACTION_RESULT__TooMuchMinerals", 23 },
  { "TooMuchVespene", "SC2_APIPROTOCOL__ACTION_RESULT__TooMuchVespene", 24 },
  { "TooMuchTerrazine", "SC2_APIPROTOCOL__ACTION_RESULT__TooMuchTerrazine", 25 },
  { "TooMuchCustom", "SC2_APIPROTOCOL__ACTION_RESULT__TooMuchCustom", 26 },
  { "TooMuchFood", "SC2_APIPROTOCOL__ACTION_RESULT__TooMuchFood", 27 },
  { "TooMuchLife", "SC2_APIPROTOCOL__ACTION_RESULT__TooMuchLife", 28 },
  { "TooMuchShields", "SC2_APIPROTOCOL__ACTION_RESULT__TooMuchShields", 29 },
  { "TooMuchEnergy", "SC2_APIPROTOCOL__ACTION_RESULT__TooMuchEnergy", 30 },
  { "MustTargetUnitWithLife", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetUnitWithLife", 31 },
  { "MustTargetUnitWithShields", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetUnitWithShields", 32 },
  { "MustTargetUnitWithEnergy", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetUnitWithEnergy", 33 },
  { "CantTrade", "SC2_APIPROTOCOL__ACTION_RESULT__CantTrade", 34 },
  { "CantSpend", "SC2_APIPROTOCOL__ACTION_RESULT__CantSpend", 35 },
  { "CantTargetThatUnit", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetThatUnit", 36 },
  { "CouldntAllocateUnit", "SC2_APIPROTOCOL__ACTION_RESULT__CouldntAllocateUnit", 37 },
  { "UnitCantMove", "SC2_APIPROTOCOL__ACTION_RESULT__UnitCantMove", 38 },
  { "TransportIsHoldingPosition", "SC2_APIPROTOCOL__ACTION_RESULT__TransportIsHoldingPosition", 39 },
  { "BuildTechRequirementsNotMet", "SC2_APIPROTOCOL__ACTION_RESULT__BuildTechRequirementsNotMet", 40 },
  { "CantFindPlacementLocation", "SC2_APIPROTOCOL__ACTION_RESULT__CantFindPlacementLocation", 41 },
  { "CantBuildOnThat", "SC2_APIPROTOCOL__ACTION_RESULT__CantBuildOnThat", 42 },
  { "CantBuildTooCloseToDropOff", "SC2_APIPROTOCOL__ACTION_RESULT__CantBuildTooCloseToDropOff", 43 },
  { "CantBuildLocationInvalid", "SC2_APIPROTOCOL__ACTION_RESULT__CantBuildLocationInvalid", 44 },
  { "CantSeeBuildLocation", "SC2_APIPROTOCOL__ACTION_RESULT__CantSeeBuildLocation", 45 },
  { "CantBuildTooCloseToCreepSource", "SC2_APIPROTOCOL__ACTION_RESULT__CantBuildTooCloseToCreepSource", 46 },
  { "CantBuildTooCloseToResources", "SC2_APIPROTOCOL__ACTION_RESULT__CantBuildTooCloseToResources", 47 },
  { "CantBuildTooFarFromWater", "SC2_APIPROTOCOL__ACTION_RESULT__CantBuildTooFarFromWater", 48 },
  { "CantBuildTooFarFromCreepSource", "SC2_APIPROTOCOL__ACTION_RESULT__CantBuildTooFarFromCreepSource", 49 },
  { "CantBuildTooFarFromBuildPowerSource", "SC2_APIPROTOCOL__ACTION_RESULT__CantBuildTooFarFromBuildPowerSource", 50 },
  { "CantBuildOnDenseTerrain", "SC2_APIPROTOCOL__ACTION_RESULT__CantBuildOnDenseTerrain", 51 },
  { "CantTrainTooFarFromTrainPowerSource", "SC2_APIPROTOCOL__ACTION_RESULT__CantTrainTooFarFromTrainPowerSource", 52 },
  { "CantLandLocationInvalid", "SC2_APIPROTOCOL__ACTION_RESULT__CantLandLocationInvalid", 53 },
  { "CantSeeLandLocation", "SC2_APIPROTOCOL__ACTION_RESULT__CantSeeLandLocation", 54 },
  { "CantLandTooCloseToCreepSource", "SC2_APIPROTOCOL__ACTION_RESULT__CantLandTooCloseToCreepSource", 55 },
  { "CantLandTooCloseToResources", "SC2_APIPROTOCOL__ACTION_RESULT__CantLandTooCloseToResources", 56 },
  { "CantLandTooFarFromWater", "SC2_APIPROTOCOL__ACTION_RESULT__CantLandTooFarFromWater", 57 },
  { "CantLandTooFarFromCreepSource", "SC2_APIPROTOCOL__ACTION_RESULT__CantLandTooFarFromCreepSource", 58 },
  { "CantLandTooFarFromBuildPowerSource", "SC2_APIPROTOCOL__ACTION_RESULT__CantLandTooFarFromBuildPowerSource", 59 },
  { "CantLandTooFarFromTrainPowerSource", "SC2_APIPROTOCOL__ACTION_RESULT__CantLandTooFarFromTrainPowerSource", 60 },
  { "CantLandOnDenseTerrain", "SC2_APIPROTOCOL__ACTION_RESULT__CantLandOnDenseTerrain", 61 },
  { "AddOnTooFarFromBuilding", "SC2_APIPROTOCOL__ACTION_RESULT__AddOnTooFarFromBuilding", 62 },
  { "MustBuildRefineryFirst", "SC2_APIPROTOCOL__ACTION_RESULT__MustBuildRefineryFirst", 63 },
  { "BuildingIsUnderConstruction", "SC2_APIPROTOCOL__ACTION_RESULT__BuildingIsUnderConstruction", 64 },
  { "CantFindDropOff", "SC2_APIPROTOCOL__ACTION_RESULT__CantFindDropOff", 65 },
  { "CantLoadOtherPlayersUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantLoadOtherPlayersUnits", 66 },
  { "NotEnoughRoomToLoadUnit", "SC2_APIPROTOCOL__ACTION_RESULT__NotEnoughRoomToLoadUnit", 67 },
  { "CantUnloadUnitsThere", "SC2_APIPROTOCOL__ACTION_RESULT__CantUnloadUnitsThere", 68 },
  { "CantWarpInUnitsThere", "SC2_APIPROTOCOL__ACTION_RESULT__CantWarpInUnitsThere", 69 },
  { "CantLoadImmobileUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantLoadImmobileUnits", 70 },
  { "CantRechargeImmobileUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantRechargeImmobileUnits", 71 },
  { "CantRechargeUnderConstructionUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantRechargeUnderConstructionUnits", 72 },
  { "CantLoadThatUnit", "SC2_APIPROTOCOL__ACTION_RESULT__CantLoadThatUnit", 73 },
  { "NoCargoToUnload", "SC2_APIPROTOCOL__ACTION_RESULT__NoCargoToUnload", 74 },
  { "LoadAllNoTargetsFound", "SC2_APIPROTOCOL__ACTION_RESULT__LoadAllNoTargetsFound", 75 },
  { "NotWhileOccupied", "SC2_APIPROTOCOL__ACTION_RESULT__NotWhileOccupied", 76 },
  { "CantAttackWithoutAmmo", "SC2_APIPROTOCOL__ACTION_RESULT__CantAttackWithoutAmmo", 77 },
  { "CantHoldAnyMoreAmmo", "SC2_APIPROTOCOL__ACTION_RESULT__CantHoldAnyMoreAmmo", 78 },
  { "TechRequirementsNotMet", "SC2_APIPROTOCOL__ACTION_RESULT__TechRequirementsNotMet", 79 },
  { "MustLockdownUnitFirst", "SC2_APIPROTOCOL__ACTION_RESULT__MustLockdownUnitFirst", 80 },
  { "MustTargetUnit", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetUnit", 81 },
  { "MustTargetInventory", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetInventory", 82 },
  { "MustTargetVisibleUnit", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetVisibleUnit", 83 },
  { "MustTargetVisibleLocation", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetVisibleLocation", 84 },
  { "MustTargetWalkableLocation", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetWalkableLocation", 85 },
  { "MustTargetPawnableUnit", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetPawnableUnit", 86 },
  { "YouCantControlThatUnit", "SC2_APIPROTOCOL__ACTION_RESULT__YouCantControlThatUnit", 87 },
  { "YouCantIssueCommandsToThatUnit", "SC2_APIPROTOCOL__ACTION_RESULT__YouCantIssueCommandsToThatUnit", 88 },
  { "MustTargetResources", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetResources", 89 },
  { "RequiresHealTarget", "SC2_APIPROTOCOL__ACTION_RESULT__RequiresHealTarget", 90 },
  { "RequiresRepairTarget", "SC2_APIPROTOCOL__ACTION_RESULT__RequiresRepairTarget", 91 },
  { "NoItemsToDrop", "SC2_APIPROTOCOL__ACTION_RESULT__NoItemsToDrop", 92 },
  { "CantHoldAnyMoreItems", "SC2_APIPROTOCOL__ACTION_RESULT__CantHoldAnyMoreItems", 93 },
  { "CantHoldThat", "SC2_APIPROTOCOL__ACTION_RESULT__CantHoldThat", 94 },
  { "TargetHasNoInventory", "SC2_APIPROTOCOL__ACTION_RESULT__TargetHasNoInventory", 95 },
  { "CantDropThisItem", "SC2_APIPROTOCOL__ACTION_RESULT__CantDropThisItem", 96 },
  { "CantMoveThisItem", "SC2_APIPROTOCOL__ACTION_RESULT__CantMoveThisItem", 97 },
  { "CantPawnThisUnit", "SC2_APIPROTOCOL__ACTION_RESULT__CantPawnThisUnit", 98 },
  { "MustTargetCaster", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetCaster", 99 },
  { "CantTargetCaster", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetCaster", 100 },
  { "MustTargetOuter", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetOuter", 101 },
  { "CantTargetOuter", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetOuter", 102 },
  { "MustTargetYourOwnUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetYourOwnUnits", 103 },
  { "CantTargetYourOwnUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetYourOwnUnits", 104 },
  { "MustTargetFriendlyUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetFriendlyUnits", 105 },
  { "CantTargetFriendlyUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetFriendlyUnits", 106 },
  { "MustTargetNeutralUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetNeutralUnits", 107 },
  { "CantTargetNeutralUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetNeutralUnits", 108 },
  { "MustTargetEnemyUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetEnemyUnits", 109 },
  { "CantTargetEnemyUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetEnemyUnits", 110 },
  { "MustTargetAirUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetAirUnits", 111 },
  { "CantTargetAirUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetAirUnits", 112 },
  { "MustTargetGroundUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetGroundUnits", 113 },
  { "CantTargetGroundUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetGroundUnits", 114 },
  { "MustTargetStructures", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetStructures", 115 },
  { "CantTargetStructures", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetStructures", 116 },
  { "MustTargetLightUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetLightUnits", 117 },
  { "CantTargetLightUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetLightUnits", 118 },
  { "MustTargetArmoredUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetArmoredUnits", 119 },
  { "CantTargetArmoredUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetArmoredUnits", 120 },
  { "MustTargetBiologicalUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetBiologicalUnits", 121 },
  { "CantTargetBiologicalUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetBiologicalUnits", 122 },
  { "MustTargetHeroicUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetHeroicUnits", 123 },
  { "CantTargetHeroicUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetHeroicUnits", 124 },
  { "MustTargetRoboticUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetRoboticUnits", 125 },
  { "CantTargetRoboticUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetRoboticUnits", 126 },
  { "MustTargetMechanicalUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetMechanicalUnits", 127 },
  { "CantTargetMechanicalUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetMechanicalUnits", 128 },
  { "MustTargetPsionicUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetPsionicUnits", 129 },
  { "CantTargetPsionicUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetPsionicUnits", 130 },
  { "MustTargetMassiveUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetMassiveUnits", 131 },
  { "CantTargetMassiveUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetMassiveUnits", 132 },
  { "MustTargetMissile", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetMissile", 133 },
  { "CantTargetMissile", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetMissile", 134 },
  { "MustTargetWorkerUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetWorkerUnits", 135 },
  { "CantTargetWorkerUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetWorkerUnits", 136 },
  { "MustTargetEnergyCapableUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetEnergyCapableUnits", 137 },
  { "CantTargetEnergyCapableUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetEnergyCapableUnits", 138 },
  { "MustTargetShieldCapableUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetShieldCapableUnits", 139 },
  { "CantTargetShieldCapableUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetShieldCapableUnits", 140 },
  { "MustTargetFlyers", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetFlyers", 141 },
  { "CantTargetFlyers", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetFlyers", 142 },
  { "MustTargetBuriedUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetBuriedUnits", 143 },
  { "CantTargetBuriedUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetBuriedUnits", 144 },
  { "MustTargetCloakedUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetCloakedUnits", 145 },
  { "CantTargetCloakedUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetCloakedUnits", 146 },
  { "MustTargetUnitsInAStasisField", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetUnitsInAStasisField", 147 },
  { "CantTargetUnitsInAStasisField", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetUnitsInAStasisField", 148 },
  { "MustTargetUnderConstructionUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetUnderConstructionUnits", 149 },
  { "CantTargetUnderConstructionUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetUnderConstructionUnits", 150 },
  { "MustTargetDeadUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetDeadUnits", 151 },
  { "CantTargetDeadUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetDeadUnits", 152 },
  { "MustTargetRevivableUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetRevivableUnits", 153 },
  { "CantTargetRevivableUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetRevivableUnits", 154 },
  { "MustTargetHiddenUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetHiddenUnits", 155 },
  { "CantTargetHiddenUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetHiddenUnits", 156 },
  { "CantRechargeOtherPlayersUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantRechargeOtherPlayersUnits", 157 },
  { "MustTargetHallucinations", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetHallucinations", 158 },
  { "CantTargetHallucinations", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetHallucinations", 159 },
  { "MustTargetInvulnerableUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetInvulnerableUnits", 160 },
  { "CantTargetInvulnerableUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetInvulnerableUnits", 161 },
  { "MustTargetDetectedUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetDetectedUnits", 162 },
  { "CantTargetDetectedUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetDetectedUnits", 163 },
  { "CantTargetUnitWithEnergy", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetUnitWithEnergy", 164 },
  { "CantTargetUnitWithShields", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetUnitWithShields", 165 },
  { "MustTargetUncommandableUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetUncommandableUnits", 166 },
  { "CantTargetUncommandableUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetUncommandableUnits", 167 },
  { "MustTargetPreventDefeatUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetPreventDefeatUnits", 168 },
  { "CantTargetPreventDefeatUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetPreventDefeatUnits", 169 },
  { "MustTargetPreventRevealUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetPreventRevealUnits", 170 },
  { "CantTargetPreventRevealUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetPreventRevealUnits", 171 },
  { "MustTargetPassiveUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetPassiveUnits", 172 },
  { "CantTargetPassiveUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetPassiveUnits", 173 },
  { "MustTargetStunnedUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetStunnedUnits", 174 },
  { "CantTargetStunnedUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetStunnedUnits", 175 },
  { "MustTargetSummonedUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetSummonedUnits", 176 },
  { "CantTargetSummonedUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetSummonedUnits", 177 },
  { "MustTargetUser1", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetUser1", 178 },
  { "CantTargetUser1", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetUser1", 179 },
  { "MustTargetUnstoppableUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetUnstoppableUnits", 180 },
  { "CantTargetUnstoppableUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetUnstoppableUnits", 181 },
  { "MustTargetResistantUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetResistantUnits", 182 },
  { "CantTargetResistantUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetResistantUnits", 183 },
  { "MustTargetDazedUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetDazedUnits", 184 },
  { "CantTargetDazedUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetDazedUnits", 185 },
  { "CantLockdown", "SC2_APIPROTOCOL__ACTION_RESULT__CantLockdown", 186 },
  { "CantMindControl", "SC2_APIPROTOCOL__ACTION_RESULT__CantMindControl", 187 },
  { "MustTargetDestructibles", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetDestructibles", 188 },
  { "CantTargetDestructibles", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetDestructibles", 189 },
  { "MustTargetItems", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetItems", 190 },
  { "CantTargetItems", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetItems", 191 },
  { "NoCalldownAvailable", "SC2_APIPROTOCOL__ACTION_RESULT__NoCalldownAvailable", 192 },
  { "WaypointListFull", "SC2_APIPROTOCOL__ACTION_RESULT__WaypointListFull", 193 },
  { "MustTargetRace", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetRace", 194 },
  { "CantTargetRace", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetRace", 195 },
  { "MustTargetSimilarUnits", "SC2_APIPROTOCOL__ACTION_RESULT__MustTargetSimilarUnits", 196 },
  { "CantTargetSimilarUnits", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetSimilarUnits", 197 },
  { "CantFindEnoughTargets", "SC2_APIPROTOCOL__ACTION_RESULT__CantFindEnoughTargets", 198 },
  { "AlreadySpawningLarva", "SC2_APIPROTOCOL__ACTION_RESULT__AlreadySpawningLarva", 199 },
  { "CantTargetExhaustedResources", "SC2_APIPROTOCOL__ACTION_RESULT__CantTargetExhaustedResources", 200 },
  { "CantUseMinimap", "SC2_APIPROTOCOL__ACTION_RESULT__CantUseMinimap", 201 },
  { "CantUseInfoPanel", "SC2_APIPROTOCOL__ACTION_RESULT__CantUseInfoPanel", 202 },
  { "OrderQueueIsFull", "SC2_APIPROTOCOL__ACTION_RESULT__OrderQueueIsFull", 203 },
  { "CantHarvestThatResource", "SC2_APIPROTOCOL__ACTION_RESULT__CantHarvestThatResource", 204 },
  { "HarvestersNotRequired", "SC2_APIPROTOCOL__ACTION_RESULT__HarvestersNotRequired", 205 },
  { "AlreadyTargeted", "SC2_APIPROTOCOL__ACTION_RESULT__AlreadyTargeted", 206 },
  { "CantAttackWeaponsDisabled", "SC2_APIPROTOCOL__ACTION_RESULT__CantAttackWeaponsDisabled", 207 },
  { "CouldntReachTarget", "SC2_APIPROTOCOL__ACTION_RESULT__CouldntReachTarget", 208 },
  { "TargetIsOutOfRange", "SC2_APIPROTOCOL__ACTION_RESULT__TargetIsOutOfRange", 209 },
  { "TargetIsTooClose", "SC2_APIPROTOCOL__ACTION_RESULT__TargetIsTooClose", 210 },
  { "TargetIsOutOfArc", "SC2_APIPROTOCOL__ACTION_RESULT__TargetIsOutOfArc", 211 },
  { "CantFindTeleportLocation", "SC2_APIPROTOCOL__ACTION_RESULT__CantFindTeleportLocation", 212 },
  { "InvalidItemClass", "SC2_APIPROTOCOL__ACTION_RESULT__InvalidItemClass", 213 },
  { "CantFindCancelOrder", "SC2_APIPROTOCOL__ACTION_RESULT__CantFindCancelOrder", 214 },
};
static const ProtobufCIntRange sc2_apiprotocol__action_result__value_ranges[] = {
{1, 0},{0, 214}
};
static const ProtobufCEnumValueIndex sc2_apiprotocol__action_result__enum_values_by_name[214] =
{
  { "AddOnTooFarFromBuilding", 61 },
  { "AlreadySpawningLarva", 198 },
  { "AlreadyTargeted", 205 },
  { "BuildTechRequirementsNotMet", 39 },
  { "BuildingIsUnderConstruction", 63 },
  { "CantAddMoreCharges", 21 },
  { "CantAttackWeaponsDisabled", 206 },
  { "CantAttackWithoutAmmo", 76 },
  { "CantBuildLocationInvalid", 43 },
  { "CantBuildOnDenseTerrain", 50 },
  { "CantBuildOnThat", 41 },
  { "CantBuildTooCloseToCreepSource", 45 },
  { "CantBuildTooCloseToDropOff", 42 },
  { "CantBuildTooCloseToResources", 46 },
  { "CantBuildTooFarFromBuildPowerSource", 49 },
  { "CantBuildTooFarFromCreepSource", 48 },
  { "CantBuildTooFarFromWater", 47 },
  { "CantDropThisItem", 95 },
  { "CantFindCancelOrder", 213 },
  { "CantFindDropOff", 64 },
  { "CantFindEnoughTargets", 197 },
  { "CantFindPlacementLocation", 40 },
  { "CantFindTeleportLocation", 211 },
  { "CantHarvestThatResource", 203 },
  { "CantHoldAnyMoreAmmo", 77 },
  { "CantHoldAnyMoreItems", 92 },
  { "CantHoldThat", 93 },
  { "CantLandLocationInvalid", 52 },
  { "CantLandOnDenseTerrain", 60 },
  { "CantLandTooCloseToCreepSource", 54 },
  { "CantLandTooCloseToResources", 55 },
  { "CantLandTooFarFromBuildPowerSource", 58 },
  { "CantLandTooFarFromCreepSource", 57 },
  { "CantLandTooFarFromTrainPowerSource", 59 },
  { "CantLandTooFarFromWater", 56 },
  { "CantLoadImmobileUnits", 69 },
  { "CantLoadOtherPlayersUnits", 65 },
  { "CantLoadThatUnit", 72 },
  { "CantLockdown", 185 },
  { "CantMindControl", 186 },
  { "CantMoveThisItem", 96 },
  { "CantPawnThisUnit", 97 },
  { "CantQueueThatOrder", 3 },
  { "CantRechargeImmobileUnits", 70 },
  { "CantRechargeOtherPlayersUnits", 156 },
  { "CantRechargeUnderConstructionUnits", 71 },
  { "CantSeeBuildLocation", 44 },
  { "CantSeeLandLocation", 53 },
  { "CantSpend", 34 },
  { "CantTargetAirUnits", 111 },
  { "CantTargetArmoredUnits", 119 },
  { "CantTargetBiologicalUnits", 121 },
  { "CantTargetBuriedUnits", 143 },
  { "CantTargetCaster", 99 },
  { "CantTargetCloakedUnits", 145 },
  { "CantTargetDazedUnits", 184 },
  { "CantTargetDeadUnits", 151 },
  { "CantTargetDestructibles", 188 },
  { "CantTargetDetectedUnits", 162 },
  { "CantTargetEnemyUnits", 109 },
  { "CantTargetEnergyCapableUnits", 137 },
  { "CantTargetExhaustedResources", 199 },
  { "CantTargetFlyers", 141 },
  { "CantTargetFriendlyUnits", 105 },
  { "CantTargetGroundUnits", 113 },
  { "CantTargetHallucinations", 158 },
  { "CantTargetHeroicUnits", 123 },
  { "CantTargetHiddenUnits", 155 },
  { "CantTargetInvulnerableUnits", 160 },
  { "CantTargetItems", 190 },
  { "CantTargetLightUnits", 117 },
  { "CantTargetMassiveUnits", 131 },
  { "CantTargetMechanicalUnits", 127 },
  { "CantTargetMissile", 133 },
  { "CantTargetNeutralUnits", 107 },
  { "CantTargetOuter", 101 },
  { "CantTargetPassiveUnits", 172 },
  { "CantTargetPreventDefeatUnits", 168 },
  { "CantTargetPreventRevealUnits", 170 },
  { "CantTargetPsionicUnits", 129 },
  { "CantTargetRace", 194 },
  { "CantTargetResistantUnits", 182 },
  { "CantTargetRevivableUnits", 153 },
  { "CantTargetRoboticUnits", 125 },
  { "CantTargetShieldCapableUnits", 139 },
  { "CantTargetSimilarUnits", 196 },
  { "CantTargetStructures", 115 },
  { "CantTargetStunnedUnits", 174 },
  { "CantTargetSummonedUnits", 176 },
  { "CantTargetThatUnit", 35 },
  { "CantTargetUncommandableUnits", 166 },
  { "CantTargetUnderConstructionUnits", 149 },
  { "CantTargetUnitWithEnergy", 163 },
  { "CantTargetUnitWithShields", 164 },
  { "CantTargetUnitsInAStasisField", 147 },
  { "CantTargetUnstoppableUnits", 180 },
  { "CantTargetUser1", 178 },
  { "CantTargetWorkerUnits", 135 },
  { "CantTargetYourOwnUnits", 103 },
  { "CantTrade", 33 },
  { "CantTrainTooFarFromTrainPowerSource", 51 },
  { "CantUnloadUnitsThere", 67 },
  { "CantUseInfoPanel", 201 },
  { "CantUseMinimap", 200 },
  { "CantWarpInUnitsThere", 68 },
  { "Cooldown", 5 },
  { "CouldntAllocateUnit", 36 },
  { "CouldntReachTarget", 207 },
  { "EnergySuppressed", 19 },
  { "Error", 2 },
  { "FoodUsageImpossible", 13 },
  { "HarvestersNotRequired", 204 },
  { "InvalidItemClass", 212 },
  { "LifeSuppressed", 17 },
  { "LoadAllNoTargetsFound", 74 },
  { "MustBuildRefineryFirst", 62 },
  { "MustLockdownUnitFirst", 79 },
  { "MustTargetAirUnits", 110 },
  { "MustTargetArmoredUnits", 118 },
  { "MustTargetBiologicalUnits", 120 },
  { "MustTargetBuriedUnits", 142 },
  { "MustTargetCaster", 98 },
  { "MustTargetCloakedUnits", 144 },
  { "MustTargetDazedUnits", 183 },
  { "MustTargetDeadUnits", 150 },
  { "MustTargetDestructibles", 187 },
  { "MustTargetDetectedUnits", 161 },
  { "MustTargetEnemyUnits", 108 },
  { "MustTargetEnergyCapableUnits", 136 },
  { "MustTargetFlyers", 140 },
  { "MustTargetFriendlyUnits", 104 },
  { "MustTargetGroundUnits", 112 },
  { "MustTargetHallucinations", 157 },
  { "MustTargetHeroicUnits", 122 },
  { "MustTargetHiddenUnits", 154 },
  { "MustTargetInventory", 81 },
  { "MustTargetInvulnerableUnits", 159 },
  { "MustTargetItems", 189 },
  { "MustTargetLightUnits", 116 },
  { "MustTargetMassiveUnits", 130 },
  { "MustTargetMechanicalUnits", 126 },
  { "MustTargetMissile", 132 },
  { "MustTargetNeutralUnits", 106 },
  { "MustTargetOuter", 100 },
  { "MustTargetPassiveUnits", 171 },
  { "MustTargetPawnableUnit", 85 },
  { "MustTargetPreventDefeatUnits", 167 },
  { "MustTargetPreventRevealUnits", 169 },
  { "MustTargetPsionicUnits", 128 },
  { "MustTargetRace", 193 },
  { "MustTargetResistantUnits", 181 },
  { "MustTargetResources", 88 },
  { "MustTargetRevivableUnits", 152 },
  { "MustTargetRoboticUnits", 124 },
  { "MustTargetShieldCapableUnits", 138 },
  { "MustTargetSimilarUnits", 195 },
  { "MustTargetStructures", 114 },
  { "MustTargetStunnedUnits", 173 },
  { "MustTargetSummonedUnits", 175 },
  { "MustTargetUncommandableUnits", 165 },
  { "MustTargetUnderConstructionUnits", 148 },
  { "MustTargetUnit", 80 },
  { "MustTargetUnitWithEnergy", 32 },
  { "MustTargetUnitWithLife", 30 },
  { "MustTargetUnitWithShields", 31 },
  { "MustTargetUnitsInAStasisField", 146 },
  { "MustTargetUnstoppableUnits", 179 },
  { "MustTargetUser1", 177 },
  { "MustTargetVisibleLocation", 83 },
  { "MustTargetVisibleUnit", 82 },
  { "MustTargetWalkableLocation", 84 },
  { "MustTargetWorkerUnits", 134 },
  { "MustTargetYourOwnUnits", 102 },
  { "NoCalldownAvailable", 191 },
  { "NoCargoToUnload", 73 },
  { "NoItemsToDrop", 91 },
  { "NotEnoughCharges", 20 },
  { "NotEnoughCustom", 11 },
  { "NotEnoughEnergy", 16 },
  { "NotEnoughFood", 12 },
  { "NotEnoughLife", 14 },
  { "NotEnoughMinerals", 8 },
  { "NotEnoughRoomToLoadUnit", 66 },
  { "NotEnoughShields", 15 },
  { "NotEnoughTerrazine", 10 },
  { "NotEnoughVespene", 9 },
  { "NotSupported", 1 },
  { "NotWhileOccupied", 75 },
  { "OrderQueueIsFull", 202 },
  { "QueueIsFull", 6 },
  { "RallyQueueIsFull", 7 },
  { "RequiresHealTarget", 89 },
  { "RequiresRepairTarget", 90 },
  { "Retry", 4 },
  { "ShieldsSuppressed", 18 },
  { "Success", 0 },
  { "TargetHasNoInventory", 94 },
  { "TargetIsOutOfArc", 210 },
  { "TargetIsOutOfRange", 208 },
  { "TargetIsTooClose", 209 },
  { "TechRequirementsNotMet", 78 },
  { "TooMuchCustom", 25 },
  { "TooMuchEnergy", 29 },
  { "TooMuchFood", 26 },
  { "TooMuchLife", 27 },
  { "TooMuchMinerals", 22 },
  { "TooMuchShields", 28 },
  { "TooMuchTerrazine", 24 },
  { "TooMuchVespene", 23 },
  { "TransportIsHoldingPosition", 38 },
  { "UnitCantMove", 37 },
  { "WaypointListFull", 192 },
  { "YouCantControlThatUnit", 86 },
  { "YouCantIssueCommandsToThatUnit", 87 },
};
const ProtobufCEnumDescriptor sc2_apiprotocol__action_result__descriptor =
{
  PROTOBUF_C__ENUM_DESCRIPTOR_MAGIC,
  "SC2APIProtocol.ActionResult",
  "ActionResult",
  "SC2APIProtocol__ActionResult",
  "SC2APIProtocol",
  214,
  sc2_apiprotocol__action_result__enum_values_by_number,
  214,
  sc2_apiprotocol__action_result__enum_values_by_name,
  1,
  sc2_apiprotocol__action_result__value_ranges,
  NULL,NULL,NULL,NULL   /* reserved[1234] */
};