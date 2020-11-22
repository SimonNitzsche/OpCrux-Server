#ifndef __REPLICA__COMPONENTS__PHANTOM_PHYSICS_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__PHANTOM_PHYSICS_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "DataTypes/Vector3.hpp"
#include "DataTypes/Quaternion.hpp"

#include "Entity/Components/SimplePhysicsComponent.hpp"
#include "Entity/Components/ControllablePhysicsComponent.hpp"

#include "bullet3-2.89/src/btBulletDynamicsCommon.h"
#include "bullet3-2.89/src/btBulletCollisionCommon.h"
#include "bullet3-2.89/src/BulletCollision/CollisionDispatch/btGhostObject.h"

#include "Entity/GameObject.hpp"

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

	std::vector<Entity::GameObject *> enteredObjects = {};

public:

	PhantomPhysicsComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 40; }

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		// Enable dirty flags on creation
		/*ENABLE_FLAG_ON_CONSTRUCTION(_isDirtyFlagPosRot);
		ENABLE_FLAG_ON_CONSTRUCTION(_isDirtyFlagPhysEffectDirection);
		ENABLE_FLAG_ON_CONSTRUCTION(_isDirtyFlagEffects);*/

		// Position, Rotation
		bool tmpFlag = false;
		tmpFlag = _isDirtyFlagPosRot;
		factory->Write(tmpFlag);
		if (tmpFlag) {
			factory->Write(position.x);
			factory->Write(position.y);
			factory->Write(position.z);
			factory->Write(rotation.x);
			factory->Write(rotation.y);
			factory->Write(rotation.z);
			factory->Write(rotation.w);
			rotation.isValid();
		}

		// Physic Effects
		tmpFlag = _isDirtyFlagEffects;
		factory->Write(tmpFlag);
		if (tmpFlag) {
			factory->Write(physEffectActive);
			if (physEffectActive) {
				factory->Write<std::uint32_t>(physEffectType);
				factory->Write(physEffectAmount);
				factory->Write(physEffectUseDistance);
				if (physEffectUseDistance) {
					factory->Write(physEffectMinDistance);
					factory->Write(physEffectMaxDistance);
				}
				tmpFlag = _isDirtyFlagPhysEffectDirection;
				factory->Write(tmpFlag);
				if (tmpFlag) {
					factory->Write(physEffectDirection.x * physEffectAmount);
					factory->Write(physEffectDirection.y * physEffectAmount);
					factory->Write(physEffectDirection.z * physEffectAmount);
				}
			}
		}
	}

	inline
	void SetEffectDirty() {
		_isDirtyFlagEffects = true;
	}

	void OnCollisionPhantom(Entity::GameObject * object) {

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

	void SetEffectStatus(bool status) {
		physEffectActive = status;
		_isDirtyFlagEffects = true;
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

	void PhysicUpdate();

};

#endif