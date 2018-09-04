#ifndef __SESSIONS__CLIENTSESSION_HPP__
#define __SESSIONS__CLIENTSESSION_HPP__

#include <string>
#include <cinttypes>
#include <stdint.h>
#include <memory>
#include <RakNet/RakNetTypes.h>
#include "DataTypes/LEGO_GUID.hpp"
#include "DataTypes/ZoneInfo.hpp"
#include "DataTypes/LWOOBJID.hpp"

// NOTES:
// Do not use pointers or references in this data structure.
// This is being sent and shared between multiple servers and machines.
// Pointers and References are not being sent correctly, as it would
// only send the address and not the data.

struct ClientSession {
	SystemAddress systemAddress;
	std::wstring sessionToken;
	LEGO_GUID legoGUID;
	ZoneInfo currentZone;
	LWOOBJID actorID;
};

#endif