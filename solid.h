#pragma once

#include "ray.h"

class material;

struct hit_rec {
	float_t prog;
	vec3 p;
	vec3 n;
	material *mat;
};

class solid {
public:
	virtual bool hit(const ray& r, const float_t min_p, const float_t max_p, hit_rec& rec) const = 0;
};