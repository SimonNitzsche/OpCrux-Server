
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

		auto minDrop = CacheLootMatrix::GetMinToDrop(lootMatrixIt);
		auto maxDrop = CacheLootMatrix::GetMaxToDrop(lootMatrixIt);

		std::uniform_int_distribution<> dropCountDist(minDrop, maxDrop);
		std::int32_t dropCount = dropCountDist(RandomUtil::GetEngine());

		auto lootTable = CacheLootTable::getRows(lootTableIndex);

		// Skip, since we're empty
		if (lootTable.size() == 0) continue;

		for (int i = 0; i < dropCount; ++i) {
			std::uniform_int_distribution<> dropItemIndexDist(0, lootTable.size() - 1);
			std::int32_t dropItemIndex = dropItemIndexDist(RandomUtil::GetEngine());

			auto dropItemIt = std::next(lootTable.begin(), dropItemIndex);
			invComp->AddItem(owner->GetProxyItemCheck(CacheLootTable::GetItemID(*dropItemIt)));
		}
	}
}

#endif