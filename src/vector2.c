#include "types.h"
#include "shape.h"
#include <float.h>

Vector2 Vector2_New(float x, float y)
{
    Vector2 vector = {x, y};
    return vector;
}

Vector2 Vector2_Add(Vector2 a, Vector2 b)
{
    return Vector2_New(a.x + b.x, a.y + b.y);
}

Vector2 Vector2_Subtr(Vector2 a, Vector2 b)
{
    return Vector2_New(a.x - b.x, a.y - b.y);
}

Vector2 Vector2_Mult(Vector2 v, float n)
{
    return Vector2_New(v.x * n, v.y * n);
}

float Vector2_Dot(Vector2 a, Vector2 b)
{
    return a.x * b.x + a.y * b.y;
}

Vector2 Vector2_Normal(Vector2 v)
{
    return Vector2_New(-v.y, v.x);
}

void Vector2_Project(Point *points, int count, Vector2 axis, float *min, float *max)
{
    *min = FLT_MAX;
    *max = -FLT_MAX;

    for (int i = 0; i < count; ++i)
    {
        Vector2 current = Vector2_New(points[i].x, points[i].y);
        float proj = Vector2_Dot(current, axis);

        if (proj < *min) *min = proj;
        if (proj > *max) *max = proj;
    }
}