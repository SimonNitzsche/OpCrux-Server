#ifndef __REPLICA__COMPONENTS__ROCKET_LAUNCH_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__ROCKET_LAUNCH_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameObject.hpp"

#include "GameCache/ComponentsRegistry.hpp"

using namespace DataTypes;

class RocketLaunchComponent : public IEntityComponent {
private:



public:

	RocketLaunchComponent(std::int32_t componentID) : IEntityComponent(componentID) {}


	static constexpr std::int16_t GetTypeID() { return 67; }

};

#endif