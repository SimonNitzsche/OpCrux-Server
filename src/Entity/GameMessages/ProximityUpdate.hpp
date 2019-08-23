#ifndef __ENTITY__GM__ProximityUpdate_HPP__
#define __ENTITY__GM__ProximityUpdate_HPP__
#include "Entity/GameMessages.hpp"

#include "Entity/GameMessages/Resurrect.hpp"

namespace GM {
	struct ProximityUpdate : GMBase {

		std::string name;
		std::string status;
		Entity::GameObject * objId;

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::PROXIMITY_UPDATE;
		}

		ProximityUpdate() {}
		void Deserialize(RakNet::BitStream * bs) {

		}

		void TriggerEvent(Entity::GameObject * sender, Entity::GameObject * target) {

		}
	};
}

#endif