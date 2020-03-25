#pragma once
#include "DefaultType.hpp"
#include "Vec4.hpp"

template <typename T>
class vec3_t;

/*
Minimal class for this application
*/

template <typename T>
class mat4_t
{
public:
	union
	{
		struct
		{
			T mv[16];
		};
		struct
		{
			vec4_t<T> v[4];
		};
	};
	explicit mat4_t();
	explicit mat4_t(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9, T v10, T v11, T v12, T v13, T v14, T v15);
	explicit mat4_t(const vec4_t<T> & v0, const vec4_t<T> & v1, const vec4_t<T> & v2, const vec4_t<T> & v3);

	T & operator() (int i, int j);
	mat4_t operator* (const mat4_t & r) const;
};

template <typename T>
mat4_t<T> translate(const mat4_t<T> & m, const vec3_t<T> & v);

template <typename T>
mat4_t<T> lookAt(const vec3_t<T> & position, const vec3_t<T> & target, const vec3_t<T> & up);

// http://www.songho.ca/opengl/gl_projectionmatrix.html
template <typename T>
mat4_t<T> perspective(T fovy, T aspect, T zNear, T zFar);

typedef mat4_t<DefaultType> mat4;
#include "Mat4.inl"