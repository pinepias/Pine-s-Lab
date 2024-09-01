#include <math.h>
#include "types.h"
#include <stdio.h>

void Vector2_Set(Vector2 *dest, float x, float y)
{
    (*dest)[0] = x;
    (*dest)[1] = y;
}

void Vector2_Setv(Vector2 *dest, Vector2 src)
{
    (*dest)[0] = src[0];
    (*dest)[1] = src[1];
}

void Vector2_SetZero(Vector2 *dest)
{
    (*dest)[0] = 0.0f;
    (*dest)[1] = 0.0f;
}

void Vector2_Add(Vector2 *dest, Vector2 a, Vector2 b)
{
    (*dest)[0] = a[0] + b[0];
    (*dest)[1] = a[1] + b[1];
}

void Vector2_Addl(Vector2 *dest, Vector2 src)
{
    (*dest)[0] = (*dest)[0] + src[0];
    (*dest)[1] = (*dest)[1] + src[1];
}

void Vector2_Sub(Vector2 *dest, Vector2 a, Vector2 b)
{
    (*dest)[0] = a[0] - b[0];
    (*dest)[1] = a[1] - b[1];
}

void Vector2_Subl(Vector2 *dest, Vector2 src)
{
    (*dest)[0] = (*dest)[0] - src[0];
    (*dest)[1] = (*dest)[1] - src[1];
}

void Vector2_Mult(Vector2 *dest, Vector2 a, float n)
{
    (*dest)[0] = a[0] * n;
    (*dest)[1] = a[1] * n;
}

void Vector2_Multl(Vector2 *dest, float n)
{
    (*dest)[0] = (*dest)[0] * n;
    (*dest)[1] = (*dest)[1] * n;
}

void Vector2_Div(Vector2 *dest, Vector2 a, float n)
{
    (*dest)[0] = a[0] / n;
    (*dest)[1] = a[1] / n;
}

void Vector2_Divl(Vector2 *dest, float n)
{
    (*dest)[0] = (*dest)[0] / n;
    (*dest)[1] = (*dest)[1] / n;
}

float Vector2_Length(Vector2 dest)
{
    return sqrtf(pow(dest[0], 2.0f) + pow(dest[1], 2.0f));
}

float Vector2_LengthSquared(Vector2 dest)
{
    return pow(dest[0], 2.0f) + pow(dest[1], 2.0f);
}

float Vector2_Dot(Vector2 a, Vector2 b)
{
    return a[0] * b[0] + a[1] * b[1];
}

void Vector2_Normalized(Vector2 *dest, Vector2 src)
{
    if (Vector2_LengthSquared(src) == 0)
    {
        Vector2_SetZero(dest);
        return;
    }

    float length = Vector2_Length(src);
    Vector2_Set(dest, src[0] / length, src[1] / length);
}

void Vector2_Normalizedl(Vector2 *result)
{
    if (Vector2_LengthSquared(*result) == 0)
    {
        Vector2_SetZero(result);
        return;
    }

    float length = Vector2_Length(*result);
    Vector2_Set(result, (*result)[0] / length, (*result)[1] / length);
}

void Vector2_Normall(Vector2 *result)
{
    (*result)[0] = -(*result)[1];
    (*result)[1] = (*result)[0];
}

void Vector2_Normal(Vector2 *dest, Vector2 src)
{
    (*dest)[0] = -src[1];
    (*dest)[1] = src[0];
}

void Vector2_Projection(Vector2 *vertices, int length, Vector2 axis, float *min, float *max)
{
    *min = __FLT_MAX__;
    *max = -__FLT_MAX__;

    for (int i = 0; i < length; ++i)
    {
        float proj = Vector2_Dot(vertices[i], axis);

        if (proj > *max) *max = proj;
        if (proj < *min) *min = proj;
    }
}

void Vector2_ProjectionCircle(Vector2 *center, float radius, Vector2 axis, float *min, float *max)
{
    Vector2 radiusDir;
    Vector2_Setv(&radiusDir, axis);
    Vector2_Multl(&radiusDir, radius);

    Vector2 leftDir;
    Vector2 rightDir;

    Vector2_Add(&rightDir, center, radiusDir);
    Vector2_Sub(&leftDir, center, radiusDir);

    *min = Vector2_Dot(rightDir, axis);
    *max = Vector2_Dot(leftDir, axis);

    if (*min > *max)
    {
        float temp = *max;
        *max = *min;
        *min = temp;
    }
}

void Vector2_Transform(Vector2 *result, float x, float y, Transform t)
{
    float rx = x * t[1][0] - y * t[1][1];
    float ry = x * t[1][1] + y * t[1][0];

    (*result)[0] = t[0][0] + rx;
    (*result)[1] = t[0][1] + rx;
}

void Vector2_Transformv(Vector2 *result, Vector2 v, Transform t)
{
    float rx = v[0] * t[1][0] - v[1] * t[1][1];
    float ry = v[0] * t[1][1] + v[1] * t[1][0];

    (*result)[0] = t[0][0] + rx;
    (*result)[1] = t[0][1] + ry;
}

void Vector2_Transformvl(Vector2 *result, Transform t)
{
    float rx = (*result)[0] * t[1][0] - (*result)[1] * t[1][1];
    float ry = (*result)[0] * t[1][1] + (*result)[1] * t[1][0];

    (*result)[0] = t[0][0] + rx;
    (*result)[1] = t[0][1] + ry;
}
