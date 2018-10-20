/*
  Main class for the project. Initialize everything to use OpenGL
*/

#pragma once

#include "Window.h"

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

  void createWindow(const std::string &title, unsigned width, unsigned height) noexcept;
  void destroyWindow(const std::string &title) noexcept;
  void updateWindowSize(GLFWwindow *window, int width, int height) noexcept;
  void updateWindowCursorPosition(GLFWwindow *window, double xpos, double ypos) noexcept;
  void startLoop();

  inline void setWindowAsMain(std::string windowTitle) noexcept { m_mainWindowName = windowTitle; }

private:
  unsigned
  getIndexOfWindow(const std::string &title);
  unsigned getIndexOfGLFWWindow(GLFWwindow *window);

private:
  std::vector<std::unique_ptr<Window>> m_windows;
  std::string m_mainWindowName = "";

private:
  Application() noexcept;
  ~Application() noexcept;
  Application(const Application &) = delete;
  void operator=(const Application &) = delete;
};

// Callbacks
void error_callback(int error, const char *description) noexcept;
} // namespace tk
