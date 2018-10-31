#pragma once

#include "ChunkColumn.h"
#include "WorldConstants.h"

#include <glm/glm.hpp>

#include <vector>
#include <memory>
#include <utility> // std::pair

namespace tk
{

class World
{
public:
  World();
  ~World();

  void update(const glm::dvec3 &playerPosition);
  std::vector<ChunkColumn *> getDrawableContent() noexcept;

  // Access the world
  void setBlockAt(const glm::ivec3 &position);
  //const Block *getBlockAt(const glm::ivec3 &position);

private:
  bool detectToLoadColumns(const glm::dvec3 &playerPosition) noexcept;   // Return true if something has been added
  bool detectToUnloadColumns(const glm::dvec3 &playerPosition) noexcept; // Return true if something has been added
  bool poolToLoadColumns() noexcept;                                     // Return true if something has been loaded
  ChunkColumn *getChunkColumn(const glm::ivec2 &position) noexcept;
  //unsigned getIndexOfChunkColumn(const glm::ivec2 &position);
  bool isColumnLoaded(const glm::ivec2 &columnPos) noexcept;
  bool canColumnBeLoaded(const glm::ivec2 &columnPos) noexcept;
  void setNeighboorsOfColumn(unsigned indexOfColumn, bool setAsPresent) noexcept;
  void regenerateMeshesAround(unsigned indexOfColumn) noexcept;

private:
  std::vector<std::shared_ptr<std::pair<glm::ivec2, ChunkColumn>>> m_loadedColumns;
  //std::vector<std::shared_ptr<std::pair<glm::ivec2, ChunkColumn>>> m_toGenMeshColumns;
  std::vector<glm::ivec2> m_toLoadColumns;
};
} // namespace tk
