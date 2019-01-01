#ifndef _STRINGENUMS_HPP__
#define _STRINGENUMS_HPP__

#include <map>
#include <vector>
#include <set>
#include <string>
#include "Utils/Logger.hpp"
#include <stdlib.h>
#include <memory>

// This file is auto-generated and should not be edited because changes
// will be lost. Generated with pyEnumGen, written by Simon Nitzsche 2019.

/// Copyright (c) 2019 Simon Nitzsche All Rights Reserved.

namespace Enums::StringEnums {
	// Enum: eBuildType \\ 
	enum class Ese_eBuildType { BUILD_NOWHERE, BUILD_IN_WORLD, BUILD_ON_PROPERTY };
	class se_eBuildType {
		template<int i>
		std::string get() {
			if ( i == Ese_eBuildType::BUILD_NOWHERE )
				return "BUILD_NOWHERE";
			if ( i == Ese_eBuildType::BUILD_IN_WORLD )
				return "BUILD_IN_WORLD";
			if ( i == Ese_eBuildType::BUILD_ON_PROPERTY )
				return "BUILD_ON_PROPERTY";
			throw new out_of_range::exception();
		}

		template<unsigned char* str>
		Ese_eBuildType get() {
			if ( str == "BUILD_NOWHERE" )
				return Ese_eBuildType::BUILD_NOWHERE;
			if ( str == "BUILD_IN_WORLD" )
				return Ese_eBuildType::BUILD_IN_WORLD;
			if ( str == "BUILD_ON_PROPERTY" )
				return Ese_eBuildType::BUILD_ON_PROPERTY;
			throw new out_of_range::exception();
		}

	};

	// Enum: CinematicEvent \\ 
	enum class Ese_CinematicEvent { STARTED, WAYPOINT, ENDED };
	class se_CinematicEvent {
		template<int i>
		std::string get() {
			if ( i == Ese_CinematicEvent::STARTED )
				return "STARTED";
			if ( i == Ese_CinematicEvent::WAYPOINT )
				return "WAYPOINT";
			if ( i == Ese_CinematicEvent::ENDED )
				return "ENDED";
			throw new out_of_range::exception();
		}

		template<unsigned char* str>
		Ese_CinematicEvent get() {
			if ( str == "STARTED" )
				return Ese_CinematicEvent::STARTED;
			if ( str == "WAYPOINT" )
				return Ese_CinematicEvent::WAYPOINT;
			if ( str == "ENDED" )
				return Ese_CinematicEvent::ENDED;
			throw new out_of_range::exception();
		}

	};

	// Enum: eCyclingMode \\ 
	enum class Ese_eCyclingMode { ALLOW_CYCLE_TEAMMATES, DISALLOW_CYCLING };
	class se_eCyclingMode {
		template<int i>
		std::string get() {
			if ( i == Ese_eCyclingMode::ALLOW_CYCLE_TEAMMATES )
				return "ALLOW_CYCLE_TEAMMATES";
			if ( i == Ese_eCyclingMode::DISALLOW_CYCLING )
				return "DISALLOW_CYCLING";
			throw new out_of_range::exception();
		}

		template<unsigned char* str>
		Ese_eCyclingMode get() {
			if ( str == "ALLOW_CYCLE_TEAMMATES" )
				return Ese_eCyclingMode::ALLOW_CYCLE_TEAMMATES;
			if ( str == "DISALLOW_CYCLING" )
				return Ese_eCyclingMode::DISALLOW_CYCLING;
			throw new out_of_range::exception();
		}

	};

	// Enum: DeleteReason \\ 
	enum class Ese_DeleteReason { PICKING_MODEL_UP, RETURNING_MODEL_TO_INVENTORY, BREAKING_MODEL_APART };
	class se_DeleteReason {
		template<int i>
		std::string get() {
			if ( i == Ese_DeleteReason::PICKING_MODEL_UP )
				return "PICKING_MODEL_UP";
			if ( i == Ese_DeleteReason::RETURNING_MODEL_TO_INVENTORY )
				return "RETURNING_MODEL_TO_INVENTORY";
			if ( i == Ese_DeleteReason::BREAKING_MODEL_APART )
				return "BREAKING_MODEL_APART";
			throw new out_of_range::exception();
		}

