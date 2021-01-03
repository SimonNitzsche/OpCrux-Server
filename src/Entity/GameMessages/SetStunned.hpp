#ifndef __ENTITY__GM__SetStunned_HPP__
#define __ENTITY__GM__SetStunned_HPP__

#include "Entity/GameMessages.hpp"
#include "../../Enums/EStunState.hpp"

namespace GM {
	struct SetStunned : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_SET_STUNNED;
		}

        DataTypes::LWOOBJID Originator = 0ULL;

        EStunState StunState;

        bool bCantAttack;
        bool bCantAttackOutChangeWasApplied;

        bool bCantEquip;
        bool bCantEquipOutChangeWasApplied;

        bool bCantInteract;
        bool bCantInteractOutChangeWasApplied;

        bool bCantJump;
        bool bCantJumpOutChangeWasApplied;

        bool bCantMove;
        bool bCantMoveOutChangeWasApplied;

        bool bCantTurn;
        bool bCantTurnOutChangeWasApplied;

        bool bCantUseItem;
        bool bCantUseItemOutChangeWasApplied;

        bool bDontTerminateInteract;
        bool bIgnoreImmunity;

		SetStunned() : GMBase() {}

		void Serialize(RakNet::BitStream* bs) {
            GM_VAR_SERIALIZE_WITH_DEFAULT(bs, Originator, 0ULL);

            GM_VAR_SERIALIZE(bs, (int32_t)StunState);

            GM_VAR_SERIALIZE(bs, bCantAttack);
            GM_VAR_SERIALIZE(bs, bCantAttackOutChangeWasApplied);
            GM_VAR_SERIALIZE(bs, bCantEquip);
            GM_VAR_SERIALIZE(bs, bCantEquipOutChangeWasApplied);
            GM_VAR_SERIALIZE(bs, bCantInteract);
            GM_VAR_SERIALIZE(bs, bCantInteractOutChangeWasApplied);
            GM_VAR_SERIALIZE(bs, bCantJump);
            GM_VAR_SERIALIZE(bs, bCantJumpOutChangeWasApplied);
            GM_VAR_SERIALIZE(bs, bCantMove);
            GM_VAR_SERIALIZE(bs, bCantMoveOutChangeWasApplied);
            GM_VAR_SERIALIZE(bs, bCantTurn);
            GM_VAR_SERIALIZE(bs, bCantTurnOutChangeWasApplied);
            GM_VAR_SERIALIZE(bs, bCantUseItem);
            GM_VAR_SERIALIZE(bs, bCantUseItemOutChangeWasApplied);
            GM_VAR_SERIALIZE(bs, bDontTerminateInteract);
            GM_VAR_SERIALIZE(bs, bIgnoreImmunity);

		}
	};
}

#endif