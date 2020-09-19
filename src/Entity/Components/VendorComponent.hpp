#ifndef __REPLICA__COMPONENTS__VENDOR_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__VENDOR_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "Entity/GameMessages/VendorOpenWindow.hpp"

using namespace DataTypes;

class VendorComponent : public IEntityComponent {
private:
	bool _isDirtyFlag = true;

	bool isActive = true;
	bool unknownBitB = false;


public:

	VendorComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr std::int16_t GetTypeID() { return 16; }

	void OnEnable() {

	}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) _isDirtyFlag = true;
		factory->Write(_isDirtyFlag);
		if (_isDirtyFlag) {
			factory->Write(isActive);
			factory->Write(unknownBitB);
		}
		_isDirtyFlag = false;
	}

	void OnRequestUse(Entity::GameObject * sender, GM::RequestUse * msg) {
		auto * charComp = sender->GetComponent<CharacterComponent>();
		if (charComp != nullptr) {
			GameMessages::Send(owner->GetZoneInstance(), charComp->clientAddress, sender->GetObjectID(), GM::VendorOpenWindow());
			
			
		}
	}

};

#endif