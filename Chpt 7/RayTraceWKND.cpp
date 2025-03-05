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
#include "camera.h"
#include "randengine.h"
#include <string>
#include <stdint.h>
#include <random>
#define WIDTH 200
#define HEIGHT 100
#define SAMPLES 50

vec3 color(const ray& r, solid& world) {
	hit_rec rec;
	if (world.hit(r, 0.001, 10000, rec)) {
		return 0.5*color(ray(rec.p,rec.p + rec.n + randengine::randsphere()), world);
	}
	else {
		vec3 udir = make_unit(r.direction());
		float p = 0.5 * (udir.y() + 1.0);
		return (1.0 - p) * vec3(1.0, 1.0, 1.0) + p * vec3(0.5, 0.7, 1.0);
	}
}
int main()
{
	frostbmpimg image(WIDTH, HEIGHT, 0, 0, 0);
	solid* objs[2];
	objs[0] = new sphere(vec3(0, 0, -1), 0.5);
	objs[1] = new plane(vec3(0, -0.5, 0), vec3(0, -1, 0));
	solid_list world(objs, 2);
	camera cam;
	for (int_fast32_t y = HEIGHT - 1;y >= 0;y--) {
		for (uint_fast32_t x = 0;x < WIDTH;x++) {
			vec3 col = vec3(0,0,0);
			for (uint_fast8_t i = 0;i < SAMPLES;i++) {
				col += color(cam.cast((x + randengine::rand()) / float(WIDTH), (y + randengine::rand()) / float(HEIGHT)), world);
			}
			col /= float(SAMPLES);
			image.setPixel(x, HEIGHT-y-1, int(255.99 * sqrt(col[0])), int(255.99 * sqrt(col[1])), int(255.99 * sqrt(col[2])));
		}
	}
	image.writeFile("out.bmp");
	return 0;
}