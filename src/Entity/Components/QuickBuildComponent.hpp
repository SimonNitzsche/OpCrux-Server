#ifndef __REPLICA__COMPONENTS__QUICKBUILD_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__QUICKBUILD_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "Entity/Components/StatsComponent.hpp"
#include "Entity/Components/PhantomPhysicsComponent.hpp"
#include "Entity/Components/ScriptedActivityComponent.hpp"
#include "Entity/GameObject.hpp"
#include "GameCache/RebuildComponent.hpp"

using namespace DataTypes;

struct ScriptedActivityStructEntry {
	DataTypes::LWOOBJID playerID;
	float parameters[10];
};

class QuickbuildComponent : public ScriptedActivityComponent {
	friend class ScriptedActivityComponent;
private:
	StatsComponent * statsComponent;

	bool _isDirtyFlagActivity = false;
	bool _isDirtyFlag = false;

	DataTypes::Vector3 rebuild_activators;

	Entity::GameObject* activator = nullptr;

	/*
		0 = Open
		2 = Completed
		4 = Resetting
		5 = Building
		6 = Incomplete
	*/
	std::uint32_t qbState = 0;
	bool qbResetEffect = false;
	bool qbEnabled = true;
	std::float_t timeSinceStartOfBuild = 0.0f;
	std::float_t timeOfPausedRebuilds = 0.0f;


	std::float_t completionTime;
	std::float_t resetTime;
	std::float_t timeSmash;

	clock_t buildStartTime = 0;
	std::int32_t playerStartImagination;
	clock_t buildCompleteTime = 0;
	Entity::GameObject * buildingPlayer = nullptr;

public:

	QuickbuildComponent(std::int32_t componentID) : ScriptedActivityComponent(componentID) {}

	static constexpr int GetTypeID() { return 48; }

	void OnEnable() {
		if ((statsComponent = ScriptedActivityComponent::owner->GetComponent<StatsComponent>()) == nullptr) {
			ScriptedActivityComponent::owner->AddComponent<StatsComponent>(0);
			statsComponent = owner->GetComponent<StatsComponent>();

			if (statsComponent == nullptr) {
				Logger::log("WRLD", "Something went wrong QuickbuildComponent::OnEnable()");
				statsComponent = new StatsComponent(-1);
			}
		}

		auto cacheRow = CacheRebuildComponent::getRow(GetComponentID());

		ScriptedActivityComponent::activityID = CacheRebuildComponent::GetActivityID(cacheRow);

	}

	void Awake() {
		
	}

