#include "ChunkRenderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace tk
{
ChunkRenderer::ChunkRenderer()
{
}

ChunkRenderer::~ChunkRenderer()
{
}

void ChunkRenderer::drawChunk(Chunk &chunk)
{
    ChunkMesh &mesh = chunk.getMesh();

    glBindVertexArray(mesh.getVAO());
    glDrawElements(GL_TRIANGLES, mesh.getVerticesCount(), GL_UNSIGNED_INT, (void *)0);
}
} // namespace tk