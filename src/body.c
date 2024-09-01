#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "body.h"
#include "engine.h"

bool Body_NewBox(Body *body, Vector2 position, float width, float height, float mass,
                 float rotation, float resistituion, bool isStatic)
{
    Vector2_Setv(&body->position, position);

    body->width = (width * 10.0f);
    body->height = (height * 10.0f);
    body->radius = 0.0f;

    float volume = (width * height * 1.0f);

    body->mass = mass;
    body->density = mass/volume;

    body->rotation = rotation;
    body->rotationVelocity = 0.0f;

    body->resistituion = resistituion;

    body->vertLength = 4;
    body->vertices = calloc(body->vertLength, sizeof(Vector2));
    body->transformedVertices = calloc(body->vertLength, sizeof(Vector2));

    if (body->vertices == NULL || body->transformedVertices == NULL)
    {
        printf("Error creating new vertices\n");
        return false;
    }
 
    Body_UpdateBox(body);

    body->isStatic = isStatic;
    body->shape = Box;

    Vector2_SetZero(&body->force);
    Vector2_SetZero(&body->linearVelocity);

    return true;
}

bool Body_NewCircle(Body *body, Vector2 center, float radius, float mass, 
                float rotation, float resistituion, bool isStatic)
{
    Vector2_Setv(&body->position, center);
    float volume = ((powf(radius, 2.0f) * PI));

    body->width = 0.0f;
    body->height = 0.0f;
    body->radius = (radius * 6.0f);

    body->mass = mass;
    body->density = mass/volume;

    body->rotation = rotation;
    body->rotationVelocity = 0.0f;

    body->resistituion = resistituion;

    body->isStatic = isStatic;
    body->shape = Circle;

    Vector2_SetZero(&body->force);
    Vector2_SetZero(&body->linearVelocity);

    return true;
}

void Body_Debug(Body *body, Window *window, Color color)
{
    SDL_SetRenderDrawColor(window->renderer, color.r, color.g, color.b, color.a);

    switch (body->shape)
    {
        case Box:


            float minY = FLT_MAX, maxY = -FLT_MAX;

            for (int i = 0; i < 4; i++) 
            {
                if (body->transformedVertices[i][1] < minY) minY = body->transformedVertices[i][1];
                if (body->transformedVertices[i][1] > maxY) maxY = body->transformedVertices[i][1];
            }

            for (int y = (int)minY; y <= (int)maxY; y++) 
            {

                float intersections[4];
                int count = 0;

                for (int i = 0; i < 4; i++) 
                {
                    int next = (i + 1) % 4;

                    if ((body->transformedVertices[i][1] <= y && body->transformedVertices[next][1] > y) || (body->transformedVertices[next][1] <= y && body->transformedVertices[i][1] > y)) 
                    {
                        float x = body->transformedVertices[i][0] + (y - body->transformedVertices[i][1]) * (body->transformedVertices[next][0] - body->transformedVertices[i][0]) / (body->transformedVertices[next][1] - body->transformedVertices[i][1]);
                        intersections[count++] = x;
                    }
                }

                for (int i = 0; i < count - 1; i++) 
                {
                    for (int j = i + 1; j < count; j++) 
                    {
                        if (intersections[j] < intersections[i]) 
                        {
                            float temp = intersections[i];
                            intersections[i] = intersections[j];
                            intersections[j] = temp;
                        }
                    }
                }

                for (int i = 0; i < count; i += 2) 
                {
                    if (i + 1 < count) 
                    {
                        SDL_RenderDrawLine(window->renderer, (int)intersections[i], y, (int)intersections[i + 1], y);
                    }
                }
            }
        break;

        case Circle:
            for (int y = -body->radius; y <= body->radius; y++) 
            {
                for (int x = -body->radius; x <= body->radius; x++) 
                {
                    if (x * x + y * y < body->radius * body->radius) 
                    {
                        SDL_RenderDrawPoint(window->renderer,
                                            body->position[0] + x, 
                                            body->position[1] + y);
                    }
                }
            }
        break;
    }

    SDL_SetRenderDrawColor(window->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}

void Body_Step(Body *body, float time)
{
    /*
        f = m * a
        > a = f/m
    */
    Vector2 accelaration;
    Vector2_Div(&accelaration, body->force, body->mass);

    /*
        a = (vf - vi)/t
        a * t = (vf - vi)
        vf = vi + a * t

        > vf = vi + a*t
    */

    Vector2_Multl(&accelaration, time);
    Vector2_Addl(body->linearVelocity, accelaration);

    /*
        v = s/t;
        s = v * t;
        s = xf - xi;
        xf = xi + s

        > xf = xi + (v * t)
    */

    Vector2 currentVeloc;
    Vector2_Mult(&currentVeloc, body->linearVelocity, time);
    Vector2_Addl(body->position, currentVeloc);
    Vector2_SetZero(body->force);

    Body_UpdateBox(body);
}

void Body_AddForce(Body *body, Vector2 amount)
{
    Vector2_Setv(body->force, amount);
}

void Body_UpdateBox(Body *box)
{
    if (box->shape == Box)
    {
        Vector2_Set(box->vertices[0], (box->position[0] - (box->width / 2.0f)), 
                                    (box->position[1] - (box->height / 2.0f)));
        
        Vector2_Set(box->vertices[1], (box->position[0] + (box->width / 2.0f)), 
                                    (box->position[1] - (box->height / 2.0f)));

        Vector2_Set(box->vertices[2], (box->position[0] + (box->width / 2.0f)), 
                                    (box->position[1] + (box->height / 2.0f)));
        
        Vector2_Set(box->vertices[3], (box->position[0] - (box->width / 2.0f)), 
                                    (box->position[1] + (box->height / 2.0f)));

        for (int i = 0; i < box->vertLength; ++i)
        {
            Transform transform;
            Transform_Set(&transform, 0.0f, 0.0f, box->rotation);

            Vector2_Sub(&box->transformedVertices[i], box->vertices[i], box->position);
            Vector2_Transformvl(&box->transformedVertices[i], transform);
            Vector2_Addl(&box->transformedVertices[i], box->position);
        }
    }
}

void Body_Move(Body *body, Vector2 amount)
{
    Vector2_Addl(&body->position, amount);
    Body_UpdateBox(body);
}