#pragma once

#include "../IChunkRenderer.hpp"
#include "../../World/ChunkWorld.hpp"
#include "GLProgram.hpp"
#include "../../ThisProjectGL.hpp"
#include "../Camera.hpp"
#include <unordered_map>

namespace GL
{
    struct BlockVertex
    {
        GLfloat x, y, z;
        // UVs
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
        void Render();
    private:
        std::vector<BlockVertex> CreateVertices(ChunkXZ chunkXZ, ChunkData& chunk);
        void UpdateChunkMesh(ChunkXZ chunkXZ, ChunkData& chunk);
        void RenderChunk(ChunkXZ chunkXZ, ChunkData& chunkData);
    };
}