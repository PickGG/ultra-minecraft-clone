#include "GLChunkRenderer.hpp"
#include <cassert>
#include <SDL3/SDL_surface.h>
#include <cstdlib>
#include "Log.hpp"
#include "GameSpriteAtlas.hpp"

GL::ChunkRenderer::ChunkRenderer(ChunkWorld *world, Camera* camera, GameSpriteAtlas *gameAtlas)
    : m_world{world}, m_camera{camera}, m_gameAtlas{gameAtlas}
{
    assert(m_world != nullptr);
    assert(m_camera != nullptr);
    assert(m_gameAtlas != nullptr);
    m_shader.Load("assets/shaders/chunk_mesh.vert.glsl", "assets/shaders/chunk_mesh.frag.glsl"); // !!
}

GL::ChunkRenderer::~ChunkRenderer()
{
    for(auto it = m_meshes.begin(); it != m_meshes.end(); ++it)
    {
        auto& mesh = it->second;
        glDeleteBuffers(1, &mesh.VBO);
        glDeleteVertexArrays(1, &mesh.VAO);
    }
}

void GL::ChunkRenderer::Render()
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glEnable(GL_DEPTH_TEST);
    
    for(auto it = m_world->begin(); it != m_world->end(); ++it)
    {
        if(m_meshes.find(it->first) == m_meshes.end())
        {
            UpdateChunkMesh(it->first, it->second);
        }
        ChunkMesh& mesh = m_meshes.find(it->first)->second;
        m_shader.Use();
        m_shader.SetUniformMat4x4("projection_view", m_camera->CalcMatrix());
        glBindVertexArray(mesh.VAO);
        m_gameAtlas->GetTexture()->Bind();
        glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
        glBindVertexArray(0);
    }
}

