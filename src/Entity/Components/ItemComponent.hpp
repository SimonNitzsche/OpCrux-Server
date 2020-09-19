#ifndef __REPLICA__COMPONENTS__ITEM_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__ITEM_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameObject.hpp"

#include "GameCache/ComponentsRegistry.hpp"

using namespace DataTypes;

class ItemComponent : public IEntityComponent {
private:

public:

	ItemComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr std::int16_t GetTypeID() { return 11; }

	void Serialize(RakNet::BitStream* factory, ReplicaTypes::PacketTypes packetType) {
		factory->Write(false);
	}

};

#endif