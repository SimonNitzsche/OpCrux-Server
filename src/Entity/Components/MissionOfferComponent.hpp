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

		missionOffer.missionID = -1;

		// pick first that meets requirements
		// get missions of npc
		auto missionsOffering = CacheMissionNPCComponent::getRow(GetComponentID()).flatIt();
		for (auto it = missionsOffering.begin(); it != missionsOffering.end(); ++it) {
			// Only if offering mission
			if (CacheMissionNPCComponent::GetOffersMission(*it)) {
				std::int32_t missionID = CacheMissionNPCComponent::GetMissionID(*it);

				// Check if it's the selected mission on multi interact
				if (msg->bIsMultiInteractUse && msg->multiInteractID != missionID) continue;

				auto cacheMission = CacheMissions::getRow(missionID);

				bool missionRepeatable = CacheMissions::GetRepeatable(cacheMission);
				
				// If mission added
				if (Database::HasMission(sender->GetObjectID() & 0xFFFFFFFF, missionID)) {
					// or available / repeatable availabl
					auto dbMission = Database::GetMission(sender->GetObjectID() & 0xFFFFFFFF, missionID);
					if (!(dbMission.state == 1 || (missionRepeatable && 9))) {
						// Skip
						continue;
					}
				}

				std::string_view prereqMissionID = CacheMissions::GetPrereqMissionID(cacheMission);

				bool missionRequirementsPassed = false;

				if (prereqMissionID != "") {
					auto missionSweep = MissionRequirementParser::sweepMissionListNumerical(prereqMissionID);

					auto missionSweepDB = Database::GetAllMissionsByIDs(sender->GetObjectID() & 0xFFFFFFFF, missionSweep);

					missionRequirementsPassed = MissionRequirementParser(prereqMissionID, missionSweepDB).result;
				}
				else {
					missionRequirementsPassed = true;
				}

				if (missionRequirementsPassed) {
					missionOffer.missionID = missionID;
					break;
				}
			}
		}
		
		if (missionOffer.missionID != -1) {

			missionOffer.offerer = owner->GetObjectID();

			GameMessages::Send(owner->GetZoneInstance(), sender->GetZoneInstance()->sessionManager.GetSession(sender->GetObjectID())->systemAddress, sender->GetObjectID(), missionOffer);
		}
	}

};

#endif