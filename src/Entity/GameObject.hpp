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

			// The Children of this object.
			std::vector<GameObject *> children;

			// The Components assigned to this object.
			std::unordered_map<std::uint32_t, IEntityComponent*> components;

			// The timers assigned to this object.
			std::unordered_map<char*, LWOTimer> timers;
			
			// Needs serialization
			bool objectDirty = false;

			WorldServer * Instance;

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
			
		public:
			/*
				Constructor
			*/
			GameObject(WorldServer * instance, std::uint32_t LOT);

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
				Sets the object ID
			*/
			void SetObjectID(DataTypes::LWOOBJID ID);

			/*
				Gets the object ID
			*/
			DataTypes::LWOOBJID GetObjectID();

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
				Quick function to create the test object.
			*/
			void Test() { objectID = 9ULL; LOT = 1234; name = L"TestName"; }
	};
}

#endif // !__ENTITY__GAMEOBJECT_HPP__
