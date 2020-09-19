#ifndef __ENTITY__GM__EnableRebuild_HPP__
#define __ENTITY__GM__EnableRebuild_HPP__
#include "Entity/GameMessages.hpp"
#include "Enums/EFailReason.hpp"
#include "DataTypes/LWOOBJID.hpp"

namespace GM {
	struct EnableRebuild : GMBase {

		inline constexpr static ::Enums::EGameMessageID GetID() {
			return ::Enums::EGameMessageID::SERVER_ENABLE_REBUILD;
		}

		bool bEnable = false;
		bool bFail = false;
		bool bSuccess = false;
		Enums::EFailReason eFailReason = ::Enums::EFailReason::REASON_NOT_GIVEN;
		std::float_t fDuration;
		DataTypes::LWOOBJID user;

		EnableRebuild() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE(bs, bEnable);
			GM_VAR_SERIALIZE(bs, bFail);
			GM_VAR_SERIALIZE(bs, bSuccess);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, eFailReason, ::Enums::EFailReason::REASON_NOT_GIVEN);
			GM_VAR_SERIALIZE(bs, fDuration);
			GM_VAR_SERIALIZE(bs, user);
		}
	};
}

#endif