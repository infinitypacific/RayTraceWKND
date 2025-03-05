#pragma once
#include "vec3.h"

class ray {
public:
	ray() {}
	ray(const vec3& a, const vec3& b) : O(a), D(b) {}
	vec3 origin() const { return O; }
	vec3 direction() const { return D; }
	vec3 pap(float p) const { return O + p*D; }
	//vec3 test() const { return O * 1.0; }

	vec3 O;
	vec3 D;
};