#ifndef __REPLICA__COMPONENTS__ROCKET_LANDING_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__ROCKET_LANDING_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameObject.hpp"

#include "GameCache/ComponentsRegistry.hpp"

using namespace DataTypes;

class RocketLandingComponent : public IEntityComponent {
private:



public:

	RocketLandingComponent(std::int32_t componentID) : IEntityComponent(componentID) {}


	static constexpr int GetTypeID() { return 68; }

};

#endif