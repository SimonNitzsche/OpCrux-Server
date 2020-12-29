#ifndef __ENTITY__NATIVESCRIPT_HPP__
#define __ENTITY__NATIVESCRIPT_HPP__

#include "Entity/GameMessages.hpp"

#include "Entity/Components/ScriptComponent.hpp"

class NativeScript {
public:
	virtual ~NativeScript() = default;
	NativeScript() {};

	// Events
	virtual void onStartup(Entity::GameObject * self) {}
	virtual void onProximityUpdate(Entity::GameObject * self, GM::ProximityUpdate msg) {}
	virtual void onArrivedAtDesiredWaypoint(Entity::GameObject * self, GM::ArrivedAtDesiredWaypoint msg) {}
	virtual void onMissionDialogueOK(Entity::GameObject* self, GM::MissionDialogueOK msg) {}
	virtual void onUse(Entity::GameObject * self, GM::RequestUse msg) {};
	virtual void onTimerDone(Entity::GameObject * self, GM::TimerDone msg) {};
	virtual void onFireEventServerSide(Entity::GameObject * self, GM::FireEventServerSide msg) {}
	virtual void onDie(Entity::GameObject* self, GM::Die msg) {}
	virtual void onCollisionPhantom(Entity::GameObject* self, Entity::GameObject * other) {}
	virtual void onPlayerReady(Entity::GameObject* self, GM::PlayerReady msg) {}
	virtual void onPlayerLoaded(Entity::GameObject* self, GM::PlayerLoaded msg) {}
	virtual void onFireEvent(Entity::GameObject* self, GM::FireEventServerSide msg) {}
	virtual void onRespondToMission(Entity::GameObject* self, GM::RespondToMission msg) {}
	virtual void onRebuildNotifyState(Entity::GameObject* self, GM::RebuildNotifyState msg) {}
	virtual void onRebuildComplete(Entity::GameObject* self, GM::RebuildNotifyState msg) {}

	// Helper functions
	void print(std::string text) {
		Logger::log("SCRIPT", text, LogType::INFO);
	}
};

#endif