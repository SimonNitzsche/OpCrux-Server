#include "Misc/LWOTimer.hpp"
#include "Entity/Components/ScriptComponent.hpp"

void LWOTimer::Update() {
	long long currentTime = ServerInfo::uptimeMs();
	for (auto it1 : timers) {
		auto r1 = timers.at(it1.first);
		for (auto it2 : r1) {
			auto r2 = r1.at(it2.first);
			if (currentTime >= r2) {
				// Time exceeded.
				// Stop timer and trigger stop event.
				reinterpret_cast<ScriptComponent*>(it1.first->GetComponentByID(5))->OnTimerDone(it2);
				//timers.at(it1.first).erase(it1.second.find(it2.first));
				timers.at(it1.first).erase(it2.first);
			}
		}
}
}

void LWOTimer::AddTimerWithCancel(int timeInMs, std::wstring name, Entity::GameObject * object) {
	if (timers.find(object) == timers.end()) timers.insert(std::pair<Entity::GameObject*, std::unordered_map<std::wstring, long long>>(object, {}));
	if (timers.at(object).find(name) == timers.at(object).end()) {
		// Add
		timers.at(object).insert(std::make_pair(name, ServerInfo::uptimeMs() + timeInMs));
	}
	else {
		// Already exists, can't add again -> Error
	}
}