#ifndef __REPLICA__COMPONENTS__VENDOR_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__VENDOR_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "Entity/GameMessages/VendorOpenWindow.hpp"

using namespace DataTypes;

class VendorComponent : public IEntityComponent {
private:
	bool _isDirtyFlag = true;

	bool unknownBitA = true;
	bool unknownBitB = false;


public:

	VendorComponent() : IEntityComponent() {}

	static constexpr int GetTypeID() { return 16; }

	void OnEnable() {

	}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		ENABLE_FLAG_ON_CONSTRUCTION(_isDirtyFlag);
		factory->Write(_isDirtyFlag);
		if (_isDirtyFlag) {
			_isDirtyFlag = false;
			factory->Write(unknownBitA);
			factory->Write(unknownBitB);
		}
	}

	void OnRequestUse(Entity::GameObject * sender, GM::RequestUse * msg) {
		CharacterComponent * charComp = sender->GetComponent<CharacterComponent>();
		if (charComp != nullptr) {
			GameMessages::Send(owner->GetZoneInstance(), charComp->clientAddress, sender->GetObjectID(), GM::VendorOpenWindow());
			
			
		}
	}

};

#endif