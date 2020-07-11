#ifndef __REPLICA__COMPONENTS__SLASH_COMMAND_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__SLASH_COMMAND_COMPONENT_HPP__

#include <codecvt>
#include <iostream>
#include <locale>
#include <string>

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameMessages/ParseChatMessage.hpp"
#include "Utils/switch.hpp"
#include "Utils/StringUtils.hpp"
#include "DataTypes/LWOOBJID.hpp"
#include "Database/Database.hpp"

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
		
		Switch(args[0])
			.Case(u"/shutdown", [&]() {
				if (accountGMLevel >= 9) {
					exit(3);
				}
				else {
					std::u16string message = u"You do not have high enough permissions for that"; // TODO: Send this as a response for the command
				}
				Logger::log("WRLD", ("Shutdown command triggered by " + sender->GetNameStr()));
				
			})
			.Case(u"/fly", [&]() {
				if (accountGMLevel >= 5) {
					DataTypes::LWOOBJID targetOBJID = sender->GetObjectID();
					GM::SetJetPackMode Jetpackmode;
					if (args[1] == u"on") {
						Jetpackmode.bBypassChecks = true;
						Jetpackmode.bUse = true;
 						Jetpackmode.effectID = 1;
						GameMessages::Send(sender, targetOBJID, Jetpackmode);
					}
					else if (args[1] == u"off") {
						GameMessages::Send(sender, targetOBJID, Jetpackmode);
					}
				}
				else {
					std::u16string message = u"You do not have high enough permissions for that";
				}
				Logger::log("WRLD", ("Fly command triggered by " + sender->GetNameStr()));
				})
			.Case(u"/teleport", [&]() {})
			.Case(u"/changegravity", [&]() {
					if (accountGMLevel >= 5) {
						if (args[1] == u"2") {
							DataTypes::LWOOBJID targetOBJID = sender->GetObjectID();
							GM::SetGravityScale GravityScale;
							GravityScale.gravityScale = 2;
							GameMessages::Send(sender, targetOBJID, GravityScale);
						}
						else if (args[1] == u"1") {
							DataTypes::LWOOBJID targetOBJID = sender->GetObjectID();
							GM::SetGravityScale GravityScale;
							GravityScale.gravityScale = 1;
							GameMessages::Send(sender, targetOBJID, GravityScale);
						}
					}
					else {
						std::u16string message = u"You do not have high enough permissions for that";
					}
			})
			.Default([&]() {
				std::u16string message = u"That command doesn't exist"; 
				Logger::log("WRLD", "Unknown Command triggered by " + sender->GetNameStr());
			});
			/*.Case(u"/<command>", [&]() {
				if (accountGMLevel <= <targetGMLevel>) {
				}
				else {
					std::u16string message = u"You do not have high enough permissions for that";
				}
				Logger::log("WRLD", ("<command> command triggered by " + sender->GetNameStr()));
				})*/
	}

private:

};

#endif