#include "Pch.hpp"
#include "Renderer.hpp"

#include "Window.hpp"
#include "Camera.hpp"
#include "TableDimensions.hpp"
#include "GlobalVariables.hpp"
#include "ShaderManager.hpp"
#include "GlobalMatrices.hpp"
#include "TextureManager.hpp"
#include "Skybox.hpp"
#include "UI.hpp"

/*
 * Specs:
 * GL_TEXTURE_0 will always be the fbo texture
 * GL_TEXTURE_1 will always be the tetris texture
 */

static GLuint cubeVAO;
static GLuint cubeVBO;
static GLuint cubeTexture;
static GLuint quadVAO;
static GLuint quadVBO;
static GLuint FBO;
static GLuint textureColorBuffer;
static GLuint RBO;
static GLuint tableSquareVAO;
static GLuint tableSquareVBO;

void Renderer::RenderSFML()
{
	// handling SFML with custom OpenGL
	glDisable(GL_DEPTH_TEST);

	if (wireframeMode) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // We don't want SFML stuff in wireframe

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glUseProgram(0);

	window->pushGLStates();
	window->resetGLStates();
	// ------------------------------------------------------------------------------
	
	ui->drawSFML();

	// ------------------------------------------------------------------------------
	window->popGLStates();

	if (wireframeMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_DEPTH_TEST);
}

bool Renderer::Init()
{
	if (!gladLoadGL())
	{
		std::cout << "Failed to load OpenGL!\n";
		return false;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shaderManager->init();
	Skybox::Init();

	// Cube
	float cubeVertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); // lPos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(1); // lNormal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2); // lTexCoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	cubeTexture = TextureManager::loadTexture("res/textures/tiles.png");
	shaderManager->cube.use();
	shaderManager->cube.setInt("tex", 1);

	// FBO boilerplate code
	float quadVertices[] = { 
	// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &textureColorBuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_RESOLUTION_WIDTH, SCREEN_RESOLUTION_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_RESOLUTION_WIDTH, SCREEN_RESOLUTION_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Table's small squares
	float tableSquareVertices[] = {
		// World coordinates of a square that will be manipulated in the shader to create the 3d table
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f
	};
	glGenVertexArrays(1, &tableSquareVAO);
	glGenBuffers(1, &tableSquareVBO);
	glBindVertexArray(tableSquareVAO);
	glBindBuffer(GL_ARRAY_BUFFER, tableSquareVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tableSquareVertices), tableSquareVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

	return true;
}

void Renderer::Terminate()
{
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
	glDeleteFramebuffers(1, &FBO);
	glDeleteTextures(1, &textureColorBuffer);
	glDeleteRenderbuffers(1, &RBO);
	glDeleteRenderbuffers(1, &tableSquareVAO);
	glDeleteRenderbuffers(1, &tableSquareVBO);
	TextureManager::OpenGLTerminateLoadedTextures();
	Skybox::Terminate();
	shaderManager->terminate();
}

void Renderer::PrepareForRendering()
{
	// Calculate projection and view matrices
	camera->update();
	projectionMatrix = perspective(radians(camera->zoom), static_cast<DefaultType>(WINDOW_WIDTH) / static_cast<DefaultType>(WINDOW_HEIGHT), DefaultType(0.01), DefaultType(viewDistance));
	viewMatrix = camera->getViewMatrix();

	if (wireframeMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Bind FBO
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glViewport(0, 0, SCREEN_RESOLUTION_WIDTH, SCREEN_RESOLUTION_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0.16f, 0.20f, 0.16f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void Renderer::PrepareForCubesDrawing()
{
	shaderManager->cube.use();
	shaderManager->cube.setMat4("projection", projectionMatrix);
	shaderManager->cube.setMat4("view", viewMatrix);
	shaderManager->cube.setVec3("viewPos", camera->pos);

	// Bind texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, cubeTexture);

	// Bind VAO for future glDraw calls
	glBindVertexArray(cubeVAO);
}

void Renderer::DrawCube(const sf::Vector3i & pos, TileColor color, float alpha)
{
	if (color == TileColor::None) return;
	mat4 model = translate(mat4(), vec3(static_cast<DefaultType>(pos.x), static_cast<DefaultType>(pos.y), static_cast<DefaultType>(pos.z)));
	shaderManager->cube.setMat4("model", model);
	shaderManager->cube.setFloat("alpha", alpha);
	shaderManager->cube.setInt("texIndex", static_cast<int>(color) - 1);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Renderer::DrawTableLayout()
{
	// Renders table in wireframe mode
	if (!wireframeMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(tableSquareVAO);
	shaderManager->table.use();
	shaderManager->table.setInt("width", width);
	shaderManager->table.setInt("depth", depth);
	shaderManager->table.setMat4("projection", projectionMatrix);
	shaderManager->table.setMat4("view", viewMatrix);

	// Renders bottom big square with stronger line width
	glLineWidth(3.0f);
	shaderManager->table.setInt("type", 0); // to scale in shader
	shaderManager->table.setVec3("offset", vec3(0, 0, 0));
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	
	// Renders all small squares now
	glLineWidth(1.0f);
	shaderManager->table.setInt("type", 1); // bottom / top
	for (float i = 0; i < width; i++)
		for (float j = 0; j < depth; j++)
		{
			shaderManager->table.setVec3("offset", vec3(i, 0.0f, j));
			glDrawArrays(GL_LINE_LOOP, 0, 4);
		}
	for (float i = 0; i < width; i++)
		for (float j = 0; j < depth; j++)
		{
			shaderManager->table.setVec3("offset", vec3(i, float(height), j));
			glDrawArrays(GL_LINE_LOOP, 0, 4);
		}
	shaderManager->table.setInt("type", 2); // left / right
	for (float i = 0; i < height; i++)
		for (float j = 0; j < depth; j++)
		{
			shaderManager->table.setVec3("offset", vec3(0.0f, i, j));
			glDrawArrays(GL_LINE_LOOP, 0, 4);
		}
	for (float i = 0; i < height; i++)
		for (float j = 0; j < depth; j++)
		{
			shaderManager->table.setVec3("offset", vec3(float(width), i, j));
			glDrawArrays(GL_LINE_LOOP, 0, 4);
		}
	shaderManager->table.setInt("type", 3); // front / back
	for (float i = 0; i < width; i++)
		for (float j = 0; j < height; j++)
		{
			shaderManager->table.setVec3("offset", vec3(i, j, 0));
			glDrawArrays(GL_LINE_LOOP, 0, 4);
		}
	for (float i = 0; i < width; i++)
		for (float j = 0; j < height; j++)
		{
			shaderManager->table.setVec3("offset", vec3(i, j, float(depth)));
			glDrawArrays(GL_LINE_LOOP, 0, 4);
		}

	// wireframe mode goes back to normal
	if (!wireframeMode) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::Display()
{
	Skybox::draw();

	// Default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	shaderManager->screen.use();
	shaderManager->screen.setInt("screenTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
	shaderManager->screen.setInt("WINDOW_HEIGHT", WINDOW_HEIGHT);
	shaderManager->screen.setInt("postProcessIndex", postProcessIndex);

	glBindVertexArray(quadVAO);

	// >>> We don't want FBO's texture's quad to be rendered in wireframe mode
	if (wireframeMode) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	if (wireframeMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// <<<

	RenderSFML();
	window->display();
}