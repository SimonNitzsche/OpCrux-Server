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
						}
						else if (args[1] == u"off") {
							GameMessages::Send(sender, targetOBJID, Jetpackmode);
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