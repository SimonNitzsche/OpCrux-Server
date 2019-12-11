#ifndef __REPLICA__COMPONENTS__PLATFORM_BOUNDARY_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__PLATFORM_BOUNDARY_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameObject.hpp"

#include "GameCache/ComponentsRegistry.hpp"
#include "Utils/ServerInfo.hpp"

class PlatformBoundaryComponent : public IEntityComponent {
private:



public:

	PlatformBoundaryComponent(std::int32_t componentID) : IEntityComponent(componentID) {}


	static constexpr int GetTypeID() { return 27; }

};

#endif