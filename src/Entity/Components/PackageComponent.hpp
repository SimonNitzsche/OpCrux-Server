
#ifndef __REPLICA__COMPONENTS__PACKAGE_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__PACKAGE_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameObject.hpp"

#include "GameCache/ComponentsRegistry.hpp"

#include "GameCache/PackageComponent.hpp"

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

	void msgClientItemConsumed(Entity::GameObject* rerouteID, GM::ClientItemConsumed* msg) {

	}
	
	void RegisterMessageHandlers() {
		REGISTER_OBJECT_MESSAGE_HANDLER(PackageComponent, GM::ClientItemConsumed, msgClientItemConsumed);
	}

};

#endif