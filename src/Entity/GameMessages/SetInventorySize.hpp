#ifndef __ENTITY__GM__SetInventorySize_HPP__
#define __ENTITY__GM__SetInventorySize_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct SetInventorySize : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_SET_INVENTORY_SIZE;
		}

		std::int32_t inventoryType;
		std::int32_t size;

		SetInventorySize() : GMBase() {}
	
		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE(bs, inventoryType);
			GM_VAR_SERIALIZE(bs, size);
		}
	};
}

#endif