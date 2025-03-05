// RayTraceWKND.cpp : Defines the entry point for the application.
//

#include "RayTraceWKND.h"
#include "frostimg.h"
#include "vec3.h"
#include "ray.h"
#include <stdint.h>
#define WIDTH 200
#define HEIGHT 100

using namespace std;
int main()
{
	frostbmpimg image(WIDTH, HEIGHT, 0, 0, 0);
	ray hello(vec3(0, 0, 0), vec3(1, 1, 1));
	for (int_fast32_t y = HEIGHT - 1;y >= 0;y--) {
		for (uint_fast32_t x = 0;x < WIDTH;x++) {
			vec3 col((float(x) / float(WIDTH)), (float(y) / float(HEIGHT)), 0.2);
			image.setPixel(x, HEIGHT-y-1, int(255.99 * col[0]), int(255.99 * col[1]), int(255.99 * col[2]));
		}
	}
	image.writeFile("out.bmp");
	return 0;
}
