#ifndef __REPLICA__COMPONENTS__SWITCH_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__SWITCH_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"


using namespace DataTypes;

class SwitchComponent : public IEntityComponent {
private:
	bool bToggled = false;
	std::int64_t deactivateAt;
	std::uint32_t peopleOnIt = 0;
private:
	bool is_hit_switch;
	bool is_pressure_switch;
	bool is_timed_switch;
	std::int32_t switch_reset_time;
	std::uint32_t switch_users_max;
	std::uint32_t switch_users_required;
public:

	SwitchComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 49; }

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		factory->Write(bToggled);
		Logger::log("WRLD", "Setting switch " + std::to_string(owner->GetObjectID()) + " to " + (bToggled ? "activated" : "deactivated")+" with "+std::to_string(peopleOnIt)+" people on it.");
		if (!bToggled) owner->SetDirty();
	}

	void OnCollisionPhantom(Entity::GameObject * other) {
		if (peopleOnIt == 0) {
			Activate();
			deactivateAt = ServerInfo::uptime() + 100000;
		}
		++peopleOnIt;
	}

	void OnOffCollisionPhantom(Entity::GameObject * other) {
		if (peopleOnIt != 0) {
			--peopleOnIt;
			if (peopleOnIt == 0) {
				// Set Deactivation Timestamp
				deactivateAt = ServerInfo::uptime() + switch_reset_time;
				//Deactivate();
			}
		}
	}


	void OnRequestUse(Entity::GameObject * sender, GM::RequestUse * msg) {
		Activate();
	}

	void PopulateFromLDF(LDFCollection * collection) {
		LDF_GET_VAL_FROM_COLLECTION(is_hit_switch, collection, u"is_hit_switch", false);
		LDF_GET_VAL_FROM_COLLECTION(is_pressure_switch, collection, u"is_pressure_switch", false);
		LDF_GET_VAL_FROM_COLLECTION(is_timed_switch, collection, u"is_timed_switch", false);
		LDF_GET_VAL_FROM_COLLECTION(switch_reset_time, collection, u"switch_reset_time", 0);
		LDF_GET_VAL_FROM_COLLECTION(switch_users_max, collection, u"switch_users_max", 20);
		LDF_GET_VAL_FROM_COLLECTION(switch_users_required, collection, u"switch_users_required", 1);
	}

	void Update() {
		if (!bToggled) return;
		if (ServerInfo::uptime() > deactivateAt) Deactivate();
	}

	void Activate() {
		// We don't want to activate if it's activated.
		if (bToggled) return;
		// Activate
		bToggled = true;
		// Tell object needs serialization
		owner->SetDirty();
		// Trigger trigger
		owner->NotifyTriggerEvent("OnActivated");
		
	}

	void Deactivate() {
		// We don't want to deactivate if it's deactivated.
		if (!bToggled) return;
		// Deactivate
		bToggled = false;
		// Tell object needs serialization
		owner->SetDirty();
		// Trigger trigger
		owner->NotifyTriggerEvent("OnDectivated");
		owner->NotifyTriggerEvent("OnDeactivated");
	}

	void RegisterMessageHandlers() {
		REGISTER_OBJECT_MESSAGE_HANDLER(SwitchComponent, GM::RequestUse, OnRequestUse);
	}
};

#endif