#include "b2Renderer.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <vector>

#include "../Renderer/vertexArray.h"
#include "../Renderer/indexBuffer.h"
#include "../Renderer/shader.h"
#include "../Renderer/texture.h"
#include "../logger.h"

void Box2DRenderer::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    std::vector<float> spriteVertices = {
        vertices[0].x, vertices[0].y, 0.0f, 0.0f,
        vertices[1].x, vertices[1].y, 1.0f, 0.0f,
        vertices[2].x, vertices[2].y, 1.0f, 1.0f,
        vertices[3].x, vertices[3].y, 0.0f, 1.0f
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 0
    };

    // Render rewrite
    VertexArray va;
    VertexBuffer vb(&spriteVertices[0], sizeof(float) * spriteVertices.size());

    VertexBufferLayout layout;
    layout.push<float>(2);
    layout.push<float>(2);
    va.addBuffer(vb, layout);

    IndexBuffer ib(&indices[0], indices.size());

    // activate corresponding render state	
    Shader shader("src/Assets/shaders/DebugColor.shader");
    shader.bind();

    // Setup texture
    shader.setUniforms3f("u_Color", color.r, color.g, color.b);

    // Stuff
    va.unbind();
    vb.unbind();
    ib.unbind();

    shader.unbind();
    glm::vec3 translation((int)round(0.0f), (int)round(0.0f), (int)round(0.0f));
    shader.bind();

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(translation));
    model = glm::rotate(model, 3.141592f / 180 * 0, glm::vec3(0, 0, 1)); // where x, y, z is axis of rotation (e.g. 0 1 0)
    model = glm::scale(model, glm::vec3(1, 1, 1));
    glm::mat4 mvp;
    mvp = proj * view * model; 

    // Set Shaders uniforms
    shader.setUniformsMat4f("u_MVP", mvp);

    shader.bind();
    va.bind();
    ib.bind();

    glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}

// We just need to have these to prevent override errors, they don't actually do anything right now
void Box2DRenderer::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {}
void Box2DRenderer::DrawCircle(const b2Vec2& center, float radius, const b2Color& color) {}
void Box2DRenderer::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) {}
void Box2DRenderer::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {}
void Box2DRenderer::DrawTransform(const b2Transform& xf) {}
void Box2DRenderer::DrawPoint(const b2Vec2& p, float size, const b2Color& color) {}
