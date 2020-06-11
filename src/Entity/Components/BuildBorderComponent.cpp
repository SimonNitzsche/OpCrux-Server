#include "Entity/Components/BuildBorderComponent.hpp"
#include "Entity/GameObject.hpp"
#include "Entity/GameMessages/RequestUse.hpp"
#include "Entity/Components/InventoryComponent.hpp"

void BuildBorderComponent::OnRequestUse(Entity::GameObject* sender, GM::RequestUse& msg) {
	// IMAGINATION HELMET LOT
	const std::uint32_t thinkingHatLOT = 6086;

	// Get inventory
	auto invComp = sender->GetComponent<InventoryComponent>();

	// auto equip hat
	if (!invComp->hasEquipped(thinkingHatLOT)) {
		// We do no have it equipped, auto equip
		invComp->EquipItem(thinkingHatLOT);
	}
}