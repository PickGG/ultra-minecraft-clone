#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <GLES3/gl3.h>
#include "Rendering/GLProgram.hpp"

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Ultra Minecraft clone", 600, 400,
        SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
            "SDL_GL_CreateContext failed! SDL_Error says: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    int interval;
    if(SDL_GL_GetSwapInterval(&interval))
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
            "SDL_GL_GetSwapInterval is set to %d", interval);
    }

    ////////////////////////////////
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };
    GLuint VBO;
    glGenBuffers(1, &VBO);

    GLShaderProgram program;
    if(!program.Load("assets/shaders/triangle.vs", "assets/shaders/triangle.fs"))
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
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

    glClearColor(0.1333f, 0.1333f, 0.2667f, 1.0f);

    bool running = true;

    while(running)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_EVENT_QUIT)
                running = false;
        }
        int width, height;
        SDL_GetWindowSizeInPixels(window, &width, &height);
        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT);
        program.Use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
};