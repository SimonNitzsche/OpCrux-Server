#ifndef __REPLICA__COMPONENTS__SOUND_AMBIENT_2D_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__SOUND_AMBIENT_2D_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameObject.hpp"

#include "GameCache/ComponentsRegistry.hpp"
#include "Utils/ServerInfo.hpp"

class SoundAmbient2DComponent : public IEntityComponent {
private:



public:

	SoundAmbient2DComponent(std::int32_t componentID) : IEntityComponent(componentID) {}


	static constexpr std::int16_t GetTypeID() { return 55; }

};

#endif