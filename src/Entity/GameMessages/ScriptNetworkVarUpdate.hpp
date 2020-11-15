#ifndef __ENTITY__GM__ScriptNetworkVarUpdate_HPP__
#define __ENTITY__GM__ScriptNetworkVarUpdate_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct ScriptNetworkVarUpdate : GMBase {
		LDFCollection tableOfVars = {};

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_SCRIPT_NETWORK_VAR_UPDATE;
		}

		ScriptNetworkVarUpdate() : GMBase() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE_LDF(bs, tableOfVars);
		}
	};
}
#endif
