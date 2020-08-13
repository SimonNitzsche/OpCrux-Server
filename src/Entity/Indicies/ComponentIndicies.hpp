#ifndef __Entity__Indicies__ComponentsIndicies_HPP__
#define __Entity__Indicies__ComponentsIndicies_HPP__

#include <vector>

#include "Entity/Components/Interface/IEntityComponent.hpp"

using namespace Entity::Components::Interface;

enum EntityComponentIndex {
	Possessable,
	ModuleAssembly,
	ControllablePhysics,
	SimplePhysics,
	RigidBodyPhantomPhysics,
	VehiclePhysics,
	PhantomPhysics,
	Destructible,
	Collectible,
	Pet,
	Character,
	ShootingGallery,
	Inventory,
	Script,
	Skill,
	BaseCombatAI,
	Rebuild,
	MovingPlatform,
	Switch,
	Vendor,
	Bouncer,
	ScriptedActivity,
	RacingControl,
	LUPExhibit,
	Model,
	Render,
	Minigame,
	Component107,
	Trigger
};



class ComponentsIndicies {
public:
	std::vector<int> comp = {
		
		
		
		-1, // None 1-based index!
		ControllablePhysics,
		Render,
		SimplePhysics,
		Character,
		Script,
		Bouncer,
		Destructible,
		-1, // Ghost
		Skill,
		-1, // Spawner
		-1, // Item
		-1, // Rebuild
		-1, // Rebuild Start
		-1, // Rebuild Activator
		-1, // Icon Only
		Vendor,
		Inventory,
		-1, // Projectile Physics
		ShootingGallery,
		RigidBodyPhantomPhysics,
		-1, // DropEffect
		-1, // Chest
		Collectible,
		-1, // Blueprint
		MovingPlatform,
		Pet,
		-1, // Platform Boundary
		-1, // Module
		-1, // Arcade,
		VehiclePhysics,
		-1, // MovementAI
		-1, // Exhibit
		-1, // OverheadIcon
		-1, // PetControl
		-1, // Minifig
		-1, // Property
		-1, // Pet Creator
		-1, // Model Builder
		ScriptedActivity,
		PhantomPhysics,
		-1, // Springpad
		-1, // B3 Behaviours
		-1, // Property Entrance
		-1, // FX
		-1, // Property Management
		-1, // Vehicle Physics 46
		-1, // Physics System
		Rebuild, // QuickBuild
		Switch,
		Minigame,
		-1, // Changling
		-1, // Choice Build
		-1, // Package
		-1, // Sound Repeater
		-1, // Sound Ambient 2D
		-1, // Sound Ambient 3D
		-1, // Precondition
		-1, // Player Flags
		-1, // Custom Build Assembly
		BaseCombatAI,
		ModuleAssembly,
		-1, // Showcase Model Handler
		-1, // Racing Module
		-1, // Generic Activator
		-1, // Property Vendor
		-1, // HF Light Direction Gadget
		-1, // Rocket Launch
		-1, // Rocket Landing
		-1, // Trigger
		-1, // Dropped Loot
		RacingControl,
		-1, // Faction Trigger
		-1, // Mission Offer
		-1, // RacingStats
		LUPExhibit,
		-1, // BBB Component
		-1, // Sound Trigger
		-1, // Proximity Monitor
		-1, // Racing Sound Trigger
		-1, // Chat
		-1, // Friends List
		-1, // Guild
		-1, // Local System
		-1, // Mission
		-1, // Mutable Model Behaviors
		-1, // Pathfinding Control
		-1, // Pet Taming Control
		-1, // Property Editor
		-1, // Skinned Render
		-1, // Slash Command
		-1, // Status Effect
		-1, // Teams
		-1, // Text Effect
		-1, // Trade
		-1, // User Control
		-1, // Not Used
		-1, // LUP Launchpad
		-1, // Unknown 98
		-1, // Unknown 99
		-1, // Brick Donation
		-1, // Unknown 101
		-1, // Commendation Vendor
		-1, // Unknown 103
		-1, // Rail Activator
		-1, // Roller
		-1, // Unknown 106
		Component107,
		Possessable,
		-1, // Unknown 109
		-1, // Unknown 110
		-1, // Unknown 111
		-1, // Unknown 112
		-1, // Property Plaque
		-1, // Build Border
		-1, // Culling Plane
	};
	static const std::vector<std::pair<EntityComponentIndex, std::vector<IEntityComponent>>> indicies;
};

#endif
