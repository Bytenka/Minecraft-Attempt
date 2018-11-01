#include "ChunkMesh.h"
#include "WorldConstants.h"
#include "../graphics/TextureAtlas.h"

namespace tk
{
namespace Faces // Defines what a block should look like
{
const std::array<GLfloat, 12> top = {
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, 0.5f};
const std::array<GLfloat, 12> bottom = {
	-0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, 0.5f};
const std::array<GLfloat, 12> left = {
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, -0.5f};
const std::array<GLfloat, 12> right = {
	0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, 0.5f};
const std::array<GLfloat, 12> front = {
	-0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f};
const std::array<GLfloat, 12> back = {
	0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, -0.5f};

const std::array<std::array<GLfloat, 12>, 6> faces = {
	top,
	bottom,
	left,
	right,
	front,
	back};
} // namespace Faces

// public:

void ChunkMesh::addFace(BlockSide face, const Block &block, const glm::ivec3 &chunkPosition, const glm::ivec3 &blockPosition) noexcept
{
	auto tex = tk::TextureAtlas::getInstance().getTextureCoords(block.textures[face]);
	m_texCoords.insert(m_texCoords.end(), tex.begin(), tex.end());

	// 4 vertices to add per face, 3 coords per vertex
	for (unsigned i = 0; i < 4; i++)
	{
		unsigned index = 3 * i;
		m_verticesPos.push_back(Faces::faces[face][index + 0] + chunkPosition.x * CHUNK_SIZE + blockPosition.x);
		m_verticesPos.push_back(Faces::faces[face][index + 1] + chunkPosition.y * CHUNK_SIZE + blockPosition.y);
		m_verticesPos.push_back(Faces::faces[face][index + 2] + chunkPosition.z * CHUNK_SIZE + blockPosition.z);
	}

	m_indices.insert(
		m_indices.end(),
		{m_indexOffset,
		 m_indexOffset + 1,
		 m_indexOffset + 2,

		 m_indexOffset,
		 m_indexOffset + 2,
		 m_indexOffset + 3});

	m_indexOffset += 4;
	m_isGLDirty = true;
	m_isEmpty = false;
}

// private:

void ChunkMesh::pushToGL() noexcept
{
	deleteFromGL();

	if (!m_isEmpty)
	{
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBOvext);
		glGenBuffers(1, &m_VBOtex);
		glGenBuffers(1, &m_EBO);

		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBOvext);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_verticesPos.size(), m_verticesPos.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void *)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBOtex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_texCoords.size(), m_texCoords.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (void *)0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);
	}
	m_isGLDirty = false;
}

void ChunkMesh::deleteFromGL() noexcept
{
	if (m_VAO != 0)
	{
		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
	}

	if (m_VBOvext != 0)
	{
		glDeleteBuffers(1, &m_VBOvext);
		m_VBOvext = 0;
	}

	if (m_VBOtex != 0)
	{
		glDeleteBuffers(1, &m_VBOtex);
		m_VBOtex = 0;
	}

	if (m_EBO != 0)
	{
		glDeleteBuffers(1, &m_EBO);
		m_EBO = 0;
	}
}
} // namespace tk