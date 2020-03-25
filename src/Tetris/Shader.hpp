#pragma once

/*
Minimal class for this application
*/

class Shader
{
	unsigned int program;

public:
	void init(const std::string & vertexPath, const std::string & fragmentPath);
	void terminate();
	void use() const;

	void setInt(const char * name, int value) const;
	void setFloat(const char * name, float value) const;

	void setVec3(const char * name, const vec3 & value) const;
	void setMat4(const char * name, const mat4 & mat) const;
};