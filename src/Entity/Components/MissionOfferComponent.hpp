#ifndef __REPLICA__COMPONENTS__MISSION_OFFER_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__MISSION_OFFER_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameObject.hpp"

#include "GameCache/ComponentsRegistry.hpp"
#include "GameCache/MovementAIComponent.hpp"
#include "Utils/ServerInfo.hpp"

#include "Entity/GameMessages.hpp"

using namespace DataTypes;

class MissionOfferComponent : public IEntityComponent {
private:



public:

	MissionOfferComponent() : IEntityComponent() {}

	void OnEnable() {

	}

	void OnRequestUse(Entity::GameObject * sender, GM::RequestUse * msg) {
		
		GM::OfferMission missionOffer = GM::OfferMission();
		missionOffer.missionID = 1727;
		missionOffer.offerer = owner->GetObjectID();

		GameMessages::Send(owner->GetZoneInstance(), UNASSIGNED_SYSTEM_ADDRESS, sender->GetObjectID(), missionOffer);

	}

};

#endif