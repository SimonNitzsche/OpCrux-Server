#ifndef __ENTITY__COMPONENTS__INTERFACE__IENTITYCOMPONENT_HPP__
#define __ENTITY__COMPONENTS__INTERFACE__IENTITYCOMPONENT_HPP__

#include "Replica/ReplicaTypes.hpp"
#include "RakNet/BitStream.h"
#include "DataTypes/LWOOBJID.hpp"
#include "DataTypes/LDF.hpp"

#include "GameCache/ComponentsRegistry.hpp"

namespace Entity {
	class GameObject;
}

//namespace Entity::Components::Interface {

	/*
		Components require to extend this class.
	*/
	class IEntityComponent {
		public:

			// The Owner Game Object.
			Entity::GameObject * owner = nullptr;

			// Called when Component Requires Serialization.
			virtual void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {};

			// Called after constructor
			virtual void OnEnable() {}

			// Called after components are added.
			virtual void Awake() {}

			// Called after components are awake
			virtual void Start() {}

			// Called when the components needs update.
			virtual void Update() {}

			// Called when physic update
			virtual void PhysicUpdate() {}

			// Optional void.
			// Used to set components variables on load from LDF like from LUZ.
			virtual void PopulateFromLDF(LDFCollection * collection) {};

			virtual ~IEntityComponent() = default;

			IEntityComponent() = default;

			void SetOwner(Entity::GameObject * obj) {
				owner = obj;
			}
			
	};
//}

//using namespace Entity::Components::Interface;

#endif // !__ENTITY__COMPONENTS__INTERFACE__IENTITYCOMPONENT_HPP__
