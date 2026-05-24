#pragma once

#include "Rendering/IChunkRenderer.hpp"
#include "World/ChunkWorld.hpp"
#include "GLProgram.hpp"
#include "ThisProjectGL.hpp"
#include "Rendering/Camera.hpp"
#include "GLTexture.hpp"
#include <unordered_map>
#include <memory>

class GameSpriteAtlas;

namespace GL
{
    class ChunkRenderer : public IChunkRenderer
    {
        struct BlockVertex
        {
            GLfloat x, y, z;
            GLfloat u, v;
        };

        struct ChunkMesh
        {
            GLsizei vertexCount;
            GLuint VAO;
            GLuint VBO;
        };
    public:
        ChunkRenderer(ChunkWorld* world, Camera* camera, GameSpriteAtlas *gameAtlas);
        ~ChunkRenderer();
        void Render() override;
    private:
        ChunkWorld* m_world;
        Camera* m_camera;
        GLShaderProgram m_shader;
        GameSpriteAtlas* m_gameAtlas;
        std::unordered_map<ChunkXZ, ChunkMesh> m_meshes;
        std::vector<BlockVertex> CreateVertices(ChunkXZ chunkXZ, const ChunkData& chunk);
        void UpdateChunkMesh(ChunkXZ chunkXZ, const ChunkData& chunk);
        //std::unique_ptr<GL::Texture> m_testTexture;
    };
}