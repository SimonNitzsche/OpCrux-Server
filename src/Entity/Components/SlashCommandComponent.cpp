#include "SlashCommandComponent.hpp"
#include "InventoryComponent.hpp"
#include "CharacterComponent.hpp"
#include "GameCache/ZoneTable.hpp"

void SlashCommandComponent::OnParseChatMessage(Entity::GameObject* sender, GM::ParseChatMessage * msg) {
	// Check if we are a command, otherwise return
	if (msg->wsString[0] != u'/') return;

	int accountGMLevel = Database::GetAccountGMLevel(owner->GetZoneInstance()->GetDBConnection(), Database::GetAccountIDFromMinifigOBJID(owner->GetZoneInstance()->GetDBConnection(), sender->GetObjectID()));

	std::u16string command = msg->wsString;
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
	.Case(u"/gmadditem", [&]() {
		if (command.find(u" ") != std::u16string::npos) {
			if (accountGMLevel >= 5) {
				InventoryComponent* inventory = sender->GetComponent<InventoryComponent>();
				inventory->AddItem(std::stoi(StringUtils::to_string(args[1])));
				Reply(u"Added item " + args[1], sender);
			}
			else {
				Reply(Response::RankTooLow, sender);
			}
		}
		else {
			Reply(Response::NoArguements, sender);
		}
	})
	.Case(u"/togglepvp", [&]() {
		auto comp = sender->GetComponent<CharacterComponent>();
		comp->TogglePvP();
		Reply(u"Toggled PvP", sender);
	})
	.Case(u"/removeItem", [&]() {
		if (command.find(u" ") != std::u16string::npos) {
			if (accountGMLevel >= 5) {
				InventoryComponent* inventory = sender->GetComponent<InventoryComponent>();
				inventory->RemoveItem2(true, std::stoi(StringUtils::to_string(args[1])), 2);
			}
			else {
				Reply(Response::RankTooLow, sender);
			}
		}
		else {
			Reply(Response::NoArguements, sender);
		}
	})
	.Case(u"/fly", [&]() {
		if (command.find(u" ") != std::u16string::npos) {
			if (accountGMLevel >= 5) {
				DataTypes::LWOOBJID targetOBJID = sender->GetObjectID();
				InventoryComponent* invComp = owner->GetComponent<InventoryComponent>();
				GM::SetJetPackMode Jetpackmode;
				if (args[1] == u"on") {
					Jetpackmode.bBypassChecks = true;
					Jetpackmode.bUse = true;
					Jetpackmode.effectID = 167;
					Jetpackmode.fAirspeed = 10 * 2;
					Jetpackmode.fMaxAirspeed = 15 * 2;
					GameMessages::Send(sender, targetOBJID, Jetpackmode);
					if (!invComp->HasItem(1727)) {
						invComp->AddItem(1727);
					}
					invComp->EquipItem(1727);
					Reply(u"Jetpack set to: True", sender);
				}
				else if (args[1] == u"off") {
					GameMessages::Send(sender, targetOBJID, Jetpackmode);
					Reply(u"Jetpack set to: False", sender);
					invComp->UnEquipItem(1727);
				}
				else {
					float speed = StringUtils::StringToFloat(StringUtils::to_string(args[1]));
					Jetpackmode.bBypassChecks = true;
					Jetpackmode.bUse = true;
					Jetpackmode.effectID = 167;
					Jetpackmode.fAirspeed = speed;
					Jetpackmode.fMaxAirspeed = speed + 5.0;
					GameMessages::Send(sender, targetOBJID, Jetpackmode);
					if (!invComp->HasItem(1727)) {
						invComp->AddItem(1727);
					}
					invComp->EquipItem(1727);
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
		}
		else {
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
	.Case(u"/gmlevel", [&]() {
		Reply(StringUtils::to_u16string(StringUtils::IntToString(Database::GetAccountGMLevel(owner->GetZoneInstance()->GetDBConnection(), Database::GetAccountIDFromMinifigOBJID(owner->GetZoneInstance()->GetDBConnection(), sender->GetObjectID())))), sender);
	})
	.Case(u"/testmap", [&]() {
		if (command.find(u" ") != std::u16string::npos) {
			if (accountGMLevel >= 5) {
				DataTypes::LWOOBJID targetOBJID = sender->GetObjectID();
				auto zoneID = StringUtils::StringToInt(StringUtils::to_string(args[1]));

				Reply(u"Requesting map change...", sender);

				std::string zoneName = CacheZoneTable::GetZoneName(zoneID).operator std::string(); // Linux is case sensitive. TODO: Find better approach
				StringUtils::ToLower(zoneName);

				if (FileUtils::FileExists("res/maps/" + zoneName)) {

					ClientSession* clSession = sender->GetZoneInstance()->sessionManager.GetSession(sender->GetObjectID());
					DataTypes::ZoneInfo zi;
					zi.zoneID = zoneID;

					masterServerBridge->ClientRequestMapChange(*clSession, zi);
				}
				else {
					Reply(u"Zone does not exist.", sender);
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
	.Case(u"/setimag", [&]() {
		if (command.find(u" ") != std::u16string::npos) {
			if (accountGMLevel >= 5) {
				DataTypes::LWOOBJID targetOBJID = sender->GetObjectID();
				auto imag = StringUtils::StringToInt(StringUtils::to_string(args[1]));

				sender->SetImagination(imag);
			}
			else {
				Reply(Response::RankTooLow, sender);
			}
		}
		else {
			Reply(Response::NoArguements, sender);
		}
	})
	.Case(u"/getlot", [&]() {
		if (command.find(u" ") != std::u16string::npos) {
			if (accountGMLevel >= 5) {
				DataTypes::LWOOBJID targetOBJID = sender->GetObjectID();
				auto objectID = std::stoull(StringUtils::to_string(args[1]));

				Entity::GameObject * go = owner->GetZoneInstance()->objectsManager->GetObjectByID(objectID);
				if (go == nullptr) {
					Reply(u"Object could not be found", sender);
				}
				else {
					Reply(u"LOT: " + StringUtils::to_u16string(std::to_string(go->GetLOT())), sender);
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
	.Case(u"/testbuff", [&]() {
		if (command.find(u" ") != std::u16string::npos) {
			if (accountGMLevel >= 5) {
				DataTypes::LWOOBJID targetOBJID = sender->GetObjectID();
				auto buffIndex = std::stoi(StringUtils::to_string(args[1]));
				auto buffValue = std::stoi(StringUtils::to_string(args[2]));

				Entity::GameObject* go = owner->GetZoneInstance()->objectsManager->GetObjectByID(targetOBJID);
				
				StatsComponent* destComp = go->GetComponent<StatsComponent>();
				
				switch (buffIndex) {
				case 0: destComp->buffs.buff_0 = buffValue; break;
				case 1: destComp->buffs.buff_1 = buffValue; break;
				case 2: destComp->buffs.buff_2 = buffValue; break;
				case 3: destComp->buffs.buff_3 = buffValue; break;
				case 4: destComp->buffs.buff_4 = buffValue; break;
				case 5: destComp->buffs.buff_5 = buffValue; break;
				case 6: destComp->buffs.buff_6 = buffValue; break;
				case 7: destComp->buffs.buff_7 = buffValue; break;
				case 8: destComp->buffs.buff_8 = buffValue; break;
				}

				destComp->_buffsDirty = true;
				go->SetDirty();

			}
			else {
				Reply(Response::RankTooLow, sender);
			}
		}
		else {
			Reply(Response::NoArguements, sender);
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