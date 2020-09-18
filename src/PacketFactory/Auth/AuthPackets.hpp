#ifndef __PACKETFACTORY__AUTH__AUTHPACKETS_HPP__
#define __PACKETFACTORY__AUTH__AUTHPACKETS_HPP__

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
#include "Utils/IPUtils.hpp"

#include "Enums/ELoginReturnCode.hpp"
#include "NetworkDataTypes/ByteBool.hpp"

namespace PacketFactory {

	namespace Auth {

		inline void doLoginResponse(RakPeerInterface * rakServer, SystemAddress client, ELoginReturnCode reason, SystemAddress wsAddr = UNASSIGNED_SYSTEM_ADDRESS, std::u16string customErrorMessage=u"") {
			RakNet::BitStream returnBS;
			// Head
			LUPacketHeader returnBSHead;
			returnBSHead.protocolID = static_cast<uint8_t>(ID_USER_PACKET_ENUM);
			returnBSHead.remoteType = static_cast<uint16_t>(ERemoteConnection::CLIENT);
			returnBSHead.packetID = static_cast<uint32_t>(EAuthPacketID::LOGIN_REQUEST);
			returnBS.Write(returnBSHead);
			//Data
			returnBS.Write(reason);

			StringUtils::writeBufferedStringToBitStream(&returnBS, "Talk_Like_A_Pirate");
			StringUtils::writeBufferedStringToBitStream(&returnBS, "guilds");
			StringUtils::writeBufferedStringToBitStream(&returnBS, "test");
			StringUtils::FillZero(&returnBS, 5 * 33);
			uint16_t majorVersion; uint16_t currentVersion; uint16_t minorVersion; ServerInfo::numericGameVersion(&majorVersion, &currentVersion, &minorVersion);
			returnBS.Write(majorVersion); returnBS.Write(currentVersion); returnBS.Write(minorVersion);

			std::string cip = client.ToString(false);
			std::string ip = (IPUtils::isIPPublic(client.binaryAddress) ? rakServer->GetExternalID(UNASSIGNED_SYSTEM_ADDRESS).ToString(false) :
							 (IPUtils::isIPIntern(client.binaryAddress) ? rakServer->GetInternalID().ToString(false) : "127.0.0.1"));
			
			ip = wsAddr.ToString(false);

			//ip = "foxsog.com";

			Logger::log("AUTH", "Redirecting " +  cip + " to " + ip + ":" + std::to_string(wsAddr.port));

			StringUtils::writeBufferedWStringToBitStream(&returnBS, (char16_t*)"$2a$10$KssILxWNR6k62B7yiX0GAe2Q7wwHlrzhF3LqtVvpyvHZf0MwvNfVu", 33);
			StringUtils::writeBufferedStringToBitStream(&returnBS, ip);
			StringUtils::writeBufferedStringToBitStream(&returnBS, ip);
			returnBS.Write<uint16_t>(wsAddr.port);
			returnBS.Write<uint16_t>(0);
			StringUtils::FillZero(&returnBS, 33);
			StringUtils::writeBufferedStringToBitStream(&returnBS, "00000000-0000-0000-0000-000000000000", 37);
			returnBS.Write<uint32_t>(0);
			StringUtils::writeBufferedStringToBitStream(&returnBS, SERVER_LANGUAGE, 3);
			returnBS.Write<ByteBool>(true); // first Subscription Logon
			returnBS.Write<ByteBool>(false); // is FTP
			returnBS.Write<uint64_t>(0);

			StringUtils::writeWStringToBitStream<std::uint16_t>(&returnBS, customErrorMessage);

			// Stamp stuff
			returnBS.Write<uint32_t>(0x0144); // Stamps
			returnBS.Write<uint64_t>(0x0000000000000000);	returnBS.Write<uint64_t>(0x000000004ee27a4c);
			returnBS.Write<uint64_t>(0x0000001c00000007);	returnBS.Write<uint64_t>(0x000000004ee27a4c);
			returnBS.Write<uint64_t>(0x0000000300000008);	returnBS.Write<uint64_t>(0x000000004ee27a4c);
			returnBS.Write<uint64_t>(0x0000000000000009);	returnBS.Write<uint64_t>(0x000000004ee27a4c);
			returnBS.Write<uint64_t>(0x000000000000000a);	returnBS.Write<uint64_t>(0x000000004ee27a4c);
			returnBS.Write<uint64_t>(0x000000010000000b);	returnBS.Write<uint64_t>(0x000000004ee27a4c);
			returnBS.Write<uint64_t>(0x000000010000000e);	returnBS.Write<uint64_t>(0x000000004ee27a4c);
			returnBS.Write<uint64_t>(0x000000000000000f);	returnBS.Write<uint64_t>(0x000000004ee27a4c);
			returnBS.Write<uint64_t>(0x0000000100000011);	returnBS.Write<uint64_t>(0x000000004ee27a4d);
			returnBS.Write<uint64_t>(0x0000000000000005);	returnBS.Write<uint64_t>(0x000000004ee27a4d);
			returnBS.Write<uint64_t>(0x0000000100000006);	returnBS.Write<uint64_t>(0x000000004ee27a4d);
			returnBS.Write<uint64_t>(0x0000000100000014);	returnBS.Write<uint64_t>(0x000000004ee27a4d);
			returnBS.Write<uint64_t>(0x000029ca00000013);	returnBS.Write<uint64_t>(0x000000004ee27a4d);
			returnBS.Write<uint64_t>(0x0000000000000015);	returnBS.Write<uint64_t>(0x000000004ee27a4c);
			returnBS.Write<uint64_t>(0x0000000000000016);	returnBS.Write<uint64_t>(0x000000004ee27a4c);
			returnBS.Write<uint64_t>(0x000029c400000017);	returnBS.Write<uint64_t>(0x000000004ee27a4d);
			returnBS.Write<uint64_t>(0x000029c40000001b);	returnBS.Write<uint64_t>(0x000000004ee27a4d);
			returnBS.Write<uint64_t>(0x000000010000001c);	returnBS.Write<uint64_t>(0x000000004ee27a4c);
			returnBS.Write<uint64_t>(0x000000000000001d);	returnBS.Write<uint64_t>(0x000000004ee27a4c);
			returnBS.Write<uint64_t>(0x000029ca0000001e);	returnBS.Write<uint64_t>(0x000000004ee27a4d);
			rakServer->Send(&returnBS, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, client, false);
		}

	};

};

#endif // !__PACKETFACTORY__AUTH__AUTHPACKETS_HPP__
