#ifndef __DATABASE__DATABASEMODELS_HPP__
#define __DATABASE__DATABASEMODELS_HPP__

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
	unsigned long long charIndex;
	unsigned long TotalCurrencyCollected;
	unsigned long TotalBricksCollected;
	unsigned long TotalSmashablesSmashed;
	unsigned long TotalQuickBuildsCompleted;
	unsigned long TotalEnemiesSmashed;
	unsigned long TotalRocketsUsed;
	unsigned long TotalMissionsCompleted;
	unsigned long TotalPetsTamed;
	unsigned long TotalImaginationPowerUpsCollected;
	unsigned long TotalLifePowerUpsCollected;
	unsigned long TotalArmorPowerUpsCollected;
	unsigned long TotalDistanceTraveled;
	unsigned long TotalSuicides;
	unsigned long TotalDamageTaken;
	unsigned long TotalDamageHealed;
	unsigned long TotalArmorRepaired;
	unsigned long TotalImaginationRestored;
	unsigned long TotalImaginationUsed;
	unsigned long TotalDistanceDriven;
	unsigned long TotalTimeAirborne;
	unsigned long TotalRacingImaginationPowerUpsCollected;
	unsigned long TotalRacingImaginationCratesSmashed;
	unsigned long TotalRacecarBoostsActivated;
	unsigned long TotalRacecarWrecks;
	unsigned long TotalRacingSmashablesSmashed;
	unsigned long TotalRacesFinished;
	unsigned long TotalFirstPlaceFinishes;
};

struct Str_DB_CharInfo {
public: 
	unsigned long accountID;
	unsigned long long objectID;
	unsigned char charIndex;
	std::string name = "";
	std::string pendingName = "";
	unsigned int styleID;
	unsigned int statsID;
	unsigned short lastWorld;
	unsigned short lastInstance;
	unsigned int lastClone;
	unsigned long long lastLog;
	DataTypes::Vector3 position;
	unsigned int shirtObjectID;
	unsigned int pantsObjectID;
	unsigned int uScore;
	unsigned int uLevel;
	unsigned int currency;
	unsigned int reputation;
	unsigned int health;
	unsigned int imagination;
	unsigned int armor;
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
	std::uint32_t templateID;
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
			if (index > 0x08) throw std::out_of_range("Index higher than 8");
			return data & (std::uint16_t(1) << index);
		}

		inline void SetAttribute(std::uint16_t index, bool flag) {
			if (index > 0x08) throw std::out_of_range("Index higher than 8");
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
}

#endif