std::vector<GL::ChunkRenderer::BlockVertex> GL::ChunkRenderer::CreateVertices(ChunkXZ chunkXZ, const ChunkData &chunk)
{
    std::vector<BlockVertex> vertices;
    
    for(int y = 0; y < CHUNK_HEIGHT; y++)
    {
        for(int z = 0; z < CHUNK_DEPTH; z++)
        {
            for(int x = 0; x < CHUNK_WIDTH; x++)
            {
                Block blockId = chunk.blocks[x][z][y];
                if(blockId == BLOCK_NONE)
                    continue;
                
                glm::vec3 block = ChunkWorld::CalcXYZofBlockInChunk(chunkXZ, x, y, z);
                const BlockUVMapping& mapping = m_gameAtlas->GetBlockUVMapping(blockId);

                // FRONT
                UVRect UVs = mapping.front;
                vertices.push_back({block.x-0.5f, block.y-0.5f, block.z+0.5f, UVs.left,  UVs.bottom});
                vertices.push_back({block.x-0.5f, block.y+0.5f, block.z+0.5f, UVs.left,  UVs.top});
                vertices.push_back({block.x+0.5f, block.y+0.5f, block.z+0.5f, UVs.right, UVs.top});

                vertices.push_back({block.x-0.5f, block.y-0.5f, block.z+0.5f, UVs.left,  UVs.bottom});
                vertices.push_back({block.x+0.5f, block.y+0.5f, block.z+0.5f, UVs.right, UVs.top});
                vertices.push_back({block.x+0.5f, block.y-0.5f, block.z+0.5f, UVs.right, UVs.bottom});

                // BACK
                UVs = mapping.back;
                vertices.push_back({block.x+0.5f, block.y-0.5f, block.z-0.5f, UVs.left,  UVs.bottom});
                vertices.push_back({block.x+0.5f, block.y+0.5f, block.z-0.5f, UVs.left,  UVs.top});
                vertices.push_back({block.x-0.5f, block.y+0.5f, block.z-0.5f, UVs.right, UVs.top});

                vertices.push_back({block.x+0.5f, block.y-0.5f, block.z-0.5f, UVs.left,  UVs.bottom});
                vertices.push_back({block.x-0.5f, block.y+0.5f, block.z-0.5f, UVs.right, UVs.top});
                vertices.push_back({block.x-0.5f, block.y-0.5f, block.z-0.5f, UVs.right, UVs.bottom});

                // LEFT
                UVs = mapping.left;
                vertices.push_back({block.x-0.5f, block.y-0.5f, block.z-0.5f, UVs.left,  UVs.bottom});
                vertices.push_back({block.x-0.5f, block.y+0.5f, block.z-0.5f, UVs.left,  UVs.top});
                vertices.push_back({block.x-0.5f, block.y+0.5f, block.z+0.5f, UVs.right, UVs.top});

                vertices.push_back({block.x-0.5f, block.y-0.5f, block.z-0.5f, UVs.left,  UVs.bottom});
                vertices.push_back({block.x-0.5f, block.y+0.5f, block.z+0.5f, UVs.right, UVs.top});
                vertices.push_back({block.x-0.5f, block.y-0.5f, block.z+0.5f, UVs.right, UVs.bottom});

                // RIGHT
                UVs = mapping.right;
                vertices.push_back({block.x+0.5f, block.y-0.5f, block.z+0.5f, UVs.left,  UVs.bottom});
                vertices.push_back({block.x+0.5f, block.y+0.5f, block.z+0.5f, UVs.left,  UVs.top});
                vertices.push_back({block.x+0.5f, block.y+0.5f, block.z-0.5f, UVs.right, UVs.top});

                vertices.push_back({block.x+0.5f, block.y-0.5f, block.z+0.5f, UVs.left,  UVs.bottom});
                vertices.push_back({block.x+0.5f, block.y+0.5f, block.z-0.5f, UVs.right, UVs.top});
                vertices.push_back({block.x+0.5f, block.y-0.5f, block.z-0.5f, UVs.right, UVs.bottom});

                // TOP
                UVs = mapping.top;
                vertices.push_back({block.x-0.5f, block.y+0.5f, block.z+0.5f, UVs.left,  UVs.bottom});
                vertices.push_back({block.x-0.5f, block.y+0.5f, block.z-0.5f, UVs.left,  UVs.top});
                vertices.push_back({block.x+0.5f, block.y+0.5f, block.z-0.5f, UVs.right, UVs.top});

                vertices.push_back({block.x-0.5f, block.y+0.5f, block.z+0.5f, UVs.left,  UVs.bottom});
                vertices.push_back({block.x+0.5f, block.y+0.5f, block.z-0.5f, UVs.right, UVs.top});
                vertices.push_back({block.x+0.5f, block.y+0.5f, block.z+0.5f, UVs.right, UVs.bottom});

                // BOTTOM
                UVs = mapping.bottom;
                vertices.push_back({block.x+0.5f, block.y-0.5f, block.z+0.5f, UVs.left,  UVs.bottom});
                vertices.push_back({block.x+0.5f, block.y-0.5f, block.z-0.5f, UVs.left,  UVs.top});
                vertices.push_back({block.x-0.5f, block.y-0.5f, block.z-0.5f, UVs.right, UVs.top});

                vertices.push_back({block.x+0.5f, block.y-0.5f, block.z+0.5f, UVs.left,  UVs.bottom});
                vertices.push_back({block.x-0.5f, block.y-0.5f, block.z-0.5f, UVs.right, UVs.top});
                vertices.push_back({block.x-0.5f, block.y-0.5f, block.z+0.5f, UVs.right, UVs.bottom});
            }
        }
    }
    
    return vertices;
}

void GL::ChunkRenderer::UpdateChunkMesh(ChunkXZ chunkXZ, const ChunkData& chunk)
{
    LOG_DEBUG("Creating chunk mesh: (%d, %d)", chunkXZ.X, chunkXZ.Z);

    std::vector<BlockVertex> vertices = CreateVertices(chunkXZ, chunk);
    ChunkMesh mesh;
    mesh.vertexCount = vertices.size();

    glGenBuffers(1, &mesh.VBO);
    glGenVertexArrays(1, &mesh.VAO);
    glBindVertexArray(mesh.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(BlockVertex), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_meshes[chunkXZ] = mesh;
}
