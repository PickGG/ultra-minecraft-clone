#pragma once

class IChunkRenderer
{
public:
    virtual void Render()=0;
    virtual ~IChunkRenderer() = default;
};