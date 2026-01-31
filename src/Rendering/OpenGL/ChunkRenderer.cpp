#include "ChunkRenderer.hpp"
#include "../../World/ChunkWorld.hpp"
#include <iostream>

using namespace GL;

GL::ChunkRenderer::ChunkRenderer(ChunkWorld *world, Camera* camera) : m_world{world}, m_camera{camera}
{
    m_shader.Load("assets/shaders/triangle.vs", "assets/shaders/triangle.fs"); // !!
}

void GL::ChunkRenderer::RenderChunk(ChunkXZ chunkXZ, ChunkData &chunkData)
{
    ChunkMesh& mesh = m_meshes.find(chunkXZ)->second; // !!

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    m_shader.Use();
    m_shader.SetUniformMat4x4("projection_view", m_camera->CalcMatrix());
    glBindVertexArray(mesh.VAO);
    glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
    glBindVertexArray(0);
}

void GL::ChunkRenderer::Render()
{
    for(auto it = m_world->begin(); it != m_world->end(); ++it)
    {
        if(m_meshes.find(it->first) == m_meshes.end())
        {
            UpdateChunkMesh(it->first, it->second);
        }
        RenderChunk(it->first, it->second);
    }
}

std::vector<BlockVertex> GL::ChunkRenderer::CreateVertices(ChunkXZ chunkXZ, ChunkData &chunk)
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
                vertices.push_back({block.x-0.5f, block.y-0.5f, block.z+0.5f});
                vertices.push_back({block.x-0.5f, block.y+0.5f, block.z+0.5f});
                vertices.push_back({block.x+0.5f,  block.y+0.5f, block.z+0.5f});

                vertices.push_back({block.x+0.5f, block.y+0.5f, block.z+0.5f});
                vertices.push_back({block.x+0.5f, block.y-0.5f, block.z+0.5f});
                vertices.push_back({block.x-0.5f, block.y-0.5f, block.z+0.5f});

                // BACK
                vertices.push_back({block.x+0.5f, block.y-0.5f, block.z-0.5f});
                vertices.push_back({block.x+0.5f, block.y+0.5f, block.z-0.5f});
                vertices.push_back({block.x-0.5f,  block.y+0.5f, block.z-0.5f});

                vertices.push_back({block.x+0.5f, block.y-0.5f, block.z-0.5f});
                vertices.push_back({block.x-0.5f, block.y+0.5f, block.z-0.5f});
                vertices.push_back({block.x-0.5f, block.y-0.5f, block.z-0.5f});

                // LEFT
                vertices.push_back({block.x-0.5f, block.y-0.5f, block.z-0.5f});
                vertices.push_back({block.x-0.5f, block.y+0.5f, block.z-0.5f});
                vertices.push_back({block.x-0.5f,  block.y+0.5f, block.z+0.5f});

                vertices.push_back({block.x-0.5f, block.y-0.5f, block.z-0.5f});
                vertices.push_back({block.x-0.5f, block.y+0.5f, block.z+0.5f});
                vertices.push_back({block.x-0.5f, block.y-0.5f, block.z+0.5f});

                // RIGHT
                vertices.push_back({block.x+0.5f, block.y-0.5f, block.z+0.5f});
                vertices.push_back({block.x+0.5f, block.y+0.5f, block.z+0.5f});
                vertices.push_back({block.x+0.5f, block.y+0.5f, block.z-0.5f});

                vertices.push_back({block.x+0.5f, block.y-0.5f, block.z+0.5f});
                vertices.push_back({block.x+0.5f, block.y+0.5f, block.z-0.5f});
                vertices.push_back({block.x+0.5f, block.y-0.5f, block.z-0.5f});

                // TOP
                vertices.push_back({block.x-0.5f, block.y+0.5f, block.z+0.5f});
                vertices.push_back({block.x-0.5f, block.y+0.5f, block.z-0.5f});
                vertices.push_back({block.x+0.5f, block.y+0.5f, block.z-0.5f});

                vertices.push_back({block.x-0.5f, block.y+0.5f, block.z+0.5f});
                vertices.push_back({block.x+0.5f, block.y+0.5f, block.z-0.5f});
                vertices.push_back({block.x+0.5f, block.y+0.5f, block.z+0.5f});

                // BOTTOM
                vertices.push_back({block.x+0.5f, block.y-0.5f, block.z+0.5f});
                vertices.push_back({block.x+0.5f, block.y-0.5f, block.z-0.5f});
                vertices.push_back({block.x-0.5f, block.y-0.5f, block.z-0.5f});

                vertices.push_back({block.x+0.5f, block.y-0.5f, block.z+0.5f});
                vertices.push_back({block.x-0.5f, block.y-0.5f, block.z-0.5f});
                vertices.push_back({block.x-0.5f, block.y-0.5f, block.z+0.5f});
            }
        }
    }
    
    return vertices;
}

void GL::ChunkRenderer::UpdateChunkMesh(ChunkXZ chunkXZ, ChunkData& chunk)
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    m_meshes[chunkXZ] = mesh;
}
