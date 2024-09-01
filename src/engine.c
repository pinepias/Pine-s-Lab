#include <SDL2/SDL.h>

#include <stdlib.h>
#include <time.h>

#include "engine.h"
#include "body.h"
#include "collision.h"

float elapsedTime = 0.0f;
float lastTime = 0.0f;

Body box0, box1, circle0, circle1;

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

    Vector2 pos0 = {100.0f, 200.0f};
    Vector2 pos1 = {100.0f, 100.0f};
    Vector2 pos2 = {400.0f, 200.0f};
    Vector2 pos3 = {400.0f, 100.0f};

    Body_NewBox(&box0, pos0, 5.0f, 5.0f, 50.0f, 0.0f, 0.5f, false);
    Body_NewBox(&box1, pos2, 5.0f, 5.0f, 50.0f, 0.0f, 0.5f, false);

    Body_NewCircle(&circle0, pos1, 5.0f, 50.0f, 0.0f, 0.5f, false);
    Body_NewCircle(&circle1, pos3, 5.0f, 50.0f, 0.0f, 0.5f, false);
}

void Engine_Update(Window *window)
{   
    Uint32 currentTime = SDL_GetTicks();
    elapsedTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    float dx = Input_KeyPress(&window->input, SDL_SCANCODE_D) - 
                Input_KeyPress(&window->input, SDL_SCANCODE_A);

    float dy = Input_KeyPress(&window->input, SDL_SCANCODE_S) - 
                Input_KeyPress(&window->input, SDL_SCANCODE_W);
    
    Vector2 velocity = {dx, dy};

    Vector2_Normalizedl(&velocity);
    Vector2_Multl(&velocity, 5000.0f);

    Body_AddForce(&circle0, velocity);
    Body_Step(&circle0, elapsedTime);

    Vector2 normal;
    float depth;

    if (IntersectPolygonCircle(box0.transformedVertices, box0.vertLength, circle0.position, circle0.radius, &normal, &depth))
    {
        Vector2 move;
        Vector2_Mult(&move, normal, depth / -2.0f);
        Body_Move(&circle0, move);
        Vector2_Mult(&move, normal, depth / 2.0f);
        Body_Move(&box0, move);
    }
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

    Body_Debug(&box0, window, Color_CreateRGB(255, 0, 0));
    Body_Debug(&circle0, window, Color_CreateRGB(0, 255, 0));
    Body_Debug(&box1, window, Color_CreateRGB(0, 0, 255));
    Body_Debug(&circle1, window, Color_CreateRGB(255, 255, 0));

    SDL_RenderPresent(window->renderer);
}

void Engine_CleanUp(Window *window)
{
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->window);
    SDL_Quit();
}