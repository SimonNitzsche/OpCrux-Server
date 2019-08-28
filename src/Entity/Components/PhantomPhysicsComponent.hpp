#ifndef __REPLICA__COMPONENTS__PHANTOM_PHYSICS_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__PHANTOM_PHYSICS_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "DataTypes/Vector3.hpp"
#include "DataTypes/Quaternion.hpp"

using namespace DataTypes;

class PhantomPhysicsComponent : public IEntityComponent {
private:
	DataTypes::Vector3 position;
	DataTypes::Quaternion rotation;
public:

	PhantomPhysicsComponent() : IEntityComponent() {}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		/* TODO: PhantomPhysics Component Serialization */
		
		// Position, Rotation
		factory->Write(false);

		// Physic Effects
		factory->Write(false);
	}

};

#endif