		template<unsigned char* str>
		Ese_DeleteReason get() {
			if ( str == "PICKING_MODEL_UP" )
				return Ese_DeleteReason::PICKING_MODEL_UP;
			if ( str == "RETURNING_MODEL_TO_INVENTORY" )
				return Ese_DeleteReason::RETURNING_MODEL_TO_INVENTORY;
			if ( str == "BREAKING_MODEL_APART" )
				return Ese_DeleteReason::BREAKING_MODEL_APART;
			throw new out_of_range::exception();
		}

	};

	// Enum: EndBehavior \\ 
	enum class Ese_EndBehavior { RETURN, WAIT };
	class se_EndBehavior {
		template<int i>
		std::string get() {
			if ( i == Ese_EndBehavior::RETURN )
				return "RETURN";
			if ( i == Ese_EndBehavior::WAIT )
				return "WAIT";
			throw new out_of_range::exception();
		}

		template<unsigned char* str>
		Ese_EndBehavior get() {
			if ( str == "RETURN" )
				return Ese_EndBehavior::RETURN;
			if ( str == "WAIT" )
				return Ese_EndBehavior::WAIT;
			throw new out_of_range::exception();
		}

	};

	// Enum: FailReason \\ 
	enum class Ese_FailReason { REASON_NOT_GIVEN, REASON_OUT_OF_IMAGINATION, REASON_CANCELED_EARLY, REASON_BUILD_ENDED };
	class se_FailReason {
		template<int i>
		std::string get() {
			if ( i == Ese_FailReason::REASON_NOT_GIVEN )
				return "REASON_NOT_GIVEN";
			if ( i == Ese_FailReason::REASON_OUT_OF_IMAGINATION )
				return "REASON_OUT_OF_IMAGINATION";
			if ( i == Ese_FailReason::REASON_CANCELED_EARLY )
				return "REASON_CANCELED_EARLY";
			if ( i == Ese_FailReason::REASON_BUILD_ENDED )
				return "REASON_BUILD_ENDED";
			throw new out_of_range::exception();
		}

		template<unsigned char* str>
		Ese_FailReason get() {
			if ( str == "REASON_NOT_GIVEN" )
				return Ese_FailReason::REASON_NOT_GIVEN;
			if ( str == "REASON_OUT_OF_IMAGINATION" )
				return Ese_FailReason::REASON_OUT_OF_IMAGINATION;
			if ( str == "REASON_CANCELED_EARLY" )
				return Ese_FailReason::REASON_CANCELED_EARLY;
			if ( str == "REASON_BUILD_ENDED" )
				return Ese_FailReason::REASON_BUILD_ENDED;
			throw new out_of_range::exception();
		}

	};

	// Enum: eImmunityState \\ 
	enum class Ese_eImmunityState { PUSH, POP };
	class se_eImmunityState {
		template<int i>
		std::string get() {
			if ( i == Ese_eImmunityState::PUSH )
				return "PUSH";
			if ( i == Ese_eImmunityState::POP )
				return "POP";
			throw new out_of_range::exception();
		}

		template<unsigned char* str>
		Ese_eImmunityState get() {
			if ( str == "PUSH" )
				return Ese_eImmunityState::PUSH;
			if ( str == "POP" )
				return Ese_eImmunityState::POP;
			throw new out_of_range::exception();
		}

	};

	// Enum: eKillType \\ 
	enum class Ese_eKillType { VIOLENT, SILENT };
	class se_eKillType {
		template<int i>
		std::string get() {
			if ( i == Ese_eKillType::VIOLENT )
				return "VIOLENT";
			if ( i == Ese_eKillType::SILENT )
				return "SILENT";
			throw new out_of_range::exception();
		}

