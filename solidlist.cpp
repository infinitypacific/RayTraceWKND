#include "solidlist.h"

solid_list::~solid_list()
{
	for (size_t i = 0;i < size;i++) {
		delete list[i];
	}
}

bool solid_list::hit(const ray& r, float min_p, float max_p, hit_rec& rec) const
{
	bool hit = false;
	double closest = max_p;
	hit_rec temp_rec;
	for (size_t i = 0;i < size;i++) {
		if (list[i]->hit(r, min_p, max_p, temp_rec) && temp_rec.prog<closest) {
			hit = true;
			//if (hit) { std::cout << closest << std::endl; }
			/*
			std::cout << closest << std::endl;
			std::cout << rec.prog << std::endl;
			*/
			closest = temp_rec.prog;
			rec = temp_rec;
		}
	}
	return hit;
}
