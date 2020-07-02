#ifndef __ENTITY__GM__PopEquippedItemsState_HPP__
#define __ENTITY__GM__PopEquippedItemsState_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct PopEquippedItemsState : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_POP_EQUIPPED_ITEMS_STATE;
		}

		PopEquippedItemsState() : GMBase() {}

		void Deserialize(RakNet::BitStream* bs) {

		}

		void TriggerEvent(Entity::GameObject* sender, Entity::GameObject* target) {

		}
	};
}

#endif