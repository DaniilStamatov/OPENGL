#include "shader.h"	
#include <fstream>
#include <iostream>
#include <glad/glad.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glfw3.lib")
static std::string ReadFileAsString(const std::string& filepath)
{
	std::string result;
	std::ifstream in(filepath, std::ios::in | std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);
		result.resize((size_t)in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&result[0], result.size());
		in.close();
	}
	else
	{
		std::cout << "Warning"<< std::endl;
	}

	return result;
}

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath/* = "" */) : m_RendererID(0)
{
	std::string vertexSource = ReadFileAsString(vertexShaderPath);
	std::string fragmentSource = ReadFileAsString(fragmentShaderPath);
	std::string geometrySource;
	if (!geometryShaderPath.empty()) {
		geometrySource = ReadFileAsString(geometryShaderPath);
	}


	unsigned int program = glCreateProgram();
	unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
	glAttachShader(program, vertexShader);
	unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
	glAttachShader(program, fragmentShader);
	unsigned int geometryShader;
	if (!geometryShaderPath.empty()) {
		geometryShader = CompileShader(GL_GEOMETRY_SHADER, geometrySource);
		glAttachShader(program, geometryShader);
	}
	glLinkProgram(program);
	int result;
	glGetShaderiv(program, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(program, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(program, length, &length, message);
		std::cout << message << std::endl;
	}

	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
	if (!geometryShaderPath.empty()) {
		glDetachShader(program, geometryShader);
		glDeleteShader(geometryShader);
	}
	glDeleteProgram(vertexShader);
	glDeleteShader(fragmentShader);

	m_RendererID = program;
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniform1f(const std::string& name, float v)
{
	glUniform1f(GetUniformLocation(name), v);
}

void Shader::SetUniform1i(const std::string& name, int v)
{
	glUniform1i(GetUniformLocation(name), v);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);//3d param-transpose - swap columns and rows
}

void Shader::SetVector3f(const std::string& name, float v0, float v1, float v2)
{
	glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetVector3f(const std::string& name, const glm::vec3& vector)
{
	glUniform3fv(GetUniformLocation(name),1, &vector[0]);
}


unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int shader;
	shader = glCreateShader(type);

	const GLchar* sourceCstr = source.c_str();
	glShaderSource(shader, 1, &sourceCstr, 0);

	glCompileShader(shader);
	auto isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		int length;
		glGetShaderiv(isCompiled, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)_malloca(length * sizeof(char));
		glGetShaderInfoLog(isCompiled, length, &length, message);
		std::cout <<"SHADER ERROR"<< message << std::endl;
		return 0;
	}

	return shader;
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocations.find(name) != m_UniformLocations.end())
		return m_UniformLocations[name];

	int location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1)
	{
		std::cout << "Warning:" << name << std::endl;
	}

	m_UniformLocations[name] = location;
	return location;
}
