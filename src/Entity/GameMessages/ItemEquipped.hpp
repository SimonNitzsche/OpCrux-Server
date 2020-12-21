#ifndef __ENTITY__GM__ItemEquipped_HPP__
#define __ENTITY__GM__ItemEquipped_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct ItemEquipped : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::INTERN_ITEM_EQUIPPED;
		}

		Entity::GameObject* approvingID = nullptr;
		bool bIsEquipped = false;
		Entity::GameObject* otherObjectID = nullptr;
		Entity::GameObject* playerID = nullptr;

		ItemEquipped() : GMBase() {}
	};
}

#endif