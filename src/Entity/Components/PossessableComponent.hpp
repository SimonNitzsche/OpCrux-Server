#ifndef __REPLICA__COMPONENTS__POSSESSABLE_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__POSSESSABLE_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

class PossessableComponent : public IEntityComponent {
public:

	PossessableComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	DataTypes::LWOOBJID driver;

	static constexpr int GetTypeID() { return 108; }

	void Serialize(RakNet::BitStream* factory, ReplicaTypes::PacketTypes packetType) {
		factory->Write(driver != 0ULL);
		if (driver != 0ULL) {
			factory->Write(driver != 0ULL);
			factory->Write(driver);
			factory->Write(false);
			factory->Write(false);
		}
	}

};

#endif