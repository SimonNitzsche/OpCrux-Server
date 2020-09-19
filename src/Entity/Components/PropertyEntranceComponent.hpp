#ifndef __REPLICA__COMPONENTS__PROPERTY_ENTRANCE_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__PROPERTY_ENTRANCE_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameObject.hpp"

#include "GameCache/ComponentsRegistry.hpp"

using namespace DataTypes;

class PropertyEntranceComponent : public IEntityComponent {
private:



public:

	PropertyEntranceComponent(std::int32_t componentID) : IEntityComponent(componentID) {}


	static constexpr int GetTypeID() { return 43; }

};

#endif