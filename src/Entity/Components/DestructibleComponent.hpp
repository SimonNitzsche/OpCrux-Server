#ifndef __REPLICA__COMPONENTS__DESTRUCTIBLE_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__DESTRUCTIBLE_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "Entity/Components/StatsComponent.hpp"
#include "Entity/Components/CharacterComponent.hpp"
#include "Entity/Components/ItemComponent.hpp"

#include "Entity/GameObject.hpp"

#include "GameCache/ComponentsRegistry.hpp"
#include "GameCache/CurrencyTable.hpp"
#include "GameCache/DestructibleComponent.hpp"
#include "GameCache/LootMatrix.hpp"
#include "GameCache/LootTable.hpp"

#include "Entity/GameMessages/RequestDie.hpp"

#include "Utils/LDFUtils.hpp"

class DestructibleComponent : public IEntityComponent {
private:
	bool _isDirtyFlag = false;

	StatsComponent * statsComponent;

	bool isDead = false;

	std::int32_t lootMatrixIndex;

public:

	DestructibleComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 7; }

	std::list<ItemModel> GetLootDrop(Entity::GameObject * lootOwner) {
		std::list<ItemModel> dropList = {};

		auto lootRowMain = CacheLootMatrix::getRow(lootMatrixIndex);
		auto lootRows = lootRowMain.flatIt();
		std::uniform_real_distribution<> dropChanceDist(0.0, 1.0);
		for (auto it = lootRows.begin(); it != lootRows.end(); ++it) {
			// Check if we have flag
			std::int32_t flag = CacheLootMatrix::GetFlagID(*it);
			if(flag != 0 && !lootOwner->GetFlag(flag)) continue;

			std::float_t chance = CacheLootMatrix::GetPercent(*it);

			// Check if drop?
			if (dropChanceDist(RandomUtil::GetEngine()) > chance) continue;

			std::int32_t minToDrop = CacheLootMatrix::GetMinToDrop(*it);
			std::int32_t maxToDrop = CacheLootMatrix::GetMaxToDrop(*it);
			maxToDrop = minToDrop > maxToDrop ? minToDrop : maxToDrop;

			std::uniform_real_distribution<> dropCountDist(minToDrop, maxToDrop);

			std::int32_t dropCount = dropCountDist(RandomUtil::GetEngine());

			std::int32_t lootTableIndex = CacheLootMatrix::GetLootTableIndex(*it);

			auto lootTableRows = CacheLootTable::getRows(lootTableIndex);
			lootTableRows.sort([](GameCache::Interface::FDB::RowInfo a, GameCache::Interface::FDB::RowInfo b)
				{return CacheLootTable::GetSortPriority(a) > CacheLootTable::GetSortPriority(b); });

			for (int i = 0; i < dropCount; ++i) {
				std::uniform_real_distribution<> dropIndexDist(0, lootTableRows.size());
				auto it2 = std::next(lootTableRows.begin(), dropIndexDist(RandomUtil::GetEngine()));

				std::int32_t itemID = CacheLootTable::GetItemID(*it2);
				bool isMisssionDrop = CacheLootTable::GetMissionDrop(*it2);

				// Check if we have mission
				if (isMisssionDrop) {
					//continue; // TODO!
					/*if (!Database::HasMission(lootOwner->GetObjectID().getPureID(), misID)) {
						continue;
					}*/
				}

				ItemModel item;
				item.templateID = itemID;
				item.count = 1;
				item.objectID = owner->GetZoneInstance()->objectsManager->GenerateSpawnedID();
				item.ownerID = lootOwner->GetObjectID();

				dropList.push_back(item);
			}
		}
		return dropList;
	}

	void OnEnable() {
		if (owner->GetComponent<StatsComponent>() == nullptr) {
			owner->AddComponent<StatsComponent>(0);
			statsComponent = owner->GetComponent<StatsComponent>();

			if (statsComponent == nullptr) {
				Logger::log("WRLD", "Something went wrong DestuctibleComponent::OnEnable()");
				statsComponent = new StatsComponent(0);
			}
		}

		StatsComponent::Attributes * attributes = &statsComponent->attributes;
		GameCache::Interface::FDB::RowInfo rowInfo = CacheDestructibleComponent::getRow(GetComponentID());

		attributes->currentHealth = CacheDestructibleComponent::GetLife(rowInfo);
		attributes->maxHealth = attributes->currentHealth;
		attributes->dupMaxHealth = attributes->maxHealth;

		attributes->currentArmor = CacheDestructibleComponent::GetArmor(rowInfo);
		attributes->maxArmor = attributes->currentArmor;
		attributes->dupMaxArmor = attributes->maxArmor;

		attributes->currentImagination = CacheDestructibleComponent::GetImagination(rowInfo);
		attributes->maxImagination = attributes->currentImagination;
		attributes->dupMaxImagination = attributes->maxImagination;

		attributes->isSmashable = CacheDestructibleComponent::GetIsSmashable(rowInfo);

		auto faction = CacheDestructibleComponent::GetFaction(rowInfo);
		if(faction != -1)
			attributes->factions.push_back(faction);

		attributes->damageAbsorptionPoints = 0;

		lootMatrixIndex = CacheDestructibleComponent::GetLootMatrixIndex(GetComponentID());
	}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		/* TODO: Destructible Component Serialization */
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
			factory->Write(true);
			factory->Write<std::uint32_t>(0);
			factory->Write(true);
			factory->Write<std::uint32_t>(0);
		}

		// Note: This is the first component that is able to serialize the StatsComponent, so no check is necessary.
		if (statsComponent != nullptr)
			statsComponent->Serialize(factory, packetType);
		else
			Logger::log("WRLD", "Couldn't serialize StatsComponent after DestructibleComponent!", LogType::UNEXPECTED);
	}

	void SetImagination(std::int32_t imag) {
		statsComponent->attributes.currentImagination = imag;
		if (statsComponent->attributes.maxImagination <= imag) {
			statsComponent->attributes.maxImagination = imag;
		}
		statsComponent->SetDirty();
	}

	std::int32_t GetImagination() {
		return statsComponent->attributes.currentImagination;
	}

	void PopulateFromLDF(LDFCollection * collection) {

		LDF_GET_VAL_FROM_COLLECTION(statsComponent->attributes.isSmashable, collection, u"is_smashable", true);
		bool smashable_loot_matrix_set = false;
		LDF_GET_VAL_FROM_COLLECTION(smashable_loot_matrix_set, collection, u"smashable_loot_matrix_set", false);
		if (smashable_loot_matrix_set) {
			LDF_GET_VAL_FROM_COLLECTION(lootMatrixIndex, collection, u"smashable_loot_matrix", lootMatrixIndex);
		}
		
	}
	
	std::int32_t GetCurrencyDrop(std::uint32_t npcLevel) {

		// if npcLevel is 0, don't drop coins
		if (npcLevel == 0) return 0;

		// Get Row with highest min npc level, that is not exceeding npcLevel
		std::int32_t currencyIndex = CacheDestructibleComponent::GetCurrencyIndex(GetComponentID());
		auto mainRow = CacheCurrencyTable::getRow(currencyIndex);
		auto rows = mainRow.flatIt();
		auto usingRow = mainRow;
		for (auto it = rows.begin(); it != rows.end(); ++it) {
			if (CacheCurrencyTable::GetNPCMinLevel(*it) <= npcLevel && CacheCurrencyTable::GetNPCMinLevel(*it) >= CacheCurrencyTable::GetNPCMinLevel(usingRow)) {
				usingRow = *it;
			}
		}

		// init random and return in range
		srand(std::uint32_t(::time(0)));
		std::int32_t randMax = CacheCurrencyTable::GetMaxValue(usingRow);
		if (randMax == 0) return 0;
		return (rand() % randMax) + CacheCurrencyTable::GetMinValue(usingRow);
	}

	void OnRequestDie(Entity::GameObject* sender, GM::RequestDie * msg) {
		GM::Die msgDie;
		msgDie.bSpawnLoot = true;
		msgDie.deathType = msg->deathType;
		msgDie.directionRelative_AngleXZ = msg->directionRelative_AngleXZ;
		msgDie.directionRelative_AngleY = msg->directionRelative_AngleY;
		msgDie.directionRelative_Force = msg->directionRelative_Force;
		msgDie.killType = msg->killType;
		msgDie.killerID = msg->killerID;
		msgDie.lootOwnerID = msg->lootOwnerID;
		GameMessages::Broadcast(this->owner->GetZoneInstance(), this->owner, msgDie);
		this->owner->OnDie(this->owner, &msgDie);
	}

	void ForcefullyPerformRequestDie(Entity::GameObject* sender, GM::RequestDie& msg) {
		statsComponent->attributes.isSmashable = true;

		statsComponent->attributes.currentHealth = 0;
		statsComponent->attributes.currentArmor = 0;

		auto caster = owner->GetZoneInstance()->objectsManager->GetObjectByID(msg.lootOwnerID);
		if (caster == nullptr) caster = owner->GetZoneInstance()->objectsManager->GetObjectByID(msg.killerID);
		if (caster == nullptr) caster = sender;

		PerformDamageRequest(caster, 0xFFFFFFFF);
	}

	void PerformDamageRequest(Entity::GameObject* caster, std::uint32_t damage) {
		// We are not smashable
		if (!statsComponent->attributes.isSmashable) return;

		// Cancle if no damage
		if (damage == 0) { Logger::log("WRLD", "Object received 0 damage", LogType::WARN); return; }

		// Cancle if dead
		if (isDead) { Logger::log("WRLD", "Object is already dead", LogType::WARN); return; }

		// TODO: Check for buffs

		/*Logger::log("WRLD", "Object taking " + std::to_string(damage) + " damage.");
		std::uint32_t oldArmor = statsComponent->attributes.currentArmor;
		std::uint32_t oldHealth = statsComponent->attributes.currentHealth;
		*/

		// If armor < damage -> 0 or below
		if (statsComponent->attributes.currentArmor <= damage) {
			// Set remaining damage
			damage -= statsComponent->attributes.currentArmor;
			// Set armor to 0
			statsComponent->attributes.currentArmor = 0;

			// check if damage is remaining
			if (damage != 0) {
				// If health < remaining damage -> 0 or below
				if (statsComponent->attributes.currentHealth <= damage) {
					// Set health to 0
					statsComponent->attributes.currentHealth = 0;
					// Since health is 0 now we are dead
					isDead = true;
				}
				else {
					// Substract damage from health
					statsComponent->attributes.currentHealth -= damage;
				}
			}
		}
		else {
			// Substract damage
			statsComponent->attributes.currentArmor -= damage;
		}

		//Logger::log("WRLD", "Object armor " + std::to_string(oldArmor) + " -> " + std::to_string(statsComponent->attributes.currentArmor));
		//Logger::log("WRLD", "Object health " + std::to_string(oldHealth) + " -> " + std::to_string(statsComponent->attributes.currentHealth));

		// Checkout changes
		this->statsComponent->SetDirty();
		// Force push
		this->owner->GetZoneInstance()->objectsManager->Serialize(this->owner);

		if (isDead) {
			{
				GM::Die msg;
				msg.killerID = caster->GetObjectID();
				GameMessages::Broadcast(owner, msg);
				this->owner->OnDie(this->owner, &msg);
			}

			// Figure out who of the two receives the loot.
			Entity::GameObject* lootOwner = caster;
			// To do so, check if the caster is a player, if so he receives it, otherwise we receive it.
			if (lootOwner->GetComponent<CharacterComponent>() == nullptr) {
				lootOwner = this->owner;

				// TODO: We need to extend the logic in the future: teams & player spawned turrets need to be handled aswell.
			}

			{
				{
					GM::DropClientLoot msg;
					msg.iCurrency = GetCurrencyDrop(lootOwner->GetLOT() == 1 ? lootOwner->GetComponent<CharacterComponent>()->GetLevel() : 0);
					msg.owner = lootOwner->GetObjectID();
					msg.sourceObj = owner->GetObjectID();
					msg.spawnPosition = owner->GetPosition();
					msg.finalPosition = owner->GetPosition();

					GameMessages::Send(lootOwner, msg.sourceObj, msg);
				}

				auto itemLoot = GetLootDrop(lootOwner);
				for (auto it = itemLoot.begin(); it != itemLoot.end(); ++it) {
					{
						GM::DropClientLoot nmsg;
						nmsg.iCurrency = 0;
						nmsg.owner = lootOwner->GetObjectID();
						nmsg.sourceObj = owner->GetObjectID();
						nmsg.spawnPosition = owner->GetPosition();
						nmsg.finalPosition = owner->GetPosition();
						nmsg.itemTemplate = it->templateID;
						nmsg.lootID = it->objectID;
						GameMessages::Send(lootOwner, nmsg.sourceObj, nmsg);

						Logger::log("WRLD", "Dropped loot " + std::to_string(std::uint64_t(nmsg.lootID)) + " with LOT " + std::to_string(nmsg.itemTemplate));
					}
				}
			}

			// Remove
			owner->InstantiateRemoval();

			// Update Missions
			MissionManager::LaunchTaskEvent(EMissionTask::KILL, this->owner, caster->GetObjectID(), this->owner->GetLOT());

			// Cancle anything else
			return;
		}
	}

	void OnDropLoot(Entity::GameObject* sender, GM::DropClientLoot* msg) {
		Entity::GameObject* droppedLoot = new Entity::GameObject(owner->GetZoneInstance(), msg->itemTemplate);
		droppedLoot->SetObjectID(msg->lootID);
		ItemComponent* lootItemComp = droppedLoot->GetComponent<ItemComponent>();
		owner->GetZoneInstance()->objectsManager->RegisterObject(droppedLoot);
		droppedLoot->SetMaxAge(120);
		droppedLoot->Finish();
	}

	void RegisterMessageHandlers() {
		REGISTER_OBJECT_MESSAGE_HANDLER(DestructibleComponent, GM::RequestDie, OnRequestDie);
		//REGISTER_OBJECT_MESSAGE_HANDLER(DestructibleComponent, GM::Drop)
	}
};



#endif