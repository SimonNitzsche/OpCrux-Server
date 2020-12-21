#ifndef __REPLICA__COMPONENTS__FACTION_TRIGGER_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__FACTION_TRIGGER_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

class FactionTriggerComponent : public IEntityComponent {
public:

	FactionTriggerComponent(std::int32_t componentID) : IEntityComponent(componentID) {}


	static constexpr int GetTypeID() { return 72; }

	void OnItemEquipped(Entity::GameObject* sender, GM::ItemEquipped* msg) {
		// Equip
		if (msg->bIsEquipped) {
			{
				GM::FactionTriggerItemEquipped nmsg;
				nmsg.itemID = owner;
				nmsg.playerID = msg->playerID;
				owner->CallMessage(nmsg);
			}
		}
		// UnEquip
		else {
			{
				GM::FactionTriggerItemUnEquipped nmsg;
				nmsg.itemID = owner;
				nmsg.playerID = msg->playerID;
				owner->CallMessage(nmsg);
			}
		}
	}

	void RegisterMessageHandlers() {
		REGISTER_OBJECT_MESSAGE_HANDLER(FactionTriggerComponent, GM::ItemEquipped, OnItemEquipped);
	}

};

#endif