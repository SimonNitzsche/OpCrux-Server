#ifndef __ENTITY__GM__BuyFromVendor_HPP__
#define __ENTITY__GM__BuyFromVendor_HPP__
#include "Entity/GameMessages.hpp"

#include "Enums/EKillType.hpp"

namespace GM {
	struct BuyFromVendor : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_BUY_FROM_VENDOR;
		}

		BuyFromVendor() : GMBase() {}

		bool confirmed = false;
		std::int32_t count = 1;
		std::int32_t item;

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, confirmed, false);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, count, 1);
			GM_VAR_DESERIALIZE(bs, item);
		}
	};
}

#endif