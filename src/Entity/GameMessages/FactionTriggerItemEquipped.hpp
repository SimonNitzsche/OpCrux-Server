#ifndef __ENTITY__GM__FactionTriggerItemEquipped_HPP__
#define __ENTITY__GM__FactionTriggerItemEquipped_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct FactionTriggerItemEquipped : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::INTERN_FACTION_TRIGGER_ITEM_EQUIPPED;
		}

		Entity::GameObject* itemID = nullptr;
		Entity::GameObject* playerID = nullptr;

		FactionTriggerItemEquipped() : GMBase() {}
	};
}

#endif