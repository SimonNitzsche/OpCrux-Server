#ifndef __REPLICA__COMPONENTS__COLLECTIBLE_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__COLLECTIBLE_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "Entity/Components/StatsComponent.hpp"

#include "Entity/GameObject.hpp"

#include "Utils/LDFUtils.hpp"

class CollectibleComponent : public IEntityComponent {
private:
	StatsComponent * statsComponent;

	std::int32_t collectibleID=0;

public:

	CollectibleComponent() : IEntityComponent() {}

	void OnEnable() {
		if ((statsComponent = static_cast<StatsComponent*>(owner->GetComponentByID(200))) == nullptr) {
			owner->AddComponentByID(200);
			statsComponent = static_cast<StatsComponent*>(owner->GetComponentByID(200));

			if (statsComponent == nullptr) {
				Logger::log("WRLD", "Something went wrong CollectibleComponent::OnEnable()");
				statsComponent = new StatsComponent();
			}
		}
	}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		
		// Check if Destructible component is attached, if so don't serialize
		if(owner->GetComponentByID(7) == nullptr)
			statsComponent->Serialize(factory, packetType);


		factory->Write<std::uint16_t>(collectibleID);
	}

	void PopulateFromLDF(LDFCollection * collection) {
		LDF_GET_VAL_FROM_COLLECTION(collectibleID, collection, L"collectible_id", -1);
	}

};

#endif