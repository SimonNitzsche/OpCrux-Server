#ifndef __FILETYPES__HKXFILE__CLASSES__HKCLASS_HPP__
#define __FILETYPES__HKXFILE__CLASSES__HKCLASS_HPP__

#include <memory>
#include <string>

class hkClassEnum;
class hkClassMember;

class hkClass {
public:
	enum class SignatureFlags {
		LOCAL = 1
	};

	enum class FlagValues {
		NONE, NOT_SERIALIZABLE
	};

private:
	std::string m_name;
	hkClass* m_parent;
	std::int32_t m_objectSize;
	std::int32_t m_numImplementedInterfaces;
	hkClassEnum* m_declaredEnums;
	hkClassMember* m_declaredMembers;
	//hkCustomAttributes* m_attributes;
	//Flags m_flags;
	std::int32_t m_describedVersion;

public:

// TODO: Getters and setters
};

#endif