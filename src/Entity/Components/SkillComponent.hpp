#ifndef __REPLICA__COMPONENTS__SKILL_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__SKILL_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"


using namespace DataTypes;

class SkillComponent : public IEntityComponent {
private:
	bool _isDirtyFlag = false;


public:

	SkillComponent() : IEntityComponent() {}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		/* TODO: Skill Component Serialization */
		factory->Write(_isDirtyFlag);
	}

};

#endif