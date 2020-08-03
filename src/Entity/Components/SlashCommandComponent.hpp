#ifndef __REPLICA__COMPONENTS__SLASH_COMMAND_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__SLASH_COMMAND_COMPONENT_HPP__

#include <codecvt>
#include <locale>

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameMessages/ParseChatMessage.hpp"
#include "Utils/switch.hpp"
#include "Utils/StringUtils.hpp"
#include "DataTypes/LWOOBJID.hpp"
#include "Database/Database.hpp"
#include "Enums/EKillType.hpp"
#include "PacketFactory/World/WorldPackets.hpp"
#include "PacketFactory/Chat/ChatPackets.hpp"
#include "DataTypes/AMF3.hpp"
#include "GameCache\Objects.hpp"

class SlashCommandComponent : public IEntityComponent {
public:
	SlashCommandComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 90; }

	void OnParseChatMessage(Entity::GameObject* sender, GM::ParseChatMessage& msg) {
		// Check if we are a command, otherwise return
		if (msg.wsString[0] != u'/') return;

		int accountGMLevel = Database::GetAccountGMLevel(Database::GetAccountIDFromMinifigOBJID(sender->GetObjectID()));

		std::u16string command = msg.wsString;
		std::vector<std::u16string> args = StringUtils::splitWString(command, { u' ' });
		std::u16string message = u"";

		Switch(args[0])
			.Case(u"/debugUI", [&]() {
				if (accountGMLevel >= 5) {
					RakNet::BitStream bs = RakNet::BitStream();
					LUPacketHeader returnBSHead;
					returnBSHead.protocolID = static_cast<uint8_t>(ID_USER_PACKET_ENUM);
					returnBSHead.remoteType = static_cast<uint16_t>(Enums::ERemoteConnection::CLIENT);
					returnBSHead.packetID = static_cast<uint32_t>(Enums::EClientPacketID::SERVER_GAME_MSG);
					bs.Write(returnBSHead);
					// GM Header
					bs.Write<std::uint64_t>(sender->GetObjectID());
					bs.Write<std::uint16_t>(1184);
					bs.Write<std::uint8_t>(9);
					bs.Write<std::uint8_t>(1);
					bs.Write<std::uint8_t>((5 << 1) + 1);
					bs.Write("state", 5);
					bs.Write<std::uint8_t>(6);
					bs.Write<std::uint8_t>((16 << 1) + 1);
					bs.Write("ToggleUIDebugger", 16);
					bs.Write<std::uint8_t>(1);
					StringUtils::writeStringToBitStream<std::uint32_t>(&bs, "ToggleUIDebugger");
					sender->GetZoneInstance()->rakServer->Send(&bs, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
				}
			})
			.Case(u"/shutdown", [&]() {
				if (accountGMLevel >= 9) {
					exit(3);
				}
				else {
					Reply(Response::RankTooLow, sender);
				}
			})
			.Case(u"/fly", [&]() {
				if (command.find(u" ") != std::u16string::npos) {
					if (accountGMLevel >= 5) {
						DataTypes::LWOOBJID targetOBJID = sender->GetObjectID();
						GM::SetJetPackMode Jetpackmode;
						if (args[1] == u"on") {
							Jetpackmode.bBypassChecks = true;
							Jetpackmode.bUse = true;
							Jetpackmode.effectID = 1;
							Jetpackmode.fAirspeed = 10*2;
							Jetpackmode.fMaxAirspeed = 15*2;
							GameMessages::Send(sender, targetOBJID, Jetpackmode);
							Reply(u"Jetpack set to: False", sender);
						}
						else if (args[1] == u"off") {
							GameMessages::Send(sender, targetOBJID, Jetpackmode);
							Reply(u"Jetpack set to: True", sender);
						}
						else {
							float speed = StringUtils::StringToFloat(StringUtils::to_string(args[1]));
							Jetpackmode.bBypassChecks = true;
							Jetpackmode.bUse = true;
							Jetpackmode.effectID = 1;
							Jetpackmode.fAirspeed = speed;
							Jetpackmode.fMaxAirspeed = speed + 5.0;
							GameMessages::Send(sender, targetOBJID, Jetpackmode);
							Reply(u"Jetpack enabled with speed " + StringUtils::to_u16string(StringUtils::FloatToString(speed)), sender);
						}
					}
					else {
						Reply(Response::RankTooLow, sender);
					}
				}
				else {
					Reply(Response::NoArguements, sender);
				}
			})
			.Case(u"/teleport", [&]() {
				if (command.find(u" ") != std::u16string::npos) {
					if (accountGMLevel >= 5) {
						DataTypes::LWOOBJID targetOBJID = sender->GetObjectID();
						GM::Teleport teleport;
						teleport.pos = DataTypes::Vector3(StringUtils::StringToFloat(StringUtils::to_string(args[1])), StringUtils::StringToFloat(StringUtils::to_string(args[2])), StringUtils::StringToFloat(StringUtils::to_string(args[3])));
						teleport.x = 0;
						teleport.y = 0;
						teleport.z = 0;
						GameMessages::Send(sender, targetOBJID, teleport);
					}
					else { Reply(Response::RankTooLow, sender); }
				} else {
					Reply(Response::NoArguements, sender);
				}
			})
			.Case(u"/changegravity", [&]() {
				if (command.find(u" ") != std::u16string::npos) {
					if (accountGMLevel >= 5) {
						DataTypes::LWOOBJID targetOBJID = sender->GetObjectID();
						GM::SetGravityScale GravityScale;
						float scale = StringUtils::StringToFloat(StringUtils::to_string(args[1]));
						GravityScale.gravityScale = scale;
						GameMessages::Send(sender, targetOBJID, GravityScale);
						std::cout << scale;
						Reply(u"Gravity changed to " + StringUtils::to_u16string(StringUtils::FloatToString(scale)), sender);
					}
					else {
						Reply(Response::RankTooLow, sender);
					}
				}
				else {
					Reply(Response::NoArguements, sender);
				}
			})
			.Case(u"/die", [&]() {
				if (accountGMLevel >= 5) {
					DataTypes::LWOOBJID targetOBJID = sender->GetObjectID();
					GM::Die die;
					die.killType = Enums::EKillType::SILENT;
					die.killerID = DataTypes::LWOOBJID();
					GameMessages::Send(sender, targetOBJID, die);
				}
				else {
					Reply(Response::RankTooLow, sender);
				}
			})
			.Case(u"/testmap", [&]() {
				bool handled = false;
				std::vector<std::int32_t> approvedWorlds = {1100, 1200};
				for (auto item : approvedWorlds) {
					if (item == std::stoi(StringUtils::to_string(args[1]))) {
						handled = true;
					}
				}
				if (!handled) { Reply(u"Sorry that zone is disabled, you can't testmap to it", sender); }
			})
			.Case(u"/gmlevel", [&]() {
				Reply(StringUtils::to_u16string(StringUtils::IntToString(Database::GetAccountGMLevel(Database::GetAccountIDFromMinifigOBJID(sender->GetObjectID())))), sender);
			})
			.Case(u"/gmadditem", [&]() {
				if (args.size() >= 2) {
					InventoryComponent* InvComp = sender->GetComponent<InventoryComponent>();
					InvComp->AddItem(std::stoi(StringUtils::to_string(args[1])));
				}
			})
			.Case(u"/getAllObjects", [&]() {
				Entity::GameObject* player = sender->GetZoneInstance()->objectsManager->GetObjectByID(sender->GetObjectID());
				for (auto obj : sender->GetZoneInstance()->objectsManager->GetObjects()) {
					if (obj != player) {
						Reply(StringUtils::to_u16string((std::string)CacheObjects::GetName(obj->GetLOT())) + StringUtils::to_u16string(" with LOT ") + StringUtils::to_u16string(std::to_string(obj->GetLOT())) + StringUtils::to_u16string(" as ") + StringUtils::to_u16string(std::to_string((std::uint64_t)obj->GetObjectID())), sender);
					}
				}
			})
			.Case(u"/getNearObject", [&]() {
				Entity::GameObject* player = sender->GetZoneInstance()->objectsManager->GetObjectByID(sender->GetObjectID());
				DataTypes::Vector3 playerPos = player->GetPosition();
				float nearestDistance = INFINITY;
				Entity::GameObject* nearestObject = nullptr;
				for (auto obj : sender->GetZoneInstance()->objectsManager->GetObjects()) {
					if (obj != player) {
						DataTypes::Vector3 objPos;
						objPos = obj->GetPosition();

						float distance = Vector3::Distance(playerPos, objPos);
						if (distance < nearestDistance) {
							nearestDistance = distance;
							nearestObject = obj;
						}
					}
				}
				Reply(u"Nearest Object is " + StringUtils::to_u16string((std::string)CacheObjects::GetName(nearestObject->GetLOT())) + StringUtils::to_u16string(" with LOT ") + StringUtils::to_u16string(std::to_string(nearestObject->GetLOT())) + StringUtils::to_u16string(" as ") + StringUtils::to_u16string(std::to_string((std::uint64_t)nearestObject->GetObjectID())), sender);
			})
			.Default([&]() {
				std::stringstream ss;
				ss << "Unknown Command \"";
				ss << StringUtils::to_string(args[0]);
				ss << "\" ran by ";
				ss << sender->GetNameStr();
				Logger::log("WRLD", ss.str());
			});

			
	}

private:
	enum class Response {RankTooLow = 1, NoCommand = 2, NoArguements = 3,};

	void Reply(std::u16string message, Entity::GameObject* sender, Entity::GameObject* empty = nullptr) {
		PacketFactory::Chat::SendPrivateChatMessage(empty, sender, message);
	}

	void Reply(Response code, Entity::GameObject* sender, Entity::GameObject* empty = nullptr) {
		switch(code) {
			case Response::RankTooLow: {
				std::u16string message = u"You do not have high enough permissions for that";
				PacketFactory::Chat::SendPrivateChatMessage(empty, sender, message);
				break;
			}
			case Response::NoCommand: {
				std::u16string message = u"That command doesn't exist";
				PacketFactory::Chat::SendPrivateChatMessage(empty, sender, message);
				break;
			}
			
			case Response::NoArguements: {
				std::u16string message = u"This command requires arguements";
				PacketFactory::Chat::SendPrivateChatMessage(empty, sender, message);
				break;
			}	
		}
	}
};

#endif