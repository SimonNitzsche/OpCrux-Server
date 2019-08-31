#ifndef __REPLICA__COMPONENTS__PHANTOM_PHYSICS_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__PHANTOM_PHYSICS_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "DataTypes/Vector3.hpp"
#include "DataTypes/Quaternion.hpp"

using namespace DataTypes;

class PhantomPhysicsComponent : public IEntityComponent {
private:
	bool _isDirtyFlagPosRot = false;
	bool _isDirtyFlagEffects = false;

	DataTypes::Vector3 position;
	DataTypes::Quaternion rotation;
public:

	PhantomPhysicsComponent() : IEntityComponent() {}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		/* TODO: PhantomPhysics Component Serialization */
		
		// Position, Rotation
		factory->Write(_isDirtyFlagPosRot);
		if (_isDirtyFlagPosRot) {
			factory->Write(position.x);
			factory->Write(position.y);
			factory->Write(position.z);
			factory->Write(rotation.x);
			factory->Write(rotation.y);
			factory->Write(rotation.z);
			factory->Write(rotation.w);
		}

		// Physic Effects
		factory->Write(_isDirtyFlagEffects);
		if (_isDirtyFlagEffects) {
			factory->Write(false);
		}
	}

	void SetPosition(DataTypes::Vector3 pos) {
		position = pos;
		_isDirtyFlagPosRot = true;
		owner->SetDirty();
	}

	void SetRotation(DataTypes::Quaternion rot) {
		rotation = rot;
		_isDirtyFlagPosRot = true;
		owner->SetDirty();
	}

};

#endif