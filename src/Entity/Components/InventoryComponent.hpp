#ifndef __REPLICA__COMPONENTS__INVENTORY_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__INVENTORY_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include <map>

#include "GameCache/InventoryComponent.hpp"
#include "GameCache/ItemComponent.hpp"
#include "Missions/MissionManager.hpp"

/*
	TODO: This component is currently only implemented for static inventory, change this in future.
*/

/* Maybe replace this in future. */
struct InventoryItemStack {
	std::int32_t LOT = -1;
	DataTypes::LWOOBJID objectID = 0ULL;
	DataTypes::LWOOBJID subkey = 0ULL;
	std::uint32_t quantity = 1;
	std::uint32_t tab = 0;
	bool equip = false;
	bool bound = false;
	LDFCollection metadata;
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
			item->SetObjectID(DataTypes::LWOOBJID((1ULL << 58) + 104120439353844ULL + owner->GetZoneInstance()->spawnedObjectIDCounter++));
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
		if (owner->GetLOT() == 1) {
			auto playerBags = Database::GetFullInventory(owner->GetObjectID().getPureID());
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

					Entity::GameObject* itmObj = new Entity::GameObject(owner->GetZoneInstance(), itemStack.LOT);
					itmObj->SetObjectID(itemStack.objectID);
					objMan->RegisterObject(itmObj);
					itmObj->isSerializable = false;

					auto tabIt = inventory.find(it->tab);
					if (tabIt != inventory.end()) {
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
		auto targetTab = GetTabForLOT(LOT);

		auto itemCompID = CacheComponentsRegistry::GetComponentID(LOT, 11);
		if (itemCompID == -1) return false;

		auto equipLocation = CacheItemComponent::GetEquipLocation(itemCompID);
		if (static_cast<std::string>(equipLocation) == "") return false;

		auto tabIt = inventory.find(targetTab);

		// We do not have tab, unable to equip
		if (tabIt == inventory.end()) return false;

		for (auto it = tabIt->second.begin(); it != tabIt->second.end(); ++it) {
			if (it->second.LOT == LOT) {
				// We found it!
				return true;
			}
		}

		// We couldn't find the item
		return false;
	}

	inline InventoryItemStack GetItem(std::uint32_t LOT) {
		auto targetTab = GetTabForLOT(LOT);

		auto itemCompID = CacheComponentsRegistry::GetComponentID(LOT, 11);
		if (itemCompID == -1) return InventoryItemStack();

		auto equipLocation = CacheItemComponent::GetEquipLocation(itemCompID);
		if (static_cast<std::string>(equipLocation) == "") return  InventoryItemStack();

		auto tabIt = inventory.find(targetTab);

		// We do not have tab, unable to equip
		if (tabIt == inventory.end()) return InventoryItemStack();

		for (auto it = tabIt->second.begin(); it != tabIt->second.end(); ++it) {
			if (it->second.LOT == LOT) {
				// We found it!
				return it->second;
			}
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
		if (tabIt == inventory.end()) return false;

		for (auto it = tabIt->second.begin(); it != tabIt->second.end(); ++it) {
			if (it->second.LOT == LOT) {
				// We found it!

				// We're already equipped!
				if (it->second.equip) return false;

				// Make sure we have nothing equipped on the location
				this->UnEquipLocation(equipLocation);

				// Equip it.
				it->second.equip = true;

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

	inline bool EquipItem(DataTypes::LWOOBJID itemToEquip) {
		Entity::GameObject* objItemToEquip = this->owner->GetZoneInstance()->objectsManager->GetObjectByID(itemToEquip);
		if (objItemToEquip == nullptr) return false;

		std::int32_t LOT = objItemToEquip->GetLOT();

		auto targetTab = GetTabForLOT(LOT);

		auto itemCompID = CacheComponentsRegistry::GetComponentID(LOT, 11);
		if (itemCompID == -1) return false;

		auto equipLocation = CacheItemComponent::GetEquipLocation(itemCompID);
		if (static_cast<std::string>(equipLocation) == "") return false;

		auto tabIt = inventory.find(targetTab);

		// We do not have tab, unable to equip
		if (tabIt == inventory.end()) return false;

		for (auto it = tabIt->second.begin(); it != tabIt->second.end(); ++it) {
			if (it->second.objectID == itemToEquip) {
				// We found it!

				// We're already equipped!
				if (it->second.equip) return false;

				// Make sure we have nothing equipped on the location
				this->UnEquipLocation(equipLocation);

				// Equip it.
				it->second.equip = true;

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

		// We do not have tab, unable to equip
		if (tabIt == inventory.end()) return false;

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

		// We do not have tab, unable to equip
		if (tabIt == inventory.end()) return false;

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

		// TODO
	}

	inline std::int32_t GetTabForLOT(std::uint32_t LOT) {
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
		return GetDefaultTabForInventoryType(invType);
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
	inline std::uint32_t GetNextFreeSlot(std::uint32_t LOT) {
		// Get Tab
		std::int32_t tab = GetTabForLOT(LOT);
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
	void AddItem(std::int32_t itemLOT, std::uint32_t incCount = 1, DataTypes::Vector3 sourcePos = DataTypes::Vector3(), DataTypes::LWOOBJID iSubKey = 0ULL, LDFCollection metadata = {}) {
		std::uint32_t nextTabAndSlot = GetNextFreeSlot(itemLOT);

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
			itemStack.LOT = itemLOT;
			itemStack.equip = false;
			itemStack.bound = false;
			itemStack.subkey = iSubKey;
			itemStack.quantity = 1;
			itemStack.tab = tab;
			if (owner->GetLOT() == 1) {
				itemStack.objectID = (1ULL << 60) | Database::reserveCountedID(Database::DBCOUNTERID::PLAYER);
			}
			else {
				itemStack.objectID = DataTypes::LWOOBJID((1ULL << 58) + 104120439353844ULL + owner->GetZoneInstance()->spawnedObjectIDCounter++);
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

			// Sync with db
			if (useStacking) {
				Database::UpdateItemFromInventory(itemModel);
				//Logger::log("WRLD", "Update item DB");
			}
			else {
				Database::AddItemToInventory(itemModel);
				//Logger::log("WRLD", "Add item DB");
			}
			MissionManager::LaunchTaskEvent(Enums::EMissionTask::GATHER, owner, owner->GetObjectID());

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

	void OnEquipInventory(Entity::GameObject* sender, GM::EquipInventory& msg) {
		this->EquipItem(msg.itemToEquip);
	}
	void OnUnEquipInventory(Entity::GameObject* sender, GM::UnEquipInventory& msg) {
		this->UnEquipItem(msg.itemToUnEquip);
		sender->GetComponentByType(4)->OnUnEquipInventory(sender, msg);
	}
};

#endif