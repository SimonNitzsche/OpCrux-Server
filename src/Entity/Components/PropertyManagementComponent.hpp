#ifndef __REPLICA__COMPONENTS__PROPERTY_MANAGEMENT_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__PROPERTY_MANAGEMENT_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameObject.hpp"

#include "GameCache/ComponentsRegistry.hpp"

using namespace DataTypes;

class PropertyManagementComponent : public IEntityComponent {
private:



public:

	PropertyManagementComponent(std::int32_t componentID) : IEntityComponent(componentID) {}


	static constexpr int GetTypeID() { return 45; }

};

#endif