#ifndef __GAMECACHE__CURRENCYTABLE_HPP__
#define __GAMECACHE__CURRENCYTABLE_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
using namespace GameCache::Interface;
extern FDB::Connection Cache;

namespace CacheCurrencyTable {
	inline FDB::RowInfo getRow(int32_t currencyIndex) {
		FDB::RowTopHeader rth = Cache.getRows("CurrencyTable");
		for (int i = 0; i < rth.getRowCount(); ++i) {
			try {
				if (!rth.isValid(i)) continue;
				FDB::RowInfo rowInfo = rth[i];
				if (rowInfo.isValid())
					if (*reinterpret_cast<int32_t*>(rowInfo[0].getMemoryLocation()) == currencyIndex)
						return rth[i];
			}
			catch (std::runtime_error e) {
				Logger::log("Cache:CurrencyTable", e.what(), LogType::ERR);
			}
		}
		return FDB::RowInfo();
	}

	CRUX_CACHE_ADD_COLUMN_GETTER(0, std::int32_t, CurrencyIndex)
	CRUX_CACHE_ADD_COLUMN_GETTER(1, std::int32_t, NPCMinLevel)
	CRUX_CACHE_ADD_COLUMN_GETTER(2, std::int32_t, MinValue)
	CRUX_CACHE_ADD_COLUMN_GETTER(3, std::int32_t, MaxValue)
	CRUX_CACHE_ADD_COLUMN_GETTER(4, std::int32_t, ID)
};

#endif
