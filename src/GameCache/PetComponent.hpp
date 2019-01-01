#ifndef __GAMECACHE__PETCOMPONENT_HPP__
#define __GAMECACHE__PETCOMPONENT_HPP__
#include "GameCache/Interface/FastDatabase.hpp"
using namespace GameCache::Interface;
extern FDB::Connection GameCache;

namespace CachePetComponent {
	inline int32_t GetComponentIndex(int32_t componentID) {
		FDB::RowTopHeader rth = GameCache.getRows("PetComponent");
		for(int  i = 0; i < rth.getRowCount(); ++i)
			if (*reinterpret_cast<int32_t*>(rth[i][0].getMemoryLocation()) == componentID)
				return i;
		return EOF;
	}

	inline float GetMinTameUpdateTime(int32_t componentID) {
		return *reinterpret_cast<float*>(GameCache.getRows("PetComponent")[GetComponentIndex(componentID)]/**/[1]/**/.getMemoryLocation());
	}

	inline float GetMaxTameUpdateTime(int32_t componentID) {
		return *reinterpret_cast<float*>(GameCache.getRows("PetComponent")[GetComponentIndex(componentID)]/**/[2]/**/.getMemoryLocation());
	}

	inline float GetPercentTameChance(int32_t componentID) {
		return *reinterpret_cast<float*>(GameCache.getRows("PetComponent")[GetComponentIndex(componentID)]/**/[3]/**/.getMemoryLocation());
	}

	inline float GetTamability(int32_t componentID) {
		return *reinterpret_cast<float*>(GameCache.getRows("PetComponent")[GetComponentIndex(componentID)]/**/[4]/**/.getMemoryLocation());
	}

	inline int32_t GetElementType(int32_t componentID) {
		return *reinterpret_cast<int32_t*>(GameCache.getRows("PetComponent")[GetComponentIndex(componentID)]/**/[5]/**/.getMemoryLocation());
	}

	inline float GetWalkSpeed(int32_t componentID) {
		return *reinterpret_cast<float*>(GameCache.getRows("PetComponent")[GetComponentIndex(componentID)]/**/[6]/**/.getMemoryLocation());
	}

	inline float GetRunSpeed(int32_t componentID) {
		return *reinterpret_cast<float*>(GameCache.getRows("PetComponent")[GetComponentIndex(componentID)]/**/[7]/**/.getMemoryLocation());
	}

	inline float GetSprintSpeed(int32_t componentID) {
		return *reinterpret_cast<float*>(GameCache.getRows("PetComponent")[GetComponentIndex(componentID)]/**/[8]/**/.getMemoryLocation());
	}

	inline float GetIdleTimeMin(int32_t componentID) {
		return *reinterpret_cast<float*>(GameCache.getRows("PetComponent")[GetComponentIndex(componentID)]/**/[9]/**/.getMemoryLocation());
	}

	inline float GetIdleTimeMax(int32_t componentID) {
		return *reinterpret_cast<float*>(GameCache.getRows("PetComponent")[GetComponentIndex(componentID)]/**/[10]/**/.getMemoryLocation());
	}

	inline int32_t GetPetForm(int32_t componentID) {
		return *reinterpret_cast<int32_t*>(GameCache.getRows("PetComponent")[GetComponentIndex(componentID)]/**/[11]/**/.getMemoryLocation());
	}

	inline float GetImaginationDrainRate(int32_t componentID) {
		return *reinterpret_cast<float*>(GameCache.getRows("PetComponent")[GetComponentIndex(componentID)]/**/[12]/**/.getMemoryLocation());
	}

	inline FDB::PointerString GetAudioMetaEventSet(int32_t componentID) {
		return FDB::PointerString(&GameCache, GameCache.getRows("PetComponent")[GetComponentIndex(componentID)]/**/[13]/**/.getMemoryLocation());
	}

	inline FDB::PointerString GetBuffIDs(int32_t componentID) {
		return FDB::PointerString(&GameCache, GameCache.getRows("PetComponent")[GetComponentIndex(componentID)]/**/[14]/**/.getMemoryLocation());
	}
}
#endif