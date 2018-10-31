#include "ChunkColumnRenderer.h"
#include "../world/WorldConstants.h"

namespace tk
{
ChunkColumnRenderer::ChunkColumnRenderer()
{
}

ChunkColumnRenderer::~ChunkColumnRenderer()
{
}

void ChunkColumnRenderer::drawChunkColumn(ChunkColumn &col)
{
    for (int i = CHUNK_COL_HEIGHT - 1; i >= 0; i--)
    {
        Chunk *current = col.getChunkAtIndex(i);
        if (current->getMesh().isDirty())
        {
            col.buildMeshOfChunkAt(i);
        }

        if (!current->isEmpty())
        {
            m_chunkRenderer.drawChunk(*current);
        }
    }
}
} // namespace tk