#ifndef __REPLICA__COMPONENTS__EXHIBIT_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__EXHIBIT_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

class ExhibitComponent : public IEntityComponent {
public:

	ExhibitComponent(std::int32_t componentID) : IEntityComponent(componentID) {}


	static constexpr std::int16_t GetTypeID() { return 32; }

};

#endif