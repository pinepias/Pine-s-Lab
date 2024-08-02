#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdint.h>

typedef uint32_t Uint32;
typedef uint16_t Uint16;
typedef uint8_t Uint8;

typedef struct Vector2          Vector2;
typedef struct Point            Point;

Vector2 Vector2_New(float x, float y);
Vector2 Vector2_Add(Vector2 a, Vector2 b);
Vector2 Vector2_Subtr(Vector2 a, Vector2 b);
Vector2 Vector2_Mult(Vector2 v, float n);
Vector2 Vector2_Normal(Vector2 v);

void Vector2_Project(Point *points, int length, Vector2 axis, float *min, float *max, int offsetX, int offsetY);

float Vector2_Dot(Vector2 a, Vector2 b);

struct Vector2
{
    float x, y;
};

#endif