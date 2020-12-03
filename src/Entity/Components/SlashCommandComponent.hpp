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

class SlashCommandComponent : public IEntityComponent {
public:
	SlashCommandComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 90; }

	void OnParseChatMessage(Entity::GameObject* sender, GM::ParseChatMessage * msg);

	void RegisterMessageHandlers() {
		REGISTER_OBJECT_MESSAGE_HANDLER(SlashCommandComponent, GM::ParseChatMessage, OnParseChatMessage);
	}

private:
	enum class Response {RankTooLow = 1, NoCommand = 2, NoArguements = 3,};

	void Reply(std::u16string message, Entity::GameObject* sender, Entity::GameObject* empty = nullptr) {
		//PacketFactory::Chat::SendPrivateChatMessage(empty, sender, message);
		PacketFactory::Chat::SendChatMessage(sender, 0x00, message);
	}

	void Reply(Response code, Entity::GameObject* sender, Entity::GameObject* empty = nullptr) {
		switch(code) {
			case Response::RankTooLow: {
				std::u16string message = u"You do not have high enough permissions for that";
				//PacketFactory::Chat::SendPrivateChatMessage(empty, sender, message);
				PacketFactory::Chat::SendChatMessage(sender, 0x00, message);
				break;
			}
			case Response::NoCommand: {
				std::u16string message = u"That command doesn't exist";
				//PacketFactory::Chat::SendPrivateChatMessage(empty, sender, message);
				PacketFactory::Chat::SendChatMessage(sender, 0x00, message);
				break;
			}
			
			case Response::NoArguements: {
				std::u16string message = u"This command requires arguements";
				//PacketFactory::Chat::SendPrivateChatMessage(empty, sender, message);
				PacketFactory::Chat::SendChatMessage(sender, 0x00, message);
				break;
			}	
		}
	}
};

#endif