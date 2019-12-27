#ifndef __REPLICA__COMPONENTS__QUICKBUILD_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__QUICKBUILD_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "Entity/Components/StatsComponent.hpp"
#include "Entity/Components/PhantomPhysicsComponent.hpp"
#include "Entity/Components/ScriptedActivityComponent.hpp"
#include "Entity/GameObject.hpp"

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

	/*
		0 = Open
		2 = Completed
		4 = Resetting
		5 = Building
		6 = Incomplete
	*/
	std::uint32_t qbState = 0;
	bool qbSuccess = false;
	bool qbEnabled = true;
	std::float_t timeSinceStartOfBuild = 0.0f;
	std::float_t timeOfPausedRebuilds = 0.0f;

public:

	QuickbuildComponent(std::int32_t componentID) : ScriptedActivityComponent(componentID) {}

	static constexpr int GetTypeID() { return 48; }

	void OnEnable() {
		if ((statsComponent = ScriptedActivityComponent::owner->GetComponent<StatsComponent>()) == nullptr) {
			ScriptedActivityComponent::owner->AddComponent<StatsComponent>(0);
			statsComponent = owner->GetComponent<StatsComponent>();

			if (statsComponent == nullptr) {
				Logger::log("WRLD", "Something went wrong QuickbuildComponent::OnEnable()");
				statsComponent = new StatsComponent(0);
			}
		}
	}

	void Awake() {
		
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

		std::u16string wRebuildPos;
		LDF_GET_VAL_FROM_COLLECTION(wRebuildPos, collection, u"rebuild_activators", u"NULL");
		std::vector<std::u16string> vecPos = StringUtils::splitWString(wRebuildPos, 0x001f);
		if (wRebuildPos != u"NULL")
			rebuild_activators = DataTypes::Vector3(std::stof(StringUtils::to_string(vecPos.at(0))), std::stof(StringUtils::to_string(vecPos.at(1))), std::stof(StringUtils::to_string(vecPos.at(2))));


		// Make Activator
		Entity::GameObject * activator = new Entity::GameObject(ScriptedActivityComponent::owner->GetZoneInstance(), 6604);
		activator->SetObjectID(DataTypes::LWOOBJID((1ULL << 58) + 104120439353844ULL + ScriptedActivityComponent::owner->GetZoneInstance()->spawnedObjectIDCounter++));
		ScriptedActivityComponent::owner->AddChild(activator);
		activator->isSerializable = true;
		activator->GetComponent<PhantomPhysicsComponent>()->SetPosition(rebuild_activators);
		ScriptedActivityComponent::owner->GetZoneInstance()->objectsManager->RegisterObject(activator);
	}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		/* TODO */
		// Check if Destructible or Collectible component is attached, if so don't serialize
		if (ScriptedActivityComponent::owner->GetComponent<DestructibleComponent>() == nullptr && ScriptedActivityComponent::owner->GetComponent<CollectibleComponent>() == nullptr)
			statsComponent->Serialize(factory, packetType);

		ScriptedActivityComponent::Serialize(factory, packetType);

		ENABLE_FLAG_ON_CONSTRUCTION(_isDirtyFlag);
		factory->Write(_isDirtyFlag);
		if (_isDirtyFlag) {
			factory->Write<std::uint32_t>(qbState);
			factory->Write(qbSuccess);
			factory->Write(qbEnabled);
			factory->Write<std::float_t>(timeSinceStartOfBuild);
			factory->Write<std::float_t>(timeOfPausedRebuilds);
			if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
				factory->Write(false);
				factory->Write(rebuild_activators.x);
				factory->Write(rebuild_activators.y);
				factory->Write(rebuild_activators.z);
				factory->Write(false);
			}
		}

		_isDirtyFlag = false;
		
	}

	void OnRequestUse(Entity::GameObject* sender, GM::RequestUse* msg) {
		if (qbState == 0) {
			AddPlayerToActivity(sender->GetObjectID());
			{GM::RebuildNotifyState msg; msg.player = sender->GetObjectID(); msg.iPrevState = qbState; msg.iState = (qbState = 5); GameMessages::Broadcast(sender->GetZoneInstance(), this->owner, msg); }
		}

		this->_isDirtyFlag = true;
		this->owner->SetDirty();
	}

};

#endif