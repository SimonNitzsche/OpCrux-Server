#include "GameMessages.hpp"

void GameMessages::Deserialize(WorldServer * Instance, ClientSession * session, RakNet::BitStream *bs) {
	// Read GM head
	DataTypes::LWOOBJID targetObjectID; bs->Read(targetObjectID);
	Enums::EGameMessageID msgID; bs->Read(msgID);

	Entity::GameObject * senderObject = Instance->objectsManager->GetObjectByID(session->actorID);
	Entity::GameObject * targetObject = Instance->objectsManager->GetObjectByID(targetObjectID);

	// Please keep it sorted by ID:
	switch (msgID) {
		GM_DESERIALIZE_SWITCH_CASE(GM::PlayEmote);
		GM_DESERIALIZE_SWITCH_CASE(GM::RequestUse);
		GM_DESERIALIZE_SWITCH_CASE(GM::RequestSmashPlayer);
		GM_DESERIALIZE_SWITCH_CASE(GM::RequestResurrect);
	default: {
	}
	}
}