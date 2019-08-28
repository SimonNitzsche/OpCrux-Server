#ifndef __REPLICA__COMPONENTS__LUP_EXHIBIT_COMPONENT_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__LUP_EXHIBIT_COMPONENT_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "Utils/ServerInfo.hpp"

using namespace DataTypes;

class LUPExhibitComponent : public IEntityComponent {
private:
	bool _isDirty = false;
	std::int32_t exhibitedLOT=0;

	std::vector<int32_t> exhibitLOTs = {};
	long long lastupdate = 0;

	static const int showTimeInSeconds = 20;
public:

	LUPExhibitComponent() : IEntityComponent() {}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		factory->Write(_isDirty && exhibitedLOT!=0);
		if (_isDirty && exhibitedLOT!=0) {
			factory->Write<std::int32_t>(exhibitedLOT);
			_isDirty = false;
		}
	}

	void Update() {
		long long uptime = ServerInfo::uptime();
		// TODO.
	}

};

#endif