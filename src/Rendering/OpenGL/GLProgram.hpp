#pragma once

#include "ThisProjectGL.hpp"
#include <glm/mat4x4.hpp>

class GLShaderProgram
{
    GLuint m_shaderProgram = 0;
    GLchar* LoadShader(const char* path);
public:
    GLShaderProgram() = default;
    GLShaderProgram(const GLShaderProgram&) = delete;
    GLShaderProgram& operator=(const GLShaderProgram&) = delete;
    GLShaderProgram(GLShaderProgram&&) = delete;
    GLShaderProgram& operator=(GLShaderProgram&&) = delete;

    ~GLShaderProgram();

    bool Load(const char* vertexPath, const char* fragmentPath);
    void Use();
    void SetUniformMat4x4(const GLchar* name, glm::mat4x4 matrix);
};