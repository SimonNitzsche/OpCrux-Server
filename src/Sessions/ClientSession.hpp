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
#include "Utils/StringUtils.hpp"

// NOTES:
// Do not use pointers or references in this data structure.
// This is being sent and shared between multiple servers and machines.
// Pointers and References are not being sent correctly, as it would
// only send the address and not the data.

#pragma pack(0)
struct ClientSession {
	SystemAddress systemAddress;
	DataTypes::ZoneInfo currentZone{};
	DataTypes::LWOOBJID actorID;
	std::uint32_t accountID;
	std::uint32_t connectedServerPort;

	std::u16string sessionToken;

	void Serialize(RakNet::BitStream * bs) {
		bs->Write(systemAddress);
		bs->Write(currentZone);
		bs->Write(actorID);
		bs->Write(accountID);
		bs->Write(connectedServerPort);
		StringUtils::writeBufferedWStringToBitStream(bs, sessionToken);
	}

	void Deserialize(RakNet::BitStream* bs) {
		bs->Read(systemAddress);
		bs->Read(currentZone);
		bs->Read(actorID);
		bs->Read(accountID);
		bs->Read(connectedServerPort);
		sessionToken = StringUtils::readBufferedWStringFromBitStream(bs);
	}
};

#pragma pack(pop)

#endif // !__SESSIONS__CLIENTSESSION_HPP__
