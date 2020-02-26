#ifndef __REPLICA__COMPONENTS__MISSION_OFFER_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__MISSION_OFFER_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameObject.hpp"

#include "GameCache/Missions.hpp"
#include "GameCache/MissionNPCComponent.hpp"
#include "Utils/ServerInfo.hpp"
#include "Missions/MissionRequirementParser.hpp"
#include "Database/Database.hpp"

#include "Entity/GameMessages.hpp"
#include "Entity/Components/MissionOfferComponent.hpp"

using namespace DataTypes;

class MissionOfferComponent : public IEntityComponent {
private:



public:

	MissionOfferComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 73; }

	void OnEnable() {

	}

	void OnRequestUse(Entity::GameObject * sender, GM::RequestUse * msg) {
		
		// If multi interact and type is not 0, we are not the receiver it.
		if (msg->bIsMultiInteractUse && msg->multiInteractType != 0) { Logger::log("WRLD", "Unknown multiInteractType found: " + std::to_string(msg->multiInteractType), LogType::UNEXPECTED); return; }

		GM::OfferMission missionOffer = GM::OfferMission();
		GM::OfferMission missionOfferGiver = GM::OfferMission();

		missionOffer.missionID = missionOfferGiver.missionID = -1;

		// pick first that meets requirements
		// get missions of npc
		auto missionsOffering = CacheMissionNPCComponent::getRow(GetComponentID()).flatIt();
		for (auto it = missionsOffering.begin(); it != missionsOffering.end(); ++it) {
			// Only if offering mission
			if (CacheMissionNPCComponent::GetAcceptsMission(*it)) {
				std::int32_t missionID = CacheMissionNPCComponent::GetMissionID(*it);

				// Check if it's the selected mission on multi interact
				if (msg->bIsMultiInteractUse && msg->multiInteractID != missionID) continue;

				auto cacheMission = CacheMissions::getRow(missionID);

				bool missionRepeatable = CacheMissions::GetRepeatable(cacheMission);

				// If mission added
				if (Database::HasMission(sender->GetObjectID() & 0xFFFFFFFF, missionID)) {
					auto dbMission = Database::GetMission(sender->GetObjectID() & 0xFFFFFFFF, missionID);
					if (dbMission.state == 4 || dbMission.state == 12) {
						missionOffer.missionID = missionOfferGiver.missionID = missionID;

						missionOffer.offerer = missionOfferGiver.offerer = msg->objectID;

						GameMessages::Send(owner->GetZoneInstance(), sender->GetZoneInstance()->sessionManager.GetSession(sender->GetObjectID())->systemAddress, msg->objectID, missionOfferGiver);
						GameMessages::Send(owner->GetZoneInstance(), sender->GetZoneInstance()->sessionManager.GetSession(sender->GetObjectID())->systemAddress, sender->GetObjectID(), missionOffer);
						return;
					}
				}
			}
			if (CacheMissionNPCComponent::GetOffersMission(*it)) {
				std::int32_t missionID = CacheMissionNPCComponent::GetMissionID(*it);

				// Check if it's the selected mission on multi interact
				if (msg->bIsMultiInteractUse && msg->multiInteractID != missionID) continue;

				auto cacheMission = CacheMissions::getRow(missionID);

				bool missionRepeatable = CacheMissions::GetRepeatable(cacheMission);
				
				bool isMissionActive = false;

				// If mission added
				if (Database::HasMission(sender->GetObjectID() & 0xFFFFFFFF, missionID)) {
					// or available / repeatable available
					auto dbMission = Database::GetMission(sender->GetObjectID() & 0xFFFFFFFF, missionID);
					if (!(dbMission.state == 1 || (missionRepeatable && dbMission.state == 9))) {
						// Skip
						continue;
					}
					if (dbMission.state == 2 || dbMission.state == 10) {
						isMissionActive = true;
					}
				}


				bool missionRequirementsPassed = false;

				if (!isMissionActive) {
					std::string_view prereqMissionID = CacheMissions::GetPrereqMissionID(cacheMission);

					if (prereqMissionID != "") {
						auto missionSweep = MissionRequirementParser::sweepMissionListNumerical(prereqMissionID);

						auto missionSweepDB = Database::GetAllMissionsByIDs(sender->GetObjectID() & 0xFFFFFFFF, missionSweep);

						missionRequirementsPassed = MissionRequirementParser(prereqMissionID, missionSweepDB).result;
					}
					else {
						missionRequirementsPassed = true;
					}
				}
				else {
					missionRequirementsPassed = true;
				}


				if (missionRequirementsPassed) {
					missionOffer.offerer = missionOfferGiver.offerer = msg->objectID;
					missionOffer.missionID = missionOfferGiver.missionID = missionID;
					GameMessages::Send(owner->GetZoneInstance(), sender->GetZoneInstance()->sessionManager.GetSession(sender->GetObjectID())->systemAddress, msg->objectID, missionOfferGiver);
					GameMessages::Send(owner->GetZoneInstance(), sender->GetZoneInstance()->sessionManager.GetSession(sender->GetObjectID())->systemAddress, sender->GetObjectID(), missionOffer);
					return;
				}
			}
		}
		
		if (missionOffer.missionID != -1) {

			missionOffer.offerer = missionOfferGiver.offerer = owner->GetObjectID();

			GameMessages::Send(owner->GetZoneInstance(), sender->GetZoneInstance()->sessionManager.GetSession(sender->GetObjectID())->systemAddress, msg->objectID, missionOfferGiver);
			GameMessages::Send(owner->GetZoneInstance(), sender->GetZoneInstance()->sessionManager.GetSession(sender->GetObjectID())->systemAddress, sender->GetObjectID(), missionOffer);
		}
	}

};

#endif