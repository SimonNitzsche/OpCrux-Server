#ifndef __ENTITY__GM__FinishArrangingWithItem_HPP__
#define __ENTITY__GM__FinishArrangingWithItem_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct FinishArrangingWithItem : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_FINISH_ARRANGING_WITH_ITEM;
		}

		
		DataTypes::LWOOBJID buildAreaID = DataTypes::LWOOBJID();
		uint32_t newSourceBAG;
		DataTypes::LWOOBJID newSourceID;
		uint32_t newSourceLOT;
		uint32_t newSourceTYPE;
		DataTypes::LWOOBJID newTargetID;
		uint32_t newTargetLOT;
		uint32_t newTargetTYPE;
		DataTypes::Vector3 newtargetPOS;
		uint32_t oldItemBAG;
		DataTypes::LWOOBJID oldItemID;
		uint32_t oldItemLOT;
		uint32_t oldItemTYPE;

		FinishArrangingWithItem() : GMBase() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, buildAreaID, DataTypes::LWOOBJID());
			GM_VAR_SERIALIZE(bs, newSourceBAG);
			GM_VAR_SERIALIZE(bs, newSourceID);
			GM_VAR_SERIALIZE(bs, newSourceLOT);
			GM_VAR_SERIALIZE(bs, newSourceTYPE);
			GM_VAR_SERIALIZE(bs, newTargetID);
			GM_VAR_SERIALIZE(bs, newTargetLOT);
			GM_VAR_SERIALIZE(bs, newTargetTYPE);
			GM_VAR_SERIALIZE(bs, newtargetPOS);
			GM_VAR_SERIALIZE(bs, oldItemBAG);
			GM_VAR_SERIALIZE(bs, oldItemID);
			GM_VAR_SERIALIZE(bs, oldItemLOT);
			GM_VAR_SERIALIZE(bs, oldItemTYPE);

		}
	};
}

#endif
