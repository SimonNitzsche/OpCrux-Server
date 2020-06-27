#ifndef __ENTITY__GM__READYFORUPDATES_HPP__
#define __ENTITY__GM__READYFORUPDATES_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct ReadyForUpdates : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_READY_FOR_UPDATES;
		}

		ReadyForUpdates() {}

		void Deserialize(RakNet::BitStream* bs) {

		}

		void TriggerEvent(Entity::GameObject* sender, Entity::GameObject* target) {

		}
	};
}

#endif