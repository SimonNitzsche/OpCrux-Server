#ifndef __ENTITY__GM__UNEQUIP_INVENTORY_HPP__
#define __ENTITY__GM__UNEQUIP_INVENTORY_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct UnEquipInventory : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::DUAL_UNEQUIP_ITEM;
		}

		UnEquipInventory() : GMBase() {}

		bool bEvenIfDead;
		bool bIgnoreCooldown;
		bool bOutSuccess;
		DataTypes::LWOOBJID itemToUnEquip;
		DataTypes::LWOOBJID replacementObjectID = DataTypes::LWOOBJID();

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, bEvenIfDead, false);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, bIgnoreCooldown, false);
			GM_VAR_DESERIALIZE(bs, bOutSuccess);
			GM_VAR_DESERIALIZE(bs, itemToUnEquip);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, replacementObjectID, DataTypes::LWOOBJID());
		}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, bEvenIfDead, false);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, bIgnoreCooldown, false);
			GM_VAR_SERIALIZE(bs, bOutSuccess);
			GM_VAR_SERIALIZE(bs, itemToUnEquip);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, replacementObjectID, DataTypes::LWOOBJID());
		}
	};
}

#endif
