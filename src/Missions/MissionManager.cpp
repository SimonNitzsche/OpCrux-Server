#include "MissionManager.hpp"
#include "GameCache/Missions.hpp"
#include "GameCache/MissionTasks.hpp"
#include "Entity/GameObject.hpp"
#include "Database/Database.hpp"
#include "Server/WorldServer.hpp"
#include "Entity/Components/CharacterComponent.hpp"
#include "Entity/GameMessages.hpp"

std::map<std::int32_t, std::map<std::int32_t, std::int32_t>> MissionManager::GetMissionTasksByTaskTypeAndTarget(Enums::EMissionTask taskType, std::int32_t target) {
    std::map<std::int32_t, std::map<std::int32_t, std::int32_t>> missionsFound;

	FDB::RowTopHeader rth = Cache.getRows("MissionTasks");
	for (int i = 0; i < rth.getRowCount(); ++i) {
		if (!rth.isValid(i)) continue;
		FDB::RowInfo rowInfo = rth[i];
        std::int32_t c_mission = CacheMissionTasks::GetID(rowInfo);
        std::map<std::int32_t, std::int32_t> tasks;
        std::int32_t taskIndex = 0;
		while (rowInfo.isValid()) {

			// Check for target first, then for taskType -> better performance
			std::int32_t c_target = CacheMissionTasks::GetTarget(rowInfo);
			if (c_target == target) {
				std::int32_t c_taskType = CacheMissionTasks::GetTaskType(rowInfo);
				if (c_taskType == static_cast<std::int32_t>(taskType)) {
                    std::int32_t c_uid = CacheMissionTasks::GetUID(rowInfo);
                    tasks.insert({ taskIndex, c_uid });
				}
			}

            if (tasks.size() != 0) {
                missionsFound.insert({ c_mission, tasks });
            }

			if (rowInfo.isLinkedRowInfoValid()) {
				rowInfo = rowInfo.getLinkedRowInfo();
			}
			else {
				break;
			}
            ++taskIndex;
		}
	}

	return missionsFound;
}

