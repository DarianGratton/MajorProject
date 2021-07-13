#pragma once

#include <string>

class Texture {
public:
    Texture(const std::string& path);
    ~Texture();

    // Windows has 32 texture slots, default use 0
    void setup();
    void bind(unsigned int slot = 0) const;
    void unbind() const;

    inline int getWidth() const { return m_Width; }
    inline int getHeight() const { return m_Width; }

private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;
};