#include "GLProgram.hpp"

#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_log.h>
#include <glm/gtc/type_ptr.hpp>

GLchar* GLShaderProgram::LoadShader(const char* path)
{
    GLchar* shaderSource = static_cast<GLchar*>(SDL_LoadFile(path, NULL));

    if(!shaderSource)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
            "Failed to load shader from path: %s\nSDL_GetError says: %s", path, SDL_GetError());
        return nullptr;
    }

    return shaderSource;
}

GLShaderProgram::~GLShaderProgram()
{
    if(m_shaderProgram != -1)
        glDeleteProgram(m_shaderProgram);
}

bool GLShaderProgram::Load(const char *vertexPath, const char *fragmentPath)
{
    if(m_shaderProgram != -1)
    {
        return true;
    }

    GLchar* vertexShaderSource = LoadShader(vertexPath);
    GLchar* fragmentShaderSource = LoadShader(fragmentPath);

    if(!vertexShaderSource || !fragmentShaderSource)
    {
        SDL_free(vertexShaderSource);
        SDL_free(fragmentShaderSource);
        return false;
    }

    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to compile Vertex shader:\n%s", infoLog);

        SDL_free(vertexShaderSource);
        SDL_free(fragmentShaderSource);
        glDeleteShader(vertexShader);
        return false;
    }

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to compile Fragment shader:\n%s", infoLog);
        SDL_free(vertexShaderSource);
        SDL_free(fragmentShaderSource);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }

    m_shaderProgram = glCreateProgram();

    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);

    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to link shader program:\n%s", infoLog);

        SDL_free(vertexShaderSource);
        SDL_free(fragmentShaderSource);
        glDeleteProgram(m_shaderProgram);
        m_shaderProgram = -1;
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

void GLShaderProgram::Use()
{
    glUseProgram(m_shaderProgram);
}

void GLShaderProgram::SetUniformMat4x4(const GLchar* name, glm::mat4x4 matrix)
{
    GLint location = glGetUniformLocation(m_shaderProgram, name);
    if(location == -1)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to get a location of uniform '%s'", name);
        return;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}
