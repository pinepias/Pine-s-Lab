#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "world.h"
#include "body.h"
#include "engine.h"
#include "collision.h"

void World_Create(World **world, Vector2 gravity)
{
    (*world) = (World *)malloc(sizeof(World));

    if (*world == NULL)
    {
        printf("Error in creating the world.\n");
        return;
    }

    (*world)->bodies = NULL;
    (*world)->length = 0;
    Vector2_Setv(&(*world)->gravity, gravity);
}
void World_CreateDefault(World **world)
{
    Vector2 gravity = {0.0f, 9.8f};
    World_Create(world, gravity);
}

void World_AddBody(World *world, Body *body)
{
    int len = world->length + 1;
    world->bodies = (Body *)realloc(world->bodies, len * sizeof(Body));

    if (world->bodies == NULL)
    {
        printf("Error in reallocating memory for bodies.\n");
        return;
    }

    Body result;
    memcpy(&result, body, sizeof(Body));

    result.vertices = NULL;
    result.transformedVertices = NULL;
    result.vertLength = 0;

    if (body->shape == Box)
    {
        result.vertLength = 4;
        result.vertices = (Vector2 *)calloc(result.vertLength, sizeof(Vector2));
        result.transformedVertices = (Vector2 *)calloc(result.vertLength, sizeof(Vector2));

        if (result.vertices == NULL || result.transformedVertices == NULL)
        {
            printf("Error in allocating memory for vertices.\n");
            return;
        }

        memcpy(result.vertices, body->vertices, result.vertLength * sizeof(Vector2));
        memcpy(result.transformedVertices, body->transformedVertices, result.vertLength * sizeof(Vector2));
    }

    world->bodies[len - 1] = result;
    world->length = len;
}


void World_Destroy(World **world)
{
    if (*world != NULL && world != NULL)
    {
        if ((*world)->bodies != NULL)
        {
            for (int i = 0; i < (*world)->length; ++i)
            {
                Body_Destroy(&(*world)->bodies[i]);
            }

            free((*world)->bodies);
        }

        free(*world);
    }
}

void World_Step(World *world, Window *window, float time)
{
    float dx = Input_KeyPress(&window->input, SDL_SCANCODE_D) - 
            Input_KeyPress(&window->input, SDL_SCANCODE_A);
    
    float dy = Input_KeyPress(&window->input, SDL_SCANCODE_S) - 
            Input_KeyPress(&window->input, SDL_SCANCODE_W);

    Vector2 velocity = {dx, dy};
    Vector2_Normalizedl(&velocity);
    Vector2_Multl(&velocity, 10000.0f);

    Body_AddForce(&world->bodies[0], velocity);

    for (int i = 0; i < world->length; ++i)
    {
        Body_Step(&world->bodies[i], time);
    }

    for (int i = 0; i < world->length; ++i)
    {
        for (int j = 0; j < world->length; ++j)
        {
            if ((i == j) || (world->bodies[i].isStatic && world->bodies[j].isStatic))
            {
                continue;
            }

            Vector2 normal;
            float depth;

            if (World_Collide(&world->bodies[i], &world->bodies[j], &normal, &depth))
            {
                Vector2 resolve;

                if (world->bodies[i].isStatic)
                {
                    Vector2_Mult(&resolve, normal, -depth);
                    Body_Move(&world->bodies[j], resolve);
                }
                else if (world->bodies[j].isStatic)
                {
                    Vector2_Mult(&resolve, normal, depth);
                    Body_Move(&world->bodies[i], resolve);
                }
                else
                {
                    Vector2_Mult(&resolve, normal, depth);
                    Body_Move(&world->bodies[i], resolve);

                    Vector2_Multl(&resolve, -1.0f);
                    Body_Move(&world->bodies[j], resolve);
                }

                World_ResolveCollision(&world->bodies[i], &world->bodies[j], normal);
            }
        }
    }
}

bool World_Collide(Body *b0, Body *b1, Vector2 *normal, float *depth)
{
    if (b0->shape == Box)
    {
        switch (b1->shape)
        {
            case Box:
                return (IntersectPolygon(b1->transformedVertices, b1->vertLength, 
                                        b0->transformedVertices, b0->vertLength, 
                                        normal, depth));
            break;

            case Circle:
                return (IntersectPolygonCircle(b0->transformedVertices, b0->vertLength,
                                            b1->position, b1->radius, 
                                            normal, depth));
            break;
        }
    }
    else
    {
        if (b0->shape == Circle)
        {
            return (IntersectCircle(b1->position, b1->radius, b0->position, b0->radius, normal, depth));
        }
    }
}

void World_ResolveCollision(Body *b0, Body *b1, Vector2 normal)
{
    Vector2 relativeVelocity;
    Vector2_Sub(&relativeVelocity, b1->linearVelocity, b0->linearVelocity);

    if (Vector2_Dot(relativeVelocity, normal) < 0.0f)
    {
        return;
    }

    float e = SDL_min(b0->resistituion, b1->resistituion);
    float j = ((-(1 + e) * Vector2_Dot(relativeVelocity, normal)) / (b0->invMass + b1->invMass));

    Vector2 impulse;
    Vector2_Mult(&impulse, normal, j);
    Vector2_Multl(&impulse, b0->invMass);
    Vector2_Subl(&b0->linearVelocity, impulse);

    Vector2_Mult(&impulse, normal, j);
    Vector2_Multl(&impulse, b1->invMass);
    Vector2_Addl(&b1->linearVelocity, impulse);
}