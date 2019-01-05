#ifndef __PACKETFACTORY__AUTH__WORLDPACKETS_HPP__
#define __PACKETFACTORY__AUTH__WORLDPACKETS_HPP__

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

namespace PacketFactory {

	namespace World {

		inline void sendCharList(RakPeerInterface * rakServer, SystemAddress client) {
			RakNet::BitStream returnBS;
			// Head
			LUPacketHeader returnBSHead;
			returnBSHead.protocolID = static_cast<uint8_t>(ID_USER_PACKET_ENUM);
			returnBSHead.remoteType = static_cast<uint16_t>(Enums::ERemoteConnection::CLIENT);
			returnBSHead.packetID = static_cast<uint32_t>(Enums::EClientPacketID::CHARACTER_LIST);
			returnBS.Write(returnBSHead);
			//Data
			returnBS.Write(static_cast<std::uint16_t>(0));

			
			rakServer->Send(&returnBS, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, client, false);
		}

	};

};

#endif // __PACKETFACTORY__AUTH__WORLDPACKETS_HPP__