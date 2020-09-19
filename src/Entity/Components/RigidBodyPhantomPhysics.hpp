#ifndef __REPLICA__COMPONENTS__RIGIDBODY_PHANTOM_PHYSICS_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__RIGIDBODY_PHANTOM_PHYSICS_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "DataTypes/Vector3.hpp"
#include "DataTypes/Quaternion.hpp"

using namespace DataTypes;

class RigidBodyPhantomPhysicsComponent : public IEntityComponent {
private:

	bool _posRotDirty = true;
	DataTypes::Vector3 position = DataTypes::Vector3::zero();
	DataTypes::Quaternion rotation = DataTypes::Quaternion();


public:

	RigidBodyPhantomPhysicsComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 20; }

	void Serialize(RakNet::BitStream* factory, ReplicaTypes::PacketTypes packetType) {
		bool tmpFlag = false;

		tmpFlag = _posRotDirty || packetType == ReplicaTypes::PacketTypes::CONSTRUCTION;
		factory->Write(tmpFlag);
		if (tmpFlag) {
			factory->Write<std::float_t>(position.x);
			factory->Write<std::float_t>(position.y);
			factory->Write<std::float_t>(position.z);
			factory->Write<std::float_t>(rotation.x);
			factory->Write<std::float_t>(rotation.y);
			factory->Write<std::float_t>(rotation.z);
			factory->Write<std::float_t>(rotation.w);
			rotation.isValid();
		}
	}

	void SetPosition(DataTypes::Vector3 pos) {
		position = pos;
		_posRotDirty = true;
		owner->SetDirty();
	}

	DataTypes::Vector3 GetPosition() {
		return position;
	}

	void SetRotation(DataTypes::Quaternion rot) {
		rotation = rot;
		_posRotDirty = true;
		owner->SetDirty();
		rotation.isValid();
	}

	DataTypes::Quaternion GetRotation() {
		return rotation;
	}

};

#endif