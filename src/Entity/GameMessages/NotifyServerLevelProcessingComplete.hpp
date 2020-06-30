#ifndef __ENTITY__GM__NOTIFYSERVERLEVELPROCESSINGCOMPLETE_HPP__
#define __ENTITY__GM__NOTIFYSERVERLEVELPROCESSINGCOMPLETE_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct NotifyServerLevelProcessingComplete : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_NOTIFY_SERVER_LEVEL_PROCESSING_COMPLETE;
		}

		NotifyServerLevelProcessingComplete() {}

		void Deserialize(RakNet::BitStream* bs) {
		
		}

		void TriggerEvent(Entity::GameObject* sender, Entity::GameObject* target) {

		}
	};
}

#endif