		template<unsigned char* str>
		Ese_eKillType get() {
			if ( str == "VIOLENT" )
				return Ese_eKillType::VIOLENT;
			if ( str == "SILENT" )
				return Ese_eKillType::SILENT;
			throw new out_of_range::exception();
		}

	};

	// Enum: eMissionLockState \\ 
	enum class Ese_eMissionLockState { LOCKED, NEW, UNLOCKED };
	class se_eMissionLockState {
		template<int i>
		std::string get() {
			if ( i == Ese_eMissionLockState::LOCKED )
				return "LOCKED";
			if ( i == Ese_eMissionLockState::NEW )
				return "NEW";
			if ( i == Ese_eMissionLockState::UNLOCKED )
				return "UNLOCKED";
			throw new out_of_range::exception();
		}

		template<unsigned char* str>
		Ese_eMissionLockState get() {
			if ( str == "LOCKED" )
				return Ese_eMissionLockState::LOCKED;
			if ( str == "NEW" )
				return Ese_eMissionLockState::NEW;
			if ( str == "UNLOCKED" )
				return Ese_eMissionLockState::UNLOCKED;
			throw new out_of_range::exception();
		}

	};

	// Enum: eNotifyType \\ 
	enum class Ese_eNotifyType { SUCCESS, QUIT, FAILED, BEGIN, READY, NAMINGPET };
	class se_eNotifyType {
		template<int i>
		std::string get() {
			if ( i == Ese_eNotifyType::SUCCESS )
				return "SUCCESS";
			if ( i == Ese_eNotifyType::QUIT )
				return "QUIT";
			if ( i == Ese_eNotifyType::FAILED )
				return "FAILED";
			if ( i == Ese_eNotifyType::BEGIN )
				return "BEGIN";
			if ( i == Ese_eNotifyType::READY )
				return "READY";
			if ( i == Ese_eNotifyType::NAMINGPET )
				return "NAMINGPET";
			throw new out_of_range::exception();
		}

		template<unsigned char* str>
		Ese_eNotifyType get() {
			if ( str == "SUCCESS" )
				return Ese_eNotifyType::SUCCESS;
			if ( str == "QUIT" )
				return Ese_eNotifyType::QUIT;
			if ( str == "FAILED" )
				return Ese_eNotifyType::FAILED;
			if ( str == "BEGIN" )
				return Ese_eNotifyType::BEGIN;
			if ( str == "READY" )
				return Ese_eNotifyType::READY;
			if ( str == "NAMINGPET" )
				return Ese_eNotifyType::NAMINGPET;
			throw new out_of_range::exception();
		}

	};

	// Enum: eObjectWorldState \\ 
	enum class Ese_eObjectWorldState { WORLDSTATE_INWORLD, WORLDSTATE_ATTACHED, WORLDSTATE_INVENTORY };
	class se_eObjectWorldState {
		template<int i>
		std::string get() {
			if ( i == Ese_eObjectWorldState::WORLDSTATE_INWORLD )
				return "WORLDSTATE_INWORLD";
			if ( i == Ese_eObjectWorldState::WORLDSTATE_ATTACHED )
				return "WORLDSTATE_ATTACHED";
			if ( i == Ese_eObjectWorldState::WORLDSTATE_INVENTORY )
				return "WORLDSTATE_INVENTORY";
			throw new out_of_range::exception();
		}

		template<unsigned char* str>
		Ese_eObjectWorldState get() {
			if ( str == "WORLDSTATE_INWORLD" )
				return Ese_eObjectWorldState::WORLDSTATE_INWORLD;
			if ( str == "WORLDSTATE_ATTACHED" )
				return Ese_eObjectWorldState::WORLDSTATE_ATTACHED;
			if ( str == "WORLDSTATE_INVENTORY" )
				return Ese_eObjectWorldState::WORLDSTATE_INVENTORY;
			throw new out_of_range::exception();
		}

	};

