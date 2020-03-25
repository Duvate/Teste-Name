#include <cmath>

template <typename T>
vec3_t<T>::vec3_t() :
	x(0),
	y(0),
	z(0)
{
}

template <typename T>
vec3_t<T>::vec3_t(T x, T y, T z) :
	x(x),
	y(y),
	z(z)
{
}

template <typename T>
vec3_t<T> vec3_t<T>::operator+(const vec3_t<T>& r) const
{
	return vec3_t<T>(x + r.x, y + r.y, z + r.z);
}

template <typename T>
vec3_t<T> vec3_t<T>::operator-(const vec3_t<T>& v) const
{
	return vec3_t<T>(x - v.x, y - v.y, z - v.z);
}

template <typename T>
T dot(const vec3_t<T>& l, const vec3_t<T>& r)
{
	return l.x * r.x + l.y * r.y + l.z * r.z;
}

template <typename T>
vec3_t<T> normalize(const vec3_t<T>& v)
{
	T f = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	return vec3_t<T>(v.x / f, v.y / f, v.z / f);
}

template <typename T>
vec3_t<T> cross(const vec3_t<T>& v0, const vec3_t<T>& v1)
{
	return vec3_t<T>(v0.y * v1.z - v0.z * v1.y, v0.z * v1.x - v0.x * v1.z, v0.x * v1.y - v0.y * v1.x);
}