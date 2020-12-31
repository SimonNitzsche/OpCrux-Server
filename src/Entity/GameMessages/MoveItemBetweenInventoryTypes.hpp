#ifndef __ENTITY__GM__MoveItemBetweenInventoryTypes_HPP__
#define __ENTITY__GM__MoveItemBetweenInventoryTypes_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct MoveItemBetweenInventoryTypes : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_MOVE_ITEM_BETWEEN_INVENTORY_TYPES;
		}

		uint32_t inventoryTypeA;
		uint32_t inventoryTypeB;
		int64_t objectID;
		bool showFlyingLoot = true;
		uint32_t stackCount = 1;
		uint32_t templateId = 0;

		MoveItemBetweenInventoryTypes() {}

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE(bs, inventoryTypeA);
			GM_VAR_DESERIALIZE(bs, inventoryTypeB);
			GM_VAR_DESERIALIZE(bs, objectID);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, showFlyingLoot, true);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, stackCount, 1);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, templateId, 0);

			Logger::log("TEST", std::to_string(inventoryTypeA));
			Logger::log("TEST", std::to_string(inventoryTypeB));
			Logger::log("TEST", std::to_string(objectID));
			Logger::log("TEST", showFlyingLoot ? "True" : "False");
			Logger::log("TEST", std::to_string(stackCount));
			Logger::log("TEST", std::to_string(templateId));
		}
	};
}

#endif