#ifndef __ENTITY__GMBASE_HPP__
#define __ENTITY__GMBASE_HPP__
#include "Enums/EGameMessages.hpp"

namespace RakNet {
	class BitStream;
}

namespace Entity {
	class GameObject;
}

namespace GM {
	struct GMBase {
		GMBase() {}
		static constexpr Enums::EGameMessageID GetID() { return (Enums::EGameMessageID)(-1); };
		virtual void Deserialize(RakNet::BitStream* bs) {};
		virtual void Serialize(RakNet::BitStream* bs) {};
		virtual void TriggerEvent(Entity::GameObject* sender, Entity::GameObject* target) {};
	};
}

#endif