#ifndef __ENTITY__GameMessages_HPP__
#define __ENTITY__GameMessages_HPP__
#include "Sessions/ClientSession.hpp"
#include <RakNet/BitStream.h>
#include <RakNet/RakNetTypes.h>
#include "DataTypes/LWOOBJID.hpp"
#include "Enums/EGameMessages.hpp"
#include "Server/WorldServer.hpp"

#include "Enums/EDisconnectReason.hpp"
#include "Enums/ERemoteConnection.hpp"
#include "Enums/EPackets.hpp"

#include "Utils/StringUtils.hpp"

#include <string>

#include "Entity/GameObject.hpp"

#define GM_DESERIALIZE_SWITCH_CASE(name)\
	case name::GetID(): {name msg = name(); msg.Deserialize(bs); msg.TriggerEvent(senderObject, targetObject); break;}

#define GM_VAR_DESERIALIZE_STRING(bs, parameter)  {parameter = StringUtils::readStringFromBitStream<std::uint32_t>(bs);}
#define GM_VAR_DESERIALIZE_WSTRING(bs, parameter) {parameter = StringUtils::readWStringFromBitStream<std::uint32_t>(bs);}
#define GM_VAR_DESERIALIZE(bs, parameter) {bs->Read(parameter);}
#define GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, parameter, defaultVal) {\
	/*Check if bool, because those don't need extra bit*/\
	if(typeid(parameter) == typeid(bool)){\
		GM_VAR_DESERIALIZE(bs,parameter);\
	}\
	else {\
		bool notDefault; bs->Read(notDefault);\
		if(!notDefault) parameter = defaultVal;\
		else GM_VAR_DESERIALIZE(bs, parameter);\
	}\
}

#define GM_VAR_SERIALIZE_STRING(bs, parameter) {bs->Write<std::uint32_t>(parameter.size()); bs->Write(reinterpret_cast<const char*>(parameter.c_str()), parameter.size());}
#define GM_VAR_SERIALIZE_WSTRING(bs, parameter) {bs->Write<std::uint32_t>(parameter.size()); bs->Write(reinterpret_cast<const char*>(parameter.c_str()), parameter.size() * 2);}
#define GM_VAR_SERIALIZE(bs, parameter) {bs->Write(parameter);}
#define GM_VAR_SERIALIZE_WITH_DEFAULT(bs, parameter, defaultVal) {\
	/*Check if bool, because those don't need extra bit*/\
	if(typeid(parameter) == typeid(bool)){\
		GM_VAR_SERIALIZE(bs,parameter);\
	}\
	else {\
		bool notDefault = !(parameter == defaultVal); bs->Write(notDefault);\
		if(notDefault)\
			GM_VAR_SERIALIZE(bs, parameter);\
	}\
}

namespace GM {
	struct GMBase {
		GMBase() {}
		static constexpr Enums::EGameMessageID GetID() { return (Enums::EGameMessageID)(-1); };
		virtual void Deserialize(RakNet::BitStream* bs) {};
		virtual void Serialize(RakNet::BitStream* bs) {};
		virtual void TriggerEvent(Entity::GameObject* sender, Entity::GameObject* target) {};
	};
}

class GameMessages {
public:
	static void Deserialize(WorldServer * Instance, ClientSession * session, RakNet::BitStream *bs);

	template<typename T = GM::GMBase>
	static void Send(WorldServer * Instance, SystemAddress systemAddress, DataTypes::LWOOBJID target, T gm, DataTypes::LWOOBJID exclude = 0ULL) {
		RakNet::BitStream bs = RakNet::BitStream();

		LUPacketHeader returnBSHead;
		returnBSHead.protocolID = static_cast<uint8_t>(ID_USER_PACKET_ENUM);
		returnBSHead.remoteType = static_cast<uint16_t>(Enums::ERemoteConnection::CLIENT);
		returnBSHead.packetID = static_cast<uint32_t>(Enums::EClientPacketID::SERVER_GAME_MSG);
		bs.Write(returnBSHead);

		// GM Header
		bs.Write<std::uint64_t>(target);
		bs.Write(T::GetID());

		gm.Serialize(&bs);

		if (systemAddress == UNASSIGNED_SYSTEM_ADDRESS) {
			auto clients = Instance->sessionManager.GetClients();
			for (int i = 0; i < clients.size(); ++i) {
				ClientSession session = clients.at(i);

				if (exclude != session.actorID) {
#define quote(x) #x
					if (Instance->objectsManager->GetObjectByID(session.actorID) == nullptr) continue;
					Logger::log(typeid(T).name(), "Broadcasting to " + Instance->objectsManager->GetObjectByID(session.actorID)->GetNameStr());
					Instance->rakServer->Send(&bs, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, session.systemAddress, false);
				}
			}
		}
		else
			Instance->rakServer->Send(&bs, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, systemAddress, false);
	}

