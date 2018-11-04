#pragma once

#include "ShaderManager.h"
#include "Camera.h"
#include "../world/World.h"
#include "WorldRenderer.h"

namespace tk
{
class RenderWindow; // Forward declaration for the pointer to be valid
                    // Real include is in the cpp file
class RenderEngine
{
public:
  RenderEngine(RenderWindow *targetWindow);
  ~RenderEngine();

  void drawNewFrame(World &world, Camera &camera);
  void computeMatrices();

private:
  WorldRenderer m_worldRenderer;
  ShaderPtr m_mainShader;
  RenderWindow *m_targetWindow;
};
} // namespace tk
