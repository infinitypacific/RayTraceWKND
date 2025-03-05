#pragma once

#include "ray.h"

class material;

struct hit_rec {
	float prog;
	vec3 p;
	vec3 n;
	material *mat;
};

class solid {
public:
	virtual bool hit(const ray& r, float min_p, float max_p, hit_rec& rec) const = 0;
};