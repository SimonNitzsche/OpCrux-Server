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

	static constexpr int GetTypeID() { return 73; }

	void OnEnable() {

	}

	void OnRequestUse(Entity::GameObject * sender, GM::RequestUse * msg) {
		
		// If multi interact and type is not 0, we are not the receiver it.
		if (msg->bIsMultiInteractUse && msg->multiInteractType != 0) { Logger::log("WRLD", "Unknown multiInteractType found: " + std::to_string(msg->multiInteractType), LogType::UNEXPECTED); return; }

		GM::OfferMission missionOffer = GM::OfferMission();

		// Get offering missions

		// If multi-interact

		if (msg->bIsMultiInteractUse) {
			// check if mission is assingable

			missionOffer.missionID = msg->multiInteractID;
		}
		else {
			// pick first that meets requirements

			missionOffer.missionID = 1727;
		}
		
		missionOffer.offerer = owner->GetObjectID();

		GameMessages::Send(owner->GetZoneInstance(), UNASSIGNED_SYSTEM_ADDRESS, sender->GetObjectID(), missionOffer);

	}

};

#endif