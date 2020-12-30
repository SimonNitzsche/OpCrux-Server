#ifndef __ENTITY__GM__ModifyLegoScore_HPP__
#define __ENTITY__GM__ModifyLegoScore_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct ModifyLegoScore : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_MODIFY_LEGO_SCORE;
		}

		std::int64_t score;
		std::int32_t sourceType = 0;

		ModifyLegoScore() : GMBase() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE(bs, score);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, sourceType, 0);
		}
	};
}

#endif