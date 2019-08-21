#ifndef __ENTITY__GM__RequestUse_HPP__
#define __ENTITY__GM__RequestUse_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct RequestUse : GMBase {
		bool bIsMultiInteractUse;
		std::uint32_t multiInteractID;
		std::int32_t multiInteractType;
		DataTypes::LWOOBJID objectID;
		bool secondary = false;

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_REQUEST_USE;
		}

		RequestUse() {}
		void Deserialize(RakNet::BitStream * bs) {
			GM_VAR_DESERIALIZE(bs, bIsMultiInteractUse);
			GM_VAR_DESERIALIZE(bs, multiInteractID);
			GM_VAR_DESERIALIZE(bs, multiInteractType);
			GM_VAR_DESERIALIZE(bs, objectID);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, secondary, true);
		}

		void TriggerEvent(Entity::GameObject * sender, Entity::GameObject * target) {
			Logger::log("WRLD", "Triggered RequestUse.");
		}
	};
}

#endif