#ifndef __REPLICA__COMPONENTS__SKILL_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__SKILL_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"


class AbstractAggregateBehavior {

};

using namespace DataTypes;

class SkillComponent : public IEntityComponent {
private:
	bool _isDirtyFlag = false;


public:

	SkillComponent() : IEntityComponent() {}

	static constexpr int GetTypeID() { return 9; }

	void Awake() {
		
	}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		/* TODO: Skill Component Serialization */
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
			factory->Write(_isDirtyFlag);
		}
	}

	void OnStartSkill(const GM::StartSkill & msg) {

	}

	void OnSyncSkill(const GM::SyncSkill & msg) {

	}

};

#endif