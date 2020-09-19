#include "MissionManager.hpp"
#include "GameCache/Missions.hpp"
#include "GameCache/MissionTasks.hpp"
#include "Entity/GameObject.hpp"
#include "Entity/Components/CharacterComponent.hpp"
#include "Database/Database.hpp"
#include "Server/WorldServer.hpp"
#include "Entity/Components/CharacterComponent.hpp"
#include "Entity/GameMessages.hpp"
#include "Entity/Components/InventoryComponent.hpp"

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

            if (taskType == Enums::EMissionTask::GATHER) {
                c_target = 0;
            }

			if (c_target == target || c_target <= 0) {
				std::int32_t c_taskType = CacheMissionTasks::GetTaskType(rowInfo);
				if (c_taskType == static_cast<std::int32_t>(taskType)) {
                    std::int32_t c_uid = CacheMissionTasks::GetUID(rowInfo);
                    tasks.insert({ taskIndex, c_uid });
				}
			}

            if (!tasks.empty()) {
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

void MissionManager::UpdateMissionTask(Entity::GameObject * sender, Entity::GameObject* player, std::int32_t missionID, std::int32_t taskMask, std::float_t update) {
    GM::NotifyMissionTask msgNotifyMissionTask;
    msgNotifyMissionTask.missionID = missionID;
    msgNotifyMissionTask.taskMask = taskMask;
    msgNotifyMissionTask.updates = { update };
    GameMessages::Send(player, player->GetObjectID(), msgNotifyMissionTask);
}

void MissionManager::GetAllNonMissionsThatAreMissingByTaskType(DataTypes::LWOOBJID player, std::map<int32_t, std::map<std::int32_t, std::int32_t>> *  selection, std::list<DatabaseModels::MissionModel> * currentMissions) {
    std::list<std::int32_t> missionFixups = {};

    for (auto & it1 : *selection) {
        bool ignore = false;
        for (auto & currentMission : *currentMissions) {
            if (it1.first == currentMission.missionID) {
                ignore = true;
                break;
            }
        }
        if (!ignore) {
            // Check that it's not a non-mission
            if (!CacheMissions::GetIsMission(it1.first)) {
                missionFixups.push_back(it1.first);
            }
        }
    }

    for (int & missionFixup : missionFixups) {
        Logger::log("WRLD", "Adding mission " + std::to_string(missionFixup));
        currentMissions->push_back(Database::AddMission(player.getPureID(), missionFixup));
    }
}

void MissionManager::LaunchTaskEvent(Enums::EMissionTask taskType, Entity::GameObject * caster, DataTypes::LWOOBJID player, std::int32_t updateVal) {
    WorldServer* Instance = caster->GetZoneInstance();
    std::int64_t dbPlayerID = player.getPureID();

    Entity::GameObject* playerObject = Instance->objectsManager->GetObjectByID(player);
    if (playerObject == nullptr) return;
    if (playerObject->GetComponent<CharacterComponent>() == nullptr) return;


    std::list<DatabaseModels::MissionModel> updateMissions = {};


    auto possibleMissions = GetMissionTasksByTaskTypeAndTarget(taskType, caster->GetLOT());

    if (!possibleMissions.empty()) {
        std::list<std::int32_t> possibleMissionsOM = {};
        for (auto & possibleMission : possibleMissions) {
            possibleMissionsOM.push_back(possibleMission.first);
        }

        auto currentMissions = Database::GetAllMissionsByIDsAndStates(dbPlayerID, possibleMissionsOM, { 2, 4, 10 });
    
        GetAllNonMissionsThatAreMissingByTaskType(player, &possibleMissions, &currentMissions);

        switch (taskType)
        {
        case Enums::EMissionTask::KILL:
            break;
        case Enums::EMissionTask::SCRIPT:
            break;
        case Enums::EMissionTask::WIN_ACTIVITY:
            break;
        case Enums::EMissionTask::COLLECTIBLE: {
            for (auto & currentMission : currentMissions) {
                auto missionModel = currentMission;


                auto missionTasksProgress = StringUtils::splitString(currentMission.progress, '|');
                auto updateTasks = possibleMissions.at(currentMission.missionID);

                std::int32_t subTaskValue = (caster->GetZoneInstance()->luZone->zoneID << 8) + (updateVal & 0xFF);

                auto cacheMissionTasks = CacheMissionTasks::getRow(currentMission.missionID).flatIt();

                for (int i = 0; i < missionTasksProgress.size(); ++i) {

                    auto subTaskProgStr = StringUtils::splitString(missionTasksProgress.at(i), ':');
                    auto cacheMissionTasksRow = *std::next(cacheMissionTasks.begin(), i);
                    if (CacheMissionTasks::GetTaskType(cacheMissionTasksRow) == std::int32_t(taskType)) {
                        // Make sure to remove placeholder 0
                        if (subTaskProgStr.size() == 1 && subTaskProgStr.at(0) == "0") subTaskProgStr.clear();

                        auto subTaskProg = StringUtils::StringVectorToIntList(subTaskProgStr);

                        if (std::find(subTaskProg.begin(), subTaskProg.end(), subTaskValue) == subTaskProg.end()) {
                            subTaskProg.push_back(subTaskValue);

                            UpdateMissionTask(caster, playerObject, missionModel.missionID, 1 << (i + 1), subTaskValue);
                        }

                        missionTasksProgress.at(i) = StringUtils::IntListToString(subTaskProg, ':');
                    }
                }

                missionModel.progress = StringUtils::StringVectorToString(missionTasksProgress, '|');
                updateMissions.push_back(missionModel);
            }
            break;
        }
        case Enums::EMissionTask::TALK_TO_NPC: {
            for (auto & currentMission : currentMissions) {
                auto missionModel = currentMission;


                auto missionTasksProgress = StringUtils::splitString(currentMission.progress, '|');
                auto updateTasks = possibleMissions.at(currentMission.missionID);
                auto cacheMissionTasks = CacheMissionTasks::getRow(currentMission.missionID).flatIt();

                for (int i = 0; i < missionTasksProgress.size(); ++i) {

                    auto cacheMissionTasksRow = *std::next(cacheMissionTasks.begin(), i);
                    if (CacheMissionTasks::GetTaskType(cacheMissionTasksRow) == std::int32_t(taskType)) {
                        missionTasksProgress.at(i) = std::to_string(std::stoi(missionTasksProgress.at(i)) + updateVal);

                        UpdateMissionTask(caster, playerObject, missionModel.missionID, 1 << (i + 1), std::stoi(missionTasksProgress.at(i)));
                    }
                }

                missionModel.progress = StringUtils::StringVectorToString(missionTasksProgress, '|');
                updateMissions.push_back(missionModel);
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
        case Enums::EMissionTask::GATHER: {
            auto invComp = playerObject->GetComponent<InventoryComponent>();
            for (auto & currentMission : currentMissions) {
                auto missionModel = currentMission;

                auto missionTasksProgress = StringUtils::splitString(currentMission.progress, '|');
                auto updateTasks = possibleMissions.at(currentMission.missionID);

                auto cacheMissionTasks = CacheMissionTasks::getRow(currentMission.missionID).flatIt();

                for (int i = 0; i < missionTasksProgress.size(); ++i) {
                    auto cacheMissionTasksRow = *std::next(cacheMissionTasks.begin(), i);

                    if (CacheMissionTasks::GetTaskType(cacheMissionTasksRow) == std::int32_t(taskType)) {

                        std::string taskParam1 = CacheMissionTasks::GetTaskParam1(cacheMissionTasksRow);
                        if (taskParam1.empty()) taskParam1 = "0";

                        auto targetGroup = StringUtils::StringVectorToIntList(StringUtils::splitString(CacheMissionTasks::GetTargetGroup(cacheMissionTasksRow), ','));

                        if (targetGroup.empty()) {
                            targetGroup.push_back(CacheMissionTasks::GetTarget(cacheMissionTasksRow));
                        }

                        auto selectedBag = invComp->inventory.find(std::stoi(taskParam1));

                        bool targetGroupResult = 0;

                        // If we don't have the bag, we don't have the item
                        if (selectedBag != invComp->inventory.end()) {

                            // Go through inventory to check for item
                            for (std::int32_t & targetGroupIterator : targetGroup) {
                                for (auto & slotIt : selectedBag->second) {
                                    if (slotIt.second.LOT == targetGroupIterator) {
                                        targetGroupResult += slotIt.second.quantity;
                                    }
                                }
                            }
                        }

                        missionTasksProgress.at(i) = std::to_string(targetGroupResult);

                        UpdateMissionTask(caster, playerObject, missionModel.missionID, 1 << (i + 1), std::stoi(missionTasksProgress.at(i)));
                    }
                }

                missionModel.progress = StringUtils::StringVectorToString(missionTasksProgress, '|');
                updateMissions.push_back(missionModel);
            }
        }
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
        case Enums::EMissionTask::FLAG: {
            for (auto & currentMission : currentMissions) {
                auto missionModel = currentMission;

                auto missionTasksProgress = StringUtils::splitString(currentMission.progress, '|');
                auto updateTasks = possibleMissions.at(currentMission.missionID);

                auto cacheMissionTasks = CacheMissionTasks::getRow(currentMission.missionID).flatIt();

                for (int i = 0; i < missionTasksProgress.size(); ++i) {
                    auto cacheMissionTasksRow = *std::next(cacheMissionTasks.begin(), i);
                    if (CacheMissionTasks::GetTaskType(cacheMissionTasksRow) == std::int32_t(taskType)) {
                        auto targetGroup = StringUtils::StringVectorToIntList(StringUtils::splitString(CacheMissionTasks::GetTargetGroup(cacheMissionTasksRow), ','));

                        bool targetGroupResult = false;
                        for (int & targetGroupIterator : targetGroup) {
                            targetGroupResult |= playerObject->GetComponent<CharacterComponent>()->GetFlag(targetGroupIterator);
                        }

                        missionTasksProgress.at(i) = targetGroupResult ? "1" : "0";

                        UpdateMissionTask(caster, playerObject, missionModel.missionID, 1 << (i + 1), std::stoi(missionTasksProgress.at(i)));
                    }
                }

                missionModel.progress = StringUtils::StringVectorToString(missionTasksProgress, '|');
                updateMissions.push_back(missionModel);
            }
        }
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
    
    }

    for (auto & updateMission : updateMissions) {
        // Check if mission is done
        if (CheckIfMissionIsReadyToComplete(updateMission.missionID, updateMission.progress)) {
            if (updateMission.state == 2 || updateMission.state == 10) {
                updateMission.state += 2;

                GM::NotifyMissionTask tMsg;
                tMsg.missionID = updateMission.missionID;

            }

            /*
                Mission completion
            */

            GM::NotifyMission msg;
            msg.missionID = updateMission.missionID;
            msg.missionState = 0;
            msg.sendingRewards = true;
            GameMessages::Send(playerObject, player, msg);

            // TODO: Send rewards

            msg.missionState = updateMission.state;
            msg.sendingRewards = false;
            GameMessages::Send(Instance, Instance->sessionManager.GetSession(player)->systemAddress, player, msg);
        }

        Database::UpdateMission(updateMission);
    }
}

bool MissionManager::CheckIfMissionIsReadyToComplete(std::int32_t missionID, std::string &progress) {
    DatabaseModels::MissionModel mm; mm.missionID; mm.progress = progress;
    auto mp = StringUtils::splitString(mm.progress, '|');

    std::int32_t rowIndex = 0;
    auto row = CacheMissionTasks::getRow(missionID);
    while (row.isValid()) {

        // Collectibles are handled differently
        if (CacheMissionTasks::GetTaskType(row) == 3) {
            std::int32_t sum = 0;
            std::int32_t targetVal = CacheMissionTasks::GetTargetValue(row);
            std::string progVal = *std::next(mp.begin(), rowIndex);
            if (StringUtils::splitString(progVal, ':').size() < targetVal) return false;
        }
        else {
            if (std::stoi(*std::next(mp.begin(), rowIndex)) < CacheMissionTasks::GetTargetValue(row)) return false;
        }

        if (!row.isLinkedRowInfoValid()) break;
        row = row.getLinkedRowInfo();
        ++rowIndex;
    }
    return true;
}

void MissionManager::SendMissionRewards(Entity::GameObject* player, DatabaseModels::MissionModel mission) {

    // Get mission info
    auto missionRow = CacheMissions::getRow(mission.missionID);

    // Stop if mission not valid
    if (!missionRow.isValid()) return;

    auto* charComp = player->GetComponent<CharacterComponent>();
    if (charComp == nullptr) return;

    auto* invComp = player->GetComponent<InventoryComponent>();
    if (invComp == nullptr) return;

	auto charInfo = charComp->GetCharInfo();
	auto charStats = charComp->GetCharStats();

    /*
        TODO: Repeatable Missions
    */

    std::int32_t tmpVal;

    // Reward: Currency
    tmpVal = CacheMissions::GetRewardCurrency(missionRow);;
    charInfo.currency += (tmpVal > 0 ? tmpVal : 0);

	charStats.TotalCurrencyCollected += (tmpVal > 0 ? tmpVal : 0);
	{GM::UpdatePlayerStatistic nmsg; nmsg.updateID = std::uint32_t(EStats::TotalCurrencyCollected); nmsg.updateValue = tmpVal > 0 ? tmpVal : 0; GameMessages::Send(player, player->GetObjectID(), nmsg); }
	{GM::SetCurrency nmsg; nmsg.currency = charInfo.currency; nmsg.sourceObject = player->GetObjectID(); nmsg.sourceType = 5; GameMessages::Send(player, player->GetObjectID(), nmsg); }

    // Reward: LEGO Score
    tmpVal = CacheMissions::GetLegoScore(missionRow);
    charInfo.uScore += (tmpVal > 0 ? tmpVal : 0);
    // TODO: Level Update
    // Reward: Reputation
    tmpVal = CacheMissions::GetRewardReputation(missionRow);
    charInfo.reputation += (tmpVal > 0 ? tmpVal : 0);
    // Reward: MaxImagination
    tmpVal = CacheMissions::GetRewardMaxImagination(missionRow);
    charInfo.imagination += (tmpVal > 0 ? tmpVal : 0);
    // Reward: MaxHealth
    tmpVal = CacheMissions::GetRewardMaxHealth(missionRow);
    charInfo.health += (tmpVal > 0 ? tmpVal : 0);
    // TODO: Reward: MaxInventory
    // TODO: Reward: MaxModel
    
    charComp->InitCharInfo(charInfo);
    Database::UpdateChar(charInfo);
	charComp->CheckLevelProgression();
    player->GetZoneInstance()->objectsManager->Serialize(player);

    // TODO: Reward: Items

    // List <LOT, count>
    std::list<std::pair<std::int32_t, std::int32_t>> rewardItems = {
        {CacheMissions::GetRewardItem1(missionRow), CacheMissions::GetRewardItem1Count(missionRow)},
        {CacheMissions::GetRewardItem2(missionRow), CacheMissions::GetRewardItem2Count(missionRow)},
        {CacheMissions::GetRewardItem3(missionRow), CacheMissions::GetRewardItem3Count(missionRow)},
        {CacheMissions::GetRewardItem4(missionRow), CacheMissions::GetRewardItem4Count(missionRow)}
    };

    bool isChoiceReward = CacheMissions::GetIsChoiceReward(missionRow);
    for (auto & rewardItem : rewardItems) {
        if (rewardItem.first == -1 || rewardItem.second <= 0) continue;
        if (isChoiceReward && (mission.chosenReward == -1 || mission.chosenReward != rewardItem.first)) continue;

        invComp->AddItem(rewardItem.first, rewardItem.second);
    }

    // TODO: Reward: Emotes
}
