#ifndef __MISSIONS__MISSIONMANAGER_HPP__
#define __MISSIONS__MISSIONMANAGER_HPP__

#include <memory>
#include <list>
#include <map>


#include "DataTypes/LWOOBJID.hpp"
#include "Enums/EMissionTask.hpp"
#include "Entity/GameObject.hpp"

static class MissionManager {
	
public:
	static std::map<std::int32_t, std::map<std::int32_t, std::int32_t>> GetMissionTasksByTaskTypeAndTarget(Enums::EMissionTask taskType, std::int32_t target);
	static void UpdateMissionTask(Entity::GameObject* sender, Entity::GameObject* player, std::int32_t missionID, std::int32_t taskMask, std::float_t update);
	static void LaunchTaskEvent(Enums::EMissionTask taskType, Entity::GameObject * caster, DataTypes::LWOOBJID player, std::int32_t updateVal=1);
	static bool CheckIfMissionIsReadyToComplete(std::int32_t missionID, std::string &progress);
	//static void UpdateMissionTask()
};

#endif