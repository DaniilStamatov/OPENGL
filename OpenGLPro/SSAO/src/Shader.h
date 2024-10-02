#pragma once
#include <unordered_map>
#include <string>
#include "Renderer.h"
class Shader
{
private:
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocations;
public:
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string&  geometryShaderPath/* = "" */);
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
		: Shader(vertexShaderPath, fragmentShaderPath, "") {}
	~Shader();
	void Bind() const;
	void Unbind() const;
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform1f(const std::string& name, float v);
	void SetUniform1i(const std::string& name, int v);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	void SetVector3f(const std::string& name, float v0, float v1, float v2);
	void SetVector3f(const std::string& name, const glm::vec3& vector);

private:
	unsigned int CompileShader(unsigned int type, const std::string& source);
	int GetUniformLocation(const std::string& name);

};