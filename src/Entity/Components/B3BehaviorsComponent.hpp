#ifndef __REPLICA__COMPONENTS__B3_BEHAVIORS_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__B3_BEHAVIORS_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

class B3BehaviorsComponent : public IEntityComponent {
public:

	B3BehaviorsComponent(std::int32_t componentID) : IEntityComponent(componentID) {}


	static constexpr int GetTypeID() { return 42; }

};

#endif