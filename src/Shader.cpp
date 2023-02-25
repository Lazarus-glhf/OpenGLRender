#include "Shader.h"
#include "Renderer.h"

#include <iostream>
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string& filePath)
	:m_FilePath(filePath), m_RendererID(0)
{
	ShaderProgramSource source = ParseShader(filePath);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertexShader" : "fragmentShader") << "shader" << std::endl;
		std::cout << message << std::endl;

		glDeleteShader(id);

		return 0;
	}

	return id;
	return false;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

ShaderProgramSource Shader::ParseShader(const std::string& filePath)
{
	std::ifstream stream(filePath);

	enum class Shadertype
	{
		NONE = -1, VERTEX = 0, FRAGEMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	Shadertype type = Shadertype::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = Shadertype::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = Shadertype::FRAGEMENT;
			}
		}
		else
		{
			ss[int(type)] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

void Shader::SetUniform1i(const std::string name, int value) const
{
	GLint location = GetUniformLocation(name);
	GLCall(glUniform1i(location, value));
}

void Shader::SetUniform1f(const std::string name, float value) const
{
	GLint location = GetUniformLocation(name);
	GLCall(glUniform1i(location, value));
}

void Shader::SetUniform2f(const std::string name, glm::vec2 value) const
{
	GLint location = GetUniformLocation(name);
	GLCall(glUniform2f(location, value.x, value.y));
}

void Shader::SetUniform3f(const std::string name, glm::vec3 value) const
{
	GLint location = GetUniformLocation(name);
	GLCall(glUniform3f(location, value.x, value.y, value.z));
}

void Shader::SetUniform4f(const std::string name, glm::vec4 value) const
{
	GLint location = GetUniformLocation(name);
	GLCall(glUniform4f(location, value.x, value.y, value.z, value.w));
}
 
void Shader::SetUniformMat3f(const std::string name, const glm::mat3& matrix) const
{
	GLint location = GetUniformLocation(name);
	GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniformMat4f(const std::string name, const glm::mat4& matrix) const
{
	GLint location = GetUniformLocation(name);
	GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name) const
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
	{
		return m_UniformLocationCache[name];
	}
	GLCall(unsigned int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
	{
		std::cout << "Warning: uniform " << name << " doesn't exist!" << std::endl;
	}
	m_UniformLocationCache[name] = location;

	return location;
}
