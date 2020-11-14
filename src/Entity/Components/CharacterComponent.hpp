#ifndef __REPLICA__COMPONENTS__CHARACTER_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__CHARACTER_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Database/Database.hpp"

#include "Entity/Components/SlashCommandComponent.hpp"

#include "Entity/GameMessages/SetFlag.hpp"

#include "Enums/EGameActivity.hpp"

#include "GameCache/LevelProgressionLookup.hpp"
#include "GameCache/ItemComponent.hpp"

class CharacterComponent : public IEntityComponent {
private:
	DatabaseModels::Str_DB_CharInfo charInfo = DatabaseModels::Str_DB_CharInfo();
	DatabaseModels::Str_DB_CharStyle charStyle = DatabaseModels::Str_DB_CharStyle();
	DatabaseModels::Str_DB_CharStats charStats = DatabaseModels::Str_DB_CharStats();

	std::map<std::uint32_t, std::uint64_t> flags;

	// Dirty flags
	bool _dirtyPart2 = true;

	Entity::GameObject* mountedObject = nullptr;

	Enums::EGameActivity gameActivity = Enums::EGameActivity::NONE;
		
	
	enum class WorldTransitionState : std::uint8_t {IN_WORLD, ENTERING_WORLD, LEAVING_WORLD} worldTransitionState = WorldTransitionState::ENTERING_WORLD;

	struct MatchLobby {
		std::int32_t t_0_activityID = 0;
		std::int32_t t_1_lobbyReady = 0;
	} matchLobby;

public:
	SystemAddress clientAddress = UNASSIGNED_SYSTEM_ADDRESS;

	void MountTo(Entity::GameObject * mount) {
		if (mountedObject != nullptr) {
			UnMount();
		}
		mountedObject = mount;
		owner->SetDirty();
	}

	void UnMount() {
		MountTo(nullptr);
	}

	void SetLevel(std::uint32_t newLevel) {
		charInfo.uLevel = newLevel;
	}

	std::uint32_t GetLevel() {
		return charInfo.uLevel;
	}

	void SetActivity(Enums::EGameActivity activity) {
		gameActivity = activity;
		this->owner->SetDirty();
	}

	CharacterComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 4; }

	void InitCharInfo(DatabaseModels::Str_DB_CharInfo info) {
		charInfo = info;

		//charStats = Database::GetCharStats(charInfo.statsID);

		StatsComponent* statsComp = this->owner->GetComponent<StatsComponent>();
		if (statsComp != nullptr) {
			statsComp->SetDirty();
		}
	}

	void InitCharStyle(DatabaseModels::Str_DB_CharStyle style) {
		charStyle = style;
	}

	DatabaseModels::Str_DB_CharInfo & GetCharInfo() {
		return charInfo;
	}

	DatabaseModels::Str_DB_CharStyle & GetCharStyle() {
		return charStyle;
	}

	DatabaseModels::Str_DB_CharStats & GetCharStats() {
		return charStats;
	}

	void SetImagination(std::int32_t imag) {
		charInfo.imagination = imag;
		Database::UpdateChar(charInfo);
	}

	std::int32_t GetImagination() {
		return charInfo.imagination;
	}

	std::map<std::uint32_t, std::uint64_t> GetFlagChunks() {
		return flags;
	}

	bool GetFlag(std::int32_t flagIndex) {
		//auto flagChunk = std::find(flags.begin(), flags.end(), flagIndex/64);
		auto flagChunk = flags.find(flagIndex / 64);
		if (flagChunk != flags.end()) {
			return (flagChunk->second & (1ULL << (flagIndex % 64)));
		}
		return false;
	}

	void SetFlag(std::int32_t flagIndex, bool value) {
		// Set flag
		//auto flagChunk = std::find(flags.begin(), flags.end(), flagIndex / 64);
		std::uint32_t chunkID = std::floor(flagIndex / 64);
		std::uint64_t chunkData;
		auto flagChunk = flags.find(chunkID);
		if (flagChunk != flags.end()) {
			flagChunk->second ^= (-value ^ flagChunk->second) & (1ULL << (flagIndex % 64));
			chunkData = flagChunk->second;
		}
		else {
			chunkData = 1ULL << (flagIndex % 64);
			flags.insert({ chunkID, chunkData });
		}


		Database::SetFlag(this->owner->GetObjectID(), chunkID, chunkData);
		GM::NotifyClientFlagChange clientResponse;
		clientResponse.bFlag = value;
		clientResponse.iFlagID = flagIndex;
		GameMessages::Send(this->owner, this->owner->GetObjectID(), clientResponse);
	}

	void OnSetFlag(Entity::GameObject* sender, GM::SetFlag & msg) {
		this->SetFlag(msg.iFlagID, msg.bFlag);
		MissionManager::LaunchTaskEvent(Enums::EMissionTask::FLAG, sender, owner->GetObjectID(), msg.iFlagID);
	}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		/* Part 1 Serialization */
		factory->Write(mountedObject != nullptr);
		if (mountedObject != nullptr) {
			factory->Write(true);
			factory->Write<std::int64_t>(mountedObject->GetObjectID());
			factory->Write<std::uint8_t>(0);
		}

		/* Part 2 Serialization */
		// ENABLE_FLAG_ON_CONSTRUCTION(_dirtyPart2);
		factory->Write(true);
		if (true) {
			factory->Write<std::uint32_t>(charInfo.uLevel);
			_dirtyPart2 = false;
		}

		/* Part 3 Serialization */
		factory->Write(true); // Unknown
		factory->Write(false); // Unknown
		factory->Write(true); // Unknown

		/* Part 4 Serialization */
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
			factory->Write(false); // Unknown
			factory->Write(false); // Unknown
			factory->Write(false); // Unknown
			factory->Write(false); // Unknown
		
			// Char Style
			auto beginSec = factory->GetWriteOffset();
			factory->Write<std::uint32_t>(charStyle.hairColor);
			factory->Write<std::uint32_t>(charStyle.hairStyle);
			factory->Write<std::uint32_t>(0); // TODO: ???, could be "hd" or "hdc" from xml data
			factory->Write<std::uint32_t>(charStyle.chestColor);
			factory->Write<std::uint32_t>(charStyle.legs);
			factory->Write<std::uint32_t>(1); // TODO: ???, could be "cd" from xml data
			factory->Write<std::uint32_t>(2); // TODO: ???, could be "hdc" or "hd" from xml data
			factory->Write<std::uint32_t>(charStyle.eyebrowStyle);
			factory->Write<std::uint32_t>(charStyle.eyesStyle);
			factory->Write<std::uint32_t>(charStyle.mouthStyle);

			// Char Info
			factory->Write<std::uint64_t>(charInfo.accountID);
			factory->Write<std::uint64_t>(charInfo.lastLog);
			factory->Write<std::uint64_t>(0); // Unknown 
			factory->Write<std::uint64_t>(charInfo.uScore);
			factory->Write(false); // is player free to play

			// Char Stats
			factory->Write<std::uint64_t>(charStats.TotalCurrencyCollected);
			factory->Write<std::uint64_t>(charStats.TotalBricksCollected);
			factory->Write<std::uint64_t>(charStats.TotalSmashablesSmashed);
			factory->Write<std::uint64_t>(charStats.TotalQuickBuildsCompleted);
			factory->Write<std::uint64_t>(charStats.TotalEnemiesSmashed);
			factory->Write<std::uint64_t>(charStats.TotalRocketsUsed);
			factory->Write<std::uint64_t>(charStats.TotalMissionsCompleted);
			factory->Write<std::uint64_t>(charStats.TotalPetsTamed);
			factory->Write<std::uint64_t>(charStats.TotalImaginationPowerUpsCollected);
			factory->Write<std::uint64_t>(charStats.TotalLifePowerUpsCollected);
			factory->Write<std::uint64_t>(charStats.TotalArmorPowerUpsCollected);
			factory->Write<std::uint64_t>(charStats.TotalDistanceTraveled);
			factory->Write<std::uint64_t>(charStats.TotalSuicides);
			factory->Write<std::uint64_t>(charStats.TotalDamageTaken);
			factory->Write<std::uint64_t>(charStats.TotalDamageHealed);
			factory->Write<std::uint64_t>(charStats.TotalArmorRepaired);
			factory->Write<std::uint64_t>(charStats.TotalImaginationRestored);
			factory->Write<std::uint64_t>(charStats.TotalImaginationUsed);
			factory->Write<std::uint64_t>(charStats.TotalDistanceDriven);
			factory->Write<std::uint64_t>(charStats.TotalTimeAirborne);
			factory->Write<std::uint64_t>(charStats.TotalRacingImaginationPowerUpsCollected);
			factory->Write<std::uint64_t>(charStats.TotalRacingImaginationCratesSmashed);
			factory->Write<std::uint64_t>(charStats.TotalRacecarBoostsActivated);
			factory->Write<std::uint64_t>(charStats.TotalRacecarWrecks);
			factory->Write<std::uint64_t>(charStats.TotalRacingSmashablesSmashed);
			factory->Write<std::uint64_t>(charStats.TotalRacesFinished);
			factory->Write<std::uint64_t>(charStats.TotalFirstPlaceFinishes);

			auto endSec = factory->GetWriteOffset();

			auto sizeSec = endSec - beginSec;

			// World transition state
			factory->Write<bool>(static_cast<std::uint8_t>(worldTransitionState) == 2);
			factory->Write<bool>(static_cast<std::uint8_t>(worldTransitionState) == 1);

			if (worldTransitionState == WorldTransitionState::ENTERING_WORLD) {
				// TODO: Rocket Info
				std::u16string rocketBuild = u"1:9746;1:9747;1:9748;";
				StringUtils::writeWStringToBitStream<std::uint16_t>(factory, rocketBuild);
			}
		}

		// TODO: Additional flags
		factory->Write(true);

		factory->Write(false); // PVP Enabled?

		//std::uint8_t GMLevel = Database::GetAccountGMLevel(charInfo.accountID);
		//if (GMLevel > 0) {
		//	factory->Write(true); // Is GM?
		//	factory->Write<std::uint8_t>(GMLevel); // GM Level
		//}
		//else {
		//	factory->Write(false);
		//	factory->Write<std::uint8_t>(GMLevel);
		//}
		// This is commented because it will query the DB too much
		factory->Write(false); 
		factory->Write<std::uint8_t>(0);

		factory->Write(false); // Unknown
		factory->Write<std::uint8_t>(0); // Unknown
		
		// Activity
		factory->Write<bool>(true);
		if (true) {
			factory->Write<std::uint32_t>(std::uint32_t(gameActivity));
		}
		

		// TODO: Guilds
		factory->Write(false);

		/* Code to be used for guilds
		factory->Write<bool>(charInfo.InGuild); // In Guild?
		if (charInfo.InGuild) {
			factory->Write<std::int64_t>(guildID); // Guild ID (In OBJID format?)
			factory->Write<std::string>(guildName); // Guild Name
			factory->Write<bool>(guildLeader); // Guild Owner?
			factory->Write<std::uint32_t>(guildDate); // Guild Creation Date
		}
		*/
		
		
	}

	void Awake() {
		StatsComponent * statsComp = this->owner->GetComponent<StatsComponent>();
		if (statsComp != nullptr) {
			statsComp->attributes.maxImagination = statsComp->attributes.currentImagination = GetImagination();
		}

		if (owner->GetComponent<SlashCommandComponent>() == nullptr)
			owner->AddComponent<SlashCommandComponent>(0);

		flags = Database::GetFlagChunks(owner->GetObjectID().getPureID());
	}

	void OnPlayerLoaded(Entity::GameObject* sender, GM::PlayerLoaded& msg) {
		GM::PlayerReady nmsg;
		auto Instance = this->owner->GetZoneInstance();
		GameMessages::Send(Instance, this->clientAddress, Instance->zoneControlObject->GetObjectID(), nmsg);
		GameMessages::Send(Instance, this->clientAddress, this->owner->GetObjectID(), nmsg);
	}


	void OnRequestUse(Entity::GameObject* sender, GM::RequestUse& msg) {
		Entity::GameObject * objectToUse = this->owner->GetZoneInstance()->objectsManager->GetObjectByID(msg.objectID);


		// OnRequestUse is always received by the player and then redirected to the object internally
		if (objectToUse != nullptr && objectToUse != this->owner) {
			objectToUse->OnRequestUse(this->owner, msg);
		}
	}

	void OnPickupCurrency(Entity::GameObject* sender, GM::PickupCurrency& msg) {
		if (DataTypes::Vector3::Distance(owner->GetPosition(), msg.position) <= 28.0f) {
			// TODO: Anti Cheat
			charStats.TotalCurrencyCollected += msg.currency;
			{GM::UpdatePlayerStatistic nmsg; nmsg.updateID = std::uint32_t(EStats::TotalCurrencyCollected); nmsg.updateValue = msg.currency; GameMessages::Send(owner, owner->GetObjectID(), nmsg);}
			charInfo.currency += msg.currency;
			
			{GM::SetCurrency nmsg; nmsg.currency = charInfo.currency; nmsg.position = msg.position; nmsg.sourceType = 11; GameMessages::Send(owner, owner->GetObjectID(), nmsg); }

			Database::UpdateChar(charInfo);
		}
	}

	void CheckLevelProgression() {
		std::int32_t level = CacheLevelProgrssionLookup::GetLevelByUScorePassed(charInfo.uScore);
		if (level != charInfo.uScore) {
			charInfo.uLevel = level;
			Database::UpdateChar(charInfo);
		}
	}

	void OnUnEquipInventory(Entity::GameObject* sender, GM::UnEquipInventory& msg) {
		Entity::GameObject * itm = owner->GetZoneInstance()->objectsManager->GetObjectByID(msg.itemToUnEquip);
		if (itm == nullptr) return;
		auto itmCompID = itm->GetComponentByType(11)->GetComponentID();
		std::string equipLocation = CacheItemComponent::GetEquipLocation(itmCompID);

		if (equipLocation == "chest") {
			GM::EquipInventory nmsg;
			nmsg.itemToEquip = this->charInfo.shirtObjectID;
			owner->GetComponentByType(17)->OnEquipInventory(sender, nmsg);
		}
		else if (equipLocation == "legs") {
			GM::EquipInventory nmsg;
			nmsg.itemToEquip = this->charInfo.pantsObjectID;
			owner->GetComponentByType(17)->OnEquipInventory(sender, nmsg);
		}
	}

	void OnRespondToMission(Entity::GameObject* sender, GM::RespondToMission& msg) {
		auto model = Database::GetMission(sender->GetObjectID().getPureID(), msg.missionID);
		model.chosenReward = msg.rewardItem;
		Database::UpdateMission(model);
	}

	void OnPlayEmote(Entity::GameObject* sender, GM::PlayEmote& msg) {
		Entity::GameObject* target = sender->GetZoneInstance()->objectsManager->GetObjectByID(msg.targetID);
		if (target == nullptr) target = sender;

		// Sync
		GM::EmotePlayed nmsg; nmsg.emoteID = msg.emoteID; nmsg.targetID = msg.targetID;
		GameMessages::Broadcast(sender, nmsg);

		// Mission Task
		MissionManager::LaunchTaskEvent(Enums::EMissionTask::EMOTE, target, sender->GetObjectID(), msg.emoteID);
	}

	void OnMatchRequest(Entity::GameObject* sender, GM::MatchRequest& msg) {
		GM::MatchResponse response;
		
		response.response = 0;

		std::list<GM::MatchUpdate> updates = {};

		if (msg.type == 0) {
			matchLobby.t_0_activityID = msg.value;

			GM::MatchUpdate update00;

			update00.type = 0;
			update00.data = {
				LDF_COLLECTION_INIT_ENTRY(u"player", owner->GetObjectID()),
				LDF_COLLECTION_INIT_ENTRY(u"playerName", owner->GetName())
			};
			updates.push_back(update00);

			update00.type = 3;
			update00.data = {
				LDF_COLLECTION_INIT_ENTRY(u"time", 60.0f)
			};
			updates.push_back(update00);

		}

		else if (msg.type == 1) {
			matchLobby.t_1_lobbyReady = msg.value;

			GM::MatchUpdate update00;

			update00.type = 5;
			update00.data = {
				LDF_COLLECTION_INIT_ENTRY(u"player", owner->GetObjectID())
			};
			//updates.push_back(update00);

			update00.type = 4;
			update00.data = {
				LDF_COLLECTION_INIT_ENTRY(u"time", 5.0f),
			};
			updates.push_back(update00);

			ClientSession * clSession = owner->GetZoneInstance()->sessionManager.GetSession(owner->GetObjectID());
			ZoneInfo zi;

			zi.zoneID = 1101;

			masterServerBridge->ClientRequestMapChange(*clSession, zi);
		}

		GameMessages::Send(owner, owner->GetObjectID(), response);

		for (auto update : updates) {
			GameMessages::Send(owner, owner->GetObjectID(), update);
		}

	}
};

#endif