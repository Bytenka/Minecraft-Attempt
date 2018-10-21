#include "Application.h"

#include "../utils/Exceptions.h"
#include "../utils/Logging.h"
#include "../utils/FPSCounter.h"

#include "../graphics/ShaderManager.h"
#include "../graphics/Image.h"
#include "../graphics/Camera.h"
#include "../graphics/RenderEngine.h"

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

void Application::startLoop()
{
    try
    {
        // @TODO Cleanup debugging stuff
        /*
        GLfloat vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f};

        GLuint indices[] = {
            0, 1, 2,
            0, 2, 3};

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint vbo, ebo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
        glEnableVertexAttribArray(0);
        */

        FPSCounter fps;
        fps.displayFPS = true;

        bool appShouldTerminate = false;
        while (!appShouldTerminate)
        {
            fps.frameBegin();
            for (int i = m_windows.size() - 1; i >= 0; i--)
            {
                auto &currentPair = m_windows[i];
                auto &w = currentPair.second;
                w->setContextCurrent();
                w->update();
                w->clear({127, 127, 255});

                //glBindVertexArray(vao);
                //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                w->draw();

                if (w->shouldClose())
                {
                    if (currentPair.first == m_mainWindowUID)
                        appShouldTerminate = true;
                    m_windows.erase(m_windows.begin() + i);
                }
            }
            fps.frameEnd();
        }
    }
    catch (RuntimeException &e)
    {
        e.pushCurrentContext(__FUNCTION__);
    }
}

unsigned Application::createWindow(const std::string &title, unsigned width, unsigned height) noexcept
{
    unsigned uid = m_windowsUIDCounter++;
    auto newWindow = std::make_pair(uid, std::make_unique<Window>(width, height, title));
    m_windows.push_back(std::move(newWindow));
    return uid;
}

unsigned Application::createRenderWindow(const std::string &title, unsigned width, unsigned height) noexcept
{
    unsigned uid = m_windowsUIDCounter++;
    auto newWindow = std::make_pair(uid, std::make_unique<RenderWindow>(width, height, title));
    m_windows.push_back(std::move(newWindow));
    return uid;
}

void Application::destroyWindow(unsigned windowUID) noexcept
{
    try
    {
        m_windows.erase(m_windows.begin() + getIndexOfWindow(windowUID));
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
        m_windows[getIndexOfGLFWWindow(window)].second->updateSize(width, height);
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
        m_windows[getIndexOfGLFWWindow(window)].second->updateCursorPosition(xpos, ypos);
    }
    catch (RuntimeException &e)
    {
        e.pushCurrentContext(__FUNCTION__);
    }
}

Window *Application::getInternalWindow(unsigned windowUID) noexcept
{
    try
    {
        return m_windows[getIndexOfWindow(windowUID)].second.get();
    }
    catch (RuntimeException &e)
    {
        e.pushCurrentContext(__FUNCTION__);
    }
    return nullptr;
}

Window *Application::getMainWindow() noexcept
{
    try
    {
        return m_windows[getIndexOfWindow(m_mainWindowUID)].second.get();
    }
    catch (RuntimeException &e)
    {
        e.pushCurrentContext(__FUNCTION__);
    }
    return nullptr;
}

// private:

unsigned Application::getIndexOfWindow(unsigned windowUID)
{
    for (int i = m_windows.size() - 1; i >= 0; i--)
    {
        if (m_windows[i].first == windowUID)
            return i;
    }
    throw RuntimeException(__FUNCTION__, "Window " + std::to_string(windowUID) + " does not exist!");
}

unsigned Application::getIndexOfGLFWWindow(GLFWwindow *window)
{
    for (int i = m_windows.size() - 1; i >= 0; i--)
    {
        if (&*(m_windows[i].second->getGLFWWindow()) == *&window)
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
