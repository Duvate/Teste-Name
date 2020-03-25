#include "Pch.hpp"
#include "Shader.hpp"

void Shader::init(const std::string & vertexPath, const std::string & fragmentPath)
{
	std::string shaderName = vertexPath.substr(0, vertexPath.find('.'));
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char * fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);

	glLinkProgram(program);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::terminate()
{
	glDeleteProgram(program);
}

void Shader::use() const
{
	glUseProgram(program);
}

void Shader::setInt(const  char * name, int value) const
{
	glUniform1i(glGetUniformLocation(program, name), value);
}

void Shader::setFloat(const char * name, float value) const
{
	glUniform1f(glGetUniformLocation(program, name), value);
}

void Shader::setVec3(const char * name, const vec3 & value) const
{
	glUniform3fv(glGetUniformLocation(program, name), 1, &value.x);
}

void Shader::setMat4(const char * name, const mat4 & mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_TRUE, &mat.mv[0]);
}