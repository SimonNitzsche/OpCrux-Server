#ifndef __DATABASE__DATABASEMODELS_HPP__
#define __DATABASE__DATABASEMODELS_HPP__

#include "DataTypes/Vector3.hpp"
#include "DataTypes/LDF.hpp"

namespace DatabaseModels {
	struct Str_DB_CharStyle {
public:
	int headColor;
	int head;
	int chestColor;
	int chest;
	int legs;
	int hairStyle;
	int hairColor;
	int leftHand;
	int rightHand;
	int eyebrowStyle;
	int eyesStyle;
	int mouthStyle;
};

struct Str_DB_CharStats {
public:
	std::uint64_t charIndex;
	std::uint64_t TotalCurrencyCollected;
	std::uint64_t TotalBricksCollected;
	std::uint64_t TotalSmashablesSmashed;
	std::uint64_t TotalQuickBuildsCompleted;
	std::uint64_t TotalEnemiesSmashed;
	std::uint64_t TotalRocketsUsed;
	std::uint64_t TotalMissionsCompleted;
	std::uint64_t TotalPetsTamed;
	std::uint64_t TotalImaginationPowerUpsCollected;
	std::uint64_t TotalLifePowerUpsCollected;
	std::uint64_t TotalArmorPowerUpsCollected;
	std::uint64_t TotalDistanceTraveled;
	std::uint64_t TotalSuicides;
	std::uint64_t TotalDamageTaken;
	std::uint64_t TotalDamageHealed;
	std::uint64_t TotalArmorRepaired;
	std::uint64_t TotalImaginationRestored;
	std::uint64_t TotalImaginationUsed;
	std::uint64_t TotalDistanceDriven;
	std::uint64_t TotalTimeAirborne;
	std::uint64_t TotalRacingImaginationPowerUpsCollected;
	std::uint64_t TotalRacingImaginationCratesSmashed;
	std::uint64_t TotalRacecarBoostsActivated;
	std::uint64_t TotalRacecarWrecks;
	std::uint64_t TotalRacingSmashablesSmashed;
	std::uint64_t TotalRacesFinished;
	std::uint64_t TotalFirstPlaceFinishes;
};

enum class EStats {
	TotalCurrencyCollected = 1,
	TotalBricksCollected,
	TotalSmashablesSmashed,
	TotalQuickBuildsCompleted,
	TotalEnemiesSmashed,
	TotalRocketsUsed,
	TotalMissionsCompleted,
	TotalPetsTamed,
	TotalImaginationPowerUpsCollected,
	TotalLifePowerUpsCollected,
	TotalArmorPowerUpsCollected,
	TotalDistanceTraveled,
	TotalSuicides,
	TotalDamageTaken,
	TotalDamageHealed,
	TotalArmorRepaired,
	TotalImaginationRestored,
	TotalImaginationUsed,
	TotalDistanceDriven,
	TotalTimeAirborne,
	TotalRacingImaginationPowerUpsCollected,
	TotalRacingImaginationCratesSmashed,
	TotalRacecarBoostsActivated,
	TotalRacecarWrecks,
	TotalRacingSmashablesSmashed,
	TotalRacesFinished,
	TotalFirstPlaceFinishes
};

struct Str_DB_CharInfo {
public: 
	std::uint32_t accountID;
	std::uint64_t objectID;
	unsigned char charIndex;
	std::string name = "";
	std::string pendingName = "";
	std::uint32_t styleID;
	std::uint32_t statsID;
	std::uint16_t lastWorld;
	std::uint16_t lastInstance;
	std::uint32_t lastClone;
	std::uint64_t lastLog;
	DataTypes::Vector3 position;
	std::uint64_t shirtObjectID;
	std::uint64_t pantsObjectID;
	std::uint32_t uScore;
	std::uint32_t uLevel;
	std::uint32_t currency;
	std::uint32_t reputation;
	std::uint32_t health;
	std::uint32_t imagination;
	std::uint32_t armor;
	unsigned int maxhealth;
	unsigned int maximagination;
	unsigned int maxarmor;
	unsigned int maxinventory;
	Str_DB_CharInfo() {
		name = "";
		pendingName = "";
	}
};

struct MissionModel {
	std::int64_t charID;
	std::int32_t missionID;
	std::int32_t state;
	std::string progress;
	std::int32_t repeatCount;
	std::int64_t time;
	std::int32_t chosenReward;

	void updateTime() {
		time = ::time(0);
	}

};

struct ItemModel {
	std::uint64_t objectID;
	std::uint64_t ownerID;
	std::uint64_t subkey;
	std::uint32_t tab;
	std::uint32_t slot;
	std::uint32_t templateID = -1;
	std::uint32_t count;
	struct ItemAttributes {
		/*
			0x01 = bound
			0x02 = equipped
		*/
	private:
		std::uint16_t data;
	public:
		ItemAttributes() {
			data = 0;
		}

		inline std::uint16_t GetAttributes() {
			return data;
		}

		inline void SetAttributes(std::uint16_t attributes) {
			data = attributes;
		}

		inline bool GetAttribute(std::uint16_t index) {
			if (index >= 0x08) throw std::out_of_range("Index higher than 8");
			return data & (std::uint16_t(1) << index);
		}

		inline void SetAttribute(std::uint16_t index, bool flag) {
			if (index >= 0x08) throw std::out_of_range("Index higher than 8");
			data ^= (-flag ^ data) & (std::uint16_t(1) << index);
		}

		inline bool GetBound() {
			return GetAttribute(0x00);
		}

		inline void SetBound(bool flag) {
			SetAttribute(0x00, flag);
		}

		inline bool GetEquipped() {
			return GetAttribute(0x01);
		}

		inline void SetEquipped(bool flag) {
			SetAttribute(0x01, flag);
		}
	} attributes;
	LDFCollection metadata;

	ItemModel() {
		subkey = 0;
		count = 1;
	}
};

struct MailModel {
	std::int64_t mailID; // Never set mailID manually, this will be taken care of the DB itself
	std::int64_t receiver;
	std::string subject;
	std::string body;
	std::string sender;
	std::int64_t attachedCurrency=std::int64_t(0);
	std::int64_t attachedObjectID = std::int64_t(0);
	std::int32_t attachedLOT;
	std::int64_t attachedSubkey = std::int64_t(0);
	std::int16_t attachmentCount=std::int64_t(0);
	std::int64_t expirationDate;
	std::int64_t sendDate;
	bool markedAsSeen;
	bool hasBeenModerated;
};
}

#endif
