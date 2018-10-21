/*
  Main class for the project. Initialize everything to use OpenGL

  /!\ Multiple windows are technically supported but remember that every
  OpenGL object is tighed to a context. Objects buffered in one are not usable
  from an other so they must be duplicated

  @TODO See if using the "sharing" option when creating a window is really what
  we want.
*/

#pragma once

#include "Window.h"
#include "RenderWindow.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <memory>
#include <string>

namespace tk
{
struct Application
{
public:
  inline static Application &getInstance() noexcept
  {
    static Application instance;
    return instance;
  }

  void startLoop();
  unsigned createWindow(const std::string &title, unsigned width, unsigned height) noexcept;
  unsigned createRenderWindow(const std::string &title, unsigned width, unsigned height) noexcept;
  void destroyWindow(unsigned windowUID) noexcept;
  void updateWindowSize(GLFWwindow *window, int width, int height) noexcept;
  void updateWindowCursorPosition(GLFWwindow *window, double xpos, double ypos) noexcept;
  Window *getInternalWindow(unsigned windowUID) noexcept;
  Window *getMainWindow() noexcept;

  inline void setWindowAsMain(unsigned windowUID) noexcept { m_mainWindowUID = windowUID; };

private:
  unsigned getIndexOfWindow(unsigned windowUID);
  unsigned getIndexOfGLFWWindow(GLFWwindow *window);

private:
  std::vector<std::pair<unsigned, std::unique_ptr<Window>>> m_windows;
  unsigned m_mainWindowUID = 0;
  unsigned m_windowsUIDCounter = 1;

private:
  Application() noexcept;
  ~Application() noexcept;
  Application(const Application &) = delete;
  void operator=(const Application &) = delete;
};

// Callbacks
void error_callback(int error, const char *description) noexcept;
} // namespace tk
