#include "Chunk.h"

#include "../utils/Logging.h"
#include "../utils/Exceptions.h"

#include <GLFW/glfw3.h>

namespace tk
{
Chunk::Chunk()
{
	fillWith(Blocks::_air);
}

Chunk::Chunk(const Block &fillBlock)
{
	fillWith(fillBlock);
}

Chunk::~Chunk()
{
}

// public:

void Chunk::setBlockAt(const glm::ivec3 &localPosition, const Block &block)
{
	if (!isPositionInChunk(localPosition))
	{
		throw RuntimeException(__FUNCTION__, "Invalid block coordinates in function: " + std::to_string(*__FUNCTION__));
	}
	else
	{
		m_blocks[toArrayCoords(localPosition)] = &block;

		if (&block != &Blocks::_air)
			m_isAir = false;

		m_isMeshDirty = true;
	}
}

const Block *Chunk::getBlockAt(const glm::ivec3 &localPosition) const
{
	if (!isPositionInChunk(localPosition))
	{
		throw RuntimeException(__FUNCTION__, "Invalid block coordinates!");
	}
	else
	{
		return m_blocks[toArrayCoords(localPosition)];
	}
}

void Chunk::fillWith(const Block &block) noexcept
{
	m_blocks.fill(&block);
	m_isAir = &block == &Blocks::_air ? true : false;
	m_isMeshDirty = true;
}

void Chunk::fillLayerWith(unsigned layer, const Block &block)
{
	if (layer >= CHUNK_SIZE)
	{
		throw RuntimeException(__FUNCTION__, "Invalid layer \"" + std::to_string(layer) + "\"!");
	}

	for (unsigned x = 0; x < CHUNK_SIZE; x++)
		for (unsigned z = 0; z < CHUNK_SIZE; z++)
		{
			m_blocks[toArrayCoords({x, layer, z})] = &block;
		}

	if (&block != &Blocks::_air)
		m_isAir = false;
	m_isMeshDirty = true;
}

void Chunk::fillColumnWith(const glm::ivec2 &localPosition, const Block &block)
{
	try
	{
		fillColumnFromToWith(localPosition, 0, CHUNK_SIZE - 1, block);
	}
	catch (RuntimeException &e)
	{
		e.pushCurrentContext(__FUNCTION__);
		throw;
	}
}

void Chunk::fillColumnFromToWith(const glm::ivec2 &localPosition, unsigned fromHeight, unsigned toHeight, const Block &block)
{
	if (!isPositionInChunk({localPosition.x, 0, localPosition.y}))
	{
		throw RuntimeException(__FUNCTION__, "Invalid column (" + std::to_string(localPosition.x) + ", " + std::to_string(localPosition.y) + ")!");
	}

	if (toHeight >= CHUNK_SIZE)
	{
		throw RuntimeException(__FUNCTION__, "Invalid height " + std::to_string(toHeight) + "! Max allowed is " + std::to_string(CHUNK_SIZE - 1));
	}

	for (; fromHeight <= toHeight; fromHeight++)
	{
		m_blocks[toArrayCoords({localPosition.x, fromHeight, localPosition.y})] = &block;
	}

	if (&block != &Blocks::_air)
		m_isAir = false;
	m_isMeshDirty = true;
}

void Chunk::generateMesh(const glm::ivec3 &chunkPosition, const std::array<Chunk *, 6> &neighboorChunks)
{
	try
	{
		if (!m_isMeshDirty)
		{
			LOG_WARN("Regenerating an already up to date mesh!");
		}

		//LOG_INFO("Generating mesh for chunk (" << chunkPosition.x << ", " << chunkPosition.y << ", " << chunkPosition.z << ")...");
		double time = glfwGetTime();
		m_mesh.clear();

		if (!m_isAir)
		{
			for (unsigned x = 0; x < CHUNK_SIZE; x++)
				for (unsigned y = 0; y < CHUNK_SIZE; y++)
					for (unsigned z = 0; z < CHUNK_SIZE; z++)
					{
						const Block *currentBlock = getBlockAt({x, y, z});
						if (currentBlock == &Blocks::_air || currentBlock == &Blocks::_opaque)
							continue;

						if (!getBlockAtWithNeighboors({x, y + 1, z}, neighboorChunks)->data.isOpaque)
							m_mesh.addFace(BlockSide::TOP, *currentBlock, chunkPosition, {x, y, z});

						if (!getBlockAtWithNeighboors({x, y - 1, z}, neighboorChunks)->data.isOpaque)
							m_mesh.addFace(BlockSide::BOTTOM, *currentBlock, chunkPosition, {x, y, z});

						if (!getBlockAtWithNeighboors({x - 1, y, z}, neighboorChunks)->data.isOpaque)
							m_mesh.addFace(BlockSide::LEFT, *currentBlock, chunkPosition, {x, y, z});

						if (!getBlockAtWithNeighboors({x + 1, y, z}, neighboorChunks)->data.isOpaque)
							m_mesh.addFace(BlockSide::RIGHT, *currentBlock, chunkPosition, {x, y, z});

						if (!getBlockAtWithNeighboors({x, y, z + 1}, neighboorChunks)->data.isOpaque)
							m_mesh.addFace(BlockSide::FRONT, *currentBlock, chunkPosition, {x, y, z});

						if (!getBlockAtWithNeighboors({x, y, z - 1}, neighboorChunks)->data.isOpaque)
							m_mesh.addFace(BlockSide::BACK, *currentBlock, chunkPosition, {x, y, z});
					}
		}

		m_isMeshDirty = false;
		//LOG_INFO("Finished in " << (glfwGetTime() - time) * 1000 << "ms");
	}
	catch (RuntimeException &e)
	{
		e.pushCurrentContext(__FUNCTION__);
		throw;
	}
}

// private:

const Block *Chunk::getBlockAtWithNeighboors(const glm::ivec3 &position, const std::array<Chunk *, 6> &neighboors)
{
	try
	{
		if (isPositionInChunk(position))
			return getBlockAt(position);
		else
		{
			glm::ivec3 newPos = (position + CHUNK_SIZE) % CHUNK_SIZE;

			if (position.y >= CHUNK_SIZE) // Top
				return neighboors[0] == nullptr ? &Blocks::_opaque : neighboors[0]->getBlockAt(newPos);

			if (position.y < 0) // Bottom
				return neighboors[1] == nullptr ? &Blocks::_opaque : neighboors[1]->getBlockAt(newPos);

			if (position.x < 0) // Left
				return neighboors[2] == nullptr ? &Blocks::_opaque : neighboors[2]->getBlockAt(newPos);

			if (position.x >= CHUNK_SIZE) // Right
				return neighboors[3] == nullptr ? &Blocks::_opaque : neighboors[3]->getBlockAt(newPos);

			if (position.z >= CHUNK_SIZE) // Front
				return neighboors[4] == nullptr ? &Blocks::_opaque : neighboors[4]->getBlockAt(newPos);

			if (position.z < 0) // Back
				return neighboors[5] == nullptr ? &Blocks::_opaque : neighboors[5]->getBlockAt(newPos);
		}
	}
	catch (RuntimeException &e)
	{
		e.pushCurrentContext(__FUNCTION__);
		throw;
	}

	throw RuntimeException(__FUNCTION__, "Impossible error: can't evaluate block position!");
}

} // namespace tk