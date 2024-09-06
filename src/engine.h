#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <stdbool.h>
#include <SDL2/SDL_events.h>

typedef struct SDL_Window           SDL_Window;
typedef struct SDL_Renderer         SDL_Renderer;

typedef struct Window               Window;
typedef struct Color                Color;
typedef struct Input                Input;
typedef struct Clock                Clock;

typedef struct ColorList            ColorList;

void Engine_Init(const char *title, int width, int height, Window *window);
void Engine_Events(Window *window);
void Engine_Update(Window *window);
void Engine_Render(Window *window);
void Engine_CleanUp(Window *window);

void Input_Begin(Input *input);

void Input_KeyDownEvent(Input *input, SDL_Event event);
void Input_KeyUpEvent(Input *input, SDL_Event event);
void Input_MouseDownEvent(Input *input, SDL_Event event);
void Input_MouseUpEvent(Input *input, SDL_Event event);
void Input_MouseMoveEvent(Input *input, SDL_Event event);

bool Input_KeyPress(Input *input, SDL_Scancode code);
bool Input_KeyPressed(Input *input, SDL_Scancode code);
bool Input_KeyRealese(Input *input, SDL_Scancode code);

bool Input_MousePress(Input *input, int code);
bool Input_MousePressed(Input *input, int code);
bool Input_MouseRealese(Input *input, int code);

Color Color_CreateRGB(int r, int g, int b);

void ColorList_Create(ColorList *list);
void ColorList_Push(ColorList *list, Color color);
void ColorList_Remove(ColorList *list, int index);
void ColorList_Destroy(ColorList *list);

struct Color
{
    Uint8 r, g, b, a;
};

struct Input
{
    bool isKeyPressed[SDL_NUM_SCANCODES];
    bool isKeyPress[SDL_NUM_SCANCODES];
    bool isKeyRealese[SDL_NUM_SCANCODES];

    bool mousePress[3];
    bool mousePressed[3];
    bool mouseRealese[3];

    int mouse_x, mouse_y;
};

struct Window
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    Input input;
    bool running;

    Uint64 frequency;
    Uint64 lastTime;
    Uint64 totalTicks;
    int trialCount;

    Uint32 avgMillis;
};

struct ColorList
{
    Color *colors;
    int length;
};

#endif