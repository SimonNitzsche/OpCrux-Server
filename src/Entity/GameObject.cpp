#include "GameObject.hpp"

#include "Utils/ServerInfo.hpp"
#include "Server/WorldServer.hpp"
#include "Missions/MissionManager.hpp"

#include <locale>
#include <string>
#include <codecvt>

//using namespace Entity::Components::Interface;
#define SERIALIZE_COMPONENT_IF_ATTACHED(COMP_T) {COMP_T * comp = this->GetComponent<COMP_T>(); if(comp != nullptr) { /*Logger::log("WRLD", "Serializing "+std::string(#COMP_T)+"...");*/ comp->Serialize(factory, packetType);}}
#define COMPONENT_ONADD_SWITCH_CASE(COMP_T) {\
	case COMP_T::GetTypeID(): {\
		COMP_T * comp = new COMP_T(compID);\
		components.insert(std::make_pair(COMP_T::GetTypeID(), comp));\
		comp->SetOwner(this); \
		comp->OnEnable();\
		/*Logger::log("WRLD", "Added Component "+std::string(#COMP_T)+"!");*/\
		if(comp == nullptr) {\
			throw new std::runtime_error(std::string(#COMP_T)+" resultet into a nullptr.");\
		}\
		return comp;\
		break;\
	}\
}

#include "GameCache/ComponentsRegistry.hpp"

#include "Utils/StringUtils.hpp"

// Components
#include "Entity/Components/BaseCombatAIComponent.hpp"
#include "Entity/Components/BouncerComponent.hpp"
#include "Entity/Components/BuildBorderComponent.hpp"
#include "Entity/Components/CharacterComponent.hpp"
#include "Entity/Components/CollectibleComponent.hpp"
#include "Entity/Components/Component107.hpp"
#include "Entity/Components/ControllablePhysicsComponent.hpp"
#include "Entity/Components/DestructibleComponent.hpp"
#include "Entity/Components/GenericActivatorComponent.hpp"
#include "Entity/Components/InventoryComponent.hpp"
#include "Entity/Components/ItemComponent.hpp"
#include "Entity/Components/LUPExhibitComponent.hpp"
#include "Entity/Components/MinifigComponent.hpp"
#include "Entity/Components/MinigameComponent.hpp"
#include "Entity/Components/MissionOfferComponent.hpp"
#include "Entity/Components/ModuleAssemblyComponent.hpp"
#include "Entity/Components/MovementAIComponent.hpp"
#include "Entity/Components/MovingPlatformComponent.hpp"
#include "Entity/Components/PropertyComponent.hpp"
#include "Entity/Components/PropertyEntranceComponent.hpp"
#include "Entity/Components/PropertyManagementComponent.hpp"
#include "Entity/Components/PropertyPlaqueComponent.hpp"
#include "Entity/Components/PropertyVendorComponent.hpp"
#include "Entity/Components/ProximityMonitorComponent.hpp"
#include "Entity/Components/PhantomPhysicsComponent.hpp"
#include "Entity/Components/PlatformBoundaryComponent.hpp"
#include "Entity/Components/PossessableComponent.hpp"
#include "Entity/Components/QuickBuildComponent.hpp"
#include "Entity/Components/RacingControlComponent.hpp"
#include "Entity/Components/RacingStatsComponent.hpp"
#include "Entity/Components/RebuildComponent.hpp"
#include "Entity/Components/RenderComponent.hpp"
#include "Entity/Components/RocketLaunchComponent.hpp"
#include "Entity/Components/ScriptComponent.hpp"
#include "Entity/Components/ScriptedActivityComponent.hpp"
#include "Entity/Components/SimplePhysicsComponent.hpp"
#include "Entity/Components/SkillComponent.hpp"
#include "Entity/Components/SoundAmbient3DComponent.hpp"
#include "Entity/Components/SoundTriggerComponent.hpp"
#include "Entity/Components/SpawnerComponent.hpp"
#include "Entity/Components/StatsComponent.hpp"
#include "Entity/Components/SwitchComponent.hpp"
#include "Entity/Components/TriggerComponent.hpp"
#include "Entity/Components/VehiclePhysicsComponent.hpp"
#include "Entity/Components/VendorComponent.hpp"

ReplicaReturnResult Entity::GameObject::SendConstruction(RakNetTime currentTime, SystemAddress systemAddress, unsigned int &flags, RakNet::BitStream *outBitStream, bool *includeTimestamp) {
	this->Serialize(outBitStream, ReplicaTypes::PacketTypes::CONSTRUCTION);
	Instance->replicaManager->SetScope(this, true, UNASSIGNED_SYSTEM_ADDRESS, true);
	return REPLICA_PROCESSING_DONE;
}
ReplicaReturnResult Entity::GameObject::SendDestruction(RakNet::BitStream *outBitStream, SystemAddress systemAddress, bool *includeTimestamp) {
	this->Serialize(outBitStream, ReplicaTypes::PacketTypes::DESTRUCTION);
	return REPLICA_PROCESSING_DONE;
}
ReplicaReturnResult Entity::GameObject::ReceiveDestruction(RakNet::BitStream *inBitStream, SystemAddress systemAddress, RakNetTime timestamp) {
	return REPLICA_PROCESSING_DONE;
}
ReplicaReturnResult Entity::GameObject::SendScopeChange(bool inScope, RakNet::BitStream *outBitStream, RakNetTime currentTime, SystemAddress systemAddress, bool *includeTimestamp) {
	outBitStream->Write(inScope);
	return REPLICA_PROCESSING_DONE;
}
ReplicaReturnResult Entity::GameObject::ReceiveScopeChange(RakNet::BitStream *inBitStream, SystemAddress systemAddress, RakNetTime timestamp) {
	return REPLICA_PROCESSING_DONE;
}
ReplicaReturnResult Entity::GameObject::Serialize(bool *sendTimestamp, RakNet::BitStream *outBitStream, RakNetTime lastSendTime, PacketPriority *priority, PacketReliability *reliability, RakNetTime currentTime, SystemAddress systemAddress, unsigned int &flags) {
	this->Serialize(outBitStream, ReplicaTypes::PacketTypes::SERIALIZATION);
	return REPLICA_PROCESSING_DONE;
}
ReplicaReturnResult Entity::GameObject::Deserialize(RakNet::BitStream *inBitStream, RakNetTime timestamp, RakNetTime lastDeserializeTime, SystemAddress systemAddress) {
	return REPLICA_PROCESSING_DONE;
}

Entity::GameObject::GameObject(WorldServer * instance, std::uint32_t LOT) {
	this->Instance = instance;
	this->LOT = LOT;

	this->groups = {};

	if (LOT == 0) {
		Logger::log("WRLD", "Failed to spawn object: LOT 0 is blacklisted!", LogType::ERR);
		return;
	}

	auto component_types_to_be_added = CacheComponentsRegistry::GetObjectComponentTypes(LOT);

	this->creationTimestamp = ServerInfo::uptime();

	// Add components
	for (auto component_type : component_types_to_be_added) {
		this->AddComponentByID(component_type.first, component_type.second);
	}
}

void Entity::GameObject::Finish() {
	// Call Awake
	for (auto component : components) {
		component.second->Awake();
	}

	// Call Startup
	for (auto component : components) {
		component.second->Start();
	}
}

Entity::GameObject::~GameObject() {
	for (auto reference : components) {
		if (reference.second) {
			delete[] reference.second;
		}
	}
}

void Entity::GameObject::SetObjectID(LWOOBJID ID) {
	this->objectID = ID;
}

DataTypes::LWOOBJID Entity::GameObject::GetObjectID() {
	return this->objectID;
}

void Entity::GameObject::Update() {
	for (auto oPair : components)
		oPair.second->Update();
}

void Entity::GameObject::PhysicUpdate() {
	for (auto oPair : components)
		oPair.second->PhysicUpdate();
}

void Entity::GameObject::Tick() {
	Update();
}

IEntityComponent* Entity::GameObject::GetComponentByType(int id) {
	if (id == -1) throw new std::runtime_error("Invalid Component Type (-1)!");
	auto it = components.find(id);
	if(it != components.end())
		return it->second;
	return nullptr;
}

template<class T>
T * Entity::GameObject::GetComponent() {
	return static_cast<T*>(GetComponentByType(T::GetTypeID()));
}

IEntityComponent * Entity::GameObject::AddComponentByID(int id, int compID) {
	switch (id) {
		/* ========== SERIALIZED ========== */
		COMPONENT_ONADD_SWITCH_CASE(StatsComponent);
		COMPONENT_ONADD_SWITCH_CASE(PossessableComponent);
		COMPONENT_ONADD_SWITCH_CASE(ModuleAssemblyComponent);
		COMPONENT_ONADD_SWITCH_CASE(ControllablePhysicsComponent);
		COMPONENT_ONADD_SWITCH_CASE(SimplePhysicsComponent);
		//COMPONENT_ONADD_SWITCH_CASE(RigidBodyPhantomPhysicsComponent);
		COMPONENT_ONADD_SWITCH_CASE(VehiclePhysicsComponent);
		COMPONENT_ONADD_SWITCH_CASE(PhantomPhysicsComponent);
		COMPONENT_ONADD_SWITCH_CASE(DestructibleComponent);
		COMPONENT_ONADD_SWITCH_CASE(CollectibleComponent);
		//COMPONENT_ONADD_SWITCH_CASE(PetComponent);
		COMPONENT_ONADD_SWITCH_CASE(CharacterComponent);
		//COMPONENT_ONADD_SWITCH_CASE(ShootingGalleryComponent);
		COMPONENT_ONADD_SWITCH_CASE(InventoryComponent);
		COMPONENT_ONADD_SWITCH_CASE(ScriptComponent);
		COMPONENT_ONADD_SWITCH_CASE(SkillComponent);
		COMPONENT_ONADD_SWITCH_CASE(ItemComponent);
		COMPONENT_ONADD_SWITCH_CASE(BaseCombatAIComponent);
		COMPONENT_ONADD_SWITCH_CASE(QuickbuildComponent);
		COMPONENT_ONADD_SWITCH_CASE(MovingPlatformComponent);
		COMPONENT_ONADD_SWITCH_CASE(SwitchComponent);
		COMPONENT_ONADD_SWITCH_CASE(VendorComponent);
		COMPONENT_ONADD_SWITCH_CASE(BouncerComponent);
		COMPONENT_ONADD_SWITCH_CASE(ScriptedActivityComponent);
		COMPONENT_ONADD_SWITCH_CASE(RacingControlComponent);
		COMPONENT_ONADD_SWITCH_CASE(LUPExhibitComponent);
		//COMPONENT_ONADD_SWITCH_CASE(ModelComponent);
		COMPONENT_ONADD_SWITCH_CASE(RenderComponent);
		COMPONENT_ONADD_SWITCH_CASE(MinigameComponent);
		COMPONENT_ONADD_SWITCH_CASE(Component107);
		COMPONENT_ONADD_SWITCH_CASE(TriggerComponent);
		/* ========== NON-SERIALIZED ========== */
		COMPONENT_ONADD_SWITCH_CASE(BuildBorderComponent);
		COMPONENT_ONADD_SWITCH_CASE(GenericActivatorComponent);
		COMPONENT_ONADD_SWITCH_CASE(MovementAIComponent);
		COMPONENT_ONADD_SWITCH_CASE(PropertyComponent);
		COMPONENT_ONADD_SWITCH_CASE(PropertyEntranceComponent);
		COMPONENT_ONADD_SWITCH_CASE(PropertyManagementComponent);
		COMPONENT_ONADD_SWITCH_CASE(PropertyPlaqueComponent);
		COMPONENT_ONADD_SWITCH_CASE(PropertyVendorComponent);
		COMPONENT_ONADD_SWITCH_CASE(ProximityMonitorComponent);
		COMPONENT_ONADD_SWITCH_CASE(RacingStatsComponent);
		COMPONENT_ONADD_SWITCH_CASE(RocketLaunchComponent);
		COMPONENT_ONADD_SWITCH_CASE(SpawnerComponent);
		COMPONENT_ONADD_SWITCH_CASE(MinifigComponent);
		COMPONENT_ONADD_SWITCH_CASE(MissionOfferComponent);
		COMPONENT_ONADD_SWITCH_CASE(PlatformBoundaryComponent);
		COMPONENT_ONADD_SWITCH_CASE(RebuildComponent);
		COMPONENT_ONADD_SWITCH_CASE(SoundAmbient3DComponent);
		COMPONENT_ONADD_SWITCH_CASE(SoundTriggerComponent);

	default: {
		Logger::log("WRLD", "Couldn't add component #" + std::to_string(id) + " to GameObject!", LogType::UNEXPECTED);
		isSerializable = false;
		return nullptr;
	}
	}
	if (GetComponentByType(id) == nullptr) {
		Logger::log("WRLD", "Failed to add component #" + std::to_string(id) + " to GameObject!", LogType::UNEXPECTED);
	}
}

template<class T>
T * Entity::GameObject::AddComponent(std::int32_t componentID) {
	return static_cast<T*>(this->AddComponentByID(T::GetTypeID(), componentID));
}

void Entity::GameObject::Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
	SerializeBaseData(factory, packetType);
	SerializeComponents(factory, packetType);
	objectDirty = false;


	if (false) {
		Logger::log("WRLD", "Saving serialization for LOT " + std::to_string(this->LOT), LogType::PASSED);
		RakNet::BitStream copyBs = RakNet::BitStream();
		copyBs.Write<std::uint8_t>(packetType == ReplicaTypes::PacketTypes::CONSTRUCTION ? 0x24 : 0x27);
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
			if (this->GetNetworkID() != UNASSIGNED_NETWORK_ID)
			{
				copyBs.Write(true);
				copyBs.Write(this->GetNetworkID());
			}
			else
				copyBs.Write(false);
		}
		else {
			copyBs.Write<std::uint16_t>(this->networkID.localSystemAddress);
		}
		factory->SetReadOffset(0);
		copyBs.Write(factory, factory->GetNumberOfBitsUsed());
		factory->SetReadOffset(0);
		copyBs.Write(reinterpret_cast<char*>(factory->GetData()), factory->GetNumberOfBytesUsed());
		FileUtils::SavePacket(&copyBs, 2000, 50000);
	}
}

