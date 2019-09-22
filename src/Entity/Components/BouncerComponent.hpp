#ifndef __REPLICA__COMPONENTS__BOUNCER_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__BOUNCER_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"


using namespace DataTypes;

class BouncerComponent : public IEntityComponent {
private:
	bool _isDirtyFlag = true;
	bool bEnabled = true;

public:

	BouncerComponent() : IEntityComponent() {}

	static constexpr int GetTypeID() { return 6; }

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		factory->Write(_isDirtyFlag);
		if (_isDirtyFlag) {
			factory->Write(bEnabled);
		}
	}

	void SetEnabled(bool enabled) {
		bEnabled = enabled;
		owner->SetDirty();
	}

	void PopulateFromLDF(LDFCollection collection) {
		// TODO
		/* SAMPLE: */
		/*
			attached_cinematic_path=0:HangarBouncePort
			bouncer_destination=0:-272.1034602.2717-34.3679
			bouncer_speed=3:83
			bouncer_uses_high_arc=7:1
			bounding_radius_override=3:0
			lead_in_time=3:0
			lock_controls=7:1
			num_targets_to_activate=1:1
			respond_to_faction=0:1
			stickLanding=7:0
		*/
	}

};

#endif