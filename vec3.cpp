#include "vec3.h"
#include <string>

vec3::operator std::string() const
{
	return std::to_string(v[0]) + ',' + std::to_string(v[1]) + ',' + std::to_string(v[2]);
}

bool refract(const vec3& v, const vec3& n, const float_t ioroverior, vec3& refract) {
	//const vec3 uv = make_unit(v);
	const float_t stheta = dot(v, n);
	const float_t disc = 1 - ioroverior * ioroverior * (1 - stheta * stheta);
	if (disc > 0) {
		refract = ioroverior * (v - n * stheta) - n * sqrt(disc);
		return true;
	}
	else {
		return false;
	}
}

float_t schlick(float_t cosv, float_t ior) {
	float_t r0 = (1 - ior) / (1 + ior);
	r0 *= r0;
	return r0 + (1 - r0) * pow(1-cosv, 5);
}