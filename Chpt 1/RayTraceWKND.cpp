// RayTraceWKND.cpp : Defines the entry point for the application.
//

#include "RayTraceWKND.h"
#include "frostimg.h"
#include <stdint.h>
#define WIDTH 200
#define HEIGHT 100

using namespace std;

int main()
{
	frostbmpimg image(WIDTH, HEIGHT, 0, 0, 0);
	for (int_fast32_t y = HEIGHT - 1;y >= 0;y--) {
		for (uint_fast32_t x = 0;x < WIDTH;x++) {
			image.setPixel(x, HEIGHT-y-1, int(255.99 * (float(x) / float(WIDTH))), int(255.99 * (float(y) / float(HEIGHT))), int(255.99 * 0.2));
		}
	}
	image.writeFile("out.bmp");
	return 0;
}
