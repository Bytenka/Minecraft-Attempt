#include "Block.h"
#include "../utils/PerlinNoise.h"

#include <glm/glm.hpp>

namespace tk
{
class WorldGenerator
{
public:
  static WorldGenerator &getInstance() noexcept
  {
    static WorldGenerator instance;
    return instance;
  }

  unsigned getTerrainHeight(glm::ivec2 at) noexcept; // "at" is the position of a column of blocks in the world
  Block &getBlockAt(glm::ivec3 worldPosition) noexcept;

private:
  PerlinNoise m_pn;

private:
  WorldGenerator(unsigned int seed = 12345) noexcept;
  ~WorldGenerator() noexcept;
  WorldGenerator(const WorldGenerator &) = delete;
  void operator=(const WorldGenerator &) = delete;
};
} // namespace tk