#ifndef __ENTITY__GM__StartBuildingWithItem_HPP__
#define __ENTITY__GM__StartBuildingWithItem_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct StartBuildingWithItem : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_START_BUILDING_WITH_ITEM;
		}

		bool bFirstTime = true;
		bool bSuccess;
		int sourceBAG;
		DataTypes::LWOOBJID sourceID;
		std::uint32_t sourceLOT;
		std::int32_t sourceTYPE;
		DataTypes::LWOOBJID targetID;
		std::uint32_t targetLOT;
		DataTypes::Vector3 targetPOS;
		std::int32_t targetTYPE;

		StartBuildingWithItem() : GMBase() {}

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, bFirstTime, true);
			GM_VAR_DESERIALIZE(bs, bSuccess);
			GM_VAR_DESERIALIZE(bs, sourceBAG);
			GM_VAR_DESERIALIZE(bs, sourceID);
			GM_VAR_DESERIALIZE(bs, sourceLOT);
			GM_VAR_DESERIALIZE(bs, sourceTYPE);
			GM_VAR_DESERIALIZE(bs, targetID);
			GM_VAR_DESERIALIZE(bs, targetLOT);
			GM_VAR_DESERIALIZE(bs, targetPOS);
			GM_VAR_DESERIALIZE(bs, targetTYPE);
		}

		void TriggerEvent(Entity::GameObject* sender, Entity::GameObject* target) {
			Logger::log("WRLD", "Triggered StartBuildingWithItem");
		}
	};
}

#endif