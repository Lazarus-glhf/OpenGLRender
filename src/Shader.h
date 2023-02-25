#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set uniforms
	void SetUniform1i(const std::string name, int value) const;
	void SetUniform1f(const std::string name, float value) const;
	void SetUniform2f(const std::string name, glm::vec2 value) const;
	void SetUniform3f(const std::string name, glm::vec3 value) const;
	void SetUniform4f(const std::string name, glm::vec4 value) const;

	void SetUniformMat3f(const std::string name, const glm::mat3& matrix) const;
	void SetUniformMat4f(const std::string name, const glm::mat4& matrix) const;

private:
	unsigned int CompileShader(unsigned int type, const std::string& source);

	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	ShaderProgramSource ParseShader(const std::string& filePath);

	int GetUniformLocation(const std::string& name) const;
};