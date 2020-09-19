#ifndef __REPLICA__COMPONENTS__MINIGAME_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__MINIGAME_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/Components/ScriptedActivityComponent.hpp"

using namespace DataTypes;

class MinigameComponent : public ScriptedActivityComponent {
	friend class ScriptedActivityComponent;
private:

public:

	MinigameComponent(std::int32_t componentID) : ScriptedActivityComponent(componentID) {}

	static constexpr int GetTypeID() { return 50; }

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		ScriptedActivityComponent::Serialize(factory, packetType);
	}
};

#endif