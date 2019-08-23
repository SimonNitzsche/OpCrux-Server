#ifndef __GAMECACHE__ZONETABLE_HPP__
#define __GAMECACHE__ZONETABLE_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
using namespace GameCache::Interface;
extern FDB::Connection Cache;

namespace CacheZoneTable {
	inline FDB::RowInfo getRow(int32_t id) {
		FDB::RowTopHeader rth = Cache.getRows("ZoneTable");
		for (int i = 0; i < rth.getRowCount(); ++i) {
			try {
				if (!rth.isValid(i)) continue;
				FDB::RowInfo rowInfo = rth[i];
				if(rowInfo.isValid())
					if (*reinterpret_cast<int32_t*>(rowInfo[0].getMemoryLocation()) == id)
						return rth[i];
			}
			catch (std::runtime_error e) {
				Logger::log("Cache:ZoneTable", e.what(), LogType::ERR);
			}
		}
		return FDB::RowInfo();
	}

	inline FDB::PointerString GetZoneName(int32_t id) {
		return FDB::PointerString(&Cache, getRow(id)/**/[2]/**/.getMemoryLocation());
	}

	// TODO: Implement others
};

#endif
