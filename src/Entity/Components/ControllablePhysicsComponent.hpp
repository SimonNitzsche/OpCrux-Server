#ifndef __REPLICA__COMPONENTS__CONTROLLABLE_PHYSICS_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__CONTROLLABLE_PHYSICS_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "DataTypes/Quaternion.hpp"

using namespace DataTypes;

class ControllablePhysicsComponent : public IEntityComponent {
private:
	Vector3 position = Vector3::zero();
	Quaternion rotation;
	bool isPlayerOnGround=false;
	Vector3 velocity = Vector3::zero();
	Vector3 angularVelocity = Vector3::zero();
public:

	ControllablePhysicsComponent() : IEntityComponent() {}

	void SetPosition(Vector3 pos) {
		position = pos;
	}

	void SetRotation(Quaternion rot) {
		rotation = rot;
	}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		/* TODO: Controllable Physics Component Serialization */
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
			factory->Write(false);
			factory->Write(false);
		}
		factory->Write(false);
		factory->Write(false);
		factory->Write(false);
		
		factory->Write(true);
		{
			factory->Write(position);
			factory->Write(rotation);
			factory->Write(isPlayerOnGround);
			factory->Write(false);
			factory->Write(false);
			factory->Write(false);
			factory->Write(false);
		}
		
		if (packetType == ReplicaTypes::PacketTypes::SERIALIZATION) {
			factory->Write(false);
		}
	}

};

#endif