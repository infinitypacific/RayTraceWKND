#pragma once

#include "ray.h"
#include "randengine.h"

class camera {
public:
	camera(const vec3 lookfrom, const vec3 lookat, const vec3 upv, const float_t vfov, const float_t aspect, const float_t aperture, const float_t dfocus) {
		lensr = aperture * 0.5;
		origin = lookfrom;
		lf = make_unit(lookfrom - lookat);
		lh = make_unit(cross(upv, lf));
		lv = cross(lf, lh);
		float_t hheight = tan((vfov*3.1415926535879)/360);
		float_t hwidth = hheight * aspect;
		bl = origin - hwidth * dfocus * lh - hheight * dfocus * lv - dfocus * lf;
		hori = 2*hwidth*dfocus*lh;
		vert = 2*hheight*dfocus*lv;
	}
	ray cast(float_t u, float_t v) const { const vec3 ric = lensr * randengine::randcirc();const vec3 offset = lh * ric.x() + lv * ric.y();return ray(origin+offset, (bl + u * hori + v * vert) - origin - offset); }
	vec3 origin;
	vec3 bl;
	vec3 hori;
	vec3 vert;
	vec3 lf, lh, lv;
	float_t lensr;
};