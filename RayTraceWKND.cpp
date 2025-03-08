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
#include <cstdint>
#define WIDTH 1280
#define HEIGHT 720
#define SAMPLES 500
#define MAXDEPTH 10

vec3 color(const ray& r, solid& world, uint_fast16_t depth) {
	hit_rec rec;
	if (world.hit(r, 0.001, 10000, rec)) {
		ray scatter;
		vec3 atten;
		if (depth < MAXDEPTH && rec.mat->scatter(r, rec, atten, scatter)) {
			return atten * color(scatter, world, depth + 1);
		}
		else {
			return vec3(0, 0, 0);
		}
	}
	else {
		vec3 udir = make_unit(r.direction());
		float_t p = 0.5 * (udir.y() + 1.0);
		return (1.0 - p) * vec3(1.0, 1.0, 1.0) + p * vec3(0.5, 0.7, 1.0);
	}
}

void processRow(frostbmpimg& img, solid& world, camera& cam, int_fast32_t y) {
	for (uint_fast32_t x = 0;x < WIDTH;x++) {
		vec3 col = vec3(0, 0, 0);
		for (uint_fast16_t i = 0;i < SAMPLES;i++) {
			col += color(cam.cast((x + randengine::rand()) / float_t(WIDTH), (y + randengine::rand()) / float_t(HEIGHT)), world, 0);
		}
		col /= float_t(SAMPLES);
		img.setPixel(x, HEIGHT - y - 1, int(255.99 * sqrt(col[0])), int(255.99 * sqrt(col[1])), int(255.99 * sqrt(col[2])));
	}
	std::cout << "EXIT ROW " << y << std::endl;
}
solid** randScene(const uint_fast16_t num, const uint_fast16_t width, const uint_fast16_t length) {
	solid** scene = new solid*[num+4];
	scene[0] = new plane(vec3(0, 0, 0), vec3(0, -1, 0), new metal(vec3(0.1, 0.1, 0.1), 0.1));
	scene[1] = new sphere(vec3(0, 1, 0), 1, new dielectric(vec3(0.8, 0.8, 0.8), 1.5, 0.02));
	scene[2] = new sphere(vec3(-4, 1, 0), 1, new lambert(vec3(0.8, 0.2, 0.1)));
	scene[3] = new sphere(vec3(4, 1, 0), 1, new metal(vec3(0.8, 0.8, 0.2), 0.02));
	for (uint_fast16_t i = 4;i < num + 4;i++) {
		const uint_fast16_t temppos = int((float_t(i - 4) / float_t(num)) * ((width*length)+1));
		switch (int(randengine::rand() * 6)) {
		case 0:
		case 1:
		case 2:
			scene[i] = new sphere(vec3((temppos % length) + (randengine::randNP() * 0.5) - width / 2, 0.2, (temppos / length) + (randengine::randNP() * 0.5) - length / 2), 0.2, new lambert(randengine::randvec()));
			break;
		case 3:
		case 4:
			scene[i] = new sphere(vec3((temppos % length) + (randengine::randNP() * 0.5) - width / 2, 0.2, (temppos / length) + (randengine::randNP() * 0.5) - length / 2), 0.2, new metal(randengine::randvec(), randengine::rand() * 0.1));
			break;
		case 5:
			scene[i] = new sphere(vec3((temppos % length) + (randengine::randNP() * 0.5) - width / 2, 0.2, (temppos / length) + (randengine::randNP() * 0.5) - length / 2), 0.2, new dielectric(randengine::randvec(), 1.5, 0.005));
			break;
		}
	}
	return scene;
}

int main()
{
	std::vector<std::thread> threads;
	const uint_fast16_t maxthreads = std::thread::hardware_concurrency();
	threads.reserve(maxthreads);
	std::cout << "MAX " << maxthreads << std::endl;
	frostbmpimg image(WIDTH, HEIGHT, 0, 0, 0);
	solid_list world(randScene(500,25,25), 504);
	const vec3 lookfrom = vec3(9, 1.5, 3);
	const vec3 lookat = vec3(0, 0.5, 0);
	camera cam(lookfrom,lookat,vec3(0,1,0), 30, float_t(WIDTH)/ float_t(HEIGHT), 0.15, (lookfrom-lookat).length()); //90
	int_fast32_t y = HEIGHT - 1;
	for (uint_fast16_t i = 0;i < maxthreads;i++) {
		std::cout << "SPAWN ROW " << y << std::endl;
		threads.emplace_back(processRow, std::ref(image), std::ref(world), std::ref(cam), y);
		--y;
	}
	while (y >= 0 || threads.size() > 0) {
		for (auto it = threads.begin(); it != threads.end();) {
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
	}
	image.writeFile("out.bmp");
	return 0;
}