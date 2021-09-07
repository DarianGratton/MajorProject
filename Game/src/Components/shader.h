#pragma once

#include "../Renderer/Shader.h"

struct ShaderComp 
{
    ShaderComp(const char* filepath) 
    {
        shader = new Shader(filepath);
    }

    ~ShaderComp() 
    {
        delete shader;
    }

    Shader* shader;
};
