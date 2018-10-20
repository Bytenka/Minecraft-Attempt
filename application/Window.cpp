#include "Window.h"
#include "Application.h"

#include "../utils/Exceptions.h"
#include "../utils/Logging.h"

namespace tk
{
// public:

Window::Window(unsigned width, unsigned height, std::string title)
    : m_title(title), m_width(width), m_height(height)
{
    try
    {
        m_glfwWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

        if (m_glfwWindow == NULL)
        {
            throw RuntimeException(__FUNCTION__, "Failed to create GLFW window!");
        }

        glfwMakeContextCurrent(m_glfwWindow);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw RuntimeException(__FUNCTION__, "Failed to initialize GLAD!");
        }

        glfwSwapInterval(0);
        glfwSetFramebufferSizeCallback(m_glfwWindow, framebuffer_size_callback);
        glfwSetCursorPosCallback(m_glfwWindow, cursor_pos_callback);

        // Setting up OpenGL for the window
        glViewport(0, 0, width, height);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        // Transparency stuff
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        LOG_INFO("Successfully created new window (" << width << ", " << height << ") with title \"" << m_title << "\"");
    }
    catch (RuntimeException &e)
    {
    }
}

Window::~Window()
{
    close();
}

void Window::setContextCurrent() noexcept
{
    glfwMakeContextCurrent(m_glfwWindow);
}

void Window::updateSize(int width, int height) noexcept
{
    m_width = width;
    m_height = height;
    glViewport(0, 0, width, height);
}

void Window::updateCursorPosition(double xpos, double ypos) noexcept
{
    glm::dvec2 newPos = {xpos, ypos};
    m_cursorTravel = newPos - m_cursorPos;

    if (m_useMouseForCamera)
        glfwSetCursorPos(m_glfwWindow, m_width / 2.0, m_height / 2.0);

    m_cursorPos = {m_width / 2.0, m_height / 2.0};
}

void Window::setUseMouseForCamera(bool value) noexcept
{
    m_useMouseForCamera = value;

    if (value)
    {
        glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        updateCursorPosition(m_width / 2.0, m_height / 2.0);
    }
    else
    {
        glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void Window::setIcon(Image &&img) noexcept
{
    try
    {
        unsigned size = img.getWidth() * img.getHeight() * img.getNbrChannels();
        GLFWimage glfwImage;

        glfwImage.width = img.getWidth();
        glfwImage.height = img.getHeight();

        if (!img.getData())
        {
            throw RuntimeException(__FUNCTION__, "Image is not loaded!");
        }

        glfwImage.pixels = new unsigned char[size];
        memcpy(glfwImage.pixels, img.getData(), size);
        glfwSetWindowIcon(m_glfwWindow, 1, &glfwImage);
        delete[] glfwImage.pixels;
    }
    catch (RuntimeException &e)
    {
    }
}

void Window::clear(Color color) noexcept
{
    glClearColor(
        color.red / 255.0f,
        color.green / 255.0f,
        color.blue / 255.0f,
        color.alpha / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::draw() noexcept
{
    glfwSwapBuffers(m_glfwWindow);
}

void Window::update()
{
    glfwPollEvents();
}

void Window::refresh()
{
}

// private:

void Window::close() noexcept
{
    glfwDestroyWindow(m_glfwWindow);
    //m_glfwWindow = nullptr;
    LOG_INFO("Destroyed window \"" << m_title << "\"");
}

//--------------------------------------------------//

// Callbacks
void framebuffer_size_callback(GLFWwindow *window, int width, int height) noexcept
{
    if (width != 0 && height != 0)
        Application::getInstance().updateWindowSize(window, width, height);
}

void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos) noexcept
{
    Application::getInstance().updateWindowCursorPosition(window, xpos, ypos);
}
} // namespace tk