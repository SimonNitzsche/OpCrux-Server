#ifndef __REPLICA__COMPONENTS__INVENTORY_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__INVENTORY_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include <map>

#include "GameCache/InventoryComponent.hpp"
#include "GameCache/ItemComponent.hpp"
#include "Missions/MissionManager.hpp"
#include "Database/Database.hpp"
#include "Entity/GameMessages.hpp"

/*
	TODO: This component is currently only implemented for static inventory, change this in future.
*/

/* Maybe replace this in future. */
struct InventoryItemStack {
	std::int32_t LOT = -1;
	DataTypes::LWOOBJID ownerID = 0ULL;
	DataTypes::LWOOBJID objectID = 0ULL;
	DataTypes::LWOOBJID subkey = 0ULL;
	std::uint32_t quantity = 1;
	std::uint32_t slot = 0;
	std::uint32_t tab = 0;
	bool equip = false;
	bool bound = false;
	LDFCollection metadata;

	DatabaseModels::ItemModel toDBModel() {
		ItemModel model;
		model.templateID = LOT;
		model.objectID = objectID;
		model.subkey = subkey;
		model.count = quantity;
		model.slot = slot;
		model.tab = tab;
		model.attributes.SetEquipped(equip);
		model.attributes.SetBound(bound);
		model.metadata = metadata;
		model.ownerID = ownerID;
		return model;
	}
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
		TEMPORARY_MODELS = 6,
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

			Entity::GameObject* item = new Entity::GameObject(owner->GetZoneInstance(), itemID);
			item->SetObjectID(owner->GetZoneInstance()->objectsManager->GenerateSpawnedID());
			item->SetIsServerOnly();
			owner->GetZoneInstance()->objectsManager->RegisterObject(item);


