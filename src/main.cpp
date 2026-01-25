#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>
#include "RmlUiBackend/RmlUi_Backend.h"
#include "RmlUiBackend/RmlUi_Include_GL3.h"
#include "RmlUiBackend/RmlUi_Renderer_GL3.h"
#include "RmlUiBackend/RmlUi_Platform_SDL.h"
#include "Rendering/GLProgram.hpp"
#include "Rendering/Camera.hpp"
#include "CameraController.hpp"

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

    const int VERTEX_COUNT = 6;
    GLfloat vertices[] =
    {
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,

         0.5f, 0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
    };
    GLuint VBO;
    glGenBuffers(1, &VBO);

    GLShaderProgram program;
    if(!program.Load("assets/shaders/triangle.vs", "assets/shaders/triangle.fs"))
    {
        return -1;
    }

    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    // 2. Копируем наш массив вершин в буфер для OpenGL
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
    // 3. Устанавливаем указатели на вершинные атрибуты 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0); 
    //4. Отвязываем VAO
    glBindVertexArray(0); 
    ////////////////////////////////
    Camera camera(window);
    camera.SetPosition(glm::vec3(1.0f, 0.0f, 11.0f));
    CameraController cameraController(&camera);

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
        program.Use();
        program.SetUniformMat4x4("projection_view", camera.CalcMatrix());
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, VERTEX_COUNT);
        glBindVertexArray(0);

		Backend::BeginFrame();
		context->Render();
		Backend::PresentFrame();
	}

	// Shutdown RmlUi.
	Rml::Shutdown();
	Backend::Shutdown();
    return 0;
}