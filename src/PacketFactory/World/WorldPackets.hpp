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
#include "Database/Database.hpp"
#include "DataTypes/LWOOBJID.hpp"

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

			// TODO: Link to Session List
			unsigned long accountID = 0;

			std::vector<Database::Str_DB_CharInfo> charsInfo = Database::GetChars(accountID);
			size_t count = charsInfo.size();
			returnBS.Write(static_cast<std::uint8_t>(count & 0xFF));
			returnBS.Write(static_cast<std::uint8_t>(0)); // front char index

			for (int i = 0; i < charsInfo.size(); ++i) {
				Database::Str_DB_CharInfo charInfo = charsInfo[i];
				Database::Str_DB_CharStyle charStyle = Database::GetCharStyle(charInfo.styleID);
				DataTypes::LWOOBJID objectID = DataTypes::LWOOBJID::makePlayerObjectID(charInfo.objectID);
				returnBS.Write(objectID);
				returnBS.Write(static_cast<std::uint32_t>(charInfo.charIndex));
				StringUtils::writeWstringToBitStream(&returnBS, std::wstring(charInfo.name.begin(), charInfo.name.end()));
				StringUtils::writeWstringToBitStream(&returnBS, std::wstring(charInfo.pendingName.begin(), charInfo.pendingName.end()));
				returnBS.Write(static_cast<std::uint16_t>(0));
				returnBS.Write(charStyle.headColor);
				returnBS.Write(static_cast<std::uint16_t>(0));
				returnBS.Write(charStyle.head);
				returnBS.Write(charStyle.chestColor);
				returnBS.Write(charStyle.chest);
				returnBS.Write(charStyle.legs);
				returnBS.Write(charStyle.hairStyle);
				returnBS.Write(charStyle.hairColor);
				returnBS.Write(charStyle.leftHand);
				returnBS.Write(charStyle.rightHand);
				returnBS.Write(charStyle.eyebrowStyle);
				returnBS.Write(charStyle.eyesStyle);
				returnBS.Write(charStyle.mouthStyle);
				returnBS.Write(0);
				returnBS.Write(charInfo.lastWorld);
				returnBS.Write(charInfo.lastInstance);
				returnBS.Write(charInfo.lastClone);
				returnBS.Write(charInfo.lastLog);

				// TODO: Inventory (Equipped Items)
				returnBS.Write(static_cast<std::uint16_t>(0));
				Logger::log("WRLD", "Sent character " + charInfo.name);
			}

			rakServer->Send(&returnBS, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, client, false);
		}

	};

};

#endif // !__PACKETFACTORY__AUTH__WORLDPACKETS_HPP__
