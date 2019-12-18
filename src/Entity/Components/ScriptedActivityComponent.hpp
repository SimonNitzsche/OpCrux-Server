#ifndef __REPLICA__COMPONENTS__SCRIPTED_ACTIVITY_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__SCRIPTED_ACTIVITY_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"


using namespace DataTypes;

class ScriptedActivityComponent : public IEntityComponent {
private:


public:

	ScriptedActivityComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 39; }

	void Serialize(RakNet::BitStream* factory, ReplicaTypes::PacketTypes packetType) {
		factory->Write(false);
	}

	void PopulateFromLDF(LDFCollection collection) {
		// TODO
		
	}

};

#endif