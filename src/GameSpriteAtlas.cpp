#include "GameSpriteAtlas.hpp"
#include "SpritePacker.hpp"
#include "Rendering/OpenGL/GLTexture.hpp"
#include <SDL3/SDL_rect.h>
#include <cassert>

GameSpriteAtlas::GameSpriteAtlas()
{
    m_packer = std::make_unique<SpritePacker>();
}

void GameSpriteAtlas::RegisterBlockFaces(Block block, const BlockFaceTextures& faces)
{
    assert(!m_packer->IsPacked());
    m_packer->AddSprite(faces.front);
    m_packer->AddSprite(faces.back);
    m_packer->AddSprite(faces.left);
    m_packer->AddSprite(faces.right);
    m_packer->AddSprite(faces.top);
    m_packer->AddSprite(faces.bottom);
    m_blockSprites[block] = faces;
}

void GameSpriteAtlas::Pack()
{
    assert(!m_packer->IsPacked());
    SDL_Surface* atlasSurface = m_packer->Pack();
    int atlasWidth = atlasSurface->w;
    int atlasHeight = atlasSurface->h;

    auto toNDC = [atlasWidth, atlasHeight](SDL_Rect uv_rect)
    {
        // Convert to NDC format
        return UVRect {
            .left   = (float)uv_rect.x / (float)atlasWidth,
            .right  = (float)(uv_rect.x+uv_rect.w) / (float)atlasWidth,
            .top    = (float)uv_rect.y / (float)atlasHeight,
            .bottom = (float)(uv_rect.y+uv_rect.h) / (float)atlasHeight,
        };
    };

    for(auto it = m_blockSprites.begin(); it != m_blockSprites.end(); ++it)
    {
        BlockFaceTextures& faces = it->second;
        m_blockUVs[it->first] = BlockUVMapping{
            .front  = toNDC(m_packer->GetSpriteRect(faces.front)),
            .back   = toNDC(m_packer->GetSpriteRect(faces.back)),
            .left   = toNDC(m_packer->GetSpriteRect(faces.left)),
            .right  = toNDC(m_packer->GetSpriteRect(faces.right)),
            .top    = toNDC(m_packer->GetSpriteRect(faces.top)),
            .bottom = toNDC(m_packer->GetSpriteRect(faces.bottom)),
        };
    }

    m_blockSprites.clear();
}

GL::Texture *GameSpriteAtlas::GetTexture()
{
    assert(m_packer->IsPacked());
    if(!m_atlasTexture) {
        SDL_Surface* atlasSurface = m_packer->GetSurface();
        m_atlasTexture = std::make_unique<GL::Texture>(atlasSurface);
    }
    return m_atlasTexture.get();
}

const BlockUVMapping& GameSpriteAtlas::GetBlockUVMapping(Block block) const
{
    assert(m_packer->IsPacked());
    auto faces = m_blockUVs.find(block);
    assert(faces != m_blockUVs.end());
    return faces->second;
}
