#pragma pack(4)
struct PacketHeader {
    uint8_t protocolID;
    uint16_t remoteType;
    uint32_t packetID;
};

typedef PACKET_HANDLER void()

class PacketHandler {
private:
	PACKET_HANDLER handler;
public:
	PacketHandler
}