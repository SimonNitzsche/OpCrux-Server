#ifndef __NETWORKDATATYPES__BYTEBOOL_HPP__
#define __NETWORKDATATYPES__BYTEBOOL_HPP__

#include <memory>

class ByteBool {
private:
	uint8_t data;
public:
	ByteBool() : data(0) {}
	ByteBool(uint8_t val) : data(val & 1) {}
	ByteBool(bool val) : data(val) {}
	~ByteBool() {}

	explicit operator bool() const {
		return data;
	}

	ByteBool & operator=(const uint8_t other) { data = (other & 1); }
	ByteBool & operator=(const bool other) { data = (other); }
};

#endif // !__NETWORKDATATYPES__BYTEBOOL_HPP__
