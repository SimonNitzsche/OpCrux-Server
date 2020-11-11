#ifndef __GAMECACHE__VENDORCOMPONENT_HPP__
#define __GAMECACHE__VENDORCOMPONENT_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
using namespace GameCache::Interface;
extern FDB::Connection Cache;

namespace CacheVendorComponent {
	inline FDB::RowInfo getRow(int32_t id) {
		FDB::RowTopHeader rth = Cache.getRows("VendorComponent");
		for(int  i = 0; i < rth.getRowCount(); ++i) {
			try {
				if (*reinterpret_cast<int32_t*>(rth[i][0].getMemoryLocation()) == id)
					return rth[i];
			}
			catch (std::runtime_error e) {
				Logger::log("Cache:VendorComponent", e.what(), LogType::ERR);
			}
		}
		return FDB::RowInfo();
	}

	CRUX_CACHE_ADD_COLUMN_GETTER(1, float, BuyScalar);
	CRUX_CACHE_ADD_COLUMN_GETTER(2, float, SellScalar);
	CRUX_CACHE_ADD_COLUMN_GETTER(3, float, RefreshTimeSeconds);
	CRUX_CACHE_ADD_COLUMN_GETTER(4, std::int32_t, LootMatrixIndex);
};

#endif // !__GAMECACHE__VENDORCOMPONENT_HPP__
