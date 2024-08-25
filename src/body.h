#ifndef _BODY_H_
#define _BODY_H_

#include "types.h"
#include <stdbool.h>

typedef struct Color                    Color;
typedef struct Window                   Window;

typedef struct Body                     Body;
typedef enum   ShapeType                ShapeType;

bool Body_CreateBox(Body *body, Vector2 position, float width, float height, float density, 
                    float restitution, float rotation, bool isStatic);

bool Body_CreateCircle(Body *body, Vector2 center, float radius, float density, 
                    float restitution, float rotation, bool isStatic);

bool Body_Destroy(Body *body);

void Body_Move(Body *body, Vector2 velocity);
void Body_Rotate(Body *body, float angle);
void Body_AddForce(Body *body, Vector2 amount);

void Body_Step(Body *body, float time);
void Body_Debug(Body *body, Color color, Window *window);

enum ShapeType
{
    Box,
    Circle,
    Polygon
};

struct Body
{
    Vector2 position;
    Vector2 linerVelocity;
    float rotation;
    float rotationVelocity;

    Vector2 force;

    float density;
    float mass;
    float restitution;
    float area;

    bool isStatic;

    float radius;
    float width;
    float height;

    Vector2 *vertices;
    Vector2 *transformedVertices;

    ShapeType shapeType;
};

#endif