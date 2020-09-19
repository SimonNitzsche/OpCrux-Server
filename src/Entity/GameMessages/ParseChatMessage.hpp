#ifndef __ENTITY__GM__ParseChatMessage_HPP__
#define __ENTITY__GM__ParseChatMessage_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct ParseChatMessage : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_PARSE_CHAT_MESSAGE;
		}

		ParseChatMessage() : GMBase() {}

		std::int16_t iClientState;
		std::u16string wsString;

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE(bs, iClientState);
			GM_VAR_DESERIALIZE_WSTRING(bs, wsString);
		}
	};
}

#endif