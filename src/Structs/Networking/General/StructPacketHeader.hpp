#ifndef __STRUCTS__NETWORKING__GENERAL__STRUCTPACKETHEADER_HPP__
#define __STRUCTS__NETWORKING__GENERAL__STRUCTPACKETHEADER_HPP__

#include <memory>

#pragma pack(push, 1)
struct LUPacketHeader {
	uint8_t protocolID;
	uint16_t remoteType;
	uint32_t packetID;
private:
	uint8_t _;
};
#pragma pack(pop)

#endif