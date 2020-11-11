#ifndef __REPLICA__COMPONENTS__VENDOR_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__VENDOR_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "GameCache/VendorComponent.hpp"

#include "Entity/GameMessages/VendorOpenWindow.hpp"

using namespace DataTypes;

class VendorComponent : public IEntityComponent {
private:
	bool _isDirtyFlag = true;

	bool isActive = true;
	bool unknownBitB = false;

	std::float_t buyScalar;
	std::float_t sellScalar;
	std::float_t refreshTimeSeconds;
	std::int32_t lootMatrixIndex;
	std::int32_t lootTableIndex;
public:

	VendorComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 16; }

	void OnEnable() {
		auto cvcRow = CacheVendorComponent::getRow(GetComponentID());

		buyScalar = CacheVendorComponent::GetBuyScalar(cvcRow);
		sellScalar = CacheVendorComponent::GetSellScalar(cvcRow);
		refreshTimeSeconds = CacheVendorComponent::GetRefreshTimeSeconds(cvcRow);
		lootMatrixIndex = CacheVendorComponent::GetLootMatrixIndex(cvcRow);
	}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) _isDirtyFlag = true;
		factory->Write(_isDirtyFlag);
		if (_isDirtyFlag) {
			factory->Write(isActive);
			factory->Write(unknownBitB);
		}
		_isDirtyFlag = false;
	}

	void execVendorStatusUpdate(Entity::GameObject * sender) {
		auto lmr = CacheLootMatrix::getRow(lootMatrixIndex);
		auto ltr = CacheLootTable::getRows(CacheLootMatrix::GetLootTableIndex(lmr));

		GM::VendorStatusUpdate nmsg;
		
		for (auto r : ltr) {
			nmsg.inventoryList.push_back({ CacheLootTable::GetItemID(r), std::uint32_t(CacheLootTable::GetSortPriority(r)) });
		}

		GameMessages::Send(sender, owner->GetObjectID(), nmsg);
	}

	void OnRequestUse(Entity::GameObject * sender, GM::RequestUse * msg) {
		CharacterComponent * charComp = sender->GetComponent<CharacterComponent>();
		if (charComp != nullptr) {
			GameMessages::Send(owner->GetZoneInstance(), charComp->clientAddress, sender->GetObjectID(), GM::VendorOpenWindow());
			
			execVendorStatusUpdate(sender);
		}
	}

};

#endif