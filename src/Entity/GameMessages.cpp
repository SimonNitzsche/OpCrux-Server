#include "GameMessages.hpp"

void GameMessages::Deserialize(WorldServer * Instance, ClientSession * session, RakNet::BitStream *bs) {
	// Read GM head
	DataTypes::LWOOBJID targetObjectID; bs->Read(targetObjectID);
	Enums::EGameMessageID msgID; bs->Read(msgID);

	Entity::GameObject * senderObject = Instance->objectsManager->GetObjectByID(session->actorID);
	Entity::GameObject * targetObject = Instance->objectsManager->GetObjectByID(targetObjectID);

	// Please keep it sorted by Name:
	switch (msgID) {
		GM_DESERIALIZE_SWITCH_CASE(GM::FireEventServerSide);
		GM_DESERIALIZE_SWITCH_CASE(GM::MissionDialogueOK);
		GM_DESERIALIZE_SWITCH_CASE(GM::PlayEmote);
		GM_DESERIALIZE_SWITCH_CASE(GM::PlayerLoaded);
		GM_DESERIALIZE_SWITCH_CASE(GM::RequestUse);
		GM_DESERIALIZE_SWITCH_CASE(GM::RequestSmashPlayer);
		GM_DESERIALIZE_SWITCH_CASE(GM::RequestResurrect);
		GM_DESERIALIZE_SWITCH_CASE(GM::StartSkill);
		GM_DESERIALIZE_SWITCH_CASE(GM::SyncSkill);
	default: {
			Logger::log("WRLD", "Tried to deserialize unhandled GM #" + std::to_string(msgID), LogType::WARN);
	}
	}
}