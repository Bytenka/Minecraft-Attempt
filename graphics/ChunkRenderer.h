#pragma once

#include "../world/Chunk.h"

namespace tk
{
class ChunkRenderer
{
public:
  ChunkRenderer();
  ~ChunkRenderer();

  void drawChunk(Chunk &chunk);
};
} // namespace tk