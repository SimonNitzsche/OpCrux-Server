#ifndef __GAMECACHE__BEHAVIORPARAMETER_HPP__
#define __GAMECACHE__BEHAVIORPARAMETER_HPP__

#include "GameCache/Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
extern FDB::Connection GameCache;

namespace CacheBehaviorParameter {
	inline int32_t GetBehaviorIndex(int32_t behaviorID) {
		FDB::RowTopHeader rth = GameCache.getRows("BehaviorParameter");
		for (int i = 0; i < rth.getRowCount(); ++i) {
			try {
				if (*reinterpret_cast<int32_t*>(rth[i][0].getMemoryLocation()) == behaviorID)
					return i;
			}
			catch (std::runtime_error e) {
				Logger::log("Cache:BehaviorParameter", e.what(), ERR);
			}
		}
		return EOF;
	}

	inline FDB::RowInfo GetBehaviorRow(int32_t behaviorID) {
		return GameCache.getRows("BehaviorParameter")[GetBehaviorIndex(behaviorID)];
	}

	// Use this if you do multiple requests
	inline float GetParameterValue(FDB::RowInfo behaviorRow, std::string parameterID) {
		while(true) {
			if (behaviorRow[1].ToString() == parameterID) {
				return *reinterpret_cast<float*>(behaviorRow[2].getMemoryLocation());
			}
			if (behaviorRow.isLinkedRowInfoValid()) {
				behaviorRow = behaviorRow.getLinkedRowInfo();
			}
			else {
				break;
			}
		}
		return EOF;
	}

	// Only use this for single requests
	inline float GetParameterValue(int32_t behaviorID, std::string parameterID) {
		return GetParameterValue(GetBehaviorRow(behaviorID), parameterID);
	}

}
#endif