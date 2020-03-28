#ifndef __REPLICA__COMPONENTS__INVENTORY_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__INVENTORY_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include <map>

#include "GameCache/InventoryComponent.hpp"
#include "GameCache/ItemComponent.hpp"

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
	typedef 
	std::map<std::uint32_t, InventoryItemStack> InventoryTab;

	std::map<std::uint32_t, InventoryTab> inventory;

	enum class EInventoryTab {
		ITEMS = 0,
		VAULT_ITEMS = 1,
		BRICKS = 2,
		TEMPORARY_ITEMS = 4,
		MODELS = 5,
		TEMPORARY_MODELS= 6,
		BEHAVIORS = 7,
		PROPERTY_DEEDS = 8,
		HIDDEN = 12,
		VAULT_MODELS = 14
	};

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
			item->SetIsServerOnly();
			owner->GetZoneInstance()->objectsManager->RegisterObject(item);


			InventoryItemStack itemStack = InventoryItemStack();
			itemStack.LOT = itemID;
			itemStack.objectID = item->GetObjectID();
			itemStack.quantity = count;
			itemStack.equip = equip;

			// Get item type
			auto itemCompID = CacheComponentsRegistry::GetComponentID(itemID, 11);
			std::int32_t itemType = 0;
			if (itemCompID != -1) {
				itemType = CacheItemComponent::GetItemType(itemCompID);
			}

			// Add item to list
			//inventory.insert({ slotID++, itemStack });

			//inventory.insert(std::pair<std::uint32_t, std::pair<std::uint32_t, InventoryItemStack>>(itemType, { slotID++, itemStack }));
		
			auto tabIt = inventory.find(itemType);
			if (tabIt != inventory.end()) {
				tabIt->second.insert({ slotID++, itemStack });
			}
			else {
				InventoryTab _tab; _tab.insert({ slotID++, itemStack });
				inventory.insert({ itemType, _tab });
			}

			// Continue
			if (rowInfo.isLinkedRowInfoValid())
				rowInfo = rowInfo.getLinkedRowInfo();
			else
				break;
		}

		if (owner->GetLOT() == 1) {
			auto playerItems = Database::GetInventoryItemsOfTab(owner->GetObjectID().getPureID(), 0);

			for (auto it = playerItems.begin(); it != playerItems.end(); ++it) {
				InventoryItemStack itemStack = InventoryItemStack();
				itemStack.LOT = it->templateID;
				itemStack.objectID = it->objectID;
				itemStack.quantity = it->count;
				itemStack.equip = it->attributes.GetEquipped();
				
				auto tabIt = inventory.find(it->tab);
				if (tabIt != inventory.end()) {
					tabIt->second.insert({ it->slot, itemStack });
				}
				else {
					InventoryTab _tab; _tab.insert({ it->slot, itemStack });
					inventory.insert({ it->tab, _tab });
				}
			}

			/*InventoryItemStack itemStack = InventoryItemStack();
			itemStack.LOT = 6086;
			itemStack.objectID = 1152921507005357158;
			itemStack.quantity = 1;
			itemStack.equip = true;
			inventory.insert({ slotID++, itemStack });*/
		}
		
		_isDirtyFlagEquippedItems = slotID != 0;

	}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		/* TODO: Inventory Component Serialization */
		_isDirtyFlagEquippedItems = true;
		factory->Write(_isDirtyFlagEquippedItems);
		if (_isDirtyFlagEquippedItems) {
			std::vector<InventoryItemStack> equippedItems{};
			for (auto itemTab : inventory) {
				for (auto itemStack : itemTab.second) {
					if (itemStack.second.equip) {
						InventoryItemStack iis = itemStack.second;
						equippedItems.push_back(iis);
					}
				}
			}

			factory->Write<std::uint32_t>(equippedItems.size());
			for (int i = 0; i < equippedItems.size(); ++i) {
				factory->Write<std::uint64_t>(equippedItems.at(i).objectID);
				factory->Write<std::int32_t>(equippedItems.at(i).LOT);
				factory->Write(false);
				factory->Write(true);
				/**/factory->Write<std::uint32_t>(equippedItems.at(i).quantity);
				factory->Write(true);
				/**/factory->Write<std::uint16_t>(i);
				factory->Write(false);
				factory->Write(false);
				factory->Write(true);
			}
		}
		factory->Write(false);
	}


	void AddItem(Entity::GameObject* item) {
		// TODO: Check for the item in here and changes

		

		// TODO: Implement Skill Casting
		// TODO: Implement ObjectSkills auto add component SkillComponent.
		// TODO: Add item to DB.
		//DatabaseModels::ItemModel itemModel;
		//itemModel.count = 1;
		//itemModel.
		//Database::AddItemToInventory(itemModel);
		// TODO: GM::AddItemToClientSync
	}
};

#endif