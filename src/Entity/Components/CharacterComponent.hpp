#ifndef __REPLICA__COMPONENTS__CHARACTER_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__CHARACTER_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Database/Database.hpp"

class CharacterComponent : public IEntityComponent {
private:
	Database::Str_DB_CharInfo charInfo = Database::Str_DB_CharInfo();
	Database::Str_DB_CharStyle charStyle = Database::Str_DB_CharStyle();

	// Dirty flags
	bool _dirtyPart2 = true;

public:
	void SetLevel(std::uint32_t newLevel) {
		charInfo.uLevel = newLevel;
	}

	CharacterComponent() : IEntityComponent() {}

	static constexpr int GetTypeID() { return 4; }

	void InitCharInfo(Database::Str_DB_CharInfo info) {
		charInfo = info;
	}

	void InitCharStyle(Database::Str_DB_CharStyle style) {
		charStyle = style;
	}

	Database::Str_DB_CharInfo GetCharInfo() {
		return charInfo;
	}

	Database::Str_DB_CharStyle GetCharStyle() {
		return charStyle;
	}

	void Serialize(RakNet::BitStream * factory, ReplicaTypes::PacketTypes packetType) {
		/* TODO: Part 1 Serialization */
		factory->Write(false);

		/* Part 2 Serialization */
		factory->Write(_dirtyPart2);
		if (_dirtyPart2) {
			factory->Write(charInfo.uLevel);
			_dirtyPart2 = false;
		}

		/* TODO: Part 3 Serialization */
		factory->Write(false);

		/* Part 4 Serialization */
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
			factory->Write(false); // Unknown
			factory->Write(false); // Unknown
			factory->Write(false); // Unknown
			factory->Write(false); // Unknown
		
			// Char Style
			factory->Write<std::uint32_t>(charStyle.hairColor);
			factory->Write<std::uint32_t>(charStyle.hairStyle);
			factory->Write<std::uint32_t>(0); // TODO: ???, could be "hd" or "hdc" from xml data
			factory->Write<std::uint32_t>(charStyle.chestColor);
			factory->Write<std::uint32_t>(charStyle.legs);
			factory->Write<std::uint32_t>(0); // TODO: ???, could be "cd" from xml data
			factory->Write<std::uint32_t>(0); // TODO: ???, could be "hdc" or "hd" from xml data
			factory->Write<std::uint32_t>(charStyle.eyebrowStyle);
			factory->Write<std::uint32_t>(charStyle.eyesStyle);
			factory->Write(std::uint32_t(charStyle.mouthStyle));

			// Char Info
			factory->Write<std::uint64_t>(charInfo.accountID);
			factory->Write<std::uint64_t>(charInfo.lastLog);
			factory->Write<std::uint64_t>(0); // Unknown 
			factory->Write<std::uint64_t>(charInfo.uScore);
			factory->Write(false); // is player free to play

			// TODO: Char Stats
			for (int i = 0; i < 27; ++i)
				factory->Write<std::uint64_t>(0);

			// Invisible flag
			factory->Write(false); // ???, makes player invisible

			// TODO: Rocket Info
			factory->Write(false);
		}

		// TODO: Additional flags
		factory->Write(false);
		
		// TODO: Activity
		factory->Write(false);

		// TODO: Guilds
		factory->Write(false);
	}
};

#endif