#ifndef __ENTITY__GM__PlayAnimation_HPP__
#define __ENTITY__GM__PlayAnimation_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct PlayAnimation : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_PLAY_ANIMATION;
		}

		std::wstring animationID = L"";
		bool bExpectAnimToExist = true;
		bool bPlayImmediate = false;
		bool bTriggerOnCompleteMsg = false;
		float fPriority = 0.400000005960465f;
		float fScale = 1.0f;

		PlayAnimation() : GMBase() {}
		void Deserialize(RakNet::BitStream * bs) {

		}

		void Serialize(RakNet::BitStream * bs) {
			GM_VAR_SERIALIZE_WSTRING(bs, animationID);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, bExpectAnimToExist, true);
			GM_VAR_SERIALIZE(bs, bPlayImmediate);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, bTriggerOnCompleteMsg, false);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, fPriority, 0.400000005960465f);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, fScale, 1.0f);
		}

		void TriggerEvent(Entity::GameObject * sender, Entity::GameObject * target) {
			GameMessages::Send(sender->GetZoneInstance(), UNASSIGNED_SYSTEM_ADDRESS, target->GetObjectID(), *this);
		}
	};
}

#endif