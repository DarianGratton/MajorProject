#pragma once

#include <string>
#include <map>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <entityx/entityx.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../components.h"
#include "../Renderer/vertexArray.h"
#include "../Renderer/indexBuffer.h"
#include "../Renderer/shader.h"
#include "../Renderer/texture.h"

#include "../logger.h"

// FreeType
#include "ft2build.h"
#include FT_FREETYPE_H

// TODO: Add error checking and unit tests to all the rendering functions

using namespace entityx;

class RenderingSystem : public System<RenderingSystem> {
public:
    struct Character {
        unsigned int textureID;
        glm::ivec3 size;
        glm::ivec3 bearing;
        unsigned int advance;
    };

    void tempTextRendering(glm::mat4 proj, glm::mat4 view) {
        // Initialize FreeType
        FT_Library library;
        if (FT_Init_FreeType(&library)) {
            LOG_ERROR("FreeType could not be initialized");
            return;
        }
        
        // Font
        FT_Face face;
        FT_Error error = FT_New_Face(library, "src/Assets/fonts/arial.ttf", 0, &face);
        if (error == FT_Err_Unknown_File_Format) {
            LOG_ERROR("FreeType font format is unsupported");
            return;
        } else if (error) {
            LOG_ERROR("FreeType font file could not be opened or read");
            return;
        }

        // Change font size
        //error = FT_Set_Char_Size(face, 0, 16*64, 300, 300);
        error = FT_Set_Pixel_Sizes(face, 0, 48);  

        std::map<char, Character> Characters;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
  
        for (unsigned char c = 0; c < 128; c++)
        {
            // load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture, 
                glm::ivec3(face->glyph->bitmap.width, face->glyph->bitmap.rows, 0),
                glm::ivec3(face->glyph->bitmap_left, face->glyph->bitmap_top, 0),
                (unsigned int)face->glyph->advance.x
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }

        FT_Done_Face(face);
        FT_Done_FreeType(library);

        VertexArray va;
        VertexBuffer vb(NULL, sizeof(float) * 6 * 4);

        VertexBufferLayout layout;
        layout.push<float>(2);
        layout.push<float>(2);
        va.addBuffer(vb, layout);

        std::string text = "Hello world";
        float x = 25.0f;
        float y = 25.0f;
        float scale = 1.0f;
        glm::vec3 color = glm::vec3(0.5f, 0.8f, 0.2f);

        // activate corresponding render state	
        Shader shader("src/Assets/shaders/Basic.shader");
        shader.bind();

        // Setup texture
        glActiveTexture(GL_TEXTURE0);
        shader.setUniforms3f("u_Texture", color.x, color.y, color.z);
        
        va.bind();

        // iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++) {
            
            Character ch = Characters[*c];

            float xpos = x + ch.bearing.x * scale;
            float ypos = y - (ch.size.y - ch.bearing.y) * scale;

            float w = ch.size.x * scale;
            float h = ch.size.y * scale;
            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },            
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }           
            };

            // Stuff
            va.unbind();
            vb.unbind();

            // update content of VBO memory
            vb.bind();
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
            vb.unbind();

            shader.unbind();
            glm::vec3 translation((int)round(0.0f), (int)round(0.0f), (int)round(0.0f));
            shader.bind();

            glBindTexture(GL_TEXTURE_2D, ch.textureID);
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(translation));
            model = glm::rotate(model, 3.141592f / 180 * 0, glm::vec3(0, 0, 1)); // where x, y, z is axis of rotation (e.g. 0 1 0)
            glm::mat4 mvp;
            mvp = proj * view * model; 

            // Set Shaders uniforms
            shader.setUniformsMat4f("u_MVP", mvp);

            shader.bind();
            va.bind();

            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        }

        va.unbind();
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    void update(EntityManager& es, EventManager& events, TimeDelta dt) override {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Pre-define variables
        ComponentHandle<Camera> mainCamera;
        ComponentHandle<Transform> mainTransform;
        glm::mat4 proj, view;

        // TODO: Get rid of for-loop
        for (Entity entity : es.entities_with_components(mainCamera, mainTransform)) {
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

        es.each<SpriteVertices, ShaderComp, TextureComp, Transform>([dt, proj, view](
            Entity entity, SpriteVertices &position, 
            ShaderComp &shaderComp, TextureComp &textureComp,
            Transform &transformComp) {

            // Create vertex buffer
            VertexArray va;
            VertexBuffer vb(&position.vertices[0], position.vertices.size() * sizeof(float));

            VertexBufferLayout layout;
            layout.push<float>(2);
            layout.push<float>(2);
            va.addBuffer(vb, layout);

            // Create index buffer
            IndexBuffer ib(&position.indices[0], position.indices.size());

            // Setup shader
            Shader shader(shaderComp.filepath);
            shader.bind();

            // Setup texture
            Texture texture(textureComp.filepath); 
            shader.setUniforms1i("u_Texture", 0);

            // Shader binds program for the gpu to use and tells it what to do with data
            // VA = The Data itself.
            // VB = Vertex data, positions, texture coords
            // IB = Contains vertex indices
            // Draw uses IB access VB and call shader program on all vertices individually
            va.unbind();
            vb.unbind();
            ib.unbind();

            shader.unbind();
            glm::vec3 translation((int)round(transformComp.xpos), (int)round(transformComp.ypos), (int)round(transformComp.zpos));
            shader.bind();

            texture.bind();
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(translation));
            model = glm::rotate(model, 3.141592f / 180 * transformComp.angle, glm::vec3(transformComp.xrot, transformComp.yrot, transformComp.zrot)); // where x, y, z is axis of rotation (e.g. 0 1 0)
            glm::mat4 mvp;
            mvp = proj * view * model; 

            // Set Shaders uniforms
            shader.setUniformsMat4f("u_MVP", mvp);

            // Draw objects 
            shader.bind();
            va.bind();
            ib.bind();
            glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
        });

        tempTextRendering(proj, view);
    }
};
