#ifndef __ENTITY__GM__SetBuildMode_HPP__
#define __ENTITY__GM__SetBuildMode_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct SetBuildMode : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_SET_BUILD_MODE;
		}

		SetBuildMode() : GMBase() {}

		bool bStart;
		std::int16_t distanceType;
		bool modePaused;
        std::int16_t modeValue;
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

		}
	};
}

#endif

