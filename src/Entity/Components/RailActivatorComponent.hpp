#ifndef __REPLICA__COMPONENTS__RAIL_ACTIVATOR_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__RAIL_ACTIVATOR_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameObject.hpp"

#include "GameCache/ComponentsRegistry.hpp"

using namespace DataTypes;

class RailActivatorComponent : public IEntityComponent {
private:



public:

	RailActivatorComponent(std::int32_t componentID) : IEntityComponent(componentID) {}


	static constexpr int GetTypeID() { return 104; }

};

#endif