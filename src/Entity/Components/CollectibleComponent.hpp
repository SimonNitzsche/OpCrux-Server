#ifndef __REPLICA__COMPONENTS__COLLECTIBLE_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__COLLECTIBLE_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "Entity/Components/StatsComponent.hpp"
#include "Entity/Components/DestructibleComponent.hpp"

#include "Entity/GameObject.hpp"

#include "Utils/LDFUtils.hpp"

class CollectibleComponent : public IEntityComponent {
private:
	StatsComponent * statsComponent;

	std::int32_t collectibleID=0;

public:

	CollectibleComponent() : IEntityComponent() {}

	static constexpr int GetTypeID() { return 23; }

	void OnEnable() {
		if ((statsComponent = owner->GetComponent<StatsComponent>()) == nullptr) {
			owner->AddComponent<StatsComponent>();
			statsComponent = owner->GetComponent<StatsComponent>();

			if (statsComponent == nullptr) {
				Logger::log("WRLD", "Something went wrong CollectibleComponent::OnEnable()");
				statsComponent = new StatsComponent();
			}
		}
	}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		
		// Check if Destructible component is attached, if so don't serialize
		if(owner->GetComponent<DestructibleComponent>() == nullptr)
			statsComponent->Serialize(factory, packetType);


		factory->Write<std::uint16_t>(collectibleID);
	}

	void PopulateFromLDF(LDFCollection * collection) {
		LDF_GET_VAL_FROM_COLLECTION(collectibleID, collection, u"collectible_id", -1);
	}

};

#endif