	template<typename T = GM::GMBase>
	static void Send(Entity::GameObject * receiver, DataTypes::LWOOBJID sender, T gm, DataTypes::LWOOBJID exclude = 0ULL) {
		Send(receiver->GetZoneInstance(), receiver->GetZoneInstance()->sessionManager.GetSession(receiver->GetObjectID())->systemAddress, sender, gm);
	}

	

	template<typename T = GM::GMBase>
	static inline void Broadcast(WorldServer * Instance, DataTypes::LWOOBJID target, T gm) {
		Send(Instance, UNASSIGNED_SYSTEM_ADDRESS, target, gm);
	}

	template<typename T = GM::GMBase>
	static inline void Broadcast(WorldServer * Instance, Entity::GameObject * target, T gm) {
		Send(Instance, UNASSIGNED_SYSTEM_ADDRESS, target->GetObjectID(), gm);
	}

	template<typename T = GM::GMBase>
	static inline void Broadcast(Entity::GameObject* target, T gm, bool excludeSelf = false) {
		Send(target->GetZoneInstance(), UNASSIGNED_SYSTEM_ADDRESS, target->GetObjectID(), gm, excludeSelf?target->GetObjectID():0Ui64);
	}
};

// The Game Messages:

// Define by include
#include "Entity/GameMessages/ActivityStart.hpp"
#include "Entity/GameMessages/ArrivedAtDesiredWaypoint.hpp"
#include "Entity/GameMessages/Die.hpp"
#include "Entity/GameMessages/EchoStartSkill.hpp"
#include "Entity/GameMessages/EchoSyncSkill.hpp"
#include "Entity/GameMessages/EmotePlayed.hpp"
#include "Entity/GameMessages/EnableRebuild.hpp"
#include "Entity/GameMessages/FireEventServerSide.hpp"
#include "Entity/GameMessages/HasBeenCollected.hpp"
#include "Entity/GameMessages/Knockback.hpp"
#include "Entity/GameMessages/MissionDialogueOK.hpp"
#include "Entity/GameMessages/ModuleAssemblyDBDataForClient.hpp"
#include "Entity/GameMessages/ModuleAssemblyQueryData.hpp"
#include "Entity/GameMessages/NotifyClientFlagChange.hpp"
#include "Entity/GameMessages/NotifyMission.hpp"
#include "Entity/GameMessages/NotifyMissionTask.hpp"
#include "Entity/GameMessages/NotifyVehicleOfRacingObject.hpp"
#include "Entity/GameMessages/OfferMission.hpp"
#include "Entity/GameMessages/PlayAnimation.hpp"
#include "Entity/GameMessages/PlayEmbeddedEffectOnAllClientsNearObject.hpp"
#include "Entity/GameMessages/PlayEmote.hpp"
#include "Entity/GameMessages/PlayerLoaded.hpp"
#include "Entity/GameMessages/PlayFXEffect.hpp"
#include "Entity/GameMessages/PlayNDAudioEmitter.hpp"
#include "Entity/GameMessages/ProximityUpdate.hpp"
#include "Entity/GameMessages/RacingPlayerLoaded.hpp"
#include "Entity/GameMessages/RebuildCancel.hpp"
#include "Entity/GameMessages/RebuildNotifyState.hpp"
#include "Entity/GameMessages/RequestDie.hpp"
#include "Entity/GameMessages/RequestLinkedMission.hpp"
#include "Entity/GameMessages/RequestResurrect.hpp"
#include "Entity/GameMessages/RequestSmashPlayer.hpp"
#include "Entity/GameMessages/RequestUse.hpp"
#include "Entity/GameMessages/RestoreToPostLoadStats.hpp"
#include "Entity/GameMessages/Resurrect.hpp"
#include "Entity/GameMessages/ServerTerminateInteraction.hpp"
#include "Entity/GameMessages/SetFlag.hpp"
#include "Entity/GameMessages/SetGravityScale.hpp"
#include "Entity/GameMessages/SetTooltipFlag.hpp"
#include "Entity/GameMessages/StartSkill.hpp"
#include "Entity/GameMessages/StopFXEffect.hpp"
#include "Entity/GameMessages/SyncSkill.hpp"
#include "Entity/GameMessages/TerminateInteraction.hpp"
#include "Entity/GameMessages/VehicleUnlockInput.hpp"
#include "Entity/GameMessages/VendorOpenWindow.hpp"

#endif