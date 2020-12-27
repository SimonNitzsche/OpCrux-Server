#ifndef __REPLICA__COMPONENTS__BASE_COMBAT_AI_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__BASE_COMBAT_AI_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"


using namespace DataTypes;

class BaseCombatAIComponent : public IEntityComponent {
public:
	enum class ECombatState : std::uint32_t {
		IDLE,
		AGGRO,
		TETHER,
		SPAWN,
		DEAD
	};
private:
	bool _isDirtyFlag = false;

	ECombatState state = ECombatState::IDLE;

	DataTypes::LWOOBJID target;

public:

	BaseCombatAIComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 60; }

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		factory->Write(_isDirtyFlag);
		if (_isDirtyFlag) {
			factory->Write(state);
			factory->Write(target);
		}
	}

	void SetState(ECombatState newState) {
		state = newState;
		_isDirtyFlag = true;
		owner->SetDirty();
	}

	void Start() {
		SetState(ECombatState::SPAWN);
	}

	void Update() {
		// We're spawned
		if (state == ECombatState::SPAWN) {
			// Wait 5 seconds
			if (owner->GetCreationTimestamp() + 5000 < ServerInfo::uptimeMs()) {
				// Go to idle
				SetState(ECombatState::IDLE);
			}
		}
	}

};

#endif