#ifndef __ENTITY__GAMEOBJECT_HPP__
#define __ENTITY__GAMEOBJECT_HPP__

#include "RakNet/Replica.h"
#include "RakNet/MessageIdentifiers.h"
#include "Replica/ReplicaTypes.hpp"
#include "RakNet/BitStream.h"
#include "DataTypes/Position.hpp"
#include "DataTypes/Quaternion.hpp"
#include "DataTypes/LWOOBJID.hpp"
#include <vector>
#include <unordered_map>
#include <optional>

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Enums/EGameActivity.hpp"

#include "Entity/GMUtils.hpp"

#define GAMEOBJECT_GM_PREDECLARE(name) class name

namespace GM {
	GM_MAKE_LIST_CLIENT(GAMEOBJECT_GM_PREDECLARE);
}

//#include "Server/WorldServer.hpp"
class WorldServer;

namespace Entity {


	/*
		Classes to be later defined.
	*/
	class LWOTimer {};

	/*
		Defines a Game Object in-game.
	*/
	class GameObject : public Replica {
		private:

			// This Object's Object ID.
			DataTypes::LWOOBJID objectID;

			// The LOT of this Object. (List in cdclient)
			int32_t LOT;

			// The Name of this Object.
			std::u16string name = u"";

			// The Timestamp this object was created on the server.
			int64_t creationTimestamp;

			int64_t maxAge = 0LL;

			// The Spawner GameObject.
			GameObject * spawner = nullptr;

			// The ID of a spawner node.
			uint32_t spawner_node = 0xFFFFFFFF;

			// The Parent Object of this Object.
			GameObject * parent = nullptr;

			// The scale of the object
			float scale = 1.0f;

			// The Children of this object.
			std::list<GameObject*> children = {};

			// Groups the object is in.
			std::list<std::u16string> groups = {};

			// The Components assigned to this object.
			std::unordered_map<std::uint32_t, IEntityComponent*> components;

			// The timers assigned to this object.
			std::unordered_map<char*, LWOTimer> timers;
			
			// Needs serialization in general.
			bool objectDirty = false;

			// Needs serialization
			bool baseDataDirty = false;

			// Don't construct to client if set
			bool serverOnly = false;

			WorldServer * Instance;

			LDFCollection configData;

			LDFCollection customConfig;
		public:

			/*
				Replica Manager Overrided functions.
			*/
			ReplicaReturnResult SendConstruction(RakNetTime currentTime, SystemAddress systemAddress, unsigned int &flags, RakNet::BitStream *outBitStream, bool *includeTimestamp);
			ReplicaReturnResult SendDestruction(RakNet::BitStream *outBitStream, SystemAddress systemAddress, bool *includeTimestamp);
			ReplicaReturnResult ReceiveDestruction(RakNet::BitStream *inBitStream, SystemAddress systemAddress, RakNetTime timestamp);
			ReplicaReturnResult SendScopeChange(bool inScope, RakNet::BitStream *outBitStream, RakNetTime currentTime, SystemAddress systemAddress, bool *includeTimestamp);
			ReplicaReturnResult ReceiveScopeChange(RakNet::BitStream *inBitStream, SystemAddress systemAddress, RakNetTime timestamp);
			ReplicaReturnResult Serialize(bool *sendTimestamp, RakNet::BitStream *outBitStream, RakNetTime lastSendTime, PacketPriority *priority, PacketReliability *reliability, RakNetTime currentTime, SystemAddress systemAddress, unsigned int &flags);
			ReplicaReturnResult Deserialize(RakNet::BitStream *inBitStream, RakNetTime timestamp, RakNetTime lastDeserializeTime, SystemAddress systemAddress);
			int GetSortPriority(void) const { return 0; }

			// Is component serializable
			bool isSerializable = true;
			
		public:
			/*
				Constructor
			*/
			GameObject(WorldServer * instance, std::uint32_t LOT);

			/*
				Use this to finish up object creation.
			*/
			void Finish();

			/*
				Destructor
			*/
			~GameObject();

			/*
				Sets name
			*/
			void SetName(std::u16string n) {
				name = n;
			}

			/*
				Gets name
			*/
			std::u16string GetName() {
				return name;
			}

			/*
				Gets name as string
			*/
			std::string GetNameStr() {
				return std::string(name.begin(), name.end());
			}

			/*
				Sets the object ID
			*/
			void SetObjectID(DataTypes::LWOOBJID ID);

			/*
				Gets the object ID
			*/
			DataTypes::LWOOBJID GetObjectID();

			/*
				Sets the scale
			*/
			void SetScale(float scale) {
				this->scale = scale;
			}

			/*
				Gets the scale
			*/
			float GetScale() {
				return this->scale;
			}

			/*
				Gets the object LOT
			*/
			std::uint32_t GetLOT() {
				return LOT;
			}

			/*
				Sets if object is server only
			*/
			void SetIsServerOnly() {
				serverOnly = true;
			}

			/*
				Gets if object is server only
			*/
			bool GetIsServerOnly() {
				return serverOnly;
			}


			/*
				Called to update the object.
			*/
			void Update();

			/*
				Called to update physic
			*/
			void PhysicUpdate();

			/*
				Ticks the object.
			*/
			void Tick();

			/*
				Returns an component by Type
			*/
			IEntityComponent* GetComponentByType(int id);

