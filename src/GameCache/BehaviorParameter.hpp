#ifndef __GAMECACHE__BEHAVIORPARAMETER_HPP__
#define __GAMECACHE__BEHAVIORPARAMETER_HPP__

#include "GameCache/Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
using namespace GameCache::Interface;
extern FDB::Connection Cache;

namespace GameCache::BehaviorParameter {

	/*
		Returns the Behavior Index, given a Behavior ID.
	*/
	inline int32_t GetBehaviorIndex(int32_t behaviorID) {
		// Get Rows on "BehaviorParameter" table.
		FDB::RowTopHeader rth = Cache.getRows("BehaviorParameter");

		// Loop all Rows.
		for (int i = 0; i < rth.getRowCount(); ++i) {
			try {
				// Attempt to cast the ID to int32_t and check if it is the behavior ID, if it is, return the index.
				if (*reinterpret_cast<int32_t*>(rth[i][0].getMemoryLocation()) == behaviorID)
					return i;
			}
			catch (std::runtime_error e) {
				// Something went wrong, log it.
				Logger::log("Cache:BehaviorParameter", e.what(), ERR);
			}
		}

		// Return error.
		return EOF;
	}

	/*
		Returns the Behavior Row, with specified behavior id.
	*/
	inline FDB::RowInfo GetBehaviorRow(int32_t behaviorID) {
		// Get the Rows, and return the correct row.
		return Cache.getRows("BehaviorParameter")[GetBehaviorIndex(behaviorID)];
	}

	/*
		Returns the value of Multiple Parameters. (multiple only)
	*/
	inline float GetParameterValue(FDB::RowInfo behaviorRow, std::string parameterID) {
		// Until we break...
		while(true) {
			// Check for Parameter ID.
			if (behaviorRow[1].ToString() == parameterID) {
				// Return the memory location of the value.
				return *reinterpret_cast<float*>(behaviorRow[2].getMemoryLocation());
			}

			// If the Linked Row info is valid.
			if (behaviorRow.isLinkedRowInfoValid()) {
				// Set Behavior Row to the linked row.
				behaviorRow = behaviorRow.getLinkedRowInfo();
			} else {
				break;
			}
		}

		// Return error.
		return EOF;
	}

	/*
		Returns the value of A Single Paramter. (single only)
	*/
	inline float GetParameterValue(int32_t behaviorID, std::string parameterID) {
		// Converts the behavior id into a row info and calls the function above.
		return GetParameterValue(GetBehaviorRow(behaviorID), parameterID);
	}

};

#endif // !__GAMECACHE__BEHAVIORPARAMETER_HPP__
