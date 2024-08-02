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

void Vector2_Project(Point *points, int length, Vector2 axis, float *min, float *max, int offsetX, int offsetY)
{
    *min = FLT_MAX;
    *max = -FLT_MAX;

    for (int i = 0; i < length; ++i)
    {
        Vector2 v = Vector2_New(points[i].x + offsetX, points[i].y + offsetY);
        float proj = Vector2_Dot(v, axis);

        if (proj < *min) { *min = proj; }
        if (proj > *max) { *max = proj; }
    }
}