#ifndef __REPLICA__COMPONENTS__VEHICLE_PHYSICS_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__VEHICLE_PHYSICS_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

using namespace DataTypes;

class VehiclePhysicsComponent : public IEntityComponent {
private:
	bool _isDirtyPositionAndStuff = true;


	Vector3 position{ 0, 0, 0 };
	Quaternion rotation;
	bool isPlayerOnGround = true;
	bool onRail = false;
	Vector3 velocity = Vector3::zero();
	Vector3 angularVelocity = Vector3::zero();
public:
	

public:

	VehiclePhysicsComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 30; }

	void SetPosition(DataTypes::Vector3 pos) {
		position = pos;
		_isDirtyPositionAndStuff = true;
		owner->SetDirty();
	}

	DataTypes::Vector3 GetPosition() {
		return position;
	}

	void SetRotation(DataTypes::Quaternion rot) {
		rotation = rot;
		_isDirtyPositionAndStuff = true;
		owner->SetDirty();
	}

	DataTypes::Quaternion GetRotation() {
		return rotation;
	}

	void SetVelocity(DataTypes::Vector3 vel) {
		velocity = vel;
		_isDirtyPositionAndStuff = true;
		owner->SetDirty();
	}

	DataTypes::Vector3 GetVelocity() {
		return velocity;
	}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		// Physic specific
		ENABLE_FLAG_ON_CONSTRUCTION(_isDirtyPositionAndStuff);
		factory->Write(_isDirtyPositionAndStuff);
		if (_isDirtyPositionAndStuff) {
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

		// Racing specific
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
			factory->Write<std::uint8_t>(0);
			factory->Write(false);
		}
		factory->Write(false);
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
};

#endif