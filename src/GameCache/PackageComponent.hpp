#ifndef __GAMECACHE__PACKAGECOMPONENT_HPP__
#define __GAMECACHE__PACKAGECOMPONENT_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
using namespace GameCache::Interface;
extern FDB::Connection Cache;

namespace CachePackageComponent {
	inline FDB::RowInfo getRow(int32_t id) {
		FDB::RowTopHeader rth = Cache.getRows("PackageComponent");
		for (int i = 0; i < rth.getRowCount(); ++i) {
			if (!rth.isValid(i)) continue;
			try {
				if (*reinterpret_cast<int32_t*>(rth[i][0].getMemoryLocation()) == id)
					return rth[i];
			}
			catch (std::runtime_error e) {
				Logger::log("Cache:PackageComponent", e.what(), LogType::ERR);
			}
		}
		return FDB::RowInfo();
	}

	CRUX_CACHE_ADD_COLUMN_GETTER(1, std::int32_t, LootMatrixIndex);
	CRUX_CACHE_ADD_COLUMN_GETTER(2, std::int32_t, PackageType);

};

#endif // !__GAMECACHE__SKILLBEHAVIOR_HPP__