#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <entityx/entityx.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <map>

#include "../Physics/PhysicsManager.h"
#include "../Renderer/VertexArray.h"
#include "../Renderer/IndexBuffer.h"
#include "../Renderer/Shader.h"
#include "../Renderer/Texture.h"
#include "../Renderer/TextRenderer.h"
#include "../Components.h"
#include "../Input.h"
#include "../Logger.h"

// FreeType
#include "ft2build.h"
#include FT_FREETYPE_H

// TODO: Add error checking and unit tests to all the rendering functions

using namespace entityx;

class RenderingSystem : public System<RenderingSystem> 
{
public:
    void update(EntityManager& es, EventManager& events, TimeDelta dt) override 
    {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Pre-define variables
        ComponentHandle<Camera> mainCamera;
        ComponentHandle<Transform> mainTransform;
        glm::mat4 proj, view;

        // TODO: Get rid of for-loop
        for (Entity entity : es.entities_with_components(mainCamera, mainTransform)) 
        {
            // Set camera variable
            mainCamera = entity.component<Camera>();
            mainTransform = entity.component<Transform>();
            float x = mainCamera.get()->x - mainTransform.get()->xpos;
            float y = mainCamera.get()->y - mainTransform.get()->ypos;
            float z = mainCamera.get()->z - mainTransform.get()->zpos;
            
            // Set matrices
            proj = glm::ortho(mainCamera.get()->leftf, mainCamera.get()->rightf, mainCamera.get()->bottomf, mainCamera.get()->topf, mainCamera.get()->znear, mainCamera.get()->zfar);
            view = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
            break;
        }

        // Render Sprites        
        es.each<SpriteVertices, ShaderComp, TextureComp, Transform, Active>([dt, proj, view](
            Entity entity, SpriteVertices &position, 
            ShaderComp &shaderComp, TextureComp &textureComp,
            Transform &transformComp, Active &active) 
        {
            // Check if entity is active
            if (active.isActive) 
            {
                // Create vertex buffer
                VertexArray va;
                VertexBuffer vb(&position.vertices[0], position.vertices.size() * sizeof(float));

                VertexBufferLayout layout;
                layout.Push<float>(2);
                layout.Push<float>(2);
                va.AddBuffer(vb, layout);

                // Create index buffer
                IndexBuffer ib(&position.indices[0], position.indices.size());

                // Setup shader
                shaderComp.shader->Bind();

                // Setup texture
                shaderComp.shader->SetUniforms1i("u_Texture", 0);

                // Shader binds program for the gpu to use and tells it what to do with data
                // VA = The Data itself.
                // VB = Vertex data, positions, texture coords
                // IB = Contains vertex indices
                // Draw uses IB access VB and call shader program on all vertices individually
                va.Unbind();
                vb.Unbind();
                ib.Unbind();

                shaderComp.shader->Unbind();
                glm::vec3 translation((int)round(transformComp.xpos), (int)round(transformComp.ypos), (int)round(transformComp.zpos));
                shaderComp.shader->Bind();

                textureComp.texture->Bind();
                glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(translation));
                model = glm::rotate(model, 3.141592f / 180 * transformComp.angle, glm::vec3(transformComp.xrot, transformComp.yrot, transformComp.zrot)); // where x, y, z is axis of rotation (e.g. 0 1 0)
                glm::mat4 mvp;
                mvp = proj * view * model; 

                // Set Shaders uniforms
                shaderComp.shader->SetUniformsMat4f("u_MVP", mvp);

                // Draw objects 
                shaderComp.shader->Bind();
                va.Bind();
                ib.Bind();
                glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
            }
        });

         // Draw physics
        if (Input::Instance().IsKeyPressed(GLFW_KEY_8) 
            && Input::Instance().IsKeyPressed(GLFW_KEY_9))
        {   
            if (!areDebugKeysHeldDown) 
            {
                debugPhysics = !debugPhysics;
                areDebugKeysHeldDown = true;
            }
        }
        else 
        {
            areDebugKeysHeldDown = false;
        }
        
        if (debugPhysics)
            PhysicsManager::Instance().Draw(proj, view);

        // Render Text
        es.each<TextSprite, Transform, Active>([this, dt, proj, view](
            Entity entity, TextSprite &textSprite, Transform &transformComp, Active &active) 
        {
            if (active.isActive) 
            {
                // TODO: Add as class member, currently the only way to do different font sizes
                TextRenderer textRenderer("src/Assets/fonts/arial.ttf", textSprite.fontpixelheight);
                glm::vec2 textPosition = glm::vec2(transformComp.xpos, transformComp.ypos);
                glm::vec2 textScale = glm::vec2(transformComp.xscl, transformComp.yscl);
                textRenderer.RenderText(textSprite.text, textPosition, textScale, textSprite.color, proj, view);
            }   
        });
    }

private:
    bool debugPhysics = false;
    bool areDebugKeysHeldDown = true;
};
