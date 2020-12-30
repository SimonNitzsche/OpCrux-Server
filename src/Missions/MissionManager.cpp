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
#include "Entity/Components/DestructibleComponent.hpp"

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
			if (taskType == Enums::EMissionTask::WIN_ACTIVITY) {
				c_target = 0;
			}

			if (c_target == target || c_target <= 0) {
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

void MissionManager::UpdateMissionTask(Entity::GameObject * sender, Entity::GameObject* player, std::int32_t missionID, std::int32_t taskMask, std::float_t update) {
    GM::NotifyMissionTask msgNotifyMissionTask;
    msgNotifyMissionTask.missionID = missionID;
    msgNotifyMissionTask.taskMask = taskMask;
    msgNotifyMissionTask.updates = { update };
    GameMessages::Send(player, player->GetObjectID(), msgNotifyMissionTask);
}

void MissionManager::GetAllNonMissionsThatAreMissingByTaskType(Entity::GameObject * player, std::map<int32_t, std::map<std::int32_t, std::int32_t>> *  selection, std::list<DatabaseModels::MissionModel> * currentMissions) {
    std::list<std::int32_t> missionFixups = {};

    for (auto it1 = selection->begin(); it1 != selection->end(); ++it1) {
        bool ignore = false;
        for (auto it2 = currentMissions->begin(); it2 != currentMissions->end(); ++it2) {
            if (it1->first == it2->missionID) {
                ignore = true;
                break;
            }
        }
        if (!ignore) {
            // Check that it's not a non-mission
            if (!CacheMissions::GetIsMission(it1->first)) {
                missionFixups.push_back(it1->first);
            }
        }
    }

    for (auto it = missionFixups.begin(); it != missionFixups.end(); ++it) {
        Logger::log("WRLD", "Adding mission " + std::to_string(*it));
        currentMissions->push_back(Database::AddMission(player->GetZoneInstance()->GetDBConnection(), player->GetObjectID().getPureID(), *it));
    }
}

void MissionManager::LaunchTaskEvent(Enums::EMissionTask taskType, Entity::GameObject * caster, DataTypes::LWOOBJID player, std::int32_t updateVal, std::int32_t extraParam) {
    WorldServer* Instance = caster->GetZoneInstance();
    std::int64_t dbPlayerID = player.getPureID();

    Entity::GameObject* playerObject = Instance->objectsManager->GetObjectByID(player);
    if (playerObject == nullptr) return;
    if (playerObject->GetComponent<CharacterComponent>() == nullptr) return;


    std::list<DatabaseModels::MissionModel> updateMissions = {};


    auto possibleMissions = GetMissionTasksByTaskTypeAndTarget(taskType, caster->GetLOT());

    if (possibleMissions.size() != 0) {
        std::list<std::int32_t> possibleMissionsOM = {};
        for (auto it = possibleMissions.begin(); it != possibleMissions.end(); ++it) {
            possibleMissionsOM.push_back(it->first);
        }

        auto currentMissions = Database::GetAllMissionsByIDsAndStates(caster->GetZoneInstance()->GetDBConnection(), dbPlayerID, possibleMissionsOM, { 2, 4, 10 });
    
        GetAllNonMissionsThatAreMissingByTaskType(playerObject, &possibleMissions, &currentMissions);

        switch (taskType)
        {
        case Enums::EMissionTask::KILL: {
			for (auto it = currentMissions.begin(); it != currentMissions.end(); ++it) {
				auto missionModel = *it;


				auto missionTasksProgress = StringUtils::splitString(it->progress, '|');
				auto updateTasks = possibleMissions.at(it->missionID);
				auto cacheMissionTasks = CacheMissionTasks::getRow(it->missionID).flatIt();

				auto iTarget = updateVal;
				updateVal = 1;

				for (int i = 0; i < missionTasksProgress.size(); ++i) {

					auto cacheMissionTasksRow = *std::next(cacheMissionTasks.begin(), i);
					if (CacheMissionTasks::GetTaskType(cacheMissionTasksRow) == std::int32_t(taskType)) {

						//if (CacheMissionTasks::GetTarget(cacheMissionTasksRow) != caster->GetLOT()) continue;
						if (CacheMissionTasks::GetTargetValue(cacheMissionTasksRow) < std::stoi(missionTasksProgress.at(i)) + updateVal) continue;

						std::string strTargetGroup = CacheMissionTasks::GetTargetGroup(cacheMissionTasksRow);
						auto listTargetGroup = StringUtils::splitString(strTargetGroup, ',');
						listTargetGroup.push_back(std::to_string(CacheMissionTasks::GetTarget(cacheMissionTasksRow)));
						std::string strTarget = std::to_string(iTarget);
						for (auto testSkill : listTargetGroup) {

							if (testSkill != strTarget) continue;

							missionTasksProgress.at(i) = std::to_string(std::stoi(missionTasksProgress.at(i)) + updateVal);
							UpdateMissionTask(caster, playerObject, missionModel.missionID, 1 << (i + 1), std::stoi(missionTasksProgress.at(i)));
							break;
						}
					}
				}

				missionModel.progress = StringUtils::StringVectorToString(missionTasksProgress, '|');
				updateMissions.push_back(missionModel);
			}
			break;
		}
        case Enums::EMissionTask::SCRIPT: {
			for (auto it = currentMissions.begin(); it != currentMissions.end(); ++it) {
				auto missionModel = *it;


				auto missionTasksProgress = StringUtils::splitString(it->progress, '|');
				auto updateTasks = possibleMissions.at(it->missionID);
				auto cacheMissionTasks = CacheMissionTasks::getRow(it->missionID).flatIt();

				for (int i = 0; i < missionTasksProgress.size(); ++i) {

					auto cacheMissionTasksRow = *std::next(cacheMissionTasks.begin(), i);
					if (CacheMissionTasks::GetTaskType(cacheMissionTasksRow) == std::int32_t(taskType)) {

						if (CacheMissionTasks::GetTarget(cacheMissionTasksRow) != caster->GetLOT()) continue;
						if (CacheMissionTasks::GetTargetValue(cacheMissionTasksRow) < std::stoi(missionTasksProgress.at(i)) + updateVal) continue;

						missionTasksProgress.at(i) = std::to_string(std::stoi(missionTasksProgress.at(i)) + updateVal);

						UpdateMissionTask(caster, playerObject, missionModel.missionID, 1 << (i + 1), std::stoi(missionTasksProgress.at(i)));
					}
				}

				missionModel.progress = StringUtils::StringVectorToString(missionTasksProgress, '|');
				updateMissions.push_back(missionModel);
			}
			break;
		}
        case Enums::EMissionTask::WIN_ACTIVITY: {
			for (auto it = currentMissions.begin(); it != currentMissions.end(); ++it) {
				auto missionModel = *it;


				auto missionTasksProgress = StringUtils::splitString(it->progress, '|');
				auto updateTasks = possibleMissions.at(it->missionID);
				auto cacheMissionTasks = CacheMissionTasks::getRow(it->missionID).flatIt();

				for (int i = 0; i < missionTasksProgress.size(); ++i) {

					auto cacheMissionTasksRow = *std::next(cacheMissionTasks.begin(), i);
					if (CacheMissionTasks::GetTaskType(cacheMissionTasksRow) == std::int32_t(taskType)) {

						auto iTarget = extraParam;

						//if (CacheMissionTasks::GetTarget(cacheMissionTasksRow) != caster->GetLOT()) continue;
						if (CacheMissionTasks::GetTargetValue(cacheMissionTasksRow) < std::stoi(missionTasksProgress.at(i)) + updateVal) continue;

						auto targetGroup = StringUtils::StringVectorToIntList(StringUtils::splitString(CacheMissionTasks::GetTargetGroup(cacheMissionTasksRow), ','));

						targetGroup.push_back(CacheMissionTasks::GetTarget(cacheMissionTasksRow));
						
						for (auto target : targetGroup) {
							if (iTarget == target) {
								missionTasksProgress.at(i) = std::to_string(std::stoi(missionTasksProgress.at(i)) + updateVal);
								UpdateMissionTask(caster, playerObject, missionModel.missionID, 1 << (i + 1), std::stoi(missionTasksProgress.at(i)));
							}
						}
					}
				}

				missionModel.progress = StringUtils::StringVectorToString(missionTasksProgress, '|');
				updateMissions.push_back(missionModel);
			}
			break;
		}
        case Enums::EMissionTask::COLLECTIBLE: {
            for (auto it = currentMissions.begin(); it != currentMissions.end(); ++it) {
                auto missionModel = *it;


                auto missionTasksProgress = StringUtils::splitString(it->progress, '|');
                auto updateTasks = possibleMissions.at(it->missionID);

                std::int32_t subTaskValue = (caster->GetZoneInstance()->luZone->zoneID << 8) + (updateVal & 0xFF);

                auto cacheMissionTasks = CacheMissionTasks::getRow(it->missionID).flatIt();

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
            for (auto it = currentMissions.begin(); it != currentMissions.end(); ++it) {
                auto missionModel = *it;


                auto missionTasksProgress = StringUtils::splitString(it->progress, '|');
                auto updateTasks = possibleMissions.at(it->missionID);
                auto cacheMissionTasks = CacheMissionTasks::getRow(it->missionID).flatIt();

                for (int i = 0; i < missionTasksProgress.size(); ++i) {

                    auto cacheMissionTasksRow = *std::next(cacheMissionTasks.begin(), i);
                    if (CacheMissionTasks::GetTaskType(cacheMissionTasksRow) == std::int32_t(taskType)) {
						if (caster->GetLOT() != CacheMissionTasks::GetTarget(cacheMissionTasksRow)) continue;

                        missionTasksProgress.at(i) = std::to_string(std::stoi(missionTasksProgress.at(i)) + updateVal);

                        UpdateMissionTask(caster, playerObject, missionModel.missionID, 1 << (i + 1), std::stoi(missionTasksProgress.at(i)));
                    }
                }

                missionModel.progress = StringUtils::StringVectorToString(missionTasksProgress, '|');
                updateMissions.push_back(missionModel);
            }
            break;
        }
        case Enums::EMissionTask::EMOTE: {
			for (auto it = currentMissions.begin(); it != currentMissions.end(); ++it) {
				auto missionModel = *it;


				auto missionTasksProgress = StringUtils::splitString(it->progress, '|');
				auto updateTasks = possibleMissions.at(it->missionID);
				auto cacheMissionTasks = CacheMissionTasks::getRow(it->missionID).flatIt();

				auto iTarget = updateVal;
				updateVal = 1;

				for (int i = 0; i < missionTasksProgress.size(); ++i) {

					auto cacheMissionTasksRow = *std::next(cacheMissionTasks.begin(), i);
					if (CacheMissionTasks::GetTaskType(cacheMissionTasksRow) == std::int32_t(taskType)) {
						

						if (caster->GetLOT() != CacheMissionTasks::GetTarget(cacheMissionTasksRow)) continue;
						if (CacheMissionTasks::GetTargetValue(cacheMissionTasksRow) < std::stoi(missionTasksProgress.at(i)) + 1) continue;


						std::string strTargetGroup = CacheMissionTasks::GetTaskParam1(cacheMissionTasksRow);
						auto listTargetGroup = StringUtils::splitString(strTargetGroup, ',');
						std::string strTarget = std::to_string(iTarget);
						for (auto testTarget : listTargetGroup) {

							if (testTarget != strTarget) continue;

							missionTasksProgress.at(i) = std::to_string(std::stoi(missionTasksProgress.at(i)) + updateVal);
							UpdateMissionTask(caster, playerObject, missionModel.missionID, 1 << (i + 1), std::stoi(missionTasksProgress.at(i)));
							break;
						}
					}
				}

				missionModel.progress = StringUtils::StringVectorToString(missionTasksProgress, '|');
				updateMissions.push_back(missionModel);
			}
			break;
		}
        case Enums::EMissionTask::SMASH_CHAIN:
            break;
        case Enums::EMissionTask::BUY:
            break;
        case Enums::EMissionTask::SELL:
            break;
        case Enums::EMissionTask::USE_ITEM: {
			for (auto it = currentMissions.begin(); it != currentMissions.end(); ++it) {
				auto missionModel = *it;


				auto missionTasksProgress = StringUtils::splitString(it->progress, '|');
				auto updateTasks = possibleMissions.at(it->missionID);
				auto cacheMissionTasks = CacheMissionTasks::getRow(it->missionID).flatIt();

				for (int i = 0; i < missionTasksProgress.size(); ++i) {

					auto cacheMissionTasksRow = *std::next(cacheMissionTasks.begin(), i);
					if (CacheMissionTasks::GetTaskType(cacheMissionTasksRow) == std::int32_t(taskType)) {

						auto iTarget = extraParam;

						//if (CacheMissionTasks::GetTarget(cacheMissionTasksRow) != caster->GetLOT()) continue;
						if (CacheMissionTasks::GetTargetValue(cacheMissionTasksRow) < std::stoi(missionTasksProgress.at(i)) + updateVal) continue;

						std::int32_t target = CacheMissionTasks::GetTarget(cacheMissionTasksRow);

						if (iTarget == target) {
							missionTasksProgress.at(i) = std::to_string(std::stoi(missionTasksProgress.at(i)) + updateVal);
							UpdateMissionTask(caster, playerObject, missionModel.missionID, 1 << (i + 1), std::stoi(missionTasksProgress.at(i)));
						}
					}
				}

				missionModel.progress = StringUtils::StringVectorToString(missionTasksProgress, '|');
				updateMissions.push_back(missionModel);
			}
			break;
		}
		case Enums::EMissionTask::USE_SKILL: {
			for (auto it = currentMissions.begin(); it != currentMissions.end(); ++it) {
				auto missionModel = *it;


				auto missionTasksProgress = StringUtils::splitString(it->progress, '|');
				auto updateTasks = possibleMissions.at(it->missionID);
				auto cacheMissionTasks = CacheMissionTasks::getRow(it->missionID).flatIt();

				auto skillID = extraParam;
				updateVal = 1;

				for (int i = 0; i < missionTasksProgress.size(); ++i) {

					auto cacheMissionTasksRow = *std::next(cacheMissionTasks.begin(), i);
					if (CacheMissionTasks::GetTaskType(cacheMissionTasksRow) == std::int32_t(taskType)) {

						//if (CacheMissionTasks::GetTarget(cacheMissionTasksRow) != caster->GetLOT()) continue;
						if (CacheMissionTasks::GetTargetValue(cacheMissionTasksRow) < std::stoi(missionTasksProgress.at(i)) + updateVal) continue;

						std::string strSkills = CacheMissionTasks::GetTaskParam1(cacheMissionTasksRow);
						auto listSkills = StringUtils::splitString(strSkills, ',');
						std::string strSkillID = std::to_string(skillID);
						for (auto testSkill : listSkills) {

							if (testSkill != strSkillID) continue;

							missionTasksProgress.at(i) = std::to_string(std::stoi(missionTasksProgress.at(i)) + updateVal);
							UpdateMissionTask(caster, playerObject, missionModel.missionID, 1 << (i + 1), std::stoi(missionTasksProgress.at(i)));
							break;
						}
					}
				}

				missionModel.progress = StringUtils::StringVectorToString(missionTasksProgress, '|');
				updateMissions.push_back(missionModel);
			}
			break;
		}
        case Enums::EMissionTask::GATHER: {
            auto invComp = playerObject->GetComponent<InventoryComponent>();
            for (auto it = currentMissions.begin(); it != currentMissions.end(); ++it) {
                auto missionModel = *it;

                auto missionTasksProgress = StringUtils::splitString(it->progress, '|');
                auto updateTasks = possibleMissions.at(it->missionID);

                auto cacheMissionTasks = CacheMissionTasks::getRow(it->missionID).flatIt();

                for (int i = 0; i < missionTasksProgress.size(); ++i) {
                    auto cacheMissionTasksRow = *std::next(cacheMissionTasks.begin(), i);

                    if (CacheMissionTasks::GetTaskType(cacheMissionTasksRow) == std::int32_t(taskType)) {

                        std::string taskParam1 = CacheMissionTasks::GetTaskParam1(cacheMissionTasksRow);
                        if (taskParam1 == "") taskParam1 = "0";

                        auto targetGroup = StringUtils::StringVectorToIntList(StringUtils::splitString(CacheMissionTasks::GetTargetGroup(cacheMissionTasksRow), ','));

                        targetGroup.push_back(CacheMissionTasks::GetTarget(cacheMissionTasksRow));


						std::int32_t targetGroupResult = 0;
						for (auto selectedBag : invComp->inventory) {
							// Go through inventory to check for item
							for (auto targetGroupIterator = targetGroup.begin(); targetGroupIterator != targetGroup.end(); ++targetGroupIterator) {
								for (auto slotIt = selectedBag.second.begin(); slotIt != selectedBag.second.end(); ++slotIt) {
									if (slotIt->second.LOT == *targetGroupIterator) {
										targetGroupResult += slotIt->second.quantity;
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
            for (auto it = currentMissions.begin(); it != currentMissions.end(); ++it) {
                auto missionModel = *it;

                auto missionTasksProgress = StringUtils::splitString(it->progress, '|');
                auto updateTasks = possibleMissions.at(it->missionID);

                auto cacheMissionTasks = CacheMissionTasks::getRow(it->missionID).flatIt();

                for (int i = 0; i < missionTasksProgress.size(); ++i) {
                    auto cacheMissionTasksRow = *std::next(cacheMissionTasks.begin(), i);
                    if (CacheMissionTasks::GetTaskType(cacheMissionTasksRow) == std::int32_t(taskType)) {
                        auto targetGroup = StringUtils::StringVectorToIntList(StringUtils::splitString(CacheMissionTasks::GetTargetGroup(cacheMissionTasksRow), ','));

                        int targetGroupResult = 0;
                        for (auto targetGroupIterator = targetGroup.begin(); targetGroupIterator != targetGroup.end(); ++targetGroupIterator) {
                            if(playerObject->GetComponent<CharacterComponent>()->GetFlag(*targetGroupIterator)) ++targetGroupResult;
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

    for (auto it = updateMissions.begin(); it != updateMissions.end(); ++it) {
        // Check if mission is done
        if (CheckIfMissionIsReadyToComplete(it->missionID, it->progress)) {
            if (it->state == 2 || it->state == 10) {
                it->state += 2;

                GM::NotifyMissionTask tMsg;
                tMsg.missionID = it->missionID;
				GameMessages::Send(playerObject, player, tMsg);
            }

			if ((it->state == 4 || it->state == 12) && !CacheMissions::GetIsMission(it->missionID)) {
				/*
				   Mission completion
			   */

				GM::NotifyMission msg;
				msg.missionID = it->missionID;
				msg.missionState = 0;
				msg.sendingRewards = true;
				GameMessages::Send(playerObject, player, msg);

				// Send rewards
				it->state = 8;
				// Update state to 8 in the db, to prevent infinity loop
				Database::UpdateMission(playerObject->GetZoneInstance()->GetDBConnection(), *it);
				SendMissionRewards(playerObject, *it);

				msg.missionState = it->state;
				msg.sendingRewards = false;
				GameMessages::Send(Instance, Instance->sessionManager.GetSession(player)->systemAddress, player, msg);
			}
        }

        Database::UpdateMission(playerObject->GetZoneInstance()->GetDBConnection(), *it);
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

    CharacterComponent* charComp = player->GetComponent<CharacterComponent>();
    if (charComp == nullptr) return;

    InventoryComponent* invComp = player->GetComponent<InventoryComponent>();
    if (invComp == nullptr) return;

	StatsComponent* statsComp = player->GetComponent<StatsComponent>();
	if (statsComp == nullptr) return;

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
	if (tmpVal > 0) {
		{GM::ModifyLegoScore nmsg; nmsg.score = charInfo.uScore; GameMessages::Send(player, player->GetObjectID(), nmsg); }
	}

    // Reward: Reputation
    tmpVal = CacheMissions::GetRewardReputation(missionRow);
    charInfo.reputation += (tmpVal > 0 ? tmpVal : 0);
	if (tmpVal > 0) {
		{GM::UpdateReputation nmsg; nmsg.iReputation = charInfo.reputation; GameMessages::Send(player, player->GetObjectID(), nmsg); }
	}
    // Reward: MaxImagination
    tmpVal = CacheMissions::GetRewardMaxImagination(missionRow);
    charInfo.maximagination += (tmpVal > 0 ? tmpVal : 0);
	if (tmpVal > 0) {
		statsComp->attributes.maxImagination += tmpVal;
	}
    // Reward: MaxHealth
    tmpVal = CacheMissions::GetRewardMaxHealth(missionRow);
    charInfo.maxhealth += (tmpVal > 0 ? tmpVal : 0);
	if (tmpVal > 0) {
		statsComp->attributes.maxHealth += tmpVal;
	}
    // Reward: MaxInventory
	tmpVal = CacheMissions::GetRewardMaxInventory(missionRow);
	charInfo.maxinventory += (tmpVal > 0 ? tmpVal : 0);
	if (tmpVal > 0) {
		{GM::SetInventorySize nmsg; nmsg.inventoryType = 0; nmsg.size = charInfo.maxinventory; GameMessages::Send(player, player->GetObjectID(), nmsg); }
	}

    charComp->InitCharInfo(charInfo);
    Database::UpdateChar(player->GetZoneInstance()->GetDBConnection(), charInfo);
	charComp->CheckLevelProgression();
	statsComp->SetDirty();
	player->GetZoneInstance()->objectsManager->Serialize(player);

    // Reward: Items

    // List <LOT, count>
    std::list<std::pair<std::int32_t, std::int32_t>> rewardItems = {
        {CacheMissions::GetRewardItem1(missionRow), CacheMissions::GetRewardItem1Count(missionRow)},
        {CacheMissions::GetRewardItem2(missionRow), CacheMissions::GetRewardItem2Count(missionRow)},
        {CacheMissions::GetRewardItem3(missionRow), CacheMissions::GetRewardItem3Count(missionRow)},
        {CacheMissions::GetRewardItem4(missionRow), CacheMissions::GetRewardItem4Count(missionRow)}
    };

    bool isChoiceReward = CacheMissions::GetIsChoiceReward(missionRow);
    for (auto rewItmIt = rewardItems.begin(); rewItmIt != rewardItems.end(); ++rewItmIt) {

		// For some reason, count 0 is a default to be count 1
		if (rewItmIt->second == 0) rewItmIt->second = 1;
        
		if (rewItmIt->first == -1 || rewItmIt->second <= 0) continue;
        if (isChoiceReward && (mission.chosenReward == -1 || mission.chosenReward != rewItmIt->first)) continue;

        invComp->AddItem(rewItmIt->first, rewItmIt->second);
    }

    // Reward: Emotes
	std::list<std::int32_t> emotesToUnlock = {
		CacheMissions::GetRewardEmote(missionRow),
		CacheMissions::GetRewardEmote2(missionRow),
		CacheMissions::GetRewardEmote3(missionRow),
		CacheMissions::GetRewardEmote4(missionRow)
	};

	for (auto emote : emotesToUnlock) {
		if (emote == -1) continue;

		// Enable Emote on client
		GM::SetEmoteLockState setEmoteLockStateGM;
		setEmoteLockStateGM.bLock = false;
		setEmoteLockStateGM.emoteID = emote;
		GameMessages::Send(player, player->GetObjectID(), setEmoteLockStateGM);

		// Save to database
		Database::UnlockEmote(player->GetZoneInstance()->GetDBConnection(), player->GetObjectID(), emote);
	}
}
