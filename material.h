#pragma once

#include "vec3.h"
#include "ray.h"
#include "solid.h"

class material {
public:
	virtual bool scatter(const ray& r_in, const hit_rec& rec, vec3& atten, ray& scatter) const = 0;
};