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
public:
	bool _buffsDirty = true;
	struct StatBuffs {
		std::uint32_t buff_0=100;
		std::uint32_t buff_1=100;
		std::uint32_t buff_2=100;
		std::uint32_t buff_3=100;
		std::uint32_t buff_4=100;
		std::uint32_t buff_5=100;
		std::uint32_t buff_6=100;
		std::uint32_t buff_7=100;
		std::uint32_t buff_8=100;
	} buffs;

public:
	struct Attributes {
		std::int32_t currentHealth;
		std::float_t dupMaxHealth;
		std::int32_t currentArmor;
		std::float_t dupMaxArmor;
		std::int32_t currentImagination;
		std::float_t dupMaxImagination;
		std::int32_t damageAbsorptionPoints;
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

	void SetDirty() {
		_isDirtyFlagAttributes = true;
		owner->SetDirty();
	}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		
		// Parameters
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
			_buffsDirty = true;
			factory->Write(_buffsDirty);
			if (_buffsDirty) {
				factory->Write<std::uint32_t>(buffs.buff_0);
				factory->Write<std::uint32_t>(buffs.buff_1);
				factory->Write<std::uint32_t>(buffs.buff_2);
				factory->Write<std::uint32_t>(buffs.buff_3);
				factory->Write<std::uint32_t>(buffs.buff_4);
				factory->Write<std::uint32_t>(buffs.buff_5);
				factory->Write<std::uint32_t>(buffs.buff_6);
				factory->Write<std::uint32_t>(buffs.buff_7);
				factory->Write<std::uint32_t>(buffs.buff_8);
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