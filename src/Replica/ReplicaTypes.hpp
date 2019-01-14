#ifndef __REPLICA__REPLICATYPES_HPP__
#define __REPLICA__REPLICATYPES_HPP__

#include <memory>

namespace ReplicaTypes {
	enum class PacketTypes : uint8_t{
		CONSTRUCTION = 0x24,
		SERIALIZATION = 0x27,
		DESTRUCTION = 0x25
	};
}

#endif // !__REPLICA__REPLICATYPES_HPP__
