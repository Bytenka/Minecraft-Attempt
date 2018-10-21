#pragma once

#include "ShaderManager.h"
#include "Camera.h"

namespace tk
{
class RenderWindow; // Forward declaration for the pointer to be valid
                    // Real include is in the cpp file
class RenderEngine
{
public:
  RenderEngine(RenderWindow *targetWindow);
  ~RenderEngine();

  void drawNewFrame(Camera &camera);
  void computeMatrices();

private:
  // ChunkRenderer
  ShaderPtr m_mainShader;
  RenderWindow *m_targetWindow;
};
} // namespace tk
