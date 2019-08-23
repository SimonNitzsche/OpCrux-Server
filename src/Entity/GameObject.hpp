#ifndef __ENTITY__GAMEOBJECT_HPP__
#define __ENTITY__GAMEOBJECT_HPP__

#include "RakNet/Replica.h"
#include "RakNet/MessageIdentifiers.h"
#include "Replica/ReplicaTypes.hpp"
#include "RakNet/BitStream.h"
#include "DataTypes/LWOOBJID.hpp"
#include <vector>
#include <unordered_map>

#include "Entity/Components/Interface/IEntityComponent.hpp"
//#include "Server/WorldServer.hpp"
class WorldServer;

namespace Entity {


	/*
		Classes to be later defined.
	*/
	class LWOTimer {};

	/*
		Defines a Game Object in-game.
	*/
	class GameObject : public Replica {
		private:

			// This Object's Object ID.
			DataTypes::LWOOBJID objectID;

			// The LOT of this Object. (List in cdclient)
			int32_t LOT;

			// The Name of this Object.
			std::wstring name = L"";

			// The Timestamp this object was created on the server.
			int64_t creationTimestamp;

			// The Spawner GameObject.
			GameObject * spawner = nullptr;

			// The ID of a spawner node.
			uint32_t spawner_node = 0xFFFFFFFF;

			// The Parent Object of this Object.
			GameObject * parent = nullptr;

			// The scale of the object
			float scale = 1.0f;

			// The Children of this object.
			std::vector<GameObject *> children;

			// The Components assigned to this object.
			std::unordered_map<std::uint32_t, IEntityComponent*> components;

			// The timers assigned to this object.
			std::unordered_map<char*, LWOTimer> timers;
			
			// Needs serialization in general.
			bool objectDirty = false;

			// Needs serialization
			bool baseDataDirty = false;

			WorldServer * Instance;

			LDFCollection configData;

		public:

			/*
				Replica Manager Overrided functions.
			*/
			ReplicaReturnResult SendConstruction(RakNetTime currentTime, SystemAddress systemAddress, unsigned int &flags, RakNet::BitStream *outBitStream, bool *includeTimestamp);
			ReplicaReturnResult SendDestruction(RakNet::BitStream *outBitStream, SystemAddress systemAddress, bool *includeTimestamp);
			ReplicaReturnResult ReceiveDestruction(RakNet::BitStream *inBitStream, SystemAddress systemAddress, RakNetTime timestamp);
			ReplicaReturnResult SendScopeChange(bool inScope, RakNet::BitStream *outBitStream, RakNetTime currentTime, SystemAddress systemAddress, bool *includeTimestamp);
			ReplicaReturnResult ReceiveScopeChange(RakNet::BitStream *inBitStream, SystemAddress systemAddress, RakNetTime timestamp);
			ReplicaReturnResult Serialize(bool *sendTimestamp, RakNet::BitStream *outBitStream, RakNetTime lastSendTime, PacketPriority *priority, PacketReliability *reliability, RakNetTime currentTime, SystemAddress systemAddress, unsigned int &flags);
			ReplicaReturnResult Deserialize(RakNet::BitStream *inBitStream, RakNetTime timestamp, RakNetTime lastDeserializeTime, SystemAddress systemAddress);
			int GetSortPriority(void) const { return 0; }

			// Is component serializable
			bool isSerializable = true;
			
		public:
			/*
				Constructor
			*/
			GameObject(WorldServer * instance, std::uint32_t LOT);

			/*
				Use this to finish up object creation.
			*/
			void Finish();

			/*
				Destructor
			*/
			~GameObject();

			/*
				Sets name
			*/
			void SetName(std::wstring n) {
				name = n;
			}

			/*
				Gets name
			*/
			std::wstring GetName() {
				return name;
			}

			/*
				Sets the object ID
			*/
			void SetObjectID(DataTypes::LWOOBJID ID);

			/*
				Gets the object ID
			*/
			DataTypes::LWOOBJID GetObjectID();

			/*
				Sets the scale
			*/
			void SetScale(float scale) {
				this->scale = scale;
			}

			/*
				Gets the scale
			*/
			float GetScale() {
				return this->scale;
			}

			/*
				Gets the object LOT
			*/
			std::uint32_t GetLOT() {
				return LOT;
			}

			/*
				Called to update the object.
			*/
			void Update();

			/*
				Called to update physic
			*/
			void PhysicUpdate();

			/*
				Ticks the object.
			*/
			void Tick();

			/*
				Returns an component by ID
			*/
			IEntityComponent * GetComponentByID(int id);

			/*
				Adds an component by ID
			*/
			void AddComponentByID(int id);

			/*
				Serializes the Object.
			*/
			void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType);

			/*
				Serializes the components of the object.
			*/
			void SerializeComponents(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType);

			/*
				Serializes the base data of the object.
			*/
			void SerializeBaseData(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType);

			/*
				Adds a child to the gameobject and tell the child it's added.
			*/
			void AddChild(GameObject * child);

			/*
				Sets the parent object
			*/
			void SetParent(GameObject * parent);

			/*
				Sets the spawner object
			*/
			void SetSpawner(GameObject * spawner,std::uint32_t spawnerNodeID);

			/*
				Only use this on a player.
			*/
			std::string GenerateXML();

			/*
				Sets the object dirty.
			*/
			void SetDirty() {
				objectDirty = true;
			}

			/*
				Checks if object dirty
			*/
			bool IsObjectDirty() {
				return objectDirty;
			}

			/*
				Populates object variables from LDF
			*/
			void PopulateFromLDF(LDFCollection * collection);

			/*
				Returns the instance
			*/
			WorldServer * GetZoneInstance() {
				return Instance;
			}

			/*
				Quick function to create the test object.
			*/
			void Test() { objectID = 9ULL; LOT = 1234; name = L"TestName"; }
		public:
			// Script Stuff
			LDFEntry GetVar(std::wstring key) {
				return configData.at(key);
			}

			template<typename T>
			void SetVar(std::wstring key, T data) {
				auto it = configData.find(key);
				if (it != configData.end()) {
					it->second.Delete();
					it->second = LDFEntry(key, data);
				}
				else {
					configData.insert({ key, LDFEntry(key, data) });
				}
			}

			void SetProximityRadius(std::string name, float radius);
			void PlayNDAudioEmitter(std::string guid);
	};
}

#endif // !__ENTITY__GAMEOBJECT_HPP__
