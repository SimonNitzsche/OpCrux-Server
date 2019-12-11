#ifndef __GAMECACHE__MISSIONNPCCOMPONENT_HPP__
#define __GAMECACHE__MISSIONNPCCOMPONENT_HPP__

#include "Interface/FastDatabase.hpp"
using namespace GameCache::Interface;
extern FDB::Connection Cache;

namespace CacheMissionNPCComponent {
	inline FDB::RowInfo getRow(int32_t id) {
		FDB::RowTopHeader rth = Cache.getRows("MissionNPCComponent");
		for (int i = 0; i < rth.getRowCount(); ++i) {
			if (!rth.isValid(i)) continue;
			try {
				FDB::RowInfo rowInfo = rth[i];
				while (rowInfo.isValid()) {
					if (*reinterpret_cast<int32_t*>(rowInfo[0].getMemoryLocation()) == id)
						return rth[i];
					if (rowInfo.isLinkedRowInfoValid()) {
						rowInfo = rowInfo.getLinkedRowInfo();
					}
					else {
						rowInfo = FDB::RowInfo();
					}
				}
			}
			catch (...) {}
		}
		return FDB::RowInfo();
	}

	CRUX_CACHE_ADD_COLUMN_GETTER(0, std::int32_t, Id)
	CRUX_CACHE_ADD_COLUMN_GETTER(1, std::int32_t, MissionID)
	CRUX_CACHE_ADD_COLUMN_GETTER(2, bool, OffersMission)
	CRUX_CACHE_ADD_COLUMN_GETTER(3, bool, AcceptsMission)
	CRUX_CACHE_ADD_COLUMN_GETTER(4, FDB::PointerString, GateVersion)
};

#endif // !__Cache__MISSIONNPCCOMPONENT_HPP__