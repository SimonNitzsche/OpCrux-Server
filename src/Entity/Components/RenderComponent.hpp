#ifndef __REPLICA__COMPONENTS__RENDER_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__RENDER_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"


using namespace DataTypes;

class RenderComponent : public IEntityComponent {
private:
	
public:

	RenderComponent() : IEntityComponent() {}

	static constexpr int GetTypeID() { return 2; }

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		/* TODO: Render Component Serialization */
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
			factory->Write<std::uint32_t>(0);
			
		}
	}

};

#endif