#ifndef __ENTITY__GM__SetCurrency_HPP__
#define __ENTITY__GM__SetCurrency_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct SetCurrency : GMBase {
		std::int64_t currency;
		std::int32_t lootType = 0;
		DataTypes::Vector3 position;
		std::int32_t sourceLOT = 0;
		DataTypes::LWOOBJID sourceObject = 0ULL;
		DataTypes::LWOOBJID sourceTradeID = 0ULL;
		std::int32_t sourceType = 0;

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_SET_CURRENCY;
		}

		SetCurrency() : GMBase() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE(bs, currency);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, lootType, 0);
			GM_VAR_SERIALIZE(bs, position);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, sourceLOT, 0);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, sourceObject, 0ULL);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, sourceTradeID, 0ULL);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, sourceType, 0);
		}
	};
}
#endif
