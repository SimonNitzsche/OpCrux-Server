#ifndef __REPLICA__COMPONENTS__SKILL_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__SKILL_COMPONENT_HPP__

#include <mutex>

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "Entity/GameMessages/StartSkill.hpp"
#include "Entity/GameMessages/SyncSkill.hpp"

#include "GameCache/BehaviorParameter.hpp"
#include "GameCache/BehaviorTemplate.hpp"
#include "GameCache/BehaviorTemplateName.hpp"
#include "GameCache/SkillBehavior.hpp"
#include "GameCache/ObjectSkills.hpp"


using namespace DataTypes;

struct SkillStackParameters {
	bool bUsedMouse = false;
	std::float_t fCasterLatency = 0.0f;
	std::int32_t iCastType = 0;
	DataTypes::Vector3 lastClickedPosit = DataTypes::Vector3::zero();
	DataTypes::LWOOBJID optionalOriginatorID = 0ULL;
	DataTypes::LWOOBJID optionalTargetID = 0ULL;
	DataTypes::Quaternion originatorRot = DataTypes::Quaternion();
	std::uint32_t uiBehvaiorHandle = 0;
	std::uint32_t uiSkillHandle = 0;
	DataTypes::LWOOBJID currentTarget = 0ULL;
	bool bDone = false;
};

class SkillComponent : public IEntityComponent {
private:
	bool _isDirtyFlag = false;

	SkillStackParameters parameters;

	std::uint32_t currentHandle = 0;
	std::uint32_t currentSkill = 0;

	void UnCast(const std::string sBitStream, long behaviorID = 0);
	void Cast(std::string * sBitStream, long behaviorID = 0);

	std::unordered_map<std::uint32_t /*behaviorHandle*/, std::pair<std::int32_t /*behaviorAction*/, std::uint64_t /* time, only used for casting*/>> behaviorHandles;
	std::mutex mutex_behaviorHandles;
public:
	std::uint32_t currentStackDepth = 0;

public:

	SkillStackParameters GetParameters() {
		return parameters;
	}

	void SetTarget(DataTypes::LWOOBJID target) {
		parameters.currentTarget = target;
	}

	void DoDamageOnTarget(std::int32_t damage);

	SkillComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 9; }

	void RegisterMessageHandlers() {
		REGISTER_OBJECT_MESSAGE_HANDLER(SkillComponent, GM::StartSkill, OnStartSkill);
		REGISTER_OBJECT_MESSAGE_HANDLER(SkillComponent, GM::SyncSkill, OnSyncSkill);
		REGISTER_OBJECT_MESSAGE_HANDLER(SkillComponent, GM::EquipInventory, OnEquipInventory);
		REGISTER_OBJECT_MESSAGE_HANDLER(SkillComponent, GM::UnEquipInventory, OnUnEquipInventory);
		REGISTER_OBJECT_MESSAGE_HANDLER(SkillComponent, GM::CastSkill, OnCastSkill);
	}

	void Awake() {

	}

	void Serialize(RakNet::BitStream* factory, ReplicaTypes::PacketTypes packetType) {
		/* TODO: Skill Component Serialization */
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
			factory->Write(true);
			if (true) {
				factory->Write<std::uint32_t>(0);
			}
		}
	}

	void Update() {
		/*
			When we are not a player,
			we need to look for skill delays
			by ourselves.1
		*/
		if (!owner->IsPlayer()) {
			for (auto it = this->behaviorHandles.begin(); it != this->behaviorHandles.end(); ++it) {
				if (it->second.second > ServerInfo::uptimeMs()) {
					// make a copy so we still have it
					auto copy = *it;
					
					// mutex_behaviorHandles.lock();
					// // remove ourselves from pending
					// this->behaviorHandles.erase(it);
					// mutex_behaviorHandles.unlock();


					auto behaviorID = copy.second.first;
					if (behaviorID <= 0) return;
					

					GM::EchoSyncSkill echoGM; {
						echoGM.bDone = parameters.bDone;
						Cast(&(echoGM.sBitStream), behaviorID);
						echoGM.uiBehaviorHandle = parameters.uiBehvaiorHandle;
						echoGM.uiSkillHandle = parameters.uiSkillHandle;
					}
					GameMessages::Broadcast(this->owner, echoGM, true);

					GM::SyncSkill syncSkillGM; {
						syncSkillGM.bDone = echoGM.bDone;
						syncSkillGM.sBitStream = echoGM.sBitStream;
						syncSkillGM.uiBehaviorHandle = echoGM.uiBehaviorHandle;
						syncSkillGM.uiSkillHandle = echoGM.uiSkillHandle;
					}
					owner->OnMessage(owner, syncSkillGM.GetID(), &syncSkillGM);
					
					
					// we need to break, cuz the list It is now invalid
					break;
				}
			}
		}
	}

	inline void OnStartSkill(Entity::GameObject* sender, GM::StartSkill* msg) {
		parameters = SkillStackParameters();

		currentSkill = msg->skillID;
		currentHandle = msg->uiSkillHandle;

		parameters.bUsedMouse = msg->bUsedMouse;
		parameters.fCasterLatency = msg->fCasterLatency;
		parameters.iCastType = msg->iCastType;
		parameters.lastClickedPosit = msg->lastClickedPosit;
		parameters.optionalOriginatorID = msg->optionalOriginatorID;
		parameters.optionalTargetID = msg->optionalTargetID;
		parameters.originatorRot = msg->originatorRot;
		parameters.uiSkillHandle = msg->uiSkillHandle;

		GM::EchoStartSkill echoGM; {
			echoGM.bUsedMouse = msg->bUsedMouse;
			echoGM.fCasterLatency = msg->fCasterLatency;
			echoGM.iCastType = msg->iCastType;
			echoGM.lastClickedPosit = msg->lastClickedPosit;
			echoGM.optionalOriginatorID = msg->optionalOriginatorID;
			echoGM.optionalTargetID = msg->optionalTargetID;
			echoGM.originatorRot = msg->originatorRot;
			echoGM.sBitStream = msg->sBitStream;
			echoGM.skillID = msg->skillID;
			echoGM.uiSkillHandle = msg->uiSkillHandle;
		}
		GameMessages::Broadcast(this->owner, echoGM, true);

		MissionManager::LaunchTaskEvent(EMissionTask::USE_SKILL, sender, sender->GetObjectID(), 1, currentSkill);

		UnCast(msg->sBitStream);
	}

	inline void OnSyncSkill(Entity::GameObject* sender, GM::SyncSkill* msg) {
		parameters.bDone = msg->bDone;
		parameters.uiBehvaiorHandle = msg->uiBehaviorHandle;
		parameters.uiSkillHandle = msg->uiSkillHandle;

		mutex_behaviorHandles.lock();
		auto behaviorIDIt = behaviorHandles.find(msg->uiBehaviorHandle);
		std::int32_t behaviorID = 0;
		if (behaviorIDIt != behaviorHandles.end()) {
			behaviorID = behaviorIDIt->second.first;
			behaviorHandles.erase(behaviorIDIt);
		}
		mutex_behaviorHandles.unlock();

		GM::EchoSyncSkill echoGM; {
			echoGM.bDone = msg->bDone;
			echoGM.sBitStream = msg->sBitStream;
			echoGM.uiBehaviorHandle = msg->uiBehaviorHandle;
			echoGM.uiSkillHandle = msg->uiSkillHandle;
		}
		GameMessages::Broadcast(this->owner, echoGM, true);

		if (behaviorID <= 0) return;
		UnCast(msg->sBitStream, behaviorID);
	}

	inline void AddBehaviorHandle(std::uint32_t behaviorHandle, std::int32_t behaviorAction, std::float_t delay = 0.0f) {
		std::uint64_t time = ServerInfo::uptimeMs() + std::int32_t(delay * 1000);
		mutex_behaviorHandles.lock();
		behaviorHandles.insert({ behaviorHandle, std::make_pair(behaviorAction, time) });
		mutex_behaviorHandles.unlock();
	}

	std::int32_t GetHotbarSlotFromEquipLocation(std::string equipLocation) {
		if (equipLocation == "special_r")
			return 0;
		if (equipLocation == "special_l")
			return 1;
		if (equipLocation == "clavicle")
			return 2;
		if (equipLocation == "hair" || equipLocation == "head")
			return 3;
		return 5;
	}

	inline void OnEquipInventory(Entity::GameObject* sender, GM::EquipInventory* msg) {
		// Add skills
		Entity::GameObject* item = sender->GetZoneInstance()->objectsManager->GetObjectByID(msg->itemToEquip);
		if (item == nullptr) return;

		IEntityComponent* invComp = item->GetComponentByType(11);
		if (invComp == nullptr) return;
		auto strEquipLocation = CacheItemComponent::GetEquipLocation(invComp->GetComponentID());

		auto rm = CacheObjectSkills::getRow(item->GetLOT());
		auto rf = rm.flatIt();
		for (auto r : rf) {
			//if (CacheObjectSkills::GetCastOnType(r) == 0 /*Use*/) {
			GM::AddSkill addSkillGM;
			addSkillGM.skillID = CacheObjectSkills::GetSkillID(r);
			addSkillGM.castType = CacheObjectSkills::GetCastOnType(r);
			addSkillGM.AICombatWeight = CacheObjectSkills::GetAICombatWeight(r);
			addSkillGM.slotID = GetHotbarSlotFromEquipLocation(strEquipLocation);
			GameMessages::Broadcast(sender, addSkillGM);
			//}

			// On Equip
			if (addSkillGM.castType == 1) {
				{GM::CastSkill nmsg; nmsg.skillID = addSkillGM.skillID; nmsg.iCastType = addSkillGM.castType; owner->CallMessage(nmsg); };
			}
		}
	}

	inline void OnUnEquipInventory(Entity::GameObject* sender, GM::UnEquipInventory* msg) {
		// Remove skills
	}

	void OnCastSkill(Entity::GameObject* sender, GM::CastSkill* msg) {
		GM::StartSkill startSkillGM;
		startSkillGM.skillID = msg->skillID;
		startSkillGM.iCastType = msg->iCastType;
		startSkillGM.uiSkillHandle = ++parameters.uiSkillHandle;
		startSkillGM.sBitStream = "";

		parameters = SkillStackParameters();

		currentSkill = msg->skillID;

		parameters.bUsedMouse = msg->bUsedMouse;
		parameters.fCasterLatency = msg->fCasterLatency;
		parameters.iCastType = msg->iCastType;
		parameters.lastClickedPosit = msg->lastClickedPosit;
		parameters.optionalOriginatorID = msg->optionalOriginatorID;
		parameters.optionalTargetID = msg->optionalTargetID;
		parameters.originatorRot = msg->originatorRot;
		parameters.uiSkillHandle = currentHandle;

		Cast(&startSkillGM.sBitStream);
		owner->OnMessage(owner, startSkillGM.GetID(), &startSkillGM);
	}

};

