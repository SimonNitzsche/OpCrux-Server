#ifndef __REPLICA__COMPONENTS__MODULE_ASSEMBLY_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__MODULE_ASSEMBLY_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

class ModuleAssemblyComponent : public IEntityComponent {
public:

	ModuleAssemblyComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	bool isDirty = false;
	DataTypes::LWOOBJID subkey;
	bool useOptionalParts = false;
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
				factory->Write(useOptionalParts);
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

	void onModuleAssemblyQueryData() {
		// TODO: Load from db.
		std::u16string ma = u"1:8129;1:8130;1:13513;1:13512;1:13515;1:13516;1:13514;";
		this->SetAssembly(ma);

		{ GM::ModuleAssemblyDBDataForClient msg; msg.assemblyID = 0; msg.blob = ma; GameMessages::Broadcast(this->owner->GetZoneInstance(), this->owner, msg); }
	}

};

#endif