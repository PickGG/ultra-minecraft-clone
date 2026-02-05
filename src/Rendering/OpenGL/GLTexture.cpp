#include "GLTexture.hpp"

#include <SDL3/SDL_surface.h>
#include <stdio.h>
#include <cassert>

GL::Texture::Texture(const SDL_Surface *surface)
{
    printf("Pixel format: %#x\n", surface->format);
    assert(surface->format == SDL_PixelFormat::SDL_PIXELFORMAT_RGBA32);

    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    GLint mode = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

}

GL::Texture::~Texture()
{
    glDeleteTextures(1, &m_textureId);
}

void GL::Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, m_textureId);
}
