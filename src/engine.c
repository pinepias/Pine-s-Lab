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
ColorList colorList;

void ColorList_Create(ColorList *list)
{
    list->colors = NULL;
    list->length = 0;
}

void ColorList_Push(ColorList *list, Color color)
{
    int len = list->length + 1;
    list->colors = (Color *)realloc(list->colors, len * sizeof(Color));

    if (list->colors == NULL)
    {
        printf("Error when creating the color list.\n");
        return;
    }

    list->colors[len - 1] = color;
    list->length = len;
}

void ColorList_Remove(ColorList *list, int index)
{
    for (int i = index; i < list->length - 1; ++i)
    {
        list->colors[i] = list->colors[i + 1];
    }   

    int len = list->length - 1;
    Color *temp = (Color *)realloc(list->colors, len * sizeof(Body));

    if (temp == NULL)
    {
        printf("Error when reallocating the bodies.\n");
        return;
    }

    list->colors = temp;
    list->length = len; 
}

void ColorList_Destroy(ColorList *list)
{
    if (list->colors != NULL)
    {
        free(list->colors);
    }
}

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

    Vector2 gravity = {0.0f, 490.0f};
    World_Create(&world, gravity);
    ColorList_Create(&colorList);

    Body ground;
    Vector2 groundPos = {512, 551};

    Body_NewBox(&ground, groundPos, 120.0f, 5.0f, 50.0f, 0.0f, 0.5f, true);
    World_AddBody(world, &ground);
    ColorList_Push(&colorList, Color_CreateRGB(160, 82, 45));
 
    window->frequency = SDL_GetPerformanceFrequency();
    window->lastTime = SDL_GetPerformanceCounter();
}

void Engine_Update(Window *window)
{   
    Uint64 currentTime = SDL_GetPerformanceCounter();
    Uint64 elapsedTicks = currentTime - window->lastTime;
    float elapsedTime = (float)elapsedTicks / window->frequency;

    window->lastTime = currentTime;
    World_Step(world, window, 20, elapsedTime);

    if (Input_MousePressed(&window->input, 0))
    {
        float posX = window->input.mouse_x;
        float posY = window->input.mouse_y;

        ShapeType shape = (rand() % 2 == 0) ? Box : Circle;

        if (shape == Box)
        {
            Body box;
            Vector2 position = {posX, posY}; 
            Color color = Color_CreateRGB(rand() % 255, rand() % 255, rand() % 255);

            int width = (rand() % 5) + 4;
            int height = (rand() % 5) + 4;

            Body_NewBox(&box, position, width, height, 50.0f, 0.0f, 0.5f, false);
            World_AddBody(world, &box);
            ColorList_Push(&colorList, color);
            Body_Destroy(&box);
        }
        else
        {
            Body circle;
            Vector2 position = {posX, posY}; 
            Color color = Color_CreateRGB(rand() % 255, rand() % 255, rand() % 255);

            int radius = (rand() % 5) + 2;

            Body_NewCircle(&circle, position, radius, 50.0f, 0.0f, 0.5f, false);
            World_AddBody(world, &circle);
            ColorList_Push(&colorList, color);
        }
    }

    for (int i = 0; i < world->bodies.length; ++i)
    {
        Body_GetAABB(&world->bodies.bodies[i]);

        if (world->bodies.bodies[i].aabb[1][1] > world->bodies.bodies->aabb[1][1])
        {
            BodyList_Remove(&world->bodies, i);
            ColorList_Remove(&colorList, i);
        }
    }

    printf("Body Count: %i\n", world->bodies.length);
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
    
    for (int i = 0; i < world->bodies.length; ++i)
    {
        Body_Debug(&world->bodies.bodies[i], window, colorList.colors[i]);
    }

    SDL_RenderPresent(window->renderer);
}

void Engine_CleanUp(Window *window)
{
    World_Destroy(&world);
    ColorList_Destroy(&colorList);
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->window);
    SDL_Quit();
}