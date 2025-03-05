#pragma once

#include "vec3.h"
#include "ray.h"
#include "solid.h"
#include "material.h"
#include "randengine.h"

class metal : public material {
public:
	metal(const vec3& a) : albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_rec& rec, vec3& atten, ray& scatter) const {
		const vec3 refed = reflect(make_unit(r_in.direction()), rec.n);
		scatter = ray(rec.p, refed);
		atten = albedo;
		return (dot(scatter.direction(),rec.n) > 0);
	};
	vec3 albedo;
};