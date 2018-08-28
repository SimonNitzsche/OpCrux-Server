#ifndef __NETWORKDATATYPES__LUPACKET_HPP__
#define __NETWORKDATATYPES__LUPACKET_HPP__

#include <RakNet/RakNetTypes.h>
#include "Structs/Networking/General/StructPacketHeader.hpp"

class LUPacket : private Packet {
public:
	Packet * getPacket() {
		return reinterpret_cast<Packet*>(this);
	}

	LUPacketHeader getHeader() {
		return *reinterpret_cast<LUPacketHeader*>(this->data);
	}

	unsigned int getLength() {
		return this->length - 8;
	}

	unsigned char* getData() {
		return this->data + 8;
	}

	BitSize_t getBitSize() {
		return this->bitSize - 64;
	}

	SystemAddress getSystemAddress() {
		return this->systemAddress;
	}
};

#endif