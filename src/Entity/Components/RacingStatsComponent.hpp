#ifndef __REPLICA__COMPONENTS__RACING_STATS_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__RACING_STATS_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameObject.hpp"

#include "GameCache/ComponentsRegistry.hpp"

using namespace DataTypes;

class RacingStatsComponent : public IEntityComponent {
private:


public:

	RacingStatsComponent(std::int32_t componentID) : IEntityComponent(componentID) {}


	static constexpr int GetTypeID() { return 74; }

};

#endif