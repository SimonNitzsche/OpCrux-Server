#ifndef __REPLICA__COMPONENTS__MovingPlatfrom_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__MovingPlatfrom_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "FileTypes/LUZFile/LUZone.hpp"
#include "FileTypes/LVLFile/LUScene.hpp"

using namespace DataTypes;

class MovingPlatformComponent : public IEntityComponent {
private:
	std::u16string pathName;
	std::uint32_t startingPointIndex = 0;
	bool isReverse = false;

	std::uint32_t state = 2;
	std::int32_t desiredWaypointIndex;
	bool stopAtDesiredWaypoint;

	std::int32_t currentWaypointIndex;


	FileTypes::LUZ::LUZonePathMovingPlatform* attachedPath = nullptr;
public:

	MovingPlatformComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 25; }

	void Serialize(RakNet::BitStream* factory, ReplicaTypes::PacketTypes packetType) {
		/* TODO: Moving Platform Component Serialization */


		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
			factory->Write(false);
			factory->Write(true);

			factory->Write(true);
			factory->Write<std::uint16_t>(pathName.length());
			factory->Write(reinterpret_cast<const char*>(pathName.c_str()), pathName.size() * 2);
			factory->Write(startingPointIndex);
			factory->Write(isReverse);

			return;

			factory->Write(true);
			factory->Write<std::uint32_t>(5);
			factory->Write(true);
			factory->Write(true);

			int currentWaypoint = 0;
			factory->Write(attachedPath->waypoints.at(currentWaypoint)->position);
			factory->Write(attachedPath->waypoints.at(currentWaypoint)->rotation.x);
			factory->Write(attachedPath->waypoints.at(currentWaypoint)->rotation.y);
			factory->Write(attachedPath->waypoints.at(currentWaypoint)->rotation.z);
			factory->Write(attachedPath->waypoints.at(currentWaypoint)->rotation.w);


			factory->Write(false);
			// factory->Write<std::uint32_t>(25);
			// factory->Write(currentWaypoint);
			// factory->Write(currentWaypoint == 1);

		}
		else {
			factory->Write(false);
			factory->Write(false);
		}


		if (attachedPath && attachedPath != nullptr) {
			//RakSleep(1000);

			RakNet::BitStream testBs = RakNet::BitStream();
			LUPacketHeader returnBSHead;
			returnBSHead.protocolID = static_cast<uint8_t>(0x53);
			returnBSHead.remoteType = static_cast<uint16_t>(0x05);
			returnBSHead.packetID = static_cast<uint32_t>(12);
			testBs.Write(returnBSHead);

			// GM Header
			testBs.Write<std::uint64_t>(owner->GetObjectID());
			testBs.Write<std::uint16_t>(0x02f9);

			testBs.Write(false);
			testBs.Write(false);
			testBs.Write(0);
			testBs.Write(25);
			testBs.Write(0);
			testBs.Write(0.0f);
			testBs.Write(0.0f);
			testBs.Write(0.0f);
			testBs.Write(0);
			testBs.Write(0);
			testBs.Write(1);
			testBs.Write(attachedPath->waypoints.at(0)->position);
			testBs.Write(false);

			auto clients = owner->GetZoneInstance()->sessionManager.GetClients();
			for (int i = 0; i < clients.size(); ++i) {
				ClientSession session = clients.at(i);
				owner->GetZoneInstance()->rakServer->Send(&testBs, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, session.systemAddress, false);
			}
		}
	}

	void Update() {
		//if ((ServerInfo::uptime() % 100) != 0) return;

	}

	void PopulateFromLDF(LDFCollection* collection) {
		LDF_GET_VAL_FROM_COLLECTION(pathName, collection, u"attached_path", u"");
		LDF_GET_VAL_FROM_COLLECTION(startingPointIndex, collection, u"attached_path_start", 0);


	}

	void Awake() {
		if (pathName != u"")
			attachedPath = static_cast<FileTypes::LUZ::LUZonePathMovingPlatform*>(owner->GetZoneInstance()->luZone->paths.at(pathName));
	}

};

