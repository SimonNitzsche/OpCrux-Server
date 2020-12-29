#ifndef __REPLICA__COMPONENTS__CONTROLLABLE_PHYSICS_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__CONTROLLABLE_PHYSICS_COMPONENT_HPP__

#include "Common/CrossPlatform.hpp"

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "DataTypes/Quaternion.hpp"

#include "bullet3-2.89/src/btBulletDynamicsCommon.h"

#include "FileTypes/HKXFile/HKXCacheManager.hpp"
#include "FileTypes/HKXFile/hkxFile.hpp"
//#include "FileTypes/HKXFile/Classes/hkRootLevelContainer.hpp"

#include "GameCache/PhysicsComponent.hpp"
#include "Entity/GameObject.hpp"
#include "Server/WorldServer.hpp"

using namespace DataTypes;

class ControllablePhysicsComponent : public IEntityComponent {
private:
	bool _isDirtyPositionAndStuff = true;

	btRigidBody * rigidBody = nullptr;
	btCollisionShape* collisionShape = nullptr;
	HKX::HKXFile* physicsAsset = nullptr;

	Vector3 position { 0, 0, 0 };
	Quaternion rotation;
	bool isPlayerOnGround=true;
	bool onRail = false;
	Vector3 velocity = Vector3::zero();
	Vector3 angularVelocity = Vector3::zero();
	/*
		set this bit to deactivate position updates
	*/
	bool puDeactivated = false;

	bool __unk__0=false;
		float __unk__1=0;
		float __unk__2=0;
	bool __unk__3=false;
		float __unk__4=0;
		bool __unk__5=false;
	bool __unk__6=false;
		bool __unk__7=false;
			float __unk__8=0;
			bool __unk__9=false;
public:

	ControllablePhysicsComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	btRigidBody* GetRigidBody() {
		return rigidBody;
	}

	void OnEnable() {
		//collisionShape = new btBoxShape(btVector3(5, 5, 5));
		if(owner->GetLOT() != 1) {
			std::string cPhysicsAsset = CachePhysicsComponent::GetPhysicsAsset(GetComponentID());
			if (cPhysicsAsset.size() != 0) {
				std::string physResPath = "res/physics/" + cPhysicsAsset;
				this->physicsAsset = HKXCacheManager::GetHKXFile(physResPath);
			}

			collisionShape = new btSphereShape(5);
			btTransform transform;
			transform.setOrigin(btVector3(3, 2, 5));
			transform.setIdentity();
			btScalar mass(0.0f);
			btVector3 inertia(0, 0, 0);	//inertia is 0,0,0 for static object, else
			if (mass != 0.0)
				collisionShape->calculateLocalInertia(mass, inertia);	//it can be determined by this function (for all kind of shapes)
			btDefaultMotionState* motionState = new btDefaultMotionState(transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, collisionShape, btVector3(0, 0, 0));
			rigidBody = new btRigidBody(rbInfo);


			
		}
		SetPosition(position);
		SetRotation(rotation);
	}

	void Awake() {
		/*if(rigidBody != nullptr)
			owner->GetZoneInstance()->dynamicsWorld->addRigidBody(rigidBody);*/
	}

	~ControllablePhysicsComponent() {
		delete rigidBody;
		delete collisionShape;
	}

	static constexpr int GetTypeID() { return 1; }

	void SetPosition(Vector3 pos) {
		position = pos;
		if (!puDeactivated) {
			_isDirtyPositionAndStuff = true;
			owner->SetDirty();
			UpdatePhysicsPosition();
		}
	}

	void UpdatePhysicsPosition() {
		if (rigidBody == nullptr) return;
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(position.getBt());
		transform.setRotation(rotation.getBt());
		rigidBody->setWorldTransform(transform);
		rigidBody->getMotionState()->setWorldTransform(transform);
		//owner->GetZoneInstance()->dynamicsWorld->updateSingleAabb(rigidBody);
	}

	Vector3 GetPosition() {
		return position;
	}

	void SetRotation(Quaternion rot) {
		rotation = rot;
		_isDirtyPositionAndStuff = true;
		owner->SetDirty();
		UpdatePhysicsPosition();
	}

	Quaternion GetRotation() {
		return rotation;
	}

	void SetVelocity(Vector3 vel) {
		velocity = vel;
		_isDirtyPositionAndStuff = true;
		owner->SetDirty();
	}

	Vector3 GetVelocity() {
		return velocity;
	}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		/* TODO: Controllable Physics Component Serialization */
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
			// Jetpack
			factory->Write(false);
			// Immunities
			factory->Write(false);

