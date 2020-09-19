#ifndef __REPLICA__COMPONENTS__HF_LIGHT_DIRECTION_GADGET_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__HF_LIGHT_DIRECTION_GADGET_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

class HFLightDirectionGadgetComponent : public IEntityComponent {
public:

	HFLightDirectionGadgetComponent(std::int32_t componentID) : IEntityComponent(componentID) {}


	static constexpr int GetTypeID() { return 66; }

};

#endif