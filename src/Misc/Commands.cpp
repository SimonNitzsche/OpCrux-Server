#include <iostream>

#include <RakNet/BitStream.h>
#include <RakNet/RakNetTypes.h>

#include "Misc/Commands.hpp"
#include "Utils/Logger.hpp"

void parseCommand(std::string message, RakNet::BitStream* bs) {
	std::string command = message.erase(0, 1);
	if (command == "test") {
		// Logger::log("WRLD", "TEST COMMAND EXECUTED", LogType::UNEXPECTED);
	}
	else {

	}
}