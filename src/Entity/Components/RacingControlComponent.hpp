#ifndef __REPLICA__COMPONENTS__RACING_CONTROL_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__RACING_CONTROL_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "Entity/Components/StatsComponent.hpp"
#include "Entity/Components/PhantomPhysicsComponent.hpp"
#include "Entity/Components/ScriptedActivityComponent.hpp"
#include "Entity/GameObject.hpp"
#include "GameCache/RebuildComponent.hpp"

using namespace DataTypes;

class RacingControlComponent : public ScriptedActivityComponent {
	friend class ScriptedActivityComponent;
private:
	

public:

	RacingControlComponent(std::int32_t componentID) : ScriptedActivityComponent(componentID) {}

	static constexpr int GetTypeID() { return 71; }

	void OnEnable() {
		
	}

	void Awake() {

	}

	void Update() {
		
	}

	void Start() {
		
	}

	void PopulateFromLDF(LDFCollection* collection) {
		
	}

	void Serialize(RakNet::BitStream* factory, ReplicaTypes::PacketTypes packetType) {
		/* TODO */
		

		ScriptedActivityComponent::Serialize(factory, packetType);

		factory->Write(false);
		factory->Write(false);
		factory->Write(false);

		factory->Write(false);
		factory->Write(false);

	}

};

#endif