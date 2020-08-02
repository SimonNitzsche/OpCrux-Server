#ifndef __ENTITY__GM__EQUIP_INVENTORY_HPP__
#define __ENTITY__GM__EQUIP_INVENTORY_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct EquipInventory : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::DUAL_EQUIP_ITEM;
		}

		EquipInventory() : GMBase() {}

		bool bIgnoreCooldown;
		bool bOutSuccess;
		DataTypes::LWOOBJID itemToEquip;

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, bIgnoreCooldown, false);
			GM_VAR_DESERIALIZE(bs, bOutSuccess);
			GM_VAR_DESERIALIZE(bs, itemToEquip);
		}
	};
}

#endif