#include "Entity/Components/SkillComponent/AbstractAggregateBehavior.hpp"


void SkillComponent::UnCast(const std::string sBitStream, long behaviorID) {
	currentStackDepth = 0;
	RakNet::BitStream bs = RakNet::BitStream(reinterpret_cast<unsigned char*>(const_cast<char*>(sBitStream.c_str())), sBitStream.size(), false);
	if (behaviorID <= 0) behaviorID = CacheSkillBehavior::GetBehaviorID(currentSkill);
	AbstractAggregateBehavior::StartUnCast(this, behaviorID, &bs);
}

void SkillComponent::Cast(std::string* sBitStream, long behaviorID) {
	currentStackDepth = 0;
	RakNet::BitStream bs;

	if (behaviorID <= 0) behaviorID = CacheSkillBehavior::GetBehaviorID(currentSkill);
	AbstractAggregateBehavior::StartCast(this, behaviorID, &bs);

	*sBitStream = std::string((const char*)bs.GetData(), bs.GetNumberOfBytesUsed());
}

#include "Entity/Components/DestructibleComponent.hpp"
void SkillComponent::DoDamageOnTarget(std::int32_t damage) {
	// Make sure objectID is not empty
	if (parameters.currentTarget == 0ULL) {
		Logger::log("WRLD", "Unable to do damage on empty objectID", LogType::WARN);
		return;
	}

	// Get object
	Entity::GameObject* target = this->owner->GetZoneInstance()->objectsManager->GetObjectByID(parameters.currentTarget);

	// Make sure object exists
	if (target == nullptr) {
		Logger::log("WRLD", "Unable to do damage on missing object", LogType::ERR);
		return;
	}

	// Get destructible component
	DestructibleComponent* targetDestComp = target->GetComponent<DestructibleComponent>();

	// make sure target has component
	if (targetDestComp == nullptr) {
		Logger::log("WRLD", "Unable to do damage on object without Destructible Component!", LogType::ERR);
		return;
	}

	// Do damage
	targetDestComp->PerformDamageRequest(this->owner, damage);
}

#endif