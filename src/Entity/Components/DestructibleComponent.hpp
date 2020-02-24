#ifndef __REPLICA__COMPONENTS__DESTRUCTIBLE_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__DESTRUCTIBLE_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "Entity/Components/StatsComponent.hpp"
#include "Entity/Components/CharacterComponent.hpp"

#include "Entity/GameObject.hpp"

#include "GameCache/ComponentsRegistry.hpp"
#include "GameCache/CurrencyTable.hpp"
#include "GameCache/DestructibleComponent.hpp"

#include "Entity/GameMessages/RequestDie.hpp"

#include "Utils/LDFUtils.hpp"

class DestructibleComponent : public IEntityComponent {
private:
	bool _isDirtyFlag = false;

	StatsComponent * statsComponent;

	bool isDead = false;

public:

	DestructibleComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 7; }

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
		if (statsComponent->attributes.maxImagination < imag) {
			statsComponent->attributes.maxImagination = imag;
		}
		statsComponent->SetDirty();
	}

	std::int32_t GetImagination() {
		return statsComponent->attributes.currentImagination;
	}

	void PopulateFromLDF(LDFCollection * collection) {

		LDF_GET_VAL_FROM_COLLECTION(statsComponent->attributes.isSmashable, collection, u"is_smashable", true);
		
	}
	
	std::int32_t GetCurrencyDrop(std::uint32_t npcLevel) {

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
		return (rand() % CacheCurrencyTable::GetMaxValue(usingRow)) + CacheCurrencyTable::GetMinValue(usingRow);
	}

	void OnRequestDie(Entity::GameObject* sender, GM::RequestDie* msg) {
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

	void PerformDamageRequest(Entity::GameObject* caster, std::uint32_t damage) {
		// Cancle if no damage
		if (damage == 0) return;

		// Cancle if dead
		if (isDead) return;

		// TODO: Check for buffs


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

		// Checkout changes
		this->statsComponent->SetDirty();
		// Force push
		this->owner->GetZoneInstance()->objectsManager->Serialize(this->owner);

		if (isDead) {
			{
				GM::Die msg;
				msg.killerID = caster->GetObjectID();
				GameMessages::Broadcast(owner, msg);
			}

			// Figure out who of the two receives the loot.
			Entity::GameObject* lootOwner = caster;
			// To do so, check if the caster is a player, if so he receives it, otherwise we receive it.
			if (lootOwner->GetComponent<CharacterComponent>() == nullptr) {
				lootOwner = this->owner;

				// TODO: We need to extend the logic in the future: teams & player spawned turrets need to be handled aswell.
			}

			{
				// TODO: DropLoot
				GM::DropClientLoot msg;
				msg.iCurrency = GetCurrencyDrop(lootOwner->GetComponent<CharacterComponent>()->GetLevel());
				msg.owner = lootOwner->GetObjectID();
				msg.sourceObj = owner->GetObjectID();
				msg.spawnPosition = owner->GetPosition();
				msg.finalPosition = owner->GetPosition();

				GameMessages::Send(lootOwner, msg.sourceObj, msg);
			}

			// Remove
			owner->Remove();

			// Cancle anything else
			return;
		}
	}
};



#endif