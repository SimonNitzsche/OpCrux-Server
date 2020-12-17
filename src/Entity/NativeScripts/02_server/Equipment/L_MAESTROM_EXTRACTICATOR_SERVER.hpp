#ifndef __ENTITY__NATIVESCRIPTS__02_SERVER__EQUIPMENT__L_MAESTROM_EXTRACTICATOR_SERVER_HPP__
#define __ENTITY__NATIVESCRIPTS__02_SERVER__EQUIPMENT__L_MAESTROM_EXTRACTICATOR_SERVER_HPP__

#include <string>
#include "Entity/GameObject.hpp"
#include "Entity/NativeScript.hpp"
#include "Entity/Components/DestructibleComponent.hpp";
#include <memory>

class NATIVESCRIPT__02SERVER__EQUIPMENT__L_MAESTROM_EXTRACTICATOR_SERVER : public NativeScript {

	const std::u16string failAnim = u"idle_maelstrom";			// Beginning opening anim
	const std::u16string collectAnim = u"collect_maelstrom";	// Animation to play when collected
	const std::int32_t defaultTime = 4;							// Min time for the collectAnim to play

	// table to find the mission ID based on the spawner network name
	const std::unordered_map<std::u16string, std::list<std::int32_t>> VisibilityObjectTable = { {u"MaelstromSamples", {1849, 1883}},
																								{u"MaelstromSamples2ndary1", {1883}},
																								{u"MaelstromSamples2ndary1", {1883}} };
public:

	//----------------------------------------------------------------
	//-- On beginning object instantiation, play the default animation
	//-- client side
	//----------------------------------------------------------------
	void onStartup(Entity::GameObject* self) {
		self->SetNetworkedVar(u"current_anim", failAnim);
	}

	//----------------------------------------------------------------
	//-- The client has verified a potential collection sample,
	//-- finalize the confirmation and collect if necessary
	//----------------------------------------------------------------
	void onFireEventServerSide(Entity::GameObject* self, GM::FireEventServerSide msg) {
		if (msg.args == u"attemptCollection") {
			// Get the name of the spawn network the attempt object is on
			std::u16string objSpawnerNom = self->GetZoneInstance()->objectsManager->GetObjectByID(msg.senderID)->GetVar(u"spawner_name").operator std::u16string();

			auto missionIDs = VisibilityObjectTable.find(objSpawnerNom);
			if (missionIDs != VisibilityObjectTable.end()) {
				// We have a valid associated mission list to process
				// Obtain a reference to the player
				Entity::GameObject* player = self->GetParentObject();

				std::list<std::int32_t> missionIDList = missionIDs->second;
				for (auto missionID = missionIDList.begin(); missionID != missionIDList.end(); ++missionID) {
					auto missionstate = Database::GetMission(player->GetZoneInstance()->GetDBConnection(), player->GetObjectID().getPureID(), *missionID).state;
					// Determine if the player is on the mission
					if (missionstate == 2 || missionstate == 10) {
						// The player is on the mission, collect the object
						
						auto senderID = self->GetZoneInstance()->objectsManager->GetObjectByID(msg.senderID);
						if(senderID != nullptr)
							CollectSample(self, senderID);
					}
				}
			}
		}
	}

	//----------------------------------------------------------------
	//-- Process sample collection events
	//----------------------------------------------------------------
	void CollectSample(Entity::GameObject* self, Entity::GameObject* sampleObj) {
		Entity::GameObject* player = self->GetParentObject();

		// Check if the parent exists
		if (player == nullptr) return;

		// Get the anim time and play the animation
		auto animTimer = playAnimAndReturnTime(self, collectAnim);

		// Play the collect anim
		self->GetZoneInstance()->timer.AddTimerWithCancelMs(std::int32_t(animTimer) * 1000, u"RemoveSample", self);
		// Destroy the sample
		{GM::RequestDie msg; msg.killerID = player->GetObjectID(); msg.lootOwnerID = player->GetObjectID(); sampleObj->GetComponent<DestructibleComponent>()->ForcefullyPerformRequestDie(self, msg); }
	}

	//----------------------------------------------------------------
	//-- Utility function that allows for object animation on the client
	//-- as well as returning the valid processed anim time
	//----------------------------------------------------------------
	std::float_t playAnimAndReturnTime(Entity::GameObject* self, std::u16string animID) {
		// Get the anim time
		auto animTimer = 4.0f;

		// If we have an animation play it
		if (animTimer > 0) {
			self->SetNetworkedVar(u"current_anim", animID);
		}

		// If the anim time is less than the the default time use default
		if (animTimer < defaultTime) {
			animTimer = defaultTime;
		}

		return animTimer;
	}

	//----------------------------------------------------------------
	//-- Catch and process timer events
	//----------------------------------------------------------------
	void onTimerDone(Entity::GameObject* self, GM::TimerDone msg) {
		if (msg.name == u"RemoveSample") {
			// Delete the object
			self->InstantiateRemoval();
		}
	}

};
#endif 