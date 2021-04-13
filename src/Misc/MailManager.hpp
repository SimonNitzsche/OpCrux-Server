#ifndef __MISC__MAILMANAGER_HPP__
#define __MISC__MAILMANAGER_HPP__

#include <memory>
#include <string>

#include "Database/DatabaseModels.hpp"

class WorldServer;
class ClientSession;

class MailManager {
public:
	static void SendMailListToClient(WorldServer * ws, ClientSession* cl);
	static bool SendMail(WorldServer* ws, std::string receiver, std::string sender, std::string subject, std::string body, bool needsModeration = true, std::uint64_t currency = std::uint64_t(0), DatabaseModels::ItemModel item = DatabaseModels::ItemModel());
	static void SendNewMailNotification(WorldServer* ws, ClientSession* cl);
	static void MarkMailAsSeen(WorldServer* ws, ClientSession* cl, std::int64_t mailID);
	static void TakeAttachment(WorldServer* ws, ClientSession* cl, std::int64_t mailID);
};

#endif
