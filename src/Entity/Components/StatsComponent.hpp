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

	static constexpr std::int16_t GetTypeID() { return 200; }

	void SetDirty() {
		_isDirtyFlagAttributes = true;
		owner->SetDirty();
	}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		
		// Parameters
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
			_useCustomParameters = true;
			factory->Write(_useCustomParameters);
			if (_useCustomParameters) {
				factory->Write<std::uint32_t>(parameters[0]);
				factory->Write<std::uint32_t>(parameters[1]);
				factory->Write<std::uint32_t>(parameters[2]);
				factory->Write<std::uint32_t>(parameters[3]);
				factory->Write<std::uint32_t>(parameters[4]);
				factory->Write<std::uint32_t>(parameters[5]);
				factory->Write<std::uint32_t>(parameters[6]);
				factory->Write<std::uint32_t>(parameters[7]);
				factory->Write<std::uint32_t>(parameters[8]);
			}
		}

		// Attributes
		// ENABLE_FLAG_ON_CONSTRUCTION(_isDirtyFlagAttributes);
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION)
			_isDirtyFlagAttributes = true;


		factory->Write(_isDirtyFlagAttributes);
		if (_isDirtyFlagAttributes) {
			factory->Write<std::uint32_t>(attributes.currentHealth);
			factory->Write<std::float_t>(attributes.dupMaxHealth);
			factory->Write<std::uint32_t>(attributes.currentArmor);
			factory->Write<std::float_t>(attributes.dupMaxArmor);
			factory->Write<std::uint32_t>(attributes.currentImagination);
			factory->Write<std::float_t>(attributes.dupMaxImagination);
			factory->Write<std::uint32_t>(attributes.damageAbsorptionPoints);
			factory->Write<bool>(attributes.immunity);
			factory->Write<bool>(attributes.isGMImmune);
			factory->Write<bool>(attributes.isShielded);
			factory->Write<std::float_t>(attributes.maxHealth);
			factory->Write<std::float_t>(attributes.maxArmor);
			factory->Write<std::float_t>(attributes.maxImagination);
			factory->Write<std::uint32_t>(attributes.factions.size());
			for (std::int32_t faction : attributes.factions)
				factory->Write<std::int32_t>(faction);
			factory->Write<bool>(attributes.isSmashable);
			
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
		factory->Write(false);
		if (false) {
			factory->Write(false);
		}
	}

};

#endif