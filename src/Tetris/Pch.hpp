#pragma once

// Precompiled header to speed-up compilations

// My Maths library
#include "Maths.hpp"

// OpenGL API
#include <glad/glad.h>

// STL
#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <random>
#include <sstream>
#include <vector>

// SFML
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#ifndef _WIN32
// Welcome to the collection of workarounds to make this code compilable with g++ -std=c++11
#include <cmath>

// Visual Studio 2017 Compiler has std::make_unique, while g++ -std=c++11 doesn't
// Adapted from https://stackoverflow.com/questions/24609271/errormake-unique-is-not-a-member-of-std
namespace std
{
	template<typename T, typename... Args>
	unique_ptr<T> make_unique(Args&&... args)
	{
		return unique_ptr<T>(new T(forward<Args>(args)...));
	}
}
#endif