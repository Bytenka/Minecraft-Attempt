#include "ChunkColumn.h"

#include "ChunkMesh.h"
#include "WorldGenerator.h"

#include "../utils/Logging.h"
#include "../utils/Exceptions.h"

namespace tk
{
ChunkColumn::ChunkColumn()
{
    neighboors.fill(nullptr); // @TODO Should't be necessary. Just for safety and clarity
}

ChunkColumn::~ChunkColumn()
{
}

// public:

Chunk *ChunkColumn::getChunkAtIndex(unsigned index)
{
    if (index >= CHUNK_COL_HEIGHT)
    {
        throw RuntimeException(__FUNCTION__, "Index " + std::to_string(index) + " is invalid!");
    }

    return &m_chunks[index];
}

void ChunkColumn::buildMeshes()
{
    try
    {
        for (int i = m_chunks.size() - 1; i >= 0; i--)
        {
            buildMeshOfChunkAt(i);
        }
    }
    catch (RuntimeException &e)
    {
        e.pushCurrentContext(__FUNCTION__);
        throw;
    }
}

void ChunkColumn::buildMeshOfChunkAt(unsigned index)
{
    if (index >= CHUNK_COL_HEIGHT)
    {
        throw RuntimeException(__FUNCTION__, "Index \"" + std::to_string(index) + "\" is invalid!");
    }
    else
    {
        Chunk &current = m_chunks[index];

        std::array<Chunk *, 6> adjacentChunks;

        try
        {
            // Air chunk so we can see block faces at the top and bottom of the column
            Chunk fakeAirChunk;

            adjacentChunks[0] = index >= CHUNK_COL_HEIGHT - 1 ? &fakeAirChunk : &m_chunks[index + 1];       // Top
            adjacentChunks[1] = index <= 0 ? &fakeAirChunk : &m_chunks[index - 1];                          // Bottom
            adjacentChunks[2] = neighboors[0] == nullptr ? nullptr : neighboors[0]->getChunkAtIndex(index); // Left
            adjacentChunks[3] = neighboors[1] == nullptr ? nullptr : neighboors[1]->getChunkAtIndex(index); // Right
            adjacentChunks[4] = neighboors[2] == nullptr ? nullptr : neighboors[2]->getChunkAtIndex(index); // Front
            adjacentChunks[5] = neighboors[3] == nullptr ? nullptr : neighboors[3]->getChunkAtIndex(index); // Back

            current.generateMesh(glm::ivec3(position.x, index, position.y), adjacentChunks);
        }
        catch (RuntimeException &e)
        {
            e.pushCurrentContext(__FUNCTION__);
            throw;
        }
    }
}

void ChunkColumn::flagMeshUpdate() noexcept
{
    for (unsigned i = 0; i < m_chunks.size(); i++)
    {
        m_chunks[i].getMesh().setDirty(true);
    }
}

void ChunkColumn::generateTerrain()
{
    //fillBlockColumnAtWith({7, 0, 7}, 255, Blocks::grass);
    WorldGenerator &wg = WorldGenerator::getInstance();
    /*
    try
    {
        for (unsigned z = 0; z < CHUNK_SIZE; z++)
            for (unsigned x = 0; x < CHUNK_SIZE; x++)
            {
                unsigned height = wg.getTerrainHeight({position.x * CHUNK_SIZE + x, position.y * CHUNK_SIZE + z});

                fillBlockColumnAtWith({x, 0, z}, height, Blocks::grass);
                //fillBlockColumnAtWith({x, 0, z}, rand() % 3, Blocks::bedrock);
            }
    }
    catch (RuntimeException &e)
    {
        e.pushCurrentContext(__FUNCTION__);
        throw;
    }

*/
    try
    {
        for (unsigned z = 0; z < CHUNK_SIZE; z++)
            for (unsigned x = 0; x < CHUNK_SIZE; x++)
            {
                unsigned height = wg.getTerrainHeight({position.x * CHUNK_SIZE + x, position.y * CHUNK_SIZE + z}) / 3;

                fillBlockColumnAtWith({x, 0, z}, height - 4, Blocks::stone);
                fillBlockColumnAtWith({x, height - 3, z}, height - 1, Blocks::dirt);

                Chunk *workingChunk = getChunkAtIndex(height / CHUNK_SIZE);
                workingChunk->setBlockAt({x, height % CHUNK_SIZE, z}, Blocks::grass);
            }
    }
    catch (RuntimeException &e)
    {
        e.pushCurrentContext(__FUNCTION__);
        throw;
    }

    // @TODO Complete -> THIS IS NOT SAFE
}

// private:

void ChunkColumn::fillBlockColumnAtWith(const glm::ivec3 &bottomLocalPos, unsigned top, const Block &block)
{
    if (!isPositionInColumn(bottomLocalPos) || !isPositionInColumn({bottomLocalPos.x, top, bottomLocalPos.z}))
    {
        throw RuntimeException(__FUNCTION__, "Invalid block coordinates! (" + std::to_string(bottomLocalPos.x) + ", " + std::to_string(bottomLocalPos.y) + ", " + std::to_string(bottomLocalPos.z) + ")");
    }
    else
    {
        try
        {
            for (unsigned h = bottomLocalPos.y / CHUNK_SIZE; h <= top / CHUNK_SIZE; h++)
            {
                Chunk *c = getChunkAtIndex(h);
                if (h == top / CHUNK_SIZE)
                {
                    c->fillColumnFromToWith({bottomLocalPos.x, bottomLocalPos.z}, 0, top % CHUNK_SIZE, block);
                }
                else
                {
                    c->fillColumnWith({bottomLocalPos.x, bottomLocalPos.z}, block);
                }
            }
        }
        catch (RuntimeException &e)
        {
            e.pushCurrentContext(__FUNCTION__);
            throw;
        }
    }
}
} // namespace tk