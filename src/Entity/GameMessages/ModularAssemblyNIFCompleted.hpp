#ifndef __ENTITY__GM__ModularAssemblyNIFCompleted_HPP__
#define __ENTITY__GM__ModularAssemblyNIFCompleted_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct ModularAssemblyNIFCompleted : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_MODULAR_ASSEMBLY_NIF_COMPLETED;
		}

		ModularAssemblyNIFCompleted() : GMBase() {}

		int64_t objectID;

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE(bs, objectID);
		}

		void TriggerEvent(Entity::GameObject* sender, Entity::GameObject* target) {
			
		}
	};
}

#endif