void Entity::GameObject::SerializeComponents(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
	SERIALIZE_COMPONENT_IF_ATTACHED(PossessableComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(ModuleAssemblyComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(ControllablePhysicsComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(SimplePhysicsComponent);
	//SERIALIZE_COMPONENT_IF_ATTACHED(RigidBodyPhantomPhysicsComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(VehiclePhysicsComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(PhantomPhysicsComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(DestructibleComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(CollectibleComponent);
	//SERIALIZE_COMPONENT_IF_ATTACHED(PetComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(CharacterComponent);
	//SERIALIZE_COMPONENT_IF_ATTACHED(ShootingGalleryComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(InventoryComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(ScriptComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(SkillComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(ItemComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(BaseCombatAIComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(QuickbuildComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(MovingPlatformComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(SwitchComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(VendorComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(BouncerComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(ScriptedActivityComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(RacingControlComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(LUPExhibitComponent);
	//SERIALIZE_COMPONENT_IF_ATTACHED(ModelComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(RenderComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(MinigameComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(Component107);
	SERIALIZE_COMPONENT_IF_ATTACHED(TriggerComponent);
}

#define VNAME(x) #x

#define ASSERT_MEMBER_VALIDATION(factory, type, valMem, check) \
{type valMem; factory->Read<type>(valMem); if(!(check)) throw new std::runtime_error(std::string("Serialization for "+std::string(#valMem)+" is broken."));}
constexpr auto VALIDATE_MEMBERS = (false);

void Entity::GameObject::SerializeBaseData(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
	auto _writeOffsetBegin = factory->GetWriteOffset();
	auto _localTimeSinceOnServer = ServerInfo::uptime() - creationTimestamp;
	if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
		factory->Write<std::uint64_t>(objectID);
		factory->Write<std::int32_t>(LOT);
		StringUtils::writeWStringToBitStream<std::uint8_t>(factory, name);

		factory->Write<uint32_t>(_localTimeSinceOnServer);

		{
			factory->Write(false);
			// TODO! -> compressed LDF
		}

		//factory->Write(false);
		factory->Write(this->GetComponent<TriggerComponent>() != nullptr);

		factory->Write(spawner != nullptr);
		if (spawner != nullptr) { factory->Write(spawner->objectID); }
		
		factory->Write(false); // disable commented out below
		//factory->Write(spawner_node != 0xFFFFFFFF);
		//if (spawner_node != 0xFFFFFFFF) { factory->Write(spawner_node); }

		// Object Scale
		factory->Write(true);
		if (true)
			factory->Write(this->scale);
		
		// object world state
		factory->Write(false);

		// gmlevel
		factory->Write(false);
	}

	/*factory->Write(baseDataDirty);
	if (baseDataDirty) {*/
	factory->Write(true);
	if(true) {
		factory->Write(parent != nullptr);
		if (parent != nullptr) {
			factory->Write<std::uint64_t>(parent->objectID);
			factory->Write(false);
		}
		factory->Write(children.size() != 0);
		if (children.size() != 0) {
			factory->Write<std::uint16_t>(children.size());
			for (int i = 0; i < children.size(); ++i) {
				factory->Write<std::uint64_t>(children.at(i)->objectID);
			}
		}
	}

	baseDataDirty = false;

	/// Validate.
	if(VALIDATE_MEMBERS) {
		auto _readOffsetBegin = factory->GetReadOffset();
		factory->SetReadOffset(_writeOffsetBegin);
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
			ASSERT_MEMBER_VALIDATION(factory, std::uint64_t, _valMem_00, _valMem_00 == objectID);
			ASSERT_MEMBER_VALIDATION(factory, std::int32_t, _valMem_01, _valMem_01 == LOT);
			ASSERT_MEMBER_VALIDATION(factory, std::uint8_t, _valMem_02, _valMem_02 == name.size());
			for (int _val_I = 0; _val_I < name.size(); ++_val_I)
				ASSERT_MEMBER_VALIDATION(factory, char16_t, _valMem_02_n, _valMem_02_n == name.at(_val_I));
			ASSERT_MEMBER_VALIDATION(factory, std::uint32_t, _valMem_03, _valMem_03 == _localTimeSinceOnServer);
			ASSERT_MEMBER_VALIDATION(factory, bool, _valMem_04, _valMem_04 == false); // compressed stuff
			ASSERT_MEMBER_VALIDATION(factory, bool, _valMem_05, _valMem_05 == false); // trigger
			ASSERT_MEMBER_VALIDATION(factory, bool, _valMem_06, _valMem_06 == (spawner != nullptr));
			if (spawner != nullptr)
				ASSERT_MEMBER_VALIDATION(factory, std::uint64_t, _valMem_06_01, _valMem_06_01 == spawner->GetObjectID());
			ASSERT_MEMBER_VALIDATION(factory, bool, _valMem_07, _valMem_07 == false); // spawner_node_id
			ASSERT_MEMBER_VALIDATION(factory, bool, _valMem_08, _valMem_08 == (scale != 1.f));
			if (scale != 1.f)
				ASSERT_MEMBER_VALIDATION(factory, std::float_t, _valMem_08_01, _valMem_08_01 == scale);
			ASSERT_MEMBER_VALIDATION(factory, bool, _valMem_09, _valMem_09 == false); // object world state
			ASSERT_MEMBER_VALIDATION(factory, bool, _valMem_10, _valMem_10 == false); // gmlevel?
			ASSERT_MEMBER_VALIDATION(factory, bool, _valMem_11, _valMem_11 == false); // parent/children
		}
		factory->SetReadOffset(_readOffsetBegin);
	}

}

void Entity::GameObject::AddChild(GameObject * child) {
	children.push_back(child);
	child->SetParent(child);
	baseDataDirty = true;
	objectDirty = true;
}

void Entity::GameObject::SetParent(GameObject * parent) {
	this->parent = parent;
	baseDataDirty = true;
	objectDirty = true;
}

void Entity::GameObject::SetSpawner(GameObject * spawner, std::uint32_t spawnerNodeID) {
	this->spawner = spawner;
	this->spawner_node = spawnerNodeID;
}

bool Entity::GameObject::IsWithinGroup(std::u16string groupName) {
	for (auto g : this->groups) {
		if (g == groupName)
			return true;
	}
	return false;
}

void Entity::GameObject::PopulateFromLDF(LDFCollection * collection) {
	
	configData = *collection;

	// TODO: Populate base data
	std::u16string groupWstr;
	LDF_GET_VAL_FROM_COLLECTION(groupWstr, collection, u"groupID", u"");
	this->groups = StringUtils::splitWString(groupWstr, u';');

	if (this->LOT != 176) {
		// Add Componets custom
		/* Script Component */ {
			std::u16string customScript = u"";
			LDF_GET_VAL_FROM_COLLECTION(customScript, collection, u"custom_script_server", u"");
			if (customScript != u"") {
				this->AddComponent<ScriptComponent>(-1);
			}
		}

		/* Trigger Component */
		std::u16string wstrTriggerID = u"";
		LDF_GET_VAL_FROM_COLLECTION(wstrTriggerID, collection, u"trigger_id", u"NULL");
		if (wstrTriggerID != u"NULL") {

			// Seperate data
			std::vector<std::u16string> spltTriggerID = StringUtils::splitWString(wstrTriggerID, L':');
			int triggerSceneID = std::stoi(StringUtils::to_string(spltTriggerID.at(0)));
			int triggerID = std::stoi(StringUtils::to_string(spltTriggerID.at(1)));

			// Get zone file
			FileTypes::LUZ::LUZone * zone = Instance->luZone;

			// Get trigger file
			if (zone->triggers.find(triggerSceneID) != zone->triggers.end()) {
				ZoneTriggerFile triggerFile = zone->triggers.at(triggerSceneID);

				// Get trigger
				for (int i = 0; i < triggerFile.triggers.size(); ++i) {
					if (triggerFile.triggers.at(i).id == triggerID) {
						this->AddComponent<TriggerComponent>(0);
						TriggerComponent * triggerComp = static_cast<TriggerComponent*>(this->GetComponent<TriggerComponent>());
						triggerComp->AssignTrigger(triggerFile.triggers.at(i));
						break;
					}
				}
			}
		}
	}
	
	for (auto component : this->components) {
		if (component.second != nullptr) {
			component.second->PopulateFromLDF(collection);
		}
		//if (this->components.size() == 1) return;
	}
}

void Entity::GameObject::SetPosition(DataTypes::Vector3 position) {
	auto controllablePhysicsComponent = this->GetComponent<ControllablePhysicsComponent>();
	if (controllablePhysicsComponent != nullptr) {
		controllablePhysicsComponent->SetPosition(position);
	}
	auto simplePhysicsComponent = this->GetComponent<SimplePhysicsComponent>();
	if (simplePhysicsComponent != nullptr) {
		simplePhysicsComponent->SetPosition(position);
	}/*
	auto rigidBodyPhantomPhysicsComponent = static_cast<RigidBodyPhantomPhysicsComponent*>(this->GetComponentByID(20));
	if (rigidBodyPhantomPhysicsComponent != nullptr) {
		rigidBodyPhantomPhysicsComponent->SetPosition(position);
	}*/
	auto vehiclePhysicsComponent = this->GetComponent<VehiclePhysicsComponent>();
	if (vehiclePhysicsComponent != nullptr) {
		vehiclePhysicsComponent->SetPosition(position);
		return;
	}
	auto phantomPhysicsComponent = this->GetComponent<PhantomPhysicsComponent>();
	if (phantomPhysicsComponent != nullptr) {
		phantomPhysicsComponent->SetPosition(position);
	}
	return;
}

void Entity::GameObject::SetRotation(DataTypes::Quaternion rotation) {
	auto controllablePhysicsComponent = this->GetComponent<ControllablePhysicsComponent>();
	if (controllablePhysicsComponent != nullptr) {
		controllablePhysicsComponent->SetRotation(rotation);
	}
	auto simplePhysicsComponent = this->GetComponent<SimplePhysicsComponent>();
	if (simplePhysicsComponent != nullptr) {
		simplePhysicsComponent->SetRotation(rotation);
	}/*
	auto rigidBodyPhantomPhysicsComponent = static_cast<RigidBodyPhantomPhysicsComponent*>(this->GetComponentByID(20));
	if (rigidBodyPhantomPhysicsComponent != nullptr) {
		rigidBodyPhantomPhysicsComponent->SetRotation(rotation);
	}*/
	auto vehiclePhysicsComponent = this->GetComponent<VehiclePhysicsComponent>();
	if (vehiclePhysicsComponent != nullptr) {
		vehiclePhysicsComponent->SetRotation(rotation);
	}
	auto phantomPhysicsComponent = this->GetComponent<PhantomPhysicsComponent>();
	if (phantomPhysicsComponent != nullptr) {
		phantomPhysicsComponent->SetRotation(rotation);
	}
}

DataTypes::Vector3 Entity::GameObject::GetPosition() {
	auto controllablePhysicsComponent = this->GetComponent<ControllablePhysicsComponent>();
	if (controllablePhysicsComponent != nullptr) {
		return controllablePhysicsComponent->GetPosition();
	}
	auto simplePhysicsComponent = this->GetComponent<SimplePhysicsComponent>();
	if (simplePhysicsComponent != nullptr) {
		return simplePhysicsComponent->GetPosition();
	}/*
	auto rigidBodyPhantomPhysicsComponent = static_cast<RigidBodyPhantomPhysicsComponent*>(this->GetComponentByID(20));
	if (rigidBodyPhantomPhysicsComponent != nullptr) {
		return rigidBodyPhantomPhysicsComponent->GetPosition();
	}*/
	auto vehiclePhysicsComponent = this->GetComponent<VehiclePhysicsComponent>();
	if (vehiclePhysicsComponent != nullptr) {
		return vehiclePhysicsComponent->GetPosition();
	}
	auto phantomPhysicsComponent = this->GetComponent<PhantomPhysicsComponent>();
	if (phantomPhysicsComponent != nullptr) {
		return phantomPhysicsComponent->GetPosition();
	}
	return DataTypes::Vector3();
}

DataTypes::Quaternion Entity::GameObject::GetRotation() {
	auto controllablePhysicsComponent = this->GetComponent<ControllablePhysicsComponent>();
	if (controllablePhysicsComponent != nullptr) {
		return controllablePhysicsComponent->GetRotation();
	}
	auto simplePhysicsComponent = this->GetComponent<SimplePhysicsComponent>();
	if (simplePhysicsComponent != nullptr) {
		return simplePhysicsComponent->GetRotation();
	}/*
	auto rigidBodyPhantomPhysicsComponent = static_cast<RigidBodyPhantomPhysicsComponent*>(this->GetComponentByID(20));
	if (rigidBodyPhantomPhysicsComponent != nullptr) {
		return rigidBodyPhantomPhysicsComponent->GetRotation();
	}*/
	auto vehiclePhysicsComponent = this->GetComponent<VehiclePhysicsComponent>();
	if (vehiclePhysicsComponent != nullptr) {
		return vehiclePhysicsComponent->GetRotation();
	}
	auto phantomPhysicsComponent = this->GetComponent<PhantomPhysicsComponent>();
	if (phantomPhysicsComponent != nullptr) {
		return phantomPhysicsComponent->GetRotation();
	}
	return DataTypes::Quaternion();
}

void Entity::GameObject::SetImagination(std::int32_t imag) {
	auto charComp = GetComponent<CharacterComponent>();
	if (charComp != nullptr) {
		charComp->SetImagination(imag);
	}
	auto destComp = GetComponent<DestructibleComponent>();
	if (destComp != nullptr) {
		destComp->SetImagination(imag);
	}
}

std::int32_t Entity::GameObject::GetImagination() {
	auto destComp = GetComponent<DestructibleComponent>();
	if (destComp != nullptr) {
		return destComp->GetImagination();
	}
	return 0;
}

void Entity::GameObject::OnCollisionPhantom(Entity::GameObject * other) {
	Logger::log("WRLD", "OnCollisionPhantom");
	for (auto component : this->components) {
		if (component.second != nullptr) {
			component.second->OnCollisionPhantom(other);
		}
	}
}

void Entity::GameObject::OnOffCollisionPhantom(Entity::GameObject * other) {
	Logger::log("WRLD", "OnOffCollisionPhantom");
	for (auto component : this->components) {
		if (component.second != nullptr) {
			component.second->OnOffCollisionPhantom(other);
		}
	}
}


#include "GameCache/MissionTasks.hpp"

void Entity::GameObject::OnHasBeenCollected(Entity::GameObject* sender, GM::HasBeenCollected* msg) {
	for (auto i : components) i.second->OnHasBeenCollected(sender, msg);
}

void Entity::GameObject::OnMissionDialogueOK(Entity::GameObject* sender, GM::MissionDialogueOK* msg) {
	for (auto i : components) i.second->OnMissionDialogueOK(sender, msg);

	// Make sure next mission gets offered on completion.
	auto misOfferComp = GetComponent<MissionOfferComponent>();
	if (misOfferComp != nullptr) {
		if (Database::HasMission(sender->GetObjectID().getPureID(), msg->missionID)) {
			std::int32_t missionState = Database::GetMission(sender->GetObjectID().getPureID(), msg->missionID).state;
			if (missionState == 8 || missionState == 9) {
				GM::RequestUse requestUseMSG;
				requestUseMSG.bIsMultiInteractUse = false;
				requestUseMSG.objectID = objectID;
				misOfferComp->OnRequestUse(sender, &requestUseMSG);
			}
		}
	}
}

void Entity::GameObject::OnRequestUse(Entity::GameObject * sender, GM::RequestUse * msg) {

	// Handle Interact task
	MissionManager::LaunchTaskEvent(Enums::EMissionTask::TALK_TO_NPC, this, sender->GetObjectID());
	MissionManager::LaunchTaskEvent(Enums::EMissionTask::INTERACT, this, sender->GetObjectID());

	/* Mailbox (Script got removed) */
	if(this->LOT == 3964) {
		Logger::log("WRLD", "GameObject::OnRequestUse TODO: Implement Mailbox", LogType::INFO);
	}

	for (auto i : components) i.second->OnRequestUse(sender, msg);
}

void Entity::GameObject::OnStartSkill(const GM::StartSkill msg) {
	SkillComponent * skillComponent = this->GetComponent<SkillComponent>();
	if (skillComponent != nullptr) {
		skillComponent->OnStartSkill(msg);
	}
}

void Entity::GameObject::OnSyncSkill(const GM::SyncSkill msg) {
	SkillComponent * skillComponent = this->GetComponent<SkillComponent>();
	if (skillComponent != nullptr) {
		skillComponent->OnSyncSkill(msg);
	}
}

void Entity::GameObject::SetProximityRadius(std::string name, float radius) {
	ScriptComponent * scriptComp = this->GetComponent<ScriptComponent>();
	if (scriptComp) {
		if (scriptComp->proximityRadii.find(name) == scriptComp->proximityRadii.end()) {
			scriptComp->proximityRadii.insert({ name, { radius, {}} });
		}
		else {
			throw new std::runtime_error("Radius already exists.");
		}
	}
	else {
		throw new std::runtime_error("Script component not attached.");
	}
}

void Entity::GameObject::PlayNDAudioEmitter(std::string guid) {
	GM::PlayNDAudioEmitter msg;
	msg.m_NDAudioEventGUID = guid;

	GameMessages::Send(this->Instance, UNASSIGNED_SYSTEM_ADDRESS, this->objectID, msg);
}

std::string Entity::GameObject::GenerateXML() {
	std::stringstream ss;

	CharacterComponent * charComponent = this->GetComponent<CharacterComponent>();
	Database::Str_DB_CharInfo charInfo = charComponent->GetCharInfo();
	Database::Str_DB_CharStyle charStyle = charComponent->GetCharStyle();

	ss << "<?xml version=\"1.0\"?>";

	ss << "<obj v=\"" << std::to_string(LOT) << "\">";
	{
		ss << "<buff/>";
		ss << "<skil/>";
		ss << "<inv csl=\"" << std::to_string(9626) << "\">";
		{
			ss << "<bag>";
			{
				// TODO
			}
			ss << "</bag>";
			ss << "<grps/>";
			ss << "<items nn=\"1\">";
			{
				// TODO
				ss << "<in t=0><i l=\"6086\" id=\"1152921507005357158\" s=\"6\" b=\"1\"/></in>";
			}
			ss << "</items>";
		}
		ss << "</inv>";
		{	
			ss << "<mf ";
				ss << "hc=\"" << std::to_string(charStyle.hairColor) << "\" ";
				ss << "hs=\"" << std::to_string(charStyle.hairStyle) << "\" ";
				ss << "hd=\"" << std::to_string(charStyle.head) << "\" ";
				ss << "t=\"" << std::to_string(charStyle.chestColor) << "\" ";
				ss << "l=\"" << std::to_string(charStyle.legs) << "\" ";
				ss << "hdc=\"" << std::to_string(charStyle.headColor) << "\" ";
				ss << "cd=\"" << std::to_string(charStyle.chest) << "\" ";
				ss << "lh=\"" << std::to_string(charStyle.leftHand) << "\" ";
				ss << "rh=\"" << std::to_string(charStyle.rightHand) << "\" ";
				ss << "es=\"" << std::to_string(charStyle.eyebrowStyle) << "\" ";
				ss << "ess=\"" << std::to_string(charStyle.eyesStyle) << "\" ";
				ss << "ms=\"" << std::to_string(charStyle.mouthStyle) << "\" ";
			ss << "/>";
		}
		{
			ss << "<char ";
				ss << "cm=\"" << std::to_string(0x7FFFFFFFFFFFFFFF) << "\" ";
				ss << "cc=\"" << charInfo.currency << "\" ";
				ss << "gm=\"" << 0 << "\" ";
				ss << "edit=\"" << 0 << "\" ";
				ss << "acct=\"" << charInfo.accountID << "\" ";
				ss << "llog=\"" << 1327707052 << "\" ";
				//ss << "ttip=\"" << Instance->sessionManager.GetSession(objectID)->systemAddress.binaryAddress << "\" ";
				//ss << "ttip=\"16777216\"";
				// ss << "mldt=\"0\ ";
				// ss << "lzid=\"2341502167811299\" ";
				// ss << "lzx=\"-25.6\" ";
				// ss << "lzy=\"463.6\" ";
				// ss << "lzz=\"185.2\" ";
				// ss << "lzrx=\"0.000000\" ";
				// ss << "lzry=\"0.924854\" ";
				// ss << "lzrz=\"0.000000\" ";
				// ss << "lzrw=\"-0.380322\" ";
				// ss << "lzcs=\"156173405\" ";
				// ss << "gid=\"0\" ";
				// ss << "gn=\"\" ";
				// ss << "lcbp=\"1:13264;1:13265;1:13266;\" ";
				// ss << "v=\"3\" ";
				// ss << "vd=\"15366\" ";
				// ss << "time=\"252519\" ";
				// ss << "lrx=\"30.5\" ";
				// ss << "lry=\"470.9\" ";
				// ss << "lrz=\"193.5\" ";
				// ss << "lrrx=\"0.000000\" ";
				// ss << "lrry=\"-0.677600\" ";
				// ss << "lrrz=\"0.000000\" ";
				// ss << "lrrw=\"0.735500\" ";
				// ss << "ls=\"93940\" ";
				// ss << "ft=\"0\" ";
				// ss << "co=\"" << Instance->sessionManager.GetSession(objectID)->systemAddress.port << "\" ";
				// ss << "stt=\"768568;13644;4683;2376;11480;345;471;22;24177;13104;13678;1951641;78;11559;947;11012;17524;16499;693599;612;1673;63;439;48;392;22;15;\"";
			ss << ">";
			{
				ss << "<ue></ue>";
				ss << "<vl></vl>";
				ss << "<zs></zs>";
			}
			ss << "</char>";
		}
		{
			ss << "<lvl l=\"" << std::to_string(charInfo.uLevel) << "\" cv=\"1\" sb=\"525\"/>";
		}
		{
			ss << "<flag>";
			{
				// TODO
				auto charComp = GetComponent<CharacterComponent>();
				auto flagChunks = charComp->GetFlagChunks();
				for (auto it = flagChunks.begin(); it != flagChunks.end(); ++it) {
					if (it->second != 0) {
						ss << "<f id=\"" << it->first << "\" v=\"" << it->second << "\"/>";
					}
				}
			}
			ss << "</flag>";
		}
		{
			ss << "<pet a=\"0\"></pet>";
		}
		{
			ss << "<mis>";
			{
				// TODO

				ss << "<done>";
				{
					auto missionsDone = Database::GetAllMissionsByStates(objectID & 0xFFFFFFFF, { 8, 9 });
					for (auto it = missionsDone.begin(); it != missionsDone.end(); ++it) {
						ss << "<m id=\"" << it->missionID << "\" cts=\"" << it->time << "\" cct=\"" << it->repeatCount << "\"/>";
					}
				}
				ss << "</done>";
				ss << "<cur>";
				{
					auto missionsActive = Database::GetAllMissionsByStates(objectID & 0xFFFFFFFF, { 2, 4, 10, 12 });
					for (auto it = missionsActive.begin(); it != missionsActive.end(); ++it) {
						ss << "<m id=\"" << it->missionID << "\">";
						auto c_missionTasks = CacheMissionTasks::getRow(it->missionID).flatIt();
						if (it->progress != "") {
							auto taskprogress = StringUtils::splitString(it->progress, '|');
							for (int i = 0; i < taskprogress.size(); ++i) {
								// Collectible tasks are handled differently.
								if (CacheMissionTasks::GetTaskType(*std::next(c_missionTasks.begin(), i)) == 3) {
									auto subtaskprogress = StringUtils::splitString(taskprogress.at(i), ':');
									ss << "<sv v=\"" << subtaskprogress.size() << "\"/>";
									for (int j = 0; j < subtaskprogress.size(); ++j) {
										ss << "<sv v=\"" << subtaskprogress.at(j) << "\"/>";
									}
								}
								else {
									ss << "<sv v=\"" << taskprogress.at(i) << "\"/>";
								}
							}
						}

						ss << "</m>";
					}
				}
				ss << "</cur>";
				ss << "<ts></ts>";
			}
			ss << "</mis>";
		}
		{
			ss << "<mnt a=\"0\"/>";
		}
		{
			ss << "<dest hm=\"9\" hc=\"9\" im=\"23\" ic=\"33\" am=\"0\" ac=\"14\" rsh=\"4\" rsi=\"6\" d=\"0\" imm=\"0\"/>";
			
				// TODO
		}
	}
	ss << "</obj>";
	return ss.str();
}
