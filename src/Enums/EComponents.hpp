#pragma once

// Based off from Xipho's list he has sent in the discord.
// Filled up by Simon based on the information in the fdb file

enum class ECOMPONENT_TYPE : long {
	CONTROLABLE_PHYSICS_COMPONENT = 1,
	RENDER_COMPONENT,
	SIMPLE_PHYSICS_COMPONENT,
	CHARACTER_COMPONENT, //only two in table
	SCRIPT_COMPONENT,
	BOUNCER_COMPONENT,
	DESTRUCTABLE_COMPONENT,
	GHOST_COMPONENT,
	SKILL_COMPONENT,
	SPAWNER_COMPONENT, //used for LOT 176, a spawner
	ITEM_COMPONENT, //used for quite a few objects, they vary too much to determine what it is
	MODULAR_BUILD_COMPONENT, //This is for modular building areas, rocket bays and car bays
	REBUILD_START_COMPONENT, //Rebuildables and Thinking hat has this for some reason
	REBUILD_ACTIVATOR_COMPONENT, //only one in table, but no object matches that id
	ICON_ONLY_RENDER_COMPONENT, //only one in table, but no object matches that id
	VENDOR_COMPONENT,
	INVENTORY_COMPONENT,
	PROJECTILE_PHYSICS_COMPONENT, //this is used by shooting gallery objects
	SHOOTING_GALLERY_COMPONENT, //cannon component? Is used by cannon objects
	RIGID_BODY_PHANTOM_PHYSICS_COMPONENT, //Is used by objects in racing
	DROP_EFFECT_COMPONENT,
	CHEST_COMPONENT, //Only used by a treasure chest
	COLLECTIBLE_COMPONENT, //used by collectable spawners
	BLUEPRINT_COMPONENT, //used in showing a model in the inventory
	MOVING_PLATFORM_COMPONENT, //Is used by moving platforms, could be a moving platform component
	PET_COMPONENT,
	PLATFORM_BOUNDRY_COMPONENT, //another moving platform component, potentially
	MODULE_COMPONENT, //Modular Component? All the objects are pieces to rockets, etc.
	JET_PACK_PAD_COMPONENT, //JetpackComponent? All objects using this have to do with jetpacks
	VEHICLE_PHYSICS_COMPONENT,
	MOVEMENT_AI_COMPONENT, //only enemies have this for the most part
	EXHIBIT_COMPONENT, //Exhibit Component?
	OVERHEAD_ICON_COMPONENT,
	PET_CONTROL_COMPONENT,
	MINIFIG_COMPONENT, //All NPC's have this component...
	PROPERTY_COMPONENT, //This component is used by property related objects
	PET_NEST_COMPONENT, //only one in table, used by LOT 3336, which is described as a nest asset. Possibly a petNestComponent
	MODEL_BUILDER_COMPONENT, //only two in table, LWOModelBuilderComponent is listed in the description of LOT 6228
	SCRIPTED_ACTIVITY_COMPONENT,
	PHANTOM_PHYSICS_COMPONENT,
	SPRINGPAD_COMPONENT, //A component for testing "new springpads" LOT 4816 for example
	MODEL_BEHAVIORS_COMPONENT, //Models, or something...?
	PROPERTY_ENTRANCE_COMPONENT, //Property Lauchpad components
	FX_COMPONENT, //Not one object uses this
	PROPERTY_MANAGEMENT_COMPONENT, //only one in table, LOT 3315
	VEHICLE_PHYSICS_2_COMPONENT, //Flying vehicle tests component
	PHYSICS_SYSTEM_COMPONENT, //Used by a lot of LUP freebuild objects, LOT 7138
	REBUILD_COMPONENT,
	SWITCH_COMPONENT,
	MINIGAME_COMPONENT, //only two in table, one is the biplane(LOT 4625), the other is LOT 2365, a zone control object
	CHANGLING_COMPONENT, //used by "Kipper Duel"  models...
	CHOICE_BUILD_COMPONENT, //choice build component?
	PACKAGE_COMPONENT, //Loot pack component?
	SOUND_REPEATER_COMPONENT, //only two in table, both are sound objects, this must be a sound component
	SOUND_AMBIENT_2D_COMPONENT, //only two in table, and those are the player objects
	SOUND_AMBIENT_3D_COMPONENT, //only one in table, which is an ambient sound object
	PRECONDITION_COMPONENT,
	PLAYER_FLAGS_COMPONENT, //used in pirate siege...
	CUSTUM_BUILD_ASSEMBLY_COMPONENT, //only one in table, LOT 6398. a test rocket
	BASE_COMBAT_AI_COMPONENT,
	MODULE_ASSEMBLY_COMPONENT, //used by cars and rockets, modular stuff
	SHOWCASE_MODEL_COMPONENT, //showcase component? (LOT 6545)
	RACING_MODULE_COMPONENT, //another modular related component
	GENERIC_ACTIVATOR_COMPONENT, //only three in table, a middle module component?
	PROPERTY_VENDOR_COMPONENT, //only two in table, property selling venders
	HF_LIGHT_DIRECTION_COMPONENT, //only one in table, LOT 6968, a light direction component?
	ROCKET_LAUNCHPAD_CONTROL_COMPONENT, //launchpad related component
	ROCKET_LANDING_COMPONENT, //only two in table, and those are the player objects
	TRIGGER_COMPONENT,
	DROPPED_LOOT_COMPONENT,
	RACING_CONTROL_COMPONENT,
	FACTION_TRIGGER_COMPONENT, //something to do with rank items... maybe to do with skills?
	MISSION_NPC_COMPONENT, //missions giver component?
	RACING_STATS_COMPONENT, //only two in table, racing car related
	LUP_EXHIBIT_COMPONENT, //only one in table, LUP exhibit related, LOT 9461
	BBB_COMPONENT,
	SOUND_TRIGGER_COMPONENT, //sound trigger component
	PROXIMITY_MONITOR_COMPONENT, //more launchpad related stuff
	RACING_SOUND_TRIGGER_COMPONENT, //only two in table, sound trigger related
	CHAT_COMPONENT,
	FRIENDS_LIST_COMPONENT,
	GUILD_COMPONENT,
	LOCAL_SYSTEM_COMPONENT,
	MISSION_COMPONENT,
	MUTABLE_MODEL_BEHAVIORS_COMPONENT,
	PATHFINDING_CONTROL_COMPONENT,
	PET_TAMING_CONTROL_COMPONENT,
	PROPERTY_EDITOR_COMPONENT,
	SKINNED_RENDER_COMPONENT,
	SLASH_COMMAND_COMPONENT,
	STATUS_EFFECT_COMPONENT,
	TEAMS_COMPONENT,
	TEXT_EFFECT_COMPONENT,
	TRADE_COMPONENT,
	USER_CONTROL_COMPONENT, //skateboard component
	IGNORE_LIST_COMPONENT,
	MULTI_ZONE_ENTRANCE, //only two in table, both are Starbase 3001 launcher related
	BUFF_COMPONENT,
	INTERACTION_MANAGER,
	DONATION_VENDOR_COMPONENT, //brick donation component
	COMBAT_MEDIATOR_COMPONENT,
	UNKOWN_102_COMPONENT, //only two in table, commendation vendor component?
	UNKOWN_103_COMPONENT, //only two in table, nothing in objects
	RAIL_ACTIVATOR_COMPONENT, //rail activator related
	UNKOWN_105_COMPONENT, //only three in table, ? I haven't a clue as to what this is supposed to be
	UNKOWN_106_COMPONENT, //only one in table, related to skateboard mount, LOT 16684
	UNKNOWN_107_COMPONENT, //only one in table, generic player
	UNKNOWN_108_COMPONENT, //for vehicles

	UNKOWN_113_COMPONENT = 113, //only one in table, property plaque
	UNKOWN_114_COMPONENT, //used by building bays

	UNKOWN_116_COMPONENT = 116 //only one in table, LOT 16512, a culling plane, culls objects behind them
};