#pragma once

#include "ray.h"

class camera {
public:
	camera() {
		origin = vec3(0, 0, 0);
		bl = vec3(-2.0, -1.0, -1.0);
		hori = vec3(4.0, 0.0, 0.0);
		vert = vec3(0.0, 2.0, 0.0);
	}
	ray cast(float u, float v) const { return ray(origin, (bl + u * hori + v * vert) - origin); }
	vec3 origin;
	vec3 bl;
	vec3 hori;
	vec3 vert;
};