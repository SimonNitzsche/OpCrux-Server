#ifndef __REPLICA__COMPONENTS__SIMPLE_PHYSICS_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__SIMPLE_PHYSICS_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "DataTypes/Vector3.hpp"
#include "DataTypes/Quaternion.hpp"

using namespace DataTypes;

class SimplePhysicsComponent : public IEntityComponent {
private:
	bool allowGlitchUp = false;
	std::int32_t unknownCreation32=0;
	
	bool _velocityDirty = false;
	DataTypes::Vector3 linearVelocity = DataTypes::Vector3::zero();
	DataTypes::Vector3 angularVelocity = DataTypes::Vector3::zero();

	bool _airSpeedDirty = true;
	std::uint32_t airSpeed = 0;

	bool _posRotDirty = true;
	DataTypes::Vector3 position = DataTypes::Vector3::zero();
	DataTypes::Quaternion rotation = DataTypes::Quaternion();


public:

	SimplePhysicsComponent() : IEntityComponent() {}

	static constexpr int GetTypeID() { return 3; }

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		bool tmpFlag = false;

		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
			factory->Write(allowGlitchUp);
			factory->Write<std::int32_t>(unknownCreation32);
		}

		if (packetType != ReplicaTypes::PacketTypes::CONSTRUCTION) {
			factory->Write(false);
			factory->Write(false);
			factory->Write(false);
			return;
		}

		tmpFlag = _velocityDirty;
		factory->Write(tmpFlag);
		if (tmpFlag) {
			factory->Write(linearVelocity);
			factory->Write(angularVelocity);
		}
		tmpFlag = _airSpeedDirty;
		factory->Write(tmpFlag);
		if (tmpFlag) {
			factory->Write(airSpeed);
		}
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