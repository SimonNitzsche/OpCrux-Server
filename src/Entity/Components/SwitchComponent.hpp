#ifndef __REPLICA__COMPONENTS__SWITCH_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__SWITCH_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"


using namespace DataTypes;

class SwitchComponent : public IEntityComponent {
private:
	bool bToggled = false;
	long long deactivateAt;
	std::uint32_t peopleOnIt = 0;
private:
	bool is_hit_switch;
	bool is_pressure_switch;
	bool is_timed_switch;
	std::int32_t switch_reset_time;
	std::uint32_t switch_users_max;
	std::uint32_t switch_users_required;
public:

	SwitchComponent() : IEntityComponent() {}

	static constexpr int GetTypeID() { return 49; }

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		factory->Write(bToggled);
	}

	void OnCollisionPhantom(Entity::GameObject * other) {
		if (peopleOnIt == 0) {
			// We don't want to activate if it's activated.
			if (bToggled) return;
			// Activate
			bToggled = true;
			// Tell object needs serialization
			owner->SetDirty();
		}
		++peopleOnIt;
	}

	void OnOffCollisionPhantom(Entity::GameObject * other) {
		if (peopleOnIt != 0) {
			--peopleOnIt;
			if (peopleOnIt == 0) {
				// Set Deactivation Timestamp
				// deactivateAt = ServerInfo::uptime() + switch_reset_time;
				bToggled = false;
				owner->SetDirty();
			}
		}
	}


	void OnRequestUse(Entity::GameObject * sender, GM::RequestUse * msg) {
		Activate();
	}

	void PopulateFromLDF(LDFCollection * collection) {
		LDF_GET_VAL_FROM_COLLECTION(is_hit_switch, collection, L"is_hit_switch", false);
		LDF_GET_VAL_FROM_COLLECTION(is_pressure_switch, collection, L"is_pressure_switch", false);
		LDF_GET_VAL_FROM_COLLECTION(is_timed_switch, collection, L"is_timed_switch", false);
		LDF_GET_VAL_FROM_COLLECTION(switch_reset_time, collection, L"switch_reset_time", 0);
		LDF_GET_VAL_FROM_COLLECTION(switch_users_max, collection, L"switch_users_max", 20);
		LDF_GET_VAL_FROM_COLLECTION(switch_users_required, collection, L"switch_users_required", 1);
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
		// Set Deactivation Timestamp
		deactivateAt = ServerInfo::uptime() + switch_reset_time;
		// Tell object needs serialization
		owner->SetDirty();
	}

	void Deactivate() {
		// We don't want to deactivate if it's deactivated.
		if (!bToggled) return;
		// Deactivate
		bToggled = false;
		// Tell object needs serialization
		owner->SetDirty();
	}
};

#endif