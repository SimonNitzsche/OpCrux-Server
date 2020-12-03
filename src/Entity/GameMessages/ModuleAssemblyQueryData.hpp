#ifndef __ENTITY__GM__ModuleAssemblyQueryData_HPP__
#define __ENTITY__GM__ModuleAssemblyQueryData_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct ModuleAssemblyQueryData : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_MODULE_ASSEMBLY_QUERY_DATA;
		}

		ModuleAssemblyQueryData() {}
		
	};
}

#endif