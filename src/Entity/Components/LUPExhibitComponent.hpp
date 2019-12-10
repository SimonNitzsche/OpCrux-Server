#ifndef __REPLICA__COMPONENTS__LUP_EXHIBIT_COMPONENT_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__LUP_EXHIBIT_COMPONENT_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "Utils/ServerInfo.hpp"

using namespace DataTypes;

class LUPExhibitComponent : public IEntityComponent {
private:
	bool _isDirty = false;
	std::int32_t exhibitedLOT=0;
	std::int32_t cycleIndex=0;

	std::vector<int32_t> exhibitLOTs = {};
	long long lastupdate = 0;

	static const int showTimeInSeconds = 20;
	long long lastSet = 0;
public:

	LUPExhibitComponent() : IEntityComponent() {}

	static constexpr int GetTypeID() { return 75; }

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		ENABLE_FLAG_ON_CONSTRUCTION(_isDirty);
		factory->Write(_isDirty && exhibitedLOT!=0);
		if (_isDirty && exhibitedLOT!=0) {
			factory->Write<std::int32_t>(exhibitedLOT);
			_isDirty = false;
		}
	}

	void SetExhibitLOT(std::int32_t _exhibitedLOT) {
		Logger::log("WRLD", "Setting exhibit LOT to " + std::to_string(_exhibitedLOT), LogType::INFO);
		exhibitedLOT = _exhibitedLOT;
		lastSet = ServerInfo::uptime();
		_isDirty = true;
		owner->SetDirty();
	}

	void CycleExhibitLOT() {
		if (exhibitLOTs.size() == 0) return;
		cycleIndex = ++cycleIndex % exhibitLOTs.size();

		std::uint32_t nextLOT = exhibitLOTs.at(cycleIndex);
		SetExhibitLOT(nextLOT);
	}

	void Awake() {
		auto rows = Cache.getRows("LUPExhibitModelData");
		for (int i = 0; i < rows.getRowCount(); ++i) {
			if (rows.isValid(i)) {
				exhibitLOTs.push_back(rows[i][0]);
			}
		}
	}

	void Update() {
		long long uptime = ServerInfo::uptime();
		if (uptime > lastSet + showTimeInSeconds) {
			CycleExhibitLOT();
		}
	}

};

#endif