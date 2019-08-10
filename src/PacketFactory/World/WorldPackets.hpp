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
#include "DataTypes/LDF.hpp"

#include "Entity/GameObject.hpp"

namespace PacketFactory {

	namespace World {

		inline void sendCharList(RakPeerInterface * rakServer, ClientSession * client) {
			RakNet::BitStream returnBS;
			// Head
			LUPacketHeader returnBSHead;
			returnBSHead.protocolID = static_cast<uint8_t>(ID_USER_PACKET_ENUM);
			returnBSHead.remoteType = static_cast<uint16_t>(Enums::ERemoteConnection::CLIENT);
			returnBSHead.packetID = static_cast<uint32_t>(Enums::EClientPacketID::CHARACTER_LIST);
			returnBS.Write(returnBSHead);
			//Data

			std::vector<Database::Str_DB_CharInfo> charsInfo = Database::GetChars(client->accountID);
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

			rakServer->Send(&returnBS, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, client->systemAddress, false);
		}

		inline void TestLoad(RakPeerInterface * rakServer, ClientSession * clientSession) {
			{
				RakNet::BitStream returnBS;
				// Head
				LUPacketHeader returnBSHead;
				returnBSHead.protocolID = static_cast<uint8_t>(ID_USER_PACKET_ENUM);
				returnBSHead.remoteType = static_cast<uint16_t>(Enums::ERemoteConnection::CLIENT);
				returnBSHead.packetID = static_cast<uint32_t>(Enums::EClientPacketID::SERVER_GAME_MSG);
				returnBS.Write(returnBSHead);
				returnBS.Write<std::uint64_t>(clientSession->actorID);
				returnBS.Write<std::uint16_t>(0x66a); // Server Done Loading All Objects

				// Send
				rakServer->Send(&returnBS, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, clientSession->systemAddress, false);
			}
			{
				RakNet::BitStream returnBS;
				// Head
				LUPacketHeader returnBSHead;
				returnBSHead.protocolID = static_cast<uint8_t>(ID_USER_PACKET_ENUM);
				returnBSHead.remoteType = static_cast<uint16_t>(Enums::ERemoteConnection::CLIENT);
				returnBSHead.packetID = static_cast<uint32_t>(Enums::EClientPacketID::SERVER_GAME_MSG);
				returnBS.Write(returnBSHead);
				returnBS.Write<std::uint64_t>(clientSession->actorID);
				returnBS.Write<std::uint16_t>(0x1fd); // Server Done Loading All Objects

				// Send
				rakServer->Send(&returnBS, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, clientSession->systemAddress, false);
			}
		}

		inline void CreateCharacter(RakPeerInterface * rakServer, ClientSession * clientSession, Entity::GameObject * go) {
			RakNet::BitStream returnBS;
			// Head
			LUPacketHeader returnBSHead;
			returnBSHead.protocolID = static_cast<uint8_t>(ID_USER_PACKET_ENUM);
			returnBSHead.remoteType = static_cast<uint16_t>(Enums::ERemoteConnection::CLIENT);
			returnBSHead.packetID = static_cast<uint32_t>(Enums::EWorldPacketID::CLIENT_LOGIN_REQUEST);
			returnBS.Write(returnBSHead);
			//Data
			RakNet::BitStream outerWrapperBS;

			RakNet::BitStream contentWrapperBS;
			
			std::vector<LDFEntry> ldfEntries;
			ldfEntries.push_back(LDFEntry(L"template", std::int32_t(1)));
			ldfEntries.push_back(LDFEntry(L"objid", clientSession->actorID));
			ldfEntries.push_back(LDFEntry(L"name", go->GetName()));
			ldfEntries.push_back(LDFEntry(L"xmlData", go->GenerateXML()));

			contentWrapperBS.Write(std::uint32_t(ldfEntries.size()));
			for (int i = 0; i < ldfEntries.size(); ++i) {
				LDFEntry entry = ldfEntries[i];
				contentWrapperBS.Write<std::uint8_t>((entry.key.size() & 0xFF) * 2);
				StringUtils::writeWstringToBitStream(&contentWrapperBS, entry.key, entry.key.size());
				contentWrapperBS.Write<std::uint8_t>(entry.type);
				entry.WriteToBitstream(&contentWrapperBS);
			}

			bool isContentCompressed = false;
			outerWrapperBS.Write<std::uint8_t>(isContentCompressed);
			if (isContentCompressed) {
				// TODO
				// [u32] size of uncompressed data
				// [u32] size of compressed data
			}

			//char * cwbsd; int cwbsl;
			//contentWrapperBS.Read(cwbsd, cwbsl);
			//outerWrapperBS.Write(cwbsd, cwbsl);
			std::string contentWrapperData((char*)contentWrapperBS.GetData(), contentWrapperBS.GetNumberOfBytesUsed());
			outerWrapperBS.Write((char*)contentWrapperBS.GetData(), contentWrapperBS.GetNumberOfBytesUsed());

			// Write
			returnBS.Write<std::uint32_t>(outerWrapperBS.GetNumberOfBytesUsed());
			//char * owbsd; int owbsl;
			//outerWrapperBS.Read(owbsd, owbsl);
			//returnBS.Write(owbsd, owbsl);
			std::string outerWrapperData((char*)contentWrapperBS.GetData(), contentWrapperBS.GetNumberOfBytesUsed());
			returnBS.Write((char*)outerWrapperBS.GetData(), outerWrapperBS.GetNumberOfBytesUsed());

			char * finalPtr = (char*)returnBS.GetData();

			// Send
			rakServer->Send(&returnBS, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, clientSession->systemAddress, false);

			// Cleanup
			for (int i = 0; i < ldfEntries.size(); ++i) {
				ldfEntries[i].Delete();
			}
		}

		inline void LoadStaticZone(RakPeerInterface * rakServer, ClientSession * clientSession, std::uint16_t zoneID, std::uint16_t mapInstance, std::uint32_t mapClone, std::uint32_t mapChecksum, DataTypes::Vector3 playerPosition, std::uint32_t activityMap) {
			RakNet::BitStream returnBS;
			// Head
			LUPacketHeader returnBSHead;
			returnBSHead.protocolID = static_cast<uint8_t>(ID_USER_PACKET_ENUM);
			returnBSHead.remoteType = static_cast<uint16_t>(Enums::ERemoteConnection::CLIENT);
			returnBSHead.packetID = static_cast<uint32_t>(Enums::EClientPacketID::MSG_CLIENT_LOAD_STATIC_ZONE);
			returnBS.Write(returnBSHead);
			//Data

			returnBS.Write<std::uint16_t>(zoneID);
			returnBS.Write<std::uint16_t>(mapInstance);
			returnBS.Write<std::uint32_t>(mapClone);
			returnBS.Write<std::uint32_t>(mapChecksum);
			returnBS.Write<DataTypes::Vector3>(playerPosition);
			returnBS.Write<std::uint32_t>(activityMap);

			// Send
			rakServer->Send(&returnBS, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, clientSession->systemAddress, false);
		}
	};

};

#endif // !__PACKETFACTORY__AUTH__WORLDPACKETS_HPP__
