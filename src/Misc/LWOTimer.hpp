#ifndef __MISC__LWOTIMER_HPP__
#define __MISC__LWOTIMER_HPP__

#include <unordered_map>
#include <string>
#include "Utils/ServerInfo.hpp"

//#include "Entity/Components/ScriptComponent.hpp"
#include "Entity/GameObject.hpp"


class LWOTimer {
	std::unordered_map<Entity::GameObject*, std::unordered_map<std::u16string, long long>> timers;
public:
	void Update();

	void AddTimerWithCancelMs(int timeInMs, std::u16string name, Entity::GameObject * object);
	void AddTimerWithCancel(float timeInSeconds, std::u16string name, Entity::GameObject * object);

	void NotifyObjectDeleted(Entity::GameObject* object);
}; 


#endif