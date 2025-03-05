#include "vec3.h"
#include <string>

vec3::operator std::string() const
{
	return std::to_string(v[0]) + ',' + std::to_string(v[1]) + ',' + std::to_string(v[2]);
}