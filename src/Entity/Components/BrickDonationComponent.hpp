#ifndef __REPLICA__COMPONENTS__BRICK_DONATION_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__BRICK_DONATION_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

class BrickDonationComponent : public IEntityComponent {
public:

	BrickDonationComponent(std::int32_t componentID) : IEntityComponent(componentID) {}


	static constexpr std::int16_t GetTypeID() { return 100; }

};

#endif