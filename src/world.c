#include <stdio.h> 
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "world.h"
#include "body.h"
#include "collision.h"
#include "engine.h"

World *World_CreateWorld(Vector2 gravity)
{
    World *world = (World *)malloc(sizeof(World));

    if (world == NULL)
    {
        printf("Error in creating the world.\n");
        return NULL;
    }

    world->gravity = gravity;
    world->bodyList = NULL;
    world->bodyCount = 0;

    return world;
}

void World_AddBox(World *world, Body body, Vector2 *vertices, Vector2 *transformedVertices)
{
    world->bodyCount = world->bodyCount + 1;
    world->bodyList = (Body *)realloc(world->bodyList, world->bodyCount * sizeof(Body));

    if  (world->bodyList == NULL)
    {
        printf("Error adding new bodies to the world.\n");
        return;
    }

    SDL_memcpy(&body.vertices, &vertices, sizeof(Vector2 *));
    SDL_memcpy(&body.transformedVertices, &transformedVertices, sizeof(Vector2 *));

    world->bodyList[world->bodyCount - 1] = body;
}

void World_AddCircle(World *world, Body body)
{
    world->bodyCount = world->bodyCount + 1;
    world->bodyList = (Body *)realloc(world->bodyList, world->bodyCount * sizeof(Body));

    if  (world->bodyList == NULL)
    {
        printf("Error adding new bodies to the world.\n");
        return;
    }

    world->bodyList[world->bodyCount - 1] = body;
}

void World_RemoveBody(World *world, int index)
{
    if (world->bodyCount > index)
    {
        if (world->bodyCount > 1)
        {
            world->bodyCount = world->bodyCount - 1;

            for (int i = 0; i < world->bodyCount; ++i)
            {
                world->bodyList[i] = world->bodyList[i + 1];
            }

            world->bodyList = (Body *)realloc(world->bodyList, world->bodyCount * sizeof(Body));

            if (world->bodyList == NULL)
            {
                printf("Error to remove a body from the world.\n");
                return;
            }
        }
        else
        {
            World_Clear(world);
        }
    }
}

void World_Clear(World *world)
{
    if (world->bodyList != NULL)
    {
        free(world->bodyList);
        world->bodyList = NULL;
        world->bodyCount = 0;
        world->gravity = Vector2_Zero();
    }
}

void World_Step(World *world, Window *window, float time)
{
    //Movement

    int dx = Input_KeyPress(&window->input, SDL_SCANCODE_D) - 
            Input_KeyPress(&window->input, SDL_SCANCODE_A);

    int dy = Input_KeyPress(&window->input, SDL_SCANCODE_S) - 
            Input_KeyPress(&window->input, SDL_SCANCODE_W);

    float forceMag = 4000;
    Vector2 amount = Vector2_Mult(Vector2_Normalized(Vector2_New(dx, dy)), forceMag);

    Body_AddForce(&world->bodyList[0], amount);

    for (int i = 0; i < BODY_COUNT; ++i)
    {
        Body_Step(&world->bodyList[i], time);
    }

    //Collision Detection

    for (int i = 0; i < world->bodyCount; ++i)
    {
        for (int j = 0; j < world->bodyCount; ++j)
        {   
            if (i == j)
            {
                continue;
            }

            Body *b0 = &world->bodyList[i];
            Body *b1 = &world->bodyList[j];

            Vector2 normal;
            float depth;

            if (World_CollisionDetection(*b0, *b1, &normal, &depth))
            {
                Body_Move(b0, Vector2_Mult(normal, depth / -2.0f));
                Body_Move(b1, Vector2_Mult(normal, depth / 2.0f));
                World_ResolveCollision(b0, b1, normal, depth);
            }
        }
    }
}

bool World_CollisionDetection(Body body0, Body body1, Vector2 *normal, float *depth)
{
    *normal = Vector2_Zero();
    *depth = 0.0f;

    ShapeType shape0 = body0.shapeType;
    ShapeType shape1 = body1.shapeType;

    if (shape0 == Box)
    {
        if (shape1 == Box)
        {
            return IntersectPolygon(body0.transformedVertices, 4, 
                                    body1.transformedVertices, 4, 
                                    normal, depth);

        }
        else if (shape1 == Circle)
        {
            bool result = IntersectPolygonCircle(body1.position, body1.radius, 
                                    body0.transformedVertices, 4, 
                                    normal, depth);

            *normal = Vector2_Mult(*normal, -1);
            return result;
        }
    }
    else if (shape1 == Circle)
    {
        if (shape0 == Box)
        {
            return IntersectPolygonCircle(body1.position, body1.radius, 
                                    body0.transformedVertices, 4, 
                                    normal, depth);
        }
        else if (shape0 == Circle)
        {
            return IntersectCircles(body0.position, body0.radius, 
                                    body1.position, body1.radius, 
                                    normal, depth);
        }
    }

    return false;
}

void World_ResolveCollision(Body *b0, Body *b1, Vector2 normal, float depth)
{
    Vector2 relativeVelocity = Vector2_Subtr(b1->linerVelocity, b0->linerVelocity);
    float e = SDL_min(b0->restitution, b1->restitution);
    
    float j = Vector2_Dot(Vector2_Mult(relativeVelocity, -(1.0f + e)), normal);
    j = j / ((1.0f/b0->mass) + (1.0f/b1->mass));

    b0->linerVelocity = Vector2_Add(b0->linerVelocity, Vector2_Mult(normal, -(j/b0->mass)));
    b1->linerVelocity = Vector2_Add(b1->linerVelocity, Vector2_Mult(normal, (j/b1->mass)));
}
