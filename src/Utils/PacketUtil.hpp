#ifndef _PACKETUTIL_HPP__
#define _PACKETUTIL_HPP__

#include "Common/CrossPlatform.hpp"
#include <memory>
#include <RakNet/BitStream.h>
#include <RakNet/MessageIdentifiers.h>
#include "Enums/ERemoteConnection.hpp"

namespace PacketUtils {
	static std::unique_ptr<RakNet::BitStream> initPacket(Enums::ERemoteConnection connectionType, uint32_t packetID) {
		auto bs = std::make_unique<RakNet::BitStream>();

		bs->Write(static_cast<uint8_t>(ID_USER_PACKET_ENUM));
		bs->Write(connectionType);
		bs->Write(packetID);
		bs->Write(static_cast<uint8_t>(0xff));

		return bs;
	}

};

#endif