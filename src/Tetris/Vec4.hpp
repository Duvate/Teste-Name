#pragma once
#include "DefaultType.hpp"

template <typename T>
class vec3_t;

/*
Minimal class for this application
*/
template <typename T>
class vec4_t
{
public:
	T x, y, z, w;
	explicit vec4_t(T x, T y, T z, T w);
};

typedef vec4_t<DefaultType> vec4;

#include "Vec4.inl"