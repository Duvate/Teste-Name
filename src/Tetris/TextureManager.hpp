#pragma once

struct TextureManager
{
	// RAW OpenGL textures
	static unsigned int loadTexture(const char * path);
	
	// Order of faces: right/left/top/bottom/front/back
	static unsigned int loadCubemap(const std::vector<std::string> & faces);
	
	static void OpenGLTerminateLoadedTextures();
};