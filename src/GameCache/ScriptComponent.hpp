#ifndef __GAMECACHE__SCRIPTCOMPONENT_HPP__
#define __GAMECACHE__SCRIPTCOMPONENT_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
using namespace GameCache::Interface;
extern FDB::Connection Cache;

namespace CacheScriptComponent {
	inline FDB::RowInfo getRow(int32_t id) {
		FDB::RowTopHeader rth = Cache.getRows("ScriptComponent");
		for(int  i = 0; i < rth.getRowCount(); ++i) {
			if (!rth.isValid(i)) continue;
			try {
				if (*reinterpret_cast<int32_t*>(rth[i][0].getMemoryLocation()) == id) {
					return rth[i];
				}
			}
			catch (std::runtime_error e) {
				Logger::log("Cache:ScriptComponent", e.what(), LogType::ERR);
			}
		}
		return FDB::RowInfo();
	}

	inline FDB::PointerString GetScriptName(int32_t id) {
		auto memlocation = getRow(id)/**/[1]/**/.getMemoryLocation();
		return FDB::PointerString(&Cache, memlocation);
	}

	inline FDB::PointerString GetClientScriptName(int32_t id) {
		return FDB::PointerString(&Cache, getRow(id)/**/[2]/**/.getMemoryLocation());
	}
};

#endif // !__GAMECACHE__SCRIPTCOMPONENT_HPP__
