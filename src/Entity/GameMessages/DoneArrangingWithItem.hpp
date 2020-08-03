#ifndef __ENTITY__GM__DoneArrangingWithItem_HPP__
#define __ENTITY__GM__DoneArrangingWithItem_HPP__
#include "Entity/GameMessages.hpp"
#include "Entity/GameMessages/FinishArrangingWithItem.hpp"

namespace GM {
	struct DoneArrangingWithItem : GMBase {
		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_DONE_ARRANGING_WITH_ITEM;
		}

		DoneArrangingWithItem() {}

		int newSourceBAG;
		DataTypes::LWOOBJID newSourceID;
		uint32_t newSourceLot;
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
			GM_VAR_DESERIALIZE(bs, newSourceLot);
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
			Entity::GameObject* buildBorder = nullptr;
			bool found;
			for (auto obj : sender->GetZoneInstance()->objectsManager->GetObjects()) {
				if (obj->GetLOT() == 9980) {
					buildBorder = obj;
					found = true;
				}
			}
			if (found) {
				GM::FinishArrangingWithItem NewGM;
				NewGM.buildAreaID = buildBorder->GetObjectID();
				NewGM.newSourceBAG = newSourceBAG;
				NewGM.newSourceID = newSourceID;
				NewGM.newSourceLOT = newSourceLot;
				NewGM.newSourceTYPE = newSourceType;
				NewGM.newTargetID = newTargetID;
				NewGM.newTargetLOT = newTargetLOT;
				NewGM.newTargetTYPE = newTargetType;
				NewGM.newtargetPOS = newTargetPos;
				NewGM.oldItemBAG = oldItemBag;
				NewGM.oldItemID = oldItemID;
				NewGM.oldItemLOT = oldItemLot;
				NewGM.oldItemTYPE = oldItemType;
				GameMessages::Send(sender, sender->GetObjectID(), NewGM);
			}
		}
	};
}

#endif
