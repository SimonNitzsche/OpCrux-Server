#ifndef __REPLICA__COMPONENTS__MOVEMENT_AI_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__MOVEMENT_AI_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameObject.hpp"

#include "GameCache/ComponentsRegistry.hpp"
#include "GameCache/MovementAIComponent.hpp"
#include "Utils/ServerInfo.hpp"

using namespace DataTypes;
class ControllablePhysicsComponent;
class NavMeshManager;

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
	time_t nextUpdate = 0;

	std::uniform_int_distribution<> nextUpdateDist;
	NavMeshManager * navManager;
	Vector3 targetPos;

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
		//nextUpdateDist = std::uniform_int_distribution<>(20000, 25000);
		nextUpdateDist = std::uniform_int_distribution<>(6000, 25000);
	}

	void Awake();

	void SetTargetMovementPos(Vector3 position);

	void Update();
	
};

#endif