#pragma once

#include "Window.h"
#include "../graphics/RenderEngine.h"
#include "../graphics/Camera.h"

namespace tk
{
class RenderWindow : public Window
{
public:
  RenderWindow(unsigned width, unsigned height, std::string title);
  virtual ~RenderWindow();

  virtual void update() noexcept;
  virtual void draw() noexcept;
  virtual void updateCursorPosition(double xpos, double ypos) noexcept;
  void setUseMouseForCamera(bool value) noexcept;

private:
  Camera m_camera;
  RenderEngine m_renderEngine;
  bool m_useMouseForCamera = true;
};
} // namespace tk