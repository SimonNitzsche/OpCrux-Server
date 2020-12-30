#ifndef __ENTITY__NATIVESCRIPTS__ScriptComponent_1088_script_name__removed_HPP__
#define __ENTITY__NATIVESCRIPTS__ScriptComponent_1088_script_name__removed_HPP__
#include "Entity/NativeScript.hpp"

/*
	LU MAILBOX Server -> 3964
*/
class NATIVESCRIPT__ScriptComponent_1088_script_name__removed : public NativeScript {

	void onUse(Entity::GameObject* self, GM::RequestUse msg) {
		RakNet::BitStream bs = RakNet::BitStream();
		LUPacketHeader returnBSHead;
		returnBSHead.protocolID = static_cast<uint8_t>(ID_USER_PACKET_ENUM);
		returnBSHead.remoteType = static_cast<uint16_t>(Enums::ERemoteConnection::CLIENT);
		returnBSHead.packetID = static_cast<uint32_t>(Enums::EClientPacketID::SERVER_GAME_MSG);
		bs.Write(returnBSHead);
		bs.Write<std::uint64_t>(msg.user->GetObjectID());
		bs.Write<std::uint16_t>(1184);
		bs.Write<std::uint8_t>(9);
		bs.Write<std::uint8_t>(1);
		bs.Write<std::uint8_t>((5 << 1) + 1);
		bs.Write("state", 5);

		bs.Write<std::uint8_t>(6);
		bs.Write<std::uint8_t>((4 << 1) + 1);
		bs.Write("Mail", 4);

		bs.Write<std::uint8_t>(1);
		StringUtils::writeStringToBitStream<std::uint32_t>(&bs, "pushGameState");

		auto csess = self->GetZoneInstance()->sessionManager.GetSession(msg.user->GetObjectID());
		if (csess != nullptr)
			self->GetZoneInstance()->rakServer->Send(&bs, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, csess->systemAddress, false);
		
	};

	void onFireEventServerSide(Entity::GameObject* self, GM::FireEventServerSide msg) {
		
		if (msg.args != u"toggleMail") return;

		RakNet::BitStream bs = RakNet::BitStream();
		LUPacketHeader returnBSHead;
		returnBSHead.protocolID = static_cast<uint8_t>(ID_USER_PACKET_ENUM);
		returnBSHead.remoteType = static_cast<uint16_t>(Enums::ERemoteConnection::CLIENT);
		returnBSHead.packetID = static_cast<uint32_t>(Enums::EClientPacketID::SERVER_GAME_MSG);
		bs.Write(returnBSHead);
		bs.Write<std::uint64_t>(msg.senderID);
		bs.Write<std::uint16_t>(1184);
		bs.Write<std::uint8_t>(9);
		bs.Write<std::uint8_t>(1);
		bs.Write<std::uint8_t>((7 << 1) + 1);
		bs.Write("visible", 7);
		bs.Write<std::uint8_t>(2);
		bs.Write<std::uint8_t>(1);
		StringUtils::writeStringToBitStream<std::uint32_t>(&bs, "ToggleMail");

		auto csess = self->GetZoneInstance()->sessionManager.GetSession(msg.senderID);
		if (csess != nullptr)
			self->GetZoneInstance()->rakServer->Send(&bs, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, csess->systemAddress, false);

	}
};

#endif