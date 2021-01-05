#ifndef __ENTITY__COMPONENTS__SKILLCOMPONENT__ABSTRACTAGGREGATEBEHAVIOR_HPP__
#define __ENTITY__COMPONENTS__SKILLCOMPONENT__ABSTRACTAGGREGATEBEHAVIOR_HPP__
#include "RakNet/BitStream.h"

class SkillComponent;

struct AbstractAggregateBehavior {
	virtual void UnCast(SkillComponent* comp, std::int32_t behaviorID, RakNet::BitStream* bs) {};
	virtual void Cast(SkillComponent* comp, std::int32_t behaviorID, RakNet::BitStream* bs) {};
	static void StartUnCast(SkillComponent* comp, std::int32_t nextBehavior, RakNet::BitStream* bs);
	static void StartCast(SkillComponent* comp, std::int32_t nextBehavior, RakNet::BitStream* bs);
};


#include "Entity/Components/SkillComponent.hpp"
#include "Entity/Components/SkillComponent/BehaviorAlterChainDelay.hpp"
#include "Entity/Components/SkillComponent/BehaviorAlterCooldown.hpp"
#include "Entity/Components/SkillComponent/BehaviorAnd.hpp"
#include "Entity/Components/SkillComponent/BehaviorAreaOfEffect.hpp"
#include "Entity/Components/SkillComponent/BehaviorAttackDelay.hpp"
#include "Entity/Components/SkillComponent/BehaviorBasicAttack.hpp"
#include "Entity/Components/SkillComponent/BehaviorChain.hpp"
#include "Entity/Components/SkillComponent/BehaviorChangeOrientation.hpp"
#include "Entity/Components/SkillComponent/BehaviorDuration.hpp"
#include "Entity/Components/SkillComponent/BehaviorForceMovement.hpp"
#include "Entity/Components/SkillComponent/BehaviorInterrupt.hpp"
#include "Entity/Components/SkillComponent/BehaviorKnockback.hpp"
#include "Entity/Components/SkillComponent/BehaviorMovementSwitch.hpp"
#include "Entity/Components/SkillComponent/BehaviorSpawnObject.hpp"
#include "Entity/Components/SkillComponent/BehaviorSpawnQuickbuild.hpp"
#include "Entity/Components/SkillComponent/BehaviorStun.hpp"
#include "Entity/Components/SkillComponent/BehaviorTacArc.hpp"

#include "Exceptions/ExNetException.hpp"

enum class eBehaviorTemplate : std::uint32_t {
	__INVALID__ = 0,
	BASIC_ATTACK = 1,
	TAC_ARC,
	AND,
	PROJECTILE_ATTACK,
	HEAL,
	MOVEMENT_SWITCH,
	AREA_OF_EFFECT,
	PLAY_EFFECT,
	IMMUNITY,
	DAMAGE_BUFF,
	DAMAGE_ABSORPTION,
	OVER_TIME,
	IMAGINATION,
	TARGET_CASTER,
	STUN,
	DURATION,
	KNOCKBACK,
	ATTACK_DELAY,
	CAR_BOOST,
	FALL_SPEED,
	SHIELD,
	REPAIR_ARMOR,
	SPEED,
	DARK_INSPIRATION,
	LOOT_BUFF,
	VENTURE_VISION,
	SPAWN_OBJECT,
	LAY_BRICK,
	SWITCH,
	BUFF,
	JETPACK,
	SKILL_EVENT,
	CONSUME_ITEM,
	SKILL_CAST_FAILED,
	IMITATION_SKUNK_STINK,
	CHANGE_IDLE_FLAGS,
	APPLY_BUFF,
	CHAIN,
	CHANGE_ORIENTATION,
	FORCE_MOVEMENT,
	INTERRUPT,
	ALTER_COOLDOWN,
	CHARGE_UP,
	SWITCH_MULTIPLE,
	START,
	END,
	ALTER_CHAIN_DELAY,
	CAMERA,
	REMOVE_BUFF,
	GRAB,
	MODULAR_BUILD,
	NPC_COMBAT_SKILL,
	BLOCK,
	VERIFY,
	TAUNT,
	AIR_MOVEMENT,
	SPAWN_QUICKBUILD,
	PULL_TO_POINT,
	PROPERTY_ROTATE,
	DAMAGE_REDUCTION,
	PROPERTY_TELEPORT,
	CLEAR_TARGET,
	TAKE_PICTURE,
	MOUNT,
	SKILL_SET
};

