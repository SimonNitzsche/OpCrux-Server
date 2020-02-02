#ifndef __ENTITY__GM__ModuleAssemblyDBDataForClient_HPP__
#define __ENTITY__GM__ModuleAssemblyDBDataForClient_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct ModuleAssemblyDBDataForClient : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_MODULE_ASSEMBLY_DBDATA_FOR_CLIENT;
		}

		DataTypes::LWOOBJID assemblyID = 0;
		std::u16string blob = u"";

		ModuleAssemblyDBDataForClient() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE(bs, assemblyID);
			GM_VAR_SERIALIZE_WSTRING(bs, blob);
		}
	};
}

#endif