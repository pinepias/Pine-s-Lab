#ifndef _WORLD_H_
#define _WORLD_H_

#include "types.h"
#include <stdbool.h>

typedef struct World            World;
typedef struct Body             Body;
typedef struct Window           Window;

#define CREATE_DEFAULT_WORLD World_CreateWorld(Vector2_New(0.0f, 9.81f));

World *World_CreateWorld(Vector2 gravity);

void World_AddBox(World *world, Body body, Vector2 *vertices, Vector2 *transformedVertices);
void World_AddCircle(World *world, Body body);

void World_RemoveBody(World *world, int index);
void World_Clear(World *world);

void World_Step(World *world, Window *window, float time);
void World_ResolveCollision(Body *b0, Body *b1, Vector2 normal, float depth);

bool World_CollisionDetection(Body body0, Body body1, Vector2 *normal, float *depth);

struct World
{
    Vector2 gravity;
    Body *bodyList;
    int bodyCount;
};

#endif