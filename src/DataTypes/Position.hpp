#ifndef _POSITION_HPP__
#define _POSITION_HPP__

#include "Vector3.hpp"

struct Position {
public:
	Vector3 pos;
	struct {
	public:
		float x;
		float y;
		float z;
		float w;
	} rot;
};

#endif // !_POSITION_HPP__
