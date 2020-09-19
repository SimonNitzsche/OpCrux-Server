#ifndef __REPLICA__COMPONENTS__PROPERTY_PLAQUE_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__PROPERTY_PLAQUE_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameObject.hpp"

#include "GameCache/ComponentsRegistry.hpp"

using namespace DataTypes;

class PropertyPlaqueComponent : public IEntityComponent {
private:



public:

	PropertyPlaqueComponent(std::int32_t componentID) : IEntityComponent(componentID) {}


	static constexpr std::int16_t GetTypeID() { return 113; }

};

#endif