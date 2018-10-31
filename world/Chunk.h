/*
	This defines a chunk of blocks. Position of blocks are given by there indices
	in the m_blocks array
*/

#pragma once

#include "ChunkMesh.h"
#include "WorldConstants.h"
#include "Block.h"

#include <glm/glm.hpp>

#include <array>

namespace tk
{
class Chunk
{
  public:
	Chunk();
	Chunk(const Block &fillBlock);
	~Chunk();

	void setBlockAt(const glm::ivec3 &localPosition, const Block &block);
	const Block *getBlockAt(const glm::ivec3 &localPosition) const;

	void fillWith(const Block &block) noexcept;
	void fillLayerWith(unsigned layer, const Block &block);
	void fillColumnWith(const glm::ivec2 &localPosition, const Block &block);
	void fillColumnFromToWith(const glm::ivec2 &localPosition, unsigned fromHeight, unsigned toHeight, const Block &block);

	void generateMesh(const glm::ivec3 &chunkPosition, const std::array<Chunk *, 6> &neighboorChunks); // Format is TOP, BOTTOM, LEFT, RIGHT, FRONT, BACK

	inline ChunkMesh &getMesh() noexcept { return m_mesh; }
	inline bool isEmpty() noexcept { return m_mesh.isEmpty(); }

  private:
	const Block *getBlockAtWithNeighboors(const glm::ivec3 &position, const std::array<Chunk *, 6> &neighboors);
	inline bool isPositionInChunk(const glm::ivec3 &position) const noexcept { return (position.x >= 0 && position.x < CHUNK_SIZE) && (position.y >= 0 && position.y < CHUNK_SIZE) && (position.z >= 0 && position.z < CHUNK_SIZE); };
	inline unsigned toArrayCoords(const glm::ivec3 &localCoords) const noexcept { return localCoords.x + localCoords.y * CHUNK_SIZE + localCoords.z * CHUNK_SIZE * CHUNK_SIZE; }

  private:
	std::array<const Block *, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE> m_blocks;
	ChunkMesh m_mesh;
	bool m_isAir;
};
} // namespace tk