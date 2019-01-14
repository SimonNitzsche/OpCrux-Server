#ifndef __ENUMS__EPROPERTYACHIEVEMENT_HPP__
#define __ENUMS__EPROPERTYACHIEVEMENT_HPP__

#include <memory>

namespace Enum {

	/*
		What achievement is it?
	*/
	enum class EPropertyAchievement : int32_t {
		None = 0,
		Builder,
		Craftsman,
		SeniorBuilder,
		Journeyman,
		MasterBuilder,
		Architect,
		SeniorArchitect,
		MasterArchitect,
		Visionary,
		Exemplar
	};
}

#endif // !__ENUMS__EPROPERTYACHIEVEMENT_HPP__
