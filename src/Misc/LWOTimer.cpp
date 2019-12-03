#include "Misc/LWOTimer.hpp"
#include "Entity/Components/ScriptComponent.hpp"

void LWOTimer::Update() {
	long long currentTime = ServerInfo::uptimeMs();
	for (auto it1 : timers) {
		auto r1 = timers.at(it1.first);
		for (auto it2 : r1) {
			auto r2 = r1.at(it2.first);

			//Logger::log("TEST", std::string(it2.first.begin(), it2.first.end()) + " time left: " + std::to_string(r2 - currentTime));
			if (currentTime >= r2) {
				// Time exceeded.
				// Stop timer and trigger stop event.
				timers.at(it1.first).erase(it2.first);
				it1.first->GetComponent<ScriptComponent>()->OnTimerDone(it2);
				//timers.at(it1.first).erase(it1.second.find(it2.first));
			}
			else {

			}
		}
}
}

void LWOTimer::AddTimerWithCancel(int timeInMs, std::u16string name, Entity::GameObject * object) {
	if (timers.find(object) == timers.end()) timers.insert(std::pair<Entity::GameObject*, std::unordered_map<std::u16string, long long>>(object, {}));
	if (timers.at(object).find(name) == timers.at(object).end()) {
		// Add
		timers.at(object).insert(std::make_pair(name, ServerInfo::uptimeMs() + timeInMs));
	}
	else {
		// Already exists, can't add again -> Error
	}
}

void LWOTimer::AddTimerWithCancel(float timeInSeconds, std::u16string name, Entity::GameObject * object) {
	AddTimerWithCancel(std::lroundf(timeInSeconds) * 1000, name, object);
}