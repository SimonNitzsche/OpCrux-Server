#ifndef __REPLICA__COMPONENTS__MISSION_OFFER_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__MISSION_OFFER_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameObject.hpp"

#include "GameCache/ComponentsRegistry.hpp"
#include "GameCache/MovementAIComponent.hpp"
#include "Utils/ServerInfo.hpp"

using namespace DataTypes;

class MissionOfferComponent : public IEntityComponent {
private:



public:

	MissionOfferComponent() : IEntityComponent() {}

};

#endif