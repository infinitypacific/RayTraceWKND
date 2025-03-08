#pragma once

#include "solid.h"
#include "material.h"

class sphere : public solid {
public:
	sphere() {};
	sphere(vec3 c, float_t r, material* m) : center(c), radius(r), mat(m) {};
	~sphere();
	virtual bool hit(const ray& r, const float_t min_p, const float_t max_p, hit_rec& rec) const;
	vec3 center;
	float_t radius;
	material* mat;
};