#ifndef __ENTITY__GM__HasBeenCollected_HPP__
#define __ENTITY__GM__HasBeenCollected_HPP__
#include "Entity/GameMessages.hpp"
#include "Missions/MissionManager.hpp"

namespace GM {
	struct HasBeenCollected : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_HAS_BEEN_COLLECTED;
		}

		DataTypes::LWOOBJID playerID;

		HasBeenCollected() {}

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE(bs, playerID);
		}

		void TriggerEvent(Entity::GameObject* sender, Entity::GameObject* target) {
			if (sender->GetObjectID() != playerID) return;
			target->OnHasBeenCollected(sender, *this);
		}
	};
}

#endif