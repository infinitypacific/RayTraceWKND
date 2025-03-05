#include "randengine.h"
#include "vec3.h"
#include <random>

std::random_device randengine::rd;
std::mt19937 randengine::gen(randengine::rd());
std::uniform_real_distribution<float> randengine::randdis(0.0, 1.0);
std::uniform_real_distribution<float> randengine::spheredis(-1.0, 1.0);
/*
void randengine::init()
{
	gen = std::mt19937(rd());
	randdis = std::uniform_real_distribution<float>(0.0, 1.0);
	spheredis = std::uniform_real_distribution<float>(-1.0, 1.0);
}
i'm stupid
*/
vec3 randengine::randsphere()
{
	vec3 out;
	do {
		out = vec3(spheredis(gen), spheredis(gen), spheredis(gen));
	} while (sqrt(out.x() * out.x() + out.y() * out.y() + out.z() * out.z()) > 1);
	return out;
}
