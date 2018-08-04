#ifndef __GAMECACHE__PROPERTYENTRANCECOMPONENT_HPP__
#define __GAMECACHE__PROPERTYENTRANCECOMPONENT_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
extern FDB::Connection GameCache;

namespace CachePropertyEntranceComponent {
	inline FDB::RowInfo getRow(int32_t id) {
		FDB::RowTopHeader rth = GameCache.getRows("PropertyEntranceComponent");
		for (int i = 0; i < rth.getRowCount(); ++i) {
			try {
				if (*reinterpret_cast<int32_t*>(rth[i][0].getMemoryLocation()) == id)
					return rth[i];
			}
			catch (std::runtime_error e ) {
				Logger::log("Cache:PropertyEntranceComponent", e.what(), ERR);
			}
		}
		return FDB::RowInfo();
	}

	inline int32_t GetMapID(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[1]/**/.getMemoryLocation());
	}

	inline FDB::PointerString GetPropertyName(int32_t id) {
		return FDB::PointerString(&GameCache, getRow(id)/**/[2]/**/.getMemoryLocation());
	}

	inline bool GetIsOnProperty(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[3]/**/.getMemoryLocation()) == 1;
	}

	inline FDB::PointerString GetGroupType(int32_t id) {
		return FDB::PointerString(&GameCache, getRow(id)/**/[1]/**/.getMemoryLocation());
	}
};
#endif