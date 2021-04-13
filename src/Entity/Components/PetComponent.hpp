#ifndef __REPLICA__COMPONENTS__Pet_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__Pet_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

using namespace DataTypes;

class PetComponent : public IEntityComponent {
private:
	bool _isDirty = false;

	std::uint32_t unknown1 = 0x80;
	std::uint32_t unknown2 = 0x80;

	DataTypes::LWOOBJID interactionObject = std::uint64_t(0);
	DataTypes::LWOOBJID ownerID = std::uint64_t(0);

	bool _isNameDirty = false;
	std::uint32_t moderationStatus = 0;
	std::u16string petName = u"";
	std::u16string ownerName = u"";

public:

	PetComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 26; }

	void Serialize(RakNet::BitStream* factory, ReplicaTypes::PacketTypes packetType) {
		factory->Write(_isDirty);
		if (_isDirty) {
			factory->Write(unknown1);
			factory->Write(unknown2);

			factory->Write(interactionObject != std::uint64_t(0));
			if (interactionObject != std::uint64_t(0)) {
				factory->Write(interactionObject);
			}

			factory->Write(ownerID != std::uint64_t(0));
			if (ownerID != std::uint64_t(0)) {
				factory->Write(ownerID);
			}

			factory->Write(_isNameDirty);
			if (_isNameDirty) {
				factory->Write(moderationStatus);
				StringUtils::writeWStringToBitStream<std::uint8_t>(factory, petName);
				StringUtils::writeWStringToBitStream<std::uint8_t>(factory, ownerName);
			}
		}
	}

	void Update() {

	}

	void PopulateFromLDF(LDFCollection* collection) {
		

	}

	void Awake() {
		
	}

};

#endif