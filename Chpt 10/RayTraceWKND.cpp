#include "frostimg.h"
#include "vec3.h"
#include "ray.h"
#include "solid.h"
#include "sphere.h"
#include "plane.h"
#include "solidlist.h"
#include "camera.h"
#include "randengine.h"
#include "lambert.h"
#include "dielectric.h"
#include "metal.h"
#include <string>
#include <thread>
#include <vector>
#include <chrono>
#include <stdint.h>
#define WIDTH 200
#define HEIGHT 100
#define SAMPLES 50

vec3 color(const ray& r, solid& world, unsigned short depth) {
	hit_rec rec;
	if (world.hit(r, 0.001, 10000, rec)) {
		ray scatter;
		vec3 atten;
		if (depth < 10 && rec.mat->scatter(r, rec, atten, scatter)) {
			return atten * color(scatter, world, depth + 1);
		}
		else {
			return vec3(0, 0, 0);
		}
	}
	else {
		vec3 udir = make_unit(r.direction());
		float p = 0.5 * (udir.y() + 1.0);
		return (1.0 - p) * vec3(1.0, 1.0, 1.0) + p * vec3(0.5, 0.7, 1.0);
	}
}

void processRow(frostbmpimg& img, solid& world, camera& cam, int_fast32_t y) {
	for (uint_fast32_t x = 0;x < WIDTH;x++) {
		vec3 col = vec3(0, 0, 0);
		for (uint_fast16_t i = 0;i < SAMPLES;i++) {
			col += color(cam.cast((x + randengine::rand()) / float(WIDTH), (y + randengine::rand()) / float(HEIGHT)), world, 0);
		}
		col /= float(SAMPLES);
		img.setPixel(x, HEIGHT - y - 1, int(255.99 * sqrt(col[0])), int(255.99 * sqrt(col[1])), int(255.99 * sqrt(col[2])));
	}
	std::cout << "EXIT ROW " << y << std::endl;
}

int main()
{
	std::vector<std::thread> threads;
	//uint_fast16_t activets = 0;
	const uint_fast16_t maxthreads = std::thread::hardware_concurrency();
	threads.reserve(maxthreads);
	std::cout << "MAX " << maxthreads << std::endl;
	frostbmpimg image(WIDTH, HEIGHT, 0, 0, 0);
	solid* objs[4];
	objs[0] = new sphere(vec3(0, 0, -2), 0.5, new lambert(vec3(0.8,0.1,0.1))); //-2.7, 0, -2
	objs[1] = new plane(vec3(0, -0.5, 0), vec3(0, -1, 0), new lambert(vec3(0.5, 0.5, 0.5)));
	objs[2] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(vec3(0.1,0.2,0.8), 1.5, 0.01)); //new metal(vec3(0.1, 0.1, 0.7),0.4)
	objs[3] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.1, 0.7, 0.1),0.1));
	solid_list world(objs, 4);
	camera cam(vec3(-2,1,1),vec3(0,0,-1),vec3(0,1,0),90, float(WIDTH)/float(HEIGHT));
	int_fast32_t y = HEIGHT - 1;
	for (uint_fast16_t i = 0;i < maxthreads;i++) {
		std::cout << "SPAWN ROW " << y << std::endl;
		threads.emplace_back(processRow, std::ref(image), std::ref(world), std::ref(cam), y);
		//activets++;
		--y;
	}
	while (y >= 0 || threads.size() > 0) {
		for (auto it = threads.begin(); it != threads.end();) { //int_fast32_t i = activets;i >= 0;i-- threads[i]
			if (it->joinable()) {
				it->join();
				it = threads.erase(it);
				if (y >= 0) {
					threads.emplace_back(processRow, std::ref(image), std::ref(world), std::ref(cam), y);
					std::cout << "SPAWN ROW " << y << std::endl;
					--y;
				}
			}
			else {
				++it;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		//processRow(image, world, cam, y);
		/*
		for (uint_fast32_t x = 0;x < WIDTH;x++) {
			//processPix(image, world, cam, x, y);
			std::thread tred(processPix, std::ref(image), std::ref(world), std::ref(cam), x, y);
			tred.join();
		}
		*/
	}
	image.writeFile("out.bmp");
	return 0;
}