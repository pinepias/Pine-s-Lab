#ifndef _BODY_H_
#define _BODY_H_

#include "types.h"
#include <stdbool.h>

typedef struct Window                   Window;
typedef struct Color                    Color;
typedef struct World                    World;

typedef struct Body                     Body;
typedef struct BodyList                 BodyList;
typedef enum   ShapeType                ShapeType;

#define PI 3.14159265358979323846264338327950288

bool Body_NewBox(Body *body, Vector2 position, float width, float height, float mass, 
                float rotation, float resistituion, bool isStatic);

bool Body_NewCircle(Body *body, Vector2 center, float radius, float density, 
                float rotation, float resistituion, bool isStatic);

void Body_AddForce(Body *body, Vector2 amount);
void Body_Debug(Body *body, Window *window, Color color);
void Body_Step(Body *body, World *world, int interations, float time);
void Body_Move(Body *body, Vector2 amount);
void Body_UpdateBox(Body *box);

void Body_GetAABB(Body *body);

void Body_Destroy(Body *body);

void BodyList_Create(BodyList *list);
void BodyList_Push(BodyList *list, Body *body);
void BodyList_Remove(BodyList *list, int index);
void BodyList_Destroy(BodyList *list);

enum ShapeType 
{
    Box,
    Circle,
    Polygon
};

struct Body
{
    Vector2 force;
    Vector2 position;
    Vector2 linearVelocity;

    float resistituion;
    float density;
    float mass;
    float invMass;

    float width;
    float height;
    float radius;

    float rotation;
    float rotationVelocity;

    Vector2 *vertices;
    Vector2 *transformedVertices;
    int vertLength;

    AABB aabb;

    ShapeType shape;
    bool isStatic;
};

struct BodyList
{
    Body *bodies;
    int length;
};

#endif