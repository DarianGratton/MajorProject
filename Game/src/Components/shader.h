#pragma once

#include "../Renderer/shader.h"

struct ShaderComp {
    ShaderComp(const char* filepath) {
        shader = new Shader(filepath);
    }

    ~ShaderComp() {
        delete shader;
    }

    Shader* shader;
};
