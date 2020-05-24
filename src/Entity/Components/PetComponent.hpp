#ifndef __REPLICA__COMPONENTS__Pet_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__Pet_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

using namespace DataTypes;

class PetComponent : public IEntityComponent {
private:
	bool _isDirty = false;

	std::uint32_t unknown1 = 0x80;
	std::uint32_t unknown2 = 0x80;

	LWOOBJID interactionObject = 0ULL;
	LWOOBJID ownerID = 0ULL;

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

			factory->Write(interactionObject != 0ULL);
			if (interactionObject != 0ULL) {
				factory->Write(interactionObject);
			}

			factory->Write(ownerID != 0ULL);
			if (ownerID != 0ULL) {
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