	// Enum: eRacingClientNotificationType \\ 
	enum class Ese_eRacingClientNotificationType { INVALID, ACTIVITY_START, REWARD_PLAYER, EXIT, REPLAY, REMOVE_PLAYER, LEADERBOARD_UPDATED };
	class se_eRacingClientNotificationType {
		template<int i>
		std::string get() {
			if ( i == Ese_eRacingClientNotificationType::INVALID )
				return "INVALID";
			if ( i == Ese_eRacingClientNotificationType::ACTIVITY_START )
				return "ACTIVITY_START";
			if ( i == Ese_eRacingClientNotificationType::REWARD_PLAYER )
				return "REWARD_PLAYER";
			if ( i == Ese_eRacingClientNotificationType::EXIT )
				return "EXIT";
			if ( i == Ese_eRacingClientNotificationType::REPLAY )
				return "REPLAY";
			if ( i == Ese_eRacingClientNotificationType::REMOVE_PLAYER )
				return "REMOVE_PLAYER";
			if ( i == Ese_eRacingClientNotificationType::LEADERBOARD_UPDATED )
				return "LEADERBOARD_UPDATED";
			throw new out_of_range::exception();
		}

		template<unsigned char* str>
		Ese_eRacingClientNotificationType get() {
			if ( str == "INVALID" )
				return Ese_eRacingClientNotificationType::INVALID;
			if ( str == "ACTIVITY_START" )
				return Ese_eRacingClientNotificationType::ACTIVITY_START;
			if ( str == "REWARD_PLAYER" )
				return Ese_eRacingClientNotificationType::REWARD_PLAYER;
			if ( str == "EXIT" )
				return Ese_eRacingClientNotificationType::EXIT;
			if ( str == "REPLAY" )
				return Ese_eRacingClientNotificationType::REPLAY;
			if ( str == "REMOVE_PLAYER" )
				return Ese_eRacingClientNotificationType::REMOVE_PLAYER;
			if ( str == "LEADERBOARD_UPDATED" )
				return Ese_eRacingClientNotificationType::LEADERBOARD_UPDATED;
			throw new out_of_range::exception();
		}

	};

	// Enum: eResultType \\ 
	enum class Ese_eResultType { NOTFOUND, INVITESENT, OUTOFRANGE, ALREADYTRADING, GENERALERROR };
	class se_eResultType {
		template<int i>
		std::string get() {
			if ( i == Ese_eResultType::NOTFOUND )
				return "NOTFOUND";
			if ( i == Ese_eResultType::INVITESENT )
				return "INVITESENT";
			if ( i == Ese_eResultType::OUTOFRANGE )
				return "OUTOFRANGE";
			if ( i == Ese_eResultType::ALREADYTRADING )
				return "ALREADYTRADING";
			if ( i == Ese_eResultType::GENERALERROR )
				return "GENERALERROR";
			throw new out_of_range::exception();
		}

		template<unsigned char* str>
		Ese_eResultType get() {
			if ( str == "NOTFOUND" )
				return Ese_eResultType::NOTFOUND;
			if ( str == "INVITESENT" )
				return Ese_eResultType::INVITESENT;
			if ( str == "OUTOFRANGE" )
				return Ese_eResultType::OUTOFRANGE;
			if ( str == "ALREADYTRADING" )
				return Ese_eResultType::ALREADYTRADING;
			if ( str == "GENERALERROR" )
				return Ese_eResultType::GENERALERROR;
			throw new out_of_range::exception();
		}

	};

	// Enum: eStunState \\ 
	enum class Ese_eStunState { PUSH, POP };
	class se_eStunState {
		template<int i>
		std::string get() {
			if ( i == Ese_eStunState::PUSH )
				return "PUSH";
			if ( i == Ese_eStunState::POP )
				return "POP";
			throw new out_of_range::exception();
		}

		template<unsigned char* str>
		Ese_eStunState get() {
			if ( str == "PUSH" )
				return Ese_eStunState::PUSH;
			if ( str == "POP" )
				return Ese_eStunState::POP;
			throw new out_of_range::exception();
		}

	};

