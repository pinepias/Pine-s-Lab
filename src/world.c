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

    BodyList_Create(&(*world)->bodies);
    Vector2_Setv(&(*world)->gravity, gravity);
}
void World_CreateDefault(World **world)
{
    Vector2 gravity = {0.0f, 9.8f};
    World_Create(world, gravity);
}

void World_AddBody(World *world, Body *body)
{
    BodyList_Push(&world->bodies, body);
}


void World_Destroy(World **world)
{
    if (*world != NULL && world != NULL)
    {
        if ((*world)->bodies.bodies != NULL)
        {
            BodyList_Destroy(&(*world)->bodies);
        }

        free(*world);
    }
}

void World_Step(World *world, Window *window, int interations, float time)
{
    for (int j = 0; j < interations; ++j)
    {
        for (int i = 0; i < world->bodies.length; ++i)
        {
            Body_Step(&world->bodies.bodies[i], world, interations, time);
        }

        for (int i = 0; i < world->bodies.length; ++i)
        {
            for (int j = 0; j < world->bodies.length; ++j)
            {
                if ((i == j) || (world->bodies.bodies[i].isStatic && world->bodies.bodies[j].isStatic))
                {
                    continue;
                }

                Vector2 normal;
                float depth;

                if (World_Collide(&world->bodies.bodies[i], &world->bodies.bodies[j], &normal, &depth))
                {
                    Vector2 resolve;

                    if (world->bodies.bodies[i].isStatic)
                    {
                        Vector2_Mult(&resolve, normal, -depth * 0.5f);
                        Body_Move(&world->bodies.bodies[j], resolve);
                    }
                    else if (world->bodies.bodies[j].isStatic)
                    {
                        Vector2_Mult(&resolve, normal, depth * 0.5f);
                        Body_Move(&world->bodies.bodies[i], resolve);
                    }
                    else
                    {
                        Vector2_Mult(&resolve, normal, depth);
                        Body_Move(&world->bodies.bodies[i], resolve);

                        Vector2_Multl(&resolve, -1.0f);
                        Body_Move(&world->bodies.bodies[j], resolve);
                    }

                    World_ResolveCollision(&world->bodies.bodies[i], &world->bodies.bodies[j], normal);
                }
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