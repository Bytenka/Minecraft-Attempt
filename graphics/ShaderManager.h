#pragma once

#define RESOURCES_PATH "resources/"

#include "../graphics/Shader.h"

#include <memory>
#include <string>
#include <map>

namespace tk
{
typedef std::shared_ptr<Shader> ShaderPtr;

struct ShaderManager
{
	static ShaderManager &getInstance() noexcept
	{
		static ShaderManager instance;
		return instance;
	}

	ShaderPtr load(const std::string &vertexName, const std::string &fragmentName) noexcept
	{
		std::pair<std::string, std::string> pair = std::make_pair(RESOURCES_PATH + vertexName, RESOURCES_PATH + fragmentName);

		auto iterator = m_loadedShaders.find(pair);
		if (iterator != m_loadedShaders.end())
		{
			return iterator->second;
		}

		ShaderPtr newShader = std::make_shared<Shader>(RESOURCES_PATH + vertexName, RESOURCES_PATH + fragmentName);
		m_loadedShaders.insert(std::make_pair(pair, newShader));

		return newShader;
	}

	inline void clearCache() noexcept { m_loadedShaders.clear(); }

  private:
	ShaderManager(const ShaderManager &) = delete;
	void operator=(const ShaderManager &) = delete;
	ShaderManager() = default;
	~ShaderManager() = default;

	std::map<std::pair<std::string, std::string>, ShaderPtr> m_loadedShaders;
};
} // namespace tk