#ifndef __REPLICA__COMPONENTS__CONTROLLABLE_PHYSICS_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__CONTROLLABLE_PHYSICS_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "DataTypes/Quaternion.hpp"

using namespace DataTypes;

class ControllablePhysicsComponent : public IEntityComponent {
private:
	bool _isDirtyPositionAndStuff = true;


	Vector3 position { 0, 0, 0 };
	Quaternion rotation;
	bool isPlayerOnGround=true;
	bool onRail = false;
	Vector3 velocity = Vector3::zero();
	Vector3 angularVelocity = Vector3::zero();

	bool __unk__0;
		float __unk__1;
		float __unk__2;
	bool __unk__3;
		float __unk__4;
		bool __unk__5;
	bool __unk__6;
		bool __unk__7;
			float __unk__8;
			bool __unk__9;
public:

	ControllablePhysicsComponent() : IEntityComponent() {}

	static constexpr int GetTypeID() { return 1; }

	void SetPosition(Vector3 pos) {
		position = pos;
		_isDirtyPositionAndStuff = true;
		owner->SetDirty();
	}

	Vector3 GetPosition() {
		return position;
	}

	void SetRotation(Quaternion rot) {
		rotation = rot;
		_isDirtyPositionAndStuff = true;
		owner->SetDirty();
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
			factory->Write(false);
			factory->Write(false);
		}
		factory->Write(false);
		factory->Write(false);
		factory->Write(false);
		
		factory->Write(_isDirtyPositionAndStuff);
		if(_isDirtyPositionAndStuff) {
			_isDirtyPositionAndStuff = false;
			factory->Write(position);
			factory->Write(rotation);
			factory->Write(isPlayerOnGround);
			factory->Write(onRail);
			factory->Write(true); // Velocity 
				factory->Write(velocity);
			factory->Write(true); // Angular Velocity
				factory->Write(angularVelocity);
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
	}


	void Update() {
		
	}
};

#endif