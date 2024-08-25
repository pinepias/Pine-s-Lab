#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

typedef struct Vector2              Vector2;
typedef struct Transform            Transform;

Transform Transform_New(Vector2 pos, float angle);

struct Transform
{
    float x, y;
    float cos, sin;
};


#endif