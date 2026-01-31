#pragma once

#include <unordered_map>
#include <glm/vec3.hpp>

constexpr int CHUNK_WIDTH = 16;
constexpr int CHUNK_DEPTH = 16;
constexpr int CHUNK_HEIGHT = 16;

struct ChunkXZ
{
    short X, Z;

    bool operator==(const ChunkXZ& other) const {
        return X == other.X && Z == other.Z;
    }
};

namespace std
{
    template<>
    struct hash<ChunkXZ>
    {
        size_t operator()(const ChunkXZ& key) const {
            return (static_cast<size_t>(key.Z) << 16) | (static_cast<size_t>(key.Z));
        }
    };
}

struct ChunkData
{
    bool blocks[CHUNK_WIDTH][CHUNK_DEPTH][CHUNK_HEIGHT];
};

class ChunkWorld
{
public:
    typedef std::unordered_map<ChunkXZ, ChunkData> MapType;
    ChunkWorld();
    void LoadChunk(ChunkXZ chunkXZ);
    MapType::iterator begin();
    MapType::iterator end();
    ChunkData& GetChunkAtXZ(ChunkXZ chunkXZ);
    static glm::vec3 CalcXYZofBlockInChunk(ChunkXZ chunkXZ, int X, int Y, int Z);
private:
    MapType m_loadedChunks;
};