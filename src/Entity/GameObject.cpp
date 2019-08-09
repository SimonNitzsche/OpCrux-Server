#include "GameObject.hpp"

#include "Utils/ServerInfo.hpp"

//using namespace Entity::Components::Interface;
#define SERIALIZE_COMPONENT_IF_ATTACHED(COMP_T, COMP_ID) {COMP_T * comp = static_cast<COMP_T*>(this->GetComponentByID(COMP_ID)); if(comp != nullptr) { Logger::log("WRLD", "Serializing "+std::string(#COMP_T)+"..."); comp->Serialize(factory, packetType);}}
#define COMPONENT_ONADD_SWITCH_CASE(COMP_T, COMP_ID) {case COMP_ID: {COMP_T * comp = new COMP_T(); comp->SetOwner(this); components.insert(std::make_pair(COMP_ID, comp)); comp->OnEnable(); Logger::log("WRLD", "Added Component "+std::string(#COMP_T)+"!"); if(comp == nullptr) throw new std::runtime_error(std::string(#COMP_T)+" resultet into a nullptr.");  break;}}

void lala(IEntityComponent * c, int id) {
	if (id == 1000) {
		int c = 3323;
	}
}

#include "GameCache/ComponentsRegistry.hpp"

#include "Utils/StringUtils.hpp"

// Components
#include "Entity/Components/CharacterComponent.hpp"
#include "Entity/Components/Component107.hpp"
#include "Entity/Components/ControllablePhysicsComponent.hpp"
#include "Entity/Components/DestructibleComponent.hpp"
#include "Entity/Components/InventoryComponent.hpp"
#include "Entity/Components/RenderComponent.hpp"
#include "Entity/Components/SkillComponent.hpp"
#include "Entity/Components/StatsComponent.hpp"

ReplicaReturnResult Entity::GameObject::SendConstruction(RakNetTime currentTime, SystemAddress systemAddress, unsigned int &flags, RakNet::BitStream *outBitStream, bool *includeTimestamp) {
	this->Serialize(outBitStream, ReplicaTypes::PacketTypes::CONSTRUCTION);
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
	return REPLICA_PROCESSING_DONE;
}
ReplicaReturnResult Entity::GameObject::ReceiveScopeChange(RakNet::BitStream *inBitStream, SystemAddress systemAddress, RakNetTime timestamp) {
	return REPLICA_PROCESSING_DONE;
}
ReplicaReturnResult Entity::GameObject::Serialize(bool *sendTimestamp, RakNet::BitStream *outBitStream, RakNetTime lastSendTime, PacketPriority *priority, PacketReliability *reliability, RakNetTime currentTime, SystemAddress systemAddress, unsigned int &flags) {
	return REPLICA_PROCESSING_DONE;
	this->Serialize(outBitStream, ReplicaTypes::PacketTypes::SERIALIZATION);
}
ReplicaReturnResult Entity::GameObject::Deserialize(RakNet::BitStream *inBitStream, RakNetTime timestamp, RakNetTime lastDeserializeTime, SystemAddress systemAddress) {
	return REPLICA_PROCESSING_DONE;
}

