#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace tk
{
class Shader
{
public:
	Shader(const std::string &vertexPath, const std::string &fragmentPath);
	~Shader() noexcept;

	void setUniform1i(const char *uniformName, int i) noexcept;
	void setUniform1f(const char *uniformName, float f) noexcept;
	void setUniform2f(const char *uniformName, float x, float y) noexcept;
	void setUniform3f(const char *uniformName, float x, float y, float z) noexcept;
	void setUniform4f(const char *uniformName, float x, float y, float z, float w) noexcept;
	void setUniformMatrix4fv(const char *uniformName, const glm::mat4 &transform) noexcept;
	void createShader(); // Can be used for overwriting
	void flushUniformCache() noexcept;

	inline const GLuint &getProgram() const noexcept { return m_shaderProgram; }
	inline void enable() const noexcept { glUseProgram(m_shaderProgram); }
	inline void disable() const noexcept { glUseProgram(0); }
	inline void setVertexShader(std::string vertexPath) noexcept { m_vertPath = vertexPath; }
	inline void setFragmentShader(std::string fragmentPath) noexcept { m_fragPath = fragmentPath; }

private:
	void compile();
	void createVertex();
	void deleteVertex() noexcept;
	void createFragment();
	void deleteFragment() noexcept;
	void createProgram();
	GLint getUniformLocation(const char *uniformName) noexcept;

	std::string m_vertPath;
	std::string m_fragPath;

	// For caching because glGetUniformLocation is slow operation
	std::vector<char *> uniformCacheNames;
	std::vector<GLint> uniformCacheValues;

	unsigned m_vertexShader, m_fragmentShader, m_shaderProgram;

private:
	Shader(const Shader &) = delete;
	void operator=(const Shader &) = delete;
};

} // namespace tk