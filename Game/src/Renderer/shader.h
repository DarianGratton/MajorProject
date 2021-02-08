#pragma once

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader {
public:
    Shader(const std::string& filepath);
    ~Shader();

    void bind() const;
    void unbind() const;

    // Set uniforms
    // May need uniform templates and add different uniform types
    void setUniforms1i(const std::string& name, int value);
    void setUniforms1f(const std::string& name, float value);
    void setUniforms4f(const std::string& name, float v0, float v1, float f2, float f3);
    void setUniformsMat4f(const std::string& name, const glm::mat4& matrix);

private:
    std:: string m_FilePath;
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache; // Caching for uniforms
    
    ShaderProgramSource parseShader(const std::string& filepath);
    unsigned int compileShader(unsigned int type, const std::string& source);
    unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
    int getUniformLocation(const std::string& name);
};