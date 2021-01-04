#ifndef __ENTITY__GM__PlayCinematic_HPP__
#define __ENTITY__GM__PlayCinematic_HPP__
#include "Entity/GameMessages.hpp"

namespace Enums {
	enum class EEndBehavior : std::uint32_t {
		RETURN,
		WAIT
	};
}

namespace GM {
	struct PlayCinematic : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_PLAY_CINEMATIC;
		}

		bool allowGhostUpdates = true;
		bool bCloseMultiInteract = false;
		bool bSendServerNotify = false;
		bool bUseControlledObjectForAudioListener = false;
		Enums::EEndBehavior endBehavior = Enums::EEndBehavior::RETURN;
		bool hidePlayerDuringCine = false;
		std::float_t leadIn = -1.0f;
		bool leavePlayerLockedWhenFinished = false;
		bool lockPlayer = true;
		std::u16string pathName = u"";
		bool result = false;
		bool skipIfSamePath = false;
		std::float_t startTimeAdvance = 0.0f;

		PlayCinematic() : GMBase() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, allowGhostUpdates, true);
			GM_VAR_SERIALIZE(bs, bCloseMultiInteract);
			GM_VAR_SERIALIZE(bs, bSendServerNotify);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, bUseControlledObjectForAudioListener, false);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, endBehavior, Enums::EEndBehavior::RETURN);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, hidePlayerDuringCine, false);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, leadIn, -1.0f);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, leavePlayerLockedWhenFinished, false);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, lockPlayer, true);
			GM_VAR_SERIALIZE_WSTRING(bs, pathName);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, result, false);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, skipIfSamePath, false);
			GM_VAR_SERIALIZE(bs, startTimeAdvance);
		}

	};
}

#endif