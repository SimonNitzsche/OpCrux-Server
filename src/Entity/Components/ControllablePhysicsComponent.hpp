#ifndef __REPLICA__COMPONENTS__CONTROLLABLE_PHYSICS_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__CONTROLLABLE_PHYSICS_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "DataTypes/Quaternion.hpp"

using namespace DataTypes;

class ControllablePhysicsComponent : public IEntityComponent {
private:
	Vector3 position { -406.6414489746094, 350.69287109375, -157.47933959960938 };
	Quaternion rotation;
	bool isPlayerOnGround=true;
	bool unknownBoolThatMakesObjectFloat = false;
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

	void SetPosition(Vector3 pos) {
		position = pos;
	}

	Vector3 GetPosition() {
		return position;
	}

	void SetRotation(Quaternion rot) {
		rotation = rot;
	}

	Quaternion GetRotation() {
		return rotation;
	}

	void SetVelocity(Vector3 vel) {
		velocity = vel;
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
		
		factory->Write(true);
		{
			factory->Write(position);
			factory->Write(rotation);
			factory->Write(isPlayerOnGround);
			factory->Write(unknownBoolThatMakesObjectFloat); // this sems to be active when the angular velocity y is negative.
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
		packet->Read(unknownBoolThatMakesObjectFloat);
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

		owner->SetDirty();
	}


	void Update() {
		
	}
};

#endif