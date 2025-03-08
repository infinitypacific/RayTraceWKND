#pragma once

#include "vec3.h"
#include <random>

class randengine {
public:
	//static void init();
	static inline float_t rand();
	static vec3 randsphere();
	static vec3 randcirc();
	static vec3 randvec();
	static float_t randNP();
	static std::random_device rd;
	static std::mt19937 gen;
	static std::uniform_real_distribution<float_t> randdis;
	static std::uniform_real_distribution<float_t> spheredis;
};

inline float_t randengine::rand()
{
	return randdis(gen);
}

inline float_t randengine::randNP()
{
	return spheredis(gen);
}