#include <SDL2/SDL.h>
#include "engine.h"
#include "shape.h"
#include "collision.h"
#include <math.h>
#include <time.h>

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
}

void Engine_Update(Window *window)
{

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



    Color color = Color_CreateRGB(255, 255, 255);
    Rectangle rect0 = Shape_NewRect(window->input.mouse_x, window->input.mouse_y, 100, 100, 0.0);
    Rectangle rect1 = Shape_NewRect(500, 450, 100, 100, 3.14*(SDL_GetTicks()/10)/180);
    Rectangle rect2 = Shape_NewRect(800, 400, 100, 100, 0.0);

    Circle circle0 = Shape_NewCircle(100, 100, 50, 0.0);
    

    Shape_DebugRect(rect0, color, window);
    Shape_DebugRect(rect1, color, window);
    Shape_DebugRect(rect2, color, window);
    Shape_DebugCircle(circle0, color, window);  

    if (Collision_DetectPolygon(rect0.points, 4, rect1.points, 4))
    {
        Shape_DebugRect(rect0, Color_CreateRGB(255, 0, 0), window);
        Shape_DebugRect(rect1, Color_CreateRGB(255, 0, 0), window);
    }

    if (Collision_DetectPolygon(rect0.points, 4, rect2.points, 4))
    {
        Shape_DebugRect(rect0, Color_CreateRGB(255, 0, 0), window);
        Shape_DebugRect(rect2, Color_CreateRGB(255, 0, 0), window);
    }

    if (Collision_DetectPolygon(rect0.points, 4, circle0.points, CIRCLE_POINTS_COUNT))
    {
        Shape_DebugRect(rect0, Color_CreateRGB(255, 0, 0), window);
        Shape_DebugCircle(circle0, Color_CreateRGB(255, 0, 0), window);
    }

    SDL_RenderPresent(window->renderer);
}

void Engine_CleanUp(Window *window)
{
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->window);
    SDL_Quit();
}
