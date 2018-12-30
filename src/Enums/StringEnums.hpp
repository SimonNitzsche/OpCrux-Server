#ifndef _STRINGENUM_HPP__
#define _STRINGENUM_HPP__

#include <map>
#include <vector>
#include <set>
#include <string>
#include "Utils/Logger.hpp"

using namespace std;

namespace Enums::Strings {

	class StringEnum {
		// A Vector of Values.
		std::vector<std::string> values;

		public:
			// Constructs a String Enum with the defined strings.
			StringEnum(std::vector<std::string> in) : values(in){}

			// The Array Operator.
			const int & operator[] (const std::string in) {
				// Loop until we find the value, if we did find it, return it.
				for (int i = 0; i < values.size(); ++i)
					if (values[i] == in)
						return i;

				// Value not found! Throw!
				Logger::log("StringEnums", "Can't find enum property: " + in, LogType::UNEXPECTED);
				throw new runtime_error("Can't find enum property!");
			}
	};

	/*
		A constant map of string enums.
	*/
	const std::map<std::string, StringEnum> get = {
		{ "eBuildType",
			StringEnum({
				"BUILD_NOWHERE",
				"BUILD_IN_WORLD"
				"BUILD_ON_PROPERTY"
			})
		},
		{ "CinematicEvent",
			StringEnum({
				"STARTED",
				"WAYPOINT"
				"ENDED"
			})
		},
		{ "eCyclingMode",
			StringEnum({
				"ALLOW_CYCLING_TEAMMATES",
				"DISALLOW_CYCLING"
			})
		},
		{ "DeleteReason",
			StringEnum({
				"PICKING_MODEL_UP",
				"RETURNING_MODEL_TO_INVENTORY",
				"BREAKING_MODEL_APART"
			})
		},
		{ "EndBehavior",
			StringEnum({
				"RETURN",
				"WAIT"
			})
		},
		{ "FailReason",
			StringEnum({
				"REASON_NOT_GIVEN",
				"REASON_OUT_OF_IMAGINATION",
				"REASON_CANCELED_EARLY",
				"REASON_BUILD_ENDED"
			})
		},
		{ "eImmunityState",
			StringEnum({
				"PUSH",
				"POP"
			})
		},
		{ "eKillType",
			StringEnum({
				"VIOLENT",
				"SILENT"
			})
		},
		{ "eMissionLockState",
			StringEnum({
				"LOCKED",
				"NEW",
				"UNLOCKED"
			})
		},
		{ "eNotifyType",
			StringEnum({
				"SUCCESS",
				"QUIT",
				"FAILED",
				"BEGIN",
				"READY",
				"NAMINGPET"
			})
		},
		{ "eObjectWorldState",
			StringEnum({
				"WORLDSTATE_INWORLD",
				"WORLDSTATE_ATTACHED",
				"WORLDSTATE_INVENTORY"
			})
		},
		{ "eRacingClientNotificationType",
			StringEnum({
				"INVALID",
				"ACTIVITY_START",
				"REWARD_PLAYER",
				"EXIT",
				"REPLAY",
				"REMOVE_PLAYER",
				"LEADERBOARD_UPDATED"
			})
		},
		{ "eResultType",
			StringEnum({
				"NOTFOUND",
				"INVITESENT",
				"OUTOFRANGE",
				"ALREADYTRADING",
				"GENERALERROR"
			})
		},
		{ "eStunState",
			StringEnum({
				"PUSH",
				"POP"
			})
		},
		{ "eTerminateType",
			StringEnum({
				"range",
				"user",
				"fromInteraction"
			})
		},
		{ "eTriggerOperator",
			StringEnum({
				"INVALID_OPER",
				"EQUAL",
				"NOT_EQUAL",
				"GREATER",
				"GREATER_EQUAL",
				"LESS",
				"LESS_EQUAL"
			})
		},
		{ "eTriggerStat",
			StringEnum({
				"INVALID_STAT",
				"HEALTH",
				"ARMOR",
				"IMAGINATION"
			})
		}
	};
}

#endif // !_STRINGENUM_HPP__