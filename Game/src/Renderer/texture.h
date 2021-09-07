#pragma once

#include <string>

using namespace std;

class Texture 
{
public:
    Texture(const string& path);
    ~Texture();

    // Windows has 32 texture slots, default use 0
    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Width; }

private:
    unsigned int m_RendererID;
    string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;
};