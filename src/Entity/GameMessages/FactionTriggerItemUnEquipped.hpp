#ifndef __ENTITY__GM__FactionTriggerItemUnEquipped_HPP__
#define __ENTITY__GM__FactionTriggerItemUnEquipped_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct FactionTriggerItemUnEquipped : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::INTERN_FACTION_TRIGGER_ITEM_UNEQUIPPED;
		}

		Entity::GameObject* itemID = nullptr;
		Entity::GameObject* playerID = nullptr;

		FactionTriggerItemUnEquipped() : GMBase() {}
	};
}

#endif