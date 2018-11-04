#include "WorldRenderer.h"
#include "../utils/Exceptions.h"

namespace tk
{
WorldRenderer::WorldRenderer()
{
}

WorldRenderer::~WorldRenderer()
{
}

void WorldRenderer::drawWorld(World &world)
{
    try
    {
        auto data = world.getDrawableContent();

        for (auto &current : data)
        {
            m_chunkColumnRenderer.drawChunkColumn(*current);
        }
    }
    catch (RuntimeException &e)
    {
        e.pushCurrentContext(__FUNCTION__);
        throw;
    }
}
} // namespace tk
