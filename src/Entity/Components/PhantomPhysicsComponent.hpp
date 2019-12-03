#ifndef __REPLICA__COMPONENTS__PHANTOM_PHYSICS_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__PHANTOM_PHYSICS_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "DataTypes/Vector3.hpp"
#include "DataTypes/Quaternion.hpp"

#include "Entity/Components/SimplePhysicsComponent.hpp"

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

	PhantomPhysicsComponent() : IEntityComponent() {}

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

	void PhysicUpdate() {
		/*
			Since we don't have propper physic detection yet,
			we will currently use a sphere with the radius of 1 times scale,
			since the default collision object is a 2x2x2 box, if a hkx file is missing.
		*/

		// Get own position
		Vector3 pos = Vector3::zero();
		ControllablePhysicsComponent * contPhysComp = owner->GetComponent<ControllablePhysicsComponent>();
		if (contPhysComp) {
			pos = contPhysComp->GetPosition();
		}
		else {
			SimplePhysicsComponent * simpPhysComp = owner->GetComponent<SimplePhysicsComponent>();
			if (simpPhysComp)
				pos = simpPhysComp->GetPosition();
			else
				return;
		}

		// Cleanup removed objects
		for (int i = 0; i < enteredObjects.size(); ++i) {
			if (!enteredObjects.at(i)) {
				owner->OnOffCollisionPhantom(enteredObjects.at(i));
				enteredObjects.erase(enteredObjects.begin() + i);
			}
		}

		for (auto o : this->owner->GetZoneInstance()->objectsManager->GetObjects()) {
			// we can assume, the object has a controllable physics object, otherwise it can't move.
			ControllablePhysicsComponent * objectPhysicsComponent = o->GetComponent<ControllablePhysicsComponent>();
			if (!objectPhysicsComponent || objectPhysicsComponent == nullptr) continue;
			Vector3 position = Vector3::zero();
			position = objectPhysicsComponent->GetPosition();

			float difference = Vector3::Distance(pos, position);

			// Check if exists in list.
			bool isInside = std::find(enteredObjects.begin(), enteredObjects.end(), o) != enteredObjects.end();
			if (isInside) {
				// Object in list, check if left

				// Look for matching radii
				if (difference > owner->GetScale()*2) {
					// Message object left.
					owner->OnOffCollisionPhantom(o);
					auto it2 = std::find(enteredObjects.begin(), enteredObjects.end(), o);
					if (it2 != enteredObjects.end())
						enteredObjects.erase(it2);
				}
			}
			else {
				// Object not in list, check if entered

				// Look for matching radii
				if (difference <= owner->GetScale()*2) {
					// Message object joined.
					auto it2 = std::find(enteredObjects.begin(), enteredObjects.end(), o);
					if (it2 != enteredObjects.end()) continue;
					enteredObjects.push_back(o);
					// OnCollisionPhantom
					owner->OnCollisionPhantom(o);
				}
			}
		}
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