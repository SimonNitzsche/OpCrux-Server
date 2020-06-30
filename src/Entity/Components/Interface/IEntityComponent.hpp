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

#include "Entity/GMUtils.hpp"

namespace GM {
	class BuildExitConfirmation;
	class Die;
	class HasBeenCollected;
	class MissionDialogueOK;
	class RequestDie;
	class RequestUse;
	class SetFlag;
	class FireEventServerSide;
	class HasBeenCollected;
	class MissionDialogueOK;
	class ModuleAssemblyQueryData;
	class NotifyClientFlagChange;
	class NotifyServerLevelProcessingComplete;
	class PickupItem;
	class PlayEmote;
	class PlayerLoaded;
	class ReadyForUpdates;
	class RebuildCancel;
	class RequestDie;
	class RequestLinkedMission;
	class RequestResurrect;
	class RequestSmashPlayer;
	class RequestUse;
	class StartBuildingWithItem;
	class StartSkill;
	class SyncSkill;
	class SetFlag;
	class TerminateInteraction;
	class ToggleSendingPositionUpdates;
	class SetTooltipFlag;
	class SetGhostReferencePosition;
};

/* set x true if x or construction */
#define ENABLE_FLAG_ON_CONSTRUCTION(x) \
x = x || packetType == ReplicaTypes::PacketTypes::CONSTRUCTION;

//namespace Entity::Components::Interface {

	/*
		Components require to extend this class.
	*/
		class IEntityComponent {
		private:
			std::int32_t componentID = -1;
		public:

			// The Owner Game Object.
			Entity::GameObject * owner = nullptr;

			std::int32_t GetComponentID() {
				return componentID;
			}

			// Return the component type
			static constexpr int GetTypeID() { return -1; };

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

			/* Non-GMs Events */
			virtual void OnCollisionPhantom(Entity::GameObject * other) {};
			virtual void OnOffCollisionPhantom(Entity::GameObject * other) {};


			/* Game Messages */
			GM_MAKE_LIST_CLIENT(GM_MAKE_COMPONENT_DECLARE);
			virtual void OnDie(Entity::GameObject* sender, GM::Die* msg) {};
			//virtual void OnHasBeenCollected(Entity::GameObject* sender, GM::HasBeenCollected* msg) {};
			//virtual void OnMissionDialogueOK(Entity::GameObject* sender, GM::MissionDialogueOK* msg) {};
			//virtual void OnRequestDie(Entity::GameObject* sender, GM::RequestDie* msg) {};
			//virtual void OnRequestUse(Entity::GameObject * sender, GM::RequestUse * msg) {};
			//virtual void OnSetFlag(Entity::GameObject* sender, GM::SetFlag* msg) {};

			// Optional void.
			// Used to set components variables on load from LDF like from LUZ.
			virtual void PopulateFromLDF(LDFCollection * collection) {};

			virtual ~IEntityComponent() = default;

			IEntityComponent(std::int32_t componentID) : componentID(componentID) {};

			void SetOwner(Entity::GameObject * obj) {
				owner = obj;
			}
			
		};
//}

//using namespace Entity::Components::Interface;

#endif // !__ENTITY__COMPONENTS__INTERFACE__IENTITYCOMPONENT_HPP__
