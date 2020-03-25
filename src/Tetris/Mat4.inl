#include "Vec3.hpp"

template <typename T>
mat4_t<T>::mat4_t()
{
	mv[0] = 1;
	mv[1] = 0;
	mv[2] = 0;
	mv[3] = 0;

	mv[4] = 0;
	mv[5] = 1;
	mv[6] = 0;
	mv[7] = 0;

	mv[8] = 0;
	mv[9] = 0;
	mv[10] = 1;
	mv[11] = 0;

	mv[12] = 0;
	mv[13] = 0;
	mv[14] = 0;
	mv[15] = 1;
}

template <typename T>
mat4_t<T>::mat4_t(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9, T v10, T v11, T v12, T v13, T v14, T v15)
{
	mv[0] = v0;
	mv[1] = v1;
	mv[2] = v2;
	mv[3] = v3;
	mv[4] = v4;
	mv[5] = v5;
	mv[6] = v6;
	mv[7] = v7;
	mv[8] = v8;
	mv[9] = v9;
	mv[10] = v10;
	mv[11] = v11;
	mv[12] = v12;
	mv[13] = v13;
	mv[14] = v14;
	mv[15] = v15;
}

template <typename T>
mat4_t<T>::mat4_t(const vec4_t<T> & v0, const vec4_t<T> & v1, const vec4_t<T> & v2, const vec4_t<T> & v3)
{
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;
	v[3] = v3;
}

template <typename T>
T & mat4_t<T>::operator()(int i, int j)
{
	return mv[i * 4 + j];
}

template <typename T>
mat4_t<T> mat4_t<T>::operator*(const mat4_t<T> & r) const
{
	vec4_t<T> l0(mv[0] * r.mv[0] + mv[1] * r.mv[4] + mv[2] * r.mv[8] + mv[3] * r.mv[12],
		mv[0] * r.mv[1] + mv[1] * r.mv[5] + mv[2] * r.mv[9] + mv[3] * r.mv[13],
		mv[0] * r.mv[2] + mv[1] * r.mv[6] + mv[2] * r.mv[10] + mv[3] * r.mv[14],
		mv[0] * r.mv[3] + mv[1] * r.mv[7] + mv[2] * r.mv[11] + mv[3] * r.mv[15]);

	vec4_t<T> l1(mv[4] * r.mv[0] + mv[5] * r.mv[4] + mv[6] * r.mv[8] + mv[7] * r.mv[12],
		mv[4] * r.mv[1] + mv[5] * r.mv[5] + mv[6] * r.mv[9] + mv[7] * r.mv[13],
		mv[4] * r.mv[2] + mv[5] * r.mv[6] + mv[6] * r.mv[10] + mv[7] * r.mv[14],
		mv[4] * r.mv[3] + mv[5] * r.mv[7] + mv[6] * r.mv[11] + mv[7] * r.mv[15]);

	vec4_t<T> l2(mv[8] * r.mv[0] + mv[9] * r.mv[4] + mv[10] * r.mv[8] + mv[11] * r.mv[12],
		mv[8] * r.mv[1] + mv[9] * r.mv[5] + mv[10] * r.mv[9] + mv[11] * r.mv[13],
		mv[8] * r.mv[2] + mv[9] * r.mv[6] + mv[10] * r.mv[10] + mv[11] * r.mv[14],
		mv[8] * r.mv[3] + mv[9] * r.mv[7] + mv[10] * r.mv[11] + mv[11] * r.mv[15]);

	vec4_t<T> l3(mv[12] * r.mv[0] + mv[13] * r.mv[4] + mv[14] * r.mv[8] + mv[15] * r.mv[12],
		mv[12] * r.mv[1] + mv[13] * r.mv[5] + mv[14] * r.mv[9] + mv[15] * r.mv[13],
		mv[12] * r.mv[2] + mv[13] * r.mv[6] + mv[14] * r.mv[10] + mv[15] * r.mv[14],
		mv[12] * r.mv[3] + mv[13] * r.mv[7] + mv[14] * r.mv[11] + mv[15] * r.mv[15]);

	return mat4_t<T>(l0, l1, l2, l3);
}

template <typename T>
mat4_t<T> translate(const mat4_t<T> & m, const vec3_t<T> & v)
{
	mat4_t<T> transform_m(1, 0, 0, v.x,
		0, 1, 0, v.y,
		0, 0, 1, v.z,
		0, 0, 0, 1);

	return m * transform_m;
}

template <typename T>
mat4_t<T> lookAt(const vec3_t<T> & position, const vec3_t<T> & target, const vec3_t<T> & up)
{
	vec3_t<T> f(normalize(target - position));
	vec3_t<T> s(normalize(cross(f, up)));
	vec3_t<T> u(cross(s, f));
	mat4_t<T> m;
	m(0, 0) = s.x;
	m(0, 1) = s.y;
	m(0, 2) = s.z;
	m(0, 3) = -dot(s, position);
	m(1, 0) = u.x;
	m(1, 1) = u.y;
	m(1, 2) = u.z;
	m(1, 3) = -dot(u, position);
	m(2, 0) = -f.x;
	m(2, 1) = -f.y;
	m(2, 2) = -f.z;
	m(2, 3) = dot(f, position);
	return m;
}

template <typename T>
mat4_t<T> perspective(T fovy, T aspect, T zNear, T zFar)
{
	const T tanHalfFovy = tan(fovy / T(2));
	mat4_t<T> m(T(1.0) / (aspect * tanHalfFovy), 0, 0, 0,
		0, T(1.0) / tanHalfFovy, 0, 0,
		0, 0, -(zFar + zNear) / (zFar - zNear), -(T(2) * zFar * zNear) / (zFar - zNear),
		0, 0, -1, 0);

	return m;
}