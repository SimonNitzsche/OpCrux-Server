#ifndef __ENTITY__GM__CastSkill_HPP__
#define __ENTITY__GM__CastSkill_HPP__
#include "Entity/GameMessages.hpp"

#include "Enums/EKillType.hpp"

namespace GM {
	struct CastSkill : public GMBase {
		DataTypes::LWOOBJID optionalOriginatorID;
		std::float_t fCasterLatency;
		bool succeeded;
		std::int32_t iCastType;
		DataTypes::LWOOBJID optionalTargetID;
		DataTypes::Quaternion originatorRot;
		DataTypes::LWOOBJID consumableItemID;
		bool bUsedMouse;
		DataTypes::Vector3 lastClickedPosit;
		std::float_t timeToRecast;
		bool bLocalCastOnly;
		std::int32_t skillID;

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::INTERN_CAST_SKILL;
		}

		CastSkill() : GMBase() {}
	};
}

#endif