#include "Pch.hpp"
#include "Skybox.hpp"

#include "ShaderManager.hpp"
#include "TextureManager.hpp"
#include "GlobalVariables.hpp"
#include "GlobalMatrices.hpp"

static GLuint VAO, VBO;
static GLuint cubeMapTexture;
static std::vector<std::string> faces = {
		"res/textures/skybox/right.png",
		"res/textures/skybox/left.png",
		"res/textures/skybox/top.png",
		"res/textures/skybox/bottom.png",
		"res/textures/skybox/front.png",
		"res/textures/skybox/back.png"
};

void Skybox::Init()
{
	float vertices[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	cubeMapTexture = TextureManager::loadCubemap(faces);
}

void Skybox::Terminate()
{
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Skybox::draw()
{
	if (wireframeMode) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDepthFunc(GL_LEQUAL);

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);

	shaderManager->skybox.use();
	shaderManager->skybox.setMat4("projection", projectionMatrix);
	shaderManager->skybox.setMat4("view", viewMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDepthFunc(GL_LESS); // set depth function back to default

	if (wireframeMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}