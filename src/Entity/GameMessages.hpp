#ifndef __ENTITY__GameMessages_HPP__
#define __ENTITY__GameMessages_HPP__

#include "Sessions/ClientSession.hpp"
#include <RakNet/BitStream.h>
#include <RakNet/RakNetTypes.h>
#include "DataTypes/LWOOBJID.hpp"
#include "Enums/EGameMessages.hpp"
#include "Server/WorldServer.hpp"

#include "Enums/EGeneralPackets.hpp"
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
		bool useDefault; bs->Read(useDefault);\
		if(!useDefault) parameter = defaultVal;\
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
		bool useDefault = parameter == defaultVal; bs->Write(useDefault);\
		if(!useDefault)\
			GM_VAR_SERIALIZE(bs, parameter);\
	}\
}

namespace GM {
	struct GMBase {
		GMBase() {}
		static constexpr Enums::EGameMessageID GetID() { return (Enums::EGameMessageID)(-1); };
		virtual void Deserialize(RakNet::BitStream * bs) {};
		virtual void Serialize(RakNet::BitStream * bs) {};
		virtual void TriggerEvent(Entity::GameObject * sender, Entity::GameObject * target) {};
	};
}

class GameMessages {
public:
	static void Deserialize(WorldServer * Instance, ClientSession * session, RakNet::BitStream *bs);

	template<typename T = GM::GMBase>
	static void Send(WorldServer * Instance, SystemAddress systemAddress, DataTypes::LWOOBJID target, T gm) {
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
				Instance->rakServer->Send(&bs, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, session.systemAddress, false);
			}
		}
		else
			Instance->rakServer->Send(&bs, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, systemAddress, systemAddress != UNASSIGNED_SYSTEM_ADDRESS);
	}

};

// The Game Messages:

// Define by include
#include "Entity/GameMessages/ArrivedAtDesiredWaypoint.hpp"
#include "Entity/GameMessages/Die.hpp"
#include "Entity/GameMessages/EmotePlayed.hpp"
#include "Entity/GameMessages/FireEventServerSide.hpp"
#include "Entity/GameMessages/Knockback.hpp"
#include "Entity/GameMessages/OfferMission.hpp"
#include "Entity/GameMessages/PlayAnimation.hpp"
#include "Entity/GameMessages/PlayEmote.hpp"
#include "Entity/GameMessages/PlayFXEffect.hpp"
#include "Entity/GameMessages/PlayNDAudioEmitter.hpp"
#include "Entity/GameMessages/ProximityUpdate.hpp"
#include "Entity/GameMessages/RequestResurrect.hpp"
#include "Entity/GameMessages/RequestSmashPlayer.hpp"
#include "Entity/GameMessages/RequestUse.hpp"
#include "Entity/GameMessages/Resurrect.hpp"
#include "Entity/GameMessages/SetGravityScale.hpp"
#include "Entity/GameMessages/StartSkill.hpp"
#include "Entity/GameMessages/StopFXEffect.hpp"
#include "Entity/GameMessages/SyncSkill.hpp"

#endif