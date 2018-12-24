#ifndef _POSITION_HPP__
#define _POSITION_HPP__

#include "DataTypes/Quaternion.hpp"
// Note: Vector3 is included in Quaternion

namespace DataTypes {

	/*
		Continutes a Position and Rotation of something in a LEGO Universe world.
	*/
	struct Position {
		public:
			Vector3 pos;
			Quaternion rot;
	};
}

#endif // !_POSITION_HPP__
