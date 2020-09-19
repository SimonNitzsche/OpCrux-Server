#ifndef __REPLICA__COMPONENTS__TRIGGER_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__TRIGGER_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "FileTypes/LVLFile/TriggerFile.hpp"

class TriggerComponent : public IEntityComponent {
private:
	ZoneTrigger trigger{};

public:

	TriggerComponent(std::int32_t componentID) : IEntityComponent(componentID), trigger(-1, false) {}
	
	static constexpr int GetTypeID() { return 69; }

	void AssignTrigger(ZoneTrigger _trigger) {
		trigger = _trigger;
		HandleEvent("OnCreate", this->owner);
	}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION && trigger.enabled) {
			factory->Write(true);
			factory->Write<std::int32_t>(trigger.id);
		}
		else {
			factory->Write(false);
		}
	}

private:

	inline
	void TriggerCommand(ZoneTriggerCommand * zCommand, Entity::GameObject * invoker) {
		/*
		Commands TODO:
			zonePlayer
				[zone ID],(0 for non-instanced, 1 for instanced), (x, y, z position), (y rotation), (spawn point name)
			fireEvent
				(String to send to the recipient)
			destroyObj
				(0 for violent, 1 for silent)
			toggleTrigger
				[0 to disable, 1 to enable]
			resetRebuild
				(0 for normal reset, 1 for "failure" reset)
			setPath
				[new path name],(starting point index),(0 for forward, 1 for reverse)
			setPickType
				[new pick type, or -1 to disable picking]
			moveObject
				[x offset],[y offset],[z offset]
			rotateObject
				[x rotation],[y rotation],[z rotation]
			pushObject
				[x direction],[y direction],[z direction]
			repelObject
				(force multiplier)
			setTimer
				[timer name],[duration in seconds]
			cancelTimer
				[timer name]
			playCinematic
				[cinematic name],(lead-in in seconds),("wait" to wait at end),("unlock" to NOT lock the player controls),("leavelocked" to leave player locked after cinematic finishes),("hideplayer" to make player invisible during cinematic
			toggleBBB
				("enter" or "exit" to force direction)
			updateMission
				[taskType],[targetid],[value1],[value2],[wsValue]
			setBouncerState
				["on" to activate bouncer or "off" to deactivate bouncer]
			bounceAllOnBouncer
				No Parameters Required
			turnAroundOnPath
				No Parameters Required
			goForwardOnPath
				No Parameters Required
			goBackwardOnPath
				No Parameters Required
			stopPathing
				No Parameters Required
			startPathing
				No Parameters Required
			LockOrUnlockControls
				["lock" to lock controls or "unlock" to unlock controls]
			PlayEffect
				[nameID],[effectID],[effectType],[priority(optional)]
			StopEffect
				[nameID]
			activateMusicCue
				DEPRECATED.  Does nothing.
			deactivateMusicCue
				DEPRECATED.  Does nothing.
			flashMusicCue
				DEPRECATED.  Does nothing.
			setMusicParameter
				DEPRECATED.  Does nothing.
			play2DAmbientSound
				DEPRECATED.  Does nothing.
			stop2DAmbientSound
				DEPRECATED.  Does nothing.
			play3DAmbientSound
				DEPRECATED.  Does nothing.
			stop3DAmbientSound
				DEPRECATED.  Does nothing.
			activateMixerProgram
				DEPRECATED.  Does nothing.
			deactivateMixerProgram
				DEPRECATED.  Does nothing.
			CastSkill
				[skillID]
			displayZoneSummary
				[1 for zone start, 0 for zone end]
			SetPhysicsVolumeStatus
				[“On”, “Off”]
			setModelToBuild
				[template ID]
			spawnModelBricks
				[amount, from 0 to 1],[x],[y],[z]
			ActivateSpawnerNetwork
				[Spawner Network Name]
			DeactivateSpawnerNetwork
				[Spawner Network Name]
			ResetSpawnerNetwork
				[Spawner Network Name]
			DestroySpawnerNetworkObjects
				[Spawner Network Name]
			Go_To_Waypoint
				[Waypoint index],("true" to allow direction change, otherwise "false"),("true" to stop at waypoint, otherwise "false")
			ActivatePhysics
				"true" to activate and add to world, "false" to deactivate and remove from the world
		*/
		
		if (zCommand->id == "SetPhysicsVolumeEffect") {

			/*
				SetPhysicsVolumeEffect
					["Push", "Attract", "Repulse", "Gravity", "Friction"],[amount],(direction x, y, z),("True" or "False")(min distance)(max distance)
			*/

			PhantomPhysicsComponent * phantomPhysicsComponent = owner->GetComponent<PhantomPhysicsComponent>();

			if (phantomPhysicsComponent) {
				phantomPhysicsComponent->SetEffectDirty();

				std::string effectTypeStr = zCommand->args.at(0);
				
					 if (effectTypeStr == "Push") { phantomPhysicsComponent->physEffectType = 0; }
				else if (effectTypeStr == "Attract") { phantomPhysicsComponent->physEffectType = 1; }
				else if (effectTypeStr == "Repulse") { phantomPhysicsComponent->physEffectType = 2; }
				else if (effectTypeStr == "Gravity") { phantomPhysicsComponent->physEffectType = 3; }
				else if (effectTypeStr == "Friction") { phantomPhysicsComponent->physEffectType = 4; }

				std::float_t effectAmount = std::stof(zCommand->args.at(1));
				phantomPhysicsComponent->physEffectAmount = effectAmount;
				if (zCommand->args.size() >= 5) {
					std::float_t dirX = std::stof(zCommand->args.at(2));
					std::float_t dirY = std::stof(zCommand->args.at(3));
					std::float_t dirZ = std::stof(zCommand->args.at(4));
					phantomPhysicsComponent->SetEffectDirection(DataTypes::Vector3(dirX, dirY, dirZ));
				}

				if (zCommand->args.size() >= 6) {
					bool enabled = zCommand->args.at(5) == "True" || zCommand->args.at(5) == "true" || zCommand->args.at(5) == "1";
					phantomPhysicsComponent->physEffectActive = enabled;
				}
				else {
					phantomPhysicsComponent->physEffectActive = true;
				}

				phantomPhysicsComponent->physEffectUseDistance = false;
				/*if (zCommand->args.size() >= 7) {
					phantomPhysicsComponent->physEffectUseDistance = true;
					std::float_t effectMin = std::stof(zCommand->args.at(6));
					phantomPhysicsComponent->physEffectMinDistance = effectMin;
				}

				if (zCommand->args.size() >= 8) {
					std::float_t effectMax = std::stof(zCommand->args.at(7));
					phantomPhysicsComponent->physEffectMinDistance = effectMax;
				}*/
			}
		}

		else {
			Logger::log("WRLD", "Unrecognized trigger command: " + zCommand->id, LogType::WARN);
		}
	}

	void HandleEvent(std::string eventID, Entity::GameObject * invoker) {
		if (trigger.enabled) {
			for (int i = 0; i < trigger.events.size(); ++i) {
				ZoneTriggerEvent * zEvent = &trigger.events.at(i);
				if (zEvent->id == eventID) {
					for (int j = 0; j < zEvent->commands.size(); ++j) {
						ZoneTriggerCommand * zCommand = &zEvent->commands.at(j);
						
						TriggerCommand(zCommand, invoker);
					}

					break;
				}
			}
		}
	}

public:


	/*
		Events TODO:
		OnCustomEvent
		OnEnter
		OnExit
		OnHit
		OnTimerDone
		OnRebuildComplete
		OnActivated
		OnDeactivated
		OnArrived
		OnArrivedAtEndOfPath
		OnZoneSummaryDismissed
		OnArrivedAtDesiredWaypoint
		OnPetOnSwitch
		OnPetOffSwitch
		OnInteract
	*/

	void Awake() {
	}

	~TriggerComponent() {
		HandleEvent("OnDestroy", this->owner);
	}

};

#endif