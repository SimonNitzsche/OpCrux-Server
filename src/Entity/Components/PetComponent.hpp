#ifndef __REPLICA__COMPONENTS__Pet_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__Pet_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "Enums/ENotifyType.hpp"
#include "Enums/EStunState.hpp"

#include "FileTypes/LXFMLFile/LXFML.hpp"
#include "GameCache/Interface/FastDatabase.hpp"

using namespace DataTypes;

class PetComponent : public IEntityComponent {
private:

	bool IsPetWild;

	bool _isDirty = true;

	std::uint32_t unknown1 = 0x4000002;
	std::uint32_t unknown2 = 0x00;

	DataTypes::LWOOBJID interactionObject = this->ownerID;
	DataTypes::LWOOBJID ownerID = 0ULL;

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
		_isDirty = false;
	}

	void Update() {

	}

	void PopulateFromLDF(LDFCollection* collection) {
		

	}

	void Awake() {
		
	}

	void OnCommandPet(Entity::GameObject* sender, GM::CommandPet* msg) {
		Logger::log("WRLD", "Recieved command pet: " + std::to_string(msg->iPetCommandType));
		switch (msg->iPetCommandType) {
		case 4: { // start pet taming minigame

			break;
		}
		}
	}

	void OnRequestUse(Entity::GameObject* sender, GM::RequestUse* msg) {
		{
			GM::NotifyPetTamingMinigame nmsg;
			nmsg.bForceTeleport = false;
			nmsg.PlayerTamingID = sender->GetObjectID();
			nmsg.PetID = msg->objectID;
			nmsg.notifyType = (uint32_t)Enums::ENotifyType::BEGIN;
			auto petPos = owner->GetPosition();
			nmsg.petsDestPos = petPos;
			// This is stolen code from a earlier version of OpCrux
			auto pos = sender->GetPosition();
			float deg = atan2(petPos.z - pos.z, petPos.x - pos.x) * 180 / M_PI;
			pos = Vector3(
				petPos.x + (owner->GetVar<float>(u"interaction_distance") * cos(-deg)),
				petPos.y,
				petPos.z + (owner->GetVar<float>(u"interaction_distance") * sin(-deg))
			);
			nmsg.telePort = pos;
			nmsg.teleRot = Quaternion::betweenPoints(pos, petPos);
			GameMessages::Broadcast(sender, nmsg);
		}
		{
			GM::SetStunned nmsg;
 			nmsg.StunState = Enums::EStunState::Push;
			
			nmsg.bCantAttack = true;
			nmsg.bCantAttackOutChangeWasApplied = false;
			nmsg.bCantEquip = true;
			nmsg.bCantEquipOutChangeWasApplied = false;
			nmsg.bCantInteract = false;
			nmsg.bCantInteractOutChangeWasApplied = false;
			nmsg.bCantJump = false;
			nmsg.bCantJumpOutChangeWasApplied = false;
			nmsg.bCantMove = false;
			nmsg.bCantMoveOutChangeWasApplied = false;
			nmsg.bCantTurn = false;
			nmsg.bCantTurnOutChangeWasApplied = false;
			nmsg.bCantUseItem = true;
			nmsg.bCantUseItemOutChangeWasApplied = false;
			nmsg.bDontTerminateInteract = false;
			nmsg.bIgnoreImmunity = true;
			GameMessages::Send(sender, sender->GetObjectID(), nmsg);
		}
		{
			std::string LXFMLName;

			auto test = owner->GetLOT();

			extern FDB::Connection Cache;
			FDB::RowTopHeader rth = Cache.getRows("TamingBuildPuzzles");
			for (int i = 0; i < rth.getRowCount(); ++i) {
				if (!rth.isValid(i)) continue;
				try {
					if (*reinterpret_cast<int32_t*>(rth[i][3].getMemoryLocation()) == owner->GetLOT()) {
						LXFMLName = FDB::PointerString(&Cache, rth[i][2].getMemoryLocation()).operator std::string();
						break;
					}
				}
				catch (std::runtime_error e) {
					Logger::log("Cache:TamingBuildPuzzles", e.what(), LogType::ERR);
				}
			}

			LXFML* lxfml = new LXFML("res/BrickModels/pettaming/cathousewscratchingpost.lxfml");

			if (lxfml->Loaded) {
				RakNet::BitStream* bs = new RakNet::BitStream();
				LUPacketHeader returnBSHead;
				returnBSHead.protocolID = static_cast<uint8_t>(ID_USER_PACKET_ENUM);
				returnBSHead.remoteType = static_cast<uint16_t>(Enums::ERemoteConnection::CLIENT);
				returnBSHead.packetID = static_cast<uint32_t>(Enums::EClientPacketID::SERVER_GAME_MSG);
				bs->Write(returnBSHead);

				bs->Write<int64_t>(sender->GetObjectID());
				bs->Write<uint16_t>(DUAL_NOTIFY_PET_TAMING_PUZZLE_SELECTED);

				bs->Write<uint32_t>(lxfml->Bricks.size());
				for (auto item : lxfml->Bricks) {
					bs->Write<uint32_t>(item.DesignID);
					bs->Write<uint32_t>(item.DesignPart.Material);
				}
			}
			else {
				Logger::log("WRLD", "Failed to load LXFML for pet minigame: " + LXFMLName);
			}
		}


	}

	void RegisterMessageHandlers() {
		REGISTER_OBJECT_MESSAGE_HANDLER(PetComponent, GM::CommandPet, OnCommandPet);
		REGISTER_OBJECT_MESSAGE_HANDLER(PetComponent, GM::RequestUse, OnRequestUse);
	}
};

#endif