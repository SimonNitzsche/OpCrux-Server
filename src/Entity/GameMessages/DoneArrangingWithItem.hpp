#ifndef __ENTITY__GM__DoneArrangingWithItem_HPP__
#define __ENTITY__GM__DoneArrangingWithItem_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct DoneArrangingWithItem : GMBase {
		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_DONE_ARRANGING_WITH_ITEM;
		}

		DoneArrangingWithItem() {}

		int newSourceBAG;
		DataTypes::LWOOBJID newSourceID;
		uint32_t newSourceLOT;
		uint32_t newSourceType;
		DataTypes::LWOOBJID newTargetID;
		uint32_t newTargetLOT;
		uint32_t newTargetType;
		Vector3 newTargetPos;
		uint32_t oldItemBag;
		DataTypes::LWOOBJID oldItemID;
		uint32_t oldItemLot;
		uint32_t oldItemType;

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE(bs, newSourceBAG);
			GM_VAR_DESERIALIZE(bs, newSourceID);
			GM_VAR_DESERIALIZE(bs, newSourceLOT);
			GM_VAR_DESERIALIZE(bs, newSourceType);
			GM_VAR_DESERIALIZE(bs, newTargetID);
			GM_VAR_DESERIALIZE(bs, newTargetLOT);
			GM_VAR_DESERIALIZE(bs, newTargetType);
			GM_VAR_DESERIALIZE(bs, newTargetPos);
			GM_VAR_DESERIALIZE(bs, oldItemBag);
			GM_VAR_DESERIALIZE(bs, oldItemID);
			GM_VAR_DESERIALIZE(bs, oldItemLot);
			GM_VAR_DESERIALIZE(bs, oldItemType);
		}

		void TriggerEvent(Entity::GameObject* sender, Entity::GameObject* target) {
			Logger::log("TEST", std::to_string(newSourceBAG));
			Logger::log("TEST", std::to_string(newSourceID));
			Logger::log("TEST", std::to_string(newSourceLOT));
			Logger::log("TEST", std::to_string(newSourceType));
			Logger::log("TEST", std::to_string(newTargetID));
			Logger::log("TEST", std::to_string(newTargetLOT));
			Logger::log("TEST", std::to_string(newTargetType));
			Logger::log("TEST", std::to_string(oldItemBag));
			Logger::log("TEST", std::to_string(oldItemID));
			Logger::log("TEST", std::to_string(oldItemLot));
			Logger::log("TEST", std::to_string(oldItemType));
		}
	};
}

#endif
