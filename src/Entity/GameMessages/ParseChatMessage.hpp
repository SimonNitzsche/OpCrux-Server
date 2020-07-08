#ifndef __ENTITY__GM__ParseChatMessage_HPP__
#define __ENTITY__GM__ParseChatMessage_HPP__
#include "Entity/GameMessages.hpp"
#include "Misc/Commands.hpp"
#include <string>
#include <bitset>
#include <sstream>

namespace GM {
	struct ParseChatMessage : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_PARSE_CHAT_MESSAGE;
		}

		ParseChatMessage() : GMBase() {}

		int iClientState;

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE(bs, iClientState);
			std::string message = "";
			bool isCommand = false;
			while (true) {
				if (bs->GetNumberOfUnreadBits() == 0) {
					break;
				}
				bool bit1 = bs->ReadBit();
				bool bit2 = bs->ReadBit();
				bool bit3 = bs->ReadBit();
				bool bit4 = bs->ReadBit();
				bool bit5 = bs->ReadBit();
				bool bit6 = bs->ReadBit();
				bool bit7 = bs->ReadBit();
				bool bit8 = bs->ReadBit();

				std::string stringValue = std::string(bit1 ? "1" : "0") + std::string(bit2 ? "1" : "0") + std::string(bit3 ? "1" : "0") + std::string(bit4 ? "1" : "0") + std::string(bit5 ? "1" : "0") + std::string(bit6 ? "1" : "0") + std::string(bit7 ? "1" : "0") + std::string(bit8 ? "1" : "0");

				if (stringValue != "00000000" && stringValue != "00000101" && stringValue != "00000100") {
					std::stringstream sstream(stringValue);
					std::string output;
					std::bitset<8> bits;
					sstream >> bits;
					char c = char(bits.to_ulong());
					message.std::string::push_back(c);
				}
			}
			if ((message.find('/') == 0) == true) {
				parseCommand(message, bs);
			}
		}
	};
}

#endif