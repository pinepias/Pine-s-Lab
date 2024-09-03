#ifndef _WORLD_H_
#define _WORLD_H_

#include "types.h"
#include <stdbool.h>

typedef struct Window           Window;
typedef struct World            World;
typedef struct Body             Body;

void World_Create(World **world, Vector2 gravity);
void World_CreateDefault(World **world);
void World_AddBody(World *world, Body *body);
void World_Destroy(World **world);

void World_Step(World *world, Window *window, float time);

void World_ResolveCollision(Body *b0, Body *b1, Vector2 normal);
bool World_Collide(Body *b0, Body *b1, Vector2 *normal, float *depth);

struct World
{
    Vector2 gravity;
    Body *bodies;
    int length;
};

#endif