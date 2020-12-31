#ifndef __ENTITY__COMPONENTS__INTERFACE__IENTITYCOMPONENT_HPP__
#define __ENTITY__COMPONENTS__INTERFACE__IENTITYCOMPONENT_HPP__

#include "Replica/ReplicaTypes.hpp"
#include "RakNet/BitStream.h"
#include "DataTypes/LWOOBJID.hpp"
#include "DataTypes/LDF.hpp"

#include "GameCache/ComponentsRegistry.hpp"

#include <unordered_map>
#include <functional>

namespace Entity {
	class GameObject;
}

#include "Entity/GMUtils.hpp"
#include "Entity/GMBase.hpp"
namespace GM {
	class AddSkill;
	class BuildExitConfirmation;
	class BuildModeSet;
	class BuyFromVendor;
	class CancelRailMovement;
	class ClientItemConsumed;
	class Die;
	class DoneArrangingWithItem;
	class EquipInventory;
	class HasBeenCollected;
	class MissionDialogueOK;
	class RequestDie;
	class RequestUse;
	class SetFlag;
	class FinishArrangingWithItem;
	class FireEventServerSide;
	class HasBeenCollected;
	class MatchRequest;
	class MissionDialogueOK;
	class ModifyGhostingDistance;
	class ModularAssemblyNIFCompleted;
	class ModularBuildFinish;
	class ModuleAssemblyQueryData;
	class MoveItemInInventory;
	class NotifyClientFlagChange;
	class NotifyServerLevelProcessingComplete;
	class ParseChatMessage;
	class PickupCurrency;
	class PickupItem;
	class PlayEmote;
	class PlayerLoaded;
	class PlayerReady;
	class PopEquippedItemsState;
	class ReadyForUpdates;
	class RebuildCancel;
	class RemoveItemFromInventory;
	class RemoveSkill;
	class ReportBug;
	class RequestDie;
	class RequestLinkedMission;
	class RequestResurrect;
	class RequestSmashPlayer;
	class RequestUse;
	class RequestPlatformResync;
	class ServerTerminateInteraction;
	class SetBuildMode;
	class SetJetPackMode;
	class StartBuildingWithItem;
	class StartSkill;
	class SyncSkill;
	class SetFlag;
	class TerminateInteraction;
	class Teleport;
	class ToggleSendingPositionUpdates;
	class ToggleGhostReferenceOverride;
	class UnEquipInventory;
	class SetTooltipFlag;
	class SetGhostReferencePosition;
	class RespondToMission;
	class SetEmoteLockState;
};

class IEntityComponent;

using MessageHandler = void(*)(IEntityComponent *, Entity::GameObject*, GM::GMBase*);
using MessageHandlerLambda = void(**)(IEntityComponent*, Entity::GameObject*, GM::GMBase*);
//using MessageHandler = void(*)(Entity::GameObject*, Entity::GameObject*);



/* set x true if x or construction */
#define ENABLE_FLAG_ON_CONSTRUCTION(x) \
x = x || packetType == ReplicaTypes::PacketTypes::CONSTRUCTION;

//namespace Entity::Components::Interface {

	/*
		Components require to extend this class.
	*/

#define REGISTER_OBJECT_MESSAGE_HANDLER(ComponentName, GMName, callback) {\
RegisterMessageHandler(this, GMName::GetID(), [](IEntityComponent* instanceContext, Entity::GameObject* rerouteID, GM::GMBase* msg)\
	{ static_cast<ComponentName *>(instanceContext)->callback(rerouteID, static_cast<GMName*>(msg)); });}

/*
RegisterMessageHandler(
	this,
	GM::StartSkill::GetID(),
	[](IEntityComponent* instanceContext, Entity::GameObject* rerouteID, GM::GMBase* msg) {
		static_cast<SkillComponent*>(instanceContext)->DynamicTestOnStartSkill(rerouteID, static_cast<GM::StartSkill*>(msg));
	}
);
*/

class IEntityComponent {
private:
	std::int32_t componentID = -1;


	std::unordered_map<std::uint32_t, std::tuple<IEntityComponent *, MessageHandler>> msgHandlers = {};

	

public:

	// The Owner Game Object.
	Entity::GameObject* owner = nullptr;

	std::int32_t GetComponentID() {
		return componentID;
	}


	// Return the component type
	static constexpr int GetTypeID() { return -1; };

	// Called when Component Requires Serialization.
	virtual void Serialize(RakNet::BitStream* factory, ReplicaTypes::PacketTypes packetType) {};

	// Called on destruct serialization while serialization is only called for construct and update
	virtual void Destruct() {};

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
	virtual void OnCollisionPhantom(Entity::GameObject* other) {};
	virtual void OnOffCollisionPhantom(Entity::GameObject* other) {};


	/* Game Messages */
	//GM_MAKE_LIST_CLIENT(GM_MAKE_COMPONENT_DECLARE);
	//virtual void OnDie(Entity::GameObject* sender, GM::Die* msg) {};
	//virtual void OnHasBeenCollected(Entity::GameObject* sender, GM::HasBeenCollected* msg) {};
	//virtual void OnMissionDialogueOK(Entity::GameObject* sender, GM::MissionDialogueOK* msg) {};
	//virtual void OnRequestDie(Entity::GameObject* sender, GM::RequestDie* msg) {};
	//virtual void OnRequestUse(Entity::GameObject * sender, GM::RequestUse * msg) {};
	//virtual void OnSetFlag(Entity::GameObject* sender, GM::SetFlag* msg) {};

	// Optional void.
	// Used to set components variables on load from LDF like from LUZ.
	virtual void PopulateFromLDF(LDFCollection* collection) {};

	virtual ~IEntityComponent() = default;

	IEntityComponent(std::int32_t componentID) : componentID(componentID) {};

	void SetOwner(Entity::GameObject* obj) {
		owner = obj;
	}

	void OnMessage(Entity::GameObject* rerouteID, std::uint32_t msgID, GM::GMBase* msg) {

		for (auto it = msgHandlers.begin(); it != msgHandlers.end(); ++it) {
			if (it->first != msgID) continue;
			
			auto [context, handler] = it->second;
			handler(context, rerouteID, msg);

			

			//auto test = OnMessage;


			//(*this.*memFun)(rerouteID, msg);
			//(*.*test2)(nullptr, nullptr);
		}
	}

	void RegisterMessageHandler(IEntityComponent * this_00, std::uint32_t msgID, MessageHandler msgHandle) {
		msgHandlers.insert(std::make_pair(msgID, std::make_tuple(this_00, msgHandle)));
	}

	// Called after OnEnable
	// We use a seperate function to allow parent class handler stacking
	virtual void RegisterMessageHandlers() {};

};
//}

//using namespace Entity::Components::Interface;

#endif // !__ENTITY__COMPONENTS__INTERFACE__IENTITYCOMPONENT_HPP__
