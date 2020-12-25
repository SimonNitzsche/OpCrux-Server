#ifndef __ENTITY__GM__MoveItemInInventory_HPP__
#define __ENTITY__GM__MoveItemInInventory_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct MoveItemInInventory : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::DUAL_MOVE_ITEM_IN_INVENTORY;
		}

		std::int32_t destInvType = 0;
		DataTypes::LWOOBJID objectID;
		std::int32_t invType;
		std::int32_t responseCode;
		std::int32_t slot;

		MoveItemInInventory() {}

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, destInvType, 0);
			GM_VAR_DESERIALIZE(bs, objectID);
			GM_VAR_DESERIALIZE(bs, invType);
			GM_VAR_DESERIALIZE(bs, responseCode);
			GM_VAR_DESERIALIZE(bs, slot);
		}
	};
}

#endif