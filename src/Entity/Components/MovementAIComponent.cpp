#include "MovementAIComponent.hpp"
#include "FileTypes/LUZFile/LUZone.hpp"
#include "ControllablePhysicsComponent.hpp"
#include "Misc/NavMeshManager.hpp"

void MovementAIComponent::Awake() {
	this->controllablePhysicsComponent = owner->GetComponent<ControllablePhysicsComponent>();
	basePosition = controllablePhysicsComponent->GetPosition();
	allowUpdate = true;

	this->navManager = &owner->GetZoneInstance()->luZone->navmeshManager;
}

void MovementAIComponent::SetTargetMovementPos(Vector3 position) {
	targetPos = position;
	owner->SetPosition(targetPos);
	this->controllablePhysicsComponent->SetVelocity(DataTypes::Vector3(0, 5.0f, 0));
}

void MovementAIComponent::Update() {
	if (!allowUpdate) {
		if (ServerInfo::uptimeMs() > nextUpdate) {
			allowUpdate = true;
			nextUpdate = ServerInfo::uptimeMs() + nextUpdateDist(RandomUtil::GetEngine());
		}
	}
		
	if (allowUpdate) {
		allowUpdate = false;
		Vector3 newPos = Vector3::zero(); // Vector3(basePosition.x, basePosition.y, basePosition.z);

		unsigned long long time = ServerInfo::uptime();

		time += (owner->GetObjectID().getPureID() * 2);

		newPos = this->navManager->findRandomPointAroundCircle(basePosition, 8.0f);

		//newPos.x += 4 * std::cos(time);
		//newPos.z += 4 * std::sin(time);

		SetTargetMovementPos(newPos);

		//controllablePhysicsComponent->SetVelocity(newPos);
		// controllablePhysicsComponent->SetPosition(newPos /*+ basePosition*/);
		// owner->SetDirty();
	}
}