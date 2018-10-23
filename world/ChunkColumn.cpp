#include "ChunkColumn.h"

#include "ChunkMesh.h"
#include "WorldGenerator.h"

#include "../utils/Logging.h"
#include "../utils/Exceptions.h"

namespace tk
{
ChunkColumn::ChunkColumn(const glm::ivec2 &position) noexcept
    : m_position(position)
{
    neighboors.fill(nullptr); // @TODO Should't be necessary. Just for safety and clarity
}

ChunkColumn::~ChunkColumn() noexcept
{
}

Chunk *ChunkColumn::getChunkAtIndex(int index) noexcept
{
    if (index >= CHUNK_COL_HEIGHT || index < 0)
    {
        return nullptr;
    }

    return &m_chunks[index];
}

void ChunkColumn::fillBlockColumnAtWith(const glm::ivec3 &bottomLocalPos, unsigned top, const Block &block)
{
    if (!isPositionInColumn(bottomLocalPos) || !isPositionInColumn({bottomLocalPos.x, top, bottomLocalPos.z}))
    {
        throw RuntimeException(__FUNCTION__, "Invalid block coordinates!");
    }
    else
    {
        try
        {
            for (unsigned h = bottomLocalPos.y; h <= top; h++)
            {
                Chunk *c = getChunkAtIndex(h / CHUNK_SIZE); // No error checking, the test before should handle that (SHOULD)
                c->setBlockAt({bottomLocalPos.x, h % CHUNK_SIZE, bottomLocalPos.z}, block);
            }
        }
        catch (RuntimeException &e)
        {
            e.pushCurrentContext(__FUNCTION__);
            throw;
        }
    }
}

void ChunkColumn::generateTerrain()
{
    WorldGenerator &wg = WorldGenerator::getInstance();

    try
    {
        for (unsigned z = 0; z < CHUNK_SIZE; z++)
        {
            for (unsigned x = 0; x < CHUNK_SIZE; x++)
            {
                unsigned height = wg.getTerrainHeight({m_position.x * CHUNK_SIZE + x, m_position.y * CHUNK_SIZE + z});

                fillBlockColumnAtWith({x, 0, z}, height - 4, Blocks::stone);
                fillBlockColumnAtWith({x, height - 3, z}, height - 1, Blocks::dirt);

                Chunk *workingChunk = getChunkAtIndex(height / CHUNK_SIZE);
                workingChunk->setBlockAt({x, height % CHUNK_SIZE, z}, Blocks::grass);
            }
        }
    }
    catch (RuntimeException &e)
    {
        e.pushCurrentContext(__FUNCTION__);
        throw;
    }

    // @TODO Complete -> THIS IS NOT SAFE
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

void ChunkColumn::buildMeshOfChunkAt(int index)
{
    if (index >= CHUNK_COL_HEIGHT || index < 0)
    {
        throw RuntimeException(__FUNCTION__, "Index \"" + std::to_string(index) + "\" is invalid!");
    }
    else
    {
        Chunk &current = m_chunks[index];

        std::array<Chunk *, 6> adjacentChunks;

        adjacentChunks[0] = index >= CHUNK_COL_HEIGHT - 1 ? nullptr : &m_chunks[index + 1];             // Top
        adjacentChunks[1] = index <= 0 ? nullptr : &m_chunks[index - 1];                                // Bottom
        adjacentChunks[2] = neighboors[0] == nullptr ? nullptr : neighboors[0]->getChunkAtIndex(index); // Left
        adjacentChunks[3] = neighboors[1] == nullptr ? nullptr : neighboors[1]->getChunkAtIndex(index); // Right
        adjacentChunks[4] = neighboors[2] == nullptr ? nullptr : neighboors[2]->getChunkAtIndex(index); // Front
        adjacentChunks[5] = neighboors[3] == nullptr ? nullptr : neighboors[3]->getChunkAtIndex(index); // Back

        try
        {
            current.generateMesh(glm::ivec3(m_position.x, index, m_position.y), adjacentChunks);
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
        // m_chunks[i].setMeshAsDirty();
    }
}

void ChunkColumn::draw()
{
    try
    {

        for (int i = m_chunks.size() - 1; i >= 0; i--)
        {
            Chunk &current = m_chunks[i];
            const ChunkMesh &mesh = current.getMesh();

            if (mesh.isDirty())
                continue;
            //buildMeshOfChunkAt(i);

            if (!mesh.isEmpty())
            {

                //glBindVertexArray(mesh.getVAO());
                //glDrawElements(GL_TRIANGLES, mesh.getVerticesCount(), GL_UNSIGNED_INT, 0);
            }
        }
    }
    catch (RuntimeException &e)
    {
        e.pushCurrentContext(__FUNCTION__);
        throw;
    }
}
} // namespace tk