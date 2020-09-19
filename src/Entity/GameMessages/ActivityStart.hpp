#ifndef __ENTITY__GM__ActivityStart_HPP__
#define __ENTITY__GM__ActivityStart_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct ActivityStart : GMBase {

		int iPathIndex;

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_ACTIVITY_START;
		}

		ActivityStart() {}
		void Deserialize(RakNet::BitStream* bs) {

		}

		void TriggerEvent(Entity::GameObject* sender, Entity::GameObject* target) {

		}
	};
}

#endif