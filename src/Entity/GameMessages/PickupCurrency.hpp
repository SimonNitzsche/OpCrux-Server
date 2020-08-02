#ifndef __ENTITY__GM__PickupCurrency_HPP__
#define __ENTITY__GM__PickupCurrency_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct PickupCurrency : GMBase {
		std::uint32_t currency;
		DataTypes::Vector3 position;

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_PICKUP_CURRENCY;
		}

		PickupCurrency() : GMBase() {}
		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE(bs, currency);
			GM_VAR_DESERIALIZE(bs, position);
		}
	};
}
#endif