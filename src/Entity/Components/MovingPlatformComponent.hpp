#ifndef __REPLICA__COMPONENTS__MovingPlatfrom_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__MovingPlatfrom_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "FileTypes/LUZFile/LUZone.hpp"
#include "FileTypes/LVLFile/LUScene.hpp"

using namespace DataTypes;

class MovingPlatformComponent : public IEntityComponent {
private:
	std::wstring pathName;
	std::uint32_t startingPointIndex = 0;
	bool isReverse = false;
	
	std::uint32_t state = 2;
	std::int32_t desiredWaypointIndex;
	bool stopAtDesiredWaypoint;

	std::int32_t currentWaypointIndex;


	FileTypes::LUZ::LUZonePathMovingPlatform * attachedPath = nullptr;
public:

	MovingPlatformComponent() : IEntityComponent() {}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
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

	}

	void Update() {
		if ((ServerInfo::uptime() % 10) != 0) return;
		if (attachedPath && attachedPath != nullptr) {
			RakSleep(1000);

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

	void PopulateFromLDF(LDFCollection * collection) {
		LDF_GET_VAL_FROM_COLLECTION(pathName, collection, L"attached_path", L"");
		LDF_GET_VAL_FROM_COLLECTION(startingPointIndex, collection, L"attached_path_start", 0);
		
		
	}

	void Awake() {
		if(pathName != L"")
			attachedPath = static_cast<FileTypes::LUZ::LUZonePathMovingPlatform*>(owner->GetZoneInstance()->luZone->paths.at(pathName));
	}

};

#endif