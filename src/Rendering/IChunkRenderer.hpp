#pragma once

struct ChunkData;

class IChunkRenderer
{
public:
    virtual void Render()=0;
};