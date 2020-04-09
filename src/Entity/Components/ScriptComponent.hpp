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
#include "Entity/NativeScripts/ai/AG/L_AG_IMAG_SMASHABLE.hpp"
#include "Entity/NativeScripts/ai/AG/L_AG_SHIP_PLAYER_SHOCK_SERVER.hpp"
#include "Entity/NativeScripts/ai/AG/L_AG_SHIP_SHAKE.hpp"
#include "Entity/NativeScripts/ai/AG/L_AG_SPACE_STUFF.hpp"
#include "Entity/NativeScripts/ai/NP/L_NPC_NP_SPACEMAN_BOB.hpp"
#include "Entity/NativeScripts/ai/RACING/TRACK_GF/GF_RACE_SERVER.hpp"
#include "Entity/NativeScripts/ai/WILD/L_ALL_CRATE_CHICKEN.hpp"
#include "Entity/NativeScripts/ScriptComponent_0952_script_name__removed.hpp"
#include "Entity/NativeScripts/ScriptComponent_1021_script_name__removed.hpp"
	const std::unordered_map<std::string, script_factory> factories{
		{"TestScript", []() -> script_ptr {return std::make_shared<TestScript>(); }},
		{"scripts\\ai\\AG\\L_AG_BUS_DOOR.lua", []()->script_ptr {return std::make_shared<NATIVESCRIPT__AI__AG__L_AG_BUS_DOOR>(); }},
		{"scripts\\ai\\AG\\L_AG_IMAG_SMASHABLE.lua", []()->script_ptr {return std::make_shared<NATIVESCRIPT__AI__AG__L_AG_IMAG_SMASHABLE>(); }},
		{"scripts\\ai\\AG\\L_AG_SHIP_PLAYER_SHOCK_SERVER.lua", []()->script_ptr {return std::make_shared<NATIVESCRIPT__AI__AG__L_AG_SHIP_PLAYER_SHOCK_SERVER>(); }},
		{"scripts\\ai\\AG\\L_AG_SHIP_SHAKE.lua", []()->script_ptr {return std::make_shared<NATIVESCRIPT__AI__AG__L_AG_SHIP_SHAKE>(); }},
		{"scripts\\ai\\AG\\L_AG_SPACE_STUFF.lua", []()->script_ptr {return std::make_shared<NATIVESCRIPT__AI__AG__L_AG_SPACE_STUFF>();  }},
		{"scripts\\ai\\NP\\L_NPC_NP_SPACEMAN_BOB.lua", []()->script_ptr {return std::make_shared<NATIVESCRIPT__AI__NP__L_NPC_SPACEMAN_BOB>();  }},
		{"scripts\\ai\\RACING\\TRACK_GF\\GF_RACE_SERVER.lua", []()->script_ptr {return std::make_shared<NATIVESCRIPT__AI__RACING__TRACK_GF__GF_RACE_SERVER>(); }},
		{"scripts\\ai\\WILD\\L_ALL_CRATE_CHICKEN.lua", []()->script_ptr {return std::make_shared<NATIVESCRIPT__AI__WILD__L_ALL_CRATE_CHICKEN>(); }},
		{"ScriptComponent_952_script_name__removed", []()->script_ptr {return std::make_shared<NATIVESCRIPT__ScriptComponent_0952_script_name__removed>(); }},
		{"ScriptComponent_1021_script_name__removed", []()->script_ptr {return std::make_shared<NATIVESCRIPT__ScriptComponent_1021_script_name__removed>(); }}
	};

private:
	bool _isDirtyFlag = false;

public:

	ScriptComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 5; }

	void OnEnable() {
		std::uint32_t compID = CacheComponentsRegistry::GetComponentID(owner->GetLOT(), 5);
		scriptName = CacheScriptComponent::GetScriptName(compID);

		if (scriptName == ("ScriptComponent_" + std::to_string(compID) + "_script_name__removed") && factories.find(scriptName) == factories.end()) {
			scriptName = "";
		}
	}

	void PopulateFromLDF(LDFCollection * collection) {
		std::u16string customScriptName = u"";
		LDF_GET_VAL_FROM_COLLECTION(customScriptName, collection, u"custom_script_server", u"");

		if (customScriptName != u"") {
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
				//owner->isSerializable = false;
			}
		}
	}

	void OnMissionDialogueOK(Entity::GameObject* sender, GM::MissionDialogueOK & msg) {
		if (instance)
			instance->onMissionDialogueOK(owner, msg);
	}

	void OnRequestUse(Entity::GameObject * sender, GM::RequestUse & msg) {
		if (instance)
			instance->onUse(owner, msg);
	}

	void OnTimerDone(std::pair<std::u16string, long long> timer) {
		if (instance)
			instance->onTimerDone(owner, TimerDone(timer.first));
	}

	void OnFireEventServerSide(Entity::GameObject * sender, GM::FireEventServerSide & msg) {
		if (instance)
			instance->onFireEventServerSide(owner, msg);
	}

	void OnDie(Entity::GameObject* sender, GM::Die* msg) {
		if (instance)
			instance->onDie(owner, *msg);
	}

	std::vector<Entity::GameObject *> objectsInProximity = {};
	std::unordered_map<std::string, std::pair<float, std::vector<Entity::GameObject *>>> proximityRadii;

	inline void DoProximityUpdate() {
		if (proximityRadii.size() == 0) return;

		// Get own position
		Vector3 pos = Vector3::zero();
		ControllablePhysicsComponent * contPhysComp = owner->GetComponent<ControllablePhysicsComponent>();
		if (contPhysComp) {
			pos = contPhysComp->GetPosition();
		}
		else {
			SimplePhysicsComponent * simpPhysComp = owner->GetComponent<SimplePhysicsComponent>();
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
			ControllablePhysicsComponent * objectPhysicsComponent = o->GetComponent<ControllablePhysicsComponent>();
			if (!objectPhysicsComponent || objectPhysicsComponent == nullptr) continue;
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
			factory->Write(false);
		}
	}

};

#endif