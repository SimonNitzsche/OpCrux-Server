#ifndef __MISC__LWOTIMER_HPP__
#define __MISC__LWOTIMER_HPP__

#include <unordered_map>
#include <string>
#include "Utils/ServerInfo.hpp"

//#include "Entity/Components/ScriptComponent.hpp"
#include "Entity/GameObject.hpp"


class LWOTimer {
	std::unordered_map<Entity::GameObject*, std::unordered_map<std::wstring, long long>> timers;
public:
	void Update();

	void AddTimerWithCancel(int timeInMs, std::wstring name, Entity::GameObject * object);
}; 


#endif