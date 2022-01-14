#include "Shader.h"

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "../Logger.h"

Shader::Shader(const string& filepath) : m_FilePath(filepath), m_RendererID(0) 
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() 
{
    glDeleteProgram(m_RendererID);
}

ShaderProgramSource Shader::ParseShader(const string& filepath) 
{
    ifstream stream(filepath);
    
    //Checks if file can be read
    if(stream.fail()) 
    {
        // TODO: Replace as strerror is not thread safe
        LOG_ERROR(strerror(errno));
    }

    enum class ShaderType 
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    string line;
    stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) 
    {
        if (line.find("#shader") != string::npos) 
        {
            if (line.find("vertex") != string::npos)
                type = ShaderType::VERTEX;
            
            else if (line.find("fragment") != string::npos)
                type = ShaderType::FRAGMENT;
        }
        else 
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const string& source) 
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) 
    {
        // Create message 
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*) malloc(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        
        // Log Message
        string err = (type == GL_VERTEX_SHADER) ? "Failed to compile shader! vertex" : "Failed to compile shader! fragment";
        LOG_ERROR(err);

        // Delete failed shader
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const  string& vertexShader, const string& fragmentShader) 
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void Shader::Bind() const 
{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const 
{
    glUseProgram(0);
}

void Shader::SetUniforms1i(const string& name, int value) 
{
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniforms1f(const string& name, float value) 
{
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniforms3f(const string& name, float v0, float v1, float v2) 
{
    glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetUniforms4f(const string& name,float v0, float v1, float v2, float v3) 
{
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformsMat4f(const string& name, const glm::mat4& matrix) 
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

int Shader::GetUniformLocation(const string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1)
        cout << "Warning: uniform " << name << " doesn't exist!" << endl;

    m_UniformLocationCache[name] = location;
    return location;
}
