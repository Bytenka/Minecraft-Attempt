#include "World.h"
#include "../utils/Exceptions.h"

#include <algorithm>

namespace tk
{
World::World()
{
    /*
    for (int x = -PLAYER_VIEW_DISTANCE / 2; x <= PLAYER_VIEW_DISTANCE / 2; x++)
        for (int z = -PLAYER_VIEW_DISTANCE / 2; z <= PLAYER_VIEW_DISTANCE / 2; z++)
        {
            m_toLoadColumns.push_back({x, z});
        }
    */
}

World::~World()
{
}

void World::update(const glm::dvec3 &playerPosition)
{
    /*
    This function is meant to be called at every frame.
*/
    poolToLoadColumns();
    detectToLoadColumns(playerPosition);
    detectToUnloadColumns(playerPosition);

    /*
    int sqAround = PLAYER_VIEW_DISTANCE / 2;
    glm::dvec2 posInChunks = {playerPosition.x / CHUNK_SIZE, playerPosition.z / CHUNK_SIZE};

    for (int x = posInChunks.x - sqAround; x < posInChunks.x + sqAround; x++)
        for (int z = posInChunks.y - sqAround; z < posInChunks.y + sqAround; z++)
        {

        }
    */
}

std::vector<std::shared_ptr<ChunkColumn>> World::getDrawableContent() noexcept
{
    std::vector<std::shared_ptr<ChunkColumn>> toReturn;
    toReturn.reserve(m_loadedColumns.size() + 1); // +1 is technically useless

    for (auto &current : m_loadedColumns)
    {
        toReturn.push_back(current.second);
    }

    return toReturn;
}

// private:

bool World::detectToLoadColumns(const glm::dvec3 &playerPosition) noexcept
{
    int sqAroundPl = PLAYER_VIEW_DISTANCE / 2;
    glm::ivec2 posInChunks = {playerPosition.x / CHUNK_SIZE, playerPosition.z / CHUNK_SIZE};

    bool asLoadedSomething = false;
    for (int x = posInChunks.x - sqAroundPl; x < posInChunks.x + sqAroundPl; x++)
        for (int z = posInChunks.y - sqAroundPl; z < posInChunks.y + sqAroundPl; z++)
        {
            glm::ivec2 current = {x, z};
            if (canColumnBeLoaded(current))
            {
                m_toLoadColumns.push_back(current);
                asLoadedSomething = true;
            }
        }

    return asLoadedSomething;
}

bool World::detectToUnloadColumns(const glm::dvec3 &playerPosition) noexcept
{
    glm::ivec2 posInChunks = {playerPosition.x / CHUNK_SIZE, playerPosition.z / CHUNK_SIZE};

    bool asUnloadedSomething = false;
    for (int i = m_loadedColumns.size() - 1; i >= 0; i--)
    {
        auto &current = m_loadedColumns[i];
        if (glm::distance(glm::vec2(posInChunks), glm::vec2(current.first)) + 1 > PLAYER_VIEW_DISTANCE)
        {
            setNeighboorsOfColumn(i, false);
            m_loadedColumns.erase(m_loadedColumns.begin() + i);
            asUnloadedSomething = true;
        }
    }

    for (int i = m_toLoadColumns.size() - 1; i >= 0; i--)
    {
        auto &current = m_toLoadColumns[i];
        if (glm::distance(glm::vec2(posInChunks), glm::vec2(current)) + 1 > PLAYER_VIEW_DISTANCE)
        {
            m_toLoadColumns.erase(m_toLoadColumns.begin() + i);
            asUnloadedSomething = true;
        }
    }

    return asUnloadedSomething;
}

bool World::poolToLoadColumns() noexcept
{
    if (!m_toLoadColumns.empty())
    {
        glm::ivec2 current = m_toLoadColumns.back();
        m_toLoadColumns.pop_back();

        auto newLoadedColumn = std::make_pair(current, std::make_shared<ChunkColumn>());
        newLoadedColumn.second->position = current;

        newLoadedColumn.second->generateTerrain();

        newLoadedColumn.second->flagMeshUpdate();

        // AAAAA @TODO Remove that
        glm::ivec2 cachePos = newLoadedColumn.second->position;
        ChunkColumn *c = getChunkColumn({cachePos.x + 1, cachePos.y}).get();
        if (c)
            c->flagMeshUpdate();
        c = getChunkColumn({cachePos.x - 1, cachePos.y}).get();
        if (c)
            c->flagMeshUpdate();
        c = getChunkColumn({cachePos.x, cachePos.y + 1}).get();
        if (c)
            c->flagMeshUpdate();
        c = getChunkColumn({cachePos.x, cachePos.y - 1}).get();
        if (c)
            c->flagMeshUpdate();

        m_loadedColumns.push_back(std::move(newLoadedColumn));
        setNeighboorsOfColumn(m_loadedColumns.size() - 1, true); // Index is the last added

        return true;
    }

    return false;
}

std::shared_ptr<ChunkColumn> World::getChunkColumn(const glm::ivec2 &position) noexcept
{
    auto it = std::find_if(
        m_loadedColumns.begin(),
        m_loadedColumns.end(),
        [&](const std::pair<glm::ivec2, std::shared_ptr<ChunkColumn>> &element) { return element.first == position; });

    if (it != m_loadedColumns.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}
/*
unsigned World::getIndexOfChunkColumn(const glm::ivec2 &position)
{
    for (int i = m_loadedColumns.size() - 1; i >= 0; i--)
    {
        if (m_loadedColumns[i]->second.position == position)
        {
            return i;
        }
    }

    throw RuntimeException(__FUNCTION__, "Column at (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ") is not loaded!");
}
*/
bool World::isColumnLoaded(const glm::ivec2 &columnPos) noexcept
{
    return getChunkColumn(columnPos) != nullptr;
}

bool World::canColumnBeLoaded(const glm::ivec2 &columnPos) noexcept
{
    auto it = std::find(m_toLoadColumns.begin(), m_toLoadColumns.end(), columnPos);

    return !isColumnLoaded(columnPos) && it == m_toLoadColumns.end();
}

void World::setNeighboorsOfColumn(unsigned indexOfColumn, bool setAsPresent) noexcept
{
    auto &column = m_loadedColumns[indexOfColumn].second;

    ChunkColumn *atLeft = getChunkColumn({column->position.x - 1, column->position.y}).get();
    ChunkColumn *atRight = getChunkColumn({column->position.x + 1, column->position.y}).get();
    ChunkColumn *atFront = getChunkColumn({column->position.x, column->position.y + 1}).get();
    ChunkColumn *atBack = getChunkColumn({column->position.x, column->position.y - 1}).get();

    ChunkColumn *valueToSet = setAsPresent == true ? column.get() : nullptr;

    // Notifiy neighboors columns that this column exists
    if (atLeft)
        atLeft->neighboors[1] = valueToSet; // Set his right
    if (atRight)
        atRight->neighboors[0] = valueToSet; // Set his left
    if (atFront)
        atFront->neighboors[3] = valueToSet; // Set his back
    if (atBack)
        atBack->neighboors[2] = valueToSet; // Set his front

    // Set this column neighboors
    if (setAsPresent)
    {
        column->neighboors[0] = atLeft;
        column->neighboors[1] = atRight;
        column->neighboors[2] = atFront;
        column->neighboors[3] = atBack;
    }
}

/*void World::regenerateMeshesAround(unsigned indexOfColumn)
{
}*/
} // namespace tk
