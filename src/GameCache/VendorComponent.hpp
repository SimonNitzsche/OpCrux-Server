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

	inline float GetBuyScalar(int32_t id) {
		return *reinterpret_cast<float*>(getRow(id)/**/[1]/**/.getMemoryLocation());
	}

	inline float GetSellScalar(int32_t id) {
		return *reinterpret_cast<float*>(getRow(id)/**/[2]/**/.getMemoryLocation());
	}

	inline float GetRefreshTimeSeconds(int32_t id) {
		return *reinterpret_cast<float*>(getRow(id)/**/[3]/**/.getMemoryLocation());
	}

	inline int32_t GetLootMatrixIndex(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[4]/**/.getMemoryLocation());
	}
};

#endif // !__GAMECACHE__VENDORCOMPONENT_HPP__
