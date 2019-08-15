#ifndef __REPLICA__COMPONENTS__VENDOR_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__VENDOR_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"


using namespace DataTypes;

class VendorComponent : public IEntityComponent {
private:
	bool _isDirtyFlag = false;


public:

	VendorComponent() : IEntityComponent() {}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		/* TODO: Vendor Component Serialization */
		factory->Write(_isDirtyFlag);
	}

};

#endif