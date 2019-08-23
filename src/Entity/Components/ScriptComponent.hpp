#ifndef __REPLICA__COMPONENTS__SCRIPT_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__SCRIPT_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "GameCache/ScriptComponent.hpp"

#include "Entity/NativeScript.hpp"

#include "Entity/GameMessages.hpp"

#include "Entity/Components/ControllablePhysicsComponent.hpp"
#include "Entity/Components/SimplePhysicsComponent.hpp"

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

class TourBus : public NativeScript {
	//---- Moves the bus door while players are close to it.
	//---- Updated: 7/01/09 jnf...
	//---- Updated: 5/19/10 djames... added second proxy radius and outerCounter
	//
	//-- the logic behind opening and closing the door is based on two proximity radii
	//-- it is possible for players to leave the proximity sphere unintentionally 
	//-- when the bus moves up because the sphere moves with the bus
	//-- by maintaining two counters, for both the inner and outer radii, players will
	//-- only be considered "in" proximity if they are inside both radii and they will
	//-- only be considered "out" of proximity if they are outside both radii
	//
	int ProxRadius = 75; // the radius for "inner" proximity detection
	int OuterProxRadius = 85; // the radius for "outer" proximity detection
	std::string soundName = "{9a24f1fa-3177-4745-a2df-fbd996d6e1e3}";
	
	void onStartup(Entity::GameObject * self) {
		self->SetVar(L"counter", 0);
		self->SetVar(L"outerCounter", 0); 
		self->SetProximityRadius("busDoor", ProxRadius);
		self->SetProximityRadius("busDoorOuter", OuterProxRadius);
		//self:StopPathing()
	}
	
	void moveDoor(Entity::GameObject * self, bool bOpen) {
 		// print("move door **************");

		if (bOpen) {
			print("open door **************");
			//self:GoToWaypoint{iPathIndex = 0}
		}
		else {
			print("close door **************");
			//self:GoToWaypoint{iPathIndex = 1}               
		}
			    
		self->PlayNDAudioEmitter(soundName);
		
	}

	void onProximityUpdate(Entity::GameObject * self, GM::ProximityUpdate msg) {
		
		// If this isn't the proximity radius for the bus door behavior, then we're done here
		if (msg.name != "busDoor" && msg.name != "busDoorOuter") return;
		//    
		//    -- Make sure only humans are taken into account
		//    if not msg.objId:BelongsToFaction{factionID = 1}.bIsInFaction then return end 
		//            

		if (msg.objId->GetLOT() != 1) return;

		int counter = self->GetVar(L"counter");
		int outerCounter = self->GetVar(L"outerCounter");

		if(msg.status == "ENTER") {
			if (msg.name == "busDoor") {
				++counter;
			}
			else if (msg.name == "busDoorOuter") {
				++outerCounter;
			}
			// move up when a player is inside both radii
			if (counter == 1 && outerCounter >= 1) {
				moveDoor(self, true);
			}
		}
		else if (msg.status == "LEAVE") {
			if (msg.name == "busDoor") {
				if (counter > 0) {
					--counter;
				}
			}
			else if (msg.name == "busDoorOuter") {
				if (outerCounter > 0) {
					--outerCounter;
					int ceds = 43;
				}
			}

			// move down when no players are inside either radii
			if (counter == 0 && outerCounter == 0) {
				moveDoor(self, false);
			}
			
		}
		//	
		// print("proximity update: status="+msg.status+", name="+msg.name+", counter="+std::to_string(counter)+", outerCounter="+std::to_string(outerCounter));
		//    
		self->SetVar(L"counter", counter);
		self->SetVar(L"outerCounter", outerCounter);
	}
	
	void onArrivedAtDesiredWaypoint(Entity::GameObject * self, GM::ArrivedAtDesiredWaypoint msg) {
		if (msg.iPathIndex == 1) {
			// self:PlayFXEffect{ name  = "busDust", effectType = "create"} -- effectID = 642,
			// print('play effect busDoor');
		}
	}
};

class ScriptComponent : public IEntityComponent {
private:
	std::string scriptName;
	// ScriptInstance;

	using script_ptr = const std::shared_ptr<NativeScript>;
	using script_factory = auto (*)()->script_ptr;

	std::shared_ptr<NativeScript> instance;

	const std::unordered_map<std::string, script_factory> factories {
		{"TestScript", []() -> script_ptr {return std::make_shared<TestScript>(); }},
		{"scripts\\ai\\AG\\L_AG_BUS_DOOR.lua", []()->script_ptr {return std::make_shared<TourBus>(); }}
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
		if(scriptName!="")
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