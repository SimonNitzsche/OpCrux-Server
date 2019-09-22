#ifndef __REPLICA__COMPONENTS__STATS_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__STATS_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include <vector>

using namespace DataTypes;

/*
	IMPORTANT:	THIS IS A SHARED COMPONENT!
				ONLY SERIALIZE THIS ONE THROUGH COMPONENTS AND ONLY ONCE!
				DON'T EVER PUT IT INTO THE SERIALIZATION OF THE GAMEOBJECT CLASS.
*/

class StatsComponent : public IEntityComponent {
private:
	bool _isDirtyFlagParameters = false;
	std::uint32_t parameters[9];

public:
	struct Attributes {
		std::uint32_t currentHealth;
		std::float_t dupMaxHealth;
		std::uint32_t currentArmor;
		std::float_t dupMaxArmor;
		std::uint32_t currentImagination;
		std::float_t dupMaxImagination;
		std::uint32_t unknown1;
		bool unknown2;
		bool unknown3;
		bool unknown4;
		std::float_t maxHealth;
		std::float_t maxArmor;
		std::float_t maxImagination;
		std::vector<std::int32_t> factions;
		bool isSmashable;
	};
private:
	bool _isDirtyFlagAttributes=false;
public:
	Attributes attributes;

public:

	StatsComponent() : IEntityComponent() {}

	static constexpr int GetTypeID() { return 200; }

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		
		// Parameters
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
			factory->Write(_isDirtyFlagParameters);
			if (_isDirtyFlagParameters) {
				factory->Write(parameters);
				_isDirtyFlagParameters = false;
			}
		}

		// Attributes
		factory->Write(_isDirtyFlagAttributes);
		if (_isDirtyFlagAttributes) {
			factory->Write(attributes.currentHealth);
			factory->Write(attributes.dupMaxHealth);
			factory->Write(attributes.currentArmor);
			factory->Write(attributes.dupMaxArmor);
			factory->Write(attributes.currentImagination);
			factory->Write(attributes.dupMaxImagination);
			factory->Write(attributes.unknown1);
			factory->Write(attributes.unknown2);
			factory->Write(attributes.unknown3);
			factory->Write(attributes.unknown4);
			factory->Write(attributes.maxHealth);
			factory->Write(attributes.maxArmor);
			factory->Write(attributes.maxImagination);
			factory->Write<std::uint32_t>(attributes.factions.size());
			for (std::int32_t faction : attributes.factions)
				factory->Write<std::int32_t>(faction);
			factory->Write(attributes.isSmashable);
			
			// Unknown stuff
			if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
				factory->Write(false);
				factory->Write(false);
				if (attributes.isSmashable) {
					factory->Write(false);
					factory->Write(false);
				}
			}
		}

		// Unknown flag and data bit
		factory->Write(false);
		if (false) {
			factory->Write(false);
		}
	}

};

#endif