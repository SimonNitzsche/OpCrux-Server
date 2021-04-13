#include "MailManager.hpp"
#include "Server/WorldServer.hpp"
#include "Database/Database.hpp"

#include "Structs/Networking/General/StructPacketHeader.hpp"
#include "Enums/ERemoteConnection.hpp"
#include "Enums/EPackets.hpp"
#include <Entity\Components\InventoryComponent.hpp>
#include <Entity\Components\CharacterComponent.hpp>

void MailManager::SendMailListToClient(WorldServer* ws, ClientSession* cl) {
	RakNet::BitStream returnBS;
	// Head
	LUPacketHeader returnBSHead{};
	returnBSHead.protocolID = static_cast<std::uint8_t>(ID_USER_PACKET_ENUM);
	returnBSHead.remoteType = static_cast<std::uint16_t>(Enums::ERemoteConnection::CLIENT);
	returnBSHead.packetID = static_cast<std::uint32_t>(Enums::EClientPacketID::MAIL_STUFF);
	returnBS.Write(returnBSHead);
	//Data
	returnBS.Write(0x04); // mail data

	bool throttled = false;
	returnBS.Write<std::uint32_t>(throttled ? 1 : 0);

	if (!throttled) {
		auto mails = Database::GetMails(ws->GetDBConnection(), cl->actorID);

		returnBS.Write<std::uint16_t>(mails.size()); // number of mails
		returnBS.Write<std::uint16_t>(0); // some offset
		for (auto& mail : mails) {
			returnBS.Write(mail.mailID);
			StringUtils::writeBufferedWStringToBitStream(&returnBS, StringUtils::to_u16string(mail.subject), 50);
			StringUtils::writeBufferedWStringToBitStream(&returnBS, StringUtils::to_u16string(mail.body), 400);
			StringUtils::writeBufferedWStringToBitStream(&returnBS, StringUtils::to_u16string(mail.sender), 32);
			returnBS.Write<std::uint32_t>(0);
			returnBS.Write(mail.attachedCurrency);
			returnBS.Write(mail.attachedObjectID);
			returnBS.Write(mail.attachedLOT);
			returnBS.Write<std::uint32_t>(0);
			returnBS.Write(mail.attachedSubkey);
			returnBS.Write(mail.attachmentCount);
			returnBS.Write<std::uint16_t>(0);
			returnBS.Write<std::uint32_t>(0);
			returnBS.Write(mail.expirationDate);
			returnBS.Write(mail.sendDate);
			returnBS.Write<std::uint8_t>(mail.markedAsSeen);
			returnBS.Write<std::uint8_t>(mail.hasBeenModerated);
			returnBS.Write<std::uint16_t>(0);
			returnBS.Write<std::uint32_t>(0);
		}
	}

	// Send
	ws->rakServer->Send(&returnBS, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, cl->systemAddress, false);
}

bool MailManager::SendMail(WorldServer* ws, std::string receiver, std::string sender, std::string subject, std::string body, bool needsModeration, std::uint64_t currency, DatabaseModels::ItemModel item) {
	// some checks
	if (receiver == "") return false;
	if (sender == "") return false;
	if (sender.size() > 32) return false;
	if (subject == "") return false;
	if (subject.size() > 50) return false;
	if (body == "") return false;
	if (body.size() > 400) return false;

	auto receiverChar = Database::GetCharByName(ws->GetDBConnection(), receiver);
	if (receiverChar.name != receiver) return false;

	MailModel mail;
	mail.receiver = receiverChar.objectID;
	mail.sender = sender;
	mail.subject = subject;
	mail.body = body;
	mail.hasBeenModerated = !needsModeration;
	mail.attachedCurrency = currency;
	mail.attachedLOT = item.templateID;
	if (mail.attachedLOT != -1) {
		mail.attachedObjectID = item.objectID;
		mail.attachedSubkey = item.subkey;
		mail.attachmentCount = item.count;
	}
	mail.markedAsSeen = false;

	mail.sendDate = ServerInfo::GetSystemTimeAsUnixTime();
	mail.expirationDate = mail.sendDate;

	Database::SendMail(ws->GetDBConnection(), mail);

	return true;
}

