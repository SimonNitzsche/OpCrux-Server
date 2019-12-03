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

	CRUX_CACHE_ADD_COLUMN_GETTER(0, std::int32_t, ZoneID)
		CRUX_CACHE_ADD_COLUMN_GETTER(1, std::int32_t, LocStatus)
		CRUX_CACHE_ADD_COLUMN_GETTER(2, FDB::PointerString, ZoneName)
		CRUX_CACHE_ADD_COLUMN_GETTER(3, std::int32_t, ScriptID)
		CRUX_CACHE_ADD_COLUMN_GETTER(4, std::float_t, GhostDistanceMin)
		CRUX_CACHE_ADD_COLUMN_GETTER(5, std::float_t, GhostDistance)
		CRUX_CACHE_ADD_COLUMN_GETTER(6, std::int32_t, PopulationSoftCap)
		CRUX_CACHE_ADD_COLUMN_GETTER(7, std::int32_t, PopulationHardCap)
		CRUX_CACHE_ADD_COLUMN_GETTER(8, FDB::PointerString, DisplayDescription)
		CRUX_CACHE_ADD_COLUMN_GETTER(9, FDB::PointerString, MapFolder)
		CRUX_CACHE_ADD_COLUMN_GETTER(10, std::float_t, SmashableMinDistance)
		CRUX_CACHE_ADD_COLUMN_GETTER(11, std::float_t, SmashableMaxDistance)
		CRUX_CACHE_ADD_COLUMN_GETTER(12, FDB::PointerString, MixerProgram)
		CRUX_CACHE_ADD_COLUMN_GETTER(13, FDB::PointerString, ClientPhysicsFramerate)
		CRUX_CACHE_ADD_COLUMN_GETTER(14, FDB::PointerString, ServerPhysicsFramerate)
		CRUX_CACHE_ADD_COLUMN_GETTER(15, std::int32_t, ZoneControlTemplate)
		CRUX_CACHE_ADD_COLUMN_GETTER(16, std::int32_t, WidthInChunks)
		CRUX_CACHE_ADD_COLUMN_GETTER(17, std::int32_t, HeightInChunks)
		CRUX_CACHE_ADD_COLUMN_GETTER(18, bool, PetsAllowed)
		CRUX_CACHE_ADD_COLUMN_GETTER(19, bool, Localize)
		CRUX_CACHE_ADD_COLUMN_GETTER(20, std::float_t, FZoneWeight)
		CRUX_CACHE_ADD_COLUMN_GETTER(21, FDB::PointerString, Thumbnail)
		CRUX_CACHE_ADD_COLUMN_GETTER(22, bool, PlayerLoseCoinsOnDeath)
		CRUX_CACHE_ADD_COLUMN_GETTER(23, bool, DisableSaveLoc)
		CRUX_CACHE_ADD_COLUMN_GETTER(24, std::float_t, TeamRadius)
		CRUX_CACHE_ADD_COLUMN_GETTER(25, FDB::PointerString, GateVersion)
		CRUX_CACHE_ADD_COLUMN_GETTER(26, bool, MountsAllowed)

	// TODO: Implement others
};

#endif
