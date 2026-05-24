#pragma once

#include <memory>

class ChunkWorld;
class IChunkRenderer;
class Camera;
class CameraController;
class SpritePacker;
class GameSpriteAtlas;
namespace Rml {
    class Context;
}
namespace GL {
    class Texture;
}

class Game
{
    bool m_isStarted = false;
    bool m_isRunning;
    static std::unique_ptr<Game> m_gameInstance;
    Rml::Context* m_rmlContext;
    std::unique_ptr<ChunkWorld> m_chunkWorld;
    std::unique_ptr<IChunkRenderer> m_chunkRenderer;
    std::unique_ptr<Camera> m_camera;
    std::unique_ptr<CameraController> m_cameraController;
    std::unique_ptr<GameSpriteAtlas> m_gameSpriteAtlas;
public:
    Game() = default;
    Game(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(const Game&) = delete;
    Game& operator=(Game&&) = delete;
    static Game* GetInstance();
    bool Startup();
    void Run();
    void Shutdown();
    GameSpriteAtlas& GetGameAtlas() const;
private:
    void LoadContent();
};