#pragma once

#include <glad/glad.h>
#include <entityx/entityx.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <../components.h>
#include <../Renderer/vertexArray.h>
#include <../Renderer/indexBuffer.h>
#include <../Renderer/shader.h>

using namespace entityx;

class RenderingSystem : public System<RenderingSystem> {
public:
    void update(EntityManager& es, EventManager& events, TimeDelta dt) override {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Pre-define variables
        ComponentHandle<Camera> mainCamera;
        ComponentHandle<Transform> mainTransform;
        glm::mat4 proj, view;

        // TODO: Get rid of for-loop
        for (Entity entity : es.entities_with_components(mainCamera)) {
            // Set camera variable
            mainCamera = entity.component<Camera>();
            
            // Set matrices
            proj = glm::ortho(mainCamera.get()->leftf, mainCamera.get()->rightf, mainCamera.get()->bottomf, mainCamera.get()->topf, mainCamera.get()->znear, mainCamera.get()->zfar);
            view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
            break;
        }

        es.each<SpriteVertices, ShaderComp, TextureComp, Transform>([dt, proj, view](
            Entity entity, SpriteVertices &position, 
            ShaderComp &shaderComp, TextureComp &textureComp,
            Transform &transformComp) {

            // Create vertex buffer
            float positions[] = {
                // pos x      // pos y      //texture coords
                position.v0x, position.v0y, position.v0t1, position.v0t2,
                position.v1x, position.v1y, position.v1t1, position.v1t2,
                position.v2x, position.v2y, position.v2t1, position.v2t2,
                position.v3x, position.v3y, position.v3t1, position.v3t2,
            };

            unsigned int indices[] = {
                position.v0, position.v1, position.v2,
                position.v3, position.v4, position.v5
            };

            VertexArray va;
            VertexBuffer vb(positions, 4 * 4 * sizeof(float));

            VertexBufferLayout layout;
            layout.push<float>(2);
            layout.push<float>(2);
            va.addBuffer(vb, layout);

            // Create index buffer
            IndexBuffer ib(indices, 6);

            // Setup shader
            Shader shader(shaderComp.filepath);
            shader.bind();

            // Setup texture
            // texture Texture(textureComp.filepath); 
            // shader.setUniforms1i("u_Texture", 0);

            // /*
            // Shader binds program for the gpu to use and tells it what to do with data
            // VA = The Data itself.
            // VB = vertex data, positions, texture coords
            // IB = contains vertex indices
            // //Draw uses IB access VB and call shader program on all vertices individually
            // */
            // va.Unbind();
            // vb.Unbind();
            // ib.Unbind();
            // shader.Unbind();
            
            // glm::vec3 translation((int)round(transformComp.x), (int)round(transformComp.y), (int)round(transformComp.z));

            // shader.Bind();
            // Texture.Bind();
            // glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(translation));
            // model = glm::rotate(model, 3.141592f / 180 * transformComp.angle, glm::vec3(transformComp.rx, transformComp.ry, transformComp.rz)); // where x, y, z is axis of rotation (e.g. 0 1 0)
            // glm::mat4 mvp;
            // mvp = proj * view * model; 

            // shader.setUniformsMat4f("u_MVP", mvp);
            // renderer.Draw(va, ib, shader);
        });
    }
};
