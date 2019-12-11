#ifndef __REPLICA__COMPONENTS__MINIFIG_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__MINIFIG_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameObject.hpp"

#include "GameCache/ComponentsRegistry.hpp"
#include "GameCache/MovementAIComponent.hpp"
#include "Utils/ServerInfo.hpp"

using namespace DataTypes;

class MinifigComponent : public IEntityComponent {
private:



public:

	MinifigComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 35; }

};

#endif