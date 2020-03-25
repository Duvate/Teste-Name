#pragma once
#include "Shader.hpp"

struct ShaderManager
{
	Shader cube,
		screen,
		skybox,
		table;

	void init();
	void terminate();
};
extern std::unique_ptr<ShaderManager> shaderManager;