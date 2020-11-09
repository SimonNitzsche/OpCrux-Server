#include "PhantomPhysicsComponent.hpp"
#include "ScriptComponent.hpp"

void PhantomPhysicsComponent::PhysicUpdate() {
	/*
		Since we don't have propper physic detection yet,
		we will currently use a sphere with the radius of 1 times scale,
		since the default collision object is a 2x2x2 box, if a hkx file is missing.
	*/


	// Have script attached?
	// If not discard
	ScriptComponent* scriptComp = owner->GetComponent<ScriptComponent>();
	if (scriptComp == nullptr) return;
	if (!scriptComp->bHasScriptLoaded()) return;

	// Cleanup removed objects
	for (int i = 0; i < enteredObjects.size(); ++i) {
		if (!enteredObjects.at(i)) {
			owner->OnOffCollisionPhantom(enteredObjects.at(i));
			enteredObjects.erase(enteredObjects.begin() + i);
		}
	}

	auto objects = this->owner->GetZoneInstance()->objectsManager->GetControllableObjects();

	for (auto o : objects) {
		// we can assume, the object has a controllable physics object, otherwise it can't move.
		ControllablePhysicsComponent* objectPhysicsComponent = o->GetComponent<ControllablePhysicsComponent>();
		if (!objectPhysicsComponent || objectPhysicsComponent == nullptr) continue;
		Vector3 pos = Vector3::zero();
		pos = objectPhysicsComponent->GetPosition();

		float difference = Vector3::Distance(pos, position);

		// Check if exists in list.
		bool isInside = std::find(enteredObjects.begin(), enteredObjects.end(), o) != enteredObjects.end();
		if (isInside) {
			// Object in list, check if left

			// Look for matching radii
			if (difference > owner->GetScale() * 2) {
				// Message object left.
				owner->OnOffCollisionPhantom(o);
				auto it2 = std::find(enteredObjects.begin(), enteredObjects.end(), o);
				if (it2 != enteredObjects.end())
					enteredObjects.erase(it2);
			}
		}
		else {
			// Object not in list, check if entered

			// Look for matching radii
			if (difference <= owner->GetScale() * 2) {
				// Message object joined.
				auto it2 = std::find(enteredObjects.begin(), enteredObjects.end(), o);
				if (it2 != enteredObjects.end()) continue;
				enteredObjects.push_back(o);
				// OnCollisionPhantom
				owner->OnCollisionPhantom(o);
			}
		}
	}
}