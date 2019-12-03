#ifndef __GAMECACHE__MISSIONS_HPP__
#define __GAMECACHE__MISSIONS_HPP__

#include "Interface/FastDatabase.hpp"
using namespace GameCache::Interface;
extern FDB::Connection Cache;

namespace CacheMissions {
	inline FDB::RowInfo getRow(int32_t id) {
		FDB::RowTopHeader rth = Cache.getRows("Missions");
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


	//////////////////////////////////////////////////////////

	// id	INTEGER

	inline std::int32_t GetId(FDB::RowInfo row) {
		return row[0];
	}

	// defined_type	TEXT

	inline FDB::PointerString GetDefinedType(FDB::RowInfo row) {
		return row[1];
	}

	inline FDB::PointerString GetDefinedType(std::int32_t id) {
		return GetDefinedType(getRow(id));
	}

	// defined_subtype	TEXT
	
	inline FDB::PointerString GetDefinedSubType(FDB::RowInfo row) {
		return row[2];
	}

	inline FDB::PointerString GetDefinedSubType(std::int32_t id) {
		return GetDefinedSubType(getRow(id));
	}

	// UISortOrder	INTEGER

	inline std::int32_t GetUISortOrder(FDB::RowInfo row) {
		return row[3];
	}

	inline std::int32_t GetUISortOrder(std::int32_t id) {
		return GetUISortOrder(getRow(id));
	}

	// offer_objectID	INTEGER

	inline std::int32_t GetOfferObjectID(FDB::RowInfo row) {
		return row[4];
	}

	inline std::int32_t GetOfferObjectID(std::int32_t id) {
		return GetOfferObjectID(getRow(id));
	}

	// target_objectID	INTEGER

	inline std::int32_t GetTargetObjectID(FDB::RowInfo row) {
		return row[5];
	}

	inline std::int32_t GetTargetObjectID(std::int32_t id) {
		return GetUISortOrder(getRow(id));
	}

	// reward_currency	BIGINT

	inline std::int64_t GetRewardCurrency(FDB::RowInfo row) {
		return row[6];
	}

	inline std::int64_t GetRewardCurrency(std::int32_t id) {
		return GetRewardCurrency(getRow(id));
	}

	// LegoScore	INTEGER

	inline std::int32_t GetLegoScore(FDB::RowInfo row) {
		return row[7];
	}

	inline std::int32_t GetLegoScore(std::int32_t id) {
		return GetLegoScore(getRow(id));
	}

	// reward_reputation	BIGINT

	inline std::int64_t GetRewardReputation(FDB::RowInfo row) {
		return row[8];
	}

	inline std::int64_t GetRewardReputation(std::int32_t id) {
		return GetRewardReputation(getRow(id));
	}

	// isChoiceReward	BOOLEAN

	inline bool GetIsChoiceReward(FDB::RowInfo row) {
		return row[9];
	}

	inline bool GetIsChoiceReward(std::int32_t id) {
		return GetIsChoiceReward(getRow(id));
	}

	// reward_item1	INTEGER

	inline std::int32_t GetRewardItem1(FDB::RowInfo row) {
		return row[10];
	}

	inline std::int32_t GetRewardItem1(std::int32_t id) {
		return GetRewardItem1(getRow(id));
	}

	// reward_item1_count	INTEGER

	inline std::int32_t GetRewardItem1Count(FDB::RowInfo row) {
		return row[11];
	}

	inline std::int32_t GetRewardItem1Count(std::int32_t id) {
		return GetRewardItem1Count(getRow(id));
	}

	// reward_item2	INTEGER

	inline std::int32_t GetRewardItem2(FDB::RowInfo row) {
		return row[12];
	}

	inline std::int32_t GetRewardItem2(std::int32_t id) {
		return GetRewardItem2(getRow(id));
	}

	// reward_item2_count	INTEGER

	inline std::int32_t GetRewardItem2Count(FDB::RowInfo row) {
		return row[13];
	}

	inline std::int32_t GetRewardItem2Count(std::int32_t id) {
		return GetRewardItem2Count(getRow(id));
	}

	// reward_item3	INTEGER

	inline std::int32_t GetRewardItem3(FDB::RowInfo row) {
		return row[14];
	}

	inline std::int32_t GetRewardItem3(std::int32_t id) {
		return GetRewardItem3(getRow(id));
	}

	// reward_item3_count	INTEGER

	inline std::int32_t GetRewardItem3Count(FDB::RowInfo row) {
		return row[15];
	}

	inline std::int32_t GetRewardItem3Count(std::int32_t id) {
		return GetRewardItem3Count(getRow(id));
	}

	// reward_item4	INTEGER

	inline std::int32_t GetRewardItem4(FDB::RowInfo row) {
		return row[16];
	}

	inline std::int32_t GetRewardItem4(std::int32_t id) {
		return GetRewardItem4(getRow(id));
	}

	// reward_item4_count	INTEGER

	inline std::int32_t GetRewardItem4Count(FDB::RowInfo row) {
		return row[17];
	}

	inline std::int32_t GetRewardItem4Count(std::int32_t id) {
		return GetRewardItem4Count(getRow(id));
	}

	// reward_emote	INTEGER

	inline std::int32_t GetRewardEmote(FDB::RowInfo row) {
		return row[18];
	}

	inline std::int32_t GetRewardEmote(std::int32_t id) {
		return GetRewardEmote(getRow(id));
	}

	// reward_emote2	INTEGER

	inline std::int32_t GetRewardEmote2(FDB::RowInfo row) {
		return row[19];
	}

	inline std::int32_t GetRewardEmote2(std::int32_t id) {
		return GetRewardEmote2(getRow(id));
	}

	// reward_emote3	INTEGER

	inline std::int32_t GetRewardEmote3(FDB::RowInfo row) {
		return row[20];
	}

	inline std::int32_t GetRewardEmote3(std::int32_t id) {
		return GetRewardEmote3(getRow(id));
	}

	// reward_emote4	INTEGER

	inline std::int32_t GetRewardEmote4(FDB::RowInfo row) {
		return row[21];
	}

	inline std::int32_t GetRewardEmote4(std::int32_t id) {
		return GetRewardEmote4(getRow(id));
	}

	// reward_maximagination	INTEGER

	inline std::int32_t GetRewardMaxImagination(FDB::RowInfo row) {
		return row[22];
	}

	inline std::int32_t GetRewardMaxImagination(std::int32_t id) {
		return GetRewardMaxImagination(getRow(id));
	}

	// reward_maxhealth	INTEGER

	inline std::int32_t GetRewardMaxHealth(FDB::RowInfo row) {
		return row[23];
	}

	inline std::int32_t GetRewardMaxHealth(std::int32_t id) {
		return GetRewardMaxHealth(getRow(id));
	}

	// reward_maxinventory	INTEGER

	inline std::int32_t GetRewardMaxInventory(FDB::RowInfo row) {
		return row[24];
	}

	inline std::int32_t GetRewardMaxInventory(std::int32_t id) {
		return GetRewardMaxInventory(getRow(id));
	}

	// reward_maxmodel	INTEGER

	inline std::int32_t GetRewardMaxModel(FDB::RowInfo row) {
		return row[25];
	}

	inline std::int32_t GetRewardMaxModel(std::int32_t id) {
		return GetRewardMaxModel(getRow(id));
	}

	// reward_maxwidget	INTEGER

	inline std::int32_t GetRewardMaxWidget(FDB::RowInfo row) {
		return row[26];
	}

	inline std::int32_t GetRewardMaxWidget(std::int32_t id) {
		return GetRewardMaxWidget(getRow(id));
	}

	// reward_maxwallet	BIGINT

	inline std::int64_t GetRewardMaxWallet(FDB::RowInfo row) {
		return row[27];
	}

	inline std::int64_t GetRewardMaxWallet(std::int32_t id) {
		return GetRewardMaxWallet(getRow(id));
	}

	// repeatable	BOOLEAN

	inline bool GetRepeatable(FDB::RowInfo row) {
		return row[28];
	}

	inline bool GetRepeatable(std::int32_t id) {
		return GetRepeatable(getRow(id));
	}

	// reward_currency_repeatable	BIGINT

	inline std::int64_t GetRewardCurrencyRepeatable(FDB::RowInfo row) {
		return row[29];
	}

	inline std::int64_t GetRewardCurrencyRepeatable(std::int32_t id) {
		return GetRewardCurrencyRepeatable(getRow(id));
	}

	// reward_item1_repeatable	INTEGER

	inline std::int32_t GetItem1Repeatable(FDB::RowInfo row) {
		return row[30];
	}

	inline std::int32_t GetItem1Repeatable(std::int32_t id) {
		return GetItem1Repeatable(getRow(id));
	}

	// reward_item1_repeat_count	INTEGER

	inline std::int32_t GetItem1RepeatCount(FDB::RowInfo row) {
		return row[31];
	}

	inline std::int32_t GetItem1RepeatCount(std::int32_t id) {
		return GetItem1RepeatCount(getRow(id));
	}

	// reward_item2_repeatable	INTEGER

	inline std::int32_t GetItem2Repeatable(FDB::RowInfo row) {
		return row[32];
	}

	inline std::int32_t GetItem2Repeatable(std::int32_t id) {
		return GetItem2Repeatable(getRow(id));
	}

	// reward_item2_repeat_count	INTEGER

	inline std::int32_t GetItem2RepeatCount(FDB::RowInfo row) {
		return row[33];
	}

	inline std::int32_t GetItem2RepeatCount(std::int32_t id) {
		return GetItem2RepeatCount(getRow(id));
	}

	// reward_item3_repeatable	INTEGER

	inline std::int32_t GetItem3Repeatable(FDB::RowInfo row) {
		return row[34];
	}

	inline std::int32_t GetItem3Repeatable(std::int32_t id) {
		return GetItem3Repeatable(getRow(id));
	}

	// reward_item3_repeat_count	INTEGER

	inline std::int32_t GetItem3RepeatCount(FDB::RowInfo row) {
		return row[35];
	}

	inline std::int32_t GetItem3RepeatCount(std::int32_t id) {
		return GetItem3RepeatCount(getRow(id));
	}

	// reward_item4_repeatable	INTEGER

	inline std::int32_t GetItem4Repeatable(FDB::RowInfo row) {
		return row[36];
	}

	inline std::int32_t GetItem4Repeatable(std::int32_t id) {
		return GetItem4Repeatable(getRow(id));
	}

	// reward_item4_repeat_count	INTEGER

	inline std::int32_t GetItem4RepeatCount(FDB::RowInfo row) {
		return row[37];
	}

	inline std::int32_t GetItem4RepeatCount(std::int32_t id) {
		return GetItem4RepeatCount(getRow(id));
	}

	// time_limit	INTEGER

	inline std::int32_t GetTimeLimit(FDB::RowInfo row) {
		return row[38];
	}

	inline std::int32_t GetTimeLimit(std::int32_t id) {
		return GetTimeLimit(getRow(id));
	}

	// isMission	BOOLEAN

	inline bool GetIsMission(FDB::RowInfo row) {
		return row[39];
	}

	inline bool GetIsMission(std::int32_t id) {
		return GetIsMission(getRow(id));
	}

	// missionIconID	INTEGER

	inline std::int32_t GetMissionIconID(FDB::RowInfo row) {
		return row[40];
	}

	inline std::int32_t GetMissionIconID(std::int32_t id) {
		return GetMissionIconID(getRow(id));
	}

	// prereqMissionID	TEXT

	inline FDB::PointerString GetPrereqMissionID(FDB::RowInfo row) {
		return row[41];
	}

	inline FDB::PointerString GetPrereqMissionID(std::int32_t id) {
		return GetPrereqMissionID(getRow(id));
	}

	// localize	BOOLEAN

	inline bool GetLocalize(FDB::RowInfo row) {
		return row[42];
	}

	inline bool GetLocalize(std::int32_t id) {
		return GetLocalize(getRow(id));
	}

	// inMOTD	BOOLEAN

	inline bool GetInMOTD(FDB::RowInfo row) {
		return row[43];
	}

	inline bool GetInMOTD(std::int32_t id) {
		return GetInMOTD(getRow(id));
	}

	// cooldownTime	BIGINT

	inline std::int64_t GetCooldownTime(FDB::RowInfo row) {
		return row[44];
	}

	inline std::int64_t GetCooldownTime(std::int32_t id) {
		return GetCooldownTime(getRow(id));
	}

	// isRandom	BOOLEAN

	inline bool GetIsRandom(FDB::RowInfo row) {
		return row[45];
	}

	inline bool GetIsRandom(std::int32_t id) {
		return GetIsRandom(getRow(id));
	}

	// randomPool	TEXT
	
	inline FDB::PointerString GetRandomPool(FDB::RowInfo row) {
		return row[46];
	}

	inline FDB::PointerString GetRandomPool(std::int32_t id) {
		return GetRandomPool(getRow(id));
	}
	
	// UIPrereqID	INTEGER

	inline std::int32_t GetUIPrereqID(FDB::RowInfo row) {
		return row[47];
	}

	inline std::int32_t GetUIPrereqID(std::int32_t id) {
		return GetUIPrereqID(getRow(id));
	}

	// gate_version	TEXT

	inline FDB::PointerString GetGateVersion(FDB::RowInfo row) {
		return row[48];
	}

	inline FDB::PointerString GetGateVersion(std::int32_t id) {
		return GetGateVersion(getRow(id));
	}

	// HUDStates	TEXT

	inline FDB::PointerString GetHUDStates(FDB::RowInfo row) {
		return row[49];
	}

	inline FDB::PointerString GetHUDStates(std::int32_t id) {
		return GetHUDStates(getRow(id));
	}

	// locStatus	INTEGER

	inline std::int32_t GetLocStatus(FDB::RowInfo row) {
		return row[50];
	}

	inline std::int32_t GetLocStatus(std::int32_t id) {
		return GetLocStatus(getRow(id));
	}

	// reward_bankinventory	INTEGER

	inline std::int32_t GetRewardBankInventory(FDB::RowInfo row) {
		return row[51];
	}

	inline std::int32_t GetRewardBankInventory(std::int32_t id) {
		return GetRewardBankInventory(getRow(id));
	}

};

#endif // !__GAMECACHE__MISSIONS_HPP__
