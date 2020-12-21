#ifndef __ENTITY__GM__UseNonEquipmentItem_HPP__
#define __ENTITY__GM__UseNonEquipmentItem_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct UseNonEquipmentItem : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_USE_NON_EQUIPMENT_ITEM;
		}

		UseNonEquipmentItem() : GMBase() {}

		DataTypes::LWOOBJID itemToUse = DataTypes::LWOOBJID();

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE(bs, itemToUse);
		}
	};
}

#endif
