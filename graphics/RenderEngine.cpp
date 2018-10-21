#include "RenderEngine.h"
#include "../application/RenderWindow.h"

#include "../utils/Exceptions.h"

#include <glm/glm.hpp>
#include <glm/matrix.hpp>

#include "../world/Chunk.h"

namespace tk
{
Chunk chunk;

RenderEngine::RenderEngine(RenderWindow *targetWindow)
    : m_targetWindow(targetWindow)
{
    try
    {
        m_mainShader = ShaderManager::getInstance().load("shaders/default3D.vert", "shaders/default3D.frag");
        m_mainShader->setUniform1i("textu", 0);
        computeMatrices();
        m_mainShader->enable();

        // Don't forget to change the _air to _opaque when removing this
        chunk.fillWith(Blocks::grass);
        chunk.generateMesh({0, 0, 0}, {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr});
    }
    catch (RuntimeException &e)
    {
        e.pushCurrentContext(__FUNCTION__);
    }
}

RenderEngine::~RenderEngine()
{
}

// public:

// Compute matrices
void RenderEngine::computeMatrices()
{
    glm::mat4 model = glm::mat4(1.0f);
    m_mainShader->setUniformMatrix4fv("modelMat", model);

    glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)m_targetWindow->getWidth() / (float)m_targetWindow->getHeight(), 0.1f, 500.0f);
    m_mainShader->setUniformMatrix4fv("projectionMat", projection);
}

void poolKeys(tk::Window *w, tk::Camera &cam);
void RenderEngine::drawNewFrame(Camera &camera)
{
    glm::vec2 rot = m_targetWindow->getCursorTravel() * 0.1;
    camera.rotate(rot.x, rot.y);
    m_mainShader->setUniformMatrix4fv("viewMat", camera.getView());

    poolKeys(m_targetWindow, camera);

    glBindVertexArray(chunk.getMesh().getVAO());
    glDrawElements(GL_TRIANGLES, chunk.getMesh().getVerticesCount(), GL_UNSIGNED_INT, 0);
}

// private:

// @TODO Cleanup
void poolKeys(tk::Window *w, tk::Camera &cam)
{
    double deltaFactor = 1;
    double speed = 0.2;

    if (w->isKeyPressed(GLFW_KEY_W))
    {
        cam.move(-cam.getForward() * deltaFactor * speed);
    }

    if (w->isKeyPressed(GLFW_KEY_S))
    {
        cam.move(cam.getForward() * deltaFactor * speed);
    }

    if (w->isKeyPressed(GLFW_KEY_A))
    {
        cam.move(-cam.getRight() * deltaFactor * speed);
    }

    if (w->isKeyPressed(GLFW_KEY_D))
    {
        cam.move(cam.getRight() * deltaFactor * speed);
    }

    if (w->isKeyPressed(GLFW_KEY_SPACE))
    {
        cam.move(glm::dvec3(0.0, 1.0, 0.0) * deltaFactor * speed);
    }

    if (w->isKeyPressed(GLFW_KEY_LEFT_SHIFT))
    {
        cam.move(glm::dvec3(0.0, -1.0, 0.0) * deltaFactor * speed);
    }

    if (w->isKeyPressed(GLFW_KEY_ESCAPE))
    {
        w->requestClosing();
    }
    if (w->isKeyPressed(GLFW_KEY_P))
    {
    }

    if (w->isKeyPressed(GLFW_KEY_O))
    {
    }
}
} // namespace tk