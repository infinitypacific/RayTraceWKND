#pragma once

#include "vec3.h"
#include "material.h"
#include "randengine.h"

class dielectric : public material {
public:
	dielectric(vec3 atten, float_t i, float_t roughness) :ior(i),attenuation(atten),rough(roughness) {};
	virtual bool scatter(const ray& r_in, const hit_rec& rec, vec3& atten, ray& scatter) const {
		vec3 outn;
		float_t ioroverior;
		atten = attenuation;
		vec3 refractv;
		float_t rprob;
		float_t costheta;
		vec3 udir = make_unit(r_in.direction());
		if (dot(r_in.direction(), rec.n) > 0) {
			outn = -rec.n;
			ioroverior = ior;
			costheta = ior*dot(udir, rec.n);
		}
		else {
			outn = rec.n;
			ioroverior = 1.0/ior;
			costheta = -dot(udir, rec.n);
		}
		if (refract(udir, outn, ioroverior, refractv)) {
			rprob = schlick(costheta,ior);
		}
		else {
			//scatter = ray(rec.p, reflect(r_in.direction(), rec.n));
			//return false;
			rprob = 1;
		}
		if (rprob < 1 && randengine::rand() < rprob) {
			scatter = ray(rec.p, reflect(r_in.direction(), rec.n + rough * randengine::randsphere()));
		}
		else {
			scatter = ray(rec.p, refractv + rough*0.5 * randengine::randsphere());
		}
		return true;
	}
	float_t ior;
	vec3 attenuation;
	float_t rough;
};