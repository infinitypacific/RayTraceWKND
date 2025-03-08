#include "sphere.h"

sphere::~sphere()
{
	delete mat;
}

bool sphere::hit(const ray& r, const float_t min_p, const float_t max_p, hit_rec& rec) const
{
	vec3 loc = r.origin() - center;
	const float_t a = dot(r.direction(), r.direction());
	const float_t b = dot(loc, r.direction());
	const float_t disc = (b * b - a * (dot(loc, loc) - radius * radius));
	if (disc > 0) {
		rec.prog = ((-b - sqrt(disc)) / a);
		if (rec.prog < max_p && rec.prog > min_p) {
			rec.p = r.pap(rec.prog);
			rec.n = (rec.p - center) / radius;
			rec.mat = mat;
			return true;
		}
		rec.prog = ((-b + sqrt(disc)) / a);
		if (rec.prog < max_p && rec.prog > min_p) {
			rec.p = r.pap(rec.prog);
			rec.n = (rec.p - center) / radius;
			rec.mat = mat;
			return true;
		}
	}
	return false;
}
