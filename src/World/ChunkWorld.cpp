#include "ChunkWorld.hpp"
#include <cassert>
#include <cstdlib>

ChunkWorld::ChunkWorld()
{
    
}

void ChunkWorld::LoadChunk(ChunkXZ chunkXZ)
{
    ChunkData chunkData;

    for(int y = 0; y < CHUNK_HEIGHT; y++)
    {
        for(int z = 0; z < CHUNK_DEPTH; z++)
        {
            for(int x = 0; x < CHUNK_WIDTH; x++)
            {
                chunkData.blocks[x][z][y] = BLOCK_NONE;

                if(std::rand() % 15 == 0)
                    chunkData.blocks[x][z][y] = (Block)((BLOCK_NONE+1) + std::rand() % BLOCK_GRASS);
            }
        }
    }

    m_loadedChunks[chunkXZ] = chunkData;
}

ChunkWorld::MapType::iterator ChunkWorld::begin()
{
    return m_loadedChunks.begin();
}

ChunkWorld::MapType::iterator ChunkWorld::end()
{
    return m_loadedChunks.end();
}

ChunkData& ChunkWorld::GetChunkAtXZ(ChunkXZ chunkXZ)
{
    auto chunkIt = m_loadedChunks.find(chunkXZ);
    assert(chunkIt != m_loadedChunks.end());

    return chunkIt->second;
}

glm::vec3 ChunkWorld::CalcXYZofBlockInChunk(ChunkXZ chunkXZ, int X, int Y, int Z)
{
    return glm::vec3((chunkXZ.X*CHUNK_WIDTH)+X, Y, (chunkXZ.Z*CHUNK_DEPTH)+Z);
}
