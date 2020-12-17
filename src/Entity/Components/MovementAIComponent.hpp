#ifndef __REPLICA__COMPONENTS__MOVEMENT_AI_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__MOVEMENT_AI_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameObject.hpp"

#include "GameCache/ComponentsRegistry.hpp"
#include "GameCache/MovementAIComponent.hpp"
#include "Utils/ServerInfo.hpp"

using namespace DataTypes;

class MovementAIComponent : public IEntityComponent {
private:

	float wanderChance;
	float wanderDelayMax;
	float wanderDelayMin;
	float wanderRadius;
	float wanderSpeed;

	ControllablePhysicsComponent * controllablePhysicsComponent;
	Vector3 basePosition;

	Entity::GameObject * pl;
	bool allowUpdate = false;

public:

	MovementAIComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 31; }

	void OnEnable() {
		std::int32_t componentID = CacheComponentsRegistry::GetComponentID(owner->GetLOT(), 31);
		wanderChance = CacheMovementAIComponent::GetWanderChance(componentID);
		wanderDelayMax = CacheMovementAIComponent::GetWanderDelayMax(componentID);
		wanderDelayMin = CacheMovementAIComponent::GetWanderDelayMin(componentID);
		wanderSpeed = CacheMovementAIComponent::GetWanderSpeed(componentID);

		
		//basePosition = Vector3(-406.6414489746094 ,350.69287109375, -157.47933959960938);
	}

	void Awake() {
		this->controllablePhysicsComponent = owner->GetComponent<ControllablePhysicsComponent>();
		basePosition = controllablePhysicsComponent->GetPosition();
		allowUpdate = false;
	}

	void Update() {
		if (!allowUpdate) return;
		Vector3 newPos = Vector3::zero(); // Vector3(basePosition.x, basePosition.y, basePosition.z);

		unsigned long long time = ServerInfo::uptime();

		time += (owner->GetObjectID().getPureID() * 2);

		newPos.x += 4 * std::cos(time);
		newPos.z += 4 * std::sin(time);

		controllablePhysicsComponent->SetVelocity(newPos);
		controllablePhysicsComponent->SetPosition(newPos + basePosition);
		owner->SetDirty();
	}
	
};

#endif