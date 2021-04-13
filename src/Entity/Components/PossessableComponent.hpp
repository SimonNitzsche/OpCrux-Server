#ifndef __REPLICA__COMPONENTS__POSSESSABLE_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__POSSESSABLE_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

class PossessableComponent : public IEntityComponent {
public:

	PossessableComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	DataTypes::LWOOBJID driver;

	static constexpr int GetTypeID() { return 108; }

	void Serialize(RakNet::BitStream* factory, ReplicaTypes::PacketTypes packetType) {
		factory->Write(driver != std::uint64_t(0));
		if (driver != std::uint64_t(0)) {
			factory->Write(driver != std::uint64_t(0));
			factory->Write(driver);
			factory->Write(false);
			factory->Write(false);
		}
	}

};

#endif