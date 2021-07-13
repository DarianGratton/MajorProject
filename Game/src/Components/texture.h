#pragma once

#include "../Renderer/texture.h"

struct TextureComp {
    TextureComp(const char* filepath) {
        texture = new Texture(filepath);
    }

    ~TextureComp() {
        delete texture;
    }

    Texture* texture;
    bool ready = false;
};