			InventoryItemStack itemStack = InventoryItemStack();
			itemStack.LOT = itemID;
			itemStack.objectID = item->GetObjectID();
			itemStack.quantity = count;
			itemStack.equip = equip;
			itemStack.tab = 0;

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
	}

	void Awake() {
		if (owner->GetObjectID() == 0ULL) {
			Logger::log("HELP", "SOMETHING WENT WRONG!!! OWNER IS 0ULL!!! INVENTORY COMPONENT!!!", LogType::ERR);
			throw;
		}

		if (owner->GetLOT() == 1 && owner->GetObjectID() != 0ULL) {
			auto playerBags = Database::GetFullInventory(owner->GetZoneInstance()->GetDBConnection(), owner->GetObjectID().getPureID());
			auto objMan = this->owner->GetZoneInstance()->objectsManager;
			for (auto playerInv : playerBags) {
				auto playerItems = playerInv.second;
				for (auto it = playerItems.begin(); it != playerItems.end(); ++it) {
					InventoryItemStack itemStack = InventoryItemStack();
					itemStack.LOT = it->templateID;
					itemStack.objectID = it->objectID;
					itemStack.quantity = it->count;
					itemStack.equip = it->attributes.GetEquipped();
					itemStack.bound = it->attributes.GetBound();
					itemStack.metadata = it->metadata;
					itemStack.subkey = it->subkey;
					itemStack.tab = it->tab;
					itemStack.ownerID = it->ownerID;
					itemStack.slot = it->slot;

					Entity::GameObject* itmObj = new Entity::GameObject(owner->GetZoneInstance(), itemStack.LOT);
					itmObj->SetObjectID(itemStack.objectID);
					objMan->RegisterObject(itmObj);
					itmObj->isSerializable = false;

					auto tabIt = inventory.find(it->tab);
					if (tabIt != inventory.end()) {
						// make sure our slot is not being taken up
						auto targetSlot = it->slot;
						// TODO check for max
						while (targetSlot < 240) {
							// We do not have item? break
							auto targetSlotIt = tabIt->second.find(targetSlot);
							if(targetSlotIt == tabIt->second.end()) break;
							// Are we the same item? break
							if (targetSlotIt->second.objectID == itemStack.objectID) break;
							// We have an item there already, count slot up
							++targetSlot;
						}

						if (targetSlot != it->slot) {
							it->slot = targetSlot;
							itemStack.slot = it->slot;
							Database::UpdateItemFromInventory(owner->GetZoneInstance()->GetDBConnection(), itemStack.toDBModel());
						}

						tabIt->second.insert({ it->slot, itemStack });
					}
					else {
						InventoryTab _tab; _tab.insert({ it->slot, itemStack });
						inventory.insert({ it->tab, _tab });
					}
				}
			}

			// Check for missions
			MissionManager::LaunchTaskEvent(Enums::EMissionTask::GATHER, owner, owner->GetObjectID());

			//_isDirtyFlagEquippedItems = _isDirtyFlagEquippedItems | playerItems.size() != 0;
		}
	}

	void Destruct() {
		if (owner->GetLOT() != 1) return;
		auto objMan = this->owner->GetZoneInstance()->objectsManager;
		for (auto it = inventory.begin(); it != inventory.end(); ++it) {
			for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
				Entity::GameObject* itmObj = objMan->GetObjectByID(it2->second.objectID);
				itmObj->InstantiateRemoval();
			}
		}
	}

	void Serialize(RakNet::BitStream* factory, ReplicaTypes::PacketTypes packetType) {
		/* TODO: Inventory Component Serialization */
		_isDirtyFlagEquippedItems = true;
		factory->Write(_isDirtyFlagEquippedItems);
		if (_isDirtyFlagEquippedItems) {
			std::vector<InventoryItemStack> equippedItems = GetEquippedItems();

			factory->Write<std::uint32_t>(equippedItems.size());
			for (int i = 0; i < equippedItems.size(); ++i) {
				factory->Write<std::uint64_t>(equippedItems.at(i).objectID);
				factory->Write<std::int32_t>(equippedItems.at(i).LOT);
				factory->Write(equippedItems.at(i).subkey != 0ULL);
				if (equippedItems.at(i).subkey != 0ULL)
					/**/factory->Write<std::int64_t>(equippedItems.at(i).subkey);
				factory->Write(true);
				/**/factory->Write<std::uint32_t>(equippedItems.at(i).quantity);
				factory->Write(true);
				/**/factory->Write<std::uint16_t>(i);
				factory->Write(true);
				/**/factory->Write<std::uint32_t>(equippedItems.at(i).tab);
				factory->Write(false); // TODO: Compression!
				factory->Write(true);
			}
		}
		factory->Write(false);
	}

	inline std::vector<InventoryItemStack> GetEquippedItems() {
		std::vector<InventoryItemStack> equippedItems{};
		for (auto itemTab : inventory) {
			for (auto itemStack : itemTab.second) {
				if (itemStack.second.equip) {
					InventoryItemStack iis = itemStack.second;
					equippedItems.push_back(iis);
				}
			}
		}
		return equippedItems;
	}

	inline bool IsItem(Entity::GameObject* item) {
		// Check if ItemComponent is attached
		return item->GetComponentByType(11) != nullptr;
	}

	inline std::int32_t GetDefaultTabForInventoryType(std::int32_t invType) {
		switch (invType) {
			// Items
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 20:
		case 23: {
			return 0;
		}
			   // Bricks
		case 1: {
			return 2;
		}
			  // Models
		case 12:
		case 19:
		case 21:
		case 22: {
			return 5;
		}
			   // Behaviors
		case 10: {
			return 7;
		}
			   // Property
		case 11: {
			return 8;
		}
			   // Mission (HIDDEN)
		case 18: {
			return 12;
		}
		default: {
			return 0;
		}
		}
	}

	inline bool hasEquipped(std::uint32_t LOT) {
		auto equippedItems = GetEquippedItems();
		for (auto it = equippedItems.begin(); it != equippedItems.end(); ++it) {
			if (it->LOT == LOT) return true;
		}
		return false;
	}

	inline bool hasEquipped(DataTypes::LWOOBJID objID) {
		auto equippedItems = GetEquippedItems();
		for (auto it = equippedItems.begin(); it != equippedItems.end(); ++it) {
			if (it->objectID == objID) return true;
		}
		return false;
	}

	inline bool HasItem(std::uint32_t LOT) {
		return GetItem(LOT).LOT == LOT;
	}

	inline InventoryItemStack GetItem(std::uint32_t LOT, bool temporary = false) {
		auto targetTab = GetTabForLOT(LOT, temporary);

		auto itemCompID = CacheComponentsRegistry::GetComponentID(LOT, 11);
		if (itemCompID == -1) return InventoryItemStack();

		//auto equipLocation = CacheItemComponent::GetEquipLocation(itemCompID);
		//if (static_cast<std::string>(equipLocation) == "") return  InventoryItemStack();

		auto tabIt = inventory.find(targetTab);

		// We do not have tab
		if (tabIt == inventory.end()) {
			// Let's also look for temporary
			targetTab = targetTab == 0 ? 4 : 6;
			tabIt = inventory.find(targetTab);
			// We really do not have it
			if (tabIt == inventory.end())
				return InventoryItemStack();
		}

		for (auto it = tabIt->second.begin(); it != tabIt->second.end(); ++it) {
			if (it->second.LOT == LOT) {
				// We found it!
				return it->second;
			}
		}

		if (!temporary) {
			// We couldn't find it permanentely, let's look for temporary
			return GetItem(LOT, true);
		}

		// We couldn't find the item
		return InventoryItemStack();
	}


	inline bool EquipItem(std::uint32_t LOT) {
		auto targetTab = GetTabForLOT(LOT);

		auto itemCompID = CacheComponentsRegistry::GetComponentID(LOT, 11);
		if (itemCompID == -1) return false;

		auto equipLocation = CacheItemComponent::GetEquipLocation(itemCompID);
		if (static_cast<std::string>(equipLocation) == "") return false;

		auto tabIt = inventory.find(targetTab);

		// We do not have tab, unable to equip
		if (tabIt == inventory.end()) {
			// Let's also look for temporary
			targetTab = targetTab == 0 ? 4 : 6;
			tabIt = inventory.find(targetTab);
			// We really do not have it
			if (tabIt == inventory.end())
				return false;
		}

		for (auto it = tabIt->second.begin(); it != tabIt->second.end(); ++it) {
			if (it->second.LOT == LOT) {
				return EquipItem(it->second.objectID);
			}
		}

		// We couldn't find the item
		return false;
	}

	inline bool EquipItem(DataTypes::LWOOBJID itemToEquip) {
		Entity::GameObject* objItemToEquip = this->owner->GetZoneInstance()->objectsManager->GetObjectByID(itemToEquip);
		if (objItemToEquip == nullptr) {
			Logger::log("WRLD", "Couldn't equip: Couldn't find equipment " + std::to_string(itemToEquip), LogType::ERR);
			return false;
		}

		std::int32_t LOT = objItemToEquip->GetLOT();

		auto itemCompID = CacheComponentsRegistry::GetComponentID(LOT, 11);
		if (itemCompID == -1) {
			Logger::log("WRLD", "Couldn't equip: itemComp is -1", LogType::ERR);
			return false;
		}

		auto equipLocation = CacheItemComponent::GetEquipLocation(itemCompID);
		if (static_cast<std::string>(equipLocation) == "") {
			Logger::log("WRLD", "Couldn't equip: no equip location specified", LogType::ERR);
			return false;
		}


		for (auto tabIt = inventory.begin(); tabIt != inventory.end(); ++tabIt) {
			auto targetTab = GetTabForLOT(LOT);
			for (auto it = tabIt->second.begin(); it != tabIt->second.end(); ++it) {
				if (it->second.objectID == itemToEquip) {
					// We found it!

					// Make sure we have nothing equipped on the location
					this->UnEquipLocation(equipLocation);

					// We're already equipped!
					if (it->second.equip) { 
						Logger::log("WRLD", "Couldn't equip: already equipped", LogType::WARN);
						return false;
					}

					// Equip it.
					it->second.equip = true;

					// Save equip
					this->SaveStack(it->second);

					// Sync equip.
					this->_isDirtyFlagEquippedItems = true;
					this->owner->SetDirty();

					// Sub-items
					auto subitems = StringUtils::StringVectorToIntList(StringUtils::splitString(CacheItemComponent::GetSubItems(itemCompID), ','));
					for (auto subitem : subitems) {
						// Get subitem stack
						auto subItemStack = GetItem(subitem);

						// Do we have item?
						if (subItemStack.LOT != subitem) {
							// We don't so let's add it
							AddItem(subitem, 1U, DataTypes::Vector3(), 0ULL, {}, true);
							// Do we have it now?
							subItemStack = GetItem(subitem);
							if (subItemStack.LOT != subitem) {
								// Nope, it can't be added.
								continue;
							}
						}

						// Alright, now equip our subitem
						if (!EquipItem(subItemStack.objectID))
							Logger::log("WRLD", "Couldn't equip subItem LOT " + std::to_string(subItemStack.LOT));

						GM::EquipInventory eqInvGM;
						eqInvGM.itemToEquip = subItemStack.objectID;
						owner->OnMessage(owner, eqInvGM.GetID(), &eqInvGM);
					}

					Logger::log("WRLD", "Equipped LOT " + std::to_string(it->second.LOT));

					// We're done
					return true;
				}
			}
		}

		// We couldn't find the item
		return false;
	}

	inline bool UnEquipLocation(FDB::PointerString location) {
		std::string locationStr = location;
		for (auto tabIt = inventory.begin(); tabIt != inventory.end(); ++tabIt) {
			for (auto it = tabIt->second.begin(); it != tabIt->second.end(); ++it) {
				auto itemCompID = CacheComponentsRegistry::GetComponentID(it->second.LOT, 11);
				if (itemCompID == -1) return false;

				auto equipLocation = CacheItemComponent::GetEquipLocation(itemCompID).operator std::string();
				if (equipLocation == locationStr) {
					// We found it!

					// We're not already equipped!
					if (!it->second.equip) continue;

					// Equip it.
					it->second.equip = false;

					// Save equip
					this->SaveStack(it->second);

					// Sync equip.
					this->_isDirtyFlagEquippedItems = true;
					this->owner->SetDirty();

					// We're done
					return true;
				}
			}
		}

		// We couldn't find the item
		return false;
	}

	inline bool UnEquipItem(std::uint32_t LOT) {
		auto targetTab = GetTabForLOT(LOT);

		auto itemCompID = CacheComponentsRegistry::GetComponentID(LOT, 11);
		if (itemCompID == -1) return false;

		auto equipLocation = CacheItemComponent::GetEquipLocation(itemCompID);
		if (static_cast<std::string>(equipLocation) == "") return false;

		auto tabIt = inventory.find(targetTab);

		// We do not have tab, unable to unequip
		if (tabIt == inventory.end()) {
			// Let's also look for temporary
			targetTab = targetTab == 0 ? 4 : 6;
			tabIt = inventory.find(targetTab);
			// We really do not have it
			if (tabIt == inventory.end()) return false;
		}

		for (auto it = tabIt->second.begin(); it != tabIt->second.end(); ++it) {
			if (it->second.LOT == LOT) {
				// We found it!

				// We're already equipped!
				if (!it->second.equip) continue;

				// Equip it.
				it->second.equip = false;

				// Save equip
				this->SaveStack(it->second);

				// Sync equip.
				this->_isDirtyFlagEquippedItems = true;
				this->owner->SetDirty();

				// We're done
				return true;
			}
		}

		// We couldn't find the item
		return false;
	}

	inline bool UnEquipItem(DataTypes::LWOOBJID itemToUnEquip) {
		Entity::GameObject* objItemToEquip = this->owner->GetZoneInstance()->objectsManager->GetObjectByID(itemToUnEquip);
		if (objItemToEquip == nullptr) return false;

		std::int32_t LOT = objItemToEquip->GetLOT();

		auto targetTab = GetTabForLOT(LOT);

		auto itemCompID = CacheComponentsRegistry::GetComponentID(LOT, 11);
		if (itemCompID == -1) return false;

		auto equipLocation = CacheItemComponent::GetEquipLocation(itemCompID);
		if (static_cast<std::string>(equipLocation) == "") return false;

		auto tabIt = inventory.find(targetTab);

		// We do not have tab, unable to unequip
		if (tabIt == inventory.end()) {
			// Let's also look for temporary
			targetTab = targetTab == 0 ? 4 : 6;
			tabIt = inventory.find(targetTab);
			// We really do not have it
			if (tabIt == inventory.end()) return false;
		}

		for (auto it = tabIt->second.begin(); it != tabIt->second.end(); ++it) {
			if (it->second.objectID == itemToUnEquip) {
				// We found it!

				// We're already equipped!
				if (!it->second.equip) return false;

				// Equip it.
				it->second.equip = false;

				// Save equip
				this->SaveStack(it->second);

				// Sync equip.
				this->_isDirtyFlagEquippedItems = true;
				this->owner->SetDirty();

				// We're done
				return true;
			}
		}

		// We couldn't find the item
		return false;
	}

	inline void SaveStack(InventoryItemStack stack) {
		// Dont save thinking hat!
		if (stack.LOT == 6068) return;

		// Dont save temporary item or temporary model
		if (stack.tab == 4 || stack.tab == 6) return;

		// Check if we need to remove item or update it.
		if (stack.quantity == 0) {
			// Remove
			Database::RemoveItemFromInventory(owner->GetZoneInstance()->GetDBConnection(), stack.objectID);
		}
		else {
			// Update
			Database::UpdateItemFromInventory(owner->GetZoneInstance()->GetDBConnection(), stack.toDBModel());
		}
	}

	inline std::int32_t GetTabForLOT(std::uint32_t LOT, bool temporary = false) {
		// Get Item component ID
		auto itemCompID = CacheComponentsRegistry::GetComponentID(LOT, 11);

		// Check if component has been found
		if (itemCompID == -1) return -1;

		// Get Item coponent row
		auto itemCompRow = CacheItemComponent::getRow(itemCompID);

		// Check if row is found
		if (!itemCompRow.isValid()) return -1;

		// Get item type
		std::int32_t invType = CacheItemComponent::GetItemType(itemCompRow);

		// Get tab
		auto defaultTab = GetDefaultTabForInventoryType(invType);

		if (temporary) {
			return defaultTab == 0 ? 4 : 6;
		}

		return defaultTab;
	}

	inline std::int32_t GetStackSizeForLOT(std::uint32_t LOT) {
		// Get Item component ID
		auto itemCompID = CacheComponentsRegistry::GetComponentID(LOT, 11);

		// Check if component has been found
		if (itemCompID == -1) return -1;

		// Get Item coponent row
		auto itemCompRow = CacheItemComponent::getRow(itemCompID);

		// Check if row is found
		if (!itemCompRow.isValid()) return -1;

		// Get tab
		return CacheItemComponent::GetStackSize(itemCompRow);
	}

	/*
		Returns uint32 with structure:
		[u16] - tab
		[u16] - slot

		When failed returns 0xFFFFFFFF
	*/
	inline std::uint32_t GetNextFreeSlot(std::uint32_t LOT, bool temporary = false) {
		// Get Tab
		std::int32_t tab = GetTabForLOT(LOT, temporary);
		// Check if tab exists
		if (tab == -1) return 0xFFFFFFFF;

		// Get tab in inventory
		auto itTab = inventory.find(tab);

		// Preallocate slot variable
		std::int32_t slot = 0;

		// Get stack size.
		std::int32_t stackSize = GetStackSizeForLOT(LOT);
		if (stackSize < 1) stackSize = 999;

		// Check if tab in inventory exists, otherwise use slot 0
		bool isFull = true;
		if (itTab != inventory.end()) {
			// Check for free spot
			// The max of the biggest slot is 240
			for (int i = 0; i < 240; ++i) {
				// Get slot at index
				auto itSlot = itTab->second.find(i);

				// Check if item exists at slot, if then check if same LOT and stackable, or if not, set slot and break
				if (itSlot == itTab->second.end() || (itSlot->second.LOT == LOT && itSlot->second.quantity < stackSize)) {
					slot = i;
					isFull = false;
					break;
				}
			}

			// We don't have slot 0 if we reach here
			if (isFull) {
				// If we reach here, inventory is full
				return 0xFFFFFFFF;
			}
		}

		// Return tab and slot as structure
		return (tab << 16) | slot;
	}

	void AddItem(Entity::GameObject* item, std::uint32_t incCount = 1) {

		// Check if item
		if (!IsItem(item)) {
			Logger::log("WRLD", "Can't add item with LOT " + std::to_string(item->GetLOT()) + ", because there is no ItemComponent attached.", LogType::ERR);
			return;
		}

		AddItem(item->GetLOT(), incCount, item->GetPosition());
	}
	void AddItem(std::int32_t itemLOT, std::uint32_t incCount = 1, DataTypes::Vector3 sourcePos = DataTypes::Vector3(), DataTypes::LWOOBJID iSubKey = 0ULL, LDFCollection metadata = {}, bool subItem = false) {
		std::uint32_t nextTabAndSlot = GetNextFreeSlot(itemLOT, subItem);

		std::uint32_t tab = (nextTabAndSlot & 0xFFFF0000) >> 16;
		std::uint32_t slot = (nextTabAndSlot & 0x0000FFFF);

		std::uint32_t overflowCount = 0;

		// Check if next slot has been determined
		if (nextTabAndSlot == 0xFFFFFFFF || slot > 240) {
			Logger::log("WRLD", "Can't add item with LOT " + std::to_string(itemLOT) + ", because we couldn't find an empty place.", LogType::ERR);
			return;
		}

		//Logger::log("WRLD", "Slot #" + std::to_string(slot));

		// Get stack size.
		std::int32_t stackSize = GetStackSizeForLOT(itemLOT);
		if (stackSize < 1) stackSize = 999;

		// TODO: Check if inventory is full.

		auto tabIt = inventory.find(tab);

		// Make Item
		InventoryItemStack itemStack;

		// Check if we already have the item for stacking
		bool useStacking = false;
		if (tabIt != inventory.end()) {
			auto slotIt = tabIt->second.find(slot);

			// We have an item there
			if (slotIt != tabIt->second.end()) {
				useStacking = true;

				// inc stack
				slotIt->second.quantity += incCount;

				if (slotIt->second.quantity > stackSize) {
					overflowCount = slotIt->second.quantity - stackSize;
					slotIt->second.quantity = stackSize;
					incCount = incCount - overflowCount;
				}

				// use stack
				itemStack = slotIt->second;
			}
		}

		//Logger::log("WRLD", "Stack item? "+std::string(useStacking?"true":"false"));

		// We don't have a stack, make a new one.
		if (!useStacking) {
			itemStack.ownerID = owner->GetObjectID().getPureID();
			itemStack.LOT = itemLOT;
			itemStack.equip = false;
			itemStack.bound = false;
			itemStack.subkey = iSubKey;
			itemStack.quantity = 1;
			itemStack.tab = tab;
			if (owner->GetLOT() == 1 && tab != 4 && tab != 6) { // player and not temporary item and not temporary model
				itemStack.objectID = (1ULL << 60) | Database::reserveCountedID(owner->GetZoneInstance()->GetDBConnection(), Database::DBCOUNTERID::PLAYER);
			}
			else {
				itemStack.objectID = owner->GetZoneInstance()->objectsManager->GenerateSpawnedID();
			}
			itemStack.metadata = metadata;

			// Subkey stuff
			// TODO: More specific for pets and such
			itemStack.subkey = iSubKey;

			// metadata stuff
			itemStack.metadata = metadata;

			// Add item to player
			if (tabIt != inventory.end()) {
				tabIt->second.insert({ slot, itemStack });
			}
			else {
				InventoryTab _tab; _tab.insert({ slot, itemStack });
				inventory.insert({ tab, _tab });
			}
		}

		// Check if object is registered or not
		Entity::GameObject* itemObj = owner->GetZoneInstance()->objectsManager->GetObjectByID(itemStack.objectID);
		if (itemObj == nullptr) {
			itemObj = new Entity::GameObject(owner->GetZoneInstance(), itemStack.LOT);
			itemObj->SetObjectID(itemStack.objectID);
			itemObj->PopulateFromLDF(&itemStack.metadata);
			itemObj->SetIsServerOnly();
			itemObj->Finish();
			owner->GetZoneInstance()->objectsManager->RegisterObject(itemObj);
		}

		// Sync with DB and client.
		if (owner->GetLOT() == 1) {
			DatabaseModels::ItemModel itemModel;
			itemModel.attributes.SetEquipped(itemStack.equip);
			itemModel.attributes.SetBound(itemStack.bound);
			itemModel.count = itemStack.quantity;
			itemModel.metadata = itemStack.metadata;
			itemModel.objectID = itemStack.objectID;
			itemModel.ownerID = owner->GetObjectID().getPureID();
			itemModel.slot = slot;
			itemModel.subkey = itemStack.subkey;
			itemModel.tab = tab;
			itemModel.templateID = itemStack.LOT;

			// Sync with client
			GM::AddItemToInventoryClientSync addItemGM;
			addItemGM.bBound = itemStack.bound;
			addItemGM.extraInfo = itemStack.metadata;
			addItemGM.invType = itemModel.tab;
			addItemGM.iObjTemplate = itemModel.templateID;
			addItemGM.iSubkey = itemModel.subkey;
			addItemGM.itemCount = incCount;
			addItemGM.itemsTotal = itemModel.count;
			addItemGM.newObjID = itemModel.objectID;
			addItemGM.slotID = itemModel.slot;
			addItemGM.eLootTypeSource = 0;
			addItemGM.showFlyingLoot = true;
			addItemGM.ni3FlyingLootPosit = sourcePos;
			GameMessages::Send(owner, owner->GetObjectID(), addItemGM);

			// Sync with db
			if (itemModel.tab != 4 && tab != 6) { // If not temporary item and not temporary model
				if (useStacking) {
					Database::UpdateItemFromInventory(owner->GetZoneInstance()->GetDBConnection(), itemModel);
					Logger::log("WRLD", "Update item DB");
				}
				else {
					Database::AddItemToInventory(owner->GetZoneInstance()->GetDBConnection(), itemModel);
					Logger::log("WRLD", "Add item DB");
				}
			}
			MissionManager::LaunchTaskEvent(Enums::EMissionTask::GATHER, owner, owner->GetObjectID());
		}

		if (overflowCount > 0)
			AddItem(itemLOT, overflowCount, sourcePos);

		// TODO: Implement Skill Casting
		// TODO: Implement ObjectSkills auto add component SkillComponent.
		// TODO: Add item to DB.
		//DatabaseModels::ItemModel itemModel;
		//itemModel.count = 1;
		//itemModel.
		//Database::AddItemToInventory(itemModel);
		// TODO: GM::AddItemToClientSync
	}

	void RemoveItem(std::uint32_t itemLot, std::uint32_t amountToRemove = 1) {
		auto targetTab = GetTabForLOT(itemLot);

		auto itemCompID = CacheComponentsRegistry::GetComponentID(itemLot, 11);
		if (itemCompID == -1) return;

		auto equipLocation = CacheItemComponent::GetEquipLocation(itemCompID);
		if (static_cast<std::string>(equipLocation) == "") return;

		auto tabIt = inventory.find(targetTab);
		if (tabIt == inventory.end()) return;

		for (auto it = tabIt->second.begin(); it != tabIt->second.end(); ++it) {
			if (it->second.LOT == itemLot) {
				if (it->second.quantity - amountToRemove <= 0) {
					Database::RemoveItemFromInventory(owner->GetZoneInstance()->GetDBConnection(), it->second.objectID);
				}
				else {
					DatabaseModels::ItemModel itemModel;
					itemModel.attributes.SetEquipped(it->second.equip);
					itemModel.attributes.SetBound(it->second.bound);
					itemModel.count = it->second.quantity - amountToRemove;
					itemModel.metadata = it->second.metadata;
					itemModel.objectID = it->second.objectID;
					itemModel.ownerID = this->owner->GetObjectID().getPureID();
					itemModel.slot = Database::GetSlotOfItemStack(owner->GetZoneInstance()->GetDBConnection(), it->second.objectID);
					itemModel.subkey = it->second.subkey;
					itemModel.tab = it->second.tab;
					itemModel.templateID = it->second.LOT;
					Database::UpdateItemFromInventory(owner->GetZoneInstance()->GetDBConnection(), itemModel);
				}

				GM::RemoveItemFromInventory gm;
				gm.Confirmed = true;
				gm.DeleteItem = true;
				gm.OutSuccess = false;
				gm.ItemType = -1;
				gm.InventoryType = it->second.tab;
				gm.ForceDeletion = true;
				gm.Item = it->second.objectID;
				gm.TotalItems = it->second.quantity - amountToRemove;

				GameMessages::Send(owner, owner->GetObjectID(), gm);
			}
		}

		return;
	}

	void RemoveItem2(std::uint32_t itemLOT, std::uint32_t amount = 1) {

		// at this point amount will be used as amountLeft
		// use while to get all stacks
		while (amount != 0) {
			// Get the stack
			InventoryItemStack stack = GetItem(itemLOT);

			// the stack doesn't exist, no item with the LOT left
			if (stack.LOT != itemLOT) break;

			// Get the safe amount to reduce on the stack
			auto reduceAmount = min(stack.quantity, amount);

			// keep track of amount reduced
			amount -= reduceAmount;

			// Reduce the stack quantity
			stack.quantity -= reduceAmount;

			// Save stack
			SaveStack(stack);

			// Try to unequip item if equipped
			UnEquipItem(stack.objectID);

			// Tell client
			GM::RemoveItemFromInventory removeItemMsg;
			removeItemMsg.Confirmed = true;
			removeItemMsg.DeleteItem = true;
			removeItemMsg.OutSuccess = false;
			removeItemMsg.ItemType = -1;
			removeItemMsg.InventoryType = stack.tab;
			removeItemMsg.ForceDeletion = true;
			removeItemMsg.Item = stack.objectID;
			removeItemMsg.Delta = reduceAmount;
			removeItemMsg.TotalItems = stack.quantity;

			GameMessages::Send(owner, owner->GetObjectID(), removeItemMsg);

			// Is the stack empty now?
			if (stack.quantity <= 0) {
				// Get the object
				Entity::GameObject* invItemObj = owner->GetZoneInstance()->objectsManager->GetObjectByID(stack.objectID);
				// Do we have the object?
				if (invItemObj != nullptr) {
					// Remove object in world
					invItemObj->InstantiateRemoval();
				}
			}
		}
	}

	void OnEquipInventory(Entity::GameObject* sender, GM::EquipInventory* msg) {
		this->EquipItem(msg->itemToEquip);
	}
	void OnUnEquipInventory(Entity::GameObject* sender, GM::UnEquipInventory* msg) {
		this->UnEquipItem(msg->itemToUnEquip);
		sender->GetComponentByType(4)->OnMessage(sender, msg->GetID(), msg);
	}

	void OnClientItemConsumed(Entity::GameObject* sender, GM::ClientItemConsumed* msg) {
		GM::UseItemResult resultMsg;
		resultMsg.m_ItemTemplateID = -1;

		// Use while to emulate sub with return
		while (true) {
			Entity::GameObject* item = sender->GetZoneInstance()->objectsManager->GetObjectByID(msg->item);

			if (item == nullptr) break;

			resultMsg.m_ItemTemplateID = item->GetLOT();

			auto stack = GetItem(item->GetLOT());

			if (stack.LOT != item->GetLOT()) break;

			if (stack.objectID != item->GetObjectID()) break;

			// TODO: Remove on castOnType 3

			resultMsg.m_UseItemResult = true;
			MissionManager::LaunchTaskEvent(EMissionTask::USE_ITEM, item, owner->GetObjectID(), 1, stack.LOT);
			
			item->CallMessage(*msg, sender);

			break;
		}

		GameMessages::Send(sender, owner->GetObjectID(), resultMsg);
	}

	void RegisterMessageHandlers() {
		REGISTER_OBJECT_MESSAGE_HANDLER(InventoryComponent, GM::EquipInventory, OnEquipInventory);
		REGISTER_OBJECT_MESSAGE_HANDLER(InventoryComponent, GM::UnEquipInventory, OnUnEquipInventory);
		REGISTER_OBJECT_MESSAGE_HANDLER(InventoryComponent, GM::ClientItemConsumed, OnClientItemConsumed);
	}
};

#endif