#pragma once

#include "solid.h"
#include "material.h"

class plane : public solid {
public:
	plane() {};
	plane(vec3 c, vec3 n, material* m) : center(c), direction(n), mat(m) { normal = make_unit(-n); };
	virtual bool hit(const ray& r, const float_t min_p, const float_t max_p, hit_rec& rec) const;
	vec3 center;
	vec3 direction;
	vec3 normal;
	material* mat;
};