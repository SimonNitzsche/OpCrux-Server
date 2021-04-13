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
	//owner->SetPosition(targetPos);
	//this->controllablePhysicsComponent->SetVelocity(DataTypes::Vector3(0, 5.0f, 0));
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

		std::uint64_t time = ServerInfo::uptime();

		time += (owner->GetObjectID().getPureID() * 2);

		newPos = this->navManager->findRandomPointAroundCircle(basePosition, 8.0f);

		//newPos.x += 4 * std::cos(time);
		//newPos.z += 4 * std::sin(time);
		if(targetPos == Vector3::zero())
			SetTargetMovementPos(newPos);

		//controllablePhysicsComponent->SetVelocity(newPos);
		// controllablePhysicsComponent->SetPosition(newPos /*+ basePosition*/);
		// owner->SetDirty();
	}

	if (targetPos != DataTypes::Vector3::zero()) {
		auto targetDistance = Vector3::Distance(targetPos, controllablePhysicsComponent->GetPosition());
		if (targetDistance > 3.f) {
			/*auto velo_rate = 0.2f;
			auto wander_speed = 12.3f;
			auto turn_speed = M_PI / 4.0f;
			std::float_t notDone = controllablePhysicsComponent->MoveTowardsLocation(targetPos, wander_speed, velo_rate);
			if (notDone) {
				auto turnProgress = controllablePhysicsComponent->TurnTowardsLocation(targetPos, turn_speed, velo_rate);
			}*/
			/*auto clients = owner->GetZoneInstance()->sessionManager.GetClients();
			for (auto cl : clients) {
				auto pl = owner->GetZoneInstance()->objectsManager->GetObjectByID(cl.actorID);
				if (pl == nullptr) continue;
				break;
			}*/

			//SetTargetMovementPos(pl->GetPosition());
			controllablePhysicsComponent->LookAt(Vector3(targetPos.x, controllablePhysicsComponent->GetPosition().y, targetPos.z));

			// get max for factoring

			Vector3 localPos = controllablePhysicsComponent->GetPosition();
			Vector3 veloDir;
			veloDir.set(targetPos.x - localPos.x, targetPos.y - localPos.y, targetPos.z - localPos.z);


			auto maxdv = ((((((veloDir.x) > (veloDir.y)) ? (veloDir.x) : (veloDir.y))) > (veloDir.z)) ? ((((veloDir.x) > (veloDir.y)) ? (veloDir.x) : (veloDir.y))) : (veloDir.z)); // this is just a expanded version of this max(max(veloDir.x, veloDir.y), veloDir.z); (i am so sorry :P) 
			auto maxdf = 1 / maxdv;

			auto speed = wanderSpeed * 5;

			veloDir = veloDir * maxdf * speed;
			if (controllablePhysicsComponent->GetVelocity() != veloDir) {
				if (veloDir.x > speed || veloDir.y > speed || veloDir.z > speed) {
					veloDir = Vector3::zero();
				}
			}
			if (controllablePhysicsComponent->GetVelocity() != veloDir) {
				controllablePhysicsComponent->SetVelocity(veloDir);
			}
			
		}
		else {
			if(controllablePhysicsComponent->GetVelocity() != Vector3::zero())
				controllablePhysicsComponent->SetVelocity(Vector3::zero());
			targetPos = Vector3::zero();
		}
	}
}
