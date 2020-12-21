#ifndef __ENTITY__GM__REMOVEITEMFROMINVENTORY_HPP__
#define __ENTITY__GM__REMOVEITEMFROMINVENTORY_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct RemoveItemFromInventory : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::DUAL_REMOVE_ITEM_FROM_INVENTORY;
		}

		RemoveItemFromInventory() {}

		bool Confirmed = false;
		bool DeleteItem = true;
		bool OutSuccess = false;
		int InventoryType = (int)Enums::EInventoryType::Items;
		int ItemType = (int)Enums::EItemType::Invalid;
		LDFCollection ExtraInfo;
		bool ForceDeletion = true;
		DataTypes::LWOOBJID Source = DataTypes::LWOOBJID();
		DataTypes::LWOOBJID Item = DataTypes::LWOOBJID();
		std::uint32_t ItemLot = 0;
		DataTypes::LWOOBJID Requesting = DataTypes::LWOOBJID();
		std::uint32_t Delta = 1;
		std::uint32_t TotalItems = 0;
		DataTypes::LWOOBJID SubKey = DataTypes::LWOOBJID();
		DataTypes::LWOOBJID TradeId = DataTypes::LWOOBJID();


		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, Confirmed, false);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, DeleteItem, true);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, OutSuccess, false);

			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, InventoryType, (int) Enums::EInventoryType::Items);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, ItemType, (int) Enums::EItemType::Invalid);

			GM_VAR_SERIALIZE_LDF(bs, ExtraInfo);

			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, ForceDeletion, true);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, Source, DataTypes::LWOOBJID());
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, Item, DataTypes::LWOOBJID());

			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, ItemLot, 0);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, Requesting, DataTypes::LWOOBJID());
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, Delta, 1);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, TotalItems, 0);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, SubKey, DataTypes::LWOOBJID());
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, TradeId, DataTypes::LWOOBJID());
		}

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, Confirmed, false);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, DeleteItem, true);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, OutSuccess, false);

			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, InventoryType, (int)Enums::EInventoryType::Items);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, ItemType, (int)Enums::EItemType::Invalid);

			std::u16string wExtraInfo;
			GM_VAR_DESERIALIZE_WSTRING(bs, wExtraInfo);
			ExtraInfo = LDFUtils::ParseCollectionFromWString(wExtraInfo);

			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, ForceDeletion, true);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, Source, DataTypes::LWOOBJID());
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, Item, DataTypes::LWOOBJID());

			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, ItemLot, 0);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, Requesting, DataTypes::LWOOBJID());
			
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, Delta, 1);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, TotalItems, 0);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, SubKey, DataTypes::LWOOBJID());
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, TradeId, DataTypes::LWOOBJID());
		}

		void TriggerEvent(Entity::GameObject* sender, Entity::GameObject* target) {

		}
	};
}

#endif