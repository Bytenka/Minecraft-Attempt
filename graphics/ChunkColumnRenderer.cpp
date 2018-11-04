#include "ChunkColumnRenderer.h"
#include "../world/WorldConstants.h"
#include "../utils/Exceptions.h"

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
    try
    {
        for (int i = CHUNK_COL_HEIGHT - 1; i >= 0; i--)
        {
            Chunk *current = col.getChunkAtIndex(i);
            //if (current->getMesh().isGLDirty())
            //{
            //    current->getMesh().buildGL();
            //}
            //
            //if (!current->isEmpty())
            //{
            m_chunkRenderer.drawChunk(*current);
            //}
        }
    }
    catch (RuntimeException &e)
    {
        e.pushCurrentContext(__FUNCTION__);
        throw;
    }
}
} // namespace tk