#ifndef __REPLICA__COMPONENTS__BASE_COMBAT_AI_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__BASE_COMBAT_AI_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"


using namespace DataTypes;

class BaseCombatAIComponent : public IEntityComponent {
private:
	bool _isDirtyFlag = false;


public:

	BaseCombatAIComponent() : IEntityComponent() {}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		/* TODO: BaseCombatAIComponent Serialization */
		factory->Write(_isDirtyFlag);
	}

};

#endif