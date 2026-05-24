#include "SpritePacker.hpp"
#include <cassert>
#include "Log.hpp"

SpritePacker::~SpritePacker()
{
    // It is fafe to pass NULL to this function
    SDL_DestroySurface(m_atlasSurface);
}

void SpritePacker::AddSprite(const std::string& filepath)
{
    assert(!IsPacked());
    m_sprites[filepath] = SDL_Rect();
}

SDL_Surface* SpritePacker::Pack()
{
    assert(!IsPacked());

    std::unordered_map<std::string, SDL_Surface*> spriteSurfaces;

    int widthSum = 0, maxHeight = 0;

    for(auto it = m_sprites.begin(); it != m_sprites.end(); ++it)
    {
        auto filepath = it->first;
        // std::cout << "Loading: " << path << std::endl;
        SDL_Surface* surface = SDL_LoadPNG(filepath.c_str());
        if(!surface)
        {
            LOG_CRIT("%s: failed to load", filepath.c_str());
            exit(1);
        }
        spriteSurfaces[filepath] = surface;
        widthSum += surface->w;
        if(surface->h > maxHeight)
            maxHeight = surface->h;
    }

    m_atlasSurface = SDL_CreateSurface(widthSum, maxHeight, SDL_PixelFormat::SDL_PIXELFORMAT_RGBA32);

    int currentSpriteX = 0;
    for(auto it = spriteSurfaces.begin(); it != spriteSurfaces.end(); ++it)
    {
        SDL_Surface *spriteSurface = spriteSurfaces[it->first];
        SDL_Rect spriteRect;
        spriteRect.w = spriteSurface->w;
        spriteRect.h = spriteSurface->h;
        spriteRect.x = currentSpriteX;
        spriteRect.y = 0;

        SDL_BlitSurface(spriteSurface, NULL, m_atlasSurface, &spriteRect);
        currentSpriteX += spriteRect.w;
        m_sprites[it->first] = spriteRect;
    }

    //SDL_SavePNG(atlas, "packer_result.png");

    for(auto it = spriteSurfaces.begin(); it != spriteSurfaces.end(); ++it)
    {
        SDL_DestroySurface(it->second);
    }

    //m_isPacked = true;
    return m_atlasSurface;
}

bool SpritePacker::IsPacked() const
{
    return m_atlasSurface != nullptr;
}

SDL_Surface *SpritePacker::GetSurface() const
{
    assert(IsPacked());
    return m_atlasSurface;
}

SDL_Rect SpritePacker::GetSpriteRect(const std::string& filepath) const
{
    assert(IsPacked());
    auto iter = m_sprites.find(filepath);
    assert(iter != m_sprites.end());
    return iter->second;
}
