#ifndef __ENTITY__GM__ModularBuildFinish_HPP__
#define __ENTITY__GM__ModularBuildFinish_HPP__
#include "Entity/GameMessages.hpp"
#include "Entity/GameObject.hpp"
#include "Entity/Components/InventoryComponent.hpp"

namespace GM {
	struct ModularBuildFinish : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_MODULAR_BUILD_FINISH;
		}

		ModularBuildFinish() : GMBase() {}

		uint8_t count;
		std::vector<int32_t> modulelot;

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE(bs, count);
			for (int i = 0; i <= count; i++) {
				int32_t value;
				GM_VAR_DESERIALIZE(bs, value);
				modulelot.push_back(value);
			}
		}

		void TriggerEvent(Entity::GameObject* sender, Entity::GameObject* target) {
			InventoryComponent* InvComp = sender->GetComponent<InventoryComponent>();
			Logger::log("TEST", "ADDED ITEM", LogType::UNEXPECTED);
			InvComp->AddItem(6416);
		}
	};
}

#endif

