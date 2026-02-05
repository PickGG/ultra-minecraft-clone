#pragma once

#include "Rendering/IChunkRenderer.hpp"
#include "World/ChunkWorld.hpp"
#include "GLProgram.hpp"
#include "ThisProjectGL.hpp"
#include "Rendering/Camera.hpp"
#include "GLTexture.hpp"
#include <unordered_map>
#include <memory>

namespace GL
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

    class ChunkRenderer : public IChunkRenderer
    {
        ChunkWorld* m_world;
        Camera* m_camera;
        GLShaderProgram m_shader;
        std::unordered_map<ChunkXZ, ChunkMesh> m_meshes;
    public:
        ChunkRenderer(ChunkWorld* world, Camera* camera);
        ~ChunkRenderer();
        void Render() override;
    private:
        std::vector<BlockVertex> CreateVertices(ChunkXZ chunkXZ, const ChunkData& chunk);
        void UpdateChunkMesh(ChunkXZ chunkXZ, const ChunkData& chunk);
        std::unique_ptr<GL::Texture> m_testTexture;
    };
}