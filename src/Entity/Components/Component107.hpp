#ifndef __REPLICA__COMPONENTS__COMPONENT_107_HPP__
#define __REPLICA__COMPONENTS__COMPONENT_107_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

//using namespace Entity::Components::Interface;

class Component107 : public IEntityComponent {
private:
	bool _isDirtyPossessingObject = false;
	DataTypes::LWOOBJID possessingObject = 0;

public:

	DataTypes::LWOOBJID GetPossessingObject() {
		return possessingObject;
	}

	void SetPossessingObject(DataTypes::LWOOBJID objectID) {
		possessingObject = objectID;
		_isDirtyPossessingObject = true;
	}

	Component107() : IEntityComponent() {}

	static constexpr int GetTypeID() { return 107; }

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		ENABLE_FLAG_ON_CONSTRUCTION(_isDirtyPossessingObject);
		factory->Write(_isDirtyPossessingObject);
		if (_isDirtyPossessingObject) {
			factory->Write<std::uint64_t>(possessingObject);
			_isDirtyPossessingObject = false;
		}
	}

};

#endif