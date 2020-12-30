#ifndef __ENTITY__GM__UpdateReputation_HPP__
#define __ENTITY__GM__UpdateReputation_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct UpdateReputation : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_UPDATE_REPUTATION;
		}

		std::int64_t iReputation;

		UpdateReputation() : GMBase() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE(bs, iReputation);
		}
	};
}

#endif