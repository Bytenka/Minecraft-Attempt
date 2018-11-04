#include "../world/World.h"

#include <glm/glm.hpp>

namespace tk
{
struct WorldUpdater
{
  public:
    static WorldUpdater &getInstance()
    {
        static WorldUpdater instance;
        return instance;
    }

    void updateWorld(World &world, const glm::dvec3 &playerPosition);

  private:
    WorldUpdater();
    ~WorldUpdater();
    WorldUpdater(const WorldUpdater &) = delete;
    void operator=(const WorldUpdater &) = delete;
};
} // namespace tk
