#ifndef __ENTITY__GM__StartArrangingWithItem_HPP__
#define __ENTITY__GM__StartArrangingWithItem_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct StartArrangingWithItem : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_START_ARRANGING_WITH_ITEM;
		}

		bool bFirstTime = true;
		DataTypes::LWOOBJID buildAreaID = 0ULL;
		DataTypes::Vector3 buildStartPOS;
		std::int32_t sourceBAG=0;
		DataTypes::LWOOBJID sourceID=0ULL;
		std::uint32_t sourceLOT=-1;
		std::int32_t sourceTYPE=0;
		DataTypes::LWOOBJID targetID=0ULL;
		std::uint32_t targetLOT=-1;
		DataTypes::Vector3 targetPOS;
		std::int32_t targetTYPE=0;

		StartArrangingWithItem() : GMBase() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, bFirstTime, true);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, buildAreaID, 0ULL);
			GM_VAR_SERIALIZE(bs, buildStartPOS);
			GM_VAR_SERIALIZE(bs, sourceBAG);
			GM_VAR_SERIALIZE(bs, sourceID);
			GM_VAR_SERIALIZE(bs, sourceLOT);
			GM_VAR_SERIALIZE(bs, sourceTYPE);
			GM_VAR_SERIALIZE(bs, targetID);
			GM_VAR_SERIALIZE(bs, targetLOT);
			GM_VAR_SERIALIZE(bs, targetPOS);
			GM_VAR_SERIALIZE(bs, targetTYPE);
		}
	};
}

#endif