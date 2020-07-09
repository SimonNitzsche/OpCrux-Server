#ifndef __REPLICA__COMPONENTS__SLASH_COMMAND_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__SLASH_COMMAND_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameMessages/ParseChatMessage.hpp"
#include "Utils/switch.hpp"
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

		Switch(command)
			.Case(u"/shutdown", [&]() {
				if (accountGMLevel <= 9) {
					exit(3);
				}
				else {
					std::u16string message = u"You do not have high enough permissions for that"; // TODO: Send this as a response for the command
				}
				Logger::log("WRLD", ("Shutdown command triggered by " + sender->GetNameStr()));
				
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