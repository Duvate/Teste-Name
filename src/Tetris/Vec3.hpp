#pragma once
/* Vectors/Matrices abstraction
 * Desc: this is a minimal vector class targeted for this project
 * -> Notice: mat4 class returns matrices at "Row Major" (not the way OpenGL expects it), which means you need to
 * set GL_TRUE as the 3rd argument of glUniformMatrix4fv to tell OpenGL to transpose it,
 * or transpose it manually with the transpose method and set GL_FALSE
 * -> Some things were adapted from GLM's source code
 */
#include "DefaultType.hpp"

/*
Minimal class for this application
*/
template <typename T>
class vec3_t
{
public:
	T x, y, z;
	explicit vec3_t();
	explicit vec3_t(T x, T y, T z);
	vec3_t operator+ (const vec3_t & r) const;
	vec3_t operator- (const vec3_t& v) const;
};

template <typename T>
T dot(const vec3_t<T>& l, const vec3_t<T>& r);

template <typename T>
vec3_t<T> normalize(const vec3_t<T>& v);

template <typename T>
vec3_t<T> cross(const vec3_t<T>& v0, const vec3_t<T>& v1);

typedef vec3_t<DefaultType> vec3;

#include "Vec3.inl"