#ifndef __REPLICA__COMPONENTS__MODULE_ASSEMBLY_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__MODULE_ASSEMBLY_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

class ModuleAssemblyComponent : public IEntityComponent {
public:

	ModuleAssemblyComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	bool isDirty = false;
	DataTypes::LWOOBJID subkey;
	bool unknownBit = false;
	std::u16string assembly;

	static constexpr int GetTypeID() { return 61; }

	void Serialize(RakNet::BitStream* factory, ReplicaTypes::PacketTypes packetType) {
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
			factory->Write(isDirty);
			if (isDirty) {
				factory->Write(subkey != 0ULL);
				if (subkey != 0ULL) {
					factory->Write(subkey);
				}
				factory->Write(unknownBit);
				StringUtils::writeWStringToBitStream<std::uint16_t>(factory, assembly);
			}
		}
	}

	void SetSubkey(DataTypes::LWOOBJID _subkey) {
		subkey = _subkey;
		isDirty = true;
		owner->SetDirty();
	}

	void SetAssembly(std::u16string _assembly) {
		assembly = _assembly;
		isDirty = true;
		owner->SetDirty();
	}

};

#endif