#include "types.h"
#include "transform.h"
#include <float.h>
#include <SDL2/SDL.h>

Vector2 Vector2_New(float x, float y)
{
    Vector2 vector = {x, y};
    return vector;
}

Vector2 Vector2_Zero()
{
    return Vector2_New(0, 0);
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

float Vector2_Magnitude(Vector2 v)
{
    return SDL_sqrtf((v.x*v.x) + (v.y*v.y));
}

float Vector2_Distance(Vector2 a, Vector2 b)
{
    Vector2 distance = Vector2_Subtr(a, b); 
    return Vector2_Magnitude(distance);
}

Vector2 Vector2_Normalized(Vector2 v)
{
    if (Vector2_Magnitude(v) == 0)
    {
        return Vector2_New(0, 0);
    }

    return Vector2_New(v.x/Vector2_Magnitude(v), v.y/Vector2_Magnitude(v));
}

Vector2 Vector2_Rotate(Vector2 point, Vector2 origin, double rotation)
{
    float deltaX = point.x - origin.x;
    float deltaY = point.y - origin.y;

    float length = SDL_sqrtf(SDL_pow(deltaX, 2) + SDL_pow(deltaY, 2));
    float angle = SDL_atan2f(deltaY, deltaX);

    point.x = origin.x + (SDL_cosf(angle + rotation) * length);
    point.y = origin.y + (SDL_sinf(angle + rotation) * length);

    return point;
}

Vector2 Vector2_Transform(Vector2 v, Transform transform)
{
    float rx = v.x * transform.cos - v.y * transform.sin;
    float ry = v.x * transform.sin + v.y * transform.cos;

    return Vector2_New(transform.x + rx, transform.y + ry);
}

Vector2 Vector2_Abs(Vector2 v)
{
    return Vector2_New(SDL_fabsf(v.x), SDL_fabsf(v.y));
}

void Vector2_Projection(Vector2 *vertices, float length, Vector2 axis, float *min, float *max)
{
    *min = __FLT_MAX__;
    *max = -__FLT_MAX__;

    for (int i = 0; i < length; ++i)
    {
        float proj = Vector2_Dot(vertices[i], axis);
        if (proj < *min) *min = proj;
        if (proj > *max) *max = proj;
    }
}

void Vector2_ProjectionCircle(Vector2 center, float radius, Vector2 axis, float *min, float *max)
{
    Vector2 direction = Vector2_Normalized(axis);
    Vector2 radiusDir = Vector2_Mult(direction, radius);

    Vector2 left = Vector2_Subtr(center, radiusDir);
    Vector2 right = Vector2_Add(center, radiusDir);

    *min = Vector2_Dot(left, axis);
    *max = Vector2_Dot(right, axis);

    if (*min > *max)
    {
        float temp = *min;
        *min = *max;
        *max = temp;
    }
}

float clamp(float value, float min, float max)
{
    const float t = value < min ? min : value;
    return t > max ? max : t;
}