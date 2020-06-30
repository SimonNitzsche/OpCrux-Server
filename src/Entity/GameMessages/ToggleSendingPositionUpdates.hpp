#ifndef __ENTITY__GM__TOGGLESENDINGPOSITIONUPDATES_HPP__
#define __ENTITY__GM__TOGGLESENDINGPOSITIONUPDATES_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct ToggleSendingPositionUpdates : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_TOGGLE_SENDING_POSITION_UPDATES;
		}

		bool bSendUpdates;

		ToggleSendingPositionUpdates() {}

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, bSendUpdates, false);
		}

		void TriggerEvent(Entity::GameObject* sender, Entity::GameObject* target) {

		}
	};
}

#endif
