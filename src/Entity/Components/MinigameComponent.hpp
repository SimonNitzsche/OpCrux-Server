#ifndef __REPLICA__COMPONENTS__MINIGAME_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__MINIGAME_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

using namespace DataTypes;

class MinigameComponent : public IEntityComponent {
private:

public:

	MinigameComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 50; }

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		factory->Write<std::uint32_t>(0x88);
	}
};

#endif