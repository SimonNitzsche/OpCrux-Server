#ifndef __ENTITY__GAMEOBJECT_HPP__
#define __ENTITY__GAMEOBJECT_HPP__

#include "RakNet/Replica.h"
#include "RakNet/MessageIdentifiers.h"
#include "Replica/ReplicaTypes.hpp"
#include "RakNet/BitStream.h"
#include "DataTypes/LWOOBJID.hpp"
#include <vector>
#include <unordered_map>

class EntityComponent;
class LWOTimer {};

class GameObject : Replica {
protected:
	DataTypes::LWOOBJID objectID;
	int32_t LOT;
	std::wstring name;
protected:
	int64_t creationTimestamp;
protected:
	GameObject * spawner = nullptr;
	uint32_t spawner_node = 0xFFFFFFFF;
protected:
	GameObject * parent = nullptr;
	std::vector<GameObject *> children;
protected:
	std::vector<EntityComponent*> components;
	std::unordered_map<char*, LWOTimer> timers;
/*----------------------------------------------------------*/
public:
	ReplicaReturnResult SendConstruction(RakNetTime currentTime, SystemAddress systemAddress, unsigned int &flags, RakNet::BitStream *outBitStream, bool *includeTimestamp);
	ReplicaReturnResult SendDestruction(RakNet::BitStream *outBitStream, SystemAddress systemAddress, bool *includeTimestamp);
	ReplicaReturnResult ReceiveDestruction(RakNet::BitStream *inBitStream, SystemAddress systemAddress, RakNetTime timestamp);
	ReplicaReturnResult SendScopeChange(bool inScope, RakNet::BitStream *outBitStream, RakNetTime currentTime, SystemAddress systemAddress, bool *includeTimestamp);
	ReplicaReturnResult ReceiveScopeChange(RakNet::BitStream *inBitStream, SystemAddress systemAddress, RakNetTime timestamp);
	ReplicaReturnResult Serialize(bool *sendTimestamp, RakNet::BitStream *outBitStream, RakNetTime lastSendTime, PacketPriority *priority, PacketReliability *reliability, RakNetTime currentTime, SystemAddress systemAddress, unsigned int &flags);
	ReplicaReturnResult Deserialize(RakNet::BitStream *inBitStream, RakNetTime timestamp, RakNetTime lastDeserializeTime, SystemAddress systemAddress);
	int GetSortPriority(void) const { return 0; }
/*----------------------------------------------------------*/
public:
	void Update();
	void Tick();
	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType);
	void Test() { objectID = 9ULL; LOT = 1234; name = L"TestName"; }
};

#endif // !__ENTITY__GAMEOBJECT_HPP__