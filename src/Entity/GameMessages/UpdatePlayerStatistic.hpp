#ifndef __ENTITY__GM__UpdatePlayerStatistic_HPP__
#define __ENTITY__GM__UpdatePlayerStatistic_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct UpdatePlayerStatistic : GMBase {
		std::uint32_t updateID;
		std::int64_t updateValue = 1LL;

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_UPDATE_PLAYER_STATISTIC;
		}

		UpdatePlayerStatistic() : GMBase() {}
		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE(bs, updateValue);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, updateValue, 1LL);
		}
	};
}
#endif
