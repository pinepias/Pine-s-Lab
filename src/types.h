#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdint.h>

typedef uint32_t Uint32;
typedef uint16_t Uint16;
typedef uint8_t Uint8;

typedef float Vector2[2];
typedef Vector2 Transform[2];

void Vector2_Set(Vector2 *dest, float x, float y);
void Vector2_Setv(Vector2 *dest, Vector2 src);
void Vector2_SetZero(Vector2 *dest);

void Vector2_Add(Vector2 *dest, Vector2 a, Vector2 b);
void Vector2_Addl(Vector2 *dest, Vector2 src);

void Vector2_Sub(Vector2 *dest, Vector2 a, Vector2 b);
void Vector2_Subl(Vector2 *dest, Vector2 src);

void Vector2_Mult(Vector2 *dest, Vector2 a, float n);
void Vector2_Multl(Vector2 *dest, float n);

void Vector2_Div(Vector2 *dest, Vector2 a, float n);
void Vector2_Divl(Vector2 *dest, float n);

void Vector2_Normalized(Vector2 *dest, Vector2 src);
void Vector2_Normalizedl(Vector2 *result);

void Vector2_Normal(Vector2 *dest, Vector2 src);
void Vector2_Normall(Vector2 *result);

void Vector2_Projection(Vector2 *vertices, int length, Vector2 axis, float *min, float *max);
void Vector2_ProjectionCircle(Vector2 *center, float radius, Vector2 axis, float *min, float *max);

void Vector2_Transform(Vector2 *result, float x, float y, Transform t);
void Vector2_Transformv(Vector2 *result, Vector2 v, Transform t);
void Vector2_Transformvl(Vector2 *result, Transform t);

float Vector2_Length(Vector2 dest);
float Vector2_LengthSquared(Vector2 dest);

float Vector2_Dot(Vector2 a, Vector2 b);

void Transform_Set(Transform *transform, float x, float y, float angle);
void Transform_Setv(Transform *transform, Vector2 v, float angle);

#endif