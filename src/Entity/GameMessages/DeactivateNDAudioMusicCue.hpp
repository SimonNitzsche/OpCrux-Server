#ifndef __ENTITY__GM__DeactivateNDAudioMusicCue_HPP__
#define __ENTITY__GM__DeactivateNDAudioMusicCue_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct DeactivateNDAudioMusicCue : GMBase {

		std::string m_NDAudioMusicCueName = "";

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::INTERN_DEACTIVATE_ND_AUDIO_MUSIC_CUE;
		}

		DeactivateNDAudioMusicCue() : GMBase() {}
	};
}

#endif