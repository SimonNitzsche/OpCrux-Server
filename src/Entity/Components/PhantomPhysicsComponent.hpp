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
	bool physEffectActive = false;
	std::uint32_t physEffectType=0;
	std::float_t physEffectAmount=0;
	bool physEffectUseDistance = false;
	std::float_t physEffectMinDistance=0;
	std::float_t physEffectMaxDistance=0;
private:
	bool _isDirtyFlagPhysEffectDirection = false;
	DataTypes::Vector3 physEffectDirection;
public:

	PhantomPhysicsComponent() : IEntityComponent() {}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
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
			factory->Write(physEffectActive);
			if (physEffectActive) {
				factory->Write(physEffectType);
				factory->Write(physEffectAmount);
				factory->Write(physEffectUseDistance);
				if (physEffectUseDistance) {
					factory->Write(physEffectMinDistance);
					factory->Write(physEffectMaxDistance);
				}
				if (_isDirtyFlagPhysEffectDirection) {
					factory->Write(physEffectDirection);
				}
			}
		}
	}

	inline
	void SetEffectDirty() {
		_isDirtyFlagEffects = true;
	}

	void SetEffectDirection(DataTypes::Vector3 direction) {
		physEffectDirection = direction;
		_isDirtyFlagPhysEffectDirection = true;
		owner->SetDirty();
		SetEffectDirty();
	}

	void SetPosition(DataTypes::Vector3 pos) {
		position = pos;
		_isDirtyFlagPosRot = true;
		owner->SetDirty();
	}

	DataTypes::Vector3 GetPosition() {
		return position;
	}

	void SetRotation(DataTypes::Quaternion rot) {
		rotation = rot;
		_isDirtyFlagPosRot = true;
		owner->SetDirty();
	}

	DataTypes::Quaternion GetRotation() {
		return rotation;
	}

};

#endif