#pragma once

#include "../Renderer/texture.h"

struct TextureComp {
    TextureComp(const char* filepath) {
        texture = new Texture(filepath);
    }

    ~TextureComp() {
        delete texture;
    }

    void setTexture(const char* filepath) {
        // Clear old texture memory
        delete texture;

        // Allocate new texture memory
        texture = new Texture(filepath);
    }

    Texture* texture;
    bool ready = false;
};