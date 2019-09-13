#ifndef __REPLICA__COMPONENTS__SCRIPT_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__SCRIPT_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "GameCache/ScriptComponent.hpp"

#include "Entity/NativeScript.hpp"

#include "Entity/GameMessages.hpp"

#include "Entity/Components/ControllablePhysicsComponent.hpp"
#include "Entity/Components/SimplePhysicsComponent.hpp"

#include "Utils/LDFUtils.hpp"

#include <string>
#include <array>

using namespace DataTypes;

class TestScript : public NativeScript {
	int counter = 0;
public:
	TestScript() : NativeScript() {
		Logger::log("TEST", "TestScript created. Instance: " + std::to_string(counter++));
	}

	~TestScript() {}
};

class ScriptComponent;

// Forward declare
class NATIVESCRIPT__AI__AG__L_AG_BUS_DOOR;

class ScriptComponent : public IEntityComponent {
private:
	std::string scriptName;
	// ScriptInstance;

	using script_ptr = const std::shared_ptr<NativeScript>;
	using script_factory = auto (*)()->script_ptr;

	std::shared_ptr<NativeScript> instance;

	// Include native scripts
#include "Entity/NativeScripts/ai/AG/L_AG_BUS_DOOR.hpp"
#include "Entity/NativeScripts/ai/AG/L_AG_SHIP_PLAYER_SHOCK_SERVER.hpp"
#include "Entity/NativeScripts/ai/AG/L_AG_SHIP_SHAKE.hpp"
#include "Entity/NativeScripts/ai/AG/L_AG_SPACE_STUFF.hpp"
	const std::unordered_map<std::string, script_factory> factories{
		{"TestScript", []() -> script_ptr {return std::make_shared<TestScript>(); }},
		{"scripts\\ai\\AG\\L_AG_BUS_DOOR.lua", []()->script_ptr {return std::make_shared<NATIVESCRIPT__AI__AG__L_AG_BUS_DOOR>(); }},
		{"scripts\\ai\\AG\\L_AG_SHIP_PLAYER_SHOCK_SERVER.lua", []()->script_ptr {return std::make_shared<NATIVESCRIPT__AI__AG__L_AG_SHIP_PLAYER_SHOCK_SERVER>(); }},
		{"scripts\\ai\\AG\\L_AG_SHIP_SHAKE.lua", []()->script_ptr {return std::make_shared<NATIVESCRIPT__AI__AG__L_AG_SHIP_SHAKE>(); }},
		{"scripts\\ai\\AG\\L_AG_SPACE_STUFF.lua", []()->script_ptr {return std::make_shared<NATIVESCRIPT__AI__AG__L_AG_SPACE_STUFF>();  }}
	};

private:
	bool _isDirtyFlag = false;

public:

	ScriptComponent() : IEntityComponent() {}

	void OnEnable() {
		std::uint32_t compID = CacheComponentsRegistry::GetComponentID(owner->GetLOT(), 5);
		scriptName = CacheScriptComponent::GetScriptName(compID);

		if (scriptName == ("ScriptComponent_" + std::to_string(compID) + "_script_name__removed")) {
			scriptName = "";
		}
	}

	void PopulateFromLDF(LDFCollection * collection) {
		std::wstring customScriptName = L"";
		LDF_GET_VAL_FROM_COLLECTION(customScriptName, collection, L"custom_script_server", L"");

		if (customScriptName != L"") {
			scriptName = std::string(customScriptName.begin(), customScriptName.end());
		}
	}

	void Start() {
		LoadScript();
		if (scriptName != "")
			if (instance)
				instance->onStartup(owner);
	}

	void LoadScript() {
		if (scriptName != "") {
			if (factories.find(scriptName) != factories.end()) {
				instance = factories.at(scriptName)();
				Logger::log("WRLD", "Loaded script \"" + scriptName + "\" sucessfully.", LogType::PASSED);
			}
			else {
				Logger::log("WRLD", "Unable to load script \"" + scriptName + "\": Script not found.", LogType::WARN);
				owner->isSerializable = false;
			}
		}
	}

	void OnRequestUse(Entity::GameObject * sender, GM::RequestUse * msg) {
		if (instance)
			instance->onUse(owner, *msg);
	}

	void OnTimerDone(std::pair<std::wstring, long long> timer) {
		if (instance)
			instance->onTimerDone(owner, TimerDone(timer.first));
	}

	std::vector<Entity::GameObject *> objectsInProximity = {};
	std::unordered_map<std::string, std::pair<float, std::vector<Entity::GameObject *>>> proximityRadii;

	inline void DoProximityUpdate() {
		if (proximityRadii.size() == 0) return;

		// Get own position
		Vector3 pos = Vector3::zero();
		ControllablePhysicsComponent * contPhysComp = reinterpret_cast<ControllablePhysicsComponent*>(owner->GetComponentByID(1));
		if (contPhysComp) {
			pos = contPhysComp->GetPosition();
		}
		else {
			SimplePhysicsComponent * simpPhysComp = reinterpret_cast<SimplePhysicsComponent*>(owner->GetComponentByID(3));
			if (simpPhysComp)
				pos = simpPhysComp->GetPosition();
			else
				return;
		}

		// Cleanup removed objects
		for (auto r : proximityRadii) {
			for (auto o : r.second.second) {
				if (!o) {
					objectsInProximity.erase(std::find(objectsInProximity.begin(), objectsInProximity.end(), o));
				}
			}
		}

		for (auto o : this->owner->GetZoneInstance()->objectsManager->GetObjects()) {
			// we can assume, the object has a controllable physics object, otherwise it can't move.
			ControllablePhysicsComponent * objectPhysicsComponent = reinterpret_cast<ControllablePhysicsComponent*>(o->GetComponentByID(1));
			if (!objectPhysicsComponent) continue;
			Vector3 position = Vector3::zero();
			position = objectPhysicsComponent->GetPosition();

			float difference = roundf(Vector3::Distance(pos, position));

			// Check if exists.
			for (auto rc : proximityRadii) {
				auto it = std::find(rc.second.second.begin(), rc.second.second.end(), o);
				if (it != rc.second.second.end()) {
					// Object in list, check if left

					// Look for matching radii
					if (difference > rc.second.first) {
						// Message object left.
						GM::ProximityUpdate msg; msg.name = rc.first; msg.status = "LEAVE"; msg.objId = o; instance->onProximityUpdate(owner, msg);
						auto it2 = std::find(proximityRadii[rc.first].second.begin(), proximityRadii[rc.first].second.end(), o);
						if (it2 != proximityRadii[rc.first].second.end())
							proximityRadii[rc.first].second.erase(it2);
					}	
				}
				else {
					// Object not in list, check if entered

					// Look for matching radii
					if (difference <= rc.second.first) {
						// Message object joined.
						auto it2 = std::find(rc.second.second.begin(), rc.second.second.end(), o);
						if (it2 != rc.second.second.end()) continue;
						proximityRadii[rc.first].second.push_back(o);
						GM::ProximityUpdate msg; msg.name = rc.first; msg.status = "ENTER"; msg.objId = o; instance->onProximityUpdate(owner, msg);
					}
				}
			}
		}
	}

	void Update() {
		
	}

	void PhysicUpdate() {
		DoProximityUpdate();
	}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		/* TODO: Script Component Serialization */
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
			factory->Write(_isDirtyFlag);
		}
	}

};

#endif