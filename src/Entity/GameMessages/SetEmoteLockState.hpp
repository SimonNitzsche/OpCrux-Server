#ifndef __ENTITY__GM__SetEmoteLockState_HPP__
#define __ENTITY__GM__SetEmoteLockState_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct SetEmoteLockState : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_SET_EMOTE_LOCK_STATE;
		}

		bool bLock = false;
		std::int32_t emoteID;

		SetEmoteLockState() : GMBase() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE(bs, bLock);
			GM_VAR_SERIALIZE(bs, emoteID);
		}
	};
}

#endif