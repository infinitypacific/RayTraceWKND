#include "plane.h"

bool plane::hit(const ray& r, float min_p, float max_p, hit_rec& rec) const
{
	rec.prog = (dot((center - r.origin()), direction) / dot(r.direction(), direction));
	if (rec.prog >= 0 && rec.prog>min_p && rec.prog<max_p) {
		rec.n = normal;
		rec.p = r.pap(rec.prog);
		rec.mat = mat;
		return true;
	}
	return false;
}
