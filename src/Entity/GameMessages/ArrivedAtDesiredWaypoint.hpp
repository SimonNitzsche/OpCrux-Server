#ifndef __ENTITY__GM__ArrivedAtDesiredWaypoint_HPP__
#define __ENTITY__GM__ArrivedAtDesiredWaypoint_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct ArrivedAtDesiredWaypoint : GMBase {

		int iPathIndex;

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::ARRIVED_AT_DESIRED_WAYPOINT;
		}

		ArrivedAtDesiredWaypoint() {}
		void Deserialize(RakNet::BitStream * bs) {

		}

		void TriggerEvent(Entity::GameObject * sender, Entity::GameObject * target) {
			
		}
	};
}

#endif