	void Update() {
		// When building
		if (qbState == 5) {
			// When build time is up
			if ((buildStartTime - ::time(0) + completionTime) <= .0f) {
				buildCompleteTime = ::time(0) + std::uint32_t(completionTime);
				{GM::RebuildNotifyState msg; msg.player = buildingPlayer->GetObjectID(); msg.iPrevState = qbState; msg.iState = (qbState = 2); GameMessages::Broadcast(this->owner, msg); owner->CallMessage(msg); }
				{GM::PlayFXEffect msg; msg.effectID = 507; msg.effectType = u"create"; msg.fScale = 1.0f; msg.name = "BrickFadeUpVisCompleteEffect"; msg.priority = 0.4000000059604645f; msg.serialize = true; GameMessages::Broadcast(this->owner, msg); }
				{GM::EnableRebuild msg; msg.user = buildingPlayer->GetObjectID(); msg.bSuccess; msg.fDuration = completionTime; GameMessages::Broadcast(this->owner, msg); }
				{GM::TerminateInteraction msg; msg.ObjIDTerminator = buildingPlayer->GetObjectID(); msg.type = Enums::ETerminateType::FROM_INTERACTION; GameMessages::Broadcast(this->owner, msg); }


				activator->InstantiateRemoval();
				activator = nullptr;

				this->_isDirtyFlag = true;
				this->owner->SetDirty();

				owner->GetZoneInstance()->objectsManager->Serialize(this->owner);

				CompleteActivity();
				buildingPlayer->SetPlayerActivity(Enums::EGameActivity::NONE);
			}
		}
		// When completed
		else if (qbState == 2) {
			if (resetTime > 0) {
				auto doResetTime = buildCompleteTime + std::uint32_t(resetTime);

				//Reset indicated
				if (qbResetEffect)
					doResetTime += std::uint32_t(timeSmash);

				auto now = ::time(0);

				// indicate resetting
				if (!qbResetEffect && now > doResetTime) {
					qbResetEffect = true;

					this->_isDirtyFlag = true;
					this->owner->SetDirty();
				}

				// reset
				else if (qbResetEffect && now > doResetTime) {
					GM::RebuildNotifyState msg;
					msg.player = buildingPlayer->GetObjectID();
					msg.iPrevState = qbState;
					msg.iState = (qbState = 4);
					GameMessages::Broadcast(this->owner, msg);

					this->_isDirtyFlag = true;
					this->owner->SetDirty();

					RemovePlayerFromActivity(buildingPlayer->GetObjectID());
				}
			}
		}
		else if (qbState == 4) {
			// Die
			{GM::Die nmsg; nmsg.bSpawnLoot = true; nmsg.killType = EKillType::VIOLENT; GameMessages::Broadcast(owner, nmsg); }
			// StopFX Effect
			{GM::StopFXEffect nmsg; nmsg.name = "BrickFadeUpVisCompleteEffect"; GameMessages::Broadcast(owner, nmsg); }
			// Remove
			owner->InstantiateRemoval();
		}
	}

	void PopulateFromLDF(LDFCollection * collection) {
		/*SAMPLE*/
		/*
		actIDovrd=7:1
		activityID=1:45
		celebAnim=0:
		compTime=3:5
		grpNameQBShowBricks=0:
		grp_name=0:
		lead_in_time=3:0
		lock_controls=7:1
		num_targets_to_activate=1:1
		qbStrtBlt=7:0
		quickbuild_cinematic=0:QBStarboardAft
		quickbuild_cinematic_lead_in=3:0
		quickbuild_single_build=7:0
		quickbuild_single_build_player_flag=1:0
		quickbuild_single_build_shared=7:0
		quickbuild_skip_slam=7:0
		rebuild_activators=0:-494.0430 580.970 918.9588
		rebuild_reset_time=3:10
		spawnActivator=7:1
		startsQBActivator=7:0
		tmeSmsh=3:10
		vlntDth=7:1
		*/

		ScriptedActivityComponent::PopulateFromLDF(collection);

		auto cacheRow = CacheRebuildComponent::getRow(GetComponentID());

		LDF_GET_VAL_FROM_COLLECTION(completionTime, collection, u"compTime", CacheRebuildComponent::GetCompleteTime(cacheRow));
		LDF_GET_VAL_FROM_COLLECTION(resetTime, collection, u"rebuild_reset_time", CacheRebuildComponent::GetResetTime(cacheRow));
		LDF_GET_VAL_FROM_COLLECTION(timeSmash, collection, u"tmeSmsh", CacheRebuildComponent::GetTimeBeforeSmash(cacheRow));
		

		std::u16string wRebuildPos;
		LDF_GET_VAL_FROM_COLLECTION(wRebuildPos, collection, u"rebuild_activators", u"NULL");
		std::vector<std::u16string> vecPos = StringUtils::splitWString(wRebuildPos, 0x001f);
		if (wRebuildPos != u"NULL")
			rebuild_activators = DataTypes::Vector3(std::stof(StringUtils::to_string(vecPos.at(0))), std::stof(StringUtils::to_string(vecPos.at(1))), std::stof(StringUtils::to_string(vecPos.at(2))));


		// Make Activator
		activator = new Entity::GameObject(ScriptedActivityComponent::owner->GetZoneInstance(), 6604);
		activator->SetObjectID(owner->GetZoneInstance()->objectsManager->GenerateSpawnedID());
		ScriptedActivityComponent::owner->AddChild(activator);
		//activator->isSerializable = true;
		activator->SetPosition(rebuild_activators);
		ScriptedActivityComponent::owner->GetZoneInstance()->objectsManager->RegisterObject(activator);
		ScriptedActivityComponent::owner->GetZoneInstance()->objectsManager->Construct(activator);
	}

