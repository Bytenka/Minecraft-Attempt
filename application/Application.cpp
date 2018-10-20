#include "Application.h"

#include "../utils/Exceptions.h"
#include "../utils/Logging.h"

namespace tk
{
Application::Application() noexcept
{
    try
    {
        if (glfwInit() == GLFW_FALSE)
        {
            throw RuntimeException(__FUNCTION__, "Unable to initialize GLFW!");
        }

        glfwSetErrorCallback(error_callback);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        LOG_INFO("Application started!");
    }
    catch (RuntimeException &e)
    {
        e.pushCurrentContext(__FUNCTION__, "Application");
    }
}

Application::~Application() noexcept
{
    m_windows.clear();
    glfwTerminate();
    LOG_INFO("Application terminated!");
    system("sleep 1");
}

// public:

void Application::createWindow(const std::string &title, unsigned width, unsigned height) noexcept
{
    std::unique_ptr<Window> newWindow = std::make_unique<Window>(width, height, title);
    m_windows.push_back(std::move(newWindow));
}

void Application::destroyWindow(const std::string &title) noexcept
{
    try
    {
        m_windows.erase(m_windows.begin() + getIndexOfWindow(title));
    }
    catch (RuntimeException &e)
    {
        e.pushCurrentContext(__FUNCTION__);
    }
}

void Application::updateWindowSize(GLFWwindow *window, int width, int height) noexcept
{
    try
    {
        m_windows[getIndexOfGLFWWindow(window)]->updateSize(width, height);
    }
    catch (RuntimeException &e)
    {
        e.pushCurrentContext(__FUNCTION__);
    }
}

void Application::updateWindowCursorPosition(GLFWwindow *window, double xpos, double ypos) noexcept
{
    try
    {
        m_windows[getIndexOfGLFWWindow(window)]->updateCursorPosition(xpos, ypos);
    }
    catch (RuntimeException &e)
    {
        e.pushCurrentContext(__FUNCTION__);
    }
}

void Application::startLoop()
{
    bool appShouldTerminate = false;
    while (!appShouldTerminate) // @TODO Is this good ?
    {
        for (int i = m_windows.size() - 1; i >= 0; i--)
        {
            auto &w = m_windows[i];
            w->setContextCurrent();
            w->update();
            w->clear({127, 127, 255});
            w->draw();

            if (w->shouldClose())
            {
                if (w->getTitle() == m_mainWindowName)
                    appShouldTerminate = true;
                m_windows.erase(m_windows.begin() + i);
            }
        }
    }
}

// private:

unsigned Application::getIndexOfWindow(const std::string &title)
{
    for (int i = m_windows.size() - 1; i >= 0; i--)
    {
        if (m_windows[i]->getTitle() == title)
            return i;
    }
    throw RuntimeException(__FUNCTION__, "Window " + title + " does not exist!");
}

unsigned Application::getIndexOfGLFWWindow(GLFWwindow *window)
{
    for (int i = m_windows.size() - 1; i >= 0; i--)
    {
        if (&*(m_windows[i]->getGLFWWindow()) == *&window)
            return i;
    }
    throw RuntimeException(__FUNCTION__, "GLFW window does not exist!");
}

//--------------------------------------------------//

// Callbacks
void error_callback(int error, const char *description) noexcept
{
    try
    {
        throw RuntimeException(__FUNCTION__, "GLFW Error [" + std::to_string(error) + "] occured: " + description);
    }
    catch (RuntimeException &e)
    {
        e.pushCurrentContext(__FUNCTION__, "Application");
    }
}
} // namespace tk