			/*
				Returns an component by class
			*/
			template<class T = IEntityComponent>
			inline T * GetComponent();

			/*
				Adds an component by ID
			*/
			IEntityComponent* AddComponentByID(int id, int compID);


			/*
				Adds an component by class
			*/
			template<class T = IEntityComponent>
			inline T* AddComponent(std::int32_t componentID);

			/*
				Serializes the Object.
			*/
			void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType);

			/*
				Deserializes the object
			*/
			void DoDeserialize(RakNet::BitStream* inBitStream, RakNetTime timestamp, RakNetTime lastDeserializeTime, SystemAddress systemAddress);

			/*
				Serializes the components of the object.
			*/
			void SerializeComponents(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType);

			/*
				Serializes the base data of the object.
			*/
			void SerializeBaseData(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType);

			/*
				Adds a child to the gameobject and tell the child it's added.
			*/
			void AddChild(GameObject * child);

			/*
				Removes a child
			*/
			void RemoveChild(Entity::GameObject* child);

			/*
				Sets the parent object
			*/
			void SetParent(GameObject * parent);

			/*
				Sets the spawner object
			*/
			void SetSpawner(GameObject * spawner,std::uint32_t spawnerNodeID);

			/*
				Returns if object is within group
			*/
			bool IsWithinGroup(std::u16string groupName);

            /*
				Use this one to remove an object.
				Everything else will be taken care of.
            */
            void InstantiateRemoval();

			/*
				INTERNAL USE ONLY
				DO NOT CALL THIS ONE!!!!!
				Completely remove object
			*/
			void Remove();

			/*
				Only use this on a player.
			*/
			std::string GenerateXML();

			/*
				Gets flag if player
			*/
			bool GetFlag(int iFlagID);

			/*
				Sets the object dirty.
			*/
			void SetDirty() {
				objectDirty = true;
			}

			/*
				Checks if object dirty
			*/
			bool IsObjectDirty() {
				return objectDirty;
			}

			/*
				Populates object variables from LDF
			*/
			void PopulateFromLDF(LDFCollection * collection);

			/*
				Set's the position on the physic component
			*/
			void SetPosition(DataTypes::Vector3 position);

			/*
				Set's the rotation of the physic component
			*/
			void SetRotation(DataTypes::Quaternion rotation);


			/*
				Get's the position on the physic component
			*/
			DataTypes::Vector3 GetPosition();

			/*
				Get's the rotation of the physic component
			*/
			DataTypes::Quaternion GetRotation();

			/*
				Returns the rigid body, used for physics
			*/
			btRigidBody* GetRigidBody();

			/*
				Returns the instance
			*/
			WorldServer * GetZoneInstance() {
				return Instance;
			}

			/*
				Sets the imagination
			*/
			void SetImagination(std::int32_t imag);

			/*
				Gets the imagination
			*/
			std::int32_t GetImagination();

			void SetMaxAge(std::int64_t age) {
				maxAge = std::int64_t(::time(0)) + std::int64_t(age);
			}

			/*
				Quick function to create the test object.
			*/
			void Test() { objectID = 9ULL; LOT = 1234; name = u"TestName"; }

			void Possess(Entity::GameObject* other);


			void OnCollisionPhantom(Entity::GameObject * other);
			void OnOffCollisionPhantom(Entity::GameObject * other);

            void SetPlayerActivity(Enums::EGameActivity activity);
		public:
			/* Game Messages */
			//void SendGM(Entity::GameObject * sender, GM::GMBase msg) { GameMessages::Send(Instance, UNASSIGNED_SYSTEM_ADDRESS, objectID, msg); }


			GM_MAKE_LIST_CLIENT(GM_MAKE_GAMEOBJECT_DECLARE);

			virtual void OnDie(Entity::GameObject* sender, GM::Die* msg);
			/*virtual void OnHasBeenCollected(Entity::GameObject* sender, GM::HasBeenCollected* msg);
			virtual void OnMissionDialogueOK(Entity::GameObject* sender, GM::MissionDialogueOK* msg);
			virtual void OnRequestDie(Entity::GameObject* sender, GM::RequestDie* msg);
			virtual void OnRequestUse(Entity::GameObject * sender, GM::RequestUse * msg);
			virtual void OnSetFlag(Entity::GameObject* sender, GM::SetFlag* msg);
			virtual void OnStartSkill(const GM::StartSkill msg);
			virtual void OnSyncSkill(const GM::SyncSkill msg);*/



			void PickupLoot(Entity::GameObject* loot);
		public:
			// Script Stuff
			LDFEntry GetVarEntry(std::u16string key) {
				if (configData.find(key) != configData.end())
					return configData.at(key);
				else
					return LDFEntry();
			}

			template<typename T=LDFEntry>
			T GetVar(std::u16string key) {
				return static_cast<T>(static_cast<LDFEntry>(GetVarEntry(key)));
			}

			template<typename T>
			void SetVar(std::u16string key, T data) {
				auto it = configData.find(key);
				if (it != configData.end()) {
					it->second = LDFEntry(key, data);
				}
				else {
					configData.insert({ key, LDFEntry(key, data) });
				}
			}

			void SetProximityRadius(std::string name, float radius);
			void PlayNDAudioEmitter(std::string guid);
	};
}

#endif // !__ENTITY__GAMEOBJECT_HPP__