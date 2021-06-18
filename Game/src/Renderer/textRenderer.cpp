
#include "textRenderer.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "../Renderer/vertexArray.h"
#include "../Renderer/indexBuffer.h"
#include "../Renderer/shader.h"
#include "../Renderer/texture.h"
#include "../logger.h"

TextRenderer::TextRenderer(const char * fontpath, unsigned int fontsize) {
    // Initialize FreeType
    if (FT_Init_FreeType(&library)) {
        LOG_ERROR("FreeType could not be initialized");
        return;
    }

    // Font
    FT_Error error = FT_New_Face(library, fontpath, 0, &face);
    if (error == FT_Err_Unknown_File_Format) {
        LOG_ERROR("FreeType font format is unsupported");
        return;
    } else if (error) {
        LOG_ERROR("FreeType font file could not be opened or read");
        return;
    }

    error = FT_Set_Pixel_Sizes(face, 0, fontsize);  

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            LOG_ERROR("ERROR::FREETYTPE: Failed to load Glyph");
            continue;
        }

        // Generate texture
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

        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // Store character for later use
        Character character = {
            texture, 
            glm::ivec3(face->glyph->bitmap.width, face->glyph->bitmap.rows, 0),
            glm::ivec3(face->glyph->bitmap_left, face->glyph->bitmap_top, 0),
            (unsigned int)face->glyph->advance.x
        };
        characters.insert(std::pair<char, Character>(c, character));
    }
}

TextRenderer::~TextRenderer() {
    FT_Done_Face(face);
    FT_Done_FreeType(library);
}

void TextRenderer::setFontSize(unsigned int pixelwidth, unsigned int pixelheight) {
    FT_Error error = FT_Set_Pixel_Sizes(face, pixelwidth, pixelheight);
}

void TextRenderer::renderText(std::string text, glm::vec2 position, glm::vec3 color, glm::mat4 proj, glm::mat4 view) {
    VertexArray va;
    VertexBuffer vb(NULL, sizeof(float) * 6 * 4);

    VertexBufferLayout layout;
    layout.push<float>(2);
    layout.push<float>(2);
    va.addBuffer(vb, layout);

    float x = position.x;
    float y = position.y; 
    float scale = 1.0f;

    // activate corresponding render state	
    Shader shader("src/Assets/shaders/BasicColor.shader");
    shader.bind();

    // Setup texture
    glActiveTexture(GL_TEXTURE0);
    shader.setUniforms1i("u_Texture", 0);
    shader.setUniforms3f("u_Color", color.x, color.y, color.z);
    
    va.bind();

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        
        Character ch = characters[*c];

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
