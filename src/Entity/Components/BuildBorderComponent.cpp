#include "Entity/Components/BuildBorderComponent.hpp"
#include "Entity/GameObject.hpp"
#include "Entity/GameMessages/RequestUse.hpp"
#include "Entity/Components/InventoryComponent.hpp"

void BuildBorderComponent::OnRequestUse(Entity::GameObject* sender, GM::RequestUse* msg) {
	// Get inventory

	GM::StartArrangingWithItem myMsg;
	myMsg.buildAreaID = owner->GetObjectID();
	myMsg.buildStartPOS = sender->GetPosition();
	myMsg.sourceTYPE = 8;

	GameMessages::Broadcast(sender, myMsg);
}

void BuildBorderComponent::OnSetBuildMode(Entity::GameObject* sender, GM::SetBuildMode* msg) {
	auto invComp = sender->GetComponent<InventoryComponent>();

	if (msg->bStart) {
		// auto equip hat
		if (!invComp->hasEquipped(thinkingHatLOT)) {
			// We do no have it equipped, auto equip
			invComp->EquipItem(thinkingHatLOT);
		}
	}
	else {
		// auto unequip hat
		if (invComp->hasEquipped(thinkingHatLOT)) {
			// We do no have it equipped, auto equip
			invComp->UnEquipItem(thinkingHatLOT);
		}
	}
}

void BuildBorderComponent::OnStartBuildingWithItem(Entity::GameObject* sender, GM::StartBuildingWithItem* msg) {
	// Check if thinking hat, if not return
	if (msg->sourceLOT != thinkingHatLOT) return;

	// Get inventory
	auto invComp = sender->GetComponent<InventoryComponent>();

	// auto equip hat
	if (!invComp->hasEquipped(thinkingHatLOT)) {
		// We do no have it equipped, auto equip
		invComp->EquipItem(thinkingHatLOT);
	}

	GM::StartArrangingWithItem myMsg;
	myMsg.buildAreaID = owner->GetObjectID();
	myMsg.buildStartPOS = sender->GetPosition();
	myMsg.sourceTYPE = 8;

	GameMessages::Broadcast(sender, myMsg);
}

void BuildBorderComponent::OnModularBuildFinish(Entity::GameObject* sender, GM::ModularBuildFinish* msg) {


	auto invComp = sender->GetComponent<InventoryComponent>();

	std::string buildString = "";
	for (auto it = msg->modules.begin(); it != msg->modules.end(); ++it) {
		if (buildString.size() != 0) buildString += "+";
		buildString += "1:" + std::to_string(*it);
	}
	LDFCollection metadata = { LDF_COLLECTION_INIT_ENTRY(u"assemblyPartLOTs", std::u16string(buildString.begin(), buildString.end())) };

	if (msg->count == 3) {
		// Rocket
		if (!invComp->HasItem(6416)) {
			auto subkey = DataTypes::LWOOBJID((std::uint64_t(1) << 60) + std::uint64_t(104120439353844) + Database::reserveCountedID(owner->GetZoneInstance()->GetDBConnection(), Database::DBCOUNTERID::PLAYER));
			invComp->AddItem(6416, 1U, owner->GetPosition(), subkey, metadata);
		}

		if (Database::HasMission(owner->GetZoneInstance()->GetDBConnection(), sender->GetObjectID().getPureID(), 809)) {
			auto mis = Database::GetMission(owner->GetZoneInstance()->GetDBConnection(), sender->GetObjectID().getPureID(), 809);
			if (mis.state == 2) {
				MissionManager::LaunchTaskEvent(EMissionTask::SCRIPT, owner, sender->GetObjectID());
			}
		}
	}
}