	void Serialize(RakNet::BitStream* factory, ReplicaTypes::PacketTypes packetType) {
		/* TODO */
		// Check if Destructible or Collectible component is attached, if so don't serialize
		if (ScriptedActivityComponent::owner->GetComponent<DestructibleComponent>() == nullptr && ScriptedActivityComponent::owner->GetComponent<CollectibleComponent>() == nullptr)
			statsComponent->Serialize(factory, packetType);

		ScriptedActivityComponent::Serialize(factory, packetType);

		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION)
			_isDirtyFlag = true;

		if (qbState == 5)
			_isDirtyFlag = true;

		factory->Write(_isDirtyFlag);
		if (_isDirtyFlag) {
			factory->Write<std::uint32_t>(qbState);
			factory->Write(qbResetEffect);
			factory->Write(qbEnabled);
			timeSinceStartOfBuild = ::time(0) - buildStartTime;
			factory->Write<std::float_t>(timeSinceStartOfBuild);
			factory->Write<std::float_t>(timeOfPausedRebuilds);
			if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
				factory->Write(false);
				factory->Write(rebuild_activators.x);
				factory->Write(rebuild_activators.y);
				factory->Write(rebuild_activators.z);
				factory->Write(true);
			}
		}

		_isDirtyFlag = false;
		
	}

	void OnRequestUse(Entity::GameObject* sender, GM::RequestUse * msg) {
		// When active
		if (qbState == 0) {
			AddPlayerToActivity(sender->GetObjectID());
			this->_isDirtyFlag = true;
			owner->GetZoneInstance()->objectsManager->Serialize(this->owner);
			buildStartTime = ::time(0);
			buildingPlayer = sender;
			playerStartImagination = sender->GetImagination();
			{GM::EnableRebuild nmsg; nmsg.user = sender->GetObjectID(); nmsg.bEnable = true; nmsg.fDuration = completionTime; GameMessages::Broadcast(this->owner, nmsg); }
			{GM::RebuildNotifyState nmsg; nmsg.player = sender->GetObjectID(); nmsg.iPrevState = qbState; nmsg.iState = (qbState = 5); GameMessages::Broadcast(this->owner, nmsg); }

			this->_isDirtyFlag = true;
			this->owner->SetDirty();
			buildingPlayer->SetPlayerActivity(Enums::EGameActivity::QUICKBUILD);
		}

	}

	void OnServerTerminateInteraction(Entity::GameObject* sender, GM::ServerTerminateInteraction * msg) {
		if (buildingPlayer != nullptr) {
			buildingPlayer->SetPlayerActivity(Enums::EGameActivity::NONE);
		}
	}

	void OnRebuildCancel(Entity::GameObject* sender, GM::RebuildCancel * msg) {
		if (buildingPlayer != nullptr) {
			buildingPlayer->SetPlayerActivity(Enums::EGameActivity::NONE);
		}
	}

	void RegisterMessageHandlers() {
		REGISTER_OBJECT_MESSAGE_HANDLER(QuickbuildComponent, GM::RequestUse, OnRequestUse);
		REGISTER_OBJECT_MESSAGE_HANDLER(QuickbuildComponent, GM::ServerTerminateInteraction, OnServerTerminateInteraction);
		REGISTER_OBJECT_MESSAGE_HANDLER(QuickbuildComponent, GM::RebuildCancel, OnRebuildCancel);
	}
};

#endif