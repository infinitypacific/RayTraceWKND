#pragma once

#include "solid.h"

class solid_list : public solid {
public:
	solid_list() {};
	solid_list(solid** l, size_t s) { list = l; size = s; }
	~solid_list();
	virtual bool hit(const ray& r, float min_p, float max_p, hit_rec& rec) const;
	solid** list;
	size_t size;
};