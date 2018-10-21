#include "RenderEngine.h"
#include "../application/RenderWindow.h"

#include "../utils/Exceptions.h"

#include <glm/glm.hpp>
#include <glm/matrix.hpp>

namespace tk
{
RenderEngine::RenderEngine(RenderWindow *targetWindow)
    : m_targetWindow(targetWindow)
{
    try
    {
        m_mainShader = ShaderManager::getInstance().load("shaders/default3D.vert", "shaders/default3D.frag");
        m_mainShader->setUniform1i("textu", 0);
        computeMatrices();
        m_mainShader->enable();
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

void RenderEngine::drawNewFrame(Camera &camera)
{
    glm::vec2 rot = m_targetWindow->getCursorTravel() * 0.1;
    camera.rotate(rot.x, rot.y);
    m_mainShader->setUniformMatrix4fv("viewMat", camera.getView());
}

// private:

} // namespace tk