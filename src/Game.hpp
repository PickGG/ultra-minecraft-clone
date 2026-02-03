#pragma once

#include <memory>

class ChunkWorld;
class IChunkRenderer;
class Camera;
class CameraController;
namespace Rml {
    class Context;
}

class Game
{
    static std::unique_ptr<Game> m_gameInstance;
    bool m_isStarted = false;
    bool m_isRunning;
    std::unique_ptr<ChunkWorld> m_chunkWorld;
    std::unique_ptr<IChunkRenderer> m_chunkRenderer;
    std::unique_ptr<Camera> m_camera;
    std::unique_ptr<CameraController> m_cameraController;
    Rml::Context* m_rmlContext;
public:
    Game() = default;
    Game(const Game&) = delete;
    Game(const Game&&) = delete;
    Game& operator=(const Game&) = delete;
    Game& operator=(const Game&&) = delete;

    static Game* GetInstance();
    bool Startup();
    void Run();
    void Shutdown();
};