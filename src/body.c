#include "engine.h"
#include "body.h"
#include "transform.h"
#include <SDL2/SDL.h>

bool Body_CreateBox(Body *body, Vector2 position, float width, float height, float density, 
                    float restitution, float rotation, bool isStatic)
{
    body->area = (width * height * 1);
    body->mass = density * body->area;

    body->position = position;
    body->rotation = rotation;

    body->force = Vector2_Zero();
    body->linerVelocity = Vector2_Zero();
    body->density = density;
    body->restitution = restitution;
    body->width = (10 * width);
    body->height = (10 * height);
    body->isStatic = isStatic;
    body->shapeType = Box;

    body->vertices = (Vector2 *)SDL_malloc(4 * sizeof(Vector2));

    if (body->vertices == NULL)
    {
        printf("Error creating the vertices of the rectangle.\n");
        return false;
    }

    body->transformedVertices = (Vector2 *)SDL_malloc(4 * sizeof(Vector2));

    if (body->transformedVertices == NULL)
    {
        printf("Error creating the vertices of the rectangle.\n");
        return false;
    }

    body->vertices[0] = Vector2_New(position.x - (body->width/2), position.y - (body->height/2));
    body->vertices[1] = Vector2_New(position.x + (body->width/2), position.y - (body->height/2));
    body->vertices[2] = Vector2_New(position.x + (body->width/2), position.y + (body->height/2));
    body->vertices[3] = Vector2_New(position.x - (body->width/2), position.y + (body->height/2));
    
    body->transformedVertices[0] = body->vertices[0];
    body->transformedVertices[1] = body->vertices[1];
    body->transformedVertices[2] = body->vertices[2];
    body->transformedVertices[3] = body->vertices[3];
}

bool Body_CreateCircle(Body *body, Vector2 center, float radius, float density, 
                    float restitution, float rotation, bool isStatic)
{
    body->area = SDL_pow((PI * radius), 2) * 1;
    body->mass = body->area *density;

    body->density = density;
    body->position = center;

    body->rotation = rotation;
    body->restitution = restitution;

    body->force = Vector2_Zero();
    body->linerVelocity = Vector2_Zero();
    body->shapeType = Circle;
    body->width = 0;
    body->height = 0;
    body->radius = (6 * radius);

    body->isStatic = isStatic;
}

void Body_Move(Body *body, Vector2 velocity)
{
    body->position = Vector2_Add(body->position, velocity);

    if (body->shapeType == Box)
    {
        body->vertices[0] = Vector2_New(body->position.x - (body->width/2), body->position.y - (body->height/2));
        body->vertices[1] = Vector2_New(body->position.x + (body->width/2), body->position.y - (body->height/2));
        body->vertices[2] = Vector2_New(body->position.x + (body->width/2), body->position.y + (body->height/2));
        body->vertices[3] = Vector2_New(body->position.x - (body->width/2), body->position.y + (body->height/2));

        for (int i = 0; i < 4; ++i)
        {
            Body_Rotate(body, body->rotation);
        }
    }
}

void Body_Rotate(Body *body, float angle)
{
    body->rotation = angle;

    if (body->shapeType == Box)
    {
        for (int i = 0; i < 4; ++i)
        {
            body->transformedVertices[i] = Vector2_Subtr(body->vertices[i], body->position);

            body->transformedVertices[i] = Vector2_Transform(body->transformedVertices[i], Transform_New(Vector2_Zero(), angle));

            body->transformedVertices[i] = Vector2_Add(body->transformedVertices[i], body->position);
        }
    }
}

void Body_Debug(Body *body, Color color, Window *window)
{
    SDL_SetRenderDrawColor(window->renderer, color.r, color.g, color.b, color.a);

    switch (body->shapeType)
    {
        case Box:


            float minY = FLT_MAX, maxY = -FLT_MAX;

            for (int i = 0; i < 4; i++) {
                if (body->transformedVertices[i].y < minY) minY = body->transformedVertices[i].y;
                if (body->transformedVertices[i].y > maxY) maxY = body->transformedVertices[i].y;
            }

            // Preencher o polígono usando o algoritmo de scanline
            for (int y = (int)minY; y <= (int)maxY; y++) {
                // Encontrar os pontos de interseção com a linha y
                float intersections[4];
                int count = 0;

                for (int i = 0; i < 4; i++) {
                    int next = (i + 1) % 4;

                    if ((body->transformedVertices[i].y <= y && body->transformedVertices[next].y > y) || (body->transformedVertices[next].y <= y && body->transformedVertices[i].y > y)) {
                        float x = body->transformedVertices[i].x + (y - body->transformedVertices[i].y) * (body->transformedVertices[next].x - body->transformedVertices[i].x) / (body->transformedVertices[next].y - body->transformedVertices[i].y);
                        intersections[count++] = x;
                    }
                }

                // Ordenar os pontos de interseção
                for (int i = 0; i < count - 1; i++) {
                    for (int j = i + 1; j < count; j++) {
                        if (intersections[j] < intersections[i]) {
                            float temp = intersections[i];
                            intersections[i] = intersections[j];
                            intersections[j] = temp;
                        }
                    }
                }

                // Preencher a linha entre os pares de interseções
                for (int i = 0; i < count; i += 2) {
                    if (i + 1 < count) {
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
                                            body->position.x + x, 
                                            body->position.y + y);
                    }
                }
            }
        break;
    }

    SDL_SetRenderDrawColor(window->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}

bool Body_Destroy(Body *body)
{
    if (body->transformedVertices == NULL && body->vertices == NULL)
    {
        return false;
    } 

    if (body->vertices != NULL)
    {
        free(body->vertices);
        body->vertices = NULL;
    }

    if (body->transformedVertices != NULL)
    {
        free(body->transformedVertices);
        body->transformedVertices = NULL;
    }

    return true;
}

void Body_Step(Body *body, float time)
{
    body->linerVelocity = Vector2_Add(body->linerVelocity, Vector2_Mult(body->force, time));

    body->position = Vector2_Add(body->position, Vector2_Mult(body->linerVelocity, time));

    if (body->shapeType == Box)
    {
        body->vertices[0] = Vector2_New(body->position.x - (body->width/2), body->position.y - (body->height/2));
        body->vertices[1] = Vector2_New(body->position.x + (body->width/2), body->position.y - (body->height/2));
        body->vertices[2] = Vector2_New(body->position.x + (body->width/2), body->position.y + (body->height/2));
        body->vertices[3] = Vector2_New(body->position.x - (body->width/2), body->position.y + (body->height/2));

        for (int i = 0; i < 4; ++i)
        {
            Body_Rotate(body, body->rotation);
        }
    }

    body->rotation = body->rotation + body->rotationVelocity * time;
    body->force = Vector2_Zero();
}

void Body_AddForce(Body *body, Vector2 amount)
{
    body->force = amount;
}