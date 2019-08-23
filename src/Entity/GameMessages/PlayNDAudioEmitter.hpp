#ifndef __ENTITY__GM__PlayNDAudioEmitter_HPP__
#define __ENTITY__GM__PlayNDAudioEmitter_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct PlayNDAudioEmitter : GMBase {
		std::int64_t m_NDAudioCallbackMessageData = 0;
		std::int64_t m_NDAudioEmitterID = 0;
		std::string m_NDAudioEventGUID="";
		std::string m_NDAudioMetaEventName = "";
		bool m_Result = false;
		DataTypes::LWOOBJID m_TargetObjectIDForNDAudioCallbackMessages = 0ULL;

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_PLAY_AUDIO_EMITTER;
		}

		PlayNDAudioEmitter() {}
		void Deserialize(RakNet::BitStream * bs) {

		}

		void Serialize(RakNet::BitStream * bs) {
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, m_NDAudioCallbackMessageData, 0LL);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, m_NDAudioEmitterID, 0LL);
			GM_VAR_SERIALIZE(bs, m_NDAudioEventGUID);
			GM_VAR_SERIALIZE(bs, m_NDAudioMetaEventName);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, m_Result, false);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, m_TargetObjectIDForNDAudioCallbackMessages, 0ULL);
		}

		void TriggerEvent(Entity::GameObject * sender, Entity::GameObject * target) {
	
		}
	};
}

#endif