void MissionManager::LaunchTaskEvent(Enums::EMissionTask taskType, Entity::GameObject * caster, DataTypes::LWOOBJID player, std::int32_t updateVal) {
    WorldServer* Instance = caster->GetZoneInstance();
    std::int64_t dbPlayerID = player.getPureID();

    Entity::GameObject* playerObject = Instance->objectsManager->GetObjectByID(player);
    if (playerObject == nullptr) return;
    if (playerObject->GetComponent<CharacterComponent>() == nullptr) return;


    std::list<Database::MissionModel> updateMissions = {};

	switch (taskType)
	{
    case Enums::EMissionTask::KILL:
        break;
    case Enums::EMissionTask::SCRIPT:
        break;
    case Enums::EMissionTask::WIN_ACTIVITY:
        break;
    case Enums::EMissionTask::COLLECTIBLE: {
        auto possibleMissions = GetMissionTasksByTaskTypeAndTarget(taskType, caster->GetLOT());

        if (possibleMissions.size() != 0) {
            std::list<std::int32_t> possibleMissionsOM = {};
            for (auto it = possibleMissions.begin(); it != possibleMissions.end(); ++it) {
                possibleMissionsOM.push_back(it->first);
            }

            auto currentMissions = Database::GetAllMissionsByIDsAndStates(dbPlayerID, possibleMissionsOM, { 2, 10 });
            for (auto it = currentMissions.begin(); it != currentMissions.end(); ++it) {
                auto missionModel = *it;


                auto missionTasksProgress = it->GetTaskProgress();
                auto updateTasks = possibleMissions.at(it->missionID);

                for (auto it2 = updateTasks.begin(); it2 != updateTasks.end(); ++it2) {
                    int currentVal = *std::next(missionTasksProgress.begin(), it2->first);
                    int updatedVal = currentVal | (1 << updateVal);
                    *std::next(missionTasksProgress.begin(), it2->first) = updatedVal;
                    if (*std::next(missionTasksProgress.begin(), it2->first) != updatedVal) throw;
                }

                missionModel.progress = StringUtils::IntListToString(missionTasksProgress, '|');
                updateMissions.push_back(missionModel);
            }
        }
        break;
    }
    case Enums::EMissionTask::TALK_TO_NPC: {
        auto possibleMissions = GetMissionTasksByTaskTypeAndTarget(taskType, caster->GetLOT());
        
        if (possibleMissions.size() != 0) {
            std::list<std::int32_t> possibleMissionsOM = {};
            for (auto it = possibleMissions.begin(); it != possibleMissions.end(); ++it) {
                possibleMissionsOM.push_back(it->first);
            }

            auto currentMissions = Database::GetAllMissionsByIDsAndStates(dbPlayerID, possibleMissionsOM, { 2, 10 });
            for (auto it = currentMissions.begin(); it != currentMissions.end(); ++it) {
                auto missionModel = *it;
                
                
                auto missionTasksProgress = it->GetTaskProgress();
                auto updateTasks = possibleMissions.at(it->missionID);
                
                for (auto it2 = updateTasks.begin(); it2 != updateTasks.end(); ++it2) {
                    *std::next(missionTasksProgress.begin(), it2->first) += updateVal;
                }

                missionModel.progress = StringUtils::IntListToString(missionTasksProgress, '|');
                updateMissions.push_back(missionModel);
            }
        }
        
        break;
    }
    case Enums::EMissionTask::EMOTE:
        break;
    case Enums::EMissionTask::SMASH_CHAIN:
        break;
    case Enums::EMissionTask::BUY:
        break;
    case Enums::EMissionTask::SELL:
        break;
    case Enums::EMissionTask::USE_ITEM:
        break;
    case Enums::EMissionTask::USE_SKILL:
        break;
    case Enums::EMissionTask::GATHER:
        break;
    case Enums::EMissionTask::EXPLORE:
        break;
    case Enums::EMissionTask::INVENTORY_CHANGE:
        break;
    case Enums::EMissionTask::MINIGAME_ACHIEVEMENT:
        break;
    case Enums::EMissionTask::INTERACT:
        break;
    case Enums::EMissionTask::MISSION_COMPLETE:
        break;
    case Enums::EMissionTask::REPUTATION:
        break;
    case Enums::EMissionTask::VOTING:
        break;
    case Enums::EMissionTask::SHOWCASE:
        break;
    case Enums::EMissionTask::HEAL:
        break;
    case Enums::EMissionTask::RECEIVE_CAST:
        break;
    case Enums::EMissionTask::TAME_PET:
        break;
    case Enums::EMissionTask::RACING:
        break;
    case Enums::EMissionTask::FLAG:
        break;
    case Enums::EMissionTask::PLACE_MODEL:
        break;
    case Enums::EMissionTask::REMOVE_MODEL:
        break;
    case Enums::EMissionTask::ADD_BEHAVIOR:
        break;
    case Enums::EMissionTask::REMOVE_BEHAVIOR:
        break;
    case Enums::EMissionTask::CLAIM_PROPERTY:
        break;
    case Enums::EMissionTask::PROPERTY_TRAVEL_TO:
        break;
    case Enums::EMissionTask::TIME_PLAYED:
        break;
    case Enums::EMissionTask::DONATE:
        break;
    case Enums::EMissionTask::UNKNOWN_33:
        break;
    case Enums::EMissionTask::UNKNOWN_34:
        break;
    case Enums::EMissionTask::TEAM:
        break;
    case Enums::EMissionTask::UNKNOWN_36:
        break;
    default:
        break;
	}


    for (auto it = updateMissions.begin(); it != updateMissions.end(); ++it) {



        // Check if mission is done
        if (CheckIfMissionIsReadyToComplete(it->missionID, it->progress)) {
            if (it->state == 2 || it->state == 10) {
                it->state += 2;

                GM::NotifyMissionTask tMsg;
                tMsg.missionID = it->missionID;

            }

            GM::NotifyMission msg;
            msg.missionID = it->missionID;
            msg.missionState = it->state;
            GameMessages::Send(Instance, Instance->sessionManager.GetSession(player)->systemAddress, player, msg);
        }

        Database::UpdateMission(*it);
    }
}

bool MissionManager::CheckIfMissionIsReadyToComplete(std::int32_t missionID, std::string &progress) {
    Database::MissionModel mm; mm.missionID; mm.progress = progress;
    auto mp = mm.GetTaskProgress();

    std::int32_t rowIndex = 0;
    auto row = CacheMissionTasks::getRow(missionID);
    while (row.isValid()) {

        // Collectibles are handled differently
        if (CacheMissionTasks::GetTaskType(row) == 3) {
            std::int32_t sum = 0;
            std::int32_t targetVal = CacheMissionTasks::GetTargetValue(row);
            std::int32_t progVal = *std::next(mp.begin(), rowIndex);
            for (int i = 0; i < targetVal; ++i) {
                if (progVal & (1 << i)) ++sum;
            }
            if (sum < targetVal) return false;
        }
        else {
            if (*std::next(mp.begin(), rowIndex) < CacheMissionTasks::GetTargetValue(row)) return false;
        }

        if (!row.isLinkedRowInfoValid()) break;
        row = row.getLinkedRowInfo();
        ++rowIndex;
    }
    return true;
}
