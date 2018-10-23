#include "Chunk.h"
#include "Block.h"

#include <glm/glm.hpp>

#pragma once

#include "WorldConstants.h"

#include <array>

namespace tk
{
class ChunkColumn
{
public:
  ChunkColumn(const glm::ivec2 &position) noexcept;
  ~ChunkColumn() noexcept;

  inline glm::ivec2 getPosition() const noexcept { return m_position; };
  inline glm::ivec2 getPositionInBlocks() const noexcept { return m_position * CHUNK_SIZE; };
  Chunk *getChunkAtIndex(int index) noexcept;
  void generateTerrain();
  void buildMeshes();
  void buildMeshOfChunkAt(int index);
  void flagMeshUpdate() noexcept;
  void draw(); // @TODO temp, need renderer

public:
  std::array<ChunkColumn *, 4> neighboors; // Fill to null at creation, should be updated by world
                                           // Left, Right, Front, Back

private:
  void fillBlockColumnAtWith(const glm::ivec3 &bottomLocalPos, unsigned top, const Block &block);
  inline bool isPositionInColumn(const glm::ivec3 &position) const noexcept { return (position.x >= 0 && position.x < CHUNK_SIZE) && (position.y >= 0 && position.y < CHUNK_SIZE * CHUNK_COL_HEIGHT) && (position.z >= 0 && position.z < CHUNK_SIZE); };

private:
  std::array<Chunk, CHUNK_COL_HEIGHT> m_chunks;
  glm::ivec2 m_position;
};
} // namespace tk