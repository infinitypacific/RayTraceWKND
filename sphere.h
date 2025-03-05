#pragma once

#include "solid.h"
#include "material.h"

class sphere : public solid {
public:
	sphere() {};
	sphere(vec3 c, float r, material* m) : center(c), radius(r), mat(m) {};
	~sphere();
	virtual bool hit(const ray& r, float min_p, float max_p, hit_rec& rec) const;
	vec3 center;
	float radius;
	material* mat;
};