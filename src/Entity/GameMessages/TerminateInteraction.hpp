#ifndef __ENTITY__GM__TerminateInteraction_HPP__
#define __ENTITY__GM__TerminateInteraction_HPP__
#include "Entity/GameMessages.hpp"
#include "Enums/ETerminateType.hpp"

namespace GM {
	struct TerminateInteraction : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_TERMINATE_INTERACTION;
		}

		DataTypes::LWOOBJID ObjIDTerminator;
		Enums::ETerminateType type;

		TerminateInteraction() : GMBase() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE(bs, ObjIDTerminator);
			GM_VAR_SERIALIZE(bs, type);
		}

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE(bs, ObjIDTerminator);
			GM_VAR_DESERIALIZE(bs, type);
		}
	};
}

#endif