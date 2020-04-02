#ifndef __REPLICA__COMPONENTS__CHARACTER_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__CHARACTER_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Database/Database.hpp"


#include "Entity/GameMessages/SetFlag.hpp"

class CharacterComponent : public IEntityComponent {
private:
	DatabaseModels::Str_DB_CharInfo charInfo = DatabaseModels::Str_DB_CharInfo();
	DatabaseModels::Str_DB_CharStyle charStyle = DatabaseModels::Str_DB_CharStyle();

	std::map<std::uint32_t, std::uint64_t> flags;

	// Dirty flags
	bool _dirtyPart2 = true;

	Entity::GameObject* mountedObject = nullptr;

	std::uint32_t gameActivity = 0;

	
	enum class WorldTransitionState : std::uint8_t {IN_WORLD, ENTERING_WORLD, LEAVING_WORLD} worldTransitionState = WorldTransitionState::ENTERING_WORLD;


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

	void SetActivity(std::uint32_t activity) {
		gameActivity = activity;
	}

	CharacterComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 4; }

	void InitCharInfo(DatabaseModels::Str_DB_CharInfo info) {
		charInfo = info;

		StatsComponent* statsComp = this->owner->GetComponent<StatsComponent>();
		if (statsComp != nullptr) {
			statsComp->SetDirty();
		}
	}

	void InitCharStyle(DatabaseModels::Str_DB_CharStyle style) {
		charStyle = style;
	}

	DatabaseModels::Str_DB_CharInfo GetCharInfo() {
		return charInfo;
	}

	DatabaseModels::Str_DB_CharStyle GetCharStyle() {
		return charStyle;
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
		std::uint32_t chunkID = flagIndex / 64;
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

	void OnSetFlag(Entity::GameObject* sender, GM::SetFlag* msg) {
		this->SetFlag(msg->iFlagID, msg->bFlag);
		MissionManager::LaunchTaskEvent(Enums::EMissionTask::FLAG, sender, owner->GetObjectID(), msg->iFlagID);
	}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		/* TODO: Part 1 Serialization */
		factory->Write(mountedObject != nullptr);
		if (mountedObject != nullptr) {
			factory->Write(true);
			factory->Write<std::int64_t>(mountedObject->GetObjectID());
			factory->Write<std::uint8_t>(1);
		}

		/* Part 2 Serialization */
		// ENABLE_FLAG_ON_CONSTRUCTION(_dirtyPart2);
		factory->Write(true);
		if (true) {
			factory->Write<std::uint32_t>(charInfo.uLevel);
			_dirtyPart2 = false;
		}

		/* TODO: Part 3 Serialization */
		factory->Write(false);

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

			// TODO: Char Stats
			for (int i = 0; i < 27; ++i)
				factory->Write<std::uint64_t>(i);

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
		factory->Write(false);
		
		// TODO: Activity
		factory->Write(true);
		factory->Write<std::uint32_t>(gameActivity);

		// TODO: Guilds
		factory->Write(false);
	}

	void Awake() {
		StatsComponent * statsComp = this->owner->GetComponent<StatsComponent>();
		if (statsComp != nullptr) {
			statsComp->attributes.maxImagination = statsComp->attributes.currentImagination = GetImagination();
		}
		flags = Database::GetFlagChunks(owner->GetObjectID().getPureID());
	}
};

#endif