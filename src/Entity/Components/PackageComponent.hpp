
#ifndef __REPLICA__COMPONENTS__PACKAGE_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__PACKAGE_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameObject.hpp"

#include "GameCache/ComponentsRegistry.hpp"

#include "GameCache/PackageComponent.hpp"
#include "GameCache/LootMatrix.hpp"
#include "GameCache/LootTable.hpp"

using namespace DataTypes;

class PackageComponent : public IEntityComponent {
private:

	std::int32_t lootMatrixIndex = -1;
	std::int32_t packageType = -1;

public:

	PackageComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 53; }

	void OnEnable() {
		auto row = CachePackageComponent::getRow(GetComponentID());
		lootMatrixIndex = CachePackageComponent::GetLootMatrixIndex(row);
		packageType = CachePackageComponent::GetPackageType(row);
	}
	
	void RegisterMessageHandlers() {
		REGISTER_OBJECT_MESSAGE_HANDLER(PackageComponent, GM::UseNonEquipmentItem, OnUseNonEquipmentItem);
	}

	void OnUseNonEquipmentItem(Entity::GameObject* rerouteID, GM::UseNonEquipmentItem* msg);

};

#include "Entity/Components/InventoryComponent.hpp"

void PackageComponent::OnUseNonEquipmentItem(Entity::GameObject* rerouteID, GM::UseNonEquipmentItem* msg) {

	InventoryComponent* invComp = rerouteID->GetComponent<InventoryComponent>();

	if (invComp == nullptr) {
		Logger::log("WRLD", "Unable to consume item when the owner InventoryComponent is nullptr.", LogType::WARN);
		return;
	}

	invComp->RemoveItem2(owner->GetLOT());

	auto lootMatrix = CacheLootMatrix::getRow(lootMatrixIndex).flatIt();
	for (auto lootMatrixIt : lootMatrix) {
		auto lootTableIndex = CacheLootMatrix::GetLootTableIndex(lootMatrixIt);

		auto lootTable = CacheLootTable::getRows(lootTableIndex);
		for (auto lootTableIt : lootTable) {
			invComp->AddItem(owner->GetProxyItemCheck(CacheLootTable::GetItemID(lootTableIt)));
		}
	}
}

#endif