#include <SDL2/SDL.h>

#include <stdlib.h>
#include <time.h>

#include "engine.h"
#include "body.h"
#include "collision.h"
#include "world.h"

float elapsedTime = 0.0f;
float lastTime = 0.0f;

World *world;
Color colors[BODY_COUNT];

void Engine_Init(const char *title, int width, int height, Window *window)
{
    srand(time(NULL));

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
                                        SDL_RENDERER_ACCELERATED);

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

    World_CreateDefault(&world);

    for (int i = 0; i <= BODY_COUNT; ++i)
    {
        float posX = (rand() % 999) + 25;
        float posY = (rand() % 551) + 25;

        ShapeType shape = (rand() % 2 == 0) ? Box : Circle;
        colors[i] = Color_CreateRGB(rand() % 255, rand() % 255, rand() % 255);

        if (shape == Box)
        {
            Body box;
            Vector2 position = {posX, posY};
            Body_NewBox(&box, position, 5.0f, 5.0f, 50.0f, 0.0f, 0.5f, false);
            World_AddBody(world, &box);
            Body_Destroy(&box);
        }
        else
        {
            Body circle;
            Vector2 position = {posX, posY};
            Body_NewCircle(&circle, position, 5.0f, 50.0f, 0.0f, 0.5f, false);
            World_AddBody(world, &circle);
        }
    }

    window->frequency = SDL_GetPerformanceFrequency();
    window->lastTime = SDL_GetPerformanceCounter();
}

void Engine_Update(Window *window)
{   
    Uint64 currentTime = SDL_GetPerformanceCounter();
    Uint64 elapsedTicks = currentTime - window->lastTime;
    float elapsedTime = (float)elapsedTicks / window->frequency;

    window->lastTime = currentTime;
    World_Step(world, window, elapsedTime);

    Uint64 start = SDL_GetPerformanceCounter();
    SDL_Delay(1);
    Uint64 end = SDL_GetPerformanceCounter();

    if (end > start) 
    {
        window->totalTicks += (end - start);
        window->trialCount++;
    }

    if (window->trialCount >= 100) 
    {
        double avgTicks = (double)window->totalTicks / 100;
        window->avgMillis = (avgTicks * 1000) / window->frequency;
            
        window->totalTicks = 0;
        window->trialCount = 0;
    }

    SDL_Delay(window->avgMillis);
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

    for (int i = 0; i < world->length; ++i)
    {
        Body_Debug(&world->bodies[i], window, colors[i]);
    }

    SDL_RenderPresent(window->renderer);
}

void Engine_CleanUp(Window *window)
{
    World_Destroy(&world);
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->window);
    SDL_Quit();
}