#pragma once

#include "ThisProjectGL.hpp"

struct SDL_Surface;

namespace GL
{
    class Texture
    {
        GLuint m_textureId;
    public:
        Texture() = delete;
        Texture(const SDL_Surface* surface);
        Texture(const Texture&) = delete;
        Texture(const Texture&&) = delete;
        ~Texture();
        void Bind();
        Texture& operator=(const Texture&) = delete;
        Texture& operator=(const Texture&&) = delete;
    };
}