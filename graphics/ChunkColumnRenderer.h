#pragma once

#include "../world/ChunkColumn.h"
#include "ChunkRenderer.h"

namespace tk
{
class ChunkColumnRenderer
{
public:
  ChunkColumnRenderer();
  ~ChunkColumnRenderer();

  void drawChunkColumn(ChunkColumn &col);

private:
  ChunkRenderer m_chunkRenderer;
};
} // namespace tk