Entity::GameObject::GameObject(std::uint32_t LOT) {
	auto d = CacheComponentsRegistry::GetObjectComponentTypes(LOT);
	for (std::uint32_t component_type : d) {
		this->AddComponentByID(component_type);
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

void Entity::GameObject::Update() {

}

void Entity::GameObject::Tick() {
	Update();
}

IEntityComponent* Entity::GameObject::GetComponentByID(int id) {
	return components[id];
}

void Entity::GameObject::AddComponentByID(int id) {
	switch (id) {
		//COMPONENT_ONADD_SWITCH_CASE(Component108, 108);
		//COMPONENT_ONADD_SWITCH_CASE(ModuleAssemblyComponent, 61);
		  COMPONENT_ONADD_SWITCH_CASE(ControllablePhysicsComponent, 1);
		//COMPONENT_ONADD_SWITCH_CASE(SimplePhysicsComponent, 3);
		//COMPONENT_ONADD_SWITCH_CASE(RigidBodyPhantomPhysicsComponent, 20);
		//COMPONENT_ONADD_SWITCH_CASE(VehiclePhysics, 30);
		//COMPONENT_ONADD_SWITCH_CASE(PhantomPhysics, 40);
		  COMPONENT_ONADD_SWITCH_CASE(DestructibleComponent, 7);
		//COMPONENT_ONADD_SWITCH_CASE(CollectibleComponent, 23);
		//COMPONENT_ONADD_SWITCH_CASE(PetComponent, 26);
		  COMPONENT_ONADD_SWITCH_CASE(CharacterComponent, 4);
		//COMPONENT_ONADD_SWITCH_CASE(ShootingGalleryComponent, 19);
		  COMPONENT_ONADD_SWITCH_CASE(InventoryComponent, 17);
		//COMPONENT_ONADD_SWITCH_CASE(ScriptComponent, 5);
		  COMPONENT_ONADD_SWITCH_CASE(SkillComponent, 9);
		//COMPONENT_ONADD_SWITCH_CASE(BaseCombatAIComponent, 60);
		//COMPONENT_ONADD_SWITCH_CASE(QuickbuildComponent, 48);
		//COMPONENT_ONADD_SWITCH_CASE(MovingPlatformComponent, 25);
		//COMPONENT_ONADD_SWITCH_CASE(SwitchComponent, 49);
		//COMPONENT_ONADD_SWITCH_CASE(VendorComponent, 16);
		//COMPONENT_ONADD_SWITCH_CASE(BouncerComponent, 6);
		//COMPONENT_ONADD_SWITCH_CASE(ScriptedActivityComponent, 39);
		//COMPONENT_ONADD_SWITCH_CASE(RacingControlComponent, 71);
		//COMPONENT_ONADD_SWITCH_CASE(LUPExhibitComponent, 75);
		//COMPONENT_ONADD_SWITCH_CASE(ModelComponent, 42);
		  COMPONENT_ONADD_SWITCH_CASE(RenderComponent, 2);
		//COMPONENT_ONADD_SWITCH_CASE(MinigameComponent, 50);
		  COMPONENT_ONADD_SWITCH_CASE(Component107, 107);
		//COMPONENT_ONADD_SWITCH_CASE(TriggerComponent, 69);

	// SPECIAL ONES
		  COMPONENT_ONADD_SWITCH_CASE(StatsComponent, 1000);

	default: {
		Logger::log("WRLD", "Couldn't add component #" + std::to_string(id) + " to GameObject!", LogType::UNEXPECTED);
	}
	}
}

void Entity::GameObject::Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
	SerializeBaseData(factory, packetType);
	SerializeComponents(factory, packetType);
}

void Entity::GameObject::SerializeComponents(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
	//SERIALIZE_COMPONENT_IF_ATTACHED(Component108, 108);
	//SERIALIZE_COMPONENT_IF_ATTACHED(ModuleAssemblyComponent, 61);
	SERIALIZE_COMPONENT_IF_ATTACHED(ControllablePhysicsComponent, 1);
	//SERIALIZE_COMPONENT_IF_ATTACHED(SimplePhysicsComponent, 3);
	//SERIALIZE_COMPONENT_IF_ATTACHED(RigidBodyPhantomPhysicsComponent, 20);
	//SERIALIZE_COMPONENT_IF_ATTACHED(VehiclePhysics, 30);
	//SERIALIZE_COMPONENT_IF_ATTACHED(PhantomPhysics, 40);
	SERIALIZE_COMPONENT_IF_ATTACHED(DestructibleComponent, 7);
	//SERIALIZE_COMPONENT_IF_ATTACHED(CollectibleComponent, 23);
	//SERIALIZE_COMPONENT_IF_ATTACHED(PetComponent, 26);
	SERIALIZE_COMPONENT_IF_ATTACHED(CharacterComponent, 4);
	//SERIALIZE_COMPONENT_IF_ATTACHED(ShootingGalleryComponent, 19);
	SERIALIZE_COMPONENT_IF_ATTACHED(InventoryComponent, 17);
	//SERIALIZE_COMPONENT_IF_ATTACHED(ScriptComponent, 5);
	SERIALIZE_COMPONENT_IF_ATTACHED(SkillComponent, 9);
	//SERIALIZE_COMPONENT_IF_ATTACHED(BaseCombatAIComponent, 60);
	//SERIALIZE_COMPONENT_IF_ATTACHED(QuickbuildComponent, 48);
	//SERIALIZE_COMPONENT_IF_ATTACHED(MovingPlatformComponent, 25);
	//SERIALIZE_COMPONENT_IF_ATTACHED(SwitchComponent, 49);
	//SERIALIZE_COMPONENT_IF_ATTACHED(VendorComponent, 16);
	//SERIALIZE_COMPONENT_IF_ATTACHED(BouncerComponent, 6);
	//SERIALIZE_COMPONENT_IF_ATTACHED(ScriptedActivityComponent, 39);
	//SERIALIZE_COMPONENT_IF_ATTACHED(RacingControlComponent, 71);
	//SERIALIZE_COMPONENT_IF_ATTACHED(LUPExhibitComponent, 75);
	//SERIALIZE_COMPONENT_IF_ATTACHED(ModelComponent, 42);
	SERIALIZE_COMPONENT_IF_ATTACHED(RenderComponent, 2);
	//SERIALIZE_COMPONENT_IF_ATTACHED(MinigameComponent, 50);
	SERIALIZE_COMPONENT_IF_ATTACHED(Component107, 107);
	//SERIALIZE_COMPONENT_IF_ATTACHED(TriggerComponent, 69);
}

void Entity::GameObject::SerializeBaseData(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
	if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
		factory->Write<std::uint64_t>(objectID);
		factory->Write<std::int32_t>(LOT);
		factory->Write<uint8_t>(name.size());
		StringUtils::writeWstringToBitStream(factory, name, name.size());

		factory->Write<uint32_t>(ServerInfo::uptime() - creationTimestamp);

		{
			factory->Write(false);
			// TODO! -> compressed LDF
		}

		factory->Write(false); // TODO! -> Trigger flag

		factory->Write(spawner != nullptr);
		if (spawner != nullptr) { factory->Write(spawner->objectID); }

		factory->Write(spawner_node != 0xFFFFFFFF);
		if (spawner_node != 0xFFFFFFFF) { factory->Write(spawner_node); }

		// Object Scale
		factory->Write(false);
		
		// object world state
		factory->Write(false);

		// gmlevel
		factory->Write(false);
	}

	// TODO: parent and child objects
	factory->Write(false);
}


std::string Entity::GameObject::GenerateXML() {
	std::stringstream ss;

	CharacterComponent * charComponent = static_cast<CharacterComponent*>(GetComponentByID(4));
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
