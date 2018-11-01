#pragma once

/*
 Placeholder for rendering data
*/

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
	inline bool isGLDirty() const noexcept { return m_isGLDirty; }
	inline bool isEmpty() const noexcept { return m_isEmpty; }
	inline void buildGL() noexcept { pushToGL(); }
	inline void clear() noexcept
	{
		m_verticesPos.clear();
		m_texCoords.clear();
		m_indices.clear();
		m_indexOffset = 0;
		deleteFromGL();
		m_isEmpty = true;
	};

  private:
	void pushToGL() noexcept;
	void deleteFromGL() noexcept;

  private:
	std::vector<GLfloat> m_verticesPos;
	std::vector<GLfloat> m_texCoords;
	std::vector<GLuint> m_indices;
	bool m_isGLDirty = false; // States if data is valid GPU side
	bool m_isEmpty = true;	// States if there is data CPU side

	GLuint m_VAO = 0, m_VBOvext = 0, m_VBOtex = 0, m_EBO = 0;

	unsigned m_indexOffset = 0;
};

} // namespace tk