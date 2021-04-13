#include "RocketLaunchComponent.hpp"
#include "Utils/LDFUtils.hpp"
#include "Entity/GameMessages.hpp"
#include "Entity/Components/InventoryComponent.hpp"

void AMF3U29(RakNet::BitStream* bs, std::uint32_t number) {
	number = number & 0x1FFFFFFF;
	/*while (number != 0) {
		std::uint8_t part = number & 0b01111111;
		if ((number & 0b10000000) != 0) {
			part |= 0b10000000;
		}
		bs->Write(part);
		number = number >> 7;
	}*/

	std::uint8_t stretched[4];
	std::uint8_t last = 0;
	for (std::uint8_t i = 0; i < 4; ++i) {
		stretched[i] = (number >> (7 * 3-i)) & 0xFF;
		if (stretched != 0x00) last = i;
	}

	for (int i = 0; i < last; ++i) {
		stretched[i] |= 0b10000000;
		bs->Write(stretched[i]);
	}

}

void RocketLaunchComponent::OnRequestUse(Entity::GameObject* sender, GM::RequestUse * msg) {
#ifndef _DEBUG // We don't want auto complete when working on it 
	if (this->targetZone == 1150) {
		RakNet::BitStream bs = RakNet::BitStream();
		LUPacketHeader returnBSHead;
		returnBSHead.protocolID = static_cast<uint8_t>(ID_USER_PACKET_ENUM);
		returnBSHead.remoteType = static_cast<uint16_t>(Enums::ERemoteConnection::CLIENT);
		returnBSHead.packetID = static_cast<uint32_t>(Enums::EClientPacketID::SERVER_GAME_MSG);
		bs.Write(returnBSHead);
		bs.Write<std::uint64_t>(sender->GetObjectID());
		bs.Write<std::uint16_t>(1184);
		bs.Write<std::uint8_t>(9);
		bs.Write<std::uint8_t>(1);
		bs.Write<std::uint8_t>((7 << 1) + 1);
		bs.Write("message", 7);



		std::string message = "The spider queen is currently not ready. We will complete some missions for you in the meantime...";
		auto mlen = (message.length() << 1) | 1;
		bs.Write<std::uint8_t>(6);
		bs.Write<std::uint8_t>(((mlen >> 7) & 0x7F) | 0x80);
		bs.Write<std::uint8_t>(mlen & 0x7F);
		bs.Write(message.c_str(), 98);

		bs.Write<std::uint8_t>((5 << 1) + 1);
		bs.Write("title", 5);

		std::string title = "Hold on!";
		bs.Write<std::uint8_t>(6);
		bs.Write<std::uint8_t>((8 << 1) + 1);
		bs.Write(title.c_str(), title.length());

		bs.Write<std::uint8_t>((7 << 1) + 1);
		bs.Write("visible", 7);

		bs.Write<std::uint8_t>(3);
		bs.Write<std::uint8_t>(1);
		StringUtils::writeStringToBitStream<std::uint32_t>(&bs, "ToggleAnnounce");

		auto csess = sender->GetZoneInstance()->sessionManager.GetSession(sender->GetObjectID());
		if(csess != nullptr)
			sender->GetZoneInstance()->rakServer->Send(&bs, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, csess->systemAddress, false);

		// Complete missions

		std::list<std::int32_t> missionsComplete = {
			377,
			1950,
			768,
			870,
			871,
			891,
			320
		};

		for (auto missionToComplete : missionsComplete) {
			DatabaseModels::MissionModel mis;
			if (Database::HasMission(sender->GetZoneInstance()->GetDBConnection(), sender->GetObjectID().getPureID(), missionToComplete)) {
				mis = Database::GetMission(sender->GetZoneInstance()->GetDBConnection(), sender->GetObjectID().getPureID(), missionToComplete);
			}
			else {
				mis = Database::AddMission(sender->GetZoneInstance()->GetDBConnection(), sender->GetObjectID().getPureID(), missionToComplete);
			}

			if (mis.state == 8) continue;

			mis.state = 8;

			GM::NotifyMission msg;
			msg.missionID = mis.missionID;
			msg.sendingRewards = false;
			msg.missionState = 2;
			GameMessages::Send(sender, sender->GetObjectID(), msg);
			msg.missionState = 4;
			GameMessages::Send(sender, sender->GetObjectID(), msg);

			msg.missionState = 0;
			msg.sendingRewards = true;
			GameMessages::Send(sender, sender->GetObjectID(), msg);


			// Send rewards
			Database::UpdateMission(sender->GetZoneInstance()->GetDBConnection(), mis);
			MissionManager::SendMissionRewards(sender, mis);

			msg.missionState = mis.state;
			msg.sendingRewards = false;
			GameMessages::Send(owner->GetZoneInstance(), owner->GetZoneInstance()->sessionManager.GetSession(sender->GetObjectID())->systemAddress, sender->GetObjectID(), msg);
		}

		// Then offer 320

		return;
	}
#endif


	GM::FireEventClientSide fecs;
	fecs.args = u"RocketEquipped";
	fecs.senderID = sender->GetObjectID();
	InventoryComponent* invComp = sender->GetComponent<InventoryComponent>();
	fecs.object = invComp->GetItem(6416).objectID;

	GameMessages::Broadcast(owner, fecs);


	GM::TransferToZone ttz;
	ttz.bCheckTransferAllowed = false;
	ttz.spawnPoint = targetScene;
	ttz.zoneID = targetZone;

	// GameMessages::Send(sender, sender->GetObjectID(), ttz);

	GM::TransferToZoneCheckedIM ttzcim;
	ttzcim.bIsThereaQueue = false;
	ttzcim.spawnPoint = targetScene;
	ttzcim.zoneID = targetZone;

	// GameMessages::Send(sender, sender->GetObjectID(), ttzcim);

}

void RocketLaunchComponent::OnFireEventServerSide(Entity::GameObject* sender, GM::FireEventServerSide * msg) {
	if (msg->args == u"ZonePlayer") {
		Entity::GameObject* senderObject = sender->GetZoneInstance()->objectsManager->GetObjectByID(msg->senderID);

		if (senderObject == nullptr) return;

		ClientSession* clSession = sender->GetZoneInstance()->sessionManager.GetSession(sender->GetObjectID());

		if (clSession == nullptr) return;

		DataTypes::ZoneInfo zi;
		zi.zoneID = msg->param3;
		masterServerBridge->ClientRequestMapChange(*clSession, zi);
	}
}

void RocketLaunchComponent::PopulateFromLDF(LDFCollection* collection) {
	LDF_GET_VAL_FROM_COLLECTION(targetScene, collection, u"targetScene", u"")
	LDF_GET_VAL_FROM_COLLECTION(targetZone, collection, u"targetZone", 1100)
	LDF_GET_VAL_FROM_COLLECTION(transferZoneID, collection, u"transferZoneID", u"")
}

void RocketLaunchComponent::RegisterMessageHandlers() {
	REGISTER_OBJECT_MESSAGE_HANDLER(RocketLaunchComponent, GM::RequestUse, OnRequestUse);
	REGISTER_OBJECT_MESSAGE_HANDLER(RocketLaunchComponent, GM::FireEventServerSide, OnFireEventServerSide);
}