#include "Game.hpp"
#include <cassert>
#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>
#include "RmlUiBackend/RmlUi_Backend.h"
#include "RmlUiBackend/RmlUi_Include_GL3.h"
#include "RmlUiBackend/RmlUi_Renderer_GL3.h"
#include "RmlUiBackend/RmlUi_Platform_SDL.h"
#include "Rendering/Camera.hpp"
#include "CameraController.hpp"
#include "World/ChunkWorld.hpp"
#include "Rendering/OpenGL/GLChunkRenderer.hpp"
#include "SpritePacker.hpp"
#include "GameSpriteAtlas.hpp"

std::unique_ptr<Game> Game::m_gameInstance = std::make_unique<Game>();

Game* Game::GetInstance()
{
    if(!m_gameInstance.get())
        m_gameInstance = std::make_unique<Game>();
    return m_gameInstance.get();
}

bool Game::Startup()
{
    assert(!m_isStarted);

    SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_TRACE);

    LoadContent();

    if(!Backend::Initialize("Ultra Minecraft clone", 800, 600, true))
    {
	    Backend::Shutdown();
        return false;
    }

    auto systemInterfaceSDL = dynamic_cast<SystemInterface_SDL*>(Backend::GetSystemInterface());
    auto renderInterfaceGL3 = dynamic_cast<RenderInterface_GL3*>(Backend::GetRenderInterface());

    Rml::SetSystemInterface(Backend::GetSystemInterface());
	Rml::SetRenderInterface(Backend::GetRenderInterface());

	// RmlUi initialisation.
	if(!Rml::Initialise())
    {
        Rml::Shutdown();
	    Backend::Shutdown();
        return false;
    }

    SDL_Window* window = systemInterfaceSDL->GetWindow();

	// Create the main RmlUi context.
	m_rmlContext = Rml::CreateContext("main", Rml::Vector2i(800, 600));
	if (!m_rmlContext)
	{
		Rml::Shutdown();
		Backend::Shutdown();
		return false;
	}

	Rml::Debugger::Initialise(m_rmlContext);
	Rml::Debugger::SetVisible(true);
    
    m_chunkWorld = std::make_unique<ChunkWorld>();
    m_camera = std::make_unique<Camera>(window);
    m_cameraController = std::make_unique<CameraController>(m_camera.get());
    m_chunkRenderer = std::make_unique<GL::ChunkRenderer>(m_chunkWorld.get(), m_camera.get(), m_gameSpriteAtlas.get());
    m_isRunning = true;
    m_isStarted = true;

    // Test Scene
    SDL_SetWindowRelativeMouseMode(window, true);
    m_chunkWorld->LoadChunk({0, 0});
    m_chunkWorld->LoadChunk({-1, -1});
    m_camera->SetPosition(glm::vec3(1.0f, 0.0f, 11.0f));
    glClearColor(0.529f, 0.808f, 0.922f, 1.0f); // Sky color
    return true;
}

void Game::Run()
{
    assert(m_isStarted);
    assert(m_isRunning);

    bool sendEventsToUi = false;

	while (m_isRunning)
	{
        if(sendEventsToUi)
		    m_isRunning = Backend::ProcessEvents(m_rmlContext, nullptr, false);
        else
        {
            SDL_Event event;
            while(SDL_PollEvent(&event))
            {
                if(event.type == SDL_EVENT_QUIT)
                {
                    m_isRunning = false;
                }
                else if(event.type == SDL_EVENT_MOUSE_MOTION)
                {
                    m_cameraController->UpdateRotation(event.motion.xrel, event.motion.yrel, 0.2f);
                }
            }
        }
		m_rmlContext->Update();
        m_cameraController->Update(1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_chunkRenderer->Render();
        // Render UI
		Backend::BeginFrame();
		m_rmlContext->Render();
		Backend::PresentFrame();
	}
}

void Game::Shutdown()
{
    assert(m_isStarted);

    m_chunkRenderer.reset();
    m_chunkWorld.reset();
    m_cameraController.reset();
    m_camera.reset();
    m_gameSpriteAtlas.reset();


    // Shutdown RmlUi.
    Rml::Shutdown();
	Backend::Shutdown();

    m_isStarted = false;
}

GameSpriteAtlas &Game::GetGameAtlas() const
{
    return *m_gameSpriteAtlas;
}

void Game::LoadContent()
{
    m_gameSpriteAtlas = std::make_unique<GameSpriteAtlas>();
    m_gameSpriteAtlas->RegisterBlockFaces(BLOCK_BEDROCK, BlockFaceTextures::WithSameFace("assets/block/bedrock.png"));
    m_gameSpriteAtlas->RegisterBlockFaces(BLOCK_STONE, BlockFaceTextures::WithSameFace("assets/block/stone.png"));
    m_gameSpriteAtlas->RegisterBlockFaces(BLOCK_COBBLESTONE, BlockFaceTextures::WithSameFace("assets/block/cobbled_stone_retro.png"));
    m_gameSpriteAtlas->RegisterBlockFaces(BLOCK_DIRT, BlockFaceTextures::WithSameFace("assets/block/dirt.png"));
    m_gameSpriteAtlas->RegisterBlockFaces(BLOCK_GRASS, BlockFaceTextures::WithSameSides(
        "assets/block/grass_side.png",
        "assets/block/grass_retro_top.png",
        "assets/block/dirt.png"
    ));
    m_gameSpriteAtlas->RegisterBlockFaces(BLOCK_GLASS, BlockFaceTextures::WithSameFace("assets/block/glass.png"));
    
    m_gameSpriteAtlas->Pack();
}