	// Enum: eTerminateType \\ 
	enum class Ese_eTerminateType { range, user, fromInteraction };
	class se_eTerminateType {
		template<int i>
		std::string get() {
			if ( i == Ese_eTerminateType::range )
				return "range";
			if ( i == Ese_eTerminateType::user )
				return "user";
			if ( i == Ese_eTerminateType::fromInteraction )
				return "fromInteraction";
			throw new out_of_range::exception();
		}

		template<unsigned char* str>
		Ese_eTerminateType get() {
			if ( str == "range" )
				return Ese_eTerminateType::range;
			if ( str == "user" )
				return Ese_eTerminateType::user;
			if ( str == "fromInteraction" )
				return Ese_eTerminateType::fromInteraction;
			throw new out_of_range::exception();
		}

	};

	// Enum: eTriggerOperator \\ 
	enum class Ese_eTriggerOperator { INVALID_OPER, EQUAL, NOT_EQUAL, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL };
	class se_eTriggerOperator {
		template<int i>
		std::string get() {
			if ( i == Ese_eTriggerOperator::INVALID_OPER )
				return "INVALID_OPER";
			if ( i == Ese_eTriggerOperator::EQUAL )
				return "EQUAL";
			if ( i == Ese_eTriggerOperator::NOT_EQUAL )
				return "NOT_EQUAL";
			if ( i == Ese_eTriggerOperator::GREATER )
				return "GREATER";
			if ( i == Ese_eTriggerOperator::GREATER_EQUAL )
				return "GREATER_EQUAL";
			if ( i == Ese_eTriggerOperator::LESS )
				return "LESS";
			if ( i == Ese_eTriggerOperator::LESS_EQUAL )
				return "LESS_EQUAL";
			throw new out_of_range::exception();
		}

		template<unsigned char* str>
		Ese_eTriggerOperator get() {
			if ( str == "INVALID_OPER" )
				return Ese_eTriggerOperator::INVALID_OPER;
			if ( str == "EQUAL" )
				return Ese_eTriggerOperator::EQUAL;
			if ( str == "NOT_EQUAL" )
				return Ese_eTriggerOperator::NOT_EQUAL;
			if ( str == "GREATER" )
				return Ese_eTriggerOperator::GREATER;
			if ( str == "GREATER_EQUAL" )
				return Ese_eTriggerOperator::GREATER_EQUAL;
			if ( str == "LESS" )
				return Ese_eTriggerOperator::LESS;
			if ( str == "LESS_EQUAL" )
				return Ese_eTriggerOperator::LESS_EQUAL;
			throw new out_of_range::exception();
		}

	};

	// Enum: eTriggerStat \\ 
	enum class Ese_eTriggerStat { INVALID_STAT, HEALTH, ARMOR, IMAGINATION };
	class se_eTriggerStat {
		template<int i>
		std::string get() {
			if ( i == Ese_eTriggerStat::INVALID_STAT )
				return "INVALID_STAT";
			if ( i == Ese_eTriggerStat::HEALTH )
				return "HEALTH";
			if ( i == Ese_eTriggerStat::ARMOR )
				return "ARMOR";
			if ( i == Ese_eTriggerStat::IMAGINATION )
				return "IMAGINATION";
			throw new out_of_range::exception();
		}

		template<unsigned char* str>
		Ese_eTriggerStat get() {
			if ( str == "INVALID_STAT" )
				return Ese_eTriggerStat::INVALID_STAT;
			if ( str == "HEALTH" )
				return Ese_eTriggerStat::HEALTH;
			if ( str == "ARMOR" )
				return Ese_eTriggerStat::ARMOR;
			if ( str == "IMAGINATION" )
				return Ese_eTriggerStat::IMAGINATION;
			throw new out_of_range::exception();
		}

	};

}

#endif // !_STRINGENUMS_HPP__