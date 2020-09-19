#ifndef __ENTITY__GM__AddItemToInventoryClientSync_HPP__
#define __ENTITY__GM__AddItemToInventoryClientSync_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct AddItemToInventoryClientSync : GMBase {
		bool bBound=false;
		bool bIsBOE=false;
		bool bIsBOP=false;
		std::int32_t eLootTypeSource;
		LDFCollection extraInfo;
		std::int32_t iObjTemplate;
		DataTypes::LWOOBJID iSubkey;
		std::int32_t invType = 0;
		std::uint32_t itemCount = 1;
		std::uint32_t itemsTotal = 0;
		DataTypes::LWOOBJID newObjID;
		DataTypes::Vector3 ni3FlyingLootPosit;
		bool showFlyingLoot = true;
		std::int32_t slotID;

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_ADD_ITEM_TO_INVENTORY_CLIENT_SYNC;
		}

		AddItemToInventoryClientSync() : GMBase() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE(bs, bBound);
			GM_VAR_SERIALIZE(bs, bIsBOE);
			GM_VAR_SERIALIZE(bs, bIsBOP);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, eLootTypeSource, 0);
			GM_VAR_SERIALIZE_LDF(bs, extraInfo);
			GM_VAR_SERIALIZE(bs, iObjTemplate);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, iSubkey, DataTypes::LWOOBJID());
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, invType, 0);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, itemCount, 1);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, itemsTotal, 0);
			GM_VAR_SERIALIZE(bs, newObjID);
			GM_VAR_SERIALIZE(bs, ni3FlyingLootPosit);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, showFlyingLoot, true);
			GM_VAR_SERIALIZE(bs, slotID);
		}
	};
}

#endif