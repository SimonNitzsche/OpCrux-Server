#ifndef __ENTITY__GAMEOBJECT_HPP__
#define __ENTITY__GAMEOBJECT_HPP__

#include "RakNet/Replica.h"
#include "RakNet/MessageIdentifiers.h"
#include "Replica/ReplicaTypes.hpp"
#include "RakNet/BitStream.h"
#include "DataTypes/LWOOBJID.hpp"
#include <vector>
#include <unordered_map>

namespace Entity {

	/*
		Classes to be later defined.
	*/
	class EntityComponent;
	class LWOTimer {};

	/*
		Defines a Game Object in-game.
	*/
	class GameObject : Replica {
		protected:

			// This Object's Object ID.
			DataTypes::LWOOBJID objectID;

			// The LOT of this Object. (List in cdclient)
			int32_t LOT;

			// The Name of this Object.
			std::wstring name;

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
			std::vector<EntityComponent*> components;

			// The timers assigned to this object.
			std::unordered_map<char*, LWOTimer> timers;
			
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
				Called to update the object.
			*/
			void Update();

			/*
				Ticks the object.
			*/
			void Tick();

			/*
				Serializes the Object.
			*/
			void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType);

			/*
				Quick function to create the test object.
			*/
			void Test() { objectID = 9ULL; LOT = 1234; name = L"TestName"; }
	};
}

#endif // !__ENTITY__GAMEOBJECT_HPP__