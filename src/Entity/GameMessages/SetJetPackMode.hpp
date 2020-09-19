#ifndef __ENTITY__GM__SetJetPackMode_HPP__
#define __ENTITY__GM__SetJetPackMode_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct SetJetPackMode : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_SET_JETPACK_MODE;
		}

		bool bBypassChecks = false;
		bool bDoHover = false;
		bool bUse = false;
        std::int16_t effectID = -1;
		std::float_t fAirspeed = 10;
        std::float_t fMaxAirspeed = 15;
        std::float_t fVertVel = 1;
		std::int16_t iWarningEffectID = -1;

		SetJetPackMode() : GMBase() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, bBypassChecks, false);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, bDoHover, false);
			GM_VAR_SERIALIZE(bs, bUse);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, effectID, -1);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, fAirspeed, 10);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, fMaxAirspeed, 15);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, fVertVel, 1);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, iWarningEffectID, -1);
		}
	};
}

#endif