#include "WorldRenderer.h"

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
    auto data = world.getDrawableContent();

    for (auto &current : data)
    {
        m_chunkColumnRenderer.drawChunkColumn(*current);
    }
}
} // namespace tk
