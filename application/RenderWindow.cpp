#include "RenderWindow.h"
#include "Application.h"

#include "../utils/Logging.h"

namespace tk
{
RenderWindow::RenderWindow(unsigned width, unsigned height, std::string title)
    : Window::Window(width, height, title), m_camera({0.0f, 257.0f, 0.0f}), m_renderEngine(this)
{
    LOG_INFO("Created renderwindow!");
    m_camera.rotate(0, -90); // @TODO Debug stuff, to remove
}

RenderWindow::~RenderWindow()
{
}

// public:

void RenderWindow::update() noexcept
{
    Window::update();
}

void RenderWindow::draw() noexcept
{
    m_renderEngine.drawNewFrame(m_camera);
    Window::draw();
}

void RenderWindow::updateCursorPosition(double xpos, double ypos) noexcept
{
    Window::updateCursorPosition(xpos, ypos);
    if (m_useMouseForCamera)
        glfwSetCursorPos(m_glfwWindow, m_width / 2.0, m_height / 2.0);

    m_cursorPos = {m_width / 2, m_height / 2};
}

void RenderWindow::setUseMouseForCamera(bool value) noexcept
{
    m_useMouseForCamera = value;

    if (value)
    {
        //glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        updateCursorPosition(m_width / 2.0, m_height / 2.0);
    }
    else
    {
        glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}
} // namespace tk