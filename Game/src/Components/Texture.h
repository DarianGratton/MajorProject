#pragma once

#include <string>
#include <sstream>
#include <cstring>

#include "../Renderer/Texture.h"

struct TextureComp 
{
    TextureComp(const char* filepath, std::string filename = "") : filename(filename)
    {
        texture = new Texture(filepath);
    }

    ~TextureComp() 
    {
        delete texture;
    }

    void SetTexture(const char* filepath) 
    {
        // Clear old texture memory
        delete texture;

        // Allocate new texture memory
        texture = new Texture(filepath);

        // Update filename
        // Convert const char * to char *
        int n = 0;
        while (filepath[n] != '\0') n++;
        char* chararray = new char [n+1];
        strcpy(chararray, filepath);

        // Get filename
        char* token = strtok(chararray, "/");
        while (token != NULL) 
        {
            filename = token;
            token = strtok(NULL, "/");
        } 
    }

    Texture* texture;
    std::string filename;
    bool ready = false;
};