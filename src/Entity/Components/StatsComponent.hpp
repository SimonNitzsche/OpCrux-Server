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
	bool _useCustomParameters = false;
	std::uint32_t parameters[9] {0,0,0,0,0,0,0,0,0};

public:
	struct Attributes {
		std::uint32_t currentHealth;
		std::float_t dupMaxHealth;
		std::uint32_t currentArmor;
		std::float_t dupMaxArmor;
		std::uint32_t currentImagination;
		std::float_t dupMaxImagination;
		std::uint32_t damageAbsorptionPoints;
		bool immunity=false;
		bool isGMImmune=false;
		bool isShielded=false;
		std::float_t maxHealth;
		std::float_t maxArmor;
		std::float_t maxImagination;
		std::vector<std::int32_t> factions;
		bool isSmashable=true;
	};
private:
	bool _isDirtyFlagAttributes=false;
public:
	Attributes attributes;

public:

	StatsComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 200; }

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		
		// Parameters
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
			_useCustomParameters = true;
			factory->Write(_useCustomParameters);
			if (_useCustomParameters) {
				factory->Write(parameters[0]);
				factory->Write(parameters[1]);
				factory->Write(parameters[2]);
				factory->Write(parameters[3]);
				factory->Write(parameters[4]);
				factory->Write(parameters[5]);
				factory->Write(parameters[6]);
				factory->Write(parameters[7]);
				factory->Write(parameters[8]);
			}
		}

		// Attributes
		// ENABLE_FLAG_ON_CONSTRUCTION(_isDirtyFlagAttributes);
		//if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION)
			//_isDirtyFlagAttributes = true;
		factory->Write(_isDirtyFlagAttributes);
		if (_isDirtyFlagAttributes) {
			factory->Write(attributes.currentHealth);
			factory->Write(attributes.dupMaxHealth);
			factory->Write(attributes.currentArmor);
			factory->Write(attributes.dupMaxArmor);
			factory->Write(attributes.currentImagination);
			factory->Write(attributes.dupMaxImagination);
			factory->Write(attributes.damageAbsorptionPoints);
			factory->Write(attributes.immunity);
			factory->Write(attributes.isGMImmune);
			factory->Write(attributes.isShielded);
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
		_isDirtyFlagAttributes = false;

		// Unknown flag and data bit
		factory->Write(true);
		if (true) {
			factory->Write(false);
		}
	}

};

#endif