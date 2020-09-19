#ifndef __REPLICA__COMPONENTS__REBUILD_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__REBUILD_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

class RebuildComponent : public IEntityComponent {
public:

	RebuildComponent(std::int32_t componentID) : IEntityComponent(componentID) {}


	static constexpr int GetTypeID() { return 12; }

};

#endif