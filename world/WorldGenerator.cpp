#include "WorldGenerator.h"
#include "WorldConstants.h"

#include "../utils/Logging.h"

// Higher is smoother
#define SMOOTHNESS 200

namespace tk
{

WorldGenerator::WorldGenerator(unsigned int seed) noexcept
{
    m_pn.reseed(seed);
    LOG_INFO("Initialized world generation engine.");
}

WorldGenerator::~WorldGenerator() noexcept
{
}

unsigned WorldGenerator::getTerrainHeight(glm::ivec2 at) noexcept
{
    float value = m_pn.octaveNoise0_1((float)at.x / SMOOTHNESS, (float)at.y / SMOOTHNESS, 3);
    return (unsigned)(value * CHUNK_COL_HEIGHT * CHUNK_SIZE);
}

} // namespace tk