#ifndef __GAMECACHE__BEHAVIORTEMPLATE_HPP__
#define __GAMECACHE__BEHAVIORTEMPLATE_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
using namespace GameCache::Interface;
extern FDB::Connection Cache;

namespace CacheBehaviorTemplate {
	inline FDB::RowInfo getRow(int32_t behaviorID) {
		FDB::RowTopHeader rth = Cache.getRows("BehaviorTemplate");
		for (int i = 0; i < rth.getRowCount(); ++i) {
			if (!rth.isValid(i)) continue;
			try {
				if (*reinterpret_cast<int32_t*>(rth[i][0].getMemoryLocation()) == behaviorID)
					return rth[i];
			}
			catch (std::runtime_error e) {
				Logger::log("Cache:BehaviorTemplate", e.what(), LogType::ERR);
			}
		}
		return FDB::RowInfo();
	}

	inline int32_t GetTemplateID(int32_t behaviorID) {
		return *reinterpret_cast<int32_t*>(getRow(behaviorID)/**/[1]/**/.getMemoryLocation());
	}

	inline int32_t GetEffectID(int32_t behaviorID) {
		return *reinterpret_cast<int32_t*>(getRow(behaviorID)/**/[2]/**/.getMemoryLocation());
	}

	inline FDB::PointerString GetEffectHandle(int32_t behaviorID) {
		return FDB::PointerString(&Cache, getRow(behaviorID)/**/[3]/**/.getMemoryLocation());
	}
};

#endif // !__GAMECACHE__SKILLBEHAVIOR_HPP__
