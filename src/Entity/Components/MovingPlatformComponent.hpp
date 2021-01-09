#ifndef __REPLICA__COMPONENTS__MovingPlatfrom_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__MovingPlatfrom_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "FileTypes/LUZFile/LUZone.hpp"
#include "FileTypes/LVLFile/LUScene.hpp"

using namespace DataTypes;

class MovingPlatformComponent;

class MPC_SC_Base {
public:
	std::uint32_t mpcType = -1;

	std::uint64_t timeWPStart;
	
	std::int32_t desiredWaypointIndex=0;
	bool stopAtDesiredWaypoint;

	bool isReverse;
	std::uint32_t state=0;
	std::int32_t currentWaypointIndex=0;
	std::int32_t nextWaypointIndex=0;

	std::float_t percToNextWP=0;

	DataTypes::Vector3 targetPosition;
	std::float_t idleTimeElapsed=0.0f;
	std::float_t moveTimeElapsed=0.0f;

	MPC_SC_Base() {

	}

	virtual void Setup(MovingPlatformComponent * mpc, FileTypes::LUZ::LUZonePathMovingPlatform* path, LDFCollection* collection) {

	}

	virtual void Serialize(RakNet::BitStream* factory, ReplicaTypes::PacketTypes packetType) {
		factory->Write(mpcType);
	};

};
class MPC_SC_Mover : public MPC_SC_Base {
public:
	MPC_SC_Mover() : MPC_SC_Base() {
		mpcType = 4;
	}

	static constexpr int GetTypeID() { return (25 << 8) + 4; }

	void Setup(MovingPlatformComponent* mpc, FileTypes::LUZ::LUZonePathMovingPlatform* path, LDFCollection* collection) {
		MPC_SC_Base::Setup(mpc, path, collection);
	}

	void Serialize(RakNet::BitStream* factory, ReplicaTypes::PacketTypes packetType) {
		MPC_SC_Base::Serialize(factory, packetType);

		bool dirtyFlag = false;
		factory->Write(dirtyFlag);
		if (dirtyFlag) {
			factory->Write(state);
			factory->Write(desiredWaypointIndex);
			factory->Write(stopAtDesiredWaypoint);
			factory->Write(isReverse);

			if (false) {

			}
			else {
				factory->Write(percToNextWP);
				factory->Write(targetPosition);
				factory->Write(currentWaypointIndex);
				factory->Write(nextWaypointIndex);

				factory->Write(idleTimeElapsed);
				factory->Write(moveTimeElapsed);
			}
		}
	};

};
class MPC_SC_SimpleMover : public MPC_SC_Base {
public:
	bool hasStartingPoint = false;
	DataTypes::Vector3 startPointPos;
	DataTypes::Quaternion startPointRot;

	MPC_SC_SimpleMover() : MPC_SC_Base() {
		mpcType = 5;
	}

	void Setup(MovingPlatformComponent* mpc, FileTypes::LUZ::LUZonePathMovingPlatform* path, LDFCollection* collection) {
		MPC_SC_Base::Setup(mpc, path, collection);
	}

	void Serialize(RakNet::BitStream* factory, ReplicaTypes::PacketTypes packetType) {
		MPC_SC_Base::Serialize(factory, packetType);

		bool locDirty = false;

		if (locDirty) {
			factory->Write(hasStartingPoint);
			if (hasStartingPoint) {
				factory->Write(startPointPos);
				factory->Write(startPointRot);
			}
		}

		bool baseDirty = false;
		if (baseDirty) {
			factory->Write(state);
			factory->Write(isReverse);
		}
	};
};
class MPC_SC_Rotater : public MPC_SC_Base {
public:

	MPC_SC_Rotater() : MPC_SC_Base() {
		mpcType = 6;
	}

	void Setup(MovingPlatformComponent* mpc, FileTypes::LUZ::LUZonePathMovingPlatform* path, LDFCollection* collection) {
		MPC_SC_Base::Setup(mpc, path, collection);
	}

	void Serialize(RakNet::BitStream* factory, ReplicaTypes::PacketTypes packetType) {
		MPC_SC_Base::Serialize(factory, packetType);
	};
};

class MovingPlatformComponent : public IEntityComponent {
private:
	std::u16string pathName;
	std::uint32_t startingPointIndex = 0;
	bool isReverse = false;


	std::list<std::unique_ptr<MPC_SC_Base>> mpcPaths = {};

	bool pathInfoDirty = true;
	FileTypes::LUZ::LUZonePathMovingPlatform * attachedPath = nullptr;

	LDFCollection* storedConfig;
public:

	MovingPlatformComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 25; }

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		
		bool sendSC = mpcPaths.size() != 0;
		factory->Write(sendSC);
		
		bool pathAttached = attachedPath != nullptr;
		factory->Write(pathAttached);

		if (pathAttached) {
			factory->Write(pathInfoDirty);
			if (pathInfoDirty) {
				pathInfoDirty = false;
				StringUtils::writeWStringToBitStream<std::uint16_t>(factory, pathName);
				factory->Write(startingPointIndex);
				factory->Write(isReverse);
			}
		}

		if (sendSC) {
			for (auto& mpcp : mpcPaths) {
				/* while */ factory->Write(true);
				mpcp->Serialize(factory, packetType);
			}
			// Indicate break on the client's while
			factory->Write(false);
		}

		return; // The stuff below is deprecated.

		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
			factory->Write(false);
			factory->Write(true);

			factory->Write(true);
			factory->Write<std::uint16_t>(pathName.length());
			factory->Write(reinterpret_cast<const char*>(pathName.c_str()), pathName.size() * 2);
			factory->Write(startingPointIndex);
			factory->Write(isReverse);

			//return;

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

			factory->Write(false);

		}
		else {
			factory->Write(false);
			factory->Write(false);
		}


		/*if (attachedPath && attachedPath != nullptr) {

			{
				GM::PlatformResync nmsg;
				nmsg.eState = 25;
				nmsg.iNextIndex = 1;
				nmsg.iIndex = 0;
				nmsg.bStopAtDesiredWaypoint = stopAtDesiredWaypoint;
				GameMessages::Broadcast(owner, nmsg);
			}
		}*/
	}

	void Update() {
		//if ((ServerInfo::uptime() % 100) != 0) return;
		
	}

	void PopulateFromLDF(LDFCollection * collection) {

		bool rotate_while_idle, platformIsRotater, platformIsMover, platformIsSimpleMover, dbonly, startPathingOnLoad, platformNoUpdateSync;
		LDF_GET_VAL_FROM_COLLECTION(rotate_while_idle, collection, u"rotate_while_idle", false);
		LDF_GET_VAL_FROM_COLLECTION(platformIsRotater, collection, u"platformIsRotater", false);
		LDF_GET_VAL_FROM_COLLECTION(platformIsMover, collection, u"platformIsMover", false);
		LDF_GET_VAL_FROM_COLLECTION(platformIsSimpleMover, collection, u"platformIsSimpleMover", false);
		LDF_GET_VAL_FROM_COLLECTION(dbonly, collection, u"dbonly", false);
		LDF_GET_VAL_FROM_COLLECTION(startPathingOnLoad, collection, u"startPathingOnLoad", false);
		LDF_GET_VAL_FROM_COLLECTION(platformNoUpdateSync, collection, u"platformNoUpdateSync", false);

		if (!(platformIsRotater || platformIsMover || platformIsSimpleMover)) {
			platformIsSimpleMover = true;
		}

		if (platformIsRotater) {
			this->mpcPaths.push_back(std::make_unique<MPC_SC_Rotater>());
		}

		if (platformIsMover) {
			this->mpcPaths.push_back(std::make_unique<MPC_SC_Mover>());
		}

		if (platformIsSimpleMover) {
			this->mpcPaths.push_back(std::make_unique<MPC_SC_SimpleMover>());
		}

		LDF_GET_VAL_FROM_COLLECTION(pathName, collection, u"attached_path", u"");
		LDF_GET_VAL_FROM_COLLECTION(startingPointIndex, collection, u"attached_path_start", 0);

		storedConfig = collection;
	}

	void Awake() {
		if(pathName != u"")
			attachedPath = static_cast<FileTypes::LUZ::LUZonePathMovingPlatform*>(owner->GetZoneInstance()->luZone->paths.at(pathName));

		for (auto& sc : mpcPaths) {
			sc->Setup(this, attachedPath, storedConfig);
		}
	}
/*
	std::uint32_t GetNextWP(bool reverable) {
		std::uint32_t next = currentWaypointIndex;

		if (isReverse)
			++next;
		else
			--next;

		if (next >= attachedPath->waypoints.size()) {
			if (!reverable)
				return 0;
			return attachedPath->waypoints.size() - 2;
		}

		return next;
	}

	bool IsReversable() {
		return attachedPath->pathBehaviour == FileTypes::LUZ::LUZonePathBehaviour::Bounce;
	}

	void Arrive() {
		if (attachedPath != nullptr) {
			bool reversable = IsReversable();
			currentWaypointIndex = GetNextWP(reversable);
		}
	}

	void OnStopPathing(Entity::GameObject* rerouteID, GM::StopPathing* msg) {
		Arrive();
		desiredWaypointIndex = GetNextWP(IsReversable());
		GM::PlatformResync nmsg;
		nmsg.bReverse = false;
		nmsg.bStopAtDesiredWaypoint = true;
		nmsg.iDesiredWaypointIndex = desiredWaypointIndex;
		nmsg.eState = 2;
		nmsg.iIndex = currentWaypointIndex;
		GameMessages::Broadcast(owner, nmsg);
	}

	void OnGoToWaypoint(Entity::GameObject* rerouteID, GM::GoToWaypoint* msg) {
		Arrive();
		stopAtDesiredWaypoint = msg->bStopAtWaypoint;
		desiredWaypointIndex = msg->iPathIndex;

		GM::PlatformResync nmsg;
		nmsg.bReverse = false;
		nmsg.bStopAtDesiredWaypoint = stopAtDesiredWaypoint;
		nmsg.eState = 25;
		nmsg.iIndex = currentWaypointIndex;
		nmsg.iDesiredWaypointIndex = desiredWaypointIndex;
		if (currentWaypointIndex == desiredWaypointIndex) return;
		GameMessages::Broadcast(owner, nmsg);
	}

	void RegisterMessageHandlers() {
		REGISTER_OBJECT_MESSAGE_HANDLER(MovingPlatformComponent, GM::StopPathing, OnStopPathing);
		REGISTER_OBJECT_MESSAGE_HANDLER(MovingPlatformComponent, GM::GoToWaypoint, OnGoToWaypoint);
	}*/
};

#endif