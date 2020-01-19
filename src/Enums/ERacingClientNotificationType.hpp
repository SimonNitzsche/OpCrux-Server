#ifndef __ENUMS__ERACINGCLIENTNOTIFICATIONTYPE_HPP__
#define __ENUMS__ERACINGCLIENTNOTIFICATIONTYPE_HPP__

#include <memory>

namespace Enums {
	enum class ERacingClientNotificationType : std::int32_t {
		INVALID = 0,
		ACTIVITY_START = 1,
		REWARD_PLAYER = 2,
		EXIT = 3,
		REPLAY = 4,
		REMOVE_PLAYER = 5,
		LEADERBOARD_UPDATED = 6
	};
}

#endif