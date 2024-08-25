#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdint.h>

#define PI 3.14159265358979323846

typedef uint32_t    Uint32;
typedef uint16_t    Uint16;
typedef uint8_t     Uint8;

typedef struct Vector2          Vector2;
typedef struct Transform        Transform;

Vector2 Vector2_New(float x, float y);
Vector2 Vector2_Zero();
Vector2 Vector2_Add(Vector2 a, Vector2 b);
Vector2 Vector2_Subtr(Vector2 a, Vector2 b);
Vector2 Vector2_Mult(Vector2 v, float n);
Vector2 Vector2_Normal(Vector2 v);
Vector2 Vector2_Normalized(Vector2 v);
Vector2 Vector2_Rotate(Vector2 point, Vector2 origin, double rotation);
Vector2 Vector2_Transform(Vector2 v, Transform transform);
Vector2 Vector2_Abs(Vector2 v);

float Vector2_Magnitude(Vector2 v);
float Vector2_Dot(Vector2 a, Vector2 b);
float Vector2_Distance(Vector2 a, Vector2 b);

void Vector2_Projection(Vector2 *vertices, float length, Vector2 axis, float *min, float *max);
void Vector2_ProjectionCircle(Vector2 center, float radius, Vector2 axis, float *min, float *max);

float clamp(float value, float min, float max);

struct Vector2
{
    float x, y;
};

#endif