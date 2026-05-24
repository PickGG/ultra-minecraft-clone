#pragma once

#include <unordered_map>
#include <memory>
#include "Block.hpp"

class SpritePacker;
namespace GL {
    class Texture;
}

struct UVRect
{
    float left, right, top, bottom;
};

struct BlockUVMapping
{
    UVRect front, back, left, right, top, bottom;
};

struct BlockFaceTextures
{
    //const char *front, *back, *left, *right, *top, *bottom;
    std::string front, back, left, right, top, bottom;

    static BlockFaceTextures WithSameFace(const std::string& filepath)
    {
        return BlockFaceTextures{
            .front = filepath,
            .back = filepath,
            .left = filepath,
            .right = filepath,
            .top = filepath,
            .bottom = filepath,
        };
    }
    static BlockFaceTextures WithSameSides(const std::string& side, const std::string& top, const std::string& bottom)
    {
        return BlockFaceTextures{
            .front = side,
            .back = side,
            .left = side,
            .right = side,
            .top = top,
            .bottom = bottom,
        };
    }
};

class GameSpriteAtlas
{
    std::unique_ptr<SpritePacker> m_packer;
    std::unique_ptr<GL::Texture> m_atlasTexture;
    std::unordered_map<Block, BlockFaceTextures> m_blockSprites;
    std::unordered_map<Block, BlockUVMapping> m_blockUVs;
public:
    GameSpriteAtlas();
    void RegisterBlockFaces(Block block, const BlockFaceTextures& faces);
    void Pack();
    GL::Texture* GetTexture();
    const BlockUVMapping& GetBlockUVMapping(Block block) const;
};