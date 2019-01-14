#ifndef __ENTITY__COMPONENTS__INTERFACE__IENTITYCOMPONENT_HPP__
#define __ENTITY__COMPONENTS__INTERFACE__IENTITYCOMPONENT_HPP__

#include "Replica/ReplicaTypes.hpp"
#include "RakNet/BitStream.h"

namespace Entity::Components::Interface{

	/*
		Components require to extend this class.
	*/
	class IEntityComponent {
		public:

			// The Owner Game Object.
			class GameObject * owner;

			// Called when Component Requires Serialization.
			virtual void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) = 0;

			// Called when the components needs update.
			virtual void Update() = 0;
	};
}

#endif // !__ENTITY__COMPONENTS__INTERFACE__IENTITYCOMPONENT_HPP__
