#ifndef __REPLICA__COMPONENTS__GENERIC_ACTIVATOR_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__GENERIC_ACTIVATOR_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

class GenericActivatorComponent : public IEntityComponent {
public:

	GenericActivatorComponent(std::int32_t componentID) : IEntityComponent(componentID) {}


	static constexpr std::int16_t GetTypeID() { return 64; }

};

#endif