#endif

//#ifndef __REPLICA__COMPONENTS__MovingPlatfrom_COMPONENT_HPP__
//#define __REPLICA__COMPONENTS__MovingPlatfrom_COMPONENT_HPP__
//
//#include "Entity/Components/Interface/IEntityComponent.hpp"
//#include "FileTypes/LUZFile/LUZone.hpp"
//#include "FileTypes/LVLFile/LUScene.hpp"
//
//using namespace DataTypes;
//
//class MovingPlatformComponent;
//
//class MPC_SC_Base {
//public:
//	std::uint32_t mpcType = -1;
//
//	std::uint64_t timeWPStart;
//	bool isRunning = false;
//	FileTypes::LUZ::LUZonePathMovingPlatform* actualPath;
//	bool pushUpdate = false;
//	DataTypes::Vector3 currentPosition;
//
//	Entity::GameObject* owner;
//
//	/*
//		State updates on AG tour bus:
//		IDLE = 28			-> 0001 1100
//		MOVING = 25			-> 0001 1001
//		REACHED_WP = 2		-> 0000 0010
//		SET_NEXT_WP = 25	-> 0001 1001
//		STOP_PATHING = 2	-> 0000 0010
//		FINISHED = 28		-> 0001 1100
//	*/
//
//	std::int32_t desiredWaypointIndex=-1;
//	bool stopAtDesiredWaypoint;
//
//	bool isReverse;
//	std::uint32_t state=0;
//	std::int32_t currentWaypointIndex=0;
//	std::int32_t nextWaypointIndex=1;
//
//	std::float_t percToNextWP=0;
//
//	DataTypes::Vector3 targetPosition;
//	std::float_t idleTimeElapsed=0.0f;
//	std::float_t moveTimeElapsed=0.0f;
//
//	MPC_SC_Base() {
//
//	}
//
//	virtual void Setup(Entity::GameObject * owner, FileTypes::LUZ::LUZonePathMovingPlatform* path, LDFCollection* collection) {
//		actualPath = path;
//		this->owner = owner;
//	}
//
//	virtual void Serialize(RakNet::BitStream* factory, ReplicaTypes::PacketTypes packetType) {
//		factory->Write(mpcType);
//	};
//
//	/*
//		gets current waypoint based from given index
//	*/
//	FileTypes::LUZ::LUZonePathWaypointMovingPlatform * GetWP(int index) {
//		return actualPath->waypoints.at(index);
//	}
//
//	/*
//		returns the time in second the mp takes to move
//	*/
//	std::float_t GetMovementDuration() {
//		auto curWP = GetWP(currentWaypointIndex);
//		auto nextWP = GetWP(nextWaypointIndex);
//		auto diff = Vector3::Distance(curWP->position, nextWP->position);
//		return diff * (1.0f / curWP->speed);
//	}
//
//	/*
//		returns the time in seconds that the mp will wait before it moves
//	*/
//	std::float_t GetIdleDuration() {
//		auto curWP = GetWP(currentWaypointIndex);
//		return curWP->wait;
//	}
//
//	/*
//		returns the time in second the mp takes to move and finish
//	*/
//	std::float_t GetTotalDuration() {
//		
//		return GetIdleDuration() + GetMovementDuration();
//	}
//
//	/*
//		Gets the next wp index once finished traveling.
//		Returns 0xFFFFFFFF when there is none (e.g. Once)
//	*/
//	std::uint32_t GetNextWPIndex() {
//
//	}
//
//	/*
//		Does the calculations to get the current status
//	*/
//	void ApplyValues() {
//		// We stopped
//		/*if (state == 2) {
//			pushUpdate = true;
//			ReachedWaypoint();
//			return;
//		}
//
//		if (state == 28) {
//			if (timeWPStart + int32_t(GetIdleDuration() * 1000.f) < ServerInfo::uptimeMs()) {
//				if (nextWaypointIndex != -1) {
//					timeWPStart = ServerInfo::uptimeMs();
//					
//					if (!stopAtDesiredWaypoint) {
//						state = 25;
//					}
//				}
//			}
//			return;
//		}
//
//		if (state == 25) {
//			auto curTime = ServerInfo::uptimeMs() - this->timeWPStart;
//			
//			
//
//			percToNextWP = 1.0f / GetMovementDuration() * curTime * 0.001f;
//
//			if (percToNextWP > 1.0f) percToNextWP = 1.0f;
//
//			if (percToNextWP < 0.05f || percToNextWP > 0.95f)
//				pushUpdate = true;
//
//			auto curWP = GetWP(currentWaypointIndex);
//			auto nextWP = GetWP(nextWaypointIndex);
//
//			currentPosition.x = curWP->position.x + (nextWP->position.x - curWP->position.x) * percToNextWP;
//			currentPosition.y = curWP->position.y + (nextWP->position.y - curWP->position.y) * percToNextWP;
//			currentPosition.z = curWP->position.z + (nextWP->position.z - curWP->position.z) * percToNextWP;
//
//			if (percToNextWP == 1.0f) {
//				state = 2;
//				pushUpdate = true;
//			}
//
//			
//			return;
//		}*/
//
//		if (pushUpdate) {
//			Sync();
//			pushUpdate = false;
//		}
//
//		if (state == 2) {
//			// Moving
//
//			auto curTime = ServerInfo::uptimeMs() - this->timeWPStart;
//
//			percToNextWP = 1.0f / GetMovementDuration() * curTime * 0.001f;
//
//			if (percToNextWP > 1.0f) percToNextWP = 1.0f;
//
//			
//			if (percToNextWP == 0.0f || percToNextWP == 1.0f)
//				pushUpdate = true;
//
//			auto curWP = GetWP(currentWaypointIndex);
//			auto nextWP = GetWP(nextWaypointIndex);
//
//			currentPosition.x = curWP->position.x + (nextWP->position.x - curWP->position.x) * percToNextWP;
//			currentPosition.y = curWP->position.y + (nextWP->position.y - curWP->position.y) * percToNextWP;
//			currentPosition.z = curWP->position.z + (nextWP->position.z - curWP->position.z) * percToNextWP;
//
//			// Reached end?
//			if (percToNextWP == 1.0f) {
//				state = 4;
//				pushUpdate = true;
//				ReachedWaypoint();
//			}
//			return;
//		}
//
//		if (state == 4) {
//			// Idle
//
//			if (currentWaypointIndex == desiredWaypointIndex) return;
//
//			if (timeWPStart + int32_t(GetIdleDuration() * 1000.f) < ServerInfo::uptimeMs()) {
//				if (nextWaypointIndex != -1) {
//					timeWPStart = ServerInfo::uptimeMs();
//
//					if (!stopAtDesiredWaypoint && nextWaypointIndex == desiredWaypointIndex) {
//						state = 4;
//						return;
//					}
//					SetupNextWP();
//					state = 2;
//				}
//			}
//
//			// Reached destination
//			
//
//			// Last waypoint in direction
//			return;
//		}
//	}
//
//	void SetupNextWP() {
//		bool reverse = this->isReverse;
//
//		bool allowReverse = actualPath->pathBehaviour == FileTypes::LUZ::LUZonePathBehaviour::Bounce;
//
//		// Keep track for later
//		auto oldNext = nextWaypointIndex;
//
//		// set next index without checks
//		nextWaypointIndex = currentWaypointIndex + (this->isReverse ? -1 : +1);
//
//		// now do the checks
//		if (nextWaypointIndex >= actualPath->waypoints.size() || nextWaypointIndex < 0) {
//			// we are beyond limits
//			switch (actualPath->pathBehaviour) {
//			case FileTypes::LUZ::LUZonePathBehaviour::Once: {
//				// we don't have a next
//				nextWaypointIndex = -1;
//				desiredWaypointIndex = oldNext;
//				state = 4;
//				break;
//			}
//			case FileTypes::LUZ::LUZonePathBehaviour::Bounce: {
//				// change direction
//				isReverse = !isReverse;
//			} [[nobreak]]
//			case FileTypes::LUZ::LUZonePathBehaviour::Loop: {
//
//				// put it back in limits
//				nextWaypointIndex = (isReverse) ? actualPath->waypoints.size() - 2 : 0;
//
//				if (nextWaypointIndex == -1) nextWaypointIndex = 0;
//				state = 4;
//
//				break;
//			}
//			}
//		}
//
//		percToNextWP == 0.0f;
//		Logger::log(CacheObjects::GetName(owner->GetLOT()), "Next waypoint: " + std::to_string(currentWaypointIndex) + " -> " + std::to_string(nextWaypointIndex));
//		if(oldNext != -1)
//			currentWaypointIndex = oldNext;
//	}
//
//	void ReachedWaypoint() {
//		Logger::log(CacheObjects::GetName(owner->GetLOT()), "Reached waypoint" + std::to_string(currentWaypointIndex));
//		pushUpdate = true;
//		state = 4;
//		timeWPStart = ServerInfo::uptimeMs();
//		
//
//		bool isDesiredWaypoint = desiredWaypointIndex == currentWaypointIndex;
//
//		if (isDesiredWaypoint) {
//			GM::ArrivedAtDesiredWaypoint nmsg;
//			nmsg.iPathIndex = desiredWaypointIndex;
//			owner->CallMessage(nmsg);
//		}
//
//		
//	}
//
//	void StartPathing() {
//		pushUpdate = true;
//		state = 2;
//		timeWPStart = ServerInfo::uptimeMs();
//		currentWaypointIndex = 0;
//		SetupNextWP();
//	}
//
//	void GoToWaypoint(GM::GoToWaypoint* msg) {
//		desiredWaypointIndex = msg->iPathIndex;
//		// = msg->bAllowPathingDirectionChange
//		stopAtDesiredWaypoint = msg->bStopAtWaypoint;
//		state = 2;
//		timeWPStart = ServerInfo::uptimeMs();
//	}
//
//
//	void Sync() {
//		GM::PlatformResync nmsg;
//		nmsg.bReverse = isReverse;
//		nmsg.bStopAtDesiredWaypoint = stopAtDesiredWaypoint;
//		nmsg.eCommand = 0;
//		nmsg.eState = state;
//		nmsg.eUnexpectedCommand = 0;
//		nmsg.fIdleTimeElapsed = idleTimeElapsed;
//		nmsg.fMoveTimeElapsed = moveTimeElapsed;
//		nmsg.fPercentBetweenPoints = percToNextWP;
//		nmsg.iDesiredWaypointIndex = desiredWaypointIndex;
//		nmsg.iIndex = currentWaypointIndex;
//		nmsg.iNextIndex = nextWaypointIndex;
//		nmsg.ptUnexpectedLocation = currentPosition;
//		GameMessages::Broadcast(owner, nmsg);
//	}
//
//};
//class MPC_SC_Mover : public MPC_SC_Base {
//public:
//	MPC_SC_Mover() : MPC_SC_Base() {
//		mpcType = 4;
//	}
//
//	static constexpr int GetTypeID() { return (25 << 8) + 4; }
//
//	void Setup(Entity::GameObject* owner, FileTypes::LUZ::LUZonePathMovingPlatform* path, LDFCollection* collection) {
//		MPC_SC_Base::Setup(owner, path, collection);
//	}
//
//	void Serialize(RakNet::BitStream* factory, ReplicaTypes::PacketTypes packetType) {
//		MPC_SC_Base::Serialize(factory, packetType);
//
//		bool dirtyFlag = false;
//		
//		dirtyFlag = pushUpdate;
//		if (pushUpdate) {
//			pushUpdate = false;
//		}
//		
//		factory->Write(dirtyFlag);
//		if (dirtyFlag) {
//			factory->Write(state);
//			factory->Write(desiredWaypointIndex);
//			factory->Write(stopAtDesiredWaypoint);
//			factory->Write(isReverse);
//
//			if (false) {
//				// TODO
//			}
//			else {
//				factory->Write(percToNextWP);
//				factory->Write(targetPosition);
//				factory->Write(currentWaypointIndex);
//				factory->Write(nextWaypointIndex);
//
//				factory->Write(idleTimeElapsed);
//				factory->Write(moveTimeElapsed);
//			}
//		}
//	};
//
//};
//class MPC_SC_SimpleMover : public MPC_SC_Base {
//public:
//	bool hasStartingPoint = false;
//	DataTypes::Vector3 startPointPos;
//	DataTypes::Quaternion startPointRot;
//
//	MPC_SC_SimpleMover() : MPC_SC_Base() {
//		mpcType = 5;
//	}
//
//	void Setup(Entity::GameObject* owner, FileTypes::LUZ::LUZonePathMovingPlatform* path, LDFCollection* collection) {
//		MPC_SC_Base::Setup(owner, path, collection);
//	}
//
//	void Serialize(RakNet::BitStream* factory, ReplicaTypes::PacketTypes packetType) {
//		MPC_SC_Base::Serialize(factory, packetType);
//
//		bool locDirty = false;
//		bool baseDirty = false;
//
//		baseDirty = pushUpdate;
//		if (pushUpdate) {
//			pushUpdate = false;
//		}
//
//		if (locDirty) {
//			factory->Write(hasStartingPoint);
//			if (hasStartingPoint) {
//				factory->Write(startPointPos);
//				factory->Write(startPointRot);
//			}
//		}
//
//		
//		if (baseDirty) {
//			factory->Write(state);
//			factory->Write(isReverse);
//		}
//	};
//};
//class MPC_SC_Rotater : public MPC_SC_Base {
//public:
//
//	MPC_SC_Rotater() : MPC_SC_Base() {
//		mpcType = 6;
//	}
//
//	void Setup(Entity::GameObject* owner, FileTypes::LUZ::LUZonePathMovingPlatform* path, LDFCollection* collection) {
//		MPC_SC_Base::Setup(owner, path, collection);
//	}
//
//	void Serialize(RakNet::BitStream* factory, ReplicaTypes::PacketTypes packetType) {
//		MPC_SC_Base::Serialize(factory, packetType);
//	};
//};
//
//class MovingPlatformComponent : public IEntityComponent {
//private:
//	std::u16string pathName;
//	std::uint32_t startingPointIndex = 0;
//	bool isReverse = false;
//
//	bool startPathingOnLoad = false;
//
//	std::list<std::unique_ptr<MPC_SC_Base>> platforms = {};
//
//	bool pathInfoDirty = true;
//	FileTypes::LUZ::LUZonePathMovingPlatform * attachedPath = nullptr;
//
//	LDFCollection* storedConfig;
//public:
//
//	MovingPlatformComponent(std::int32_t componentID) : IEntityComponent(componentID) {}
//
//	static constexpr int GetTypeID() { return 25; }
//
//	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
//		
//		bool sendSC = platforms.size() != 0;
//		sendSC = false;
//		factory->Write(sendSC);
//		
//		bool pathAttached = attachedPath != nullptr;
//		factory->Write(pathAttached);
//
//		if (pathAttached) {
//			factory->Write(pathInfoDirty);
//			if (pathInfoDirty) {
//				pathInfoDirty = false;
//				StringUtils::writeWStringToBitStream<std::uint16_t>(factory, pathName);
//				factory->Write(startingPointIndex);
//				factory->Write(isReverse);
//			}
//		}
//
//		if (sendSC) {
//			for (auto& mpcp : platforms) {
//				/* while */ factory->Write(true);
//				mpcp->Serialize(factory, packetType);
//			}
//			// Indicate break on the client's while
//			factory->Write(false);
//		}
//
//		return; // The stuff below is deprecated.
//
//		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
//			factory->Write(false);
//			factory->Write(true);
//
//			factory->Write(true);
//			factory->Write<std::uint16_t>(pathName.length());
//			factory->Write(reinterpret_cast<const char*>(pathName.c_str()), pathName.size() * 2);
//			factory->Write(startingPointIndex);
//			factory->Write(isReverse);
//
//			//return;
//
//			factory->Write(true);
//			factory->Write<std::uint32_t>(5);
//			factory->Write(true);
//			factory->Write(true);
//
//			int currentWaypoint = 0;
//			factory->Write(attachedPath->waypoints.at(currentWaypoint)->position);
//			factory->Write(attachedPath->waypoints.at(currentWaypoint)->rotation.x);
//			factory->Write(attachedPath->waypoints.at(currentWaypoint)->rotation.y);
//			factory->Write(attachedPath->waypoints.at(currentWaypoint)->rotation.z);
//			factory->Write(attachedPath->waypoints.at(currentWaypoint)->rotation.w);
//
//
//			factory->Write(false);
//			// factory->Write<std::uint32_t>(25);
//			// factory->Write(currentWaypoint);
//			// factory->Write(currentWaypoint == 1);
//
//			factory->Write(false);
//
//		}
//		else {
//			factory->Write(false);
//			factory->Write(false);
//		}
//
//
//		/*if (attachedPath && attachedPath != nullptr) {
//
//			{
//				GM::PlatformResync nmsg;
//				nmsg.eState = 25;
//				nmsg.iNextIndex = 1;
//				nmsg.iIndex = 0;
//				nmsg.bStopAtDesiredWaypoint = stopAtDesiredWaypoint;
//				GameMessages::Broadcast(owner, nmsg);
//			}
//		}*/
//	}
//
//	void Update() {
//		//if ((ServerInfo::uptime() % 100) != 0) return;
//		
//		for (auto& p : platforms) {
//			p->ApplyValues();
//			if (p->pushUpdate) {
//				//owner->SetDirty();
//				p->Sync();
//				p->pushUpdate = false;
//			}
//		}
//	}
//
//	void PopulateFromLDF(LDFCollection * collection) {
//
//		bool rotate_while_idle, platformIsRotater, platformIsMover, platformIsSimpleMover, dbonly, platformNoUpdateSync;
//		LDF_GET_VAL_FROM_COLLECTION(rotate_while_idle, collection, u"rotate_while_idle", false);
//		LDF_GET_VAL_FROM_COLLECTION(platformIsRotater, collection, u"platformIsRotater", false);
//		LDF_GET_VAL_FROM_COLLECTION(platformIsMover, collection, u"platformIsMover", false);
//		LDF_GET_VAL_FROM_COLLECTION(platformIsSimpleMover, collection, u"platformIsSimpleMover", false);
//		LDF_GET_VAL_FROM_COLLECTION(dbonly, collection, u"dbonly", false);
//		LDF_GET_VAL_FROM_COLLECTION(startPathingOnLoad, collection, u"startPathingOnLoad", false);
//		LDF_GET_VAL_FROM_COLLECTION(platformNoUpdateSync, collection, u"platformNoUpdateSync", false);
//
//		if (!(platformIsRotater || platformIsMover || platformIsSimpleMover)) {
//			platformIsSimpleMover = true;
//		}
//
//		if (platformIsRotater) {
//			this->platforms.push_back(std::make_unique<MPC_SC_Rotater>());
//		}
//
//		if (platformIsMover) {
//			this->platforms.push_back(std::make_unique<MPC_SC_Mover>());
//		}
//
//		if (platformIsSimpleMover) {
//			this->platforms.push_back(std::make_unique<MPC_SC_SimpleMover>());
//		}
//
//		LDF_GET_VAL_FROM_COLLECTION(pathName, collection, u"attached_path", u"");
//		LDF_GET_VAL_FROM_COLLECTION(startingPointIndex, collection, u"attached_path_start", 0);
//
//		storedConfig = collection;
//	}
//
//	void Awake() {
//		if(pathName != u"")
//			attachedPath = static_cast<FileTypes::LUZ::LUZonePathMovingPlatform*>(owner->GetZoneInstance()->luZone->paths.at(pathName));
//
//		for (auto& sc : platforms) {
//			sc->Setup(this->owner, attachedPath, storedConfig);
//		}
//
//		if (startPathingOnLoad) {
//			for (auto& sc : platforms) {
//				sc->StartPathing();
//			}
//		}
//	}
///*
//	std::uint32_t GetNextWP(bool reverable) {
//		std::uint32_t next = currentWaypointIndex;
//
//		if (isReverse)
//			++next;
//		else
//			--next;
//
//		if (next >= attachedPath->waypoints.size()) {
//			if (!reverable)
//				return 0;
//			return attachedPath->waypoints.size() - 2;
//		}
//
//		return next;
//	}
//
//	bool IsReversable() {
//		return attachedPath->pathBehaviour == FileTypes::LUZ::LUZonePathBehaviour::Bounce;
//	}
//
//	void Arrive() {
//		if (attachedPath != nullptr) {
//			bool reversable = IsReversable();
//			currentWaypointIndex = GetNextWP(reversable);
//		}
//	}
//
//	void OnStopPathing(Entity::GameObject* rerouteID, GM::StopPathing* msg) {
//		Arrive();
//		desiredWaypointIndex = GetNextWP(IsReversable());
//		GM::PlatformResync nmsg;
//		nmsg.bReverse = false;
//		nmsg.bStopAtDesiredWaypoint = true;
//		nmsg.iDesiredWaypointIndex = desiredWaypointIndex;
//		nmsg.eState = 2;
//		nmsg.iIndex = currentWaypointIndex;
//		GameMessages::Broadcast(owner, nmsg);
//	}
//
//	void OnGoToWaypoint(Entity::GameObject* rerouteID, GM::GoToWaypoint* msg) {
//		Arrive();
//		stopAtDesiredWaypoint = msg->bStopAtWaypoint;
//		desiredWaypointIndex = msg->iPathIndex;
//
//		GM::PlatformResync nmsg;
//		nmsg.bReverse = false;
//		nmsg.bStopAtDesiredWaypoint = stopAtDesiredWaypoint;
//		nmsg.eState = 25;
//		nmsg.iIndex = currentWaypointIndex;
//		nmsg.iDesiredWaypointIndex = desiredWaypointIndex;
//		if (currentWaypointIndex == desiredWaypointIndex) return;
//		GameMessages::Broadcast(owner, nmsg);
//	}
//	*/
//
//	void OnGoToWaypoint(Entity::GameObject* rerouteID, GM::GoToWaypoint* msg) {
//		for (auto& p : platforms) {
//			p->GoToWaypoint(msg);
//		}
//	}
//
//	void OnRequestPlatformResync(Entity::GameObject* rerouteID, GM::RequestPlatformResync* msg) {
//		for (auto& p : platforms) {
//			p->Sync();
//		}
//	}
//
//	void RegisterMessageHandlers() {
//		//REGISTER_OBJECT_MESSAGE_HANDLER(MovingPlatformComponent, GM::StopPathing, OnStopPathing);
//		REGISTER_OBJECT_MESSAGE_HANDLER(MovingPlatformComponent, GM::GoToWaypoint, OnGoToWaypoint);
//		REGISTER_OBJECT_MESSAGE_HANDLER(MovingPlatformComponent, GM::RequestPlatformResync, OnRequestPlatformResync);
//	}
//};
//
//#endif