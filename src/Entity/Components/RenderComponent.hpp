#ifndef __REPLICA__COMPONENTS__RENDER_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__RENDER_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include <Utils/LDFUtils.hpp>
#include "GameCache/RenderComponent.hpp"

using namespace DataTypes;

class RenderComponent : public IEntityComponent {
private:

public:

	bool renderDisabled = false;

	RenderComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	struct PlayedEffect {
		std::string effectName = "";
		std::uint32_t effectID = 0;
		std::u16string effectType = u"";
		std::float_t priority = 1.0f;
		std::int64_t secondary = 0ULL;
	};
	std::list<PlayedEffect> effects = {};

	static constexpr int GetTypeID() { return 2; }

	void Serialize(RakNet::BitStream* factory, ReplicaTypes::PacketTypes packetType) {
		if (renderDisabled) return;

		/* TODO: Render Component Serialization */
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
			factory->Write<std::uint32_t>(effects.size());
			for (auto effect : effects) {
				StringUtils::writeStringToBitStream<std::uint8_t>(factory, effect.effectName);
				factory->Write(effect.effectID);
				StringUtils::writeWStringToBitStream<std::uint8_t>(factory, effect.effectType);
				factory->Write(effect.priority);
				factory->Write(effect.secondary);
			}

		}
	}

	FDB::PointerString GetRenderAsset() {
		return CacheRenderComponent::GetRenderAsset(IEntityComponent::GetComponentID());
	}

	void OnPlayFXEffect(Entity::GameObject * sender, GM::PlayFXEffect * msg) {
		// Make sure we don't play it yet
		for (auto e : effects) if (e.effectName == msg->name) return;

		PlayedEffect eff;
		eff.effectID = msg->effectID;
		eff.effectName = msg->name;
		eff.effectType = msg->effectType;
		eff.priority = msg->priority;
		eff.secondary = msg->secondary;
		effects.push_back(eff);
	}

	void OnStopFXEffect(Entity::GameObject * sender, GM::StopFXEffect * msg) {
		auto it = effects.begin();
		for (; it != effects.end(); ++it) {
			if (it->effectName == msg->name) {
				break;
			}
		}
		if (it != effects.end()) {
			effects.erase(it);
		}
	}

	void PopulateFromLDF(LDFCollection* collection) {
		LDF_GET_VAL_FROM_COLLECTION(renderDisabled, collection, u"renderDisabled", false);

		std::uint32_t lot = owner->GetLOT();
		renderDisabled |= (lot == 5937 || lot == 5938 || lot == 9741 || lot == 9742 || lot == 9862 || lot == 9863 /*|| lot == 6368*/);
		if (lot == 1) {
			renderDisabled = true;
		}
		if (lot == 7001) {
			renderDisabled = false;
		}
	}

	void RegisterMessageHandlers() {
		REGISTER_OBJECT_MESSAGE_HANDLER(RenderComponent, GM::PlayFXEffect, OnPlayFXEffect);
		REGISTER_OBJECT_MESSAGE_HANDLER(RenderComponent, GM::StopFXEffect, OnStopFXEffect);
	}
};

#endif