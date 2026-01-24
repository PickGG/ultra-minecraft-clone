#pragma once

#include <GLES3/gl3.h>

class GLShaderProgram
{
    GLuint m_shaderProgram = -1;
    GLchar* LoadShader(const char* path);
public:
    GLShaderProgram() = default;
    
    // Запрещаем копирование
    GLShaderProgram(const GLShaderProgram&) = delete;
    GLShaderProgram& operator=(const GLShaderProgram&) = delete;
    
    // Запрещаем перемещение
    GLShaderProgram(GLShaderProgram&&) = delete;
    GLShaderProgram& operator=(GLShaderProgram&&) = delete;

    ~GLShaderProgram();

    bool Load(const char* vertexPath, const char* fragmentPath);
    void Use();
};