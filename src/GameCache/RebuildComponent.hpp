#ifndef __GAMECACHE__REBUILDCOMPONENT_HPP__
#define __GAMECACHE__REBUILDCOMPONENT_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
using namespace GameCache::Interface;
extern FDB::Connection Cache;

namespace CacheRebuildComponent {
	inline FDB::RowInfo getRow(int32_t id) {
		FDB::RowTopHeader rth = Cache.getRows("RebuildComponent");
		for (int i = 0; i < rth.getRowCount(); ++i) {
			try {
				if (!rth.isValid(i)) continue;
				FDB::RowInfo rowInfo = rth[i];
				if (rowInfo.isValid())
					if (*reinterpret_cast<int32_t*>(rowInfo[0].getMemoryLocation()) == id)
						return rth[i];
			}
			catch (std::runtime_error e) {
				Logger::log("Cache:RebuildComponent", e.what(), LogType::ERR);
			}
		}
		return FDB::RowInfo();
	}

	CRUX_CACHE_ADD_COLUMN_GETTER(0, std::int32_t, ID)
		CRUX_CACHE_ADD_COLUMN_GETTER(1, std::float_t, ResetTime)
		CRUX_CACHE_ADD_COLUMN_GETTER(2, std::float_t, CompleteTime)
		CRUX_CACHE_ADD_COLUMN_GETTER(3, std::int32_t, TakeImagination)
		CRUX_CACHE_ADD_COLUMN_GETTER(4, bool, Interruptible)
		CRUX_CACHE_ADD_COLUMN_GETTER(5, bool, SelfActivator)
		CRUX_CACHE_ADD_COLUMN_GETTER(6, FDB::PointerString, CustomModules)
		CRUX_CACHE_ADD_COLUMN_GETTER(7, std::int32_t, ActivityID)
		CRUX_CACHE_ADD_COLUMN_GETTER(8, std::int32_t, PostImaginationCost)
		CRUX_CACHE_ADD_COLUMN_GETTER(9, std::float_t, TimeBeforeSmash)
};

#endif
