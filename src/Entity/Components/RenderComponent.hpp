#ifndef __REPLICA__COMPONENTS__RENDER_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__RENDER_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

using namespace DataTypes;

class RenderComponent : public IEntityComponent {
private:
	
public:

	bool renderDisabled = false;

	RenderComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 2; }

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		if (renderDisabled) return;

		/* TODO: Render Component Serialization */
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
			factory->Write<std::uint32_t>(0);
			
		}
	}

	void PopulateFromLDF(LDFCollection* collection) {
		LDF_GET_VAL_FROM_COLLECTION(renderDisabled, collection, u"renderDisabled", false);
		
		std::uint32_t lot = owner->GetLOT();
		renderDisabled |= (lot == 5937 || lot == 5938 || lot == 9741 || lot == 9742 || lot == 9862 || lot == 9863 /*|| lot == 6368*/);
	}

};

#endif