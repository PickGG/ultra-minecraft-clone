#pragma once

#include <unordered_map>
#include <string>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_surface.h>
#include <memory>

class SpritePacker
{
    std::unordered_map<std::string, SDL_Rect> m_sprites;
    //bool m_isPacked = false;
    SDL_Surface *m_atlasSurface = nullptr;
public:
    SpritePacker() = default;
    ~SpritePacker();
    SpritePacker(const SpritePacker&) = delete;
    SpritePacker(SpritePacker&&) = delete;
    SpritePacker& operator=(const SpritePacker&) = delete;
    SpritePacker& operator=(SpritePacker&&) = delete;

    void AddSprite(const std::string& filepath);
    SDL_Surface* Pack();
    bool IsPacked() const;
    SDL_Surface* GetSurface() const;

    SDL_Rect GetSpriteRect(const std::string& filepath) const;
};