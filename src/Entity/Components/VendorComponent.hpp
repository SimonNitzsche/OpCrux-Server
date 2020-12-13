#ifndef __REPLICA__COMPONENTS__VENDOR_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__VENDOR_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "GameCache/VendorComponent.hpp"

#include "Entity/GameMessages/VendorOpenWindow.hpp"
#include "Entity/GameMessages/VendorTransactionResult.hpp"

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
		//if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) _isDirtyFlag = true;
		factory->Write(_isDirtyFlag);
		if (_isDirtyFlag) {
			factory->Write(isActive);
			factory->Write(unknownBitB);
		}
		//_isDirtyFlag = false;
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
			GameMessages::Send(sender, owner->GetObjectID(), GM::VendorOpenWindow());
			
			execVendorStatusUpdate(sender);
		}
	}

	void OnBuyFromVendor(Entity::GameObject* sender, GM::BuyFromVendor * msg) {
		// Prepare response
		GM::VendorTransactionResult response;
		response.iResult = 3; // vendor purchase fail

		// Get base cost
		auto itemCompID = CacheComponentsRegistry::GetComponentID(msg->item, 11);
		auto itemCompRow = CacheItemComponent::getRow(itemCompID);
		auto baseValue = CacheItemComponent::GetBaseValue(itemCompRow);

		// Use multiplier to get real cost
		std::float_t pieceValue = baseValue * buyScalar;

		// Calculate buy price
		std::int32_t sumValue = pieceValue * msg->count;

		// Do boundary test (can player buy that much?)
		auto charComp = sender->GetComponent<CharacterComponent>();
		if (charComp != nullptr) {
			auto charInfo = charComp->GetCharInfo();

			if (charInfo.currency >= sumValue) {
				// Remove currency
				charInfo.currency -= sumValue;
				Database::UpdateChar(owner->GetZoneInstance()->GetDBConnection(), charInfo);
				charComp->InitCharInfo(charInfo);

				// Add item
				InventoryComponent* invComp = sender->GetComponent<InventoryComponent>();

				if (invComp != nullptr) {
					invComp->AddItem(msg->item, msg->count, DataTypes::Vector3(), std::uint64_t(0), {
						LDF_COLLECTION_INIT_ENTRY(u"_Metric_Currency_Delta_Int", -sumValue),
						LDF_COLLECTION_INIT_ENTRY(u"_Metric_Source_LOT_Int", owner->GetLOT())
					});

					// Respond success
					response.iResult = 2; // vendor-purchase-success
				}
			}
		}
		
		// respond 
		GameMessages::Send(sender, owner->GetObjectID(), response);
	}

	void RegisterMessageHandlers() {
		REGISTER_OBJECT_MESSAGE_HANDLER(VendorComponent, GM::RequestUse, OnRequestUse);
		REGISTER_OBJECT_MESSAGE_HANDLER(VendorComponent, GM::BuyFromVendor, OnBuyFromVendor);
	}
};

#endif