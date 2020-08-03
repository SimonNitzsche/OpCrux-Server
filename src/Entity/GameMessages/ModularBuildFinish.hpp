#ifndef __ENTITY__GM__ModularBuildFinish_HPP__
#define __ENTITY__GM__ModularBuildFinish_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct ModularBuildFinish : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_MODULAR_BUILD_FINISH;
		}

		ModularBuildFinish() : GMBase() {}

		uint8_t count;
		int32_t modulelot;

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE(bs, count);
			GM_VAR_DESERIALIZE(bs, modulelot);
		}

		void TriggerEvent(Entity::GameObject* sender, Entity::GameObject* target) {
			
		}
	};
}

#endif

