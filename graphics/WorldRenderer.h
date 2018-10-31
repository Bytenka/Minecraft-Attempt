#pragma once

#include "ChunkColumnRenderer.h"
#include "../world/World.h"

namespace tk
{
class WorldRenderer
{
public:
  WorldRenderer();
  ~WorldRenderer();

  void drawWorld(World &world);

private:
  ChunkColumnRenderer m_chunkColumnRenderer;
};
} // namespace tk