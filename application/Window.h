#pragma once

#include "../graphics/Image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>

namespace tk
{
struct Color
{
  unsigned red;
  unsigned green;
  unsigned blue;
  unsigned alpha = 255;
};

class Window
{
public:
  Window(unsigned width, unsigned height, std::string title);
  virtual ~Window();

  virtual void update() noexcept;
  virtual void draw() noexcept;
  virtual void updateCursorPosition(double xpos, double ypos) noexcept;
  void updateSize(int width, int height) noexcept;
  void setContextCurrent() noexcept;
  void setIcon(Image &&img) noexcept;
  void clear(Color color) noexcept;

  inline const GLFWwindow *getGLFWWindow() const noexcept { return m_glfwWindow; }
  inline glm::dvec2 getCursorPos() const noexcept { return m_cursorPos; }
  inline glm::dvec2 getCursorTravel() const noexcept { return m_cursorTravel; }
  inline unsigned getWidth() const noexcept { return m_width; }
  inline unsigned getHeight() const noexcept { return m_height; }
  inline std::string getTitle() const noexcept { return m_title; }
  inline bool shouldClose() const noexcept { return glfwWindowShouldClose(m_glfwWindow); }

protected:
  void close() noexcept;

protected:
  std::string m_title;
  GLFWwindow *m_glfwWindow;
  glm::dvec2 m_cursorTravel, m_cursorPos;
  unsigned m_width, m_height;

protected:
  Window(const Window &) = delete;
  void operator=(const Window &) = delete;
};

// Callbacks
void framebuffer_size_callback(GLFWwindow *window, int width, int height) noexcept;
void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos) noexcept;

} // namespace tk