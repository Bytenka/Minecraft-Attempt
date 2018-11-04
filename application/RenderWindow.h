#pragma once

#include "Window.h"
#include "../graphics/RenderEngine.h"
#include "../graphics/Camera.h"

#include <string>

namespace tk
{
class RenderWindow : public Window
{
public:
  RenderWindow(unsigned width, unsigned height, std::string title);
  virtual ~RenderWindow();

  virtual void update() noexcept;
  virtual void updateCursorPosition(double xpos, double ypos) noexcept;
  void drawWithWorld(World &world) noexcept;
  void setUseMouseForCamera(bool value) noexcept;
  inline const Camera &getCamera() const { return m_camera; }

private:
  Camera m_camera;
  RenderEngine m_renderEngine;
  bool m_useMouseForCamera = true;
};
} // namespace tk