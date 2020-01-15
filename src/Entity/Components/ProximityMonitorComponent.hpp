#ifndef __REPLICA__COMPONENTS__PROXIMITY_MONITOR_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__PROXIMITY_MONITOR_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameObject.hpp"

#include "GameCache/ComponentsRegistry.hpp"

using namespace DataTypes;

class ProximityMonitorComponent : public IEntityComponent {
private:



public:

	ProximityMonitorComponent(std::int32_t componentID) : IEntityComponent(componentID) {}


	static constexpr int GetTypeID() { return 78; }

};

#endif