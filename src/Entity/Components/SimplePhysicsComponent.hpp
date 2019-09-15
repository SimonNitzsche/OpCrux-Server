#ifndef __REPLICA__COMPONENTS__SIMPLE_PHYSICS_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__SIMPLE_PHYSICS_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "DataTypes/Vector3.hpp"
#include "DataTypes/Quaternion.hpp"

using namespace DataTypes;

class SimplePhysicsComponent : public IEntityComponent {
private:
	bool allowGlitchUp = false;
	std::uint32_t unknownCreation32=0;
	
	bool _velocityDirty = false;
	DataTypes::Vector3 linearVelocity = DataTypes::Vector3::zero();
	DataTypes::Vector3 angularVelocity = DataTypes::Vector3::zero();

	bool _unknownU32Dirty = false;
	std::uint32_t unknownU32 = 0;

	bool _posRotDirty = true;
	DataTypes::Vector3 position = DataTypes::Vector3::zero();
	DataTypes::Quaternion rotation = DataTypes::Quaternion();


public:

	SimplePhysicsComponent() : IEntityComponent() {}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
			factory->Write(allowGlitchUp);
			factory->Write<std::uint32_t>(0);
		}
		factory->Write(_velocityDirty);
		if (_velocityDirty) {
			factory->Write(linearVelocity);
			factory->Write(angularVelocity);
		}
		factory->Write(_unknownU32Dirty);
		if (_unknownU32Dirty) {
			factory->Write(unknownU32);
		}
		factory->Write(_posRotDirty);
		if (_posRotDirty) {
			factory->Write(position);
			factory->Write(rotation);
		}
	}

	void SetPosition(DataTypes::Vector3 pos) {
		position = pos;
		_posRotDirty = true;
	}

	DataTypes::Vector3 GetPosition() {
		return position;
	}

	void SetRotation(DataTypes::Quaternion rot) {
		rotation = rot;
		_posRotDirty = true;
	}

	DataTypes::Quaternion GetRotation() {
		return rotation;
	}

};

#endif