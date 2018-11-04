#pragma once
#include "Image.h"

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <array>
#include <map>
#include <memory>
#include <string>

namespace tk
{
struct TextureData
{
	glm::ivec2 topLeft;
	unsigned sideSize;
};

class TextureAtlas
{
  public:
	static TextureAtlas &getInstance() noexcept
	{
		static TextureAtlas instance;
		return instance;
	}

	void init();
	std::array<GLfloat, 8> getTextureCoords(const std::string &textureName);
	inline GLuint getAtlasID() const noexcept { return m_atlasID; }

  private:
	void loadAtlas(const std::string &path);
	void loadTexCoords() noexcept;
	std::array<GLfloat, 8> dataToGL(const TextureData &data) noexcept;
	std::array<GLfloat, 2> toNormalizedCoordinates(const glm::ivec2 &pixelCoordinates) noexcept;

	GLuint m_atlasID;
	std::unique_ptr<Image> m_atlas;
	std::map<std::string, TextureData> m_textures;
	std::array<GLfloat, 8> m_invalidTexture;

	bool m_isAtlasLoaded = false;
	bool m_areCoordsLoaded = false;

  private:
	TextureAtlas();
	~TextureAtlas();
	TextureAtlas(const TextureAtlas &) = delete;
	void operator=(const TextureAtlas &) = delete;
};

} // namespace tk