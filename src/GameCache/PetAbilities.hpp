#ifndef __GAMECACHE__PETABILITIES_HPP__
#define __GAMECACHE__PETABILITIES_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
extern FDB::Connection GameCache;

namespace CachePetAbilities {
	inline FDB::RowInfo getRow(int32_t id) {
		FDB::RowTopHeader rth = GameCache.getRows("PetAbilities");
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
		return FDB::PointerString(&GameCache, getRow(id)/**/[1]/**/.getMemoryLocation());
	}

	inline int32_t GetImaginationCost(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[2]/**/.getMemoryLocation());
	}

	inline int32_t GetLocaleStatus(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[3]/**/.getMemoryLocation());
	}
};
#endif