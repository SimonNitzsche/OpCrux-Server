#ifndef __COMPONENTS__INTERFACE__IENTITYCOMPONENT_HPP__
#define __COMPONENTS__INTERFACE__IENTITYCOMPONENT_HPP__

#include "Replica/ReplicaTypes.hpp"
#include "RakNet/BitStream.h"

class IEntityComponent {
public:
	class GameObject * owner;

	virtual void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) = 0;
	virtual void Update() = 0;
};

#endif