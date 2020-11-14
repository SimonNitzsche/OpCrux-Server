#ifndef __ENTITY__GM__MatchUpdate_HPP__
#define __ENTITY__GM__MatchUpdate_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct MatchUpdate : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_MATCH_UPDATE;
		}

		LDFCollection data;
		std::int32_t type;

		MatchUpdate() : GMBase() {}

		void Serialize(RakNet::BitStream* bs) {
			std::u16string strData = LDFUtils::PackCollectionToWString(data);
			GM_VAR_SERIALIZE_WSTRING(bs, strData);
			GM_VAR_SERIALIZE(bs, type);
		}

	};
}

#endif