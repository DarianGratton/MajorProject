#pragma once

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

using namespace std;

struct ShaderProgramSource 
{
    string VertexSource;
    string FragmentSource;
};

class Shader 
{
public:
    Shader(const string& filepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // Set uniforms
    // May need uniform templates and add different uniform types
    void SetUniforms1i(const string& name, int value);
    void SetUniforms1f(const string& name, float value);
    void SetUniforms4f(const string& name, float v0, float v1, float f2, float f3);
    void SetUniforms3f(const string& name, float v0, float v1, float v2);
    void SetUniformsMat4f(const string& name, const glm::mat4& matrix);

private:
    string m_FilePath;
    unsigned int m_RendererID;
    unordered_map<string, int> m_UniformLocationCache; // Caching for uniforms
    
    ShaderProgramSource ParseShader(const string& filepath);
    unsigned int CompileShader(unsigned int type, const string& source);
    unsigned int CreateShader(const string& vertexShader, const string& fragmentShader);
    int GetUniformLocation(const string& name);
};