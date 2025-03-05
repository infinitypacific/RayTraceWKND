// RayTraceWKND.cpp : Defines the entry point for the application.
//

#include "RayTraceWKND.h"
#include "frostimg.h"
#include "vec3.h"
#include "ray.h"
#include <stdint.h>
#define WIDTH 200
#define HEIGHT 100

vec3 color(const ray& r) {
	vec3 udir = make_unit(r.direction());
	float p = 0.5 * (udir.y() + 1.0);
	return (1.0 - p) * vec3(1.0, 1.0, 1.0) + p * vec3(0.5, 0.7, 1.0);
}

using namespace std;
int main()
{
	frostbmpimg image(WIDTH, HEIGHT, 0, 0, 0);
	//ray hello(vec3(0, 0, 0), vec3(1, 1, 1));
	vec3 bl(-2.0, -1.0, -1.0);
	vec3 hori(4.0, 0.0, 0.0);
	vec3 vert(0.0, 2.0, 0.0);
	vec3 orig(0.0, 0.0, 0.0);
	for (int_fast32_t y = HEIGHT - 1;y >= 0;y--) {
		for (uint_fast32_t x = 0;x < WIDTH;x++) {
			ray r(orig,bl+(float(x) / float(WIDTH))*hori+(float(y) / float(HEIGHT))*vert);
			vec3 col = color(r);
			image.setPixel(x, HEIGHT-y-1, int(255.99 * col[0]), int(255.99 * col[1]), int(255.99 * col[2]));
		}
	}
	image.writeFile("out.bmp");
	return 0;
}
