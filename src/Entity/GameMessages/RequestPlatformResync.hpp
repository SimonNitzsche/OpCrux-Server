#ifndef __ENTITY__GM__RequestPlatformResync_HPP__
#define __ENTITY__GM__RequestPlatformResync_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct RequestPlatformResync : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_REQUEST_PLATFORM_RESYNC;
		}

		RequestPlatformResync() : GMBase() {}

		void Deserialize(RakNet::BitStream* bs) {
			
		}

		void TriggerEvent(Entity::GameObject* sender, Entity::GameObject* target) {
			// GameMessages::Send(sender->GetZoneInstance(), UNASSIGNED_SYSTEM_ADDRESS, target->GetObjectID(), PlatformResync);
		}
	};
}

#endif



