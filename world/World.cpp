#include "World.h"
#include "../utils/Exceptions.h"
#include "../utils/Logging.h"

#include <algorithm>
#include <thread>

namespace tk
{
World::World()
{
}

World::~World()
{
}

void World::updateLoop(World &world, const glm::dvec3 &playerPos, bool &shouldStop)
{
    while (!shouldStop)
    {
        world.update(playerPos);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void World::update(const glm::dvec3 &playerPosition)
{
    detectToLoadColumns(playerPosition);
    detectToUnloadColumns(playerPosition);
    poolToLoadColumns();
    //poolToUnloadColumns();

    m_dcmutex.lock();

    updateDrawableCache();

    m_dcmutex.unlock();

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
    m_dcmutex.lock();

    std::vector<std::shared_ptr<ChunkColumn>> toReturn(m_drawableColumns);

    m_dcmutex.unlock();
    return toReturn;
}

// private:

bool World::detectToLoadColumns(const glm::dvec3 &playerPosition) noexcept
{
    int sqAroundPl = PLAYER_VIEW_DISTANCE / 2;
    glm::ivec2 posInChunks = {playerPosition.x / CHUNK_SIZE, playerPosition.z / CHUNK_SIZE};

    bool asLoadedSomething = false;

    /*
    auto attemptLoading = [&](const glm::ivec2 &localDesired) -> void {
        glm::ivec3 globalDesired = {localDesired.x + posInChunks.x, localDesired.y, posInChunks.y};

        if (canColumnBeLoaded(globalDesired))
        {
            m_toLoadColumns.push_back(globalDesired);
            asLoadedSomething = true;
        }
    };
    // Columns to load are pooled from the back so we must test things backwards
    /// Circles around the player
    for (int c = sqAroundPl - 1; c >= 0; c--)
    {
        // Front & back
        for (int i = sqAroundPl - 1; i >= 0; i--)
        {
            attemptLoading({i, c});   // Front to left
            attemptLoading({-i, c});  // Front to right
            attemptLoading({i, -c});  // Back to left
            attemptLoading({-i, -c}); // Back to right
        }

        // Left & Right
        for (int i = sqAroundPl - 1; i >= 0; i--)
        {
            attemptLoading({-c, i});  // Left to front
            attemptLoading({-c, -i}); // Left to back
            attemptLoading({c, i});   // Right to front
            attemptLoading({c, -i});  // Right to back
        }
    }
    */

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
        LOG_INFO("Column: (" << current.x << ", " << current.y << ")");
        m_toLoadColumns.pop_back();

        auto newLoadedColumn = std::make_pair(current, std::make_shared<ChunkColumn>());
        newLoadedColumn.second->position = current;

        newLoadedColumn.second->generateTerrain();

        // AAAAA @TODO Remove that mess
        glm::ivec2 cachePos = newLoadedColumn.second->position;
        m_loadedColumns.push_back(std::move(newLoadedColumn));
        setNeighboorsOfColumn(m_loadedColumns.size() - 1, true); // Index is the last added
        m_loadedColumns[m_loadedColumns.size() - 1].second->generateMeshes();

        ChunkColumn *c = getChunkColumn({cachePos.x + 1, cachePos.y}).get();
        if (c)
            c->updateMeshes();
        c = getChunkColumn({cachePos.x - 1, cachePos.y}).get();
        if (c)
            c->updateMeshes();
        c = getChunkColumn({cachePos.x, cachePos.y + 1}).get();
        if (c)
            c->updateMeshes();
        c = getChunkColumn({cachePos.x, cachePos.y - 1}).get();
        if (c)
            c->updateMeshes();

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

void World::updateDrawableCache() noexcept
{
    m_drawableColumns.clear();
    for (auto &c : m_loadedColumns)
    {
        m_drawableColumns.push_back(c.second);
    }
}

/*void World::regenerateMeshesAround(unsigned indexOfColumn)
{
}*/
} // namespace tk
