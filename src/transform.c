#include <math.h>
#include "types.h"

void Transform_Set(Transform *transform, float x, float y, float angle)
{
    (*transform)[0][0] = x;
    (*transform)[0][1] = y;
    (*transform)[1][0] = cosf(angle);
    (*transform)[1][1] = sinf(angle);
}

void Transform_Setv(Transform *transform, Vector2 v, float angle)
{
    (*transform)[0][0] = v[0];
    (*transform)[0][1] = v[1];
    (*transform)[1][0] = cosf(angle);
    (*transform)[1][1] = sinf(angle);
}