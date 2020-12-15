#ifndef __ENTITY__NATIVESCRIPTS__AI__AG__L_AG_IMAG_SMASHABLE_HPP__
#define __ENTITY__NATIVESCRIPTS__AI__AG__L_AG_IMAG_SMASHABLE_HPP__


#include <string>
#include "Entity/GameObject.hpp"
#include "Entity/NativeScript.hpp"
#include "Entity/NativeScripts/OMIS.hpp"

class NATIVESCRIPT__AI__AG__L_AG_IMAG_SMASHABLE : public NativeScript, public NATIVESCRIPT__OMIS {
public:
	
	void onDie(Entity::GameObject* self, GM::Die msg) {
		
		std::int32_t BobMissionStatus = Database::HasMission(self->GetZoneInstance()->GetDBConnection(), msg.killerID.getPureID(), 173) ? Database::GetMission(self->GetZoneInstance()->GetDBConnection(), msg.killerID.getPureID(), 173).state : 0;

		// If the spaceman bob mission is complete.
		if (BobMissionStatus >= 8) {
			srand(::time(0));
			std::int32_t amount = rand() % 2 + 1;

			for (int i = 0; i < amount; ++i) {
				DataTypes::LWOOBJID newID = self->GetZoneInstance()->objectsManager->GenerateSpawnedID();
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
		std::int32_t funnychance = rand() % 25 + 0;// Random::get<std::int32_t>(0, 25);

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
					delete spawnedObject;
					spawnedObject = new Entity::GameObject(Instance, 1845);

				}

				// Set ObjectID
				spawnedObject->SetObjectID(self->GetZoneInstance()->objectsManager->GenerateSpawnedID());
				//spawnedObject->SetObjectID(DataTypes::LWOOBJID(288334496658198694ULL + Instance->spawnedObjectIDCounter++));

				LDFCollection config = {
					LDF_COLLECTION_INIT_ENTRY(u"is_smashable", false)
				};
				spawnedObject->PopulateFromLDF(&config);

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