#ifndef __PACKETFACTORY__CHAT__CHATPACKETS_HPP__
#define __PACKETFACTORY__CHAT__CHATPACKETS_HPP__

#include <memory>
#include <RakNet/BitStream.h>
#include <RakNet/Types.h>
#include <RakNet/RakNetTypes.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/RakPeerInterface.h>


#include "Enums/EPackets.hpp"
#include "Structs/Networking/General/StructPacketHeader.hpp"
#include "Enums/ERemoteConnection.hpp"
#include "Utils/ServerInfo.hpp"

#include "Enums/EPackets.hpp"
#include "NetworkDataTypes/ByteBool.hpp"
#include "Database/Database.hpp"
#include "DataTypes/LWOOBJID.hpp"
#include "DataTypes/LDF.hpp"

#include "Entity/GameObject.hpp"


using namespace Enums;

namespace PacketFactory {

	namespace Chat {
		inline void SendChatMessage(Entity::GameObject* sender, char chatChannel, std::u16string message) {
			if (sender == nullptr) return;

			RakNet::BitStream returnBS;

			// Head
			LUPacketHeader returnBSHead;
			returnBSHead.protocolID = static_cast<uint8_t>(ID_USER_PACKET_ENUM);
			returnBSHead.remoteType = static_cast<uint16_t>(ERemoteConnection::CHAT);
			returnBSHead.packetID = static_cast<std::uint32_t>(EChatPacketID::GENERAL_CHAT_MESSAGE);
			returnBS.Write(returnBSHead);

			returnBS.Write<std::uint64_t>(0ULL);
			returnBS.Write(chatChannel);

			returnBS.Write<std::uint32_t>(message.size());

			if (sender->GetObjectID() != 0x3FFFFFFFFFFE && chatChannel != 0x00) {
				StringUtils::writeBufferedWStringToBitStream(&returnBS, sender->GetName());
				returnBS.Write<std::uint64_t>(sender->GetObjectID());
				returnBS.Write<std::uint16_t>(0ULL);
				returnBS.Write<std::uint8_t>(0);
			}
			else {
				StringUtils::writeBufferedWStringToBitStream(&returnBS, u"");
				returnBS.Write<std::uint64_t>(0ULL);
				returnBS.Write<std::uint16_t>(0ULL);
				returnBS.Write<std::uint8_t>(0);
			}

			StringUtils::writeBufferedWStringToBitStream(&returnBS, message, message.size() + 1);
			auto clients = sender->GetZoneInstance()->sessionManager.GetClients();
			for (auto & client : clients)
				if(chatChannel != 0x00 || client.actorID == sender->GetObjectID())
					sender->GetZoneInstance()->rakServer->Send(&returnBS, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, client.systemAddress, false);
		}

		inline void SendPrivateChatMessage(Entity::GameObject* sender, Entity::GameObject* target, const std::u16string& message) {
			RakNet::BitStream returnBS;

			// Head
			LUPacketHeader returnBSHead{};
			returnBSHead.protocolID = static_cast<std::uint8_t>(ID_USER_PACKET_ENUM);
			returnBSHead.remoteType = static_cast<std::uint16_t>(ERemoteConnection::CHAT);
			returnBSHead.packetID = static_cast<std::uint32_t>(EChatPacketID::PRIVATE_CHAT_MESSAGE);
			returnBS.Write(returnBSHead);

			returnBS.Write<std::uint64_t>(0ULL);
			returnBS.Write<std::uint8_t>(0x07);

			returnBS.Write<std::uint32_t>(message.size());

			if (sender != nullptr) {
				StringUtils::writeBufferedWStringToBitStream(&returnBS, target->GetName()); // Sender Name
				returnBS.Write<std::uint64_t>(target->GetObjectID()); // Sender OBJID
				returnBS.Write<std::uint16_t>(0ULL); // ???

				// Is sender Mythran
				returnBS.Write<std::uint8_t>(Database::GetAccountGMLevel(sender->GetZoneInstance()->GetDBConnection(), Database::GetAccountIDFromMinifigOBJID(sender->GetZoneInstance()->GetDBConnection(), sender->GetObjectID())) >= 5);

				StringUtils::writeBufferedWStringToBitStream(&returnBS, sender->GetName()); // Senders Name
				returnBS.Write<std::uint8_t>(Database::GetAccountGMLevel(sender->GetZoneInstance()->GetDBConnection(), Database::GetAccountIDFromMinifigOBJID(sender->GetZoneInstance()->GetDBConnection(), target->GetObjectID())) >= 5); // Is Sender 
			
				returnBS.Write<std::uint8_t>(0);
			}
			else {
				StringUtils::writeBufferedWStringToBitStream(&returnBS, u"");
				returnBS.Write<std::uint64_t>(0ULL);
				returnBS.Write<std::uint16_t>(0ULL);
				returnBS.Write<std::uint8_t>(false);
				StringUtils::writeBufferedWStringToBitStream(&returnBS, u"");
				returnBS.Write<std::uint8_t>(Database::GetAccountGMLevel(sender->GetZoneInstance()->GetDBConnection(), Database::GetAccountIDFromMinifigOBJID(sender->GetZoneInstance()->GetDBConnection(), target->GetObjectID())) >= 5);
				
				returnBS.Write<std::uint8_t>(0);
			}

			StringUtils::writeBufferedWStringToBitStream(&returnBS, message, message.size() + 1);
			ClientSession* targetsession = target->GetZoneInstance()->sessionManager.GetSession(target->GetObjectID());

			target->GetZoneInstance()->rakServer->Send(&returnBS, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, targetsession->systemAddress, false);
		}

		inline void StringCheck(Entity::GameObject* target, uint8_t chatMode, uint8_t chatChannel, const std::u16string& message) {

			LUPacketHeader responseHead{};
			responseHead.protocolID = 0x53;
			responseHead.remoteType = 0x05;
			responseHead.packetID = 0x3b;
			RakNet::BitStream response;
			response.Write(responseHead);

			bool messageIsApproved = true;

			if (messageIsApproved) {
				response.Write<std::uint8_t>(1);
				response.Write<std::uint16_t>(0);
				response.Write(chatChannel);
			}
			else {
				response.Write<std::uint8_t>(0);
				response.Write<std::uint16_t>(0);
				response.Write(chatChannel);
			}

			ClientSession* targetsession = target->GetZoneInstance()->sessionManager.GetSession(target->GetObjectID());
			target->GetZoneInstance()->rakServer->Send(&response, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, targetsession->systemAddress, false);
		}

		inline void FriendRequest(Entity::GameObject* target, Entity::GameObject* sender, bool isBestFriendRequest) {
			RakNet::BitStream bs;

			LUPacketHeader responseHead{};
			responseHead.protocolID = 0x53;
			responseHead.remoteType = 0x05;
			responseHead.packetID = 0x1b;

			bs.Write(responseHead);

			StringUtils::writeBufferedWStringToBitStream(&bs, sender->GetName());

			bs.Write<std::uint8_t>(isBestFriendRequest);

			ClientSession* targetsession = target->GetZoneInstance()->sessionManager.GetSession(target->GetObjectID());
			target->GetZoneInstance()->rakServer->Send(&bs, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, targetsession->systemAddress, false);
		}

		inline void FriendRequestResponse() {

		}

		inline void RemoveFriendResponse() {

		}
	};
};

#endif // !__PACKETFACTORY__CHAT__CHATPACKETS_HPP__
