#ifndef __ENTITY__COMPONENTS__SKILLCOMPONENT__BEHAVIORAND_HPP__
#define __ENTITY__COMPONENTS__SKILLCOMPONENT__BEHAVIORAND_HPP__
#include "Entity/Components/SkillComponent/AbstractAggregateBehavior.hpp"

struct BehaviorAnd : AbstractAggregateBehavior {

	void UnCast(SkillComponent * comp, std::int32_t behaviorID, RakNet::BitStream* bs) {

		auto ri = CacheBehaviorParameter::GetBehaviorRow(behaviorID);
		if (!ri.isValid())
			throw std::runtime_error("Unable to query row.");

		// Initial call
		std::int32_t nextID = *reinterpret_cast<float*>(ri[2].getMemoryLocation());
		StartUnCast(comp, nextID, bs);

		// For all others
		while (ri.isLinkedRowInfoValid()) {
			ri = ri.getLinkedRowInfo();
			nextID = *reinterpret_cast<float*>(ri[2].getMemoryLocation());
			StartUnCast(comp, nextID, bs);
		}
	}

	void Cast(SkillComponent* comp, std::int32_t behaviorID, RakNet::BitStream* bs) {

		auto ri = CacheBehaviorParameter::GetBehaviorRow(behaviorID);
		if (!ri.isValid())
			throw std::runtime_error("Unable to query row.");

		// Initial call
		std::int32_t nextID = *reinterpret_cast<float*>(ri[2].getMemoryLocation());
		StartCast(comp, nextID, bs);

		// For all others
		while (ri.isLinkedRowInfoValid()) {
			ri = ri.getLinkedRowInfo();
			nextID = *reinterpret_cast<float*>(ri[2].getMemoryLocation());
			StartCast(comp, nextID, bs);
		}
	}
};
#endif