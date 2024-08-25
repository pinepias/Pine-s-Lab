#include "types.h"
#include "transform.h"

#include <SDL2/SDL.h>

Transform Transform_New(Vector2 pos, float angle)
{
    Transform __transform__;
    __transform__.x = pos.x;
    __transform__.y = pos.y;

    __transform__.cos = SDL_cosf(angle);
    __transform__.sin = SDL_sinf(angle);

    return __transform__;
}