			_isDirtyPositionAndStuff = true;
		}
		// Gravity / Runspeed
		factory->Write(false);
		// ???
		factory->Write(false);
		// ???, plays fx bubble
		factory->Write(false);
		
		factory->Write(_isDirtyPositionAndStuff);
		if(_isDirtyPositionAndStuff) {
			//_isDirtyPositionAndStuff = false;
			factory->Write<std::float_t>(position.x);
			factory->Write<std::float_t>(position.y);
			factory->Write<std::float_t>(position.z);
			factory->Write<std::float_t>(rotation.x);
			factory->Write<std::float_t>(rotation.y);
			factory->Write<std::float_t>(rotation.z);
			factory->Write<std::float_t>(rotation.w);
			factory->Write<bool>(isPlayerOnGround);
			factory->Write<bool>(onRail);
			factory->Write(true); // Velocity 
				factory->Write<std::float_t>(velocity.x);
				factory->Write<std::float_t>(velocity.y);
				factory->Write<std::float_t>(velocity.z);
			factory->Write(true); // Angular Velocity
				factory->Write<std::float_t>(angularVelocity.x);
				factory->Write<std::float_t>(angularVelocity.y);
				factory->Write<std::float_t>(angularVelocity.z);
			factory->Write(false);
		}
		
		if (packetType == ReplicaTypes::PacketTypes::SERIALIZATION) {
			factory->Write(false);
		}
	}

	/*
		Deserialize the packet (POSITION_UPDATE), so only serialization!
	*/
	void Deserialize(RakNet::BitStream * packet) {
		packet->Read(position);
		packet->Read(rotation);
		packet->Read(isPlayerOnGround);
		packet->Read(onRail);
		bool velocityFlag; packet->Read(velocityFlag);
		if (velocityFlag) {
			packet->Read(velocity);
		}
		else {
			velocity = DataTypes::Vector3();
		}
		bool angularVelocityFlag; packet->Read(angularVelocityFlag);
		if (angularVelocityFlag) {
			packet->Read(angularVelocity);
		}
		else {
			angularVelocity = DataTypes::Vector3();
		}

		_isDirtyPositionAndStuff = true;
		owner->SetDirty();
	}


	void Update() {
		const auto updateFactor = 0.33f;
		if(velocity != DataTypes::Vector3::zero())
			position += velocity * updateFactor;
	}

	void LookAt(DataTypes::Vector3 lookAtPos) {
		Vector3 toVector = (lookAtPos - position).normalized();

		// compute rotation axis
		Vector3 rotAxis = Vector3::Cross(Vector3::forward(), toVector).normalized();
		if ((rotAxis.x * rotAxis.x + rotAxis.y * rotAxis.y + rotAxis.z * rotAxis.z) == 0)
			rotAxis = Vector3::up();

		// find the angle around rotation axis
		std::float_t dot = Vector3::Dot(Vector3::forward(), toVector);
		std::float_t ang = std::acosf(dot);

		auto s = std::sinf(ang * 0.5f);
		auto u = rotAxis.normalized();
		SetRotation(DataTypes::Quaternion(u.x * s, u.y * s, u.z * s, std::cosf(ang * 0.5)));
	}

	/*
		Use this to move an object
	*/
	std::float_t MoveTowardsLocation(DataTypes::Vector3 destination, std::float_t speed, std::float_t velocityRate) {
		GENERIC_POSITION pos = position;

		Vector3 difference = Vector3(pos.x - destination.x, pos.y - destination.y, pos.z - destination.z);

		float distanceRemaining = Vector3::Distance(pos, destination);

		float xSpeed = ((difference.x > 0.5) ? 1.0f : 0.0f) *speed;
		float zSpeed = ((difference.z > 0.5) ? 1.0f : 0.0f) * speed;

		pos.x += xSpeed / (1 / velocityRate);
		pos.z += zSpeed / (1 / velocityRate);
		SetPosition(pos);

		GENERIC_VELOCITY velocity = velocity;

		velocity.x = xSpeed;
		velocity.z = zSpeed;
		SetVelocity(velocity);

		return distanceRemaining;
	}

	/*
		Use this to rotate an object
	*/
	std::float_t TurnTowardsLocation(DataTypes::Vector3 targetLocation, std::float_t turnSpeed, std::float_t velocityRate) {

		targetLocation.y = position.y;

		GENERIC_POSITION objLocal = position;

		Quaternion currentQuaternion = rotation;
		Quaternion quaternionGoal = Quaternion::betweenPoints(objLocal, targetLocation);
		auto eu = Quaternion::toEuler(quaternionGoal);
		quaternionGoal = Quaternion::toQuaternion(Vector3(0, eu.x, 0));

		float currentRadians = currentQuaternion.yaw();
		float goalRadians = quaternionGoal.yaw();
		float radianDifference = abs(currentRadians - goalRadians);
		if (!radianDifference) {
			Quaternion::slerp(currentQuaternion, quaternionGoal, 1.0f);
			rotation = (currentQuaternion);

			angularVelocity = (Vector3());
			return 1.0f;
		}

		int direction;
		if (radianDifference < M_PI) direction = (currentRadians < goalRadians) ? 1 : -1;
		else {
			if (currentRadians > goalRadians) {
				radianDifference = goalRadians + 2 * M_PI - currentRadians;
				direction = 1;
			}
			else {
				radianDifference = currentRadians + 2 * M_PI - goalRadians;
				direction = -1;
			}
		}

		float turnProgress = turnSpeed / radianDifference;
		if (turnProgress > 1.0f) turnProgress = 1.0f;

		Quaternion::slerp(currentQuaternion, quaternionGoal, turnProgress);
		rotation = (currentQuaternion);

		GENERIC_VELOCITY_ANGULAR qu_velocity = angularVelocity;
		if (turnProgress < 1.0f) qu_velocity.y = turnSpeed / (velocityRate * 2) * direction;
		else qu_velocity.y = 0.0f;
		angularVelocity = (qu_velocity);

		return turnProgress;
	}
};

#endif