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

		// Make sure to recheck if we have the item already.
		MissionManager::LaunchTaskEvent(Enums::EMissionTask::GATHER, msg->user, msg->user->GetObjectID());

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
				if (Database::HasMission(owner->GetZoneInstance()->GetDBConnection(), sender->GetObjectID().getPureID(), missionID)) {
					auto dbMission = Database::GetMission(owner->GetZoneInstance()->GetDBConnection(), sender->GetObjectID().getPureID(), missionID);
					
					// Trouble caused by good LU dev ideas for NS vendors
					if (msg->bIsMultiInteractUse && dbMission.state == 2) {
						if (
								(dbMission.missionID == 588 && owner->GetLOT() == 6633) || // assembly
								(dbMission.missionID == 589 && owner->GetLOT() == 6634) || // venture
								(dbMission.missionID == 590 && owner->GetLOT() == 6632) || // paradox
								(dbMission.missionID == 591 && owner->GetLOT() == 6631)    // sentinel
							)
						{
							dbMission.state = 4;
							Database::UpdateMission(owner->GetZoneInstance()->GetDBConnection(), dbMission);
						}
					}

					if (dbMission.state == 4 || dbMission.state == 12 || dbMission.state == 2 || dbMission.state == 10 ) {
						missionOffer.missionID = missionOfferGiver.missionID = missionID;

						missionOffer.offerer = missionOfferGiver.offerer = msg->objectID;

						GameMessages::Send(sender, owner->GetObjectID(), missionOffer);
						GameMessages::Send(sender, sender->GetObjectID(), missionOffer);
						GameMessages::Send(sender, sender->GetObjectID(), missionOffer);
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
				if (Database::HasMission(owner->GetZoneInstance()->GetDBConnection(), sender->GetObjectID().getPureID(), missionID)) {
					// or available / repeatable available
					auto dbMission = Database::GetMission(owner->GetZoneInstance()->GetDBConnection(), sender->GetObjectID().getPureID(), missionID);
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

						auto missionSweepDB = Database::GetAllMissionsByIDs(owner->GetZoneInstance()->GetDBConnection(), sender->GetObjectID().getPureID(), missionSweep);

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
					
					GameMessages::Send(sender, owner->GetObjectID(), missionOffer);
					GameMessages::Send(sender, sender->GetObjectID(), missionOffer);
					GameMessages::Send(sender, sender->GetObjectID(), missionOffer);
					return;
				}
			}
		}
		
		if (missionOffer.missionID != -1) {

			missionOffer.offerer = missionOfferGiver.offerer = owner->GetObjectID();

			GameMessages::Send(sender, owner->GetObjectID(), missionOffer);
			GameMessages::Send(sender, sender->GetObjectID(), missionOffer);
			GameMessages::Send(sender, sender->GetObjectID(), missionOffer);
		}
	}

	void OnMissionDialogueOK(Entity::GameObject * sender, GM::MissionDialogueOK * msg) {
		Logger::log("WRLD", "Triggered MissionDialogueOK.");
			
		if (!Database::HasMission(owner->GetZoneInstance()->GetDBConnection(), sender->GetObjectID() & 0xFFFFFFFF, msg->missionID)) {
			auto mis = Database::AddMission(owner->GetZoneInstance()->GetDBConnection(), sender->GetObjectID() & 0xFFFFFFFF, msg->missionID);
			{
				GM::NotifyMission gm;
				gm.missionID = mis.missionID;
				gm.missionState = mis.state;
				GameMessages::Send(sender->GetZoneInstance(), sender->GetZoneInstance()->sessionManager.GetSession(sender->GetObjectID())->systemAddress, sender->GetObjectID(), gm);
			}
		}
		else {
			auto mis = Database::GetMission(owner->GetZoneInstance()->GetDBConnection(), sender->GetObjectID() & 0xFFFFFFFF, msg->missionID);
			if (msg->bIsComplete && (mis.state == 4 || mis.state == 12)) {
				mis.state = 8;
				Database::UpdateMission(owner->GetZoneInstance()->GetDBConnection(), mis);
				GM::NotifyMission gm;
				gm.missionID = mis.missionID;
				gm.missionState = mis.state;
				gm.sendingRewards = true;
				GameMessages::Send(sender->GetZoneInstance(), sender->GetZoneInstance()->sessionManager.GetSession(sender->GetObjectID())->systemAddress, sender->GetObjectID(), gm);
				MissionManager::SendMissionRewards(sender, mis);
				gm.sendingRewards = false;
				GameMessages::Send(sender->GetZoneInstance(), sender->GetZoneInstance()->sessionManager.GetSession(sender->GetObjectID())->systemAddress, sender->GetObjectID(), gm);

				// Try to offer next mission.
				auto responderObj = sender->GetZoneInstance()->objectsManager->GetObjectByID(msg->responder);
				if (responderObj != nullptr) {
					GM::RequestUse requestUseMSG;
					requestUseMSG.bIsMultiInteractUse = false;
					requestUseMSG.objectID = msg->responder;
					requestUseMSG.user = sender;
					responderObj->OnMessage(sender, requestUseMSG.GetID(), &requestUseMSG);
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
		if (Database::HasMission(owner->GetZoneInstance()->GetDBConnection(), sender->GetObjectID().getPureID(), msg->missionID)) {
			std::int32_t missionState = Database::GetMission(owner->GetZoneInstance()->GetDBConnection(), sender->GetObjectID().getPureID(), msg->missionID).state;
			if (missionState == 8 || missionState == 9) {
				GM::RequestUse requestUseMSG;
				requestUseMSG.bIsMultiInteractUse = false;
				requestUseMSG.objectID = owner->GetObjectID();
				this->OnMessage(sender, requestUseMSG.GetID(), &requestUseMSG);
			}
		}
	}

	void RegisterMessageHandlers() {
		REGISTER_OBJECT_MESSAGE_HANDLER(MissionOfferComponent, GM::RequestUse, OnRequestUse);
		REGISTER_OBJECT_MESSAGE_HANDLER(MissionOfferComponent, GM::MissionDialogueOK, OnMissionDialogueOK);
	}
};

#endif