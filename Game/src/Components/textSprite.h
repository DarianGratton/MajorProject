#pragma once

#include <glm/glm.hpp>

#include <string>

struct TextSprite 
{
    TextSprite(std::string text, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f), unsigned int fontpixelwidth = 0, unsigned int fontpixelheight = 24) 
        : text(text), color(color), fontpixelwidth(fontpixelwidth), fontpixelheight(fontpixelheight) {}

    std::string text;
    unsigned int fontpixelwidth;
    unsigned int fontpixelheight;
    glm::vec3 color;
};