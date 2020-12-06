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
	};
}

#endif