#ifndef __ENTITY__GM__ModularBuildFinish_HPP__
#define __ENTITY__GM__ModularBuildFinish_HPP__
#include "Entity/GameMessages.hpp"
#include <list>

namespace GM {
	struct ModularBuildFinish : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_MODULAR_BUILD_FINISH;
		}

		ModularBuildFinish() : GMBase() {}

		uint8_t count;
		std::list<std::uint32_t> modules = {};

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE(bs, count);
			for (int i = 0; i < count; ++i) {
				std::uint32_t modulelot;
				GM_VAR_DESERIALIZE(bs, modulelot);
				modules.push_back(modulelot);
			}
		}

		void TriggerEvent(Entity::GameObject* sender, Entity::GameObject* target) {

		}
	};
}

#endif