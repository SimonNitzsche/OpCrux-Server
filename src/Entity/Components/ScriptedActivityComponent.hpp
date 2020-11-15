#ifndef __REPLICA__COMPONENTS__SCRIPTED_ACTIVITY_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__SCRIPTED_ACTIVITY_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include <memory>
#include <unordered_map>

#include "DataTypes/LWOOBJID.hpp"

#include "Missions/MissionManager.hpp"

class ScriptedActivityComponent : public IEntityComponent {
private:

	bool _componentDirty = false;

	std::map<DataTypes::LWOOBJID, std::vector<std::float_t>> playerActivityData;

public:

	std::int32_t activityID = -1;

	ScriptedActivityComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 39; }

	void Serialize(RakNet::BitStream* factory, ReplicaTypes::PacketTypes packetType) {
		//ENABLE_FLAG_ON_CONSTRUCTION(_componentDirty);
		
		factory->Write(_componentDirty);
		if (_componentDirty) {
			factory->Write<std::uint32_t>(playerActivityData.size());
			for (auto it = playerActivityData.begin(); it != playerActivityData.end(); ++it) {
				factory->Write<std::uint64_t>(it->first);
				for (int i = 0; i < 10; ++i) {
					factory->Write<std::float_t>(it->second.at(i));
				}
			}
		}
	}

	bool PlayerInActivity(DataTypes::LWOOBJID player) {
		// std::find is broken somehow, so we have to iterate by our own.
		for (auto it = playerActivityData.begin(); it != playerActivityData.end(); ++it)
			if (it->first == player)
				return true;
		return false;
	}

	std::list<DataTypes::LWOOBJID> GetPlayersInActivity() {
		std::list<DataTypes::LWOOBJID> retList {};
		for (auto it = playerActivityData.begin(); it != playerActivityData.end(); ++it)
			retList.push_back(it->first);
		return retList;
	}

	void AddPlayerToActivity(DataTypes::LWOOBJID player) {
		if (PlayerInActivity(player)) return;
		playerActivityData.insert({ player, {0,0, 0,0, 0,0, 0,0, 0,0} });
		_componentDirty = true;
		this->owner->SetDirty();
	}

	void RemovePlayerFromActivity(DataTypes::LWOOBJID player) {
		if (!PlayerInActivity(player)) return;
		playerActivityData.erase(player);
		_componentDirty = true;
		this->owner->SetDirty();
	}

	void SetActivityUserData(Entity::GameObject* userID, int typeIndex, std::float_t value) {
		SetActivityUserData(userID->GetObjectID(), typeIndex, value);
	}

	void SetActivityUserData(DataTypes::LWOOBJID userID, int typeIndex, std::float_t value) {
		if (typeIndex > 10) return;
		for (auto it = playerActivityData.begin(); it != playerActivityData.end(); ++it) {
			if (it->first == userID) {
				it->second.at(typeIndex) = value;
				return;
			}
		}

		_componentDirty = true;
		owner->SetDirty();
	}

	std::float_t GetActivityUserData(Entity::GameObject* userID, int typeIndex) {
		return GetActivityUserData(userID->GetObjectID(), typeIndex);
	}

	std::float_t GetActivityUserData(DataTypes::LWOOBJID userID, int typeIndex) {
		if (typeIndex > 10) return 0.0f;
		for (auto it = playerActivityData.begin(); it != playerActivityData.end(); ++it) {
			if (it->first == userID) {
				return it->second.at(typeIndex);
			}
		}
	}

	void PopulateFromLDF(LDFCollection * collection) {
		// TODO
		LDF_GET_VAL_FROM_COLLECTION(activityID, collection, u"activityID", activityID);
	}

	void CompleteActivity() {
		for (auto player : playerActivityData) {
			MissionManager::LaunchTaskEvent(Enums::EMissionTask::WIN_ACTIVITY, owner, player.first, 1, activityID);
		}
	}

	void MiniGameSetParameters(LDFCollection parameters) {

	}

};

#endif