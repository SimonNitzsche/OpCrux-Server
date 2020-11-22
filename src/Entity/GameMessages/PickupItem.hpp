#ifndef __ENTITY__GM__PickupItem_HPP__
#define __ENTITY__GM__PickupItem_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct PickupItem : GMBase {
		DataTypes::LWOOBJID lootObjectID;
		DataTypes::LWOOBJID playerID;

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_PICKUP_ITEM;
		}

		PickupItem() : GMBase() {}
		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE(bs, lootObjectID);
			GM_VAR_DESERIALIZE(bs, playerID);
		}

		void TriggerEvent(Entity::GameObject* sender, Entity::GameObject* target) {
			Entity::GameObject * lootObject = sender->GetZoneInstance()->objectsManager->GetObjectByID(lootObjectID);
			Entity::GameObject* player = sender->GetZoneInstance()->objectsManager->GetObjectByID(playerID);

			if (lootObject != nullptr && player != nullptr) {
				player->PickupLoot(lootObject);
			}
			else {
				Logger::log("WRLD", "PickupItem: Unable to pickup unknown object " + std::to_string(std::uint64_t(lootObjectID)), LogType::WARN);
			}
		}
	};
}

#endif