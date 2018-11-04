#define RESOURCES_PATH "resources/"

#include "TextureAtlas.h"
#include "../utils/Logging.h"
#include "../utils/Exceptions.h"

namespace tk
{
TextureAtlas::TextureAtlas()
{
}

TextureAtlas::~TextureAtlas() noexcept
{
	if (m_isAtlasLoaded)
		glDeleteTextures(1, &m_atlasID);
}

// public:

void TextureAtlas::init()
{
	try
	{
		loadAtlas("textures/blocks/atlas.png");
		loadTexCoords();
	}
	catch (RuntimeException &e)
	{
		e.pushCurrentContext(__FUNCTION__);
		throw;
	}
}

std::array<GLfloat, 8> TextureAtlas::getTextureCoords(const std::string &textureName)
{
	// @TODO make a cache for the function
	try
	{
		if (!m_areCoordsLoaded)
			throw RuntimeException(__FUNCTION__, "Texture coordinates are not loaded! As init() been called?..");

		auto iterator = m_textures.find(textureName);
		if (iterator != m_textures.end())
		{
			return dataToGL(iterator->second);
		}
		else
		{												// @TODO generate invalid texture
			return getTextureCoords("invalid_texture"); // Kind of unsafe
		}
	}
	catch (RuntimeException &e)
	{
		e.pushCurrentContext(__FUNCTION__);
		throw;
	}
}

// private:

void TextureAtlas::loadAtlas(const std::string &path)
{
	try
	{
		if (!m_isAtlasLoaded)
		{
			m_atlas = std::make_unique<Image>(RESOURCES_PATH + path);

			glGenTextures(1, &m_atlasID);
			glBindTexture(GL_TEXTURE_2D, m_atlasID);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA,
				m_atlas->getWidth(),
				m_atlas->getWidth(),
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				m_atlas->getData());

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glGenerateMipmap(GL_TEXTURE_2D);

			m_isAtlasLoaded = true;
		}
	}
	catch (RuntimeException &e)
	{
		e.pushCurrentContext(__FUNCTION__);
		throw;
	}
}

void TextureAtlas::loadTexCoords() noexcept
{
	// @TODO Alexa, change that
	TextureData t = {glm::ivec2(128, 0), 16};
	m_textures.insert(m_textures.end(), std::make_pair("tnt_side", t));
	t = {glm::ivec2(144, 0), 16};
	m_textures.insert(m_textures.end(), std::make_pair("tnt_top", t));
	t = {glm::ivec2(160, 0), 16};
	m_textures.insert(m_textures.end(), std::make_pair("tnt_bottom", t));
	t = {glm::ivec2(192, 192), 16};
	m_textures.insert(m_textures.end(), std::make_pair("grass_top", t));
	t = {glm::ivec2(32, 0), 16};
	m_textures.insert(m_textures.end(), std::make_pair("dirt", t));
	t = {glm::ivec2(48, 0), 16};
	m_textures.insert(m_textures.end(), std::make_pair("grass_side", t));
	t = {glm::ivec2(32, 48), 16};
	m_textures.insert(m_textures.end(), std::make_pair("diamond_ore", t));
	t = {glm::ivec2(80, 32), 16};
	m_textures.insert(m_textures.end(), std::make_pair("obsidian", t));
	t = {glm::ivec2(0, 16), 16};
	m_textures.insert(m_textures.end(), std::make_pair("cobble_stone", t));
	t = {glm::ivec2(16, 16), 16};
	m_textures.insert(m_textures.end(), std::make_pair("bedrock", t));
	t = {glm::ivec2(16, 0), 16};
	m_textures.insert(m_textures.end(), std::make_pair("stone", t));

	t = {glm::ivec2(208, 240), 16};
	m_textures.insert(m_textures.end(), std::make_pair("invalid_texture", t)); // This one MUST exist

	m_areCoordsLoaded = true;
}

std::array<GLfloat, 8> TextureAtlas::dataToGL(const TextureData &data) noexcept
{
	// Get the texture data of a block, using TextureData
	std::array<GLfloat, 2> bottomLeft = toNormalizedCoordinates({data.topLeft.x, data.topLeft.y + data.sideSize});
	std::array<GLfloat, 2> bottomRight = toNormalizedCoordinates({data.topLeft.x + data.sideSize, data.topLeft.y + data.sideSize});
	std::array<GLfloat, 2> topRight = toNormalizedCoordinates({data.topLeft.x + data.sideSize, data.topLeft.y});
	std::array<GLfloat, 2> topLeft = toNormalizedCoordinates(data.topLeft);

	std::array<GLfloat, 8> result = {
		bottomLeft[0], bottomLeft[1],
		bottomRight[0], bottomRight[1],
		topRight[0], topRight[1],
		topLeft[0], topLeft[1]};

	return result;
}

std::array<GLfloat, 2> TextureAtlas::toNormalizedCoordinates(const glm::ivec2 &pixelCoordinates) noexcept
{
	// This converts pixel coordinates from the atlas to normalized coordinates that OpenGL can use
	return {pixelCoordinates.x / (float)m_atlas->getWidth(), pixelCoordinates.y / (float)m_atlas->getHeight()};
}
} // namespace tk