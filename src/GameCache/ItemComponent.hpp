#ifndef __GAMECACHE__ITEMCOMPONENT_HPP__
#define __GAMECACHE__ITEMCOMPONENT_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
using namespace GameCache::Interface;
extern FDB::Connection Cache;

namespace CacheItemComponent {
	inline FDB::RowInfo getRow(int32_t id) {
		FDB::RowTopHeader rth = Cache.getRows("ItemComponent");
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

	CRUX_CACHE_ADD_COLUMN_GETTER(1, FDB::PointerString, EquipLocation);
	CRUX_CACHE_ADD_COLUMN_GETTER(2, std::int32_t, BaseValue);
	CRUX_CACHE_ADD_COLUMN_GETTER(3, bool, IsKitPiece);
	CRUX_CACHE_ADD_COLUMN_GETTER(4, std::int32_t, Rarity);
	CRUX_CACHE_ADD_COLUMN_GETTER(5, std::int32_t, ItemType);
	CRUX_CACHE_ADD_COLUMN_GETTER(6, std::int64_t, ItemInfo);
	CRUX_CACHE_ADD_COLUMN_GETTER(7, bool, InLootTable);
	CRUX_CACHE_ADD_COLUMN_GETTER(8, bool, InVendor);
	CRUX_CACHE_ADD_COLUMN_GETTER(9, bool, IsUnique);
	CRUX_CACHE_ADD_COLUMN_GETTER(10, bool, IsBOP);
	CRUX_CACHE_ADD_COLUMN_GETTER(11, bool, IsBOE);
	CRUX_CACHE_ADD_COLUMN_GETTER(12, std::int32_t, ReqFlagID);
	CRUX_CACHE_ADD_COLUMN_GETTER(13, std::int32_t, ReqSpecialtyID);
	CRUX_CACHE_ADD_COLUMN_GETTER(14, std::int32_t, ReqSpecRank);
	CRUX_CACHE_ADD_COLUMN_GETTER(15, std::int32_t, AchievementID);
	CRUX_CACHE_ADD_COLUMN_GETTER(16, std::int32_t, StackSize);
	CRUX_CACHE_ADD_COLUMN_GETTER(17, std::int32_t, Color1);
	CRUX_CACHE_ADD_COLUMN_GETTER(18, std::int32_t, Decal);
	CRUX_CACHE_ADD_COLUMN_GETTER(19, std::int32_t, OffsetGroupID);
	CRUX_CACHE_ADD_COLUMN_GETTER(20, std::int32_t, BuildTypes);
	CRUX_CACHE_ADD_COLUMN_GETTER(21, FDB::PointerString, ReqPrecondition);
	CRUX_CACHE_ADD_COLUMN_GETTER(22, std::int32_t, AnimationFlag);
	CRUX_CACHE_ADD_COLUMN_GETTER(23, std::int32_t, EquipEffects);
	CRUX_CACHE_ADD_COLUMN_GETTER(24, bool, ReadyForQA);
	CRUX_CACHE_ADD_COLUMN_GETTER(25, std::int32_t, ItemRating);
	CRUX_CACHE_ADD_COLUMN_GETTER(26, bool, IsTwoHanded);
	CRUX_CACHE_ADD_COLUMN_GETTER(27, std::int32_t, MinNumRequired);
	CRUX_CACHE_ADD_COLUMN_GETTER(28, std::int32_t, DelResIndex);
	CRUX_CACHE_ADD_COLUMN_GETTER(29, std::int32_t, CurrencyLOT);
	CRUX_CACHE_ADD_COLUMN_GETTER(30, std::int32_t, AltCurrencyCost);
	CRUX_CACHE_ADD_COLUMN_GETTER(31, FDB::PointerString, SubItems);
	CRUX_CACHE_ADD_COLUMN_GETTER(32, FDB::PointerString, AudioEventUse);
	CRUX_CACHE_ADD_COLUMN_GETTER(33, bool, NoEquipAnimation);
	CRUX_CACHE_ADD_COLUMN_GETTER(34, std::int32_t, CommendationLOT);
	CRUX_CACHE_ADD_COLUMN_GETTER(35, std::int32_t, CommendationCost);
	CRUX_CACHE_ADD_COLUMN_GETTER(36, FDB::PointerString, AudioEquipMetaEventSet);
	CRUX_CACHE_ADD_COLUMN_GETTER(37, FDB::PointerString, CurrencyCosts);
	CRUX_CACHE_ADD_COLUMN_GETTER(38, FDB::PointerString, IngredientInfo);
	CRUX_CACHE_ADD_COLUMN_GETTER(39, std::int32_t, LocaleStatus);
	CRUX_CACHE_ADD_COLUMN_GETTER(40, std::int32_t, ForgeType);
	CRUX_CACHE_ADD_COLUMN_GETTER(41, std::float_t, SellMultiplier);
};

#endif // !__GAMECACHE__ITEMCOMPONENT_HPP__
