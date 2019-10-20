#ifndef __REPLICA__COMPONENTS__DESTRUCTIBLE_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__DESTRUCTIBLE_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "Entity/Components/StatsComponent.hpp"

#include "Entity/GameObject.hpp"

#include "GameCache/ComponentsRegistry.hpp"
#include "GameCache/DestructibleComponent.hpp"

#include "Utils/LDFUtils.hpp"

class DestructibleComponent : public IEntityComponent {
private:
	bool _isDirtyFlag = false;

	StatsComponent * statsComponent;

public:

	DestructibleComponent() : IEntityComponent() {}

	static constexpr int GetTypeID() { return 7; }

	void OnEnable() {
		if (owner->GetComponent<StatsComponent>() == nullptr) {
			owner->AddComponent<StatsComponent>();
			statsComponent = owner->GetComponent<StatsComponent>();

			if (statsComponent == nullptr) {
				Logger::log("WRLD", "Something went wrong DestuctibleComponent::OnEnable()");
				statsComponent = new StatsComponent();
			}
		}

		StatsComponent::Attributes * attributes = &statsComponent->attributes;
		std::uint32_t compID = CacheComponentsRegistry::GetComponentID(owner->GetLOT(), 7);
		GameCache::Interface::FDB::RowInfo rowInfo = CacheDestructibleComponent::getRow(compID);

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
			factory->Write(false);
			factory->Write(false);
		}

		// Note: This is the first component that is able to serialize the StatsComponent, so no check is necessary.
		if (statsComponent != nullptr)
			statsComponent->Serialize(factory, packetType);
		else
			Logger::log("WRLD", "Couldn't serialize StatsComponent after DestructibleComponent!", LogType::UNEXPECTED);
	}

	void PopulateFromLDF(LDFCollection * collection) {

		LDF_GET_VAL_FROM_COLLECTION(statsComponent->attributes.isSmashable, collection, L"is_smashable", true);
		
	}
};

#endif