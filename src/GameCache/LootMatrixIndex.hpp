#ifndef __GAMECACHE__LOOTMATRIXINDEX_HPP__
#define __GAMECACHE__LOOTMATRIXINDEX_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
using namespace GameCache::Interface;
extern FDB::Connection GameCache;

namespace CacheLootMatrixIndex {
	inline FDB::RowInfo getRow(int lootMatrixIndex) {
		FDB::RowTopHeader rth = GameCache.getRows("LootMatrixIndex");
		for(int  i = 0; i < rth.getRowCount(); ++i) {
			try {
				if (*reinterpret_cast<int32_t*>(rth[i][0].getMemoryLocation()) == lootMatrixIndex)
					return rth[i];
			}
			catch (std::runtime_error e) {
				Logger::log("Cache:LootMatrixIndex", e.what(), ERR);
			}
		}
		return FDB::RowInfo();
	}

	inline bool GetInNpcEditor(int32_t lootMatrixIndex) {
		return *reinterpret_cast<int32_t*>(getRow(lootMatrixIndex)/**/[1]/**/.getMemoryLocation()) == 1;
	}
};

#endif // !__GAMECACHE__LOOTMATRIXINDEX_HPP__
