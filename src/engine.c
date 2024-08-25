#include <SDL2/SDL.h>

#include <stdlib.h>
#include <time.h>

#include "engine.h"
#include "body.h"
#include "types.h"
#include "collision.h"
#include "world.h"

Color colors[BODY_COUNT];
World *world;
float elapsedTime = 0.0f;
float lastTime = 0.0f;

void Engine_Init(const char *title, int width, int height, Window *window)
{
    srand(time(NULL));

    Body box;
    Body_CreateBox(&box, Vector2_Zero(), 5.0f, 5.0f, 2.0f, 0.5f, 0.0f, false);

    world = CREATE_DEFAULT_WORLD;
    

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("Error starting SDL.\n");
        window->running = false;
        return;
    }

    window->window = SDL_CreateWindow(
                                title, 
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                width, height, 
                                0);
    
    if (window->window == NULL)
    {
        printf("Error when creating the window.\n");
        window->running = false;
        return;
    }

    window->renderer = SDL_CreateRenderer(
                                        window->window, 
                                        -1, 
                                        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (window->renderer == NULL)
    {
        printf("Error when creating a render.\n");
        window->running = false;
        return;
    }

    window->running = true;
    SDL_memset(window->input.isKeyPress, 0, SDL_NUM_SCANCODES);
    SDL_memset(window->input.isKeyRealese, 0, SDL_NUM_SCANCODES);

    SDL_memset(window->input.mousePressed, 0, 3);
    SDL_memset(window->input.mouseRealese, 0, 3);

    for (int i = 0; i < BODY_COUNT; ++i)
    {
        float posX = clamp((rand() % 1024), 100, 1024 - 100);
        float posY = clamp((rand() % 576), 100, 576 - 100);

        ShapeType shape = ((rand() % 2) == 0) ? Box : Circle;
        colors[i] = Color_CreateRGB(rand() % 255, rand() % 255, rand() % 255);

        Body body;

        switch (shape)
        {
            case Box: 
                Body_CreateBox(&body, Vector2_New(posX, posY), 5.0f, 5.0f, 2.0f, 0.5f, 0.0f, false); 
                World_AddBox(world, body, body.vertices, body.transformedVertices);
            break;

            case Circle: 
                Body_CreateCircle(&body, Vector2_New(posX, posY), 5.0f, 2.0f, 0.0f, 0.5f, false); 
                World_AddCircle(world, body);
            break;
        }
        
    }
}

void Engine_Update(Window *window)
{   
    Uint32 currentTime = SDL_GetTicks();
    elapsedTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    World_Step(world, window, elapsedTime);
}

void Input_Begin(Input *input)
{
    SDL_memset(input->isKeyPressed, 0, SDL_NUM_SCANCODES);
    SDL_memset(input->isKeyRealese, 0, SDL_NUM_SCANCODES);

    SDL_memset(input->mousePressed, 0, 3);
    SDL_memset(input->mouseRealese, 0, 3);
}

void Input_KeyDownEvent(Input *input, SDL_Event event)
{
    input->isKeyPress[event.key.keysym.scancode] = true;
    input->isKeyPressed[event.key.keysym.scancode] = true;
}

void Input_KeyUpEvent(Input *input, SDL_Event event)
{
    input->isKeyRealese[event.key.keysym.scancode] = true;
    input->isKeyPress[event.key.keysym.scancode] = false;
}

void Input_MouseDownEvent(Input *input, SDL_Event event)
{
    input->mousePress[event.button.button - 1]  = true;
    input->mousePressed[event.button.button - 1]  = true;
}

void Input_MouseUpEvent(Input *input, SDL_Event event)
{
    input->mousePress[event.button.button - 1]  = false;
    input->mouseRealese[event.button.button - 1]  = true;
}

void Input_MouseMoveEvent(Input *input, SDL_Event event)
{
    if (event.type == SDL_MOUSEMOTION)
    {
        input->mouse_x = event.button.x;
        input->mouse_y = event.button.y;
    }
}

bool Input_KeyPress(Input *input, SDL_Scancode code)
{
    return input->isKeyPress[code];
}

bool Input_KeyPressed(Input *input, SDL_Scancode code)
{
    return input->isKeyPressed[code];
}

bool Input_KeyRealese(Input *input, SDL_Scancode code)
{
    return input->isKeyRealese[code];
}

void Engine_Events(Window *window)
{
    SDL_Event event;
    Input_Begin(&window->input);

    while (SDL_PollEvent(&event))
    {
        Input_MouseMoveEvent(&window->input, event);

        switch (event.type)
        {
            case SDL_QUIT: window->running = false; break;
            case SDL_KEYDOWN: if (event.key.repeat == 0) Input_KeyDownEvent(&window->input, event); break;
            case SDL_KEYUP: Input_KeyUpEvent(&window->input, event); break;
            case SDL_MOUSEBUTTONDOWN: Input_MouseDownEvent(&window->input, event); break;
            case SDL_MOUSEBUTTONUP: Input_MouseUpEvent(&window->input, event); break;
        }
    }

    
}

bool Input_MousePress(Input *input, int code)
{
    return input->mousePress[code];
}

bool Input_MousePressed(Input *input, int code)
{
    return input->mousePressed[code];
}

bool Input_MouseRealese(Input *input, int code)
{
    return input->mouseRealese[code];
}

Color Color_CreateRGB(int r, int g, int b)
{
    Color color = {r, g, b, SDL_ALPHA_OPAQUE};
    return color;
}

void Engine_Render(Window *window)
{
    SDL_SetRenderDrawColor(window->renderer, 35, 35, 35, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(window->renderer);

    for (int i = 0; i < BODY_COUNT; ++i)
    {
        Body_Debug(&world->bodyList[i], colors[i], window);
    }

    SDL_RenderPresent(window->renderer);
}

void Engine_CleanUp(Window *window)
{
    World_Clear(world);
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->window);
    SDL_Quit();
}
