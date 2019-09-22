#include "GameObject.hpp"

#include "Utils/ServerInfo.hpp"
#include "Server/WorldServer.hpp"

//using namespace Entity::Components::Interface;
#define SERIALIZE_COMPONENT_IF_ATTACHED(COMP_T) {COMP_T * comp = this->GetComponent<COMP_T>(); if(comp != nullptr) { /*Logger::log("WRLD", "Serializing "+std::string(#COMP_T)+"...");*/ comp->Serialize(factory, packetType);}}
#define COMPONENT_ONADD_SWITCH_CASE(COMP_T) {\
	case COMP_T::GetTypeID(): {\
		COMP_T * comp = new COMP_T();\
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
#include "Entity/Components/CharacterComponent.hpp"
#include "Entity/Components/CollectibleComponent.hpp"
#include "Entity/Components/Component107.hpp"
#include "Entity/Components/ControllablePhysicsComponent.hpp"
#include "Entity/Components/DestructibleComponent.hpp"
#include "Entity/Components/InventoryComponent.hpp"
#include "Entity/Components/ItemComponent.hpp"
#include "Entity/Components/LUPExhibitComponent.hpp"
#include "Entity/Components/MinifigComponent.hpp"
#include "Entity/Components/MissionOfferComponent.hpp"
#include "Entity/Components/MovementAIComponent.hpp"
#include "Entity/Components/MovingPlatformComponent.hpp"
#include "Entity/Components/PhantomPhysicsComponent.hpp"
#include "Entity/Components/QuickBuildComponent.hpp"
#include "Entity/Components/RenderComponent.hpp"
#include "Entity/Components/ScriptComponent.hpp"
#include "Entity/Components/SimplePhysicsComponent.hpp"
#include "Entity/Components/SkillComponent.hpp"
#include "Entity/Components/SpawnerComponent.hpp"
#include "Entity/Components/StatsComponent.hpp"
#include "Entity/Components/SwitchComponent.hpp"
#include "Entity/Components/TriggerComponent.hpp"
#include "Entity/Components/VendorComponent.hpp"

ReplicaReturnResult Entity::GameObject::SendConstruction(RakNetTime currentTime, SystemAddress systemAddress, unsigned int &flags, RakNet::BitStream *outBitStream, bool *includeTimestamp) {
	this->Serialize(outBitStream, ReplicaTypes::PacketTypes::CONSTRUCTION);
	Instance->replicaManager->SetScope(this, true, UNASSIGNED_SYSTEM_ADDRESS, true);
	return REPLICA_PROCESSING_DONE;
}
ReplicaReturnResult Entity::GameObject::SendDestruction(RakNet::BitStream *outBitStream, SystemAddress systemAddress, bool *includeTimestamp) {
	return REPLICA_PROCESSING_DONE;
	this->Serialize(outBitStream, ReplicaTypes::PacketTypes::DESTRUCTION);
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

	if (LOT == 0) {
		Logger::log("WRLD", "Failed to spawn object: LOT 0 is blacklisted!", LogType::ERR);
		return;
	}

	auto component_types_to_be_added = CacheComponentsRegistry::GetObjectComponentTypes(LOT);

	this->creationTimestamp = ServerInfo::uptime();

	// Add components
	for (std::uint32_t component_type : component_types_to_be_added) {
		this->AddComponentByID(component_type);
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

IEntityComponent * Entity::GameObject::AddComponentByID(int id) {
	switch (id) {
		/* ========== SERIALIZED ========== */
		COMPONENT_ONADD_SWITCH_CASE(StatsComponent);
		//COMPONENT_ONADD_SWITCH_CASE(Component108);
		//COMPONENT_ONADD_SWITCH_CASE(ModuleAssemblyComponent);
		COMPONENT_ONADD_SWITCH_CASE(ControllablePhysicsComponent);
		COMPONENT_ONADD_SWITCH_CASE(SimplePhysicsComponent);
		//COMPONENT_ONADD_SWITCH_CASE(RigidBodyPhantomPhysicsComponent);
		//COMPONENT_ONADD_SWITCH_CASE(VehiclePhysicsComponent);
		COMPONENT_ONADD_SWITCH_CASE(PhantomPhysicsComponent);
		COMPONENT_ONADD_SWITCH_CASE(DestructibleComponent);
		COMPONENT_ONADD_SWITCH_CASE(CollectibleComponent);
		//COMPONENT_ONADD_SWITCH_CASE(PetComponent);
		COMPONENT_ONADD_SWITCH_CASE(CharacterComponent);
		//COMPONENT_ONADD_SWITCH_CASE(ShootingGalleryComponent);
		COMPONENT_ONADD_SWITCH_CASE(InventoryComponent);
		COMPONENT_ONADD_SWITCH_CASE(ScriptComponent);
		COMPONENT_ONADD_SWITCH_CASE(SkillComponent);
		COMPONENT_ONADD_SWITCH_CASE(BaseCombatAIComponent);
		COMPONENT_ONADD_SWITCH_CASE(QuickbuildComponent);
		COMPONENT_ONADD_SWITCH_CASE(MovingPlatformComponent);
		COMPONENT_ONADD_SWITCH_CASE(SwitchComponent);
		COMPONENT_ONADD_SWITCH_CASE(VendorComponent);
		COMPONENT_ONADD_SWITCH_CASE(BouncerComponent);
		//COMPONENT_ONADD_SWITCH_CASE(ScriptedActivityComponent);
		//COMPONENT_ONADD_SWITCH_CASE(RacingControlComponent);
		COMPONENT_ONADD_SWITCH_CASE(LUPExhibitComponent);
		//COMPONENT_ONADD_SWITCH_CASE(ModelComponent);
		COMPONENT_ONADD_SWITCH_CASE(RenderComponent);
		//COMPONENT_ONADD_SWITCH_CASE(MinigameComponent);
		  COMPONENT_ONADD_SWITCH_CASE(Component107);
		COMPONENT_ONADD_SWITCH_CASE(TriggerComponent);
		/* ========== NON-SERIALIZED ========== */
		COMPONENT_ONADD_SWITCH_CASE(MovementAIComponent);
		COMPONENT_ONADD_SWITCH_CASE(SpawnerComponent);
		COMPONENT_ONADD_SWITCH_CASE(MinifigComponent);
		COMPONENT_ONADD_SWITCH_CASE(MissionOfferComponent);

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
T * Entity::GameObject::AddComponent() {
	return static_cast<T*>(this->AddComponentByID(T::GetTypeID()));
}

void Entity::GameObject::Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
	SerializeBaseData(factory, packetType);
	SerializeComponents(factory, packetType);
	objectDirty = false;
}

void Entity::GameObject::SerializeComponents(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
	//SERIALIZE_COMPONENT_IF_ATTACHED(Component108);
	//SERIALIZE_COMPONENT_IF_ATTACHED(ModuleAssemblyComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(ControllablePhysicsComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(SimplePhysicsComponent);
	//SERIALIZE_COMPONENT_IF_ATTACHED(RigidBodyPhantomPhysicsComponent);
	//SERIALIZE_COMPONENT_IF_ATTACHED(VehiclePhysicsComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(PhantomPhysicsComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(DestructibleComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(CollectibleComponent);
	//SERIALIZE_COMPONENT_IF_ATTACHED(PetComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(CharacterComponent);
	//SERIALIZE_COMPONENT_IF_ATTACHED(ShootingGalleryComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(InventoryComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(ScriptComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(SkillComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(BaseCombatAIComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(QuickbuildComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(MovingPlatformComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(SwitchComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(VendorComponent,);
	SERIALIZE_COMPONENT_IF_ATTACHED(BouncerComponent);
	//SERIALIZE_COMPONENT_IF_ATTACHED(ScriptedActivityComponent);
	//SERIALIZE_COMPONENT_IF_ATTACHED(RacingControlComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(LUPExhibitComponent);
	//SERIALIZE_COMPONENT_IF_ATTACHED(ModelComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(RenderComponent);
	//SERIALIZE_COMPONENT_IF_ATTACHED(MinigameComponent);
	SERIALIZE_COMPONENT_IF_ATTACHED(Component107);
	SERIALIZE_COMPONENT_IF_ATTACHED(TriggerComponent);
}

void Entity::GameObject::SerializeBaseData(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
	if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
		factory->Write<std::uint64_t>(objectID);
		factory->Write<std::int32_t>(LOT);
		StringUtils::writeWStringToBitStream<std::uint8_t>(factory, name);

		factory->Write<uint32_t>(ServerInfo::uptime() - creationTimestamp);

		{
			factory->Write(false);
			// TODO! -> compressed LDF
		}

		factory->Write(this->GetComponent<TriggerComponent>() != nullptr);

		factory->Write(spawner != nullptr);
		if (spawner != nullptr) { factory->Write(spawner->objectID); }

		factory->Write(spawner_node != 0xFFFFFFFF);
		if (spawner_node != 0xFFFFFFFF) { factory->Write(spawner_node); }

		// Object Scale
		factory->Write(this->scale != 1.0);
		if (this->scale != 1.0)
			factory->Write(this->scale);
		
		// object world state
		factory->Write(false);

		// gmlevel
		factory->Write(false);
	}

	factory->Write(baseDataDirty);
	if (baseDataDirty) {
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

void Entity::GameObject::PopulateFromLDF(LDFCollection * collection) {
	
	configData = *collection;

	// TODO: Populate base data

	if (this->LOT != 176) {
		// Add Componets custom
		/* Script Component */ {
			std::wstring customScript = L"";
			LDF_GET_VAL_FROM_COLLECTION(customScript, collection, L"custom_script_server", L"");
			if (customScript != L"") {
				this->AddComponent<ScriptComponent>();
			}
		}

		/* Trigger Component */
		std::wstring wstrTriggerID = L"";
		LDF_GET_VAL_FROM_COLLECTION(wstrTriggerID, collection, L"trigger_id", L"NULL");
		if (wstrTriggerID != L"NULL") {

			// Seperate data
			std::vector<std::wstring> spltTriggerID = StringUtils::splitWString(wstrTriggerID, L':');
			int triggerSceneID = std::stoi(spltTriggerID.at(0));
			int triggerID = std::stoi(spltTriggerID.at(1));

			// Get zone file
			FileTypes::LUZ::LUZone * zone = Instance->luZone;

			// Get trigger file
			if (zone->triggers.find(triggerSceneID) != zone->triggers.end()) {
				ZoneTriggerFile triggerFile = zone->triggers.at(triggerSceneID);

				// Get trigger
				for (int i = 0; i < triggerFile.triggers.size(); ++i) {
					if (triggerFile.triggers.at(i).id == triggerID) {
						this->AddComponent<TriggerComponent>();
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
		return;
	}
	auto simplePhysicsComponent = this->GetComponent<SimplePhysicsComponent>();
	if (simplePhysicsComponent != nullptr) {
		simplePhysicsComponent->SetPosition(position);
		return;
	}/*
	auto rigidBodyPhantomPhysicsComponent = static_cast<RigidBodyPhantomPhysicsComponent*>(this->GetComponentByID(20));
	if (rigidBodyPhantomPhysicsComponent != nullptr) {
		rigidBodyPhantomPhysicsComponent->SetPosition(position);
		return;
	}
	auto vehiclePhysicsComponent = static_cast<VehiclePhysicsComponent*>(this->GetComponentByID(30));
	if (vehiclePhysicsComponent != nullptr) {
		vehiclePhysicsComponent->SetPosition(position);
		return;
	}*/
	auto phantomPhysicsComponent = this->GetComponent<PhantomPhysicsComponent>();
	if (phantomPhysicsComponent != nullptr) {
		phantomPhysicsComponent->SetPosition(position);
		return;
	}

	return;
}

void Entity::GameObject::SetRotation(DataTypes::Quaternion rotation) {
	auto controllablePhysicsComponent = this->GetComponent<ControllablePhysicsComponent>();
	if (controllablePhysicsComponent != nullptr) {
		controllablePhysicsComponent->SetRotation(rotation);
		return;
	}
	auto simplePhysicsComponent = this->GetComponent<SimplePhysicsComponent>();
	if (simplePhysicsComponent != nullptr) {
		simplePhysicsComponent->SetRotation(rotation);
		return;
	}/*
	auto rigidBodyPhantomPhysicsComponent = static_cast<RigidBodyPhantomPhysicsComponent*>(this->GetComponentByID(20));
	if (rigidBodyPhantomPhysicsComponent != nullptr) {
		rigidBodyPhantomPhysicsComponent->SetRotation(rotation);
		return;
	}
	auto vehiclePhysicsComponent = static_cast<VehiclePhysicsComponent*>(this->GetComponentByID(30));
	if (vehiclePhysicsComponent != nullptr) {
		vehiclePhysicsComponent->SetRotation(rotation);
		return;
	}*/
	auto phantomPhysicsComponent = this->GetComponent<PhantomPhysicsComponent>();
	if (phantomPhysicsComponent != nullptr) {
		phantomPhysicsComponent->SetRotation(rotation);
		return;
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
	}
	auto vehiclePhysicsComponent = static_cast<VehiclePhysicsComponent*>(this->GetComponentByID(30));
	if (vehiclePhysicsComponent != nullptr) {
		return vehiclePhysicsComponent->GetPosition();
	}*/
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
	}
	auto vehiclePhysicsComponent = static_cast<VehiclePhysicsComponent*>(this->GetComponentByID(30));
	if (vehiclePhysicsComponent != nullptr) {
		return vehiclePhysicsComponent->GetRotation();
	}*/
	auto phantomPhysicsComponent = this->GetComponent<PhantomPhysicsComponent>();
	if (phantomPhysicsComponent != nullptr) {
		return phantomPhysicsComponent->GetRotation();
	}
	return DataTypes::Quaternion();
}

void Entity::GameObject::OnRequestUse(Entity::GameObject * sender, GM::RequestUse * msg) {
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

	ss << "<obj v=\"" << std::to_string(LOT) << ">";
	{
		ss << "<buff/>";
		ss << "<skil/>";
		ss << "<inv csl=\"" << std::to_string(0) << "\">";
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
				ss << "ms=\"" << std::to_string(charStyle.mouthStyle) << "\"";
			ss << "/>";
		}
		{
			ss << "<lvl l=\"" << std::to_string(charInfo.uLevel) << "\" cv=\"1\" sb=\"525\"/>";
		}
		{
			ss << "<flag>";
			{
				// TODO
			}
			ss << "</flag>";
		}
		{
			ss << "<mis>";
			{
				// TODO
			}
			ss << "</mis>";
		}
		{
			ss << "<mnt a=\"0\"/>";
		}
		{
			ss << "<dest ";
			
				// TODO
			ss << "/>";
		}
	}
	ss << "</obj>";

	return ss.str();
}
