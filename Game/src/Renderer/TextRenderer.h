#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

class TextRenderer
{
public:
    // TODO: Currently the font is attached to the constructor as a quick solution. Font should be able to be
    // dynamically changed but I currently (Jun 6th, 2021) can't think of a robust way of doing it. 
    TextRenderer(const char * fontpath, unsigned int fontsize);
    ~TextRenderer();

    void SetFontSize(unsigned int pixelwidth, unsigned int pixelheight);
    void RenderText(std::string text, glm::vec2 position, glm::vec2 scale, glm::vec3 color, glm::mat4 proj, glm::mat4 view);

private:
    struct Character 
    {
        unsigned int textureID;
        glm::ivec3 size;
        glm::ivec3 bearing;
        unsigned int advance;
    };
    std::map<char, Character> characters;

    FT_Library library;
    FT_Face face;
};