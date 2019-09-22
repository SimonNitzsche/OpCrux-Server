#ifndef __REPLICA__COMPONENTS__INVENTORY_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__INVENTORY_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

using namespace DataTypes;

class InventoryComponent : public IEntityComponent {
private:
	bool _isDirtyFlag = false;
	

public:

	InventoryComponent() : IEntityComponent() {}

	static constexpr int GetTypeID() { return 17; }

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		/* TODO: Inventory Component Serialization */
		factory->Write(_isDirtyFlag);
		factory->Write(_isDirtyFlag);
	}

};

#endif