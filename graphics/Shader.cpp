#include "Shader.h"

#include "../utils/glutils.h"
#include "../utils/Logging.h"

#include "../utils/Exceptions.h"

namespace tk
{
Shader::Shader(const std::string &vertexPath, const std::string &fragmenPath)
	: m_shaderProgram(0)
{
	setVertexShader(vertexPath);
	setFragmentShader(fragmenPath);

	try
	{
		createShader();
	}
	catch (RuntimeException &e)
	{
		e.pushCurrentContext(__FUNCTION__, "class Shader");
	}
}

Shader::~Shader() noexcept
{
	glDeleteProgram(m_shaderProgram);
	flushUniformCache();
}

void Shader::compile()
{
	try
	{
		createVertex();
		createFragment();

		createProgram();

		deleteVertex();
		deleteFragment();
	}
	catch (RuntimeException &e)
	{
		e.pushCurrentContext(__FUNCTION__);
		throw;
	}
}

void Shader::createVertex()
{
	m_vertexShader = glCreateShader(GL_VERTEX_SHADER);

	std::string vertexShaderSourceString;
	try
	{
		vertexShaderSourceString = read_file(m_vertPath.c_str());
	}
	catch (RuntimeException &e)
	{
		e.pushCurrentContext(__FUNCTION__);
		throw;
	}
	const char *vertexShaderSource = vertexShaderSourceString.c_str();

	glShaderSource(m_vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(m_vertexShader);

	// Error checking
	int success;
	glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(m_vertexShader, 512, NULL, infoLog);
		throw RuntimeException(__FUNCTION__, "Vertex shader compilation error: " + std::to_string(*infoLog));
	}
}

void Shader::deleteVertex() noexcept
{
	glDeleteShader(m_vertexShader);
}

void Shader::createFragment()
{
	m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string fragmentShaderSourceString;
	try
	{
		fragmentShaderSourceString = read_file(m_fragPath.c_str());
	}
	catch (RuntimeException &e)
	{
		e.pushCurrentContext(__FUNCTION__);
		throw;
	}

	const char *fragmentShaderSource = fragmentShaderSourceString.c_str();

	glShaderSource(m_fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(m_fragmentShader);

	// Error checking
	int success;
	glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(m_fragmentShader, 512, NULL, infoLog);
		throw RuntimeException(__FUNCTION__, "Fragment shader compilation error: " + std::to_string(*infoLog));
	}
}

void Shader::deleteFragment() noexcept
{
	glDeleteShader(m_fragmentShader);
}

void Shader::createProgram()
{
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, m_vertexShader);
	glAttachShader(m_shaderProgram, m_fragmentShader);
	glLinkProgram(m_shaderProgram);

	// Error checking
	int success;
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
		throw RuntimeException(__FUNCTION__, "Shader linkage process failed: " + std::to_string(*infoLog));
	}
}

GLint Shader::getUniformLocation(const char *uniformName) noexcept
{
	for (unsigned i = 0; i < uniformCacheNames.size(); i++)
	{
		if (!strcmp(uniformCacheNames.at(i), uniformName))
			return uniformCacheValues.at(i);
	}

	char *newName = new char[strlen(uniformName) + 1]; // Memory is freed when calling flushUniformCache, which is also called by the destructor
	strcpy(newName, uniformName);
	uniformCacheNames.push_back(newName);

	GLint newValue = glGetUniformLocation(m_shaderProgram, uniformName);
	uniformCacheValues.push_back(newValue);

	return newValue;
}

void Shader::setUniform1i(const char *uniformName, int i) noexcept
{
	enable();
	GLint uniformLocation = getUniformLocation(uniformName);
	glUniform1i(uniformLocation, i);
}

void Shader::setUniform1f(const char *uniformName, float f) noexcept
{
	enable();
	GLint uniformLocation = getUniformLocation(uniformName);
	glUniform1f(uniformLocation, f);
}

void Shader::setUniform2f(const char *uniformName, float x, float y) noexcept
{
	enable();
	GLint uniformLocation = getUniformLocation(uniformName);
	glUniform2f(uniformLocation, x, y);
}

void Shader::setUniform3f(const char *uniformName, float x, float y, float z) noexcept
{
	enable();
	GLint uniformLocation = getUniformLocation(uniformName);
	glUniform3f(uniformLocation, x, y, z);
}

void Shader::setUniform4f(const char *uniformName, float x, float y, float z, float w) noexcept
{
	enable();
	GLint uniformLocation = getUniformLocation(uniformName);
	glUniform4f(uniformLocation, x, y, z, w);
}

void Shader::setUniformMatrix4fv(const char *uniformName, const glm::mat4 &transform) noexcept
{
	enable();
	GLint uniformLocation = getUniformLocation(uniformName);
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(transform));
}

void Shader::createShader()
{
	if (m_shaderProgram != 0)
	{
		LOG_WARN("Warning: Deleting old shader to create new one");
		flushUniformCache();
		glDeleteProgram(m_shaderProgram);
	}

	if (m_vertPath.empty())
	{
		throw RuntimeException(__FUNCTION__, "Vertex shader path not set, can't generate shader!");
	}

	if (m_fragPath.empty())
	{
		throw RuntimeException(__FUNCTION__, "Fragment shader path not set, can't generate shader!");
	}

	try
	{
		compile();
	}
	catch (RuntimeException &e)
	{
		e.pushCurrentContext(__FUNCTION__);
		throw;
	}
}

void Shader::flushUniformCache() noexcept
{
	for (int i = uniformCacheNames.size() - 1; i >= 0; i--)
	{
		delete uniformCacheNames.at(i);
	}

	uniformCacheNames.clear();
	uniformCacheValues.clear();
}
} // namespace tk