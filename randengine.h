#pragma once

#include "vec3.h"
#include <random>

class randengine {
public:
	//static void init();
	static inline float rand();
	static vec3 randsphere();
	static std::random_device rd;
	static std::mt19937 gen;
	static std::uniform_real_distribution<float> randdis;
	static std::uniform_real_distribution<float> spheredis;
};

float randengine::rand()
{
	return randdis(gen);
}