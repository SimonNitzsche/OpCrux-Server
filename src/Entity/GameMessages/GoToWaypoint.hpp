#ifndef __ENTITY__GM__GoToWaypoint_HPP__
#define __ENTITY__GM__GoToWaypoint_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct GoToWaypoint : public GMBase {
		std::int32_t iPathIndex = 0;
		bool bAllowPathingDirectionChange = true;
		bool bStopAtWaypoint = true;

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::INTERN_GO_TO_WAYPOINT;
		}

		GoToWaypoint() : GMBase() {}
	};
}

#endif