#ifndef __GAMECACHE__ITEMCOMPONENT_HPP__
#define __GAMECACHE__ITEMCOMPONENT_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
using namespace GameCache::Interface;
extern FDB::Connection GameCache;

namespace CacheItemComponent {
	inline FDB::RowInfo getRow(int32_t id) {
		FDB::RowTopHeader rth = GameCache.getRows("ItemComponent");
		for(int  i = 0; i < rth.getRowCount(); ++i) {
			try {
				if (*reinterpret_cast<int32_t*>(rth[i][0].getMemoryLocation()) == id)
					return rth[i];
			}
			catch (std::runtime_error e) {
				Logger::log("Cache:ItemComponent", e.what(), ERR);
			}
		}
		return FDB::RowInfo();
	}

	inline FDB::PointerString GetEquipLocation(int32_t id) {
		return FDB::PointerString(&GameCache, getRow(id)/**/[1]/**/.getMemoryLocation());
	}
	
	inline int32_t GetBaseValue(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[2]/**/.getMemoryLocation());
	}

	inline bool GetIsKitPiece(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[3]/**/.getMemoryLocation()) == 1;
	}

	inline int32_t GetRarity(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[4]/**/.getMemoryLocation());
	}

	inline int32_t GetItemType(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[5]/**/.getMemoryLocation());
	}

	inline int64_t GetItemInfo(int32_t id) {
		return *reinterpret_cast<int64_t*>(GameCache.getFileData()+*reinterpret_cast<int32_t*>(getRow(id)/**/[6]/**/.getMemoryLocation()));
	}

	inline bool GetInLootTable(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[7]/**/.getMemoryLocation()) == 1;
	}

	inline bool GetInVendor(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[8]/**/.getMemoryLocation()) == 1;
	}

	inline bool GetIsUnique(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[9]/**/.getMemoryLocation()) == 1;
	}

	inline bool GetIsBOP(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[10]/**/.getMemoryLocation()) == 1;
	}

	inline bool GetIsBOE(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[11]/**/.getMemoryLocation()) == 1;
	}

	inline int32_t GetReqFlagID(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[12]/**/.getMemoryLocation());
	}

	inline int32_t GetReqSpecialtyID(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[13]/**/.getMemoryLocation());
	}

	inline int32_t GetReqSpecRank(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[14]/**/.getMemoryLocation());
	}

	inline int32_t GetAchievementID(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[15]/**/.getMemoryLocation());
	}

	inline int32_t GetStackSize(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[16]/**/.getMemoryLocation());
	}

	inline int32_t GetColor1(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[17]/**/.getMemoryLocation());
	}

	inline int32_t GetDecal(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[18]/**/.getMemoryLocation());
	}

	inline int32_t GetOffsetGroupID(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[19]/**/.getMemoryLocation());
	}

	inline int32_t GetBuildTypes(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[20]/**/.getMemoryLocation());
	}

	inline FDB::PointerString GetReqPrecondition(int32_t id) {
		return FDB::PointerString(&GameCache, getRow(id)/**/[21]/**/.getMemoryLocation());
	}

	inline int32_t GetAnimationFlag(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[22]/**/.getMemoryLocation());
	}

	inline int32_t GetEquipEffects(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[23]/**/.getMemoryLocation());
	}

	inline bool GetReadyForQA(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[24]/**/.getMemoryLocation()) == 1;
	}

	inline int32_t GetItemRating(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[25]/**/.getMemoryLocation());
	}

	inline bool GetIsTwoHanded(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[26]/**/.getMemoryLocation()) == 1;
	}

	inline int32_t GetMinNumRequired(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[27]/**/.getMemoryLocation());
	}

	inline int32_t GetDelResIndex(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[28]/**/.getMemoryLocation());
	}

	inline int32_t GetCurrencyLOT(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[29]/**/.getMemoryLocation());
	}

	inline int32_t GetAltCurrencyCost(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[30]/**/.getMemoryLocation());
	}

	inline FDB::PointerString GetSubItems(int32_t id) {
		return FDB::PointerString(&GameCache, getRow(id)/**/[31]/**/.getMemoryLocation());
	}

	inline FDB::PointerString GetAudioEventUse(int32_t id) {
		return FDB::PointerString(&GameCache, getRow(id)/**/[32]/**/.getMemoryLocation());
	}

	inline bool GetNoEquipAnimation(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[33]/**/.getMemoryLocation()) == 1;
	}

	inline int32_t GetCommendationLOT(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[34]/**/.getMemoryLocation());
	}

	inline int32_t GetCommendationCost(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[35]/**/.getMemoryLocation());
	}

	inline FDB::PointerString GetAudioEquipMetaEventSet(int32_t id) {
		return FDB::PointerString(&GameCache, getRow(id)/**/[36]/**/.getMemoryLocation());
	}

	inline FDB::PointerString GetCurrencyCosts(int32_t id) {
		return FDB::PointerString(&GameCache, getRow(id)/**/[37]/**/.getMemoryLocation());
	}

	inline FDB::PointerString GetIngredientInfo(int32_t id) {
		return FDB::PointerString(&GameCache, getRow(id)/**/[38]/**/.getMemoryLocation());
	}

	inline int32_t GetLocaleStatus(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[39]/**/.getMemoryLocation());
	}

	inline int32_t GetForgeType(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[40]/**/.getMemoryLocation());
	}

	inline float GetSellMultiplier(int32_t id) {
		return *reinterpret_cast<float*>(getRow(id)/**/[41]/**/.getMemoryLocation());
	}
};

#endif // !__GAMECACHE__ITEMCOMPONENT_HPP__
