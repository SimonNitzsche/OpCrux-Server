#ifndef __ENTITY__GM__ClientItemConsumed_HPP__
#define __ENTITY__GM__ClientItemConsumed_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct ClientItemConsumed : GMBase {
		DataTypes::LWOOBJID item;

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_CLIENT_ITEM_CONSUMED;
		}

		ClientItemConsumed() : GMBase() {}
		
		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE(bs, item);
		}
	};
}

#endif