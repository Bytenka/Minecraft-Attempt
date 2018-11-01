#pragma once

#include "Chunk.h"
#include "Block.h"
#include "WorldConstants.h"

#include <glm/glm.hpp>

#include <array>

namespace tk
{
class ChunkColumn
{
public:
  ChunkColumn();
  ~ChunkColumn();

  Chunk *getChunkAtIndex(unsigned index);
  void generateMeshes();
  void generateMeshOfChunkAt(unsigned index);
  void updateMeshes();
  void generateTerrain();

  //inline glm::ivec2 getPosition() const noexcept { return m_position; };
  //inline glm::ivec2 getPositionInBlocks() const noexcept { return m_position * CHUNK_SIZE; };

public:
  // These are duplicate information used to build the meshes of the chunk. The superclass World should keep
  // them up to date.
  std::array<ChunkColumn *, 4> neighboors; // Fill to null at creation, should be updated by world
                                           // Left, Right, Front, Back
  glm::ivec2 position;

private:
  void fillBlockColumnAtWith(const glm::ivec3 &bottomLocalPos, unsigned top, const Block &block);
  inline bool isPositionInColumn(const glm::ivec3 &position) const noexcept { return (position.x >= 0 && position.x < CHUNK_SIZE) && (position.y >= 0 && position.y < CHUNK_SIZE * CHUNK_COL_HEIGHT) && (position.z >= 0 && position.z < CHUNK_SIZE); };

private:
  std::array<Chunk, CHUNK_COL_HEIGHT> m_chunks;
};
} // namespace tk