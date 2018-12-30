#include "GameObject.hpp"
#include "Utils/ServerInfo.hpp"
using namespace Entity;

ReplicaReturnResult GameObject::SendConstruction(RakNetTime currentTime, SystemAddress systemAddress, unsigned int &flags, RakNet::BitStream *outBitStream, bool *includeTimestamp) {
	return REPLICA_PROCESSING_DONE;
}
ReplicaReturnResult GameObject::SendDestruction(RakNet::BitStream *outBitStream, SystemAddress systemAddress, bool *includeTimestamp) {
	return REPLICA_PROCESSING_DONE;
}
ReplicaReturnResult GameObject::ReceiveDestruction(RakNet::BitStream *inBitStream, SystemAddress systemAddress, RakNetTime timestamp) {
	return REPLICA_PROCESSING_DONE;
}
ReplicaReturnResult GameObject::SendScopeChange(bool inScope, RakNet::BitStream *outBitStream, RakNetTime currentTime, SystemAddress systemAddress, bool *includeTimestamp) {
	return REPLICA_PROCESSING_DONE;
}
ReplicaReturnResult GameObject::ReceiveScopeChange(RakNet::BitStream *inBitStream, SystemAddress systemAddress, RakNetTime timestamp) {
	return REPLICA_PROCESSING_DONE;
}
ReplicaReturnResult GameObject::Serialize(bool *sendTimestamp, RakNet::BitStream *outBitStream, RakNetTime lastSendTime, PacketPriority *priority, PacketReliability *reliability, RakNetTime currentTime, SystemAddress systemAddress, unsigned int &flags) {
	return REPLICA_PROCESSING_DONE;
}
ReplicaReturnResult GameObject::Deserialize(RakNet::BitStream *inBitStream, RakNetTime timestamp, RakNetTime lastDeserializeTime, SystemAddress systemAddress) {
	return REPLICA_PROCESSING_DONE;
}

void GameObject::Update() {

}

void GameObject::Tick() {
	Update();
}

void GameObject::Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
	if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
		factory->Write(objectID);
		factory->Write(LOT);
		factory->Write(static_cast<uint8_t>(name.length())); {
			factory->Write(reinterpret_cast<const char*>(name.c_str()), name.length() * 2); }

		factory->Write(static_cast<uint32_t>(ServerInfo::uptime() - creationTimestamp));

		{
			factory->Write(false);
			// TODO! -> compressed LDF
		}

		factory->Write(false); // TODO! -> Trigger flag

		factory->Write(spawner != nullptr);
		if (spawner != nullptr) { factory->Write(spawner->objectID); }

		factory->Write(spawner_node != 0xFFFFFFFF);
		if (spawner_node != 0xFFFFFFFF) { factory->Write(spawner_node); }

		factory->Write(false);
		factory->Write(false);
	}


}