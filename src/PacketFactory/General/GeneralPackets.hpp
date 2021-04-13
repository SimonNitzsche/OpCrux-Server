#ifndef __PACKETFACTORY__GENERAL__GENERALPACKETS_HPP__
#define __PACKETFACTORY__GENERAL__GENERALPACKETS_HPP__

#include <memory>
#include <RakNet/BitStream.h>
#include <RakNet/Types.h>
#include <RakNet/RakNetTypes.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/RakPeerInterface.h>

#include "Common/HardConfig.hpp"

#include "Enums/EPackets.hpp"
#include "Structs/Networking/General/StructPacketHeader.hpp"
#include "Enums/ERemoteConnection.hpp"
#include "Enums/EDisconnectReason.hpp"
using namespace Enums;

#include "Utils/ServerInfo.hpp"

namespace PacketFactory {

	namespace General {

		inline void doHandshake(RakPeerInterface * rakServer, SystemAddress client, bool isAuth) {
			RakNet::BitStream returnBS;
			// Head
			LUPacketHeader returnBSHead;
			returnBSHead.protocolID = static_cast<std::uint8_t>(ID_USER_PACKET_ENUM);
			returnBSHead.remoteType = static_cast<std::uint16_t>(ERemoteConnection::GENERAL);
			returnBSHead.packetID = static_cast<std::uint32_t>(EServerPacketID::VERSION_CONFIRM);
			returnBS.Write(returnBSHead);
			//Data
			returnBS.Write<std::uint32_t>(SERVER_VERSION); // version
			returnBS.Write<std::uint32_t>(0x93); // ???
			returnBS.Write<std::uint32_t>(isAuth ? 1 : 4 ); // connType
			returnBS.Write(ServerInfo::processID);
			returnBS.Write<std::uint16_t>(0xff);
			returnBS.Write(ServerInfo::GetAuthIP().c_str(), 264);

			rakServer->Send(&returnBS, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, client, false);
		};

		inline void doDisconnect(RakPeerInterface * rakServer, SystemAddress client, EDisconnectReason reason) {
			RakNet::BitStream returnBS;
			// Head
			LUPacketHeader returnBSHead{};
			returnBSHead.protocolID = static_cast<std::uint8_t>(ID_USER_PACKET_ENUM);
			returnBSHead.remoteType = static_cast<std::uint16_t>(ERemoteConnection::GENERAL);
			returnBSHead.packetID = static_cast<std::uint32_t>(EServerPacketID::DISCONNECT_NOTIFY);
			returnBS.Write(returnBSHead);
			//Data
			returnBS.Write(reason);

			std::string disconnectString = "Unknown reason.";

			switch (int(reason)) {
			case 4: disconnectString = "Account signed in from another location"; break;
			case 5: disconnectString = "Server Shutdown"; break;
			case 6: disconnectString = "Server unable to load map"; break;
			case 7: disconnectString = "Invalid Session Key"; break;
			case 8: disconnectString = "Account Was Not In Pending List"; break;
			case 9: disconnectString = "Character not found"; break;
			case 10: disconnectString = "Character corruption"; break;
			}

			Logger::log("WRLD", "Disconnected player: " + disconnectString, LogType::WARN);

			rakServer->Send(&returnBS, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, client, false);
		}

	};

};

#endif // !__PACKETFACTORY__AUTH__AUTHPACKETS_HPP__
