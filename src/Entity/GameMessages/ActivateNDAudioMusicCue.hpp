#ifndef __ENTITY__GM__ActivateNDAudioMusicCue_HPP__
#define __ENTITY__GM__ActivateNDAudioMusicCue_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct ActivateNDAudioMusicCue : GMBase {

		std::string m_NDAudioMusicCueName = "";

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::INTERN_ACTIVATE_ND_AUDIO_MUSIC_CUE;
		}

		ActivateNDAudioMusicCue() : GMBase() {}
	};
}

#endif