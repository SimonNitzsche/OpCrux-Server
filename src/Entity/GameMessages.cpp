#include "GameMessages.hpp"

using namespace GM;

void GameMessages::Deserialize(WorldServer * Instance, ClientSession * session, RakNet::BitStream *bs) {
	// Read GM head
	DataTypes::LWOOBJID targetObjectID; bs->Read(targetObjectID);
	Enums::EGameMessageID msgID; bs->Read(msgID);

	Entity::GameObject * senderObject = Instance->objectsManager->GetObjectByID(session->actorID);
	Entity::GameObject * targetObject = Instance->objectsManager->GetObjectByID(targetObjectID);

	// Only put GMs sent by the client below
	// Please keep it sorted by Name:
	switch (msgID) {
		GM_MAKE_LIST_CLIENT(GM_DESERIALIZE_SWITCH_CASE);
	default: {
		std::stringstream ss; ss << std::hex << msgID;
		Logger::log("WRLD", "Tried to deserialize unhandled GM #" + std::to_string(msgID) + ", 0x" + ss.str(), LogType::WARN);
	}
	}
}