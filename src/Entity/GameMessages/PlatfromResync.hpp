#ifndef __ENTITY__GM__PlatformResync_HPP__
#define __ENTITY__GM__PlatformResync_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct PlatformResync : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_PLATFORM_RESYNC;
		}

		bool bReverse = false;
		bool bStopAtDesiredWaypoint = false;
		std::int32_t eCommand = 0;
		std::int32_t eState = 0;
		std::int32_t eUnexpectedCommand = 0;
		std::float_t fIdleTimeElapsed = 0.0f;
		std::float_t fMoveTimeElapsed = 0.0f;
		std::float_t fPercentBetweenPoints = 0.0f;
		std::int32_t iDesiredWaypointIndex = 0;
		std::int32_t iIndex = 0;
		std::int32_t iNextIndex = 0;
		DataTypes::Vector3 ptUnexpectedLocation;
		DataTypes::Quaternion ptUnexpectedRotation;

		PlatformResync() : GMBase() {}
		

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE(bs, bReverse);
			GM_VAR_SERIALIZE(bs, bStopAtDesiredWaypoint);
			GM_VAR_SERIALIZE(bs, eCommand);
			GM_VAR_SERIALIZE(bs, eState);
			GM_VAR_SERIALIZE(bs, eUnexpectedCommand);
			GM_VAR_SERIALIZE(bs, fIdleTimeElapsed);
			GM_VAR_SERIALIZE(bs, fMoveTimeElapsed);
			GM_VAR_SERIALIZE(bs, fPercentBetweenPoints);
			GM_VAR_SERIALIZE(bs, iDesiredWaypointIndex);
			GM_VAR_SERIALIZE(bs, iIndex);
			GM_VAR_SERIALIZE(bs, iNextIndex);
			GM_VAR_SERIALIZE(bs, ptUnexpectedLocation);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, ptUnexpectedRotation, DataTypes::Quaternion());
		}
	};
}

#endif