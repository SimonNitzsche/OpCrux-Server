#ifndef __REPLICA__COMPONENTS__PROPERTY_VENDOR_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__PROPERTY_VENDOR_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameObject.hpp"

#include "GameCache/ComponentsRegistry.hpp"

using namespace DataTypes;

class PropertyVendorComponent : public IEntityComponent {
private:



public:

	PropertyVendorComponent(std::int32_t componentID) : IEntityComponent(componentID) {}


	static constexpr int GetTypeID() { return 65; }

};

#endif