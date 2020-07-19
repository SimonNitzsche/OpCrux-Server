#ifndef __UTILS__RANDOMUTIL_HPP__
#define __UTILS__RANDOMUTIL_HPP__
#include <random>
/*
	MT19937 is pretty big. For this reason we should only have it once.
*/
class RandomUtil {
	static std::random_device rd;
	static std::mt19937 engine;
public:
	static std::mt19937 & GetEngine() {
		return engine;
	}
};
#endif