#ifndef __GAMECACHE__PETABILITIES_HPP__
#define __GAMECACHE__PETABILITIES_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
using namespace GameCache::Interface;
extern FDB::Connection Cache;

namespace CachePetAbilities {
	inline FDB::RowInfo getRow(int32_t id) {
		FDB::RowTopHeader rth = Cache.getRows("PetAbilities");
		for(int  i = 0; i < rth.getRowCount(); ++i) {
			try {
				if (*reinterpret_cast<int32_t*>(rth[i][0].getMemoryLocation()) == id)
					return rth[i];
			}
			catch (std::runtime_error e) {
				Logger::log("Cache:PetAbilities", e.what(), ERR);
			}
		}
		return FDB::RowInfo();
	}

	inline FDB::PointerString GetAbilityName(int32_t id) {
		return FDB::PointerString(&Cache, getRow(id)/**/[1]/**/.getMemoryLocation());
	}

	inline int32_t GetImaginationCost(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[2]/**/.getMemoryLocation());
	}

	inline int32_t GetLocaleStatus(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[3]/**/.getMemoryLocation());
	}
};

#endif // !__GAMECACHE__PETABILITIES_HPP__
