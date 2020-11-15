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
			LUPacketHeader returnBSHead{};
			returnBSHead.protocolID = static_cast<std::uint8_t>(ID_USER_PACKET_ENUM);
			returnBSHead.remoteType = static_cast<std::uint16_t>(Enums::ERemoteConnection::CLIENT);
			returnBSHead.packetID = static_cast<std::uint32_t>(Enums::EClientPacketID::CHARACTER_LIST);
			returnBS.Write(returnBSHead);
			//Data

			std::vector<DatabaseModels::Str_DB_CharInfo> charsInfo = Database::GetChars(client->accountID);
			size_t count = charsInfo.size();
			returnBS.Write<std::uint8_t>(count & 0xFF);
			returnBS.Write<std::uint8_t>(0); // front char index

			for (auto charInfo : charsInfo) {
			    DatabaseModels::Str_DB_CharStyle charStyle = Database::GetCharStyle(charInfo.styleID);
				DataTypes::LWOOBJID objectID = DataTypes::LWOOBJID::makePlayerObjectID(charInfo.objectID);
				returnBS.Write(objectID);
				returnBS.Write<std::uint32_t>(charInfo.charIndex);
				StringUtils::writeBufferedWStringToBitStream(&returnBS, std::u16string(charInfo.name.begin(), charInfo.name.end()));
				StringUtils::writeBufferedWStringToBitStream(&returnBS, std::u16string(charInfo.pendingName.begin(), charInfo.pendingName.end()));
				returnBS.Write<std::uint16_t>(0);
				returnBS.Write(charStyle.headColor);
				returnBS.Write<std::uint16_t>(0);
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

				// Inventory (Equipped Items)

				auto inventory = Database::GetFullInventory(charInfo.objectID);

				std::list<std::int32_t> equippedLOTs = {};

				for (auto & it : inventory) {
					for (auto & it2 : it.second) {
						if (it2.attributes.GetEquipped()) {
							equippedLOTs.push_back(it2.templateID);
						}
					}
				}

				returnBS.Write<std::uint16_t>(equippedLOTs.size());
				for (int & equippedLOT : equippedLOTs) {
					returnBS.Write<std::uint32_t>(equippedLOT);
				}

				Logger::log("WRLD", "Sent character " + charInfo.name);
			}

			rakServer->Send(&returnBS, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, client->systemAddress, false);
		}

		inline void TestLoad(RakPeerInterface * rakServer, ClientSession * clientSession) {
			{
				RakNet::BitStream returnBS;
				// Head
				LUPacketHeader returnBSHead{};
				returnBSHead.protocolID = static_cast<std::uint8_t>(ID_USER_PACKET_ENUM);
				returnBSHead.remoteType = static_cast<std::uint16_t>(Enums::ERemoteConnection::CLIENT);
				returnBSHead.packetID = static_cast<std::uint32_t>(Enums::EClientPacketID::SERVER_GAME_MSG);
				returnBS.Write(returnBSHead);
				returnBS.Write<std::uint64_t>(clientSession->actorID);
				returnBS.Write<std::uint16_t>(0x66a); // Server Done Loading All Objects

				// Send
				rakServer->Send(&returnBS, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, clientSession->systemAddress, false);
			}
			{
				RakNet::BitStream returnBS;
				// Head
				LUPacketHeader returnBSHead{};
				returnBSHead.protocolID = static_cast<std::uint8_t>(ID_USER_PACKET_ENUM);
				returnBSHead.remoteType = static_cast<std::uint16_t>(Enums::ERemoteConnection::CLIENT);
				returnBSHead.packetID = static_cast<std::uint32_t>(Enums::EClientPacketID::SERVER_GAME_MSG);
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
			LUPacketHeader returnBSHead{};
			returnBSHead.protocolID = static_cast<std::uint8_t>(ID_USER_PACKET_ENUM);
			returnBSHead.remoteType = static_cast<std::uint16_t>(Enums::ERemoteConnection::CLIENT);
			returnBSHead.packetID = static_cast<std::uint32_t>(Enums::EWorldPacketID::CLIENT_LOGIN_REQUEST);
			returnBS.Write(returnBSHead);
			//Data

			// std::string& xmlFN = go->GetNameStr() + ".xml";
			// FileUtils::SaveTextFile(xmlFN, go->GenerateXML());

			std::int64_t levelid = clientSession->currentZone.zoneID | (clientSession->currentZone.zoneInstance << 16) | (clientSession->currentZone.zoneClone << 32);


			LDFCollection ldfEntries = {
				LDF_COLLECTION_INIT_ENTRY(u"template", std::int32_t(go->GetLOT())),
				LDF_COLLECTION_INIT_ENTRY(u"objid", clientSession->actorID),
				LDF_COLLECTION_INIT_ENTRY(u"xmlData", go->GenerateXML()),
				LDF_COLLECTION_INIT_ENTRY(u"name", go->GetName()),
				LDF_COLLECTION_INIT_ENTRY(u"accountID", std::int64_t(clientSession->accountID)),
				LDF_COLLECTION_INIT_ENTRY(u"levelid", levelid)
			};

			LDFUtils::SerializeCollection(returnBS, ldfEntries);

			char * finalPtr = (char*)returnBS.GetData();

			// Send
			rakServer->Send(&returnBS, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, clientSession->systemAddress, false);
		}

		inline void LoadStaticZone(RakPeerInterface * rakServer, ClientSession * clientSession, std::uint16_t zoneID, std::uint16_t mapInstance, std::uint32_t mapClone, std::uint32_t mapChecksum, DataTypes::Vector3 playerPosition, std::uint32_t activityMap) {
			RakNet::BitStream returnBS;
			// Head
			LUPacketHeader returnBSHead{};
			returnBSHead.protocolID = static_cast<std::uint8_t>(ID_USER_PACKET_ENUM);
			returnBSHead.remoteType = static_cast<std::uint16_t>(Enums::ERemoteConnection::CLIENT);
			returnBSHead.packetID = static_cast<std::uint32_t>(Enums::EClientPacketID::MSG_CLIENT_LOAD_STATIC_ZONE);
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

		inline void TransferToWorld(RakPeerInterface* rakServer, SystemAddress clientSession, char* ipAddress, std::uint16_t portOrErrorCode, bool doAnnouncement = false) {
			RakNet::BitStream returnBS;
			// Head
			LUPacketHeader returnBSHead;
			returnBSHead.protocolID = std::uint8_t(ID_USER_PACKET_ENUM);
			returnBSHead.remoteType = std::uint16_t(Enums::ERemoteConnection::CLIENT);
			returnBSHead.packetID = std::uint32_t(Enums::EClientPacketID::SERVER_REDIRECT);
			returnBS.Write(returnBSHead);

			// Data
			if (ipAddress != nullptr) {
				// Success, redirect
				StringUtils::writeBufferedStringToBitStream(&returnBS, std::string(ipAddress));
				returnBS.Write(portOrErrorCode);
				returnBS.Write<std::uint8_t>(doAnnouncement);
			}
			else {
				// Error
				StringUtils::writeBufferedStringToBitStream(&returnBS, "");
				returnBS.Write(portOrErrorCode);
			}

			Logger::log("WORLD", "Sending world redirect to " + std::string(const_cast<const char*>(ipAddress)) + ":" + std::to_string(portOrErrorCode) + " for " + clientSession.ToString());

			// Send
			rakServer->Send(&returnBS, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, clientSession, false);
		}
	};

};

#endif // !__PACKETFACTORY__AUTH__WORLDPACKETS_HPP__
