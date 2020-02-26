#ifndef __ENTITY__NATIVESCRIPTS__AI__AG__L_AG_IMAG_SMASHABLE_HPP__
#define __ENTITY__NATIVESCRIPTS__AI__AG__L_AG_IMAG_SMASHABLE_HPP__

#include <string>
#include "Entity/GameObject.hpp"
#include "Entity/NativeScript.hpp"
#include "Entity/NativeScripts/OMIS.hpp"

class NATIVESCRIPT__AI__AG__L_AG_IMAG_SMASHABLE : public NativeScript, public NATIVESCRIPT__OMIS {
public:
	
	void onDie(Entity::GameObject* self, GM::Die msg) {
		
		std::int32_t BobMissionStatus = Database::HasMission(msg.killerID.getPureID(), 173) ? Database::GetMission(msg.killerID.getPureID(), 173).state : 0;

		// If the spaceman bob mission is complete.
		if (BobMissionStatus >= 8) {
			srand(::time(0));
			std::int32_t amount = rand() % 2 + 1;

			for (int i = 0; i < amount; ++i) {
				LWOOBJID newID = DataTypes::LWOOBJID((1ULL << 58) + 104120439353844ULL + self->GetZoneInstance()->spawnedObjectIDCounter++);
				{
					GM::DropClientLoot nmsg; nmsg.itemTemplate = 935; nmsg.owner = msg.killerID; nmsg.lootID = newID; nmsg.sourceObj = self->GetObjectID();
					GameMessages::Send(self->GetZoneInstance()->objectsManager->GetObjectByID(msg.killerID), self->GetObjectID(), nmsg);
				}
			}
		}
		crateAnimal(self);
	}


	void crateAnimal(Entity::GameObject * self) {
		DataTypes::Vector3 mypos = self->GetPosition();

		// How often do we spawn weird creatures
		srand(::time(0));
		std::int32_t funnychance = rand() % 25 + 0;

		// What kind of creatures do we want? Will be expanded in the future.
		std::int32_t animaltype = 1;

		if (funnychance == 1) {
			if (animaltype == 1) {
				// Spawn the Crate Chicken

				WorldServer* Instance = self->GetZoneInstance();

				// Create
				Entity::GameObject* spawnedObject = new Entity::GameObject(Instance, 8114);


				if (!spawnedObject->isSerializable) {
					// Spawn Error Object
					delete[] spawnedObject;
					spawnedObject = new Entity::GameObject(Instance, 1845);

				}

				// Set ObjectID
				spawnedObject->SetObjectID(DataTypes::LWOOBJID((1ULL << 58) + 104120439353844ULL + Instance->spawnedObjectIDCounter++));
				//spawnedObject->SetObjectID(DataTypes::LWOOBJID(288334496658198694ULL + Instance->spawnedObjectIDCounter++));

				// Set Position/Rotation
				spawnedObject->SetPosition(mypos);
				
				spawnedObject->Finish();

				// Register
				Instance->objectsManager->RegisterObject(spawnedObject);

				// Construct
				if (spawnedObject->isSerializable)
					Instance->objectsManager->Construct(spawnedObject);
			}
		}
	}

};
#endif 