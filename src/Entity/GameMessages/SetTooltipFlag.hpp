#ifndef __ENTITY__GM__SetTooltipFlag_HPP__
#define __ENTITY__GM__SetTooltipFlag_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct SetTooltipFlag : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_SET_TOOLTIP_FLAG;
		}

		bool bFlag;
		std::int32_t iToolTip;

		SetTooltipFlag() : GMBase() {}

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE(bs, bFlag);
			GM_VAR_DESERIALIZE(bs, iToolTip);
		}
	};
}

#endif