void AbstractAggregateBehavior::StartUnCast(SkillComponent * comp, std::int32_t nextBehavior, RakNet::BitStream* bs) {
	if (nextBehavior <= 0) return;
	++comp->currentStackDepth;
	if (comp->currentStackDepth > 50000) {
		throw Exceptions::NetException::CorruptPacket();
	}

	std::int32_t templateID = CacheBehaviorTemplate::GetTemplateID(nextBehavior);

	Logger::log("WRLD", "[UnCast] behavior template " + std::string(CacheBehaviorTemplateName::GetName(templateID)) + " " + std::to_string(nextBehavior), LogType::UNEXPECTED);

	switch (eBehaviorTemplate(templateID)) {
	case eBehaviorTemplate::BASIC_ATTACK: {
		// Basic Attack
		BehaviorBasicAttack basicAttack = BehaviorBasicAttack();
		basicAttack.UnCast(comp, nextBehavior, bs);
		break;
	}
	case eBehaviorTemplate::TAC_ARC: {
		// Tac Arc
		BehaviorTacArc tacArc = BehaviorTacArc();
		tacArc.UnCast(comp, nextBehavior, bs);
		break;
	}
	case eBehaviorTemplate::AND: {
		// And
		BehaviorAnd bAnd = BehaviorAnd();
		bAnd.UnCast(comp, nextBehavior, bs);
		break;
	}
	case eBehaviorTemplate::MOVEMENT_SWITCH: {
		// Movement Switch
		BehaviorMovementSwitch movementSwitch = BehaviorMovementSwitch();
		movementSwitch.UnCast(comp, nextBehavior, bs);
		break;
	}
	case eBehaviorTemplate::AREA_OF_EFFECT: {
		// Area of Effect
		BehaviorAreaOfEffect areaOfEffect = BehaviorAreaOfEffect();
		areaOfEffect.UnCast(comp, nextBehavior, bs);
	}
	case eBehaviorTemplate::STUN: {
		// Stun
		BehaviorStun stun = BehaviorStun();
		stun.UnCast(comp, nextBehavior, bs);
		break;
	}
	case eBehaviorTemplate::DURATION: {
		// Duration
		BehaviorDuration duration = BehaviorDuration();
		duration.UnCast(comp, nextBehavior, bs);
		break;
	}
	case eBehaviorTemplate::ATTACK_DELAY: {
		// Attack Delay
		BehaviorAttackDelay bAttackDelay = BehaviorAttackDelay();
		bAttackDelay.UnCast(comp, nextBehavior, bs);
		break;
	}
	case eBehaviorTemplate::SPAWN_OBJECT: {
		// Spawn Object
		BehaviorSpawnObject bSpawnObject = BehaviorSpawnObject();
		bSpawnObject.UnCast(comp, nextBehavior, bs);
		break;
	}
	case eBehaviorTemplate::CHAIN: {
		// Chain
		BehaviorChain bChain = BehaviorChain();
		bChain.UnCast(comp, nextBehavior, bs);
		break;
	}
	case eBehaviorTemplate::CHANGE_ORIENTATION: {
		// Change Orientation
		BehaviorChangeOrientation bChangeOrientation = BehaviorChangeOrientation();
		bChangeOrientation.UnCast(comp, nextBehavior, bs);
		break;
	}
	case eBehaviorTemplate::FORCE_MOVEMENT: {
		// Force Movement
		BehaviorForceMovement bForceMovement = BehaviorForceMovement();
		bForceMovement.UnCast(comp, nextBehavior, bs);
		break;
	}
	case eBehaviorTemplate::INTERRUPT: {
		// Interrupt
		BehaviorInterrupt bInterrupt = BehaviorInterrupt();
		bInterrupt.UnCast(comp, nextBehavior, bs);
		break;
	}
	case eBehaviorTemplate::ALTER_COOLDOWN: {
		// Alter Cooldown
		BehaviorAlterCooldown alterCooldown = BehaviorAlterCooldown();
		alterCooldown.UnCast(comp, nextBehavior, bs);
		break;
	}
	case eBehaviorTemplate::KNOCKBACK: {
		// Knockback
		BehaviorKnockback knockback = BehaviorKnockback();
		knockback.UnCast(comp, nextBehavior, bs);
		break;
	}
	case eBehaviorTemplate::ALTER_CHAIN_DELAY: {
		// Alter Chain Delay
		BehaviorAlterChainDelay alterChainDelay = BehaviorAlterChainDelay();
		alterChainDelay.UnCast(comp, nextBehavior, bs);
		break;
	}
	case eBehaviorTemplate::SPAWN_QUICKBUILD: {
		// Spawn Quickbuild
		BehaviorSpawnQuickbuild spawnQB = BehaviorSpawnQuickbuild();
		spawnQB.UnCast(comp, nextBehavior, bs);
	}
	default:
		Logger::log("WRLD", "TODO: Implement behavior template " + std::string(CacheBehaviorTemplateName::GetName(templateID)) + " " + std::to_string(nextBehavior), LogType::UNEXPECTED);
		break;
	}
}

