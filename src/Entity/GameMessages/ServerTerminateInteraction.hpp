#ifndef __ENTITY__GM__ServerTerminateInteraction_HPP__
#define __ENTITY__GM__ServerTerminateInteraction_HPP__
#include "Entity/GameMessages.hpp"
#include "Enums/ETerminateType.hpp"

namespace GM {
	struct ServerTerminateInteraction : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_TERMINATE_INTERACTION;
		}

		DataTypes::LWOOBJID ObjIDTerminator;
		Enums::ETerminateType type;

		ServerTerminateInteraction() : GMBase() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE(bs, ObjIDTerminator);
			GM_VAR_SERIALIZE(bs, type);
		}
	};
}

#endif