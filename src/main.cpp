#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>
#include "RmlUiBackend/RmlUi_Backend.h"
#include "RmlUiBackend/RmlUi_Include_GL3.h"
#include "RmlUiBackend/RmlUi_Renderer_GL3.h"
#include "RmlUiBackend/RmlUi_Platform_SDL.h"
#include "Rendering/Camera.hpp"
#include "CameraController.hpp"
#include "World/ChunkWorld.hpp"
#include "Rendering/OpenGL/ChunkRenderer.hpp"

int main()
{
    if(!Backend::Initialize("Rml backend window", 800, 600, true))
    {
	    Backend::Shutdown();
        return -1;
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
        return -1;
    }

    SDL_Window* window = systemInterfaceSDL->GetWindow();

	// Create the main RmlUi context.
	Rml::Context* context = Rml::CreateContext("main", Rml::Vector2i(800, 600));
	if (!context)
	{
		Rml::Shutdown();
		Backend::Shutdown();
		return -1;
	}

	Rml::Debugger::Initialise(context);
	Rml::Debugger::SetVisible(true);
    
    ChunkWorld world;
    world.LoadChunk({0, 0});
    world.LoadChunk({-1, -1});
    
    Camera camera(window);
    camera.SetPosition(glm::vec3(1.0f, 0.0f, 11.0f));
    CameraController cameraController(&camera);

    GL::ChunkRenderer renderer(&world, &camera);

    SDL_SetWindowRelativeMouseMode(window, true);

    glClearColor(0.0, 0.0, 1.0, 1.0);
    
    bool running = true;
    bool sendEventsToUi = false;

	while (running)
	{
        if(sendEventsToUi)
		    running = Backend::ProcessEvents(context, nullptr, false);
        else
        {
            SDL_Event event;
            while(SDL_PollEvent(&event))
            {
                if(event.type == SDL_EVENT_QUIT)
                {
                    running = false;
                }
                else if(event.type == SDL_EVENT_MOUSE_MOTION)
                {
                    cameraController.UpdateRotation(event.motion.xrel, event.motion.yrel, 0.2f);
                }
            }
        }
		context->Update();
        cameraController.Update(1.0f);

        glClear(GL_COLOR_BUFFER_BIT);
        renderer.Render();
		Backend::BeginFrame();
		context->Render();
		Backend::PresentFrame();
	}

	// Shutdown RmlUi.
	Rml::Shutdown();
	Backend::Shutdown();
    return 0;
}