#ifndef __REPLICA__COMPONENTS__SKILL_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__SKILL_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "Entity/GameMessages/StartSkill.hpp"
#include "Entity/GameMessages/SyncSkill.hpp"

#include "GameCache/BehaviorParameter.hpp"
#include "GameCache/BehaviorTemplate.hpp"
#include "GameCache/SkillBehavior.hpp"

struct AbstractAggregateBehavior {
	virtual void UnCast() {};
	static void StartUnCast(long nextBehavior, RakNet::BitStream * bs);
};

struct BehaviorAnd : AbstractAggregateBehavior {

	void UnCast(std::int32_t behaviorID, RakNet::BitStream * bs) {
		
		auto ri = CacheBehaviorParameter::GetBehaviorRow(behaviorID);
		if (!ri.isValid())
			throw new std::runtime_error("Unable to query row.");

		// Initial call
		std::int32_t nextID = *reinterpret_cast<float*>(ri[2].getMemoryLocation());
		StartUnCast(nextID, bs);

		// For all others
		while (ri.isLinkedRowInfoValid()) {
			ri = ri.getLinkedRowInfo();
			nextID = *reinterpret_cast<float*>(ri[2].getMemoryLocation());
			StartUnCast(nextID, bs);
		}
	}
};

struct BehaviorMovementSwitch : AbstractAggregateBehavior {
	
	void UnCast(std::int32_t behaviorID, RakNet::BitStream * bs) {
		std::uint32_t movementType; bs->Read(movementType);

		static const std::string switchTypes[] { "moving_action", "ground_action", "jump_action", "falling_action", "air_action", "double_jump_action", "jetpack_action" };
		if (movementType <= 6) {
			std::int32_t nextID = CacheBehaviorParameter::GetParameterValue(behaviorID, switchTypes[movementType]);
			StartUnCast(nextID, bs);
		}
	}
};


void AbstractAggregateBehavior::StartUnCast(long nextBehavior, RakNet::BitStream * bs) {
	if (nextBehavior == -1) return;
	long templateID = CacheBehaviorTemplate::GetTemplateID(nextBehavior);
	switch (templateID) {
	case 3: {
		// And
		BehaviorAnd bAnd = BehaviorAnd();
		bAnd.UnCast(nextBehavior, bs);
	}
	case 6: {
		// Movement Switch
		BehaviorMovementSwitch movementSwitch = BehaviorMovementSwitch();
		movementSwitch.UnCast(nextBehavior, bs);
	}
	default:
		Logger::log("WRLD", "TODO: Implement behavior template id #" + std::to_string(templateID), LogType::UNEXPECTED);
		break;
	}
}

using namespace DataTypes;

class SkillComponent : public IEntityComponent {
private:
	bool _isDirtyFlag = false;

	std::uint32_t currentHandle = 0;
	std::uint32_t currentSkill = 0;

	void UnCast(const std::string sBitStream) {
		RakNet::BitStream bs = RakNet::BitStream(reinterpret_cast<unsigned char*>(const_cast<char*>(sBitStream.c_str())), sBitStream.size(), false);
		unsigned long behaviorID = CacheSkillBehavior::GetBehaviorID(currentSkill);
		AbstractAggregateBehavior::StartUnCast(behaviorID, &bs);
	}

public:

	SkillComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 9; }

	void Awake() {
		
	}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		/* TODO: Skill Component Serialization */
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
			factory->Write(false);
		}
	}

	inline void OnStartSkill(GM::StartSkill msg) {
		currentSkill = msg.skillID;
		currentHandle = msg.uiSkillHandle;
		UnCast(msg.sBitStream);
	}

	inline void OnSyncSkill(GM::SyncSkill msg) {

	}

};

#endif