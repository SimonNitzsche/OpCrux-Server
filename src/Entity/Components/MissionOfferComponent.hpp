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

	static constexpr std::int16_t GetTypeID() { return 73; }

	void OnEnable() {

	}

	void OnRequestUse(Entity::GameObject * sender, GM::RequestUse & msg) {
		
		// If multi interact and type is not 0, we are not the receiver it.
		if (msg.bIsMultiInteractUse && msg.multiInteractType != 0) { Logger::log("WRLD", "Unknown multiInteractType found: " + std::to_string(msg.multiInteractType), LogType::UNEXPECTED); return; }

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
				if (msg.bIsMultiInteractUse && msg.multiInteractID != missionID) continue;

				auto cacheMission = CacheMissions::getRow(missionID);

				bool missionRepeatable = CacheMissions::GetRepeatable(cacheMission);

				// If mission added
				if (Database::HasMission(sender->GetObjectID().getPureID(), missionID)) {
					auto dbMission = Database::GetMission(sender->GetObjectID().getPureID(), missionID);
					if (dbMission.state == 4 || dbMission.state == 12) {
						missionOffer.missionID = missionOfferGiver.missionID = missionID;

						missionOffer.offerer = missionOfferGiver.offerer = msg.objectID;

						GameMessages::Send(sender, owner->GetObjectID(), missionOffer);
						GameMessages::Send(sender, sender->GetObjectID(), missionOffer);
						return;
					}
				}
			}
			if (CacheMissionNPCComponent::GetOffersMission(*it)) {
				std::int32_t missionID = CacheMissionNPCComponent::GetMissionID(*it);

				// Check if it's the selected mission on multi interact
				if (msg.bIsMultiInteractUse && msg.multiInteractID != missionID) continue;

				auto cacheMission = CacheMissions::getRow(missionID);

				bool missionRepeatable = CacheMissions::GetRepeatable(cacheMission);
				
				bool isMissionActive = false;

				// If mission added
				if (Database::HasMission(sender->GetObjectID().getPureID(), missionID)) {
					// or available / repeatable available
					auto dbMission = Database::GetMission(sender->GetObjectID().getPureID(), missionID);
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

						auto missionSweepDB = Database::GetAllMissionsByIDs(sender->GetObjectID().getPureID(), missionSweep);

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
					missionOffer.offerer = missionOfferGiver.offerer = msg.objectID;
					missionOffer.missionID = missionOfferGiver.missionID = missionID;
					
					GameMessages::Send(sender, owner->GetObjectID(), missionOffer);
					GameMessages::Send(sender, sender->GetObjectID(), missionOffer);
					return;
				}
			}
		}
		
		if (missionOffer.missionID != -1) {

			missionOffer.offerer = missionOfferGiver.offerer = owner->GetObjectID();

			GameMessages::Send(sender, owner->GetObjectID(), missionOffer);
			GameMessages::Send(sender, sender->GetObjectID(), missionOffer);
		}
	}

	void OnMissionDialogueOK(Entity::GameObject * sender, GM::MissionDialogueOK & msg) {
			Logger::log("WRLD", "Triggered MissionDialogueOK.");
			
			if (!Database::HasMission(sender->GetObjectID() & 0xFFFFFFFF, msg.missionID)) {
				auto mis = Database::AddMission(sender->GetObjectID() & 0xFFFFFFFF, msg.missionID);
				{
					GM::NotifyMission gm;
					gm.missionID = mis.missionID;
					gm.missionState = mis.state;
					GameMessages::Send(sender->GetZoneInstance(), sender->GetZoneInstance()->sessionManager.GetSession(sender->GetObjectID())->systemAddress, sender->GetObjectID(), gm);
				}
			}
			else {
				auto mis = Database::GetMission(sender->GetObjectID() & 0xFFFFFFFF, msg.missionID);
				if (msg.bIsComplete && (mis.state == 4 || mis.state == 12)) {
					mis.state = 8;
					Database::UpdateMission(mis);
					GM::NotifyMission gm;
					gm.missionID = mis.missionID;
					gm.missionState = mis.state;
					gm.sendingRewards = true;
					GameMessages::Send(sender->GetZoneInstance(), sender->GetZoneInstance()->sessionManager.GetSession(sender->GetObjectID())->systemAddress, sender->GetObjectID(), gm);
					MissionManager::SendMissionRewards(sender, mis);
					gm.sendingRewards = false;
					GameMessages::Send(sender->GetZoneInstance(), sender->GetZoneInstance()->sessionManager.GetSession(sender->GetObjectID())->systemAddress, sender->GetObjectID(), gm);

					// Try to offer next mission.
					auto responderObj = sender->GetZoneInstance()->objectsManager->GetObjectByID(msg.responder);
					if (responderObj != nullptr) {
						GM::RequestUse requestUseMSG;
						requestUseMSG.bIsMultiInteractUse = false;
						requestUseMSG.objectID = msg.responder;
						requestUseMSG.user = sender;
						responderObj->OnRequestUse(sender, requestUseMSG);
						/*MissionOfferComponent* misOfferComp = responderObj->GetComponent<MissionOfferComponent>();
						if (misOfferComp != nullptr) {
							GM::RequestUse requestUseMSG;
							requestUseMSG.bIsMultiInteractUse = false;
							requestUseMSG.objectID = responder;
							requestUseMSG.user = sender;
							misOfferComp->OnRequestUse(sender, &requestUseMSG);
						}*/
					}
				}
			}

			// Entity::GameObject* targetObject = sender->GetZoneInstance()->objectsManager->GetObjectByID(objectID);
			/*if (targetObject)
				targetObject->OnRequestUse(sender, this);
			else
				Logger::log("WRLD", "RequestUse: Unable to find object " + std::to_string(objectID), LogType::ERR);*/

			// Make sure next mission gets offered on completion.
			if (Database::HasMission(sender->GetObjectID().getPureID(), msg.missionID)) {
				std::int32_t missionState = Database::GetMission(sender->GetObjectID().getPureID(), msg.missionID).state;
				if (missionState == 8 || missionState == 9) {
					GM::RequestUse requestUseMSG;
					requestUseMSG.bIsMultiInteractUse = false;
					requestUseMSG.objectID = owner->GetObjectID();
					this->OnRequestUse(sender, requestUseMSG);
				}
			}
	}

};

#endif