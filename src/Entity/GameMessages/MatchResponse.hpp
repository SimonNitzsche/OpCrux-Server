#ifndef __ENTITY__GM__MatchResponse_HPP__
#define __ENTITY__GM__MatchResponse_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct MatchResponse : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_MATCH_RESPONSE;
		}

		std::int32_t response;

		MatchResponse() : GMBase() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE(bs, response);
		}

	};
}

#endif