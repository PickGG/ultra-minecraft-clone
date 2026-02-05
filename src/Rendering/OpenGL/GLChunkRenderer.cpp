#include "GLChunkRenderer.hpp"
#include <cassert>
#include <iostream>
#include <SDL3/SDL_surface.h>
#include <cstdlib>
#include "Log.hpp"

using namespace GL;

GL::ChunkRenderer::ChunkRenderer(ChunkWorld *world, Camera* camera)
    : m_world{world}, m_camera{camera}
{
    assert(m_world != nullptr);
    assert(m_camera != nullptr);
    m_shader.Load("assets/shaders/chunk_mesh.vert.glsl", "assets/shaders/chunk_mesh.frag.glsl"); // !!

    SDL_Surface* surface = SDL_LoadPNG("assets/block/dirt.png");
    if(!surface)
    {
        LOG_CRIT("Failed to load image");
        exit(1);
    }
    SDL_FlipSurface(surface, SDL_FLIP_VERTICAL);
    m_testTexture = std::make_unique<GL::Texture>(surface);
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
        m_testTexture->Bind();
        glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
        glBindVertexArray(0);
    }
}

std::vector<BlockVertex> GL::ChunkRenderer::CreateVertices(ChunkXZ chunkXZ, const ChunkData &chunk)
{
    std::vector<BlockVertex> vertices;
    
    for(int y = 0; y < CHUNK_HEIGHT; y++)
    {
        for(int z = 0; z < CHUNK_DEPTH; z++)
        {
            for(int x = 0; x < CHUNK_WIDTH; x++)
            {
                if(chunk.blocks[x][z][y] == false)
                    continue;
                
                glm::vec3 block = ChunkWorld::CalcXYZofBlockInChunk(chunkXZ, x, y, z);

                // FRONT
                vertices.push_back({block.x-0.5f, block.y-0.5f, block.z+0.5f, 0.0f, 0.0f});
                vertices.push_back({block.x-0.5f, block.y+0.5f, block.z+0.5f, 0.0f, 1.0f});
                vertices.push_back({block.x+0.5f, block.y+0.5f, block.z+0.5f, 1.0f, 1.0f});

                vertices.push_back({block.x-0.5f, block.y-0.5f, block.z+0.5f, 0.0f, 0.0f});
                vertices.push_back({block.x+0.5f, block.y+0.5f, block.z+0.5f, 1.0f, 1.0f});
                vertices.push_back({block.x+0.5f, block.y-0.5f, block.z+0.5f, 1.0f, 0.0f});

                // BACK
                vertices.push_back({block.x+0.5f, block.y-0.5f, block.z-0.5f, 0.0f, 0.0f});
                vertices.push_back({block.x+0.5f, block.y+0.5f, block.z-0.5f, 0.0f, 1.0f});
                vertices.push_back({block.x-0.5f, block.y+0.5f, block.z-0.5f, 1.0f, 1.0f});

                vertices.push_back({block.x+0.5f, block.y-0.5f, block.z-0.5f, 0.0f, 0.0f});
                vertices.push_back({block.x-0.5f, block.y+0.5f, block.z-0.5f, 1.0f, 1.0f});
                vertices.push_back({block.x-0.5f, block.y-0.5f, block.z-0.5f, 1.0f, 0.0f});

                // LEFT
                vertices.push_back({block.x-0.5f, block.y-0.5f, block.z-0.5f, 0.0f, 0.0f});
                vertices.push_back({block.x-0.5f, block.y+0.5f, block.z-0.5f, 0.0f, 1.0f});
                vertices.push_back({block.x-0.5f, block.y+0.5f, block.z+0.5f, 1.0f, 1.0f});

                vertices.push_back({block.x-0.5f, block.y-0.5f, block.z-0.5f, 0.0f, 0.0f});
                vertices.push_back({block.x-0.5f, block.y+0.5f, block.z+0.5f, 1.0f, 1.0f});
                vertices.push_back({block.x-0.5f, block.y-0.5f, block.z+0.5f, 1.0f, 0.0f});

                // RIGHT
                vertices.push_back({block.x+0.5f, block.y-0.5f, block.z+0.5f, 0.0f, 0.0f});
                vertices.push_back({block.x+0.5f, block.y+0.5f, block.z+0.5f, 0.0f, 1.0f});
                vertices.push_back({block.x+0.5f, block.y+0.5f, block.z-0.5f, 1.0f, 1.0f});

                vertices.push_back({block.x+0.5f, block.y-0.5f, block.z+0.5f, 0.0f, 0.0f});
                vertices.push_back({block.x+0.5f, block.y+0.5f, block.z-0.5f, 1.0f, 1.0f});
                vertices.push_back({block.x+0.5f, block.y-0.5f, block.z-0.5f, 1.0f, 0.0f});

                // TOP
                vertices.push_back({block.x-0.5f, block.y+0.5f, block.z+0.5f, 0.0f, 0.0f});
                vertices.push_back({block.x-0.5f, block.y+0.5f, block.z-0.5f, 0.0f, 1.0f});
                vertices.push_back({block.x+0.5f, block.y+0.5f, block.z-0.5f, 1.0f, 1.0f});

                vertices.push_back({block.x-0.5f, block.y+0.5f, block.z+0.5f, 0.0f, 0.0f});
                vertices.push_back({block.x+0.5f, block.y+0.5f, block.z-0.5f, 1.0f, 1.0f});
                vertices.push_back({block.x+0.5f, block.y+0.5f, block.z+0.5f, 1.0f, 0.0f});

                // BOTTOM
                vertices.push_back({block.x+0.5f, block.y-0.5f, block.z+0.5f, 0.0f, 0.0f});
                vertices.push_back({block.x+0.5f, block.y-0.5f, block.z-0.5f, 0.0f, 1.0f});
                vertices.push_back({block.x-0.5f, block.y-0.5f, block.z-0.5f, 1.0f, 1.0f});

                vertices.push_back({block.x+0.5f, block.y-0.5f, block.z+0.5f, 0.0f, 0.0f});
                vertices.push_back({block.x-0.5f, block.y-0.5f, block.z-0.5f, 1.0f, 1.0f});
                vertices.push_back({block.x-0.5f, block.y-0.5f, block.z+0.5f, 1.0f, 0.0f});
            }
        }
    }
    
    return vertices;
}

void GL::ChunkRenderer::UpdateChunkMesh(ChunkXZ chunkXZ, const ChunkData& chunk)
{
    std::cout << "Creating mesh: " << chunkXZ.X << " " << chunkXZ.Z << std::endl;

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
