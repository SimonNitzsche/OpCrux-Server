#ifndef __REPLICA__COMPONENTS__SCRIPTED_ACTIVITY_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__SCRIPTED_ACTIVITY_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include <memory>
#include <unordered_map>

#include "DataTypes/LWOOBJID.hpp"

class ScriptedActivityComponent : public IEntityComponent {
private:

	bool _componentDirty = false;

	std::map<DataTypes::LWOOBJID, std::vector<std::float_t>> playerActivityData;

public:

	ScriptedActivityComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr std::int16_t GetTypeID() { return 39; }

	void Serialize(RakNet::BitStream* factory, ReplicaTypes::PacketTypes packetType) {
		//ENABLE_FLAG_ON_CONSTRUCTION(_componentDirty);
		
		factory->Write(_componentDirty);
		if (_componentDirty) {
			factory->Write<std::uint32_t>(playerActivityData.size());
			for (auto & it : playerActivityData) {
				factory->Write<std::uint64_t>(it.first);
				for (int i = 0; i < 10; ++i) {
					factory->Write<std::float_t>(it.second.at(i));
				}
			}
		}
	}

	bool PlayerInActivity(DataTypes::LWOOBJID player) {
		// std::find is broken somehow, so we have to iterate by our own.
		for (auto & it : playerActivityData)
			if (it.first == player)
				return true;
		return false;
	}

	std::list<DataTypes::LWOOBJID> GetPlayersInActivity() {
		std::list<DataTypes::LWOOBJID> retList {};
		for (auto & it : playerActivityData)
			retList.push_back(it.first);
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

	void PopulateFromLDF(const LDFCollection& collection) {
		// TODO
		
	}

};

#endif