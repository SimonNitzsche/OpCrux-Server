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


using script_ptr = const std::shared_ptr<NativeScript>;
using script_factory = auto (*)()->script_ptr;

class ScriptComponent : public IEntityComponent {
private:
	std::string scriptName;
	// ScriptInstance;

	std::shared_ptr<NativeScript> instance;

	// Include native scripts

	static const std::unordered_map<std::string, script_factory> factories; 

private:
	bool _isDirtyFlag = false;

public:

	ScriptComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	inline bool bHasScriptLoaded() {
		if (instance)
			return true;
		return false;
	}

	static constexpr int GetTypeID() { return 5; }

	void OnEnable() {
		std::uint32_t compID = GetComponentID();
		scriptName = CacheScriptComponent::GetScriptName(compID);

		if (/*scriptName == ("ScriptComponent_" + std::to_string(compID) + "_script_name__removed") && */factories.find(scriptName) == factories.end()) {
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
		// Only load script once.
		if (instance)
			return;

		if (scriptName != "") {
			if (factories.find(scriptName) != factories.end()) {
				instance = factories.at(scriptName)();
				Logger::log("WRLD", "Loaded script \"" + scriptName + "\" sucessfully. LOT " + std::to_string(owner->GetLOT()), LogType::PASSED);
			}
			else {
				Logger::log("WRLD", "Unable to load script \"" + scriptName + "\": Script not found.", LogType::WARN);
				//owner->isSerializable = false;
			}
		}
	}

	void OnPlayerReady(Entity::GameObject* sender, GM::PlayerReady* msg) {
		if (instance)
			instance->onPlayerReady(owner, *msg);
	}

	void OnPlayerLoaded(Entity::GameObject* sender, GM::PlayerLoaded* msg) {
		if (instance)
			instance->onPlayerLoaded(owner, *msg);
	}

	void OnMissionDialogueOK(Entity::GameObject* sender, GM::MissionDialogueOK * msg) {
		if (instance)
			instance->onMissionDialogueOK(owner, *msg);
	}

	void OnRequestUse(Entity::GameObject * sender, GM::RequestUse * msg) {
		if (instance)
			instance->onUse(owner, *msg);
	}

	void OnTimerDone(std::pair<std::u16string, long long> timer) {
		if (instance)
			instance->onTimerDone(owner, TimerDone(timer.first));
	}

	void OnFireEventServerSide(Entity::GameObject * sender, GM::FireEventServerSide * msg) {
		if (instance)
			instance->onFireEventServerSide(owner, *msg);
	}

	void OnDie(Entity::GameObject* sender, GM::Die* msg) {
		if (instance)
			instance->onDie(owner, *msg);
	}

	void OnCollisionPhantom(Entity::GameObject* sender) {
		if (instance)
			instance->onCollisionPhantom(owner, sender);
	}

	void FireEvent(GM::FireEventServerSide msg) {
		if (instance)
			instance->onFireEvent(owner, msg);
	}

	void OnRespondToMission(Entity::GameObject* sender, GM::RespondToMission* msg) {
		if (instance)
			instance->onRespondToMission(sender, *msg);
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
			LDFCollection netConf = owner->GetNetworkedConfig();

			bool hasNetConf = netConf.begin() != netConf.end();

			factory->Write(hasNetConf);
			if (hasNetConf) {
				LDFUtils::SerializeCollection(*factory, netConf);
			}
		}
	}

	void RegisterMessageHandlers() {
		REGISTER_OBJECT_MESSAGE_HANDLER(ScriptComponent, GM::PlayerReady, OnPlayerReady);
		REGISTER_OBJECT_MESSAGE_HANDLER(ScriptComponent, GM::PlayerLoaded, OnPlayerLoaded);
		REGISTER_OBJECT_MESSAGE_HANDLER(ScriptComponent, GM::MissionDialogueOK, OnMissionDialogueOK);
		REGISTER_OBJECT_MESSAGE_HANDLER(ScriptComponent, GM::RequestUse, OnRequestUse);
		REGISTER_OBJECT_MESSAGE_HANDLER(ScriptComponent, GM::FireEventServerSide, OnFireEventServerSide);
		REGISTER_OBJECT_MESSAGE_HANDLER(ScriptComponent, GM::Die, OnDie);
		REGISTER_OBJECT_MESSAGE_HANDLER(ScriptComponent, GM::RespondToMission, OnRespondToMission);
	}

};

#endif