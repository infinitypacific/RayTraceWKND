#pragma once

#include "vec3.h"
#include "ray.h"
#include "solid.h"
#include "material.h"
#include "randengine.h"

class lambert : public material {
public:
	lambert(const vec3& a) : albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_rec& rec, vec3& atten, ray& scatter) const {
		scatter = ray(rec.p, rec.n + randengine::randsphere()); //rec.p + bug?
		atten = albedo;
		return true;
	};
	vec3 albedo;
};