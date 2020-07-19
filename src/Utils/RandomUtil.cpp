#include "RandomUtil.hpp"
std::random_device RandomUtil::rd;
std::mt19937 RandomUtil::engine(rd());