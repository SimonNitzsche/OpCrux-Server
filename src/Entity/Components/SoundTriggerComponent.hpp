#ifndef __REPLICA__COMPONENTS__SOUND_TRIGGER_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__SOUND_TRIGGER_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameObject.hpp"

#include "GameCache/ComponentsRegistry.hpp"
#include "Utils/ServerInfo.hpp"

class SoundTriggerComponent : public IEntityComponent {
private:

	bool m_isDirty = true;

	struct NDAudioMusicCue {
		std::string m_Name = "";
		std::uint32_t m_Result = 1;
		std::float_t m_BoredomTime = -1.0f;
	};

	std::list<NDAudioMusicCue> m_musicCues = {};

	std::uint16_t unknownUInt16 = 0;

	struct AudioGUID {
		std::uint64_t data0 = std::uint32_t(0);
		std::uint64_t data1 = std::uint32_t(0);

		void FromString(std::string& str) {
			// Save computation time, by ignoring default
			//			0		  1			2		  3
			//			01234567890123456789012345678901234567
			// -> 1-8:8, 10-13:4, 15-18:4, 20-23:4, 25-26:12
			if (str == "{00000000-0000-0000-0000-000000000000}") return;

			// src: {55820982-0a6f-498e-9b5b-c4d57625499b}
			// target:
			// a0: 1434585474
			// b0: 2671
			// c0: 18830
			// d0: 23451
			// e0: 54724
			// e1: 2605262198

			// extract the source
			std::uint64_t placeHolderU64;
			std::uint32_t placeHolderU32;
			std::uint16_t placeHolderU16;
			std::stringstream ss;

			ss << std::hex << str.substr(1, 8);
			ss >> placeHolderU32;
			data0 |= (std::uint64_t(placeHolderU32)<< 32);
			ss.clear();

			ss << std::hex << str.substr(10, 4);
			ss >> placeHolderU16;
			data0 |= (std::uint64_t(placeHolderU16) << 16);
			ss.clear();

			ss << std::hex << str.substr(15, 4);
			ss >> placeHolderU16;
			data0 |= (std::uint64_t(placeHolderU16));
			ss.clear();

			ss << std::hex << str.substr(20, 4);
			ss >> placeHolderU16;
			data1 |= (std::uint64_t(placeHolderU16) << 48);
			ss.clear();

			ss << std::hex << str.substr(25, 12);
			ss >> placeHolderU64;
			data1 |= (placeHolderU64);
			ss.clear();

			// Now we need to reverse both data buffers
			for (int i = 0; i < 2; ++i) {
				auto dataPtr = (reinterpret_cast<std::uint64_t*>(&data0) + i);
				auto tempData = *dataPtr;
				auto tmpData8 = reinterpret_cast<std::uint8_t*>(&tempData);
				auto data8 = reinterpret_cast<std::uint8_t*>(dataPtr);
				for (int j = 0; j < 8; ++j) {
					*(data8 + j) = *(tmpData8 + (7-j));
				}
			}
		}
	};

	struct GUID2Content {
		AudioGUID guid;
		std::uint32_t unknownGUID32 = 1;
	};
	std::list<GUID2Content> GUID2 = {};

	struct NDAudioMixerProgram {
		std::string m_Name;
		std::uint32_t m_UnnownU32 = 1;
	};

	std::list<NDAudioMixerProgram> m_mixers = {};

public:

	SoundTriggerComponent(std::int32_t componentID) : IEntityComponent(componentID) {}


	static constexpr int GetTypeID() { return 77; }

	void Serialize(RakNet::BitStream* bs, ReplicaTypes::PacketTypes packetType) {
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) m_isDirty = true;
		
		bs->Write(m_isDirty);
		if (m_isDirty) {
			
			bs->Write<std::uint8_t>(m_musicCues.size());
			for (auto cue : m_musicCues) {
				StringUtils::writeStringToBitStream<std::uint8_t>(bs, cue.m_Name);
				bs->Write(cue.m_Result);
				bs->Write(cue.m_BoredomTime);
			}

			bs->Write(unknownUInt16);

			bs->Write<std::uint8_t>(GUID2.size());
			for (auto guid2 : GUID2) {
				bs->Write(guid2.guid.data0);
				bs->Write(guid2.guid.data1);
				bs->Write(guid2.unknownGUID32);
			}

			bs->Write<std::uint8_t>(m_mixers.size());
			for (auto mixer : m_mixers) {
				StringUtils::writeStringToBitStream<std::uint8_t>(bs, mixer.m_Name);
				bs->Write(mixer.m_UnnownU32);
			}

			m_isDirty = false;
		}
	}

	void PopulateFromLDF(LDFCollection* collection) {
		NDAudioMusicCue musicCue;
		LDF_GET_VAL_FROM_COLLECTION(musicCue.m_Name, collection, u"NDAudioMusicCue_Name", "");
		LDF_GET_VAL_FROM_COLLECTION(musicCue.m_BoredomTime, collection, u"NDAudioMusicCue_BoredomTime", -1.0f);
		m_musicCues.push_back(musicCue);

		NDAudioMixerProgram program;
		LDF_GET_VAL_FROM_COLLECTION(program.m_Name, collection, u"NDAudioMixerProgram_Name", "");
		m_mixers.push_back(program);

		GUID2Content guid2Content;
		std::string guid2Str;
		LDF_GET_VAL_FROM_COLLECTION(guid2Str, collection, u"NDAudioEventGUID2", "{00000000-0000-0000-0000-000000000000}");
		guid2Content.guid.FromString(guid2Str);
		GUID2.push_back(guid2Content);
	}

	bool bCueExists(std::string name) {
		for (auto cue : m_musicCues) {
			if (cue.m_Name == name) return true;
		}
		return false;
	}

	void OnActivateNDAudioMusicCue(Entity::GameObject* sender, GM::ActivateNDAudioMusicCue* msg) {
		// We already have it
		if (bCueExists(msg->m_NDAudioMusicCueName)) return;

		NDAudioMusicCue cue;
		cue.m_Name = msg->m_NDAudioMusicCueName;
		m_musicCues.push_back(cue);
		m_isDirty = true;
		owner->SetDirty();
	}

	void OnDeactivateNDAudioMusicCue(Entity::GameObject* sender, GM::DeactivateNDAudioMusicCue* msg) {
		for (auto it = m_musicCues.begin(); it != m_musicCues.end(); ++it) {
			if (it->m_Name == msg->m_NDAudioMusicCueName) {
				m_musicCues.erase(it);
				break;
			}
		}

		m_isDirty = true;
		owner->SetDirty();
	}

	void RegisterMessageHandlers() {
		REGISTER_OBJECT_MESSAGE_HANDLER(SoundTriggerComponent, GM::ActivateNDAudioMusicCue, OnActivateNDAudioMusicCue);
		REGISTER_OBJECT_MESSAGE_HANDLER(SoundTriggerComponent, GM::DeactivateNDAudioMusicCue, OnDeactivateNDAudioMusicCue);
	}

};

#endif