void AbstractAggregateBehavior::StartCast(SkillComponent* comp, std::int32_t nextBehavior, RakNet::BitStream* bs) {
	if (nextBehavior <= 0) return;
	++comp->currentStackDepth;
	if (comp->currentStackDepth > 50000) {
		throw Exceptions::NetException::CorruptPacket();
	}

	std::int32_t templateID = CacheBehaviorTemplate::GetTemplateID(nextBehavior);

	Logger::log("WRLD", "[Cast] behavior template " + std::string(CacheBehaviorTemplateName::GetName(templateID)) + " " + std::to_string(nextBehavior), LogType::UNEXPECTED);

	switch (eBehaviorTemplate(templateID)) {
	case eBehaviorTemplate::BASIC_ATTACK: {
		// Basic Attack
		BehaviorBasicAttack basicAttack = BehaviorBasicAttack();
		basicAttack.Cast(comp, nextBehavior, bs);
		break;
	}
	//case eBehaviorTemplate::TAC_ARC: {
	//	// Tac Arc
	//	BehaviorTacArc tacArc = BehaviorTacArc();
	//	tacArc.Cast(comp, nextBehavior, bs);
	//	break;
	//}
	case eBehaviorTemplate::AND: {
		// And
		BehaviorAnd bAnd = BehaviorAnd();
		bAnd.Cast(comp, nextBehavior, bs);
		break;
	}
	//case eBehaviorTemplate::MOVEMENT_SWITCH: {
	//	// Movement Switch
	//	BehaviorMovementSwitch movementSwitch = BehaviorMovementSwitch();
	//	movementSwitch.Cast(comp, nextBehavior, bs);
	//	break;
	//}
	case eBehaviorTemplate::AREA_OF_EFFECT: {
		// Area of Effect
		BehaviorAreaOfEffect areaOfEffect = BehaviorAreaOfEffect();
		areaOfEffect.Cast(comp, nextBehavior, bs);
	}
	//case eBehaviorTemplate::STUN: {
	//	// Stun
	//	BehaviorStun stun = BehaviorStun();
	//	stun.Cast(comp, nextBehavior, bs);
	//	break;
	//}
	//case eBehaviorTemplate::DURATION: {
	//	// Duration
	//	BehaviorDuration duration = BehaviorDuration();
	//	duration.Cast(comp, nextBehavior, bs);
	//	break;
	//}
	case eBehaviorTemplate::ATTACK_DELAY: {
		// Attack Delay
		BehaviorAttackDelay bAttackDelay = BehaviorAttackDelay();
		bAttackDelay.Cast(comp, nextBehavior, bs);
		break;
	}
	//case eBehaviorTemplate::SPAWN_OBJECT: {
	//	// Spawn Object
	//	BehaviorSpawnObject bSpawnObject = BehaviorSpawnObject();
	//	bSpawnObject.Cast(comp, nextBehavior, bs);
	//	break;
	//}
	//case eBehaviorTemplate::CHAIN: {
	//	// Chain
	//	BehaviorChain bChain = BehaviorChain();
	//	bChain.Cast(comp, nextBehavior, bs);
	//	break;
	//}
	//case eBehaviorTemplate::CHANGE_ORIENTATION: {
	//	// Change Orientation
	//	BehaviorChangeOrientation bChangeOrientation = BehaviorChangeOrientation();
	//	bChangeOrientation.Cast(comp, nextBehavior, bs);
	//	break;
	//}
	//case eBehaviorTemplate::FORCE_MOVEMENT: {
	//	// Force Movement
	//	BehaviorForceMovement bForceMovement = BehaviorForceMovement();
	//	bForceMovement.Cast(comp, nextBehavior, bs);
	//	break;
	//}
	//case eBehaviorTemplate::INTERRUPT: {
	//	// Interrupt
	//	BehaviorInterrupt bInterrupt = BehaviorInterrupt();
	//	bInterrupt.Cast(comp, nextBehavior, bs);
	//	break;
	//}
	//case eBehaviorTemplate::ALTER_COOLDOWN: {
	//	// Alter Cooldown
	//	BehaviorAlterCooldown alterCooldown = BehaviorAlterCooldown();
	//	alterCooldown.Cast(comp, nextBehavior, bs);
	//	break;
	//}
	case eBehaviorTemplate::KNOCKBACK: {
		// Knockback
		BehaviorKnockback knockback = BehaviorKnockback();
		knockback.Cast(comp, nextBehavior, bs);
		break;
	}
	//case eBehaviorTemplate::ALTER_CHAIN_DELAY: {
	//	// Alter Chain Delay
	//	BehaviorAlterChainDelay alterChainDelay = BehaviorAlterChainDelay();
	//	alterChainDelay.Cast(comp, nextBehavior, bs);
	//	break;
	//}
	default:
		Logger::log("WRLD", "[Cast] TODO: Implement behavior template " + std::string(CacheBehaviorTemplateName::GetName(templateID)) + " " + std::to_string(nextBehavior), LogType::UNEXPECTED);
		break;
	}
}
#endif
