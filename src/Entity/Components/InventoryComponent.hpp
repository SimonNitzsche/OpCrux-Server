#ifndef __REPLICA__COMPONENTS__INVENTORY_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__INVENTORY_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include <map>

#include "GameCache/InventoryComponent.hpp"

/*
	TODO: This component is currently only implemented for static inventory, change this in future.
*/

/* Maybe replace this in future. */
struct InventoryItemStack {
	std::int32_t LOT;
	DataTypes::LWOOBJID objectID;
	std::uint32_t quantity;
	bool equip;
};

class InventoryComponent : public IEntityComponent {
private:
	bool _isDirtyFlagEquippedItems = false;
	bool _isDirtyFlagNextStruct = false;


public:
	std::map<std::uint32_t, InventoryItemStack> inventory = {};

	InventoryComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 17; }

	void OnEnable() {
		// Get componentID
		std::uint32_t componentID = CacheComponentsRegistry::GetComponentID(owner->GetLOT(), GetTypeID());
		// Get static inventory
		GameCache::Interface::FDB::RowInfo rowInfo = CacheInventoryComponent::getRow(componentID);

		std::uint32_t slotID = 0;

		while (rowInfo.isValid()) {
			// Create static items
			std::int32_t itemID = CacheInventoryComponent::GetItemID(rowInfo);
			std::int32_t count = CacheInventoryComponent::GetCount(rowInfo);
			bool equip = CacheInventoryComponent::GetEquip(rowInfo);
			
			if (itemID >= 17000)
				throw new std::runtime_error("Invalid LOT: " + std::to_string(itemID));

			Entity::GameObject * item = new Entity::GameObject(owner->GetZoneInstance(), itemID);
			item->SetObjectID(DataTypes::LWOOBJID((1ULL << 58) + 104120439353844ULL + owner->GetZoneInstance()->spawnedObjectIDCounter++));
			owner->GetZoneInstance()->objectsManager->RegisterObject(item);


			InventoryItemStack itemStack = InventoryItemStack();
			itemStack.LOT = itemID;
			itemStack.objectID = item->GetObjectID();
			itemStack.quantity = count;
			itemStack.equip = equip;

			// Add item to list
			inventory.insert({ slotID++, itemStack });
		
			// Continue
			if (rowInfo.isLinkedRowInfoValid())
				rowInfo = rowInfo.getLinkedRowInfo();
			else
				break;
		}

		if (owner->GetLOT() == 1) {
			InventoryItemStack itemStack = InventoryItemStack();
			itemStack.LOT = 6086;
			itemStack.objectID = 1152921507005357158;
			itemStack.quantity = 1;
			itemStack.equip = true;
			inventory.insert({ slotID++, itemStack });
		}
		
		_isDirtyFlagEquippedItems = slotID != 0;

	}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		/* TODO: Inventory Component Serialization */
		factory->Write(_isDirtyFlagEquippedItems);
		if (_isDirtyFlagEquippedItems) {
			std::vector<InventoryItemStack> equippedItems{};
			for (auto itemStack : inventory) {
				if (itemStack.second.equip) {
					InventoryItemStack iis = itemStack.second;
					equippedItems.push_back(iis);
				}
			}

			factory->Write<std::uint32_t>(equippedItems.size());
			for (int i = 0; i < equippedItems.size(); ++i) {
				factory->Write<std::int64_t>(equippedItems.at(i).objectID);
				factory->Write<std::int32_t>(equippedItems.at(i).LOT);
				factory->Write(false);
				factory->Write(true);
				/**/factory->Write<std::uint32_t>(equippedItems.at(i).quantity);
				factory->Write(true);
				/**/factory->Write<std::uint16_t>(i);
				factory->Write(false);
				factory->Write(false);
				factory->Write(false);
			}
		}
		factory->Write(false);
	}

};

#endif