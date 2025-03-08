#pragma once

#include <cmath>
#include <cstdlib>
#include <iostream>

class vec3 {
public:
	vec3() {};
	vec3(float_t v0, float_t v1, float_t v2) { v[0] = v0;v[1] = v1;v[2] = v2; };

	inline float_t x() const { return v[0]; }
	inline float_t y() const { return v[1]; }
	inline float_t z() const { return v[2]; }
	inline float_t r() const { return v[0]; }
	inline float_t g() const { return v[1]; }
	inline float_t b() const { return v[2]; }

	inline const vec3& operator+() const { return *this; }
	inline const vec3& operator-() const { return vec3(-v[0], -v[1], -v[2]); }
	inline float_t operator[](int i) const { return v[i]; }
	inline float_t& operator[](int i) { return v[i]; }

	inline vec3& operator+=(const vec3& v2);
	inline vec3& operator-=(const vec3& v2);
	inline vec3& operator*=(const vec3& v2);
	inline vec3& operator/=(const vec3& v2);
	inline vec3& operator+=(const float_t o);
	inline vec3& operator-=(const float_t o);
	inline vec3& operator*=(const float_t o);
	inline vec3& operator/=(const float_t o);

	inline float_t length() const { return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]); }
	inline float_t sqlength() const { return v[0] * v[0] + v[1] * v[1] + v[2] * v[2]; }
	inline void unit();

	operator std::string() const;

	float_t v[3];
};

inline static std::istream& operator>>(std::istream& is, vec3 v1) {
	is >> v1[0] >> v1[1] >> v1[2];
	return is;
};
inline static std::ostream& operator<<(std::ostream& os, vec3 v1) {
	os << v1[0] << ' ' << v1[1] << ' ' << v1[2];
	return os;
};
inline static vec3 operator+(const vec3& v1, const vec3& v2) {
	return vec3(v1.v[0] + v2.v[0], v1.v[1] + v2.v[1], v1.v[2] + v2.v[2]);
};
inline static vec3 operator-(const vec3& v1, const vec3& v2) {
	return vec3(v1.v[0] - v2.v[0], v1.v[1] - v2.v[1], v1.v[2] - v2.v[2]);
};
inline static vec3 operator*(const vec3& v1, const vec3& v2) {
	return vec3(v1.v[0] * v2.v[0], v1.v[1] * v2.v[1], v1.v[2] * v2.v[2]);
};
inline static vec3 operator/(const vec3& v1, const vec3& v2) {
	return vec3(v1.v[0] / v2.v[0], v1.v[1] / v2.v[1], v1.v[2] / v2.v[2]);
};
inline static vec3 operator+(const vec3& v1, const float_t o) {
	return vec3(v1.v[0] + o, v1.v[1] + o, v1.v[2] + o);
};
inline static vec3 operator-(const vec3& v1, const float_t o) {
	return vec3(v1.v[0] - o, v1.v[1] - o, v1.v[2] - o);
};
inline static vec3 operator*(const vec3& v1, const float_t o) {
	return vec3(v1.v[0] * o, v1.v[1] * o, v1.v[2] * o);
};
inline static vec3 operator/(const vec3& v1, const float_t o) {
	return vec3(v1.v[0] / o, v1.v[1] / o, v1.v[2] / o);
};
inline static vec3 operator+(const float_t o, const vec3& v1) {
	return vec3(v1.v[0] + o, v1.v[1] + o, v1.v[2] + o);
};
inline static vec3 operator-(const float_t o, const vec3& v1) {
	return vec3(o - v1.v[0], o - v1.v[1], o - v1.v[2]);
};
inline static vec3 operator*(const float_t o, const vec3& v1) {
	return vec3(v1.v[0] * o, v1.v[1] * o, v1.v[2] * o);
};
inline static vec3 operator/(const float_t o, const vec3& v1) {
	return vec3(o / v1.v[0], o / v1.v[1], o / v1.v[2]);
};

inline static float_t dot(const vec3& v1, const vec3& v2) {
	return v1.v[0] * v2.v[0] + v1.v[1] * v2.v[1] + v1.v[2] * v2.v[2];
};

inline static vec3 cross(const vec3& v1, const vec3& v2) {
	return vec3((v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1]), -(v1.v[0] * v2.v[2] - v1.v[2] * v2.v[0]), (v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0]));
};

inline static vec3 make_unit(vec3 v) {
	return v / v.length();
}

inline vec3& vec3::operator+=(const vec3& v2)
{
	v[0] += v2.v[0];
	v[1] += v2.v[1];
	v[2] += v2.v[2];
	return *this;
}

inline vec3& vec3::operator-=(const vec3& v2)
{
	v[0] -= v2.v[0];
	v[1] -= v2.v[1];
	v[2] -= v2.v[2];
	return *this;
}

inline vec3& vec3::operator*=(const vec3& v2)
{
	v[0] *= v2.v[0];
	v[1] *= v2.v[1];
	v[2] *= v2.v[2];
	return *this;
}

inline vec3& vec3::operator/=(const vec3& v2)
{
	v[0] /= v2.v[0];
	v[1] /= v2.v[1];
	v[2] /= v2.v[2];
	return *this;
}

inline vec3& vec3::operator+=(const float_t o)
{
	v[0] += o;
	v[1] += o;
	v[2] += o;
	return *this;
}

inline vec3& vec3::operator-=(const float_t o)
{
	v[0] -= o;
	v[1] -= o;
	v[2] -= o;
	return *this;
}

inline vec3& vec3::operator*=(const float_t o)
{
	v[0] *= o;
	v[1] *= o;
	v[2] *= o;
	return *this;
}

inline vec3& vec3::operator/=(const float_t o)
{
	v[0] /= o;
	v[1] /= o;
	v[2] /= o;
	return *this;
}

inline void vec3::unit()
{
	const float_t l = 1.0 / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] *= l;v[1] *= l;v[2] *= l;
}

inline vec3 reflect(const vec3& v, const vec3 n) {
	return v - 2 * dot(v, n) * n;
}

bool refract(const vec3& v, const vec3& n, const float_t ioroverior, vec3& refract);
float_t schlick(float_t cosv, float_t ior);

/*
inline static std::istream& operator>>(std::istream& is, vec3& v1);
inline static std::ostream& operator<<(std::ostream& os, vec3& v1);
inline static vec3 operator+(const vec3& v1, const vec3& v2);
inline static vec3 operator-(const vec3& v1, const vec3& v2);
inline static vec3 operator*(const vec3& v1, const vec3& v2);
inline static vec3 operator/(const vec3& v1, const vec3& v2);
inline static vec3 operator+(const vec3& v1, const float o);
inline static vec3 operator-(const vec3& v1, const float o);
inline static vec3 operator*(const vec3& v1, const float o);
inline static vec3 operator/(const vec3& v1, const float o);
inline static vec3 operator+(const float o, const vec3& v1);
inline static vec3 operator-(const float o, const vec3& v1);
inline static vec3 operator*(const float o, const vec3& v1);
inline static vec3 operator/(const float o, const vec3& v1);
inline static float dot(const vec3& v1, const vec3& v2);
inline static vec3 cross(const vec3& v1, const vec3& v2);
inline static vec3 make_unit(vec3 v);
*/