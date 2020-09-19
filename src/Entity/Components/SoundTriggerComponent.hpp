#ifndef __REPLICA__COMPONENTS__SOUND_TRIGGER_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__SOUND_TRIGGER_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameObject.hpp"

#include "GameCache/ComponentsRegistry.hpp"
#include "Utils/ServerInfo.hpp"

class SoundTriggerComponent : public IEntityComponent {
private:



public:

	SoundTriggerComponent(std::int32_t componentID) : IEntityComponent(componentID) {}


	static constexpr std::int16_t GetTypeID() { return 77; }

};

#endif