void MailManager::SendNewMailNotification(WorldServer* ws, ClientSession* cl) {
	auto mailCount = Database::GetNewMailCount(ws->GetDBConnection(), cl->actorID);
	if (mailCount == 0) return;

	RakNet::BitStream returnBS;
	// Head
	LUPacketHeader returnBSHead{};
	returnBSHead.protocolID = static_cast<std::uint8_t>(ID_USER_PACKET_ENUM);
	returnBSHead.remoteType = static_cast<std::uint16_t>(Enums::ERemoteConnection::CLIENT);
	returnBSHead.packetID = static_cast<std::uint32_t>(Enums::EClientPacketID::MAIL_STUFF);
	returnBS.Write(returnBSHead);
	//Data
	returnBS.Write(0x02); // mail notification
	returnBS.Write<std::uint32_t>(0);
	returnBS.Write<std::uint64_t>(0);
	returnBS.Write<std::uint64_t>(0);
	returnBS.Write<std::uint64_t>(0);
	returnBS.Write<std::uint64_t>(0);
	returnBS.Write<std::uint32_t>(mailCount);
	returnBS.Write<std::uint32_t>(0);

	// Send
	ws->rakServer->Send(&returnBS, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, cl->systemAddress, false);
}

void MailManager::MarkMailAsSeen(WorldServer* ws, ClientSession* cl, std::int64_t mailID) {
	auto mail = Database::GetMail(ws->GetDBConnection(), mailID);
	if (mail.mailID != mailID) return;
	if (mail.receiver != cl->actorID.getPureID()) return;
	mail.markedAsSeen = true;
	Database::UpdateMail(ws->GetDBConnection(), mail);

	RakNet::BitStream returnBS;
	// Head
	LUPacketHeader returnBSHead{};
	returnBSHead.protocolID = static_cast<std::uint8_t>(ID_USER_PACKET_ENUM);
	returnBSHead.remoteType = static_cast<std::uint16_t>(Enums::ERemoteConnection::CLIENT);
	returnBSHead.packetID = static_cast<std::uint32_t>(Enums::EClientPacketID::MAIL_STUFF);
	returnBS.Write(returnBSHead);
	//Data
	returnBS.Write(0x0a); // mail notification
	returnBS.Write<std::uint32_t>(0);
	returnBS.Write<std::uint64_t>(mailID);

	// Send
	ws->rakServer->Send(&returnBS, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, cl->systemAddress, false);
}

void MailManager::TakeAttachment(WorldServer* ws, ClientSession* cl, std::int64_t mailID) {
	int returnCode = 3;
	while (true) {
		auto mail = Database::GetMail(ws->GetDBConnection(), mailID);
		if (mail.mailID != mailID) break;
		if (mail.receiver != cl->actorID.getPureID()) break;

		returnCode = 5;
		auto playerObj = ws->objectsManager->GetObjectByID(cl->actorID);
		if (playerObj == nullptr) break;

		auto invComp = playerObj->GetComponent<InventoryComponent>();

		returnCode = 2;
		if (invComp->GetNextFreeSlot(mail.attachedLOT) == 0xFFFFFFFF)
			break;

		invComp->AddItem(mail.attachedLOT, mail.attachmentCount, Vector3(), mail.attachedSubkey);

		if (mail.attachedCurrency != 0) {
			GM::PickupCurrency msgCurrency;
			msgCurrency.currency = mail.attachedCurrency;
			msgCurrency.position = playerObj->GetPosition();
			playerObj->CallMessage(msgCurrency);
		}

		auto mailCopy = mail;
		mail.attachedCurrency = 0;
		mail.attachedLOT = -1;
		mail.attachmentCount = 0;
		Database::UpdateMail(ws->GetDBConnection(), mail);
		returnCode = 0;
		break;
	}

	RakNet::BitStream returnBS;
	// Head
	LUPacketHeader returnBSHead{};
	returnBSHead.protocolID = static_cast<std::uint8_t>(ID_USER_PACKET_ENUM);
	returnBSHead.remoteType = static_cast<std::uint16_t>(Enums::ERemoteConnection::CLIENT);
	returnBSHead.packetID = static_cast<std::uint32_t>(Enums::EClientPacketID::MAIL_STUFF);
	returnBS.Write(returnBSHead);
	//Data
	returnBS.Write(0x06); // mail notification
	returnBS.Write<std::uint32_t>(returnCode);
	returnBS.Write<std::uint64_t>(mailID);

	// Send
	ws->rakServer->Send(&returnBS, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, cl->systemAddress, false);
}
