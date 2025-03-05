// RayTraceWKND.cpp : Defines the entry point for the application.
//

#include "RayTraceWKND.h"
#include "frostimg.h"
#include "vec3.h"
#include "ray.h"
#include "solid.h"
#include "sphere.h"
#include "plane.h"
#include "solidlist.h"
#include <string>
#include <stdint.h>
#define WIDTH 200
#define HEIGHT 100
//#define SCENTER vec3(0, 0, -1)

/*
float sphere_hit(const vec3& center, const float radius, const ray& r) {
	vec3 loc = r.origin() - center;
	const float a = dot(r.direction(), r.direction());
	const float b = (2.0 * dot(loc, r.direction()));
	const float disc = (b * b - 4.0 * a * (dot(loc, loc) - radius * radius));
	if (disc < 0) {
		return -1.0;
	}
	else {
		return ((-b - sqrt(disc)) / (2.0 * a));
	}
}
float plane_hit(const ray& plane, const ray& r) {
	return (dot((plane.origin()-r.origin()),plane.direction()) / dot(r.direction(),plane.direction()));
}
*/

vec3 color(const ray& r, solid& world) {
	/*
	ray plane = ray(vec3(0, -1, 0), vec3(0, -1, 0));
	float prog = sphere_hit(SCENTER, 0.5, r);
	if (prog > 0) {
		vec3 N = make_unit(r.pap(prog) - SCENTER);
		return 0.5 * vec3(N.x() + 1, N.y() + 1, N.z() + 1);
	}
	prog = plane_hit(plane, r);
	if (prog > 0) {
		vec3 N = make_unit(-plane.direction());
		return 0.5 * vec3(N.x() + 1, N.y() + 1, N.z() + 1);
	}
	*/
	hit_rec rec;
	if (world.hit(r, 0.001, 10000, rec)) {
		return 0.5 * vec3(rec.n.x() + 1, rec.n.y() + 1, rec.n.z() + 1);
	}
	else {
		vec3 udir = make_unit(r.direction());
		float p = 0.5 * (udir.y() + 1.0);
		return (1.0 - p) * vec3(1.0, 1.0, 1.0) + p * vec3(0.5, 0.7, 1.0);
	}
}

using namespace std;
int main()
{
	frostbmpimg image(WIDTH, HEIGHT, 0, 0, 0);
	vec3 bl(-2.0, -1.0, -1.0);
	vec3 hori(4.0, 0.0, 0.0);
	vec3 vert(0.0, 2.0, 0.0);
	vec3 orig(0.0, 0.0, 0.0);
	solid* objs[2];
	objs[0] = new sphere(vec3(0, 0, -1), 0.5);
	objs[1] = new plane(vec3(0, -1, 0), vec3(0, -1, 0));
	solid_list world(objs, 2);
	for (int_fast32_t y = HEIGHT - 1;y >= 0;y--) {
		for (uint_fast32_t x = 0;x < WIDTH;x++) {
			ray r(orig,bl+(float(x) / float(WIDTH))*hori+(float(y) / float(HEIGHT))*vert);
			vec3 col = color(r,world);
			image.setPixel(x, HEIGHT-y-1, int(255.99 * col[0]), int(255.99 * col[1]), int(255.99 * col[2]));
		}
	}
	image.writeFile("out.bmp");
	return 0;
}
