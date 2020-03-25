#include "Pch.hpp"
#include "ShaderManager.hpp"

std::unique_ptr<ShaderManager> shaderManager = std::make_unique<ShaderManager>();

void ShaderManager::init()
{
	cube.init("res/shaders/cube.vert", "res/shaders/cube.frag");

	screen.init("res/shaders/screen.vert", "res/shaders/screen.frag");

	skybox.init("res/shaders/skybox.vert", "res/shaders/skybox.frag");
	skybox.use();
	skybox.setInt("albedoMap", 0);

	table.init("res/shaders/table.vert", "res/shaders/table.frag");
}

void ShaderManager::terminate()
{
	cube.terminate();
	screen.terminate();
	skybox.terminate();
	table.terminate();
}
