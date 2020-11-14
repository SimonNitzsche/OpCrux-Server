#ifndef __ENTITY__GM__MatchRequest_HPP__
#define __ENTITY__GM__MatchRequest_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct MatchRequest : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_MATCH_REQUEST;
		}

		DataTypes::LWOOBJID activator;
		LDFCollection playerChoices;

		/* todo: additional players */

		std::int32_t type;
		std::int32_t value;

		MatchRequest() {}

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE(bs, activator);

			std::u16string strCollection;
			GM_VAR_DESERIALIZE_WSTRING(bs, strCollection);
			playerChoices = LDFUtils::ParseCollectionFromWString(strCollection);

			GM_VAR_DESERIALIZE(bs, type);
			GM_VAR_DESERIALIZE(bs, value);
		}
	};
}

#endif