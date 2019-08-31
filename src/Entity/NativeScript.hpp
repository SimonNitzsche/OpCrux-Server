#ifndef __ENTITY__NATIVESCRIPT_HPP__
#define __ENTITY__NATIVESCRIPT_HPP__

#include "Entity/GameMessages.hpp"

#include "Entity/Components/ScriptComponent.hpp"

struct TimerDone {
	std::wstring name;

	TimerDone(std::wstring name) : name(name) {}
};

class NativeScript {
public:
	virtual ~NativeScript() = default;
	NativeScript() {};

	// Events
	virtual void onStartup(Entity::GameObject * self) {}
	virtual void onProximityUpdate(Entity::GameObject * self, GM::ProximityUpdate msg) {}
	virtual void onArrivedAtDesiredWaypoint(Entity::GameObject * self, GM::ArrivedAtDesiredWaypoint msg) {}
	virtual void onUse(Entity::GameObject * self, GM::RequestUse msg) {};
	virtual void onTimerDone(Entity::GameObject * self, TimerDone msg) {};

	// Helper functions
	void print(std::string text) {
		Logger::log("SCRIPT", text, LogType::INFO);
	}
};

#endif