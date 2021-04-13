#ifndef __REPLICA__COMPONENTS__MODULE_ASSEMBLY_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__MODULE_ASSEMBLY_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "Entity/GameMessages/ModuleAssemblyQueryData.hpp"

class ModuleAssemblyComponent : public IEntityComponent {
public:

	ModuleAssemblyComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	bool isDirty = false;
	DataTypes::LWOOBJID subkey;
	bool useOptionalParts = false;
	std::u16string assemblyPartLOTs;

	static constexpr int GetTypeID() { return 61; }

	void Serialize(RakNet::BitStream* factory, ReplicaTypes::PacketTypes packetType) {
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
			factory->Write(isDirty);
			if (isDirty) {
				factory->Write(subkey != std::uint64_t(0));
				if (subkey != std::uint64_t(0)) {
					factory->Write(subkey);
				}
				factory->Write(useOptionalParts);
				StringUtils::writeWStringToBitStream<std::uint16_t>(factory, assemblyPartLOTs);
			}
		}
	}

	void SetSubkey(DataTypes::LWOOBJID _subkey) {
		subkey = _subkey;
		isDirty = true;
		owner->SetDirty();
	}

	void SetAssembly(std::u16string _assembly) {
		assemblyPartLOTs = _assembly;
		isDirty = true;
		owner->SetDirty();
	}

	void onModuleAssemblyQueryData(Entity::GameObject * sender, GM::ModuleAssemblyQueryData * msg) {
		// TODO: Load from db.
		std::u16string ma = u"1:8129;1:8130;1:13513;1:13512;1:13515;1:13516;1:13514;";
		this->SetAssembly(ma);

		{ GM::ModuleAssemblyDBDataForClient nmsg; nmsg.assemblyID = 0; nmsg.blob = ma; GameMessages::Broadcast(this->owner->GetZoneInstance(), this->owner, nmsg); }
	}

	void RegisterMessageHandlers() {
		REGISTER_OBJECT_MESSAGE_HANDLER(ModuleAssemblyComponent, GM::ModuleAssemblyQueryData, onModuleAssemblyQueryData);
	}
};

#endif