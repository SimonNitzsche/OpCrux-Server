#ifndef __REPLICA__COMPONENTS__SCRIPT_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__SCRIPT_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"


using namespace DataTypes;

class ScriptComponent : public IEntityComponent {
private:
	bool _isDirtyFlag = false;


public:

	ScriptComponent() : IEntityComponent() {}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		/* TODO: Script Component Serialization */
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
			factory->Write(_isDirtyFlag);
		}
	}

};

#endif