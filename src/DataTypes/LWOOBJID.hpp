#pragma once
#ifndef _LWOOBJID_HPP__
#define _LWOOBJID_HPP__

#include <string>
#include <sstream>
#include "Utils/Logger.hpp"

class LWOOBJID {
public:
	enum class CATEGORY {
		STATIC = 0,
		LOCAL,
		GLOBAL,
		SPAWNED
	};

	// LWOOBJID --> unsigned long long
	operator unsigned long long() const {
		return data;
	}
	// unsigned long long --> LWOOBJID
	LWOOBJID & operator=(const unsigned long long other) {
		this->data = other;
		if (other == -1) this->data = 0;
		return *this;
	}

	LWOOBJID(unsigned long long unsll) {
		if (unsll == -1) unsll = 0;
		this->data = unsll;
		initDone();
	}

	void initDone() {
		static const std::string CATEGORY_NAMES[4] = { "STATIC","LOCAL","GLOBAL","SPAWNED" };
		std::stringstream ss; ss << this->data << " [ " << (int)this->getFlags() << " | " << this->getPureID() << " ] -> " << CATEGORY_NAMES[(int)this->getCategory()];
		Logger::log("LWOOBJID", ss.str());
	}

	LWOOBJID() { this->data = NULL; }
private:
	static const unsigned char flagSize = 8; // Adjustable (make sure the type specifier fits it)
	static const unsigned char idSize = 64 - flagSize;
	unsigned long long data;
public:
	unsigned char getFlags() {
		return this->data >> (idSize - 1);
	}
	unsigned long long getPureID() {
		return (this->data & 0x00FFFFFFFFFFFFFF);
	}
	CATEGORY getCategory() {
		unsigned char flags = getFlags();

		// SPAWNED
		if (flags == (1 << (57 - idSize)))
			return CATEGORY::SPAWNED;

		// GLOBAL
		unsigned char highest5 = (flags >> (flagSize - 5));
		if (highest5 && !(highest5 & (highest5 - 1)))
			return CATEGORY::GLOBAL;

		// LOCAL
		if ((flags & (1 << (57 - idSize))) && (flags & (1 << (45 - idSize))))
			return CATEGORY::LOCAL;

		// STATIC
		if ((flags >> (flagSize - 6)) == NULL)
			return CATEGORY::STATIC;

		Logger::log("LWOOBJID", "Impossible flag category", LogType::UNEXPECTED);
		throw std::runtime_error("Impossible Flag Category.");
	}
};

#endif // !_LWOOBJID_HPP__