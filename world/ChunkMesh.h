#pragma once

#include "Block.h"
//#include "../graphics/RenderInfo.h"

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <vector>

namespace tk
{
class ChunkMesh
{
  public:
	void addFace(BlockSide face, const Block &block, const glm::ivec3 &chunkPosition, const glm::ivec3 &blockPosition) noexcept;
	//inline RenderInfo getRenderInfo() noexcept { return {m_VAO, m_indices.size()}; }
	inline GLuint getVAO() const { return m_VAO; }
	inline unsigned getVerticesCount() const { return m_indices.size(); }
	inline bool isDirty() const noexcept { return m_isDirty; }
	inline void setDirty(bool as) noexcept { m_isDirty = as; }
	inline bool isEmpty() const noexcept { return m_isEmpty; }
	inline void build() noexcept { pushToGL(); }
	inline void clear() noexcept
	{
		m_verticesPos.clear();
		m_texCoords.clear();
		m_indices.clear();
		m_indexOffset = 0;
		deleteFromGL();
	};

  private:
	void pushToGL() noexcept;
	void deleteFromGL() noexcept;

  private:
	std::vector<GLfloat> m_verticesPos;
	std::vector<GLfloat> m_texCoords;
	std::vector<GLuint> m_indices;
	bool m_isDirty = true;
	bool m_isEmpty = true;

	GLuint m_VAO = 0, m_VBOvext = 0, m_VBOtex = 0, m_EBO = 0;

	unsigned m_indexOffset = 0;
};

} // namespace tk