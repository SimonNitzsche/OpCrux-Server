#ifndef __ENTITY__GM__BuildModeSet_HPP__
#define __ENTITY__GM__BuildModeSet_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct BuildModeSet : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_BUILD_MODE_SET;
		}

		BuildModeSet() : GMBase() {}

		bool bStart;
		int distanceType;
		bool modePaused;
		int modeValue;
		DataTypes::LWOOBJID playerID;
		DataTypes::Vector3 startPos;
		DataTypes::Vector3 empty;

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE(bs, bStart);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, distanceType, -1);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, modePaused, false);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, modeValue, 1);
			GM_VAR_DESERIALIZE(bs, playerID);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, startPos, empty);

		}

		void TriggerEvent(Entity::GameObject* sender, Entity::GameObject* target) {
			Logger::log("WRLD", "Triggered BuildModeSet");
		}
	};
}

#endif