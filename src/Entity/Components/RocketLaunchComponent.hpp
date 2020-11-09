#ifndef __REPLICA__COMPONENTS__ROCKET_LAUNCH_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__ROCKET_LAUNCH_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameObject.hpp"

#include "GameCache/ComponentsRegistry.hpp"

using namespace DataTypes;

class RocketLaunchComponent : public IEntityComponent {
private:

	std::u16string targetScene;
	std::int32_t targetZone;
	std::u16string transferZoneID;

public:

	RocketLaunchComponent(std::int32_t componentID) : IEntityComponent(componentID) {}


	static constexpr int GetTypeID() { return 67; }

	void OnRequestUse(Entity::GameObject* sender, GM::RequestUse& msg);

	void OnFireEventServerSide(Entity::GameObject* sender, GM::FireEventServerSide& msg);

	void PopulateFromLDF(LDFCollection* collection);

};

#endif