#ifndef __ENTITY__GM__StartCelebrationEffect_HPP__
#define __ENTITY__GM__StartCelebrationEffect_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct StartCelebrationEffect : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_START_CELEBRATION_EFFECT;
		}

		std::u16string animation = u"";
		std::int32_t backgroundObject = 11164;
		std::int32_t cameraPathLOT = 12458;
		std::float_t celeLeadIn = 1.0f;
		std::float_t celeLeadOut = 0.8f;
		std::int32_t celebrationID = -1;
		std::float_t duration = 0;
		std::uint32_t iconID = 0;
		std::u16string mainText = u"";
		std::string mixerProgram = "";
		std::string musicCue = "";
		std::string pathNodeName = "";
		std::string soundGUID = "";
		std::u16string subText = u"";

		StartCelebrationEffect() : GMBase() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE_WSTRING(bs, animation);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, backgroundObject, 11164);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, cameraPathLOT, 12458);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, celeLeadIn, 1.0f);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, celeLeadOut, 0.8f);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, celebrationID, -1);
			GM_VAR_SERIALIZE(bs, duration);
			GM_VAR_SERIALIZE(bs, iconID);
			GM_VAR_SERIALIZE_WSTRING(bs, mainText);
			GM_VAR_SERIALIZE_STRING(bs, mixerProgram);
			GM_VAR_SERIALIZE_STRING(bs, musicCue);
			GM_VAR_SERIALIZE_STRING(bs, pathNodeName);
			GM_VAR_SERIALIZE_STRING(bs, soundGUID);
			GM_VAR_SERIALIZE_WSTRING(bs, subText);
		}
	};
}

#endif