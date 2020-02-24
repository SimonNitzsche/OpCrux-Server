#ifndef __ENTITY__GM__DropClientLoot_HPP__
#define __ENTITY__GM__DropClientLoot_HPP__
#include "Entity/GameMessages.hpp"
#include "Enums/EFailReason.hpp"
#include "DataTypes/LWOOBJID.hpp"

namespace GM {
	struct DropClientLoot : GMBase {

		inline constexpr static ::Enums::EGameMessageID GetID() {
			return ::Enums::EGameMessageID::SERVER_DROP_CLIENT_LOOT;
		}

		bool bUsePosition = false;
		DataTypes::Vector3 finalPosition;
		std::int32_t iCurrency = 0;
		std::int32_t itemTemplate = 0;
		DataTypes::LWOOBJID lootID;
		DataTypes::LWOOBJID owner;
		DataTypes::LWOOBJID sourceObj;
		DataTypes::Vector3 spawnPosition;

		DropClientLoot() : GMBase() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, bUsePosition, false);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, finalPosition, DataTypes::Vector3::zero());
			GM_VAR_SERIALIZE(bs, iCurrency);
			GM_VAR_SERIALIZE(bs, itemTemplate);
			GM_VAR_SERIALIZE(bs, lootID);
			GM_VAR_SERIALIZE(bs, owner);
			GM_VAR_SERIALIZE(bs, sourceObj);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, spawnPosition, DataTypes::Vector3